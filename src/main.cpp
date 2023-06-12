#include <Stardust-Celeste.hpp>
#include <GameState.hpp>

#ifdef _WIN32
extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 1;
    __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 1;
}
#endif

using namespace Stardust_Celeste;

class GameApplication : public Core::Application {
public:
    void on_start() override {
        // Create new Game State
        auto state = create_refptr<CrossCraft::GameState>();
        // Set to our state
        this->set_state(state);

        // Set the background color
        Rendering::Color color{};
        color.color = 0xFFFFFFFF;

        Rendering::RenderContext::get().vsync = false;
        Rendering::RenderContext::get().set_color(color);
    }
};


Core::Application *CreateNewSCApp() {
    // Configure the engine
    Core::AppConfig config;
    config.headless = false;
    config.networking = true;
    config.render_settings.title = "CrossCraft Indev";
    config.render_settings.width = 960;
    config.render_settings.height = 544;
    config.render_settings.renderingApi = OpenGL;

    Core::PlatformLayer::get().initialize(config);

    // Return the game
    return new GameApplication();
}