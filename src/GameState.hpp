#pragma once
#include <Stardust-Celeste.hpp>
#include <World/World.hpp>
#include <Player/Player.hpp>

using namespace Stardust_Celeste;

namespace CrossCraft {
class GameState : public Core::ApplicationState {
      public:
        void on_start();
        void on_update(Core::Application* app, double dt) override;
        void on_draw(Core::Application* app, double dt) override;
        void on_cleanup();

      private:
        RefPtr<Player> player;
        RefPtr<World> world;
        Utilities::Controller *kb_controller;
        uint32_t terrainTexID;
    };
}