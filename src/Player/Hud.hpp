#pragma once

#include <UI/FontRender.hpp>
#include <Player/Player.hpp>

namespace CrossCraft {

    class Player;

    class HUD {
    public:
        HUD();

        ~HUD();

        void draw(Player *player, double dt);

    private:
        RefPtr<FontRender> font_render;

        ScopePtr<Graphics::G2D::Sprite> heartBG, heartHalf, heartFull;
        ScopePtr<Graphics::G2D::Sprite> bubble, bubbleHalf;
        ScopePtr<Graphics::G2D::Sprite> water_sprite, crosshair;

        double timer{};
        double heartTimer{};
        int fpsCount, currentFPS{};
    };
}