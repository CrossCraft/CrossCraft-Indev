#pragma once
#include <array>
#include <CC/core.h>

namespace CrossCraft::Crafting
{

enum class RecipeType {
	Shaped,
	Shapeless
};

enum class Shape2x2 {
	Vertical,
	Horizontal,
	Diagonal,
	Full,
	Shapeless
};

struct ResultItem {
	item_t item;
	int count;
};

struct Recipe2x2 {
    RecipeType type;
    Shape2x2 shape;
    std::array<item_t, 4> ingredients;
    ResultItem result;
};

auto get_recipe_array(std::array<item_t, 4> ingredients, Recipe2x2& retrieved) -> bool;

}