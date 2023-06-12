#pragma once

#include <Utilities/Singleton.hpp>
#include <Rendering/Texture.hpp>
#include <ResourcePack.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Rendering/Primitive/Rectangle.hpp>
#include <UI/FontRender.hpp>
#include <CC/core.h>

namespace CrossCraft {
    using namespace Stardust_Celeste;

    struct SelData {
        uint8_t idx;
    };

    class Inventory : public Singleton {
    public:

        Inventory();

        ~Inventory();

        auto try_add_item(ItemData item) -> bool;

        inline static auto get() -> Inventory & {
            static Inventory inventory;
            return inventory;
        }

        inline static auto get_selected() -> ItemData & {
            return get().item_array[get().selection_idx + 36];
        }

        inline static auto is_open() -> bool {
            return get().open;
        }

        static auto left_action(std::any p) -> void;
        static auto right_action(std::any p) -> void;

        auto update() -> void;
        auto draw(double dt) -> void;

        auto draw_block_hand(const mathfu::Vector<float, 3>& position, const mathfu::Vector<float, 2>& rotation, double dt) -> void;

        auto draw_hotbar(double dt) -> void;

        static auto increment_selection(const std::any p) -> void;

        static auto decrement_selection(const std::any p) -> void;

        static auto set_selection(std::any p) -> void;

        static auto drop_selection(std::any p) -> void;

        static auto toggle_inventory(const std::any p) -> void;

    private:
        bool open;
        std::array<ItemData, 45> item_array{};
        ItemData pickup_slot;
        uint8_t selection_idx = 0;
        mathfu::Vector<float, 2> mouse_pos;

        RefPtr<FontRender> font_render_hotbar, font_render_inventory;
        ScopePtr<Graphics::G2D::Sprite> hotbar, hotbar_select;

        RefPtr<Rendering::Primitive::Rectangle> background_rectangle;
        ScopePtr<Graphics::G2D::Sprite> inventory_background;


        bool is_mouse_pressed = false;
        bool is_mouse_held = false;
    };
}
