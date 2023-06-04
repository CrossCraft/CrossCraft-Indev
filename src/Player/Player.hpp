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

        void do_rotate(double dt);

        void update(double dt);
        void draw(double dt);

        static auto move_forward(std::any p) -> void;
        static auto move_backward(std::any p) -> void;
        static auto move_left(std::any p) -> void;
        static auto move_right(std::any p) -> void;

    private:
        Math::Vector3<float> position;
        Math::Vector3<float> velocity;
        Math::Vector2<float> rotation;

        float horizInput;
        float vertInput;

        bool on_ground;

        Rendering::Camera camera;
    };

}