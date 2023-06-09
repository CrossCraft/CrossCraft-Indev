#pragma once
#include <UI/FontRender.hpp>

namespace CrossCraft{

    class HUD{
    public:
        HUD();
        ~HUD();

        void draw(mathfu::Vector<float, 3> pos, double dt);

    private:
        RefPtr<FontRender> font_render;

        double timer;
        int fpsCount, currentFPS;
    };
}