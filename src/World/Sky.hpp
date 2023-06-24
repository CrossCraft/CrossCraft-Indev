#pragma once
#include <Rendering/RenderContext.hpp>
#include <Rendering/Mesh.hpp>

namespace CrossCraft {
    using namespace Stardust_Celeste;

    class Sky {
    public:
        Sky();

        void generate(Rendering::Color color, Rendering::Color color2);
        void draw(mathfu::Vector<float, 3> cam_pos, uint64_t tick);
    private:
        ScopePtr<Rendering::Mesh<Rendering::Vertex>> mesh, mesh2;
        uint64_t last_tick;
    };
}