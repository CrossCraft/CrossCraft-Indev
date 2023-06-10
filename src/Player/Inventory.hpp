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

        inline static auto get() -> Inventory& {
            static Inventory inventory;
            return inventory;
        }

        inline static auto get_selected() -> ItemData& {
            return get().item_array[get().selection_idx];
        }

        auto draw(double dt) -> void;
        auto draw_block_hand(mathfu::Vector<float, 3> position, mathfu::Vector<float, 2> rotation, double dt) -> void;
        auto draw_hotbar(double dt) -> void;

        static auto increment_selection(std::any p) -> void;
        static auto decrement_selection(std::any p) -> void;
        static auto set_selection(std::any p) -> void;

        private:
        std::array<ItemData, 45> item_array;
        uint8_t selection_idx = 0;

        RefPtr<FontRender> font_render;
        ScopePtr<Graphics::G2D::Sprite> hotbar, hotbar_select;
    };
}
