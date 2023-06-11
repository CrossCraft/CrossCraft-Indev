#pragma once

#include <Stardust-Celeste.hpp>
#include <World/World.hpp>
#include <Rendering/Camera.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Player/Hud.hpp>
#include <mathfu/vector.h>

namespace CrossCraft {

    using namespace Stardust_Celeste;

    class HUD;

    class Player;

    struct HealthUpdate {
        Player *player;
        int16_t health;
    };

    struct BreakInformation {
        // Break Timer Variables
        bool gBreaking = false;
        mathfu::Vector<int, 3> gBreakingPos = mathfu::Vector<int, 3>{0, 0, 0};
        float gBreakingTimer = 0.0f;
        float gBreakingTotal = 0.0f;

        mathfu::Vector<int, 3> gSelectedPos = mathfu::Vector<int, 3>{0, 0, 0};

        inline static auto get() -> BreakInformation & {
            static BreakInformation instance;
            return instance;
        }
    };

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

        static auto move_release(std::any p) -> void;

        static auto jump(std::any p) -> void;

        static auto break_block(std::any p) -> void;

        static auto break_block_up(const std::any& p) -> void;

        static auto place_block(std::any p) -> void;

        static auto handle_health_update(std::any p) -> void;

        mathfu::Vector<float, 3> position;
        mathfu::Vector<float, 3> velocity;
        mathfu::Vector<float, 2> rotation;
    private:
        friend class HUD;

        static mathfu::Vector<float, 3> size;
        float lastCX, lastCY;

        float tickTimer;

        float horizInput;
        float vertInput;

        bool on_ground;
        bool in_water;
        bool water_face;

        int16_t health{};
        int16_t air{};

        void do_rotate();

        void do_move(double dt);

        void calculate_velocity(double dt);

        void test_collide(double dt);

        void perform_checks();

        bool safety_check_place(const mathfu::Vector<int, 3>& position) const;

        Rendering::Camera camera;
        ScopePtr<HUD> hud;
    };

}