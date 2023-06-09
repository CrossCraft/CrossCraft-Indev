#include <Player/Hud.hpp>

namespace CrossCraft {

        HUD::HUD() {
            font_render = create_refptr<FontRender>();
        }

        HUD::~HUD() {

        }

        void HUD::draw(mathfu::Vector<float, 3> pos, double dt) {
            timer += dt;
            fpsCount++;

            if (timer >= 1.0) {
                timer = 0.0;
                currentFPS = fpsCount;
                fpsCount = 0;
            }

            auto ipos = mathfu::Vector<float, 3>(pos.x, pos.y, pos.z);

            font_render->clear();
            font_render->draw_text_aligned(CC_TEXT_COLOR_YELLOW, CC_TEXT_ALIGN_LEFT, "POS: " + std::to_string(ipos.x) + " " + std::to_string(ipos.y) + " " + std::to_string(ipos.z), mathfu::Vector<float, 2>(0, 264), 3.0f);
            font_render->draw_text_aligned(CC_TEXT_COLOR_YELLOW, CC_TEXT_ALIGN_RIGHT, "FPS: " + std::to_string(currentFPS), mathfu::Vector<float, 2>(480, 264), 3.0f);
            font_render->build();
            font_render->draw();
        }
}