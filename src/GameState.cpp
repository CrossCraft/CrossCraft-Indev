#include <GameState.hpp>
#include <CC/core.h>
#include <Utilities/Controllers/KeyboardController.hpp>
#include <Utilities/Controllers/MouseController.hpp>

namespace CrossCraft {

    using namespace Stardust_Celeste::Utilities;
    void GameState::on_start() {
        // Initialize the core library.
        CC_Core_Init();

        // Send initial player position.
        CC_Event_Push_PlayerUpdate(PLAYER_SELF, 8.0f, 48.0f, 8.0f, 0.0f, 0.0f, false);

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
        kb_controller->add_command({(int)Input::Keys::Space, KeyFlag::Press | KeyFlag::Held}, {Player::jump, player.get()});

        mb_controller = new Utilities::Input::MouseController();
        mb_controller->add_command({(int)Input::MouseButtons::Left, KeyFlag::Press}, {Player::break_block, player.get()});
        mb_controller->add_command({(int)Input::MouseButtons::Right, KeyFlag::Press}, {Player::place_block, player.get()});

        Input::add_controller(kb_controller);
        Input::add_controller(mb_controller);
        Input::set_differential_mode("Mouse", true);
        Input::set_cursor_center();

        Rendering::RenderContext::get().set_mode_3D();

        world = create_refptr<World>();

        terrainTexID = Rendering::TextureManager::get().load_texture("resourcepacks/default/assets/minecraft/textures/terrain.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true);
        Rendering::TextureManager::get().bind_texture(terrainTexID);
    }

    void GameState::on_update(Core::Application* app, double dt) {
        Input::update();

        player->update(dt);
        world->update(dt);

        CC_Core_Update(dt);

        CC_Event* event;
        while((event = CC_Event_Poll()) != NULL) {
            switch(event->type) {
                case CC_EVENT_SET_BLOCK: {
                    world->handle_block_update(event->data.set_block.x, event->data.set_block.y, event->data.set_block.z);
                    break;
                }

                default:
                    SC_APP_INFO("Unhandled event type: {}", event->type);
                    break;
            }
        }
    }

    void GameState::on_draw(Core::Application* app, double dt) {
        world->draw();
        player->draw(dt);
    }

    void GameState::on_cleanup() {
        CC_Core_Term();
    }

} // namespace CrossCraft