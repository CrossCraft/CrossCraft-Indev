#include <GameState.hpp>
#include <CC/core.h>
#include <Utilities/Controllers/KeyboardController.hpp>

namespace CrossCraft {

    using namespace Stardust_Celeste::Utilities;
    void GameState::on_start() {
        // Initialize the core library.
        CC_Core_Init();

        // Send initial player position.
        CC_Event_Push_PlayerUpdate(PLAYER_SELF, 0.0f, 2.625f, 0.0f, 0.0f, 0.0f, false);

        // Make sure player position is handled.
        CC_Core_Update(0.0);

        // Create the player.
        player = create_refptr<Player>();

        // Setup controls
        kb_controller = new Utilities::Input::KeyboardController();

        kb_controller->add_command({(int)Input::Keys::W, KeyFlag::Press | KeyFlag::Held}, {Player::move_forward, player.get()});
        kb_controller->add_command({(int)Input::Keys::S, KeyFlag::Press | KeyFlag::Held}, {Player::move_backward, player.get()});
        kb_controller->add_command({(int)Input::Keys::A, KeyFlag::Press | KeyFlag::Held}, {Player::move_left, player.get()});
        kb_controller->add_command({(int)Input::Keys::D, KeyFlag::Press | KeyFlag::Held}, {Player::move_right, player.get()});

        Input::add_controller(kb_controller);
        Input::set_differential_mode("Mouse", true);
        Input::set_cursor_center();

        Rendering::RenderContext::get().set_mode_3D();

        chunk_mesh = create_refptr<ChunkMesh>(0, 0, 0);

        terrainTexID = Rendering::TextureManager::get().load_texture("resourcepacks/default/assets/minecraft/textures/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true);
        Rendering::TextureManager::get().bind_texture(terrainTexID);
    }

    double fpsTimer = 0.0;
    int totalFPS = 0;

    void GameState::on_update(Core::Application* app, double dt) {
        Input::update();

        player->update(dt);

        fpsTimer += dt;
        if(fpsTimer > 1.0) {
            SC_APP_INFO("FPS: {}", totalFPS);
            totalFPS = 0;
            fpsTimer = 0.0;
        }

        CC_Core_Update(dt);
    }

    void GameState::on_draw(Core::Application* app, double dt) {
        player->draw(dt);
        chunk_mesh->draw(ChunkMeshSelection::Opaque);
        totalFPS++;
    }

    void GameState::on_cleanup() {
        CC_Core_Term();
    }

} // namespace CrossCraft