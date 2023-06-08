#pragma once
#include <Stardust-Celeste.hpp>
#include <World/World.hpp>
#include <Player/Player.hpp>
#include <UI/FontRender.hpp>

using namespace Stardust_Celeste;

namespace CrossCraft {
    class GameState : public Core::ApplicationState {
    public:
        void on_start() override;
        void on_update(Core::Application* app, double dt) override;
        void on_draw(Core::Application* app, double dt) override;
        void on_cleanup() override;

    private:
        RefPtr<Player> player;
        RefPtr<World> world;
        RefPtr<FontRender> font_render;
        Utilities::Controller *kb_controller;
        Utilities::Controller *mb_controller;
    };
}