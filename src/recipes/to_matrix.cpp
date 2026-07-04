#include "to_matrix.h"

#include <algorithm>
#include <iterator>
#include <vector>

namespace {

std::vector<std::size_t> itemsToIdx(
    const std::span<const RecipeItem> items, ItemManager& itemManager
) {
	std::vector<std::size_t> res;
	res.reserve(items.size());

	// Get the item indices of all items.
	std::transform(
	    items.begin(), items.end(), std::back_inserter(res),
	    [&itemManager](const RecipeItem& item) { return itemManager[item.name]; }
	);

	return res;
}

}  // namespace

math::Matrix recipesToMatrix(const std::span<const Recipe> recipes, ItemManager& itemManager) {
	// Get all the indices of items first, such that we know the total amount of items.
	std::vector<std::vector<std::size_t>> ingredientIdx, productIdx;
	ingredientIdx.reserve(recipes.size());
	productIdx.reserve(recipes.size());

	for (const auto& recipe : recipes) {
		auto curIngredients = itemsToIdx(recipe.ingredients, itemManager);
		ingredientIdx.push_back(std::move(curIngredients));

		auto curProducts = itemsToIdx(recipe.products, itemManager);
		productIdx.push_back(std::move(curProducts));
	}


	math::Matrix matrix{itemManager.size(), recipes.size()};
	for (std::size_t i = 0; i < recipes.size(); i++) {
		for (std::size_t j = 0; j < recipes[i].ingredients.size(); j++) {
			const double cur =
			    static_cast<double>(recipes[i].ingredients[j].amount) / recipes[i].duration;
			matrix[ingredientIdx[i][j], i] -= cur; // Negative for consumption.
		}

		for (std::size_t j = 0; j < recipes[i].products.size(); j++) {
			const double cur =
			    static_cast<double>(recipes[i].products[j].amount) / recipes[i].duration;
			matrix[productIdx[i][j], i] += cur; // Positive for production.
		}
	}

	return matrix;
}

std::size_t ItemManager::operator[](const std::string_view key) {
	auto it = data.lower_bound(key);

	if (it == data.end() || it->first != key) {
		// Key does not exist in map.
		it = data.emplace_hint(it, key, data.size());
	}

	return it->second;
}
