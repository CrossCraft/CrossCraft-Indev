#pragma once

#include <Stardust-Celeste.hpp>
#include <World/World.hpp>
#include <Player/Player.hpp>
#include <World/Sky.hpp>

using namespace Stardust_Celeste;

namespace CrossCraft {
    class GameState : public Core::ApplicationState {
    public:
        void on_start() override;

        void on_update(Core::Application *app, double dt) override;

        void on_draw(Core::Application *app, double dt) override;

        void on_cleanup() override;

    private:
        void tick(double dt);
        void check_poll_input(double dt);
        void handle_events(double dt);
        static void setup_2d_rendering();

        void setup_input();

        static void setup_3d_rendering();

        static auto on_action_left(std::any p) -> void;

        static auto on_action_left_up(std::any p) -> void;

        static auto on_action_right(std::any p) -> void;

        RefPtr<Sky> sky;
        Utilities::Controller *kb_controller;
        Utilities::Controller *mb_controller;
        Utilities::Controller *psp_controller;
        double poll_time = 10.0f;
        double tick_time = 10.0f;
    };
}