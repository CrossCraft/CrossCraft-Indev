#include <GameState.hpp>
#include <CC/core.h>

#include <ResourcePack.hpp>
#include <Player/InGameMenu.hpp>
#include <Player/Inventory.hpp>
#include <ModelRenderer.hpp>
#include <Entity/EntityManager.hpp>
#include <CC/eventloop.h>
#include <CC/alphaindev.pb-c.h>
#include <CC/eventpackets.h>
#include <Chunk/ChunkMeta.hpp>
#include <ELoop.hpp>
#include "World/WorldTime.hpp"

#if PSP
#include <pspsdk.h>
#endif

namespace CrossCraft
{

bool gLoggedIn = false;

auto GameState::on_action_left_up(std::any p) -> void
{
	auto player = std::any_cast<Player *>(p);

	Player::break_block_up(player);
}

auto GameState::on_action_left(std::any p) -> void
{
	auto player = std::any_cast<Player *>(p);

	if (InGameMenu::get().is_open() || Inventory::is_open()) {
		InGameMenu::left_click_command(p);
	} else {
		Player::break_block(player);
	}
}

auto GameState::on_action_right(std::any p) -> void
{
	auto player = std::any_cast<Player *>(p);

	if (InGameMenu::get().is_open() || Inventory::is_open()) {
		InGameMenu::right_click_command();
	} else {
		Player::place_block(player);
	}
}

void GameState::on_start()
{
	ResourcePack::get().add_pack("resourcepacks/default.zip");
	ResourcePack::get().load();
	ModelRenderer::init();

	// Initialize the core library.
	CC_Core_Init();

	ChunkMeta::get().generate();

	// Create the server-side event loop for SP
	auto event_loop = CC_EventLoop_Init();

	// Create the client-side event loop for SP
	ELoop::get().client_event_loop = CC_EventLoop_Init();

	// Create the server-side data bus for SP
	auto server_bus = SharedDataBus_Init();

	// Create che client-side data bus for SP
	auto data_bus = SharedDataBus_Init();

	// Set the inbound bus for the event loop.
	CC_EventLoop_SetInboundBus(event_loop, &data_bus->bus, data_bus);
	CC_EventLoop_SetOutboundBus(event_loop, &server_bus->bus, server_bus);

	// Set the inbound bus for the client event loop.
	CC_EventLoop_SetInboundBus(ELoop::get().client_event_loop,
				   &server_bus->bus, server_bus);
	CC_EventLoop_SetOutboundBus(ELoop::get().client_event_loop,
				    &data_bus->bus, data_bus);

	// Set the server event loop for the core library.
	CC_Core_SetEventLoop(event_loop);

	// Send the handshake to the server.
	EventPacket handshake_packet =
		CC_EventPacket_Create_Handshake("IridescentRose", false);
	CC_EventLoop_PushPacketOutbound(ELoop::get().client_event_loop,
					&handshake_packet);
	CC_EventLoop_Update(ELoop::get().client_event_loop);

	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop, CC_PACKET_TYPE_HANDSHAKE,
		[](void *loop, EventPacket *packet) {
			auto handshake =
				packet->data.handshake_sc.response.data;
			if (handshake[0] == '-') {
				SC_APP_INFO(
					"Handshake successful! Logging in!");
				auto p = CC_EventPacket_Create_LoginClient(
					"IridescentRose", "password");
				CC_EventLoop_PushPacketOutbound(
					(EventLoop *)loop, &p);
			} else {
				throw std::runtime_error("Handshake failed!");
			}
		});

	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop, CC_PACKET_TYPE_LOGIN,
		[](void *loop, EventPacket *packet) { gLoggedIn = true; });
	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop, CC_PACKET_TYPE_SPAWN_POSITION,
		[](void *loop, EventPacket *packet) {
			auto spawn = packet->data.spawn_position;
			Player::get().position = { static_cast<float>(spawn.x),
						   static_cast<float>(spawn.y),
						   static_cast<float>(
							   spawn.z) };
		});

	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop,
		CC_PACKET_TYPE_PLAYER_POSITION_AND_LOOK,
		[](void *loop, EventPacket *packet) {
			auto p = packet->data.player_position_and_look_sc;
			Player::get().position = { static_cast<float>(p.x),
						   static_cast<float>(p.y),
						   static_cast<float>(p.z) };
			Player::get().rotation = { static_cast<float>(p.yaw),
						   static_cast<float>(
							   p.pitch) };
		});
	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop, CC_PACKET_TYPE_TIME_UPDATE,
		[](void* loop, EventPacket *packet) {
			WorldTime::get().tickTime = packet->data.time_update.time;
		});
	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop, CC_PACKET_TYPE_UPDATE_HEALTH,
		[](void *loop, EventPacket *packet) {
			Player::handle_health_update(HealthUpdate{
				&Player::get(),
				packet->data.update_health.health });
		});
	CC_EventLoop_RegisterHandler(
		ELoop::get().client_event_loop, CC_PACKET_TYPE_BLOCK_CHANGE,
		[](void *loop, EventPacket *packet) {
			World::get().handle_block_update(
				packet->data.block_change_sc.x,
				packet->data.block_change_sc.y,
				packet->data.block_change_sc.z);
		});

	while (!gLoggedIn) {
		CC_Core_Update(1);
		CC_EventLoop_Update(ELoop::get().client_event_loop);
	}
	InGameMenu::init();

	setup_input();

	SC_APP_INFO("Game State Initialized");
	Rendering::RenderContext::get().set_mode_3D();

	sky = create_refptr<Sky>();
}

void GameState::check_poll_input(double dt)
{
	poll_time += dt;
	if (poll_time > (1.0f / 240.0f)) {
		Utilities::Input::update();
		poll_time = 0.0f;
		Inventory::get().update();
	}
}

auto tick_count = 0;
void GameState::tick(double dt)
{
	tick_time += dt;

	if (tick_time > (1.0f / 20.0f)) {
		tick_time = 0.0f;
		tick_count++;
		Player::get().tick();

		auto packet = CC_EventPacket_Create_PlayerPositionAndLookClient(
			Player::get().position.x, Player::get().position.y,
			Player::get().position.z, Player::get().rotation.x,
			Player::get().rotation.y, Player::get().on_ground);
		CC_EventLoop_PushPacketOutbound(ELoop::get().client_event_loop,
						&packet);
		CC_EventLoop_Update(ELoop::get().client_event_loop);
	}
}

void GameState::on_update(Core::Application *app, double dt)
{
	check_poll_input(dt);
	tick(dt);

	Player::get().update(dt);
	World::get().update(dt);
	EntityManager::get().update(&Player::get(), dt);

	CC_Core_Update(dt);

	handle_events(dt);
}

void GameState::handle_events(double dt)
{
	CC_Event *event;
	while ((event = CC_Event_Poll()) != nullptr) {
		switch (event->type) {

		case CC_EVENT_SPAWN_ITEM: {
			World::get().handle_spawn_item(
				event->data.spawn_item.eid,
				event->data.spawn_item.x,
				event->data.spawn_item.y,
				event->data.spawn_item.z,
				event->data.spawn_item.vx,
				event->data.spawn_item.vy,
				event->data.spawn_item.vz,
				event->data.spawn_item.item);
			break;
		}

		case CC_EVENT_DESTROY_ENTITY: {
			EntityManager::get().remove(
				event->data.destroy_entity.eid);
			break;
		}

		default:
			SC_APP_INFO("Unhandled event type: {}", event->type);
			break;
		}
	}
}

void GameState::setup_2d_rendering()
{
	Rendering::RenderContext::get().set_mode_2D();
	Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -200, 200);
	GI::clearDepth();
	GI::disable(GI_CULL_FACE);
}

void GameState::setup_3d_rendering()
{
	GI::enable(GI_CULL_FACE);
	Rendering::RenderContext::get().set_mode_3D();
}

float x = 0;

void GameState::on_draw(Core::Application *app, double dt)
{
	sky->draw(dt, Player::get().position, tick_count);

	World::get().draw();

	Inventory::get().draw_block_hand(Player::get().position,
					 Player::get().rotation, dt);

	setup_2d_rendering();

	Player::get().draw(dt);
	Inventory::get().draw(dt);

	setup_3d_rendering();
}

void GameState::on_cleanup()
{
	CC_Core_Term();
	CC_EventLoop_Destroy(ELoop::get().client_event_loop);
}

} // namespace CrossCraft