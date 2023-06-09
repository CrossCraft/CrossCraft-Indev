#pragma once
#include <Utilities/Singleton.hpp>
#include <Rendering/Texture.hpp>
#include <Graphics/2D/Sprite.hpp>
#include <Math/Math.hpp>

namespace CrossCraft{
    using namespace Stardust_Celeste;

    class ModelRenderer {
        public:
        ModelRenderer();
        ~ModelRenderer();

        static auto init() -> void;
        static auto get() -> ModelRenderer&;

        auto draw_block(uint8_t id, Math::Vector3<float> position, Math::Vector3<float> rotation) -> void;
        auto draw_item(uint16_t id, Math::Vector3<float> position, Math::Vector3<float> rotation) -> void;

        auto draw_block_hand(uint8_t id, Math::Vector3<float> position) -> void;
        auto draw_item_hand(uint16_t id, Math::Vector3<float> position) -> void;

        auto draw_block_isometric(uint8_t id, Math::Vector3<float> position) -> void;
        auto draw_item_isometric(uint16_t id, Math::Vector3<float> position) -> void;

        private:
        
        uint32_t terrainTexID;
        uint32_t itemsTexID;

        ScopePtr<Graphics::G2D::Sprite> blockModels[63];
        ScopePtr<Graphics::G2D::Sprite> itemModels[66];
    };
}
