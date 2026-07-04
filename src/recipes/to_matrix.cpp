#include "to_matrix.h"

#include <algorithm>
#include <iterator>
#include <vector>

namespace {

template <std::output_iterator<std::size_t> OutputIt>
void itemsToIdx(const std::span<const RecipeItem> items, ItemManager& itemManager, OutputIt out) {
	// Get the item indices of all items.
	std::transform(items.begin(), items.end(), out, [&itemManager](const RecipeItem& item) {
		return itemManager[item.name];
	});
}

}  // namespace

math::Matrix recipesToMatrix(const std::span<const Recipe> recipes, ItemManager& itemManager) {
	// Get all the indices of items first, such that we know the total amount of items.
	std::vector<std::size_t> ingredientIdx, ingredientOffset, productIdx, productOffset;
	ingredientOffset.reserve(recipes.size());
	productOffset.reserve(recipes.size());

	for (const auto& recipe : recipes) {
		ingredientOffset.push_back(ingredientIdx.size());
		productOffset.push_back(productIdx.size());
		itemsToIdx(recipe.ingredients, itemManager, std::back_inserter(ingredientIdx));
		itemsToIdx(recipe.products, itemManager, std::back_inserter(productIdx));
	}

	math::Matrix matrix{itemManager.size(), recipes.size()};
	for (std::size_t i = 0; i < recipes.size(); i++) {
		for (std::size_t j = 0; j < recipes[i].ingredients.size(); j++) {
			const double cur =
			    static_cast<double>(recipes[i].ingredients[j].amount) / recipes[i].duration;
			const std::size_t idx = ingredientIdx[ingredientOffset[i] + j];
			matrix[idx, i] -= cur;  // Negative for consumption.
		}

		for (std::size_t j = 0; j < recipes[i].products.size(); j++) {
			const double cur =
			    static_cast<double>(recipes[i].products[j].amount) / recipes[i].duration;
			const std::size_t idx = productIdx[productOffset[i] + j];
			matrix[idx, i] += cur;  // Positive for production.
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
