#include <Player/Player.hpp>
#include <CC/core.h>
#include <Utilities/Input.hpp>
#include <Player/InGameMenu.hpp>
#include <Player/Inventory.hpp>
#include <CC/eventpackets.h>
#include <ELoop.hpp>

namespace CrossCraft
{

Player::Player()
	: camera(mathfu::Vector<float, 3>{ 0.0f, 0.0f, 0.0f },
		 mathfu::Vector<float, 3>{ 0.0f, 0.0f, 0.0f }, 70.0f,
		 16.0f / 9.0f, 0.2f,
#if PSP
		 256.0f)
{
#else
		 1024.0f)
{
#endif
	auto pd = CC_Player_GetData();

	position = mathfu::Vector<float, 3>{ 0, 0, 0 };
	velocity = mathfu::Vector<float, 3>{ 0, 0, 0 };
	rotation = mathfu::Vector<float, 2>{ 0, 0 };
	on_ground = pd->on_ground;
	in_water = false;
	water_face = false;

	horizInput = 0.0f;
	vertInput = 0.0f;

	hud = create_scopeptr<HUD>();
	tickTimer = 0.0f;

	mX = 0.0f;
	mY = 0.0f;
}

Player::~Player() = default;

auto Player::handle_health_update(std::any p) -> void
{
	auto update = std::any_cast<HealthUpdate>(p);
	update.player->health = update.health;
	if (update.player->health == 0) {
		InGameMenu::get().is_dead = true;
	}
}

const float BREAK_DISTANCE = 5.0f;
const float PLACE_DISTANCE = 4.0f;

auto path_trace(mathfu::Vector<float, 3> pos,
		const mathfu::Vector<float, 3> &step, float max,
		mathfu::Vector<float, 3> &output) -> bool
{
	pos.y += 1.625f; // This is a hack to make the ray trace work properly

	for (int i = 0; i < max * 10; i++) {
		pos += step;

		block_t out;
		bool res = CC_World_TryGetBlock(pos.x, pos.y, pos.z, &out);
		if (res && out != BLK_Air && out != BLK_Water) {
			output = pos;
			return true;
		}
	}

	return false;
}

auto get_rotation(const mathfu::Vector<float, 2> &rotation)
	-> mathfu::Vector<float, 3>
{
	auto rx =
		mathfu::Matrix<float, 3>::RotationX(rotation.x / 180.0f * M_PI);
	auto ry = mathfu::Matrix<float, 3>::RotationY((-rotation.y + 180.0f) /
						      180.0f * M_PI);

	mathfu::Vector<float, 3> view = mathfu::Vector<float, 3>(0, 0, 1);
	view = rx * view;
	view = ry * view;

	return view;
}

auto Player::break_block_up(const std::any &p) -> void
{
	BreakInformation::get().gBreaking = false;
	BreakInformation::get().gBreakingTimer = 0.0f;
	BreakInformation::get().gBreakingTotal = 0.0f;
}

auto Player::break_block(std::any p) -> void
{
	auto player = std::any_cast<Player *>(p);

	mathfu::Vector<float, 3> pos = player->position;
	mathfu::Vector<float, 3> step = get_rotation(player->rotation) * 0.1f;

	mathfu::Vector<float, 3> out;
	if (!path_trace(pos, step, BREAK_DISTANCE, out))
		return;

	if (!BreakInformation::get().gBreaking) {
		BreakInformation::get().gBreaking = true;
		BreakInformation::get().gBreakingTimer =
			1.0f; // TODO: Get Time To Break From Equipment + Block Hit
		BreakInformation::get().gBreakingTotal =
			1.0f; // TODO: Get Time To Break From Equipment + Block Hit
		BreakInformation::get().gBreakingPos =
			mathfu::Vector<int, 3>{ static_cast<int>(out.x),
						static_cast<int>(out.y),
						static_cast<int>(
							out.z) };
		auto p4 = CC_EventPacket_Create_PlayerDigging(
			out.x, out.y, out.z, 0, BREAK_START);
		CC_EventLoop_PushPacketOutbound(
			ELoop::get().client_event_loop, &p4);
		return;
	}

	if (!BreakInformation::get().gBreaking)
		return;

	if (BreakInformation::get().gBreakingPos.x !=
		    (int)out.x ||
	    BreakInformation::get().gBreakingPos.y !=
		    (int)out.y ||
	    BreakInformation::get().gBreakingPos.z !=
		    (int)out.z) {
		BreakInformation::get().gBreaking = false;
		BreakInformation::get().gBreakingTotal = 0.0f;
		auto p1 = CC_EventPacket_Create_PlayerDigging(
			out.x, out.y, out.z, 0, BREAK_ABORT);
		CC_EventLoop_PushPacketOutbound(
			ELoop::get().client_event_loop, &p1);
		return;
	}

	auto p2 = CC_EventPacket_Create_PlayerDigging(
		out.x, out.y, out.z, 0, BREAK_DIG);
		CC_EventLoop_PushPacketOutbound(
			ELoop::get().client_event_loop, &p2);

	if (BreakInformation::get().gBreakingTimer < 0.0f) {
		auto p3 = CC_EventPacket_Create_PlayerDigging(
			out.x, out.y, out.z, 0, BREAK_FINISH);
		CC_EventLoop_PushPacketOutbound(
			ELoop::get().client_event_loop, &p3);

		BreakInformation::get().gBreaking = false;
		BreakInformation::get().gBreakingTotal = 0.0f;
	}
}

bool Player::safety_check_place(
	const mathfu::Vector<int, 3> &updatePosition) const
{
	mathfu::Vector<int, 3> currentPosition =
		mathfu::Vector<int, 3>{ static_cast<int>(position.x),
					static_cast<int>(position.y),
					static_cast<int>(position.z) };
	mathfu::Vector<int, 3> currentPositionY =
		mathfu::Vector<int, 3>{ static_cast<int>(position.x),
					static_cast<int>(position.y + 1),
					static_cast<int>(position.z) };

	return currentPosition != updatePosition &&
	       currentPositionY != updatePosition;
}

auto Player::place_block(std::any p) -> void
{
	auto player = std::any_cast<Player *>(p);

	mathfu::Vector<float, 3> pos = player->position;
	mathfu::Vector<float, 3> step = get_rotation(player->rotation) * 0.1f;

	mathfu::Vector<float, 3> out;
	if (!path_trace(pos, step, PLACE_DISTANCE, out))
		return;

	out -= step;
	mathfu::Vector<int, 3> updatePosition =
		mathfu::Vector<int, 3>{ static_cast<int>(out.x),
					static_cast<int>(out.y),
					static_cast<int>(out.z) };

	if (!player->safety_check_place(updatePosition))
		return;

	// Okay let's check the player inventory for the block they want to place
	auto &itm = Inventory::get().get_selected();
	if (itm.item_id == 0 || itm.count == 0) {
		return;
	}
	auto id = itm.item_id;
	if (id >= 256) {
		return;
	}

	itm.count--;

	if (itm.count == 0) {
		itm.item_id = 0;
	}
	auto p1 = CC_EventPacket_Create_PlayerPlace(
		out.x, out.y, out.z, 0, id);
	CC_EventLoop_PushPacketOutbound(
		ELoop::get().client_event_loop, &p1);
}

void Player::tick()
{
	if (water_face) {
		air -= 1;
	} else {
		air = 300;
	}
}

void Player::compute_breaking(double dt)
{
	if (BreakInformation::get().gBreaking) {
		BreakInformation::get().gBreakingTimer -= dt;
	}

	mathfu::Vector<float, 3> pos = position;
	mathfu::Vector<float, 3> step = get_rotation(rotation) * 0.1f;

	mathfu::Vector<float, 3> out;
	if (path_trace(pos, step, BREAK_DISTANCE, out)) {
		BreakInformation::get().gSelectedPos =
			mathfu::Vector<int, 3>{ static_cast<int>(out.x),
						static_cast<int>(out.y),
						static_cast<int>(out.z) };
	} else {
		BreakInformation::get().gSelectedPos =
			mathfu::Vector<int, 3>{ -1, -1, -1 };
	}
}

void Player::update(double dt)
{
	// Handle input updates
	if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
		do_rotate(dt);
	}

	// Calculate velocity
	calculate_velocity(dt);

	// Test for collisions
	if (!freecam) {
		test_collide(dt);

		// Check the Player's status
		perform_checks();
	}

	// Move the player
	do_move(dt);

	// Compute breaking and selection
	compute_breaking(dt);
}

void Player::draw(double dt)
{
	// Draw the HUD
	hud->draw(this, dt);

	Inventory::get().draw_hotbar(dt);

	InGameMenu::get().draw();
}
}
