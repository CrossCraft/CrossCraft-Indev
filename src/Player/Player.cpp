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

        double cX = Utilities::Input::get_axis("Mouse", "X"); // TODO: Sensitivity
        double cY = Utilities::Input::get_axis("Mouse", "Y"); // TODO: Sensitivity

        int rx, ry;
        glfwGetWindowSize(GI::window, &rx, &ry);
        cX *= (double)rx * dt;
        cY *= (double)ry * dt;

        rotation.y += cX * rotSpeed;
        rotation.x += cY * rotSpeed;

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

    void Player::update(double dt) {

        // Handle input updates
        do_rotate(dt);

        float inputLen = sqrtf(horizInput * horizInput + vertInput * vertInput);
        if (inputLen > 0.0f) {
            horizInput /= inputLen;
            vertInput /= inputLen;
        }

        const float moveSpeed = 5.0f;

        float movementX = horizInput * cosf(Math::toRadians(rotation.y)) + vertInput * sinf(Math::toRadians(rotation.y));
        float movementZ = horizInput * sinf(Math::toRadians(rotation.y)) - vertInput * cosf(Math::toRadians(rotation.y));

        velocity.x = movementX * moveSpeed;
        velocity.z = movementZ * moveSpeed;

        position.x += velocity.x * dt;
        position.z += velocity.z * dt;

        horizInput = 0.0f;
        vertInput = 0.0f;

        //CC_Event_Push_PlayerUpdate(PLAYER_SELF, position.x, position.y, position.z, rotation.x, rotation.y, on_ground);
        camera.pos = position;
        camera.rot = Math::Vector3<float>{Math::toRadians(rotation.x), Math::toRadians(rotation.y), 0.0f};

        SC_APP_INFO("Position: {} {} {}", position.x, position.y, position.z);

        camera.update();
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

}
