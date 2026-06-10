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

		extractRecipes(parsedRecipes, docs, i);
	}

	return parsedRecipes;
}

void extractRecipes(std::map<std::string, Recipe>& parsedRecipes, json& docs, int i) {
	auto recipeArr = docs[i]["Classes"];
	for (std::size_t j = 0; j < recipeArr.size(); j++) {
		std::string recipeName;
		std::vector<RecipeItem> input;
		std::vector<RecipeItem> output;
		double manDur = 0.0;

		for (auto it = recipeArr[j].begin(); it != recipeArr[j].end(); ++it) {
			const std::string jsonKey = it.key();
			const std::string jsonValue = it.value().get<std::string>();
			static const std::unordered_set<std::string> AllowedKeys{
			    "mDisplayName", "mIngredients", "mProduct", "mManufactoringDuration"};
			if (!AllowedKeys.count(jsonKey))
				continue;

			if (jsonKey == "mDisplayName") {
				recipeName = jsonValue;
			} else if (jsonKey == "mManufactoringDuration") {
				manDur = std::stod(jsonValue);
			} else if (jsonKey == "mIngredients") {
				identifyItems(jsonValue, input);
			} else if (jsonKey == "mProduct") {
				identifyItems(jsonValue, output);
			}
		}

		parsedRecipes[recipeName] = {manDur, input, output};
	}
}

void identifyItems(const std::string& stringVal, std::vector<RecipeItem>& list) {
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