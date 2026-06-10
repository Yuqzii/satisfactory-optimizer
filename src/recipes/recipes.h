#pragma once

#include <map>
#include <string>
#include <vector>

struct RecipeItem {
	int amount;
	std::string name;
};

struct Recipe {
	double duration;
	std::vector<RecipeItem> ingredients;
	std::vector<RecipeItem> products;
};

std::map<std::string, Recipe> getRecipes(std::string filePath);