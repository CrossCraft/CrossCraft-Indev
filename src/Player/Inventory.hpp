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

    class Inventory : public Singleton {
        public:

        Inventory();
        ~Inventory();

        inline static auto get() -> Inventory& {
            static Inventory inventory;
            return inventory;
        }

        auto draw(double dt) -> void;
        auto draw_hotbar(double dt) -> void;

        private:
        std::array<ItemData, 45> item_array;

    }
}
