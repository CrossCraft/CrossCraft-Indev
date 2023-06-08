#pragma once
#include <Stardust-Celeste.hpp>
#include <Math/Math.hpp>
#include <World/World.hpp>
#include <Rendering/Camera.hpp>

namespace CrossCraft {

    using namespace Stardust_Celeste;
    using namespace Stardust_Celeste::Math;

    class Player {
    public:
        Player();
        ~Player();

        void update(double dt);
        void draw(double dt);

        static auto move_forward(std::any p) -> void;
        static auto move_backward(std::any p) -> void;
        static auto move_left(std::any p) -> void;
        static auto move_right(std::any p) -> void;

        static auto jump(std::any p) -> void;

    private:
        Math::Vector3<float> position;
        Math::Vector3<float> velocity;
        Math::Vector2<float> rotation;

        static Math::Vector3<float> size;

        float horizInput;
        float vertInput;

        bool on_ground;
        bool in_water;

        void do_rotate(double dt);
        void do_move(double dt);
        void calculate_velocity(double dt);
        void test_collide(double dt);
        void perform_checks();

        Rendering::Camera camera;
    };

}