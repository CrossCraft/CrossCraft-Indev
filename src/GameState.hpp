#pragma once

#include <Stardust-Celeste.hpp>
#include <World/World.hpp>
#include <Player/Player.hpp>

using namespace Stardust_Celeste;

namespace CrossCraft {
    class GameState : public Core::ApplicationState {
    public:
        void on_start() override;

        void on_update(Core::Application *app, double dt) override;

        void on_draw(Core::Application *app, double dt) override;

        void on_cleanup() override;

    private:
        static void setup_2d_rendering();

        static void setup_3d_rendering();

        static auto on_action_left(std::any p) -> void;

        static auto on_action_left_up(std::any p) -> void;

        static auto on_action_right(std::any p) -> void;

        RefPtr<Player> player;
        RefPtr<World> world;
        Utilities::Controller *kb_controller;
        Utilities::Controller *mb_controller;
    };
}