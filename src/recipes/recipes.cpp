#include "recipes/recipes.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
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

	for (int i = 0; i < docs.size(); i++) {
		if (!docs[i].contains("NativeClass"))
			continue;
		if (docs[i]["NativeClass"] != "/Script/CoreUObject.Class'/Script/FactoryGame.FGRecipe'")
			continue;
	}
}