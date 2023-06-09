#include <Player/Inventory.hpp>

namespace CrossCraft {

    Inventory::Inventory() {
        hotbar = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{ {0.0f, 0.0f} , {182.0f, 22.0f}},
                Rendering::Rectangle{ {0.0f, 0.0f}, {182.0f/ 256.0f, 22.0f / 256.0f}}
        );
        hotbar->set_layer(2);
        hotbar_select = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("gui_common"),
                Rendering::Rectangle{ {0.0f, 0.0f} , {24.0f, 24.0f}},
                Rendering::Rectangle{ {0.0f, 22.0f / 256.0f}, {24.0f/ 256.0f, 24.0f / 256.0f}}
        );
        hotbar_select->set_layer(3);
    }

    Inventory::~Inventory() {

    }

    auto Inventory::draw(double dt) -> void {

    }

    auto Inventory::increment_selection(std::any p) -> void {
        auto new_idx = get().selection_idx + 1;
        if (new_idx > 8) {
            new_idx = 0;
        }
        get().selection_idx = new_idx;
    }
    auto Inventory::decrement_selection(std::any p) -> void {
        auto new_idx = get().selection_idx - 1;
        if (new_idx < 0) {
            new_idx = 8;
        }
        get().selection_idx = new_idx;
    }

    auto Inventory::set_selection(std::any p) -> void {
        auto new_idx = std::any_cast<SelData>(p).idx;
        if (new_idx < 0 || new_idx > 8) {
            return;
        }
        get().selection_idx = new_idx;
    }


    auto Inventory::draw_hotbar(double dt) -> void {
        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate({ 149.0f, 1.0f, 0.0f });
        hotbar->draw();

        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate({ 148.0f + 20.0f * (float)selection_idx, 0.0f, 0.0f });
        hotbar_select->draw();
    }

}