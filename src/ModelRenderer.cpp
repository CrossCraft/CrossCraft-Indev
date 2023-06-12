#include <ModelRenderer.hpp>
#include <ResourcePack.hpp>
#include <Chunk/ChunkUtil.hpp>
#include <Chunk/FaceConst.hpp>
#include <Player/Player.hpp>

namespace CrossCraft {

    ModelRenderer *gModelRenderer;

    ModelRenderer::ModelRenderer() {
        terrainTexID = ResourcePack::get().get_texture("terrain");
        itemsTexID = ResourcePack::get().get_texture("items");
    }

    ModelRenderer::~ModelRenderer() = default;


    auto get_rendering_rectangle(int id) -> Rendering::Rectangle {
        int x = id % 16;
        int y = id / 16;

        return Rendering::Rectangle{{(float) x * 16.0f / 256.0f, (float) y * 16.0f / 256.0f},
                                    {16.0f / 256.0f,             16.0f / 256.0f}};
    }

    constexpr int makeIndex(int x, int y) {
        return x + y * 16;
    }

    auto get_mapped_item_index(int id) -> int {
        id += 256;

        static const std::unordered_map<int, int> itemIndexMap = {
                {ITM_Iron_Shovel, makeIndex(2, 5)},
                {ITM_Iron_Pickaxe, makeIndex(2, 6)},
                {ITM_Iron_Axe, makeIndex(2, 7)},
                {ITM_Flint_and_Steel, makeIndex(5, 0)},
                {ITM_Apple, makeIndex(10, 0)},
                {ITM_Bow, makeIndex(5, 1)},
                {ITM_Arrow, makeIndex(5, 2)},
                {ITM_Coal, makeIndex(7, 0)},
                {ITM_Diamond, makeIndex(7, 3)},
                {ITM_Iron_Ingot, makeIndex(7, 1)},
                {ITM_Gold_Ingot, makeIndex(7, 2)},
                {ITM_Iron_Sword, makeIndex(2, 4)},
                {ITM_Wooden_Sword, makeIndex(0, 4)},
                {ITM_Wooden_Shovel, makeIndex(0, 5)},
                {ITM_Wooden_Pickaxe, makeIndex(0, 6)},
                {ITM_Wooden_Axe, makeIndex(0, 7)},
                {ITM_Stone_Sword, makeIndex(1, 4)},
                {ITM_Stone_Shovel, makeIndex(1, 5)},
                {ITM_Stone_Pickaxe, makeIndex(1, 6)},
                {ITM_Stone_Axe, makeIndex(1, 7)},
                {ITM_Diamond_Sword, makeIndex(3, 4)},
                {ITM_Diamond_Shovel, makeIndex(3, 5)},
                {ITM_Diamond_Pickaxe, makeIndex(3, 6)},
                {ITM_Diamond_Axe, makeIndex(3, 7)},
                {ITM_Stick, makeIndex(5, 3)},
                {ITM_Bowl, makeIndex(7, 4)},
                {ITM_Mushroom_Stew, makeIndex(8, 4)},
                {ITM_Golden_Sword, makeIndex(4, 4)},
                {ITM_Golden_Shovel, makeIndex(4, 5)},
                {ITM_Golden_Pickaxe, makeIndex(4, 6)},
                {ITM_Golden_Axe, makeIndex(4, 7)},
                {ITM_String, makeIndex(8, 0)},
                {ITM_Feather, makeIndex(8, 1)},
                {ITM_Gunpowder, makeIndex(8, 2)},
                {ITM_Wooden_Hoe, makeIndex(0, 8)},
                {ITM_Stone_Hoe, makeIndex(1, 8)},
                {ITM_Iron_Hoe, makeIndex(2, 8)},
                {ITM_Diamond_Hoe, makeIndex(3, 8)},
                {ITM_Golden_Hoe, makeIndex(4, 8)},
                {ITM_Seeds, makeIndex(9, 0)},
                {ITM_Wheat, makeIndex(9, 1)},
                {ITM_Bread, makeIndex(9, 2)},
                {ITM_Leather_Cap, makeIndex(0, 0)},
                {ITM_Leather_Tunic, makeIndex(0, 1)},
                {ITM_Leather_Pants, makeIndex(0, 2)},
                {ITM_Leather_Boots, makeIndex(0, 3)},
                {ITM_Chain_Helmet, makeIndex(1, 0)},
                {ITM_Chain_Chestplate, makeIndex(1, 1)},
                {ITM_Chain_Leggings, makeIndex(1, 2)},
                {ITM_Chain_Boots, makeIndex(1, 3)},
                {ITM_Iron_Helmet, makeIndex(2, 0)},
                {ITM_Iron_Chestplate, makeIndex(2, 1)},
                {ITM_Iron_Leggings, makeIndex(2, 2)},
                {ITM_Iron_Boots, makeIndex(2, 3)},
                {ITM_Diamond_Helmet, makeIndex(3, 0)},
                {ITM_Diamond_Chestplate, makeIndex(3, 1)},
                {ITM_Diamond_Leggings, makeIndex(3, 2)},
                {ITM_Diamond_Boots, makeIndex(3, 3)},
                {ITM_Golden_Helmet, makeIndex(4, 0)},
                {ITM_Golden_Chestplate, makeIndex(4, 1)},
                {ITM_Golden_Leggings, makeIndex(4, 2)},
                {ITM_Golden_Boots, makeIndex(4, 3)},
                {ITM_Flint, makeIndex(6, 0)},
                {ITM_Raw_Porkchop, makeIndex(7, 5)},
                {ITM_Cooked_Porkchop, makeIndex(8, 5)},
                {ITM_Painting, makeIndex(10, 1)}
        };

        auto it = itemIndexMap.find(id);
        if (it != itemIndexMap.end()) {
            return it->second;
        }

        return -1;
    }

    void
    add_face_to_mesh(const std::array<float, 12> &face, std::array<float, 8> tex, const mathfu::Vector<float, 3>& position,
                     uint32_t value, Rendering::Mesh<Rendering::Vertex> &mesh, uint32_t &idx_counter) {
        Rendering::Color c{};
        c.color = value;

        for (size_t i = 0, tx = 0, idx = 0; i < 4; i++) {
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

    void add_cube(block_t id, const mathfu::Vector<float, 3>& position, Rendering::Mesh<Rendering::Vertex> &mesh) {
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

        for (int i = 0; i < 63; i++) {
            gModelRenderer->blockModels[i] = create_scopeptr<Rendering::Mesh<Rendering::Vertex>>();

            add_cube(i, {-0.5f, -0.5f, -0.5f}, *gModelRenderer->blockModels[i]);
        }

        for (int i = 0; i < 65; i++) {
            gModelRenderer->itemModels[i] = create_scopeptr<Graphics::G2D::Sprite>(
                    ResourcePack::get().get_texture("items"),
                    Rendering::Rectangle{{0.0f,  0.0f},
                                         {16.0f, 16.0f}},
                    get_rendering_rectangle(
                            get_mapped_item_index(i)
                    )
            );
        }

        for (int i = 1; i < 11; i++) {
            uint32_t idx_counter = 0;
            gModelRenderer->breakModels[i] = create_scopeptr<Rendering::Mesh<Rendering::Vertex>>();

            auto p = mathfu::Vector<float, 3>(0, 0, 0);

            add_face_to_mesh(topFace, getTexCoord(239 + i, LIGHT_TOP), p, LIGHT_TOP,
                             *(gModelRenderer->breakModels[i].get()), idx_counter);
            add_face_to_mesh(bottomFace, getTexCoord(239 + i, LIGHT_TOP), p, LIGHT_TOP,
                             *(gModelRenderer->breakModels[i].get()), idx_counter);
            add_face_to_mesh(leftFace, getTexCoord(239 + i, LIGHT_TOP), p, LIGHT_TOP,
                             *(gModelRenderer->breakModels[i].get()), idx_counter);
            add_face_to_mesh(rightFace, getTexCoord(239 + i, LIGHT_TOP), p, LIGHT_TOP,
                             *(gModelRenderer->breakModels[i].get()), idx_counter);
            add_face_to_mesh(frontFace, getTexCoord(239 + i, LIGHT_TOP), p, LIGHT_TOP,
                             *(gModelRenderer->breakModels[i].get()), idx_counter);
            add_face_to_mesh(backFace, getTexCoord(239 + i, LIGHT_TOP), p, LIGHT_TOP,
                             *(gModelRenderer->breakModels[i].get()), idx_counter);

            gModelRenderer->breakModels[i]->setup_buffer();
        }

        //TODO: TIDY
        gModelRenderer->breakModels[0] = create_scopeptr<Rendering::Mesh<Rendering::Vertex>>();
        gModelRenderer->breakModels[0]->vertices.clear();
        gModelRenderer->breakModels[0]->vertices.shrink_to_fit();
        gModelRenderer->breakModels[0]->indices.clear();
        gModelRenderer->breakModels[0]->indices.shrink_to_fit();
        gModelRenderer->breakModels[0]->delete_data();

        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 1, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 1, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 1, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 1, 1});

        gModelRenderer->breakModels[0]->indices.push_back(0);
        gModelRenderer->breakModels[0]->indices.push_back(1);
        gModelRenderer->breakModels[0]->indices.push_back(2);
        gModelRenderer->breakModels[0]->indices.push_back(3);
        gModelRenderer->breakModels[0]->indices.push_back(0);

        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 0, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 0, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 0, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 0, 1});

        gModelRenderer->breakModels[0]->indices.push_back(4);
        gModelRenderer->breakModels[0]->indices.push_back(5);
        gModelRenderer->breakModels[0]->indices.push_back(6);
        gModelRenderer->breakModels[0]->indices.push_back(7);
        gModelRenderer->breakModels[0]->indices.push_back(4);

        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 0, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 1, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 1, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 0, 1});

        gModelRenderer->breakModels[0]->indices.push_back(8);
        gModelRenderer->breakModels[0]->indices.push_back(9);
        gModelRenderer->breakModels[0]->indices.push_back(10);
        gModelRenderer->breakModels[0]->indices.push_back(11);
        gModelRenderer->breakModels[0]->indices.push_back(8);

        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 0, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 1, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 1, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 0, 1});

        gModelRenderer->breakModels[0]->indices.push_back(12);
        gModelRenderer->breakModels[0]->indices.push_back(13);
        gModelRenderer->breakModels[0]->indices.push_back(14);
        gModelRenderer->breakModels[0]->indices.push_back(15);
        gModelRenderer->breakModels[0]->indices.push_back(12);

        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 0, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 0, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 1, 1, 0});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, {{0, 0, 0, 128}}, 0, 1, 0});

        gModelRenderer->breakModels[0]->indices.push_back(16);
        gModelRenderer->breakModels[0]->indices.push_back(17);
        gModelRenderer->breakModels[0]->indices.push_back(18);
        gModelRenderer->breakModels[0]->indices.push_back(19);
        gModelRenderer->breakModels[0]->indices.push_back(16);

        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, Rendering::Color{{0, 0, 0, 128}}, 0, 0, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, Rendering::Color{{0, 0, 0, 128}}, 1, 0, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, Rendering::Color{{0, 0, 0, 128}}, 1, 1, 1});
        gModelRenderer->breakModels[0]->vertices.push_back(
                Rendering::Vertex{0, 0, Rendering::Color{{0, 0, 0, 128}}, 0, 1, 1});

        gModelRenderer->breakModels[0]->indices.push_back(20);
        gModelRenderer->breakModels[0]->indices.push_back(21);
        gModelRenderer->breakModels[0]->indices.push_back(22);
        gModelRenderer->breakModels[0]->indices.push_back(23);
        gModelRenderer->breakModels[0]->indices.push_back(20);

        gModelRenderer->breakModels[0]->setup_buffer();
    }


    auto ModelRenderer::draw_block_outline() -> void {
        auto selPos = BreakInformation::get().gSelectedPos;
        mathfu::Vector<float, 3> position = mathfu::Vector<float, 3>(selPos.x, selPos.y, selPos.z);
        auto ctx = &Rendering::RenderContext::get();
        ctx->matrix_clear();
        ctx->matrix_translate(mathfu::Vector<float, 3>(position.x, position.y, position.z));
        breakModels[0]->draw(Rendering::PrimType::PRIM_TYPE_LINE);

        ctx->matrix_clear();
   }

    auto ModelRenderer::draw_break() -> void {
        if (BreakInformation::get().gBreaking) {
            auto ctx = &Rendering::RenderContext::get();
            auto pos = BreakInformation::get().gBreakingPos;

            ctx->matrix_clear();
            ctx->matrix_translate(mathfu::Vector<float, 3>(pos.x, pos.y, pos.z));
            ctx->matrix_translate(mathfu::Vector<float, 3>(-0.01f, -0.01f, -0.01f));
            ctx->matrix_scale({1.02f, 1.02f, 1.02f});

            if (BreakInformation::get().gBreakingTotal == 0.0f)
                return;

            int i = (BreakInformation::get().gBreakingTotal - BreakInformation::get().gBreakingTimer) /
                    (BreakInformation::get().gBreakingTotal / 10.0f);
            if (i >= 0 && i < 10) {
                GI::blend_func(GI_DST_COLOR, GI_SRC_COLOR);

                breakModels[i + 1]->draw();

                GI::blend_func(GI_SRC_ALPHA, GI_ONE_MINUS_SRC_ALPHA);
            }

            ctx->matrix_clear();
            return;
        }
    }

    auto ModelRenderer::get() -> ModelRenderer & {
        return *gModelRenderer;
    }

    auto ModelRenderer::draw_block(block_t id, const mathfu::Vector<float, 3>& position,
                                   const mathfu::Vector<float, 3>& rotation) -> void {
        Rendering::TextureManager::get().bind_texture(terrainTexID);
        Rendering::RenderContext::get().matrix_clear();

        Rendering::RenderContext::get().matrix_translate(position);
        Rendering::RenderContext::get().matrix_rotate({-rotation.x, -rotation.y, 0.0f});

        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_translate({0, -0.25f, 0});
        Rendering::RenderContext::get().matrix_scale({0.25f, 0.25f, 0.25f});

        blockModels[id]->draw();
        Rendering::RenderContext::get().matrix_pop();
    }

    auto
    ModelRenderer::draw_item(item_t id, const mathfu::Vector<float, 3>& position, const mathfu::Vector<float, 3>& rotation) -> void {
        auto idTransform = id - 256;
        if (idTransform < 0 || idTransform > 65) {
            return;
        }
        //TODO: USE CC Transforms

        GI::disable(GI_CULL_FACE);

        Rendering::TextureManager::get().bind_texture(itemsTexID);
        Rendering::RenderContext::get().matrix_clear();

        // Give model to world
        Rendering::RenderContext::get().matrix_translate(position); // [6] Position the model to player
        Rendering::RenderContext::get().matrix_rotate(
                {-rotation.x, -rotation.y, 0.0f}); // [5] Rotate the model to player

        // Scale and Center
        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_translate({0, 0, 0}); // [6] Position the model to player
        Rendering::RenderContext::get().matrix_scale({1.0f / 32.0f, 1.0f / 32.0f, 1.0f / 32.0f}); // [2] Scale the model
        Rendering::RenderContext::get().matrix_translate({-8.0f, -8.0f, 0.0f}); // [1] Center the model

        itemModels[idTransform]->draw();
        GI::enable(GI_CULL_FACE);
        Rendering::RenderContext::get().matrix_pop();

    }

    auto ModelRenderer::draw_block_hand(block_t id, const mathfu::Vector<float, 2>& rotation) -> void {
        GI::clearDepth();

        //TODO: USE CC Transforms

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

    auto ModelRenderer::draw_item_hand(item_t id, const mathfu::Vector<float, 2>& rotation) -> void {
        auto idTransform = id - 256;
        if (idTransform < 0 || idTransform > 65) {
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

    auto ModelRenderer::draw_block_isometric(block_t id, const mathfu::Vector<float, 3>& position, float scale) -> void {
        Rendering::TextureManager::get().bind_texture(terrainTexID);
        Rendering::RenderContext::get().matrix_clear();

        Rendering::RenderContext::get().matrix_translate(position);

        Rendering::RenderContext::get().matrix_push();
        Rendering::RenderContext::get().matrix_rotate({30.0f, 45.0f, 0.0f});
        Rendering::RenderContext::get().matrix_scale({10.0f * scale, 10.0f * scale, 10.0f * scale});

        blockModels[id]->draw();
        Rendering::RenderContext::get().matrix_pop();
    }

    auto ModelRenderer::draw_item_isometric(item_t id, const mathfu::Vector<float, 3>& position) -> void {
        auto idTransform = id - 256;
        if (idTransform < 0 || idTransform > 65) {
            return;
        }

        Rendering::TextureManager::get().bind_texture(itemsTexID);
        Rendering::RenderContext::get().matrix_clear();
        Rendering::RenderContext::get().matrix_translate(position - mathfu::Vector<float, 3>{8.0f, 8.0f, 0.0f});
        itemModels[idTransform]->draw();
    }

}