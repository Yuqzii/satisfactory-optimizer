#pragma once

#include <vector>
#include <string>

struct RecipeItem {
	int amount;
	std::string name;
};

struct Recipe {
	double duration;
	std::vector<RecipeItem> ingredients;
	std::vector<RecipeItem> products;
};