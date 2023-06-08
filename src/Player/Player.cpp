#include <Player/Player.hpp>
#include <CC/core.h>
#include <Utilities/Input.hpp>

#include <GLFW/glfw3.h>

namespace GI {
    extern GLFWwindow *window;
}

namespace CrossCraft {

    Player::Player() : camera(Math::Vector3<float>{0.0f, 0.0f, 0.0f}, Math::Vector3<float>{0.0f, 0.0f, 0.0f}, 70.0f, 16.0f / 9.0f, 0.1f, 1000.0f) {
        auto pd = CC_Player_GetData();

        position = Math::Vector3<float>{pd->x, pd->y, pd->z};
        velocity = Math::Vector3<float>{pd->vx, pd->vy, pd->vz};
        rotation = Math::Vector2<float>{pd->pitch, pd->yaw};
        on_ground = pd->on_ground;
    }

    Player::~Player() {

    }


    void Player::do_rotate(double dt) {
        const auto rotSpeed = 100.0f;

        float cX = Utilities::Input::get_axis("Mouse", "X") * 0.1; // TODO: Sensitivity
        float cY = Utilities::Input::get_axis("Mouse", "Y") * 0.1; // TODO: Sensitivity

        int rx = 0, ry = 0;
        glfwGetWindowSize(GI::window, &rx, &ry);
        cX *= rx;
        cY *= ry;

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

        Utilities::Input::set_cursor_center();
    }

    const double GRAVITY = 28.0f;
    void Player::calculate_velocity(double dt) {
        float inputLen = sqrtf(horizInput * horizInput + vertInput * vertInput);
        if (inputLen > 0.0f) {
            horizInput /= inputLen;
            vertInput /= inputLen;
        }

        const float moveSpeed = 3.417f;

        float movementX = horizInput * cosf(Math::toRadians(rotation.y)) + vertInput * sinf(Math::toRadians(rotation.y));
        float movementZ = horizInput * sinf(Math::toRadians(rotation.y)) - vertInput * cosf(Math::toRadians(rotation.y));

        velocity.x = movementX * moveSpeed;
        velocity.z = movementZ * moveSpeed;

        velocity.y -= GRAVITY * dt;
    }

    void Player::do_move(double dt) {
        if(in_water) {
            position.x += velocity.x * dt * 0.75f;
            position.y += velocity.y * dt * 0.33f;
            position.z += velocity.z * dt * 0.75f;
        } else {
            position.x += velocity.x * dt;
            position.y += velocity.y * dt;
            position.z += velocity.z * dt;
        }

        horizInput = 0.0f;
        vertInput = 0.0f;

        CC_Event_Push_PlayerUpdate(PLAYER_SELF, position.x, position.y + 1.625f, position.z, rotation.x, rotation.y, on_ground);
        camera.pos = position;
        camera.pos.y += 1.5f;
        camera.rot = Math::Vector3<float>{Math::toRadians(rotation.x), Math::toRadians(rotation.y), 0.0f};

        camera.update();
    }


    Math::Vector3<float> Player::size = Math::Vector3<float>{0.6f, 1.8f, 0.6f};

    bool test_block(Math::Vector3<int> pos) {
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

        if(velocity.x < 0.0) {
            worldX = (int) (position.x - size.x / 2.0f + velocity.x * dt);
            testX = true;
        } else if(velocity.x > 0.0) {
            worldX = (int) (position.x + size.x / 2.0f + velocity.x * dt);
            testX = true;
        }

        if(velocity.y < 0.0) {
            worldY = (int) (position.y - size.y + velocity.y * dt);
            testY = true;
        } else if(velocity.y > 0.0) {
            worldY = (int) (position.y + velocity.y * dt);
            testY = true;
        }

        if(velocity.z < 0.0) {
            worldZ = (int) (position.z - size.z / 2.0f + velocity.z * dt);
            testZ = true;
        } else if(velocity.z > 0.0) {
            worldZ = (int) (position.z + size.z / 2.0f + velocity.z * dt);
            testZ = true;
        }

        if(testX) {
            for(int y = yMin; y <= yMax; y++) {
                for(int z = zMin; z <= zMax; z++) {
                    Math::Vector3<int> pos = Math::Vector3<int>{worldX, y, z};
                    if(test_block(pos)) {
                        velocity.x = 0.0f;
                        break;
                    }
                }
            }
        }

        if(testY) {
            for(int x = xMin; x <= xMax; x++) {
                for(int z = zMin; z <= zMax; z++) {
                    Math::Vector3<int> pos = Math::Vector3<int>{x, worldY, z};
                    if(test_block(pos)) {
                        velocity.y = 0.0f;
                        break;
                    }
                }
            }
        }

        if(testZ) {
            for(int y = yMin; y <= yMax; y++) {
                for(int x = xMin; x <= xMax; x++) {
                    Math::Vector3<int> pos = Math::Vector3<int>{x, y, worldZ};
                    if(test_block(pos)) {
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
        Math::Vector3 testPos = position;
        testPos.y -= 0.1f;
        on_ground = test_block(Math::Vector3<int>{static_cast<int>(testPos.x), static_cast<int>(testPos.y), static_cast<int>(testPos.z)});

        // Check in Water
        block_t out, out2;
        bool res = CC_World_TryGetBlock(testPos.x, testPos.y - 0.125f, testPos.z, &out);
        bool res2 = CC_World_TryGetBlock(testPos.x, testPos.y - 0.375f, testPos.z, &out2);
        if(res) {
            in_water = out == BLK_Water;

            if(in_water && res2 && out2 == BLK_Water) {
                on_ground = true;
            }
        }
    }

    void Player::update(double dt) {
        // Handle input updates
        do_rotate(dt);

        // Calculate velocity
        calculate_velocity(dt);

        // Test for collisions
        test_collide(dt);

        // Check the Player's status
        perform_checks();

        // Move the player
        do_move(dt);
    }

    void Player::draw(double dt) {

    }

    auto Player::move_forward(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);
        player->vertInput += 1.0f;
    }
    auto Player::move_backward(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);
        player->vertInput -= 1.0f;
    }
    auto Player::move_left(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);
        player->horizInput -= 1.0f;
    }
    auto Player::move_right(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);
        player->horizInput = 1.0f;
    }
    auto Player::jump(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);

        if(player->on_ground) {
            player->velocity.y = 8.367f;
            player->on_ground = false;
        }
    }

    const float BREAK_DISTANCE = 5.0f;
    const float PLACE_DISTANCE = 4.0f;

    auto path_trace(Math::Vector3<float> pos, Math::Vector3<float> step, float max, Math::Vector3<float>& output) -> bool {
        pos.y += 1.625f; // TODO: This is a hack to make the ray trace work properly

        for(int i = 0; i < max * 10; i++) {
            pos += step;

            block_t out;
            bool res = CC_World_TryGetBlock(pos.x, pos.y, pos.z, &out);
            if(res && out != BLK_Air && out != BLK_Water) {
                output = pos;
                return true;
            }
        }

        return false;
    }

    auto get_rotation(Math::Vector2<float> rotation) -> Math::Vector3<float> {
        auto rx = Math::Matrix::Rotate(Math::toRadians(rotation.x), Math::Vector3<float>(1, 0, 0));
        auto ry = Math::Matrix::Rotate(Math::toRadians(-rotation.y + 180.0f), Math::Vector3<float>(0, 1, 0));

        Math::Vector3<float> view = Math::Vector3<float>(0, 0, 1);
        view = rx.mul(view);
        view = ry.mul(view);

        return view;
    }

    auto Player::break_block(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);

        Math::Vector3<float> pos = player->position;
        Math::Vector3<float> step = get_rotation(player->rotation);

        Math::Vector3<float> out;
        if(path_trace(pos, step, BREAK_DISTANCE, out)) {
            CC_Event_Push_SetBlock(out.x, out.y, out.z, SET_BLOCK_MODE_BREAK, 0);
        }
    }

    bool Player::safety_check_place(Math::Vector3<int> updatePosition) {
        Math::Vector3<int> currentPosition = Math::Vector3<int>{static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z)};
        Math::Vector3<int> currentPositionY = Math::Vector3<int>{static_cast<int>(position.x), static_cast<int>(position.y + 1), static_cast<int>(position.z)};

        return currentPosition != updatePosition && currentPositionY != updatePosition;
    }

    auto Player::place_block(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);

        Math::Vector3<float> pos = player->position;
        Math::Vector3<float> step = get_rotation(player->rotation);

        Math::Vector3<float> out;
        if(path_trace(pos, step, PLACE_DISTANCE, out)) {
            out -= step;
            Math::Vector3<int> updatePosition = Math::Vector3<int>{static_cast<int>(out.x), static_cast<int>(out.y), static_cast<int>(out.z)};

            if(player->safety_check_place(updatePosition)) {
                CC_Event_Push_SetBlock(out.x, out.y, out.z, SET_BLOCK_MODE_PLACE, 1); //TODO: PLAYER SELECTION
            }
        }
    }

}
