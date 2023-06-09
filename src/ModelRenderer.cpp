#include <ModelRenderer.hpp>
#include <ResourcePack.hpp>

typedef uint8_t block_t;
typedef uint16_t item_t;

const item_t ITM_Iron_Shovel = 256;
const item_t ITM_Iron_Pickaxe = 257;
const item_t ITM_Iron_Axe = 258;
const item_t ITM_Flint_and_Steel = 259;
const item_t ITM_Apple = 260;
const item_t ITM_Bow = 261;
const item_t ITM_Arrow = 262;
const item_t ITM_Coal = 263;
const item_t ITM_Diamond = 264;
const item_t ITM_Iron_Ingot = 265;
const item_t ITM_Gold_Ingot = 266;
const item_t ITM_Iron_Sword = 267;
const item_t ITM_Wooden_Sword = 268;
const item_t ITM_Wooden_Shovel = 269;
const item_t ITM_Wooden_Pickaxe = 270;
const item_t ITM_Wooden_Axe = 271;
const item_t ITM_Stone_Sword = 272;
const item_t ITM_Stone_Shovel = 273;
const item_t ITM_Stone_Pickaxe = 274;
const item_t ITM_Stone_Axe = 275;
const item_t ITM_Diamond_Sword = 276;
const item_t ITM_Diamond_Shovel = 277;
const item_t ITM_Diamond_Pickaxe = 278;
const item_t ITM_Diamond_Axe = 279;
const item_t ITM_Stick = 280;
const item_t ITM_Bowl = 281;
const item_t ITM_Mushroom_Stew = 282;
const item_t ITM_Golden_Sword = 283;
const item_t ITM_Golden_Shovel = 284;
const item_t ITM_Golden_Pickaxe = 285;
const item_t ITM_Golden_Axe = 286;
const item_t ITM_String = 287;
const item_t ITM_Feather = 288;
const item_t ITM_Gunpowder = 289;
const item_t ITM_Wooden_Hoe = 290;
const item_t ITM_Stone_Hoe = 291;
const item_t ITM_Iron_Hoe = 292;
const item_t ITM_Diamond_Hoe = 293;
const item_t ITM_Golden_Hoe = 294;
const item_t ITM_Seeds = 295;
const item_t ITM_Wheat = 296;
const item_t ITM_Bread = 297;
const item_t ITM_Leather_Cap = 298;
const item_t ITM_Leather_Tunic = 299;
const item_t ITM_Leather_Pants = 300;
const item_t ITM_Leather_Boots = 301;
const item_t ITM_Chain_Helmet = 302;
const item_t ITM_Chain_Chestplate = 303;
const item_t ITM_Chain_Leggings = 304;
const item_t ITM_Chain_Boots = 305;
const item_t ITM_Iron_Helmet = 306;
const item_t ITM_Iron_Chestplate = 307;
const item_t ITM_Iron_Leggings = 308;
const item_t ITM_Iron_Boots = 309;
const item_t ITM_Diamond_Helmet = 310;
const item_t ITM_Diamond_Chestplate = 311;
const item_t ITM_Diamond_Leggings = 312;
const item_t ITM_Diamond_Boots = 313;
const item_t ITM_Golden_Helmet = 314;
const item_t ITM_Golden_Chestplate = 315;
const item_t ITM_Golden_Leggings = 316;
const item_t ITM_Golden_Boots = 317;
const item_t ITM_Flint = 318;
const item_t ITM_Raw_Porkchop = 319;
const item_t ITM_Cooked_Porkchop = 320;
const item_t ITM_Painting = 321;


namespace CrossCraft {

    ModelRenderer* gModelRenderer;

    ModelRenderer::ModelRenderer() {
        terrainTexID = ResourcePack::get().get_texture("terrain");
        itemsTexID = ResourcePack::get().get_texture("items");
    }

    ModelRenderer::~ModelRenderer() {

    }


    auto get_rendering_rectangle(int id) -> Rendering::Rectangle {
        int x = id % 16;
        int y = id / 16;

        return Rendering::Rectangle{ {(float)x * 16.0f / 256.0f, (float)y * 16.0f / 256.0f}, {16.0f / 256.0f, 16.0f / 256.0f}};
    }

#define makeIndex(x, y) ((x) + (y) * 16)

    auto get_mapped_item_index(int id) -> int {
        id += 256;

        switch(id) {
case ITM_Iron_Shovel: {
    return makeIndex(2, 5);
}
case ITM_Iron_Pickaxe: {
    return makeIndex(2, 6);
}
case ITM_Iron_Axe: {
    return makeIndex(2, 7);
}
case ITM_Flint_and_Steel: {
    return makeIndex(5, 0);
}
case ITM_Apple: {
    return makeIndex(10, 0);
}
case ITM_Bow: {
    return makeIndex(5, 1);
}
case ITM_Arrow: {
    return makeIndex(5, 2);
}
case ITM_Coal: {
    return makeIndex(7, 0);
}
case ITM_Diamond: {
    return makeIndex(7, 3);
}
case ITM_Iron_Ingot: {
    return makeIndex(7, 1);
}
case ITM_Gold_Ingot: {
    return makeIndex(7, 2);
}
case ITM_Iron_Sword: {
    return makeIndex(2, 4);
}
case ITM_Wooden_Sword: {
    return makeIndex(0, 4);
}
case ITM_Wooden_Shovel: {
    return makeIndex(0, 5);
}
case ITM_Wooden_Pickaxe: {
    return makeIndex(0, 6);
}
case ITM_Wooden_Axe: {
    return makeIndex(0, 7);
}
case ITM_Stone_Sword: {
    return makeIndex(1, 4);
}
case ITM_Stone_Shovel: {
    return makeIndex(1, 5);
}
case ITM_Stone_Pickaxe: {
    return makeIndex(1, 6);
}
case ITM_Stone_Axe: {
    return makeIndex(1, 7);
}
case ITM_Diamond_Sword: {
    return makeIndex(3, 4);
}
case ITM_Diamond_Shovel: {
    return makeIndex(3, 5);
}
case ITM_Diamond_Pickaxe: {
    return makeIndex(3, 6);
}
case ITM_Diamond_Axe: {
    return makeIndex(3, 7);
}
case ITM_Stick: {
    return makeIndex(5, 3);
}
case ITM_Bowl: {
    return makeIndex(7, 4);
}
case ITM_Mushroom_Stew: {
    return makeIndex(8, 4);
}
case ITM_Golden_Sword: {
    return makeIndex(4, 4);
}
case ITM_Golden_Shovel: {
    return makeIndex(4, 5);
}
case ITM_Golden_Pickaxe: {
    return makeIndex(4, 6);
}
case ITM_Golden_Axe: {
    return makeIndex(4, 7);
}
case ITM_String: {
    return makeIndex(8, 0);
}
case ITM_Feather: {
    return makeIndex(8, 1);
}
case ITM_Gunpowder: {
    return makeIndex(8, 2);
}
case ITM_Wooden_Hoe: {
    return makeIndex(0, 8);
}
case ITM_Stone_Hoe: {
    return makeIndex(1, 8);
}
case ITM_Iron_Hoe: {
    return makeIndex(2, 8);
}
case ITM_Diamond_Hoe: {
    return makeIndex(3, 8);
}
case ITM_Golden_Hoe: {
    return makeIndex(4, 8);
}
case ITM_Seeds: {
    return makeIndex(9, 0);
}
case ITM_Wheat: {
    return makeIndex(9, 1);
}
case ITM_Bread: {
    return makeIndex(9, 2);
}
case ITM_Leather_Cap: {
    return makeIndex(0, 0);
}
case ITM_Leather_Tunic: {
    return makeIndex(0, 1);
}
case ITM_Leather_Pants: {
    return makeIndex(0, 2);
}
case ITM_Leather_Boots: {
    return makeIndex(0, 3);
}
case ITM_Chain_Helmet: {
    return makeIndex(1, 0);
}
case ITM_Chain_Chestplate: {
    return makeIndex(1, 1);
}
case ITM_Chain_Leggings: {
    return makeIndex(1, 2);
}
case ITM_Chain_Boots: {
    return makeIndex(1, 3);
}
case ITM_Iron_Helmet: {
    return makeIndex(2, 0);
}
case ITM_Iron_Chestplate: {
    return makeIndex(2, 1);
}
case ITM_Iron_Leggings: {
    return makeIndex(2, 2);
}
case ITM_Iron_Boots: {
    return makeIndex(2, 3);
}
case ITM_Diamond_Helmet: {
    return makeIndex(3, 0);
}
case ITM_Diamond_Chestplate: {
    return makeIndex(3, 1);
}
case ITM_Diamond_Leggings: {
    return makeIndex(3, 2);
}
case ITM_Diamond_Boots: {
    return makeIndex(3, 3);
}
case ITM_Golden_Helmet: {
    return makeIndex(4, 0);
}
case ITM_Golden_Chestplate: {
    return makeIndex(4, 1);
}
case ITM_Golden_Leggings: {
    return makeIndex(4, 2);
}
case ITM_Golden_Boots: {
    return makeIndex(4, 3);
}
case ITM_Flint: {
    return makeIndex(6, 0);
}
case ITM_Raw_Porkchop: {
    return makeIndex(7, 5);
}
case ITM_Cooked_Porkchop: {
    return makeIndex(8, 5);
}
case ITM_Painting: {
    return makeIndex(10, 1);
}
        }

        return -1;
    }

    auto ModelRenderer::init() -> void {
        gModelRenderer = new ModelRenderer();
        for(int i = 0; i < 65; i++) {
            gModelRenderer->itemModels[i] = create_scopeptr<Graphics::G2D::Sprite>(
                ResourcePack::get().get_texture("items"),
                Rendering::Rectangle{ {0.0f, 0.0f} , {16.0f, 16.0f}},
                get_rendering_rectangle(
                    get_mapped_item_index(i)
                )
            );
        }
    }

    auto ModelRenderer::get() -> ModelRenderer& {
        return *gModelRenderer;    
    }

    auto ModelRenderer::draw_block(block_t id, Math::Vector3<float> position, Math::Vector3<float> rotation) -> void{}
    auto ModelRenderer::draw_item(item_t id, Math::Vector3<float> position, Math::Vector3<float> rotation) -> void{}

    auto ModelRenderer::draw_block_hand(block_t id, Math::Vector3<float> position) -> void{}
    auto ModelRenderer::draw_item_hand(item_t id, Math::Vector3<float> position) -> void{}

    auto ModelRenderer::draw_block_isometric(block_t id, Math::Vector3<float> position) -> void{}
    auto ModelRenderer::draw_item_isometric(item_t id, Math::Vector3<float> position) -> void {
        auto idTransform = id - 256;
        if(idTransform < 0 || idTransform > 65) {
            return;
        }

        Rendering::TextureManager::get().bind_texture(itemsTexID);
        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(position);
        itemModels[idTransform]->draw();
    }

}