#pragma once
#include <Stardust-Celeste.hpp>
#include <World/World.hpp>
#include <Player/Player.hpp>

namespace CrossCraft{
    using namespace Stardust_Celeste;

    class OptimizerState : public Core::ApplicationState {
    public:
        void on_start() override;

        void on_update(Core::Application *app, double dt) override;

        void on_draw(Core::Application *app, double dt) override;

        void on_cleanup() override;

    private:
    };
}