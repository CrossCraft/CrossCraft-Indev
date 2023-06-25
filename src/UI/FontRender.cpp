#include <UI/FontRender.hpp>
#include <ResourcePack.hpp>

#define CC_TEXT_COLOR_SELECT_FRONT \
	{{                         \
		255, 255, 160, 255 \
	}}
#define CC_TEXT_COLOR_SELECT_BACK \
	{{                        \
		63, 63, 40, 255   \
	}}
#define CC_TEXT_COLOR_SPLASH_FRONT \
	{{                         \
		63, 63, 0, 255     \
	}}
#define CC_TEXT_COLOR_SPLASH_BACK \
	{{                        \
		255, 255, 0, 255  \
	}}

namespace CrossCraft
{

FontRender::FontRender()
{
	fontRenderer = create_scopeptr<Graphics::G2D::FontRenderer>(
		ResourcePack::get().get_texture("default"),
		mathfu::Vector<float, 2>(16, 16));
	fontRenderer->scale_factor = 1.0f;
}

FontRender::~FontRender() = default;

void FontRender::draw()
{
	fontRenderer->draw();
}

void FontRender::get_color(uint8_t col, Rendering::Color &front,
			   Rendering::Color &back)
{
	static const std::map<uint8_t,
			      std::pair<Rendering::Color, Rendering::Color> >
		colors = { { CC_TEXT_COLOR_BLACK,
			     { { {0, 0, 0, 255} }, { {0, 0, 0, 255} } } },
			   { CC_TEXT_COLOR_DARK_BLUE,
			     { { {0, 0, 170, 255} }, { {0, 0, 42, 255} } } },
			   { CC_TEXT_COLOR_DARK_GREEN,
			     { { {0, 170, 0, 255} }, { {0, 42, 0, 255} } } },
			   { CC_TEXT_COLOR_DARK_AQUA,
			     { { {0, 170, 170, 255} }, { {0, 42, 42, 255} } } },
			   { CC_TEXT_COLOR_DARK_RED,
			     { { {170, 0, 0, 255} }, { {42, 0, 0, 255} } } },
			   { CC_TEXT_COLOR_DARK_PURPLE,
			     { { {170, 0, 170, 255} }, { {42, 0, 42, 255} } } },
			   { CC_TEXT_COLOR_GOLD,
			     { { {255, 170, 0, 255} }, { {42, 42, 0, 255} } } },
			   { CC_TEXT_COLOR_GRAY,
			     { { {170, 170, 170, 255} }, { {42, 42, 42, 255} } } },
			   { CC_TEXT_COLOR_DARK_GRAY,
			     { { {85, 85, 85, 255} }, { {21, 21, 21, 255} } } },
			   { CC_TEXT_COLOR_BLUE,
			     { { {85, 85, 255, 255} }, { {21, 21, 63, 255} } } },
			   { CC_TEXT_COLOR_GREEN,
			     { { {85, 255, 85, 255} }, { {21, 63, 21, 255} } } },
			   { CC_TEXT_COLOR_AQUA,
			     { { {85, 255, 255, 255} }, { {21, 63, 63, 255} } } },
			   { CC_TEXT_COLOR_RED,
			     { { {255, 85, 85, 255} }, { {63, 21, 21, 255} } } },
			   { CC_TEXT_COLOR_LIGHT_PURPLE,
			     { { {255, 85, 255, 255} }, { {63, 21, 63, 255} } } },
			   { CC_TEXT_COLOR_YELLOW,
			     { { {255, 255, 85, 255} }, { {63, 63, 21, 255} } } },
			   { CC_TEXT_COLOR_BE_MTX_GOLD,
			     { { {221, 214, 5, 255} }, { {55, 53, 1, 255} } } },
			   { CC_TEXT_COLOR_SELECT,
			     { CC_TEXT_COLOR_SELECT_FRONT,
			       CC_TEXT_COLOR_SELECT_BACK } },
			   { CC_TEXT_COLOR_SPLASH,
			     { CC_TEXT_COLOR_SPLASH_FRONT,
			       CC_TEXT_COLOR_SPLASH_BACK } } };

	auto it = colors.find(col);
	if (it != colors.end()) {
		front = it->second.first;
		back = it->second.second;
	} else {
		// default color (white)
		front = { {255, 255, 255, 255} };
		back = { {63, 63, 63, 255} };
	}

	// Apply alpha value
	front.rgba.a = 255;
	back.rgba.a = 255;
}

[[maybe_unused]] void FontRender::draw_text(uint8_t color,
					    const std::string &text,
					    const mathfu::Vector<float, 2> &pos,
					    float layer)
{
	Rendering::Color front{}, back{};
	get_color(color, front, back);

	fontRenderer->add_text(text, pos, front, layer);
	fontRenderer->add_text(text,
			       pos - mathfu::Vector<float, 2>{ -1.0f, 1.0f },
			       back, layer - 1.0f);
}

void FontRender::draw_text_aligned(uint8_t color, uint8_t align,
				   const std::string &text,
				   mathfu::Vector<float, 2> pos, float layer)
{
	Rendering::Color front{}, back{};
	get_color(color, front, back);

	float width = fontRenderer->calculate_size(text);

	if (align == CC_TEXT_ALIGN_CENTER)
		pos.x -= width / 2.0f;
	else if (align == CC_TEXT_ALIGN_RIGHT)
		pos.x -= width;

	fontRenderer->add_text(text, pos, front, layer);
	fontRenderer->add_text(text,
			       pos - mathfu::Vector<float, 2>{ -1.0f, 1.0f },
			       back, layer - 1.0f);
}

void FontRender::clear()
{
	fontRenderer->clear_tiles();
}

void FontRender::build()
{
	fontRenderer->generate_map();
}

void FontRender::finalize_draw()
{
	build();
	draw();
}
}