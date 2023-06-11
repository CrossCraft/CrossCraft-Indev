#include <Player/Player.hpp>
#include <CC/core.h>
#include <Utilities/Input.hpp>
#include <Player/InGameMenu.hpp>

#include <GLFW/glfw3.h>
#include <Player/Inventory.hpp>

namespace GI {
    extern GLFWwindow *window;
}

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

    void Player::do_rotate() {
        float cX = Utilities::Input::get_axis("Mouse", "X") * 0.1; // TODO: Sensitivity
        float cY = Utilities::Input::get_axis("Mouse", "Y") * 0.1; // TODO: Sensitivity

        cX *= 960;
        cY *= 544;


        if(lastCX == cX && lastCY == cY) {
            return;
        }

        if(lastCX != cX || lastCY != cY) {
            lastCX = cX;
            lastCY = cY;
            Utilities::Input::set_cursor_center();
        }

        rotation.y += cX;
        rotation.x += cY;

        if (rotation.y > 360.0f) {
            rotation.y -= 360.0f;
        }

        if (rotation.y < 0.0f) {
            rotation.y += 360.0f;
        }

        if (rotation.x < -89.9f) {
            rotation.x = -89.9f;
        }

        if (rotation.x > 89.9f) {
            rotation.x = 89.9f;
        }
    }

    const double GRAVITY = 28.0f;

    void Player::calculate_velocity(double dt) {
        float inputLen = sqrtf(horizInput * horizInput + vertInput * vertInput);
        if (inputLen > 0.0f) {
            horizInput /= inputLen;
            vertInput /= inputLen;
        }

        const float moveSpeed = 3.417f;

        float yrad = rotation.y / 180.0f * M_PI;
        float movementX = horizInput * cosf(yrad) + vertInput * sinf(yrad);
        float movementZ = horizInput * sinf(yrad) - vertInput * cosf(yrad);

        velocity.x = movementX * moveSpeed;
        velocity.z = movementZ * moveSpeed;

        velocity.y -= GRAVITY * dt;
    }

    void Player::do_move(double dt) {
        if (in_water) {
            position.x += velocity.x * dt * 0.75f;
            position.y += velocity.y * dt * 0.33f;
            position.z += velocity.z * dt * 0.75f;
        } else {
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
            position.z += velocity.z * dt;
        }

        CC_Event_Push_PlayerUpdate(PLAYER_SELF, position.x, position.y + 1.625f, position.z, rotation.x, rotation.y,
                                   on_ground);
        camera.pos = -position;
        camera.pos.y -= 1.5f;
        camera.rot = mathfu::Vector<float, 3>{rotation.x, rotation.y, 0.0f};

        camera.update();
    }


    mathfu::Vector<float, 3> Player::size = mathfu::Vector<float, 3>{0.6f, 1.8f, 0.6f};

    bool test_block(const mathfu::Vector<int, 3>& pos) {
        block_t block;
        bool res = CC_World_TryGetBlock(pos.x, pos.y, pos.z, &block);

        return (block != BLK_Air && block != BLK_Water) || !res;
    }

    void Player::test_collide(double dt) {
        int worldX, worldY, worldZ;

        bool testX = false;
        bool testY = false;
        bool testZ = false;

        position.y += 1.8f;

        int xMin = static_cast<int>(position.x - size.x / 2.0f);
        int xMax = static_cast<int>(position.x + size.x / 2.0f);
        int yMin = static_cast<int>(position.y - 1.625f);
        int yMax = static_cast<int>(position.y);
        int zMin = static_cast<int>(position.z - size.z / 2.0f);
        int zMax = static_cast<int>(position.z + size.z / 2.0f);

        if (velocity.x < 0.0) {
            worldX = (int) (position.x - size.x / 2.0f + velocity.x * dt);
            testX = true;
        } else if (velocity.x > 0.0) {
            worldX = (int) (position.x + size.x / 2.0f + velocity.x * dt);
            testX = true;
        }

        if (velocity.y < 0.0) {
            worldY = (int) (position.y - size.y + velocity.y * dt);
            testY = true;
        } else if (velocity.y > 0.0) {
            worldY = (int) (position.y + velocity.y * dt);
            testY = true;
        }

        if (velocity.z < 0.0) {
            worldZ = (int) (position.z - size.z / 2.0f + velocity.z * dt);
            testZ = true;
        } else if (velocity.z > 0.0) {
            worldZ = (int) (position.z + size.z / 2.0f + velocity.z * dt);
            testZ = true;
        }

        if (testX) {
            for (int y = yMin; y <= yMax; y++) {
                for (int z = zMin; z <= zMax; z++) {
                    mathfu::Vector<int, 3> pos = mathfu::Vector<int, 3>{worldX, y, z};
                    if (test_block(pos)) {
                        velocity.x = 0.0f;
                        break;
                    }
                }
            }
        }

        if (testY) {
            for (int x = xMin; x <= xMax; x++) {
                for (int z = zMin; z <= zMax; z++) {
                    mathfu::Vector<int, 3> pos = mathfu::Vector<int, 3>{x, worldY, z};
                    if (test_block(pos)) {
                        velocity.y = 0.0f;
                        break;
                    }
                }
            }
        }

        if (testZ) {
            for (int y = yMin; y <= yMax; y++) {
                for (int x = xMin; x <= xMax; x++) {
                    mathfu::Vector<int, 3> pos = mathfu::Vector<int, 3>{x, y, worldZ};
                    if (test_block(pos)) {
                        velocity.z = 0.0f;
                        break;
                    }
                }
            }
        }

        position.y -= 1.8f;

    }

    void Player::perform_checks() {
        // Check on Ground
        mathfu::Vector<float, 3> testPos = position;
        testPos.y -= 0.1f;
        on_ground = test_block(mathfu::Vector<int, 3>{static_cast<int>(testPos.x), static_cast<int>(testPos.y),
                                                      static_cast<int>(testPos.z)});

        // Check in Water
        block_t out, out2, out3;
        bool res = CC_World_TryGetBlock(testPos.x, testPos.y - 0.125f, testPos.z, &out);
        bool res2 = CC_World_TryGetBlock(testPos.x, testPos.y - 0.375f, testPos.z, &out2);
        bool res3 = CC_World_TryGetBlock(testPos.x, testPos.y + 1.3f, testPos.z, &out3);
        if (res) {
            in_water = out == BLK_Water;

            water_face = res3 && out3 == BLK_Water;

            if (in_water && res2 && out2 == BLK_Water) {
                on_ground = true;
            }
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

        // TODO: Cleanup Break Information
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

        // TODO: Cleanup Tick (maybe move to a separate function)
        tickTimer += dt;
        if (tickTimer > 0.05f) {
            tickTimer = 0.0f;
            if (water_face) {
                air -= 1;
            } else {
                air = 300;
            }
        }
    }

    void Player::draw(double dt) {
        // Draw the HUD
        hud->draw(this, dt);

        Inventory::get().draw_hotbar(dt);

        InGameMenu::get().draw();
    }

    auto Player::move_forward(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
            player->vertInput += 1.0f;
        }
    }

    auto Player::move_backward(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
            player->vertInput -= 1.0f;
        }
    }

    auto Player::move_left(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
            player->horizInput -= 1.0f;
        }
    }


    auto Player::move_release(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
            player->vertInput = 0.0f;
            player->horizInput = 0.0f;
        }
    }
    auto Player::move_right(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if (!InGameMenu::get().is_open() && !Inventory::is_open()) {
            player->horizInput = 1.0f;
        }
    }

    auto Player::jump(std::any p) -> void {
        auto player = std::any_cast<Player *>(p);

        if(!InGameMenu::get().is_open() && !Inventory::is_open()) {
            if (player->on_ground) {
                player->velocity.y = 8.367f;
                player->on_ground = false;
            }
        }
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

}
