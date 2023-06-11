#include <Player/Hud.hpp>
#include <ResourcePack.hpp>

namespace CrossCraft {

    HUD::HUD() {
        font_render = create_refptr<FontRender>();
        fpsCount = 0;

        water_sprite = create_scopeptr<Graphics::G2D::Sprite>(ResourcePack::get().get_texture("water_overlay"),
                                                              Rendering::Rectangle{{0.0f,   0.0f},
                                                                                   {480.0f, 272.0f}});
        water_sprite->set_layer(-5);

        crosshair = create_scopeptr<Graphics::G2D::Sprite>(ResourcePack::get().get_texture("gui_common"),
                                                           Rendering::Rectangle{{232.0f, 128.0f},
                                                                                {16.0f,  16.0f}},
                                                           Rendering::Rectangle{{15.0f / 16.0f, 0.0f},
                                                                                {1.0f / 16.0f,  1.0f / 16.0f}});
        crosshair->set_layer(2);

        auto icons_texture = ResourcePack::get().get_texture("icons");
        heartBG = create_scopeptr<Graphics::G2D::Sprite>(
                icons_texture, Rendering::Rectangle{{148, 25},
                                                    {9,   9}},
                Rendering::Rectangle{{16.0f / 256.0f, 0.0f},
                                     {9.0f / 256.0f,  9.0f / 256.0f}});
        heartBG->set_layer(1);

        heartFull = create_scopeptr<Graphics::G2D::Sprite>(
                icons_texture, Rendering::Rectangle{{149, 27},
                                                    {7,   7}},
                Rendering::Rectangle{{53.0f / 256.0f, 0.0f / 256.0f},
                                     {7.0f / 256.0f,  7.0f / 256.0f}});
        heartFull->set_layer(2);

        bubble = create_scopeptr<Graphics::G2D::Sprite>(
                icons_texture, Rendering::Rectangle{{148, 38},
                                                    {9,   8}},
                Rendering::Rectangle{{16.0f / 256.0f, 18.0f / 256.0f},
                                     {9.0f / 256.0f,  8.0f / 256.0f}});
        bubble->set_layer(-2);

        bubbleHalf = create_scopeptr<Graphics::G2D::Sprite>(
                icons_texture, Rendering::Rectangle{{149, 38},
                                                    {9,   8}},
                Rendering::Rectangle{
                        {(16.0f + 9.0f) / 256.0f, 18.0f / 256.0f},
                        {9.0f / 256.0f,           8.0f / 256.0f}});
        bubbleHalf->set_layer(-2);

        heartHalf = create_scopeptr<Graphics::G2D::Sprite>(
                icons_texture, Rendering::Rectangle{{147, 27},
                                                    {7,   7}},
                Rendering::Rectangle{{60.0f / 256.0f, 0.0f / 256.0f},
                                     {7.0f / 256.0f,  7.0f / 256.0f}});
        heartHalf->set_layer(2);
    }

    HUD::~HUD() = default;

    void HUD::draw(Player *player, double dt) {
        heartTimer += dt;
        timer += dt;
        fpsCount++;

        if (timer >= 1.0) {
            timer = 0.0;
            currentFPS = fpsCount;
            fpsCount = 0;
        }

        if (player->water_face) {
            water_sprite->draw();
        }

        crosshair->draw();


        Rendering::RenderContext::get().matrix_clear();
        if (player->air < 300) {
            for (int i = 0; i < 10; i++) {
                if (player->air - i * 30 > 0) {
                    auto diff = player->air - i * 30;
                    if (diff > 15 || diff == 0) {
                        bubble->draw();
                    } else {
                        bubbleHalf->draw();
                    }
                }
                Rendering::RenderContext::get().matrix_translate(
                        {9.0f, 0.0f, 0.0f});
            }
        }

        int off = 0;
        Rendering::RenderContext::get().matrix_clear();

        if (player->health <= 5) {
            Rendering::RenderContext::get().matrix_translate(
                    {(float) off, sinf(heartTimer * 25), 0.0f});
        } else {
            Rendering::RenderContext::get().matrix_translate({(float) off, 0.0f, 0.0f});
        }

        for (int i = 0; i < 10; i++) {
            heartBG->draw();
            if (player->health - i * 2 > 0) {
                auto diff = player->health - i * 2;
                if (diff > 1 || diff == 0) {
                    heartFull->draw();
                } else {
                    heartHalf->draw();
                }
            }

            off += 9;

            Rendering::RenderContext::get().matrix_clear();
            if (player->health <= 5) {
                Rendering::RenderContext::get().matrix_translate(
                        {(float) off, sinf(i + heartTimer * 25), 0.0f});
            } else {
                Rendering::RenderContext::get().matrix_translate(
                        {(float) off, 0.0f, 0.0f});
            }
        }

        Rendering::RenderContext::get().matrix_clear();
        auto ipos = mathfu::Vector<int, 3>(player->position.x, player->position.y, player->position.z);

        font_render->clear();
        font_render->draw_text_aligned(CC_TEXT_COLOR_YELLOW, CC_TEXT_ALIGN_LEFT,
                                       "POS: " + std::to_string(ipos.x) + " " + std::to_string(ipos.y) + " " +
                                       std::to_string(ipos.z), mathfu::Vector<float, 2>(0, 264), 3.0f);
        font_render->draw_text_aligned(CC_TEXT_COLOR_YELLOW, CC_TEXT_ALIGN_RIGHT, "FPS: " + std::to_string(currentFPS),
                                       mathfu::Vector<float, 2>(480, 264), 3.0f);
        font_render->build();
        font_render->draw();
    }
}