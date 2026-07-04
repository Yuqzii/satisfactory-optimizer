#pragma once

#include <cstddef>
#include <map>
#include <span>
#include <string>
#include <string_view>

#include "math/matrix.h"
#include "recipes/recipes.h"

class ItemManager {
public:
	std::size_t operator[](std::string_view key);

	auto begin() {
		return data.begin();
	}
	auto end() {
		return data.end();
	}
	std::size_t size() const {
		return data.size();
	}

private:
	std::map<std::string, std::size_t, std::less<>> data;
};

math::Matrix recipesToMatrix(std::span<const Recipe> recipes, ItemManager& itemManager);
