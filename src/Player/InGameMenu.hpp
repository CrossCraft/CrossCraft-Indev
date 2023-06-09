#pragma once
#include <Utilities/Singleton.hpp>
#include <Rendering/Texture.hpp>
#include <ResourcePack.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Rendering/Primitive/Rectangle.hpp>
#include <UI/FontRender.hpp>

namespace CrossCraft {
    using namespace Stardust_Celeste;

    class InGameMenu : public Singleton {
    public:
        InGameMenu();
        ~InGameMenu();

        void update(double dt);
        void draw(double dt);

        void toggle();

        inline bool is_open() const {
            return open;
        }

        static auto init() -> void;
        static auto get() -> InGameMenu&;

        static auto toggle_command(std::any p) -> void {
            get().toggle();
        }

        static auto left_click_command(std::any p) -> void;
        static auto right_click_command(std::any p) -> void;

    private:
        auto draw_button(mathfu::Vector<float, 2> mousePos, mathfu::Vector<float, 2> pos, std::string text, bool disabled) -> void;
        bool open;

        RefPtr<Graphics::G2D::Sprite> button_selected, button_unselected, button_disabled;
        RefPtr<Rendering::Primitive::Rectangle> background_rectangle;

        RefPtr<FontRender> font_render = nullptr;
    };
}