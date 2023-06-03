#include <GameState.hpp>
#include <CC/core.h>

namespace CrossCraft {

void GameState::on_start() {
    CC_Core_Init();

    // Send initial player position.
    CC_Event_Push_PlayerUpdate(-1, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, false);
}

void GameState::on_update(Core::Application* app, double dt) {
    CC_Core_Update(dt);
}

void GameState::on_draw(Core::Application* app, double dt) {
}

void GameState::on_cleanup() {
    CC_Core_Term();
}

} // namespace CrossCraft