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
	std::map<std::string, Recipe> extractedRecipes;

	// open sfDocs.json file
	std::ifstream file("../../sfDocs.json");
	if (!file.is_open()) {
		std::cerr << "Failed to open sfDocs.json" << std::endl;
		return extractedRecipes;
	}

	json docs;
	try {
		docs = json::parse(file);
	} catch (json::parse_error& e) {
		std::cerr << ".json parse error: " << e.what() << std::endl;
		return extractedRecipes;
	}
	file.close();
}