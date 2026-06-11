#pragma once

#include <map>
#include <string>
#include <vector>

struct RecipeItem {
	int amount;
	std::string name;

	RecipeItem(int a, std::string n) : amount{a}, name{n} {};
};

struct Recipe {
	double duration;
	std::vector<RecipeItem> ingredients;
	std::vector<RecipeItem> products;

	Recipe(double dur, std::vector<RecipeItem> ingr, std::vector<RecipeItem> prod)
	    : duration{dur}, ingredients{ingr}, products{prod} {};
};

std::vector<Recipe> getRecipes(std::string& filePath);