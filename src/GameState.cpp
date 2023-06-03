#include <GameState.hpp>
#include <CC/core.h>

namespace CrossCraft {

void GameState::on_start() {
    Utilities::Input::set_differential_mode("Mouse", true);
    Utilities::Input::set_cursor_center();

    CC_Core_Init();

    // Send initial player position.
    CC_Event_Push_PlayerUpdate(PLAYER_SELF, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, false);

    CC_Core_Update(0.0);

    player = create_refptr<Player>();
}

void GameState::on_update(Core::Application* app, double dt) {
    player->update(dt);

    CC_Core_Update(dt);
}

void GameState::on_draw(Core::Application* app, double dt) {
    player->draw(dt);
}

void GameState::on_cleanup() {
    CC_Core_Term();
}

} // namespace CrossCraft