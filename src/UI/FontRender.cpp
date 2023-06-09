#include <UI/FontRender.hpp>
#include <ResourcePack.hpp>

#define CC_TEXT_COLOR_SELECT_FRONT \
    { 255, 255, 160, 255 }
#define CC_TEXT_COLOR_SELECT_BACK \
    { 63, 63, 40, 255 }
#define CC_TEXT_COLOR_SPLASH_FRONT \
    { 63, 63, 0, 255 }
#define CC_TEXT_COLOR_SPLASH_BACK \
    { 255, 255, 0, 255 }


namespace CrossCraft {

    FontRender::FontRender() {
        fontRenderer = create_scopeptr<Graphics::G2D::FontRenderer>(ResourcePack::get().get_texture("default"), Math::Vector2<float>(16, 16));
        fontRenderer->scale_factor = 1.0f;
    }

    FontRender::~FontRender() {

    }

    void FontRender::draw() {
        fontRenderer->draw();
    }

    void FontRender::get_color(uint8_t col, Rendering::Color& front, Rendering::Color& back, uint8_t alpha) {
        switch (col) {
            case CC_TEXT_COLOR_BLACK:
                front = {0, 0, 0, alpha};
                back = {0, 0, 0, alpha};
                break;
            case CC_TEXT_COLOR_DARK_BLUE:
                front = {0, 0, 170, alpha};
                back = {0, 0, 42, alpha};
                break;
            case CC_TEXT_COLOR_DARK_GREEN:
                front = {0, 170, 0, alpha};
                back = {0, 42, 0, alpha};
                break;
            case CC_TEXT_COLOR_DARK_AQUA:
                front = {0, 170, 170, alpha};
                back = {0, 42, 42, alpha};
                break;
            case CC_TEXT_COLOR_DARK_RED:
                front = {170, 0, 0, alpha};
                back = {42, 0, 0, alpha};
                break;
            case CC_TEXT_COLOR_DARK_PURPLE:
                front = {170, 0, 170, alpha};
                back = {42, 0, 42, alpha};
                break;
            case CC_TEXT_COLOR_GOLD:
                front = {255, 170, 0, alpha};
                back = {42, 42, 0, alpha};
                break;
            case CC_TEXT_COLOR_GRAY:
                front = {170, 170, 170, alpha};
                back = {42, 42, 42, alpha};
                break;
            case CC_TEXT_COLOR_DARK_GRAY:
                front = {85, 85, 85, alpha};
                back = {21, 21, 21, alpha};
                break;
            case CC_TEXT_COLOR_BLUE:
                front = {85, 85, 255, alpha};
                back = {21, 21, 63, alpha};
                break;
            case CC_TEXT_COLOR_GREEN:
                front = {85, 255, 85, alpha};
                back = {21, 63, 21, alpha};
                break;
            case CC_TEXT_COLOR_AQUA:
                front = {85, 255, 255, alpha};
                back = {21, 63, 63, alpha};
                break;
            case CC_TEXT_COLOR_RED:
                front = {255, 85, 85, alpha};
                back = {21, 63, 63, alpha};
                break;
            case CC_TEXT_COLOR_LIGHT_PURPLE:
                front = {255, 85, 255, alpha};
                back = {63, 21, 63, alpha};
                break;
            case CC_TEXT_COLOR_YELLOW:
                front = {255, 255, 85, alpha};
                back = {63, 63, 21, alpha};
                break;
            default:
            case CC_TEXT_COLOR_WHITE:
                front = {255, 255, 255, alpha};
                back = {63, 63, 63, alpha};
                break;
            case CC_TEXT_COLOR_BE_MTX_GOLD:
                front = {221, 214, 5, alpha};
                back = {55, 53, 1, alpha};
                break;
            case CC_TEXT_COLOR_SELECT:
                front = CC_TEXT_COLOR_SELECT_FRONT;
                back = CC_TEXT_COLOR_SELECT_BACK;
                break;
            case CC_TEXT_COLOR_SPLASH:
                front = CC_TEXT_COLOR_SPLASH_FRONT;
                back = CC_TEXT_COLOR_SPLASH_BACK;
                break;
        }
    }

    void FontRender::draw_text(uint8_t color, std::string text, Math::Vector2<float> pos, float layer) {
        Rendering::Color front, back;
        get_color(color, front, back);

        fontRenderer->add_text(text, pos, front, layer);
        fontRenderer->add_text(text, pos - Math::Vector2<float>{-1.0f, 1.0f}, back, layer - 1.0f);
    }


    void FontRender::draw_text_aligned(uint8_t color, uint8_t align, std::string text, Math::Vector2<float> pos, float layer) {
        Rendering::Color front, back;
        get_color(color, front, back);

        float width = fontRenderer->calculate_size(text);

        if(align == CC_TEXT_ALIGN_CENTER)
            pos.x -= width / 2.0f;
        else if(align == CC_TEXT_ALIGN_RIGHT)
            pos.x -= width;

        fontRenderer->add_text(text, pos, front, layer);
        fontRenderer->add_text(text, pos - Math::Vector2<float>{-1.0f, 1.0f}, back, layer - 1.0f);
    }

    void FontRender::clear() {
        fontRenderer->clear_tiles();
    }
    void FontRender::build() {
        fontRenderer->generate_map();
    }
}