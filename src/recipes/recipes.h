#pragma once

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

void identifyItem(std::string& k, std::string& v, std::vector<RecipeItem>& list);