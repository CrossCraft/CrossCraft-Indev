#include <Player/Crafting.hpp>
#include <vector>
#include <algorithm>
#include "Utilities/Logger.hpp"
namespace CrossCraft::Crafting
{

auto find_match(std::array<item_t, 4> ingredients, Recipe2x2 recipe, Recipe2x2& retrieved) -> bool {
	if(recipe.type == RecipeType::Shapeless) {
		// In shapeless recipes, it's just a list to match any order
		// So we just sort both arrays and compare them
		std::sort(ingredients.begin(), ingredients.end());
		std::sort(recipe.ingredients.begin(), recipe.ingredients.end());

		if (ingredients == recipe.ingredients) {
			retrieved = recipe;
			return true;
		}
	} else if (recipe.type == RecipeType::Shaped) {
		// In shaped recipes, we have to match the shape
		// So we just compare the first 4 elements
		bool flag = true;
		for(int i = 0; i < 4; i++) {
			if(ingredients[i] != recipe.ingredients[i]) {
				flag = false;
				break;
			}
		}

		if(flag) {
			retrieved = recipe;
			return true;
		}
	}

	return false;
};

auto get_recipe_array(std::array<item_t, 4> ingredients, Recipe2x2& retrieved) -> bool {
	static std::vector<Recipe2x2> recipe_map = {
		{RecipeType::Shapeless, Shape2x2::Shapeless, {BLK_Logs, 0, 0, 0}, {BLK_Wood, 4}}, // Logs -> Wood
		{RecipeType::Shaped, Shape2x2::Full, {BLK_Wood, BLK_Wood, BLK_Wood, BLK_Wood}, {BLK_Crafting_Table, 1}}, // Wood -> Crafting Table
		{RecipeType::Shaped, Shape2x2::Horizontal, {BLK_Wood, 0, BLK_Wood, 0}, {ITM_Stick, 4}}, // Wood -> Sticks
	};

	for (auto& recipe : recipe_map) {
		if(find_match(ingredients, recipe, retrieved)) {
			return true;
		}
	}

	return false;
}

}