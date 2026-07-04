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
	std::vector<std::vector<double>> columns;

	for (const auto& recipe : recipes) {
		// Get all the indices of items first, such that we don't need to resize the newRow vector
		// because we already know the maximum item index.
		const std::vector<std::size_t> ingredientIdx = itemsToIdx(recipe.ingredients, itemManager);
		const std::vector<std::size_t> productIdx = itemsToIdx(recipe.products, itemManager);

		std::vector<double> newCol(itemManager.size());
		for (std::size_t i = 0; i < recipe.ingredients.size(); i++) {
			double res = static_cast<double>(recipe.ingredients[i].amount) / recipe.duration;
			newCol[ingredientIdx[i]] -= res;  // Negative because it consumes ingredients.
		}
		for (std::size_t i = 0; i < recipe.products.size(); i++) {
			double res = static_cast<double>(recipe.products[i].amount) / recipe.duration;
			newCol[productIdx[i]] += res;  // Positive because it produces the products.
		}

		columns.push_back(std::move(newCol));
	}

	math::Matrix matrix{itemManager.size(), columns.size()};
	for (std::size_t c = 0; c < columns.size(); c++)
		for (std::size_t r = 0; r < columns[c].size(); r++)
			matrix[r, c] = columns[c][r];

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
