#pragma once

#include <Graphics/2D/FixedFontRenderer.hpp>

using namespace Stardust_Celeste;

// creds to https://minecraft.fandom.com/wiki/Formatting_codes
#define CC_TEXT_COLOR_BLACK 0
#define CC_TEXT_COLOR_DARK_BLUE 1
#define CC_TEXT_COLOR_DARK_GREEN 2
#define CC_TEXT_COLOR_DARK_AQUA 3
#define CC_TEXT_COLOR_DARK_RED 4
#define CC_TEXT_COLOR_DARK_PURPLE 5
#define CC_TEXT_COLOR_GOLD 6
#define CC_TEXT_COLOR_GRAY 7
#define CC_TEXT_COLOR_DARK_GRAY 8
#define CC_TEXT_COLOR_BLUE 9
#define CC_TEXT_COLOR_GREEN 10
#define CC_TEXT_COLOR_AQUA 11
#define CC_TEXT_COLOR_RED 12
#define CC_TEXT_COLOR_LIGHT_PURPLE 13
#define CC_TEXT_COLOR_YELLOW 14
#define CC_TEXT_COLOR_WHITE 15
#define CC_TEXT_COLOR_BE_MTX_GOLD 16
#define CC_TEXT_COLOR_SELECT 17
#define CC_TEXT_COLOR_SPLASH 18

#define CC_TEXT_ALIGN_LEFT 0
#define CC_TEXT_ALIGN_CENTER 1
#define CC_TEXT_ALIGN_RIGHT 2

namespace CrossCraft
{
class FontRender {
    public:
	FontRender();

	~FontRender();

	[[maybe_unused]] void draw_text(uint8_t color, const std::string &text,
					const mathfu::Vector<float, 2> &pos,
					float layer);

	void draw_text_aligned(uint8_t color, uint8_t align,
			       const std::string &text,
			       mathfu::Vector<float, 2> pos, float layer);

	void clear();

	void build();

	void draw();

	void finalize_draw();

    private:
	static void get_color(uint8_t color, Rendering::Color &fg,
			      Rendering::Color &bg);

	ScopePtr<Graphics::G2D::FontRenderer> fontRenderer;
};
}