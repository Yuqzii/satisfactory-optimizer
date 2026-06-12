#include <iostream>
#include <string>
#include <vector>

#include "recipes/recipes.h"

int main() {
	std::string path;
	std::cout << "Enter absolute file path for Docs.json or en-US.json:" << std::endl;
	std::cin >> path;
	std::vector<Recipe> recipes = getRecipes(path);

	return 0;
}
