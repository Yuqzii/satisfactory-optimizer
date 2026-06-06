#include "recipes.h"

#include <algorithm>
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

void identifyItem(std::string& k, std::string& v, std::vector<RecipeItem>& list) {
	std::string mainStr = v;
	std::string factoryGameStr = "FactoryGame/";
	std::string amountStr = "Amount=";

	std::size_t factoryGameIndx = mainStr.find(factoryGameStr);
	std::size_t amountIndx = mainStr.find(amountStr);

	while (factoryGameIndx != std::string::npos && amountIndx != std::string::npos) {
		std::size_t dotIndx = mainStr.find('.', factoryGameIndx);
		std::size_t fileSlash = mainStr.rfind('/', dotIndx);
		std::size_t folderSlash = mainStr.rfind('/', fileSlash - 1);

		std::string itemName = mainStr.substr(folderSlash + 1, fileSlash - folderSlash - 1);
		int amount = std::stoi(mainStr.substr(amountIndx + amountStr.length()));

		RecipeItem currItem;
		currItem.name = itemName;
		currItem.amount = amount;
		list.push_back(currItem);

		// update indexes
		factoryGameIndx = mainStr.find(factoryGameStr, factoryGameIndx + factoryGameStr.length());
		amountIndx = mainStr.find(amountStr, amountIndx + amountStr.length());
	}
}

std::map<std::string, Recipe> getRecipes() {
	std::map<std::string, Recipe> parsedRecipes;

	// open sfData.json file
	std::ifstream file("../sfData.json");
	if (!file.is_open()) {
		std::cerr << "Failed to open sfData.json" << std::endl;
		return parsedRecipes;
	} else {
		std::cerr << "sfData.json opened successfully" << std::endl;
	}

	json docs;
	try {
		docs = json::parse(file);
	} catch (json::parse_error& e) {
		std::cerr << ".json parse error: " << e.what() << std::endl;
		return parsedRecipes;
	}
	file.close();


	// iterate through sfData.json
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
			double manDur = 0.0;

			for (auto it = recipeArr[j].begin(); it != recipeArr[j].end(); ++it) {
				std::string k = it.key();
				std::string v = it.value().get<std::string>();
				static const std::unordered_set<std::string> AllowedKeys{
				    "mDisplayName", "mIngredients", "mProduct", "mManufactoringDuration"};
				if (!AllowedKeys.count(k))
					continue;

				if (k == "mDisplayName") {
					v.erase(std::remove(v.begin(), v.end(), ' '), v.end());
					recipeName = v;
				} else if (k == "mManufactoringDuration") {
					std::string manDurStr = v;
					manDur = std::stod(manDurStr);
				} else if (k == "mIngredients") {
					identifyItem(k, v, input);
				} else if (k == "mProduct") {
					identifyItem(k, v, output);
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
	return parsedRecipes;
}