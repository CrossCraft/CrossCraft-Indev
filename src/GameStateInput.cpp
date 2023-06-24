#include <GameState.hpp>
#include <Utilities/Controllers/KeyboardController.hpp>
#include <Utilities/Controllers/MouseController.hpp>
#include <Utilities/Controllers/PSPController.hpp>
#include <Player/InGameMenu.hpp>
#include <Player/Inventory.hpp>

namespace CrossCraft{

    void GameState::setup_input() {
        using namespace Stardust_Celeste::Utilities;

        // Setup controls
        kb_controller = new Utilities::Input::KeyboardController();

        kb_controller->add_command({(int) Input::Keys::W, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::S, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::A, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::D, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::Space, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::LShift, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        kb_controller->add_command({(int) Input::Keys::W, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_forward, player.get()});
        kb_controller->add_command({(int) Input::Keys::S, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_backward, player.get()});
        kb_controller->add_command({(int) Input::Keys::A, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_left, player.get()});
        kb_controller->add_command({(int) Input::Keys::D, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_right, player.get()});
        kb_controller->add_command({(int) Input::Keys::Space, KeyFlag::Press | KeyFlag::Held},
                                   {Player::jump, player.get()});
        kb_controller->add_command({(int) Input::Keys::Enter, KeyFlag::Press}, {World::save, nullptr});
        kb_controller->add_command({(int) Input::Keys::Escape, KeyFlag::Press}, {InGameMenu::toggle_command, nullptr});
        kb_controller->add_command({(int) Input::Keys::Num1, KeyFlag::Press}, {Inventory::set_selection, SelData{0}});
        kb_controller->add_command({(int) Input::Keys::Num2, KeyFlag::Press}, {Inventory::set_selection, SelData{1}});
        kb_controller->add_command({(int) Input::Keys::Num3, KeyFlag::Press}, {Inventory::set_selection, SelData{2}});
        kb_controller->add_command({(int) Input::Keys::Num4, KeyFlag::Press}, {Inventory::set_selection, SelData{3}});
        kb_controller->add_command({(int) Input::Keys::Num5, KeyFlag::Press}, {Inventory::set_selection, SelData{4}});
        kb_controller->add_command({(int) Input::Keys::Num6, KeyFlag::Press}, {Inventory::set_selection, SelData{5}});
        kb_controller->add_command({(int) Input::Keys::Num7, KeyFlag::Press}, {Inventory::set_selection, SelData{6}});
        kb_controller->add_command({(int) Input::Keys::Num8, KeyFlag::Press}, {Inventory::set_selection, SelData{7}});
        kb_controller->add_command({(int) Input::Keys::Num9, KeyFlag::Press}, {Inventory::set_selection, SelData{8}});
        kb_controller->add_command({(int) Input::Keys::Q, KeyFlag::Press}, {Inventory::drop_selection, player.get()});
        kb_controller->add_command({(int) Input::Keys::E, KeyFlag::Press}, {Inventory::toggle_inventory, player.get()});
        kb_controller->add_command({(int) Input::Keys::LShift, KeyFlag::Press | KeyFlag::Held}, {Player::sneak_sink, player.get()});
        kb_controller->add_command({(int) Input::Keys::Backslash, KeyFlag::Press}, {Player::toggle_freecam, player.get()});

        mb_controller = new Utilities::Input::MouseController();
        mb_controller->add_command({(int) Input::MouseButtons::Left, KeyFlag::Release | KeyFlag::Untouched},
                                   {on_action_left_up, player.get()});
        mb_controller->add_command({(int) Input::MouseButtons::Left, KeyFlag::Press | KeyFlag::Held},
                                   {on_action_left, player.get()});
        mb_controller->add_command({(int) Input::MouseButtons::Right, KeyFlag::Press}, {on_action_right, player.get()});
        mb_controller->add_command({(int) Input::MouseButtons::MWheelUp, KeyFlag::Press},
                                   {Inventory::decrement_selection, nullptr});
        mb_controller->add_command({(int) Input::MouseButtons::MWheelDown, KeyFlag::Press},
                                   {Inventory::increment_selection, nullptr});

        psp_controller = new Utilities::Input::PSPController();
        psp_controller->add_command({(int) Input::PSPButtons::Triangle, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Cross, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Square, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Circle, KeyFlag::Release},
                                   {Player::move_release, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Triangle, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_forward, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Cross, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_backward, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Square, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_left, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Circle, KeyFlag::Press | KeyFlag::Held},
                                   {Player::move_right, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::LTrigger, KeyFlag::Release | KeyFlag::Untouched},
                                   {on_action_left_up, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::LTrigger, KeyFlag::Press | KeyFlag::Held},
                                   {on_action_left, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::RTrigger, KeyFlag::Press}, {on_action_right, player.get()});
        psp_controller->add_command({(int) Input::PSPButtons::Left, KeyFlag::Press},
                                   {Inventory::decrement_selection, nullptr});
        psp_controller->add_command({(int) Input::PSPButtons::Right, KeyFlag::Press},
                                   {Inventory::increment_selection, nullptr});
        psp_controller->add_command({(int) Input::PSPButtons::Select, KeyFlag::Press | KeyFlag::Held},
                                   {Player::jump, player.get()});

        Input::add_controller(kb_controller);
        Input::add_controller(mb_controller);
        Input::add_controller(psp_controller);
        Input::set_differential_mode("Mouse", true);
        Input::set_differential_mode("PSP", true);
        Input::set_cursor_center();
    }
}