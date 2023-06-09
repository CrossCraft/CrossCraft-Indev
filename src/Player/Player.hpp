#pragma once
#include <Stardust-Celeste.hpp>
#include <Math/Math.hpp>
#include <World/World.hpp>
#include <Rendering/Camera.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Player/Hud.hpp>

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

        static auto break_block(std::any p) -> void;
        static auto place_block(std::any p) -> void;

        Math::Vector3<float> position;
        Math::Vector3<float> velocity;
        Math::Vector2<float> rotation;
    private:
        static Math::Vector3<float> size;

        float horizInput;
        float vertInput;

        bool on_ground;
        bool in_water;
        bool water_face;

        void do_rotate(double dt);
        void do_move(double dt);
        void calculate_velocity(double dt);
        void test_collide(double dt);
        void perform_checks();

        bool safety_check_place(Math::Vector3<int> position);

        Rendering::Camera camera;
        ScopePtr<Graphics::G2D::Sprite> water_sprite, crosshair;
        ScopePtr<HUD> hud;
    };

}