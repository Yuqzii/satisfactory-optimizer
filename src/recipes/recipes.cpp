#include "recipes/recipes.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using json = nlohmann::json;

std::map<std::string, Recipe> getRecipes() {
	std::map<std::string, Recipe> parsedRecipes;

	// open sfDocs.json file
	std::ifstream file("../../sfDocs.json");
	if (!file.is_open()) {
		std::cerr << "Failed to open sfDocs.json" << std::endl;
		return parsedRecipes;
	}

	json docs;
	try {
		docs = json::parse(file);
	} catch (json::parse_error& e) {
		std::cerr << ".json parse error: " << e.what() << std::endl;
		return parsedRecipes;
	}
	file.close();


	// iterate through sfDocs.json
	for (int i = 0; i < docs.size(); i++) {
		if (!docs[i].contains("NativeClass"))
			continue;
		if (docs[i]["NativeClass"] != "/Script/CoreUObject.Class'/Script/FactoryGame.FGRecipe'")
			continue;

		// iterate through alle recipes
		auto recipeArr = docs[i]["Classes"];
		for (int j = 0; j < recipeArr.size(); j++) {
			std::string recipeName;
			std::vector<RecipeItem> input;
			std::vector<RecipeItem> output;
			double manDur;

			for (auto it = recipeArr[j].begin(); it != recipeArr[j].end(); ++it) {
				auto k = it.key();
				auto v = it.value();
				static const std::unordered_set<std::string> AllowedKeys{
				    "mDisplayName", "mIngredients", "mProduct", "mManufactoringDuration"};
				if (!AllowedKeys.count(k))
					continue;

				if (k == "mDisplayName") {
					recipeName = v;
				} else if (k == "mManufactoringDuration") {
					std::string manDurStr = v;
					manDur = std::stoi(manDurStr);
				} else if (k == "mIngredients") {
					// parse name(s) of ingredient(s)
					// parse amount of the ingredient(s)
				} else if (k == "mProduct") {
					// parse name(s) of ouput(s)
					// parse amount of the output(s)
				}
			}
			Recipe currentRecipe;
			currentRecipe.ingredients = input;
			currentRecipe.products = output;
			currentRecipe.duration = manDur;
			parsedRecipes[recipeName] = std::move(currentRecipe);
		}
		break;
	}
}