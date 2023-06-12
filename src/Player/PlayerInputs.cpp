#include <Player/Player.hpp>
#include <GLFW/glfw3.h>
#include <Player/Inventory.hpp>
#include <Player/InGameMenu.hpp>

namespace CrossCraft {
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

}