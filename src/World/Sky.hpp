#pragma once
#include <Rendering/RenderContext.hpp>
#include <Rendering/Mesh.hpp>

namespace CrossCraft
{
using namespace Stardust_Celeste;

class Sky {
    public:
	Sky();

	void generate(Rendering::Color color, Rendering::Color color2);
	void draw(double dt, mathfu::Vector<float, 3> cam_pos, uint64_t tick);

    private:
	uint32_t cloud_id;
	RefPtr<Rendering::Mesh<Rendering::Vertex> > mesh, mesh2, clouds;
	uint64_t last_tick{};
	double scroll{};
	void generateMesh(const Rendering::Color &color,
			  RefPtr<Rendering::Mesh<Rendering::Vertex>> mesh, bool useScroll = false);
};
}