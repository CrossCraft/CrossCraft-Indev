#include <GameState.hpp>
#include <CC/core.h>

namespace CrossCraft {

void GameState::on_start() {
    CC_Core_Init();
}

void GameState::on_update(Core::Application* app, double dt) {
}

void GameState::on_draw(Core::Application* app, double dt) {
}

void GameState::on_cleanup() {
    CC_Core_Term();
}

} // namespace CrossCraft