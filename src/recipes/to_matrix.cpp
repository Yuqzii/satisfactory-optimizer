#include "to_matrix.h"

#include <algorithm>
#include <iterator>
#include <vector>

math::Matrix recipesToMatrix(const std::vector<Recipe>& recipes, ItemManager& itemManager) {
	std::vector<std::vector<double>> matrix;

	for (const auto& recipe : recipes) {
		auto initFunc = [&itemManager](const std::vector<RecipeItem>& vec) {
			std::vector<std::size_t> temp;
			temp.reserve(vec.size());

			// Get the item indices of all items in vec.
			std::transform(
			    vec.begin(), vec.end(), std::back_inserter(temp),
			    [&itemManager](const RecipeItem& item) { return itemManager[item.name]; }
			);

			return temp;
		};

		// Get all the indices of items first, such that we don't need to resize the newRow vector
		// because we already know the maximum item index.
		const std::vector<std::size_t> ingredientIdx = initFunc(recipe.ingredients);
		const std::vector<std::size_t> productIdx = initFunc(recipe.products);

		std::vector<double> newRow(itemManager.size());
		for (std::size_t i = 0; i < recipe.ingredients.size(); i++) {
			double res = static_cast<double>(recipe.ingredients[i].amount) / recipe.duration;
			newRow[ingredientIdx[i]] = -res;  // Negative because it consumes ingredients.
		}
		for (std::size_t i = 0; i < recipe.products.size(); i++) {
			double res = static_cast<double>(recipe.products[i].amount) / recipe.duration;
			newRow[productIdx[i]] = res;  // Positive because it produces the products.
		}

		matrix.push_back(std::move(newRow));
	}

	return math::Matrix{matrix};
}

std::size_t ItemManager::operator[](const std::string_view key) {
	auto it = data.lower_bound(key);

	if (it == data.end() || it->first != key) {
		// Key does not exist in map.
		it = data.emplace_hint(it, key, data.size());
	}

	return it->second;
}
