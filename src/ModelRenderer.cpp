#include <ModelRenderer.hpp>
#include <ResourcePack.hpp>
#include <Chunk/ChunkUtil.hpp>
#include <Chunk/FaceConst.hpp>

const item_t I_Iron_Shovel = 256;
const item_t I_Iron_Pickaxe = 257;
const item_t I_Iron_Axe = 258;
const item_t I_Flint_and_Steel = 259;
const item_t I_Apple = 260;
const item_t I_Bow = 261;
const item_t I_Arrow = 262;
const item_t I_Coal = 263;
const item_t I_Diamond = 264;
const item_t I_Iron_Ingot = 265;
const item_t I_Gold_Ingot = 266;
const item_t I_Iron_Sword = 267;
const item_t I_Wooden_Sword = 268;
const item_t I_Wooden_Shovel = 269;
const item_t I_Wooden_Pickaxe = 270;
const item_t I_Wooden_Axe = 271;
const item_t I_Stone_Sword = 272;
const item_t I_Stone_Shovel = 273;
const item_t I_Stone_Pickaxe = 274;
const item_t I_Stone_Axe = 275;
const item_t I_Diamond_Sword = 276;
const item_t I_Diamond_Shovel = 277;
const item_t I_Diamond_Pickaxe = 278;
const item_t I_Diamond_Axe = 279;
const item_t I_Stick = 280;
const item_t I_Bowl = 281;
const item_t I_Mushroom_Stew = 282;
const item_t I_Golden_Sword = 283;
const item_t I_Golden_Shovel = 284;
const item_t I_Golden_Pickaxe = 285;
const item_t I_Golden_Axe = 286;
const item_t I_String = 287;
const item_t I_Feather = 288;
const item_t I_Gunpowder = 289;
const item_t I_Wooden_Hoe = 290;
const item_t I_Stone_Hoe = 291;
const item_t I_Iron_Hoe = 292;
const item_t I_Diamond_Hoe = 293;
const item_t I_Golden_Hoe = 294;
const item_t I_Seeds = 295;
const item_t I_Wheat = 296;
const item_t I_Bread = 297;
const item_t I_Leather_Cap = 298;
const item_t I_Leather_Tunic = 299;
const item_t I_Leather_Pants = 300;
const item_t I_Leather_Boots = 301;
const item_t I_Chain_Helmet = 302;
const item_t I_Chain_Chestplate = 303;
const item_t I_Chain_Leggings = 304;
const item_t I_Chain_Boots = 305;
const item_t I_Iron_Helmet = 306;
const item_t I_Iron_Chestplate = 307;
const item_t I_Iron_Leggings = 308;
const item_t I_Iron_Boots = 309;
const item_t I_Diamond_Helmet = 310;
const item_t I_Diamond_Chestplate = 311;
const item_t I_Diamond_Leggings = 312;
const item_t I_Diamond_Boots = 313;
const item_t I_Golden_Helmet = 314;
const item_t I_Golden_Chestplate = 315;
const item_t I_Golden_Leggings = 316;
const item_t I_Golden_Boots = 317;
const item_t I_Flint = 318;
const item_t I_Raw_Porkchop = 319;
const item_t I_Cooked_Porkchop = 320;
const item_t I_Painting = 321;


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
case I_Iron_Shovel: {
    return makeIndex(2, 5);
}
case I_Iron_Pickaxe: {
    return makeIndex(2, 6);
}
case I_Iron_Axe: {
    return makeIndex(2, 7);
}
case I_Flint_and_Steel: {
    return makeIndex(5, 0);
}
case I_Apple: {
    return makeIndex(10, 0);
}
case I_Bow: {
    return makeIndex(5, 1);
}
case I_Arrow: {
    return makeIndex(5, 2);
}
case I_Coal: {
    return makeIndex(7, 0);
}
case I_Diamond: {
    return makeIndex(7, 3);
}
case I_Iron_Ingot: {
    return makeIndex(7, 1);
}
case I_Gold_Ingot: {
    return makeIndex(7, 2);
}
case I_Iron_Sword: {
    return makeIndex(2, 4);
}
case I_Wooden_Sword: {
    return makeIndex(0, 4);
}
case I_Wooden_Shovel: {
    return makeIndex(0, 5);
}
case I_Wooden_Pickaxe: {
    return makeIndex(0, 6);
}
case I_Wooden_Axe: {
    return makeIndex(0, 7);
}
case I_Stone_Sword: {
    return makeIndex(1, 4);
}
case I_Stone_Shovel: {
    return makeIndex(1, 5);
}
case I_Stone_Pickaxe: {
    return makeIndex(1, 6);
}
case I_Stone_Axe: {
    return makeIndex(1, 7);
}
case I_Diamond_Sword: {
    return makeIndex(3, 4);
}
case I_Diamond_Shovel: {
    return makeIndex(3, 5);
}
case I_Diamond_Pickaxe: {
    return makeIndex(3, 6);
}
case I_Diamond_Axe: {
    return makeIndex(3, 7);
}
case I_Stick: {
    return makeIndex(5, 3);
}
case I_Bowl: {
    return makeIndex(7, 4);
}
case I_Mushroom_Stew: {
    return makeIndex(8, 4);
}
case I_Golden_Sword: {
    return makeIndex(4, 4);
}
case I_Golden_Shovel: {
    return makeIndex(4, 5);
}
case I_Golden_Pickaxe: {
    return makeIndex(4, 6);
}
case I_Golden_Axe: {
    return makeIndex(4, 7);
}
case I_String: {
    return makeIndex(8, 0);
}
case I_Feather: {
    return makeIndex(8, 1);
}
case I_Gunpowder: {
    return makeIndex(8, 2);
}
case I_Wooden_Hoe: {
    return makeIndex(0, 8);
}
case I_Stone_Hoe: {
    return makeIndex(1, 8);
}
case I_Iron_Hoe: {
    return makeIndex(2, 8);
}
case I_Diamond_Hoe: {
    return makeIndex(3, 8);
}
case I_Golden_Hoe: {
    return makeIndex(4, 8);
}
case I_Seeds: {
    return makeIndex(9, 0);
}
case I_Wheat: {
    return makeIndex(9, 1);
}
case I_Bread: {
    return makeIndex(9, 2);
}
case I_Leather_Cap: {
    return makeIndex(0, 0);
}
case I_Leather_Tunic: {
    return makeIndex(0, 1);
}
case I_Leather_Pants: {
    return makeIndex(0, 2);
}
case I_Leather_Boots: {
    return makeIndex(0, 3);
}
case I_Chain_Helmet: {
    return makeIndex(1, 0);
}
case I_Chain_Chestplate: {
    return makeIndex(1, 1);
}
case I_Chain_Leggings: {
    return makeIndex(1, 2);
}
case I_Chain_Boots: {
    return makeIndex(1, 3);
}
case I_Iron_Helmet: {
    return makeIndex(2, 0);
}
case I_Iron_Chestplate: {
    return makeIndex(2, 1);
}
case I_Iron_Leggings: {
    return makeIndex(2, 2);
}
case I_Iron_Boots: {
    return makeIndex(2, 3);
}
case I_Diamond_Helmet: {
    return makeIndex(3, 0);
}
case I_Diamond_Chestplate: {
    return makeIndex(3, 1);
}
case I_Diamond_Leggings: {
    return makeIndex(3, 2);
}
case I_Diamond_Boots: {
    return makeIndex(3, 3);
}
case I_Golden_Helmet: {
    return makeIndex(4, 0);
}
case I_Golden_Chestplate: {
    return makeIndex(4, 1);
}
case I_Golden_Leggings: {
    return makeIndex(4, 2);
}
case I_Golden_Boots: {
    return makeIndex(4, 3);
}
case I_Flint: {
    return makeIndex(6, 0);
}
case I_Raw_Porkchop: {
    return makeIndex(7, 5);
}
case I_Cooked_Porkchop: {
    return makeIndex(8, 5);
}
case I_Painting: {
    return makeIndex(10, 1);
}
        }

        return -1;
    }

    void add_face_to_mesh(const std::array<float, 12> &face, std::array<float, 8> tex, mathfu::Vector<float, 3> position,
                                uint32_t value, Rendering::Mesh<Rendering::Vertex>& mesh, uint32_t& idx_counter) {
        Rendering::Color c;
        c.color = value;

        for(size_t i =0, tx = 0, idx = 0; i < 4; i++) {
            mesh.vertices.push_back(Rendering::Vertex{
                    tex[tx++],
                    tex[tx++],
                    c,
                    face[idx++] + position.x,
                    face[idx++] + position.y,
                    face[idx++] + position.z,
            });
        }

        mesh.indices.push_back(idx_counter + 0);
        mesh.indices.push_back(idx_counter + 1);
        mesh.indices.push_back(idx_counter + 2);
        mesh.indices.push_back(idx_counter + 2);
        mesh.indices.push_back(idx_counter + 3);
        mesh.indices.push_back(idx_counter + 0);
        idx_counter += 4;
    }
    
    void add_cube(block_t id, mathfu::Vector<float, 3> position, Rendering::Mesh<Rendering::Vertex>& mesh) {
        uint32_t idx_counter = 0;
        add_face_to_mesh(topFace, getTexCoord(id, LIGHT_TOP), position, LIGHT_TOP, mesh, idx_counter);
        add_face_to_mesh(bottomFace, getTexCoord(id, LIGHT_BOT), position, LIGHT_BOT, mesh, idx_counter);

        add_face_to_mesh(leftFace, getTexCoord(id, LIGHT_SIDE_X), position, LIGHT_SIDE_X, mesh, idx_counter);
        add_face_to_mesh(rightFace, getTexCoord(id, LIGHT_SIDE_X), position, LIGHT_SIDE_X, mesh, idx_counter);

        add_face_to_mesh(frontFace, getTexCoord(id, LIGHT_SIDE_Z), position, LIGHT_SIDE_Z, mesh, idx_counter);
        add_face_to_mesh(backFace, getTexCoord(id, LIGHT_SIDE_Z), position, LIGHT_SIDE_Z, mesh, idx_counter);

        mesh.setup_buffer();
    }

    auto ModelRenderer::init() -> void {
        gModelRenderer = new ModelRenderer();

        for(int i = 0; i < 63; i++) {
            gModelRenderer->blockModels[i] = create_scopeptr<Rendering::Mesh<Rendering::Vertex>>();

            add_cube(i, {-0.5f, -0.5f, -0.5f}, *gModelRenderer->blockModels[i]);
        }

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

    auto ModelRenderer::draw_block(block_t id, mathfu::Vector<float, 3> position, mathfu::Vector<float, 3> rotation) -> void{
        Rendering::TextureManager::get().bind_texture(terrainTexID);
        Rendering::RenderContext::get().matrix_clear();

        Rendering::RenderContext::get().matrix_translate(position);
        Rendering::RenderContext::get().matrix_rotate({-rotation.x, -rotation.y, 0.0f});

        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_translate({0, 0.5f, 0});
        Rendering::RenderContext::get().matrix_scale({0.25f, 0.25f, 0.25f});

        blockModels[id]->draw();
        Rendering::RenderContext::get().matrix_pop();
    }

    auto ModelRenderer::draw_item(item_t id, mathfu::Vector<float, 3> position, mathfu::Vector<float, 3> rotation) -> void{
        auto idTransform = id - 256;
        if(idTransform < 0 || idTransform > 65) {
            return;
        }

        GI::disable(GI_CULL_FACE);

        Rendering::TextureManager::get().bind_texture(itemsTexID);
        Rendering::RenderContext::get().matrix_clear();

        // Give model to world
        Rendering::RenderContext::get().matrix_translate(position); // [6] Position the model to player
        Rendering::RenderContext::get().matrix_rotate({-rotation.x, -rotation.y, 0.0f}); // [5] Rotate the model to player

        // Scale and Center
        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_translate({0, 0.5f, 0}); // [6] Position the model to player
        Rendering::RenderContext::get().matrix_scale({1.0f / 32.0f, 1.0f / 32.0f, 1.0f / 32.0f}); // [2] Scale the model
        Rendering::RenderContext::get().matrix_translate({-8.0f, -8.0f, 0.0f}); // [1] Center the model

        itemModels[idTransform]->draw();
        GI::enable(GI_CULL_FACE);
        Rendering::RenderContext::get().matrix_pop();

    }

    auto ModelRenderer::draw_block_hand(block_t id, mathfu::Vector<float, 3> position, mathfu::Vector<float, 2> rotation) -> void {
        GI::clearDepth();

        Rendering::TextureManager::get().bind_texture(terrainTexID);
        Rendering::RenderContext::get().set_mode_3D();
        Rendering::RenderContext::get().matrix_clear();

        // Rotate and Position in Hand
        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_translate({0.9f, -0.5f, -1.0f}); // [3] Position the model

        // Scale and Center
        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_rotate({0.0f, 15.0f, 0.0f}); // [5] Rotate the model to player
        Rendering::RenderContext::get().matrix_scale({0.5f, 0.5f, 0.5f}); // [2] Scale the model

        blockModels[id]->draw();
        GI::enable(GI_CULL_FACE);
        Rendering::RenderContext::get().matrix_pop();
        Rendering::RenderContext::get().matrix_pop();
    }
    auto ModelRenderer::draw_item_hand(item_t id, mathfu::Vector<float, 3> position, mathfu::Vector<float, 2> rotation) -> void{
        auto idTransform = id - 256;
        if(idTransform < 0 || idTransform > 65) {
            return;
        }

        GI::disable(GI_CULL_FACE);
        GI::clearDepth();

        Rendering::TextureManager::get().bind_texture(itemsTexID);
        Rendering::RenderContext::get().set_mode_3D();
        Rendering::RenderContext::get().matrix_clear();

        // Rotate and Position in Hand
        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_translate({0.45f, -0.25f, -0.48f}); // [3] Position the model

        // Scale and Center
        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_rotate({-30.0f, -60.0f, 0.0f}); // [5] Rotate the model to player
        Rendering::RenderContext::get().matrix_scale({1.0f / 48.0f, 1.0f / 48.0f, 1.0f / 48.0f}); // [2] Scale the model
        Rendering::RenderContext::get().matrix_translate({-8.0f, -8.0f, 0.0f}); // [1] Center the model

        itemModels[idTransform]->draw();
        GI::enable(GI_CULL_FACE);
        Rendering::RenderContext::get().matrix_pop();
        Rendering::RenderContext::get().matrix_pop();
    }

    auto ModelRenderer::draw_block_isometric(block_t id, mathfu::Vector<float, 3> position) -> void{
        Rendering::TextureManager::get().bind_texture(terrainTexID);
        Rendering::RenderContext::get().matrix_clear();

        Rendering::RenderContext::get().matrix_translate(position);

        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_rotate({30.0f, 45.0f, 0.0f});
        Rendering::RenderContext::get().matrix_scale({16.0f, 16.0f, 16.0f});

        blockModels[id]->draw();
        Rendering::RenderContext::get().matrix_pop();
    }
    auto ModelRenderer::draw_item_isometric(item_t id, mathfu::Vector<float, 3> position) -> void {
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