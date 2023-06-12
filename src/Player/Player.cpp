#include <Player/Player.hpp>
#include <CC/core.h>
#include <Utilities/Input.hpp>
#include <Player/InGameMenu.hpp>
#include <Player/Inventory.hpp>

namespace CrossCraft {

    Player::Player() : camera(mathfu::Vector<float, 3>{0.0f, 0.0f, 0.0f}, mathfu::Vector<float, 3>{0.0f, 0.0f, 0.0f},
                              70.0f, 16.0f / 9.0f, 0.1f, 1000.0f) {
        auto pd = CC_Player_GetData();

        position = mathfu::Vector<float, 3>{pd->x, pd->y, pd->z};
        velocity = mathfu::Vector<float, 3>{pd->vx, pd->vy, pd->vz};
        rotation = mathfu::Vector<float, 2>{pd->pitch, pd->yaw};
        on_ground = pd->on_ground;
        in_water = false;
        water_face = false;

        horizInput = 0.0f;
        vertInput = 0.0f;

        hud = create_scopeptr<HUD>();
        tickTimer = 0.0f;
    }

    Player::~Player() = default;

    auto Player::handle_health_update(std::any p) -> void {
        auto update = std::any_cast<HealthUpdate>(p);
        update.player->health = update.health;
        if(update.player->health == 0) {
            InGameMenu::get().is_dead = true;
        }
    }

    const float BREAK_DISTANCE = 5.0f;
    const float PLACE_DISTANCE = 4.0f;

    auto path_trace(mathfu::Vector<float, 3> pos, const mathfu::Vector<float, 3>& step, float max,
                    mathfu::Vector<float, 3> &output) -> bool {
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

    auto get_rotation(const mathfu::Vector<float, 2>& rotation) -> mathfu::Vector<float, 3> {

        auto rx = mathfu::Matrix<float, 3>::RotationX(rotation.x / 180.0f * M_PI);
        auto ry = mathfu::Matrix<float, 3>::RotationY((-rotation.y + 180.0f) / 180.0f * M_PI);

        mathfu::Vector<float, 3> view = mathfu::Vector<float, 3>(0, 0, 1);
        view = rx * view;
        view = ry * view;

        return view;
    }


    auto Player::break_block_up(const std::any& p) -> void {
        BreakInformation::get().gBreaking = false;
        BreakInformation::get().gBreakingTimer = 0.0f;
        BreakInformation::get().gBreakingTotal = 0.0f;
    }

    auto Player::break_block(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        mathfu::Vector<float, 3> pos = player->position;
        mathfu::Vector<float, 3> step = get_rotation(player->rotation) * 0.1f;

        mathfu::Vector<float, 3> out;
        if (path_trace(pos, step, BREAK_DISTANCE, out)) {

            if (!BreakInformation::get().gBreaking) {
                BreakInformation::get().gBreaking = true;
                BreakInformation::get().gBreakingTimer = 1.0f; // TODO: Get Time To Break From Equipment + Block Hit
                BreakInformation::get().gBreakingTotal = 1.0f; // TODO: Get Time To Break From Equipment + Block Hit
                BreakInformation::get().gBreakingPos = mathfu::Vector<int, 3>{static_cast<int>(out.x),
                                                                              static_cast<int>(out.y),
                                                                              static_cast<int>(out.z)};
            }

            if (BreakInformation::get().gBreaking) {
                if (BreakInformation::get().gBreakingPos.x != (int) out.x ||
                    BreakInformation::get().gBreakingPos.y != (int) out.y ||
                    BreakInformation::get().gBreakingPos.z != (int) out.z) {
                    BreakInformation::get().gBreaking = false;
                    BreakInformation::get().gBreakingTotal = 0.0f;
                }

                if (BreakInformation::get().gBreakingTimer < 0.0f) {
                    CC_Event_Push_SetBlock(out.x, out.y, out.z, SET_BLOCK_MODE_BREAK, 0);
                    BreakInformation::get().gBreaking = false;
                    BreakInformation::get().gBreakingTotal = 0.0f;
                }
            }
        }
    }

    bool Player::safety_check_place(const mathfu::Vector<int, 3>& updatePosition) const {
        mathfu::Vector<int, 3> currentPosition = mathfu::Vector<int, 3>{static_cast<int>(position.x),
                                                                        static_cast<int>(position.y),
                                                                        static_cast<int>(position.z)};
        mathfu::Vector<int, 3> currentPositionY = mathfu::Vector<int, 3>{static_cast<int>(position.x),
                                                                         static_cast<int>(position.y + 1),
                                                                         static_cast<int>(position.z)};

        return currentPosition != updatePosition && currentPositionY != updatePosition;
    }

    auto Player::place_block(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        mathfu::Vector<float, 3> pos = player->position;
        mathfu::Vector<float, 3> step = get_rotation(player->rotation) * 0.1f;

        mathfu::Vector<float, 3> out;
        if (path_trace(pos, step, PLACE_DISTANCE, out)) {
            out -= step;
            mathfu::Vector<int, 3> updatePosition = mathfu::Vector<int, 3>{static_cast<int>(out.x),
                                                                           static_cast<int>(out.y),
                                                                           static_cast<int>(out.z)};

            if (player->safety_check_place(updatePosition)) {

                // Okay let's check the player inventory for the block they want to place
                auto &itm = Inventory::get().get_selected();
                if (itm.id == 0 || itm.count == 0) {
                    return;
                }
                auto id = itm.id;
                if (id >= 256) {
                    //DON'T PLACE ITEMS!
                    return;
                }

                itm.count--;

                if (itm.count == 0) {
                    itm.id = 0;
                }
                CC_Event_Push_SetBlock(out.x, out.y, out.z, SET_BLOCK_MODE_PLACE, id);
            }
        }
    }

    void Player::tick() {
        if (water_face) {
            air -= 1;
        } else {
            air = 300;
        }
    }

    void Player::compute_breaking(double dt) {
        if (BreakInformation::get().gBreaking) {
            BreakInformation::get().gBreakingTimer -= dt;
        }

        mathfu::Vector<float, 3> pos = position;
        mathfu::Vector<float, 3> step = get_rotation(rotation) * 0.1f;

        mathfu::Vector<float, 3> out;
        if (path_trace(pos, step, BREAK_DISTANCE, out)) {
            BreakInformation::get().gSelectedPos = mathfu::Vector<int, 3>{static_cast<int>(out.x),
                                                                          static_cast<int>(out.y),
                                                                          static_cast<int>(out.z)};
        } else {
            BreakInformation::get().gSelectedPos = mathfu::Vector<int, 3>{-1, -1, -1};
        }
    }

    void Player::update(double dt) {
        // Handle input updates
        if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
            do_rotate();
        }

        // Calculate velocity
        calculate_velocity(dt);

        // Test for collisions
        test_collide(dt);

        // Check the Player's status
        perform_checks();

        // Move the player
        do_move(dt);

        // Compute breaking and selection
        compute_breaking(dt);
    }

    void Player::draw(double dt) {
        // Draw the HUD
        hud->draw(this, dt);

        Inventory::get().draw_hotbar(dt);

        InGameMenu::get().draw();
    }
}
