#include "recipes.h"

#include <algorithm>
#include <filesystem>
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

void identifyItems(std::string& stringVal, std::vector<RecipeItem>& list) {
	static const std::string factoryGameStr = "FactoryGame/";
	static const std::string amountStr = "Amount=";

	std::size_t factoryGameIndx = stringVal.find(factoryGameStr);
	std::size_t amountIndx = stringVal.find(amountStr);

	while (factoryGameIndx != std::string::npos && amountIndx != std::string::npos) {
		const std::size_t dotIndx = stringVal.find('.', factoryGameIndx);
		const std::size_t fileSlash = stringVal.rfind('/', dotIndx);
		const std::size_t folderSlash = stringVal.rfind('/', fileSlash - 1);

		const std::string itemName = stringVal.substr(folderSlash + 1, fileSlash - folderSlash - 1);
		const int amount = std::stoi(stringVal.substr(amountIndx + amountStr.length()));

		list.push_back({amount, itemName});

		// update indexes
		factoryGameIndx = stringVal.find(factoryGameStr, factoryGameIndx + factoryGameStr.length());
		amountIndx = stringVal.find(amountStr, amountIndx + amountStr.length());
	}
}

std::map<std::string, Recipe> getRecipes(std::string ans) {
	std::map<std::string, Recipe> parsedRecipes;

	// open the .json file
	std::filesystem::path jsonFilePath(ans);
	std::ifstream file(jsonFilePath);
	if (!file.is_open()) {
		std::cerr << "failed to find or open " << jsonFilePath.filename() << '\n' << std::endl;
		return parsedRecipes;
	} else {
		std::cerr << jsonFilePath.filename() << "opened successfully\n" << std::endl;
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