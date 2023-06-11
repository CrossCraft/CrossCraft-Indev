#include <Player/InGameMenu.hpp>
#include <Utilities/Input.hpp>
#include <World/World.hpp>
#include <utility>
#include <GLFW/glfw3.h>
#include <Player/Inventory.hpp>

namespace GI {
    extern GLFWwindow *window;
}

namespace CrossCraft {
    using namespace Stardust_Celeste;
    using namespace Stardust_Celeste::Utilities;

    InGameMenu::InGameMenu() {
        open = false;

        //TODO: Blue color <70, 70, 96, 216>
        background_rectangle = create_refptr<Rendering::Primitive::Rectangle>(
                Rendering::Rectangle{{0,   0},
                                     {480, 272}},
                Rendering::Color{{70, 70, 96, 216}}, 10);

        death_overlay = create_refptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("death_overlay"),
                Rendering::Rectangle{{0,   0},
                                     {480, 272}});
        death_overlay->set_layer(10);
        //TODO: Calculate the correct values for the death overlay
        death_overlay->set_color({192, 144, 168, 192});

        font_render = create_refptr<FontRender>();

        button_disabled = create_refptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{{-100, -10},
                                     {200,  20}}, Rendering::Rectangle{{0.0f,            46.0f / 256.0f},
                                                                       {200.0f / 256.0f, 20.0f / 256.0f}});
        button_disabled->set_layer(11);

        button_unselected = create_refptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{{-100, -10},
                                     {200,  20}}, Rendering::Rectangle{{0.0f,            66.0f / 256.0f},
                                                                       {200.0f / 256.0f, 20.0f / 256.0f}});
        button_unselected->set_layer(11);

        button_selected = create_refptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{{-100, -10},
                                     {200,  20}}, Rendering::Rectangle{{0.0f,            86.0f / 256.0f},
                                                                       {200.0f / 256.0f, 20.0f / 256.0f}});
        button_selected->set_layer(11);

    }

    InGameMenu::~InGameMenu() = default;

    void InGameMenu::update() {

    }

    InGameMenu *internal_state;

    auto InGameMenu::init() -> void {
        internal_state = new InGameMenu();
    }

    auto InGameMenu::get() -> InGameMenu & {
        return *internal_state;
    }


    auto InGameMenu::left_click_command(std::any p) -> void {
        auto cX = Input::get_axis("Mouse", "X");
        auto cY = Input::get_axis("Mouse", "Y");
        auto mousePos = mathfu::Vector<float, 2>(cX, cY);

        mathfu::Vector<float, 2> windowSize(480, 272);
        auto mouseVec = mousePos * windowSize;
        mouseVec.y = windowSize.y - mouseVec.y;


        if(get().is_dead) {
            if (mouseVec.x >= 190 && mouseVec.x <= 290 && mouseVec.y >= 76 && mouseVec.y <= 96) {
                CC_Event_Push_PlayerRespawn();
                get().is_dead = false;


                Input::set_cursor_center();
                glfwSetInputMode(GI::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                Input::set_differential_mode("Mouse", true);
            } else {
                SC_APP_INFO("Unimplemented");
            }
        } else {
            if (mouseVec.x >= 190 && mouseVec.x <= 290 && mouseVec.y >= 130 && mouseVec.y <= 150) {
                get().toggle();
            } else if (mouseVec.x >= 190 && mouseVec.x <= 290 && mouseVec.y >= 106 && mouseVec.y <= 126) {
                SC_APP_INFO("Unimplemented");
            } else if (mouseVec.x >= 190 && mouseVec.x <= 290 && mouseVec.y >= 82 && mouseVec.y <= 102) {
                World::save(std::move(p));
            }
        }
    }

    auto InGameMenu::right_click_command() -> void {

    }

    auto InGameMenu::draw_button(const mathfu::Vector<float, 2>& mousePos, mathfu::Vector<float, 2> pos, const std::string& text,
                                 bool disabled) -> void {
        mathfu::Vector<float, 2> windowSize(480, 272);
        auto mouseVec = mousePos * windowSize;
        mouseVec.y = windowSize.y - mouseVec.y;

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(mathfu::Vector<float, 3>(pos.x, pos.y, 6));

        if (disabled) {
            button_disabled->draw();
            pos.y -= 4;
            font_render->draw_text_aligned(CC_TEXT_COLOR_GRAY, CC_TEXT_ALIGN_CENTER, text, pos, 20.0f);
        } else {
            if (mouseVec.x >= pos.x - 100 && mouseVec.x <= pos.x + 100 && mouseVec.y >= pos.y - 10 &&
                mouseVec.y <= pos.y + 10) {
                button_selected->draw();
                pos.y -= 4;
                font_render->draw_text_aligned(CC_TEXT_COLOR_SELECT, CC_TEXT_ALIGN_CENTER, text, pos, 20.0f);
            } else {
                button_unselected->draw();
                pos.y -= 4;
                font_render->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_CENTER, text, pos, 20.0f);
            }
        }

    }

    void InGameMenu::draw() {
        auto cX = Input::get_axis("Mouse", "X");
        auto cY = Input::get_axis("Mouse", "Y");
        auto mouseVec = mathfu::Vector<float, 2>(cX, cY);

        Rendering::RenderContext::get().matrix_clear();

        if(is_dead) {
            death_overlay->draw();

            font_render->clear();
            font_render->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_CENTER, "Game over!",
                                           mathfu::Vector<float, 2>(240, 192 - 24), 20.0f);
            font_render->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_CENTER, "Score: 0",
                                           mathfu::Vector<float, 2>(240, 168 - 24), 20.0f);

            draw_button(mouseVec, mathfu::Vector<float, 2>(240, 120 - 24), "Respawn", false);
            draw_button(mouseVec, mathfu::Vector<float, 2>(240, 96 - 24), "Title menu", false);

            font_render->build();
            Rendering::RenderContext::get().matrix_clear();
            font_render->draw();

            glfwSetInputMode(GI::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Input::set_differential_mode("Mouse", false);
        }

        if (!open)
            return;

        background_rectangle->draw();

        font_render->clear();
        font_render->draw_text_aligned(CC_TEXT_COLOR_WHITE, CC_TEXT_ALIGN_CENTER, "Game Menu",
                                       mathfu::Vector<float, 2>(240, 216), 20.0f);

        draw_button(mouseVec, mathfu::Vector<float, 2>(240, 140), "Back to Game", false);
        draw_button(mouseVec, mathfu::Vector<float, 2>(240, 116), "Options", true);
        draw_button(mouseVec, mathfu::Vector<float, 2>(240, 92), "Save Game", false);

        font_render->build();
        Rendering::RenderContext::get().matrix_clear();
        font_render->draw();
    }

    void InGameMenu::toggle() {
        if(Inventory::is_open())
            return;

        open = !open;

        if (open) {
            glfwSetInputMode(GI::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Input::set_differential_mode("Mouse", false);
        } else {
            Input::set_cursor_center();
            glfwSetInputMode(GI::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Input::set_differential_mode("Mouse", true);
        }
    }
}