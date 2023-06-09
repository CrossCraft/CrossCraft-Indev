#include <GameState.hpp>
#include <CC/core.h>
#include <Utilities/Controllers/KeyboardController.hpp>
#include <Utilities/Controllers/MouseController.hpp>
#include <ResourcePack.hpp>
#include <Player/InGameMenu.hpp>
#include <Player/Inventory.hpp>

namespace CrossCraft {

    using namespace Stardust_Celeste::Utilities;


    auto GameState::on_action_left(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);

        if(InGameMenu::get().is_open()) {
            InGameMenu::left_click_command(p);
        } else {
            Player::break_block(player);
        }
    }
    auto GameState::on_action_right(std::any p) -> void {
        auto player = std::any_cast<Player*>(p);

        if(InGameMenu::get().is_open()) {
            InGameMenu::right_click_command(p);
        } else {
            Player::place_block(player);
        }
    }

    void GameState::on_start() {
        ResourcePack::get().add_pack("resourcepacks/default.zip");
        ResourcePack::get().load();

        // Initialize the core library.
        CC_Core_Init();

        // Send initial player position.
        CC_Event_Push_PlayerUpdate(PLAYER_SELF, 128.0f, 48.0f, 128.0f, 0.0f, 0.0f, false);

        // Make sure player position is handled.
        CC_Core_Update(0.0);

        // Create the player.
        player = create_refptr<Player>();
        InGameMenu::init();

        // Setup controls
        kb_controller = new Utilities::Input::KeyboardController();

        kb_controller->add_command({(int)Input::Keys::W, KeyFlag::Press | KeyFlag::Held}, {Player::move_forward, player.get()});
        kb_controller->add_command({(int)Input::Keys::S, KeyFlag::Press | KeyFlag::Held}, {Player::move_backward, player.get()});
        kb_controller->add_command({(int)Input::Keys::A, KeyFlag::Press | KeyFlag::Held}, {Player::move_left, player.get()});
        kb_controller->add_command({(int)Input::Keys::D, KeyFlag::Press | KeyFlag::Held}, {Player::move_right, player.get()});
        kb_controller->add_command({(int)Input::Keys::Space, KeyFlag::Press | KeyFlag::Held}, {Player::jump, player.get()});
        kb_controller->add_command({(int)Input::Keys::Enter, KeyFlag::Press}, {World::save, nullptr});
        kb_controller->add_command({(int)Input::Keys::Escape, KeyFlag::Press}, {InGameMenu::toggle_command, nullptr});
        kb_controller->add_command({(int)Input::Keys::Num1, KeyFlag::Press}, {Inventory::set_selection, SelData{0}});
        kb_controller->add_command({(int)Input::Keys::Num2, KeyFlag::Press}, {Inventory::set_selection, SelData{1}});
        kb_controller->add_command({(int)Input::Keys::Num3, KeyFlag::Press}, {Inventory::set_selection, SelData{2}});
        kb_controller->add_command({(int)Input::Keys::Num4, KeyFlag::Press}, {Inventory::set_selection, SelData{3}});
        kb_controller->add_command({(int)Input::Keys::Num5, KeyFlag::Press}, {Inventory::set_selection, SelData{4}});
        kb_controller->add_command({(int)Input::Keys::Num6, KeyFlag::Press}, {Inventory::set_selection, SelData{5}});
        kb_controller->add_command({(int)Input::Keys::Num7, KeyFlag::Press}, {Inventory::set_selection, SelData{6}});
        kb_controller->add_command({(int)Input::Keys::Num8, KeyFlag::Press}, {Inventory::set_selection, SelData{7}});
        kb_controller->add_command({(int)Input::Keys::Num9, KeyFlag::Press}, {Inventory::set_selection, SelData{8}});

        mb_controller = new Utilities::Input::MouseController();
        mb_controller->add_command({(int)Input::MouseButtons::Left, KeyFlag::Press}, {on_action_left, player.get()});
        mb_controller->add_command({(int)Input::MouseButtons::Right, KeyFlag::Press}, {on_action_right, player.get()});
        mb_controller->add_command({(int)Input::MouseButtons::MWheelUp, KeyFlag::Press}, {Inventory::decrement_selection, nullptr});
        mb_controller->add_command({(int)Input::MouseButtons::MWheelDown, KeyFlag::Press}, {Inventory::increment_selection, nullptr});

        Input::add_controller(kb_controller);
        Input::add_controller(mb_controller);
        Input::set_differential_mode("Mouse", true);
        Input::set_cursor_center();

        Rendering::RenderContext::get().set_mode_3D();

        world = create_refptr<World>();

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

    void GameState::setup_2d_rendering() {
        Rendering::RenderContext::get().set_mode_2D();
        Rendering::RenderContext::get().matrix_ortho(0, 480, 0, 272, -10, 10);
        GI::clearDepth();
        GI::disable(GI_CULL_FACE);
    }

    void GameState::setup_3d_rendering() {
        GI::enable(GI_CULL_FACE);
        Rendering::RenderContext::get().set_mode_3D();
    }

    void GameState::on_draw(Core::Application* app, double dt) {
        world->draw();

        setup_2d_rendering();

        player->draw(dt);

        setup_3d_rendering();
    }

    void GameState::on_cleanup() {
        CC_Core_Term();
    }

} // namespace CrossCraft