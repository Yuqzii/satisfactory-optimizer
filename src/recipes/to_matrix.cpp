#include "to_matrix.h"

std::size_t ItemManager::operator[](const std::string_view key) {
	auto it = data.lower_bound(key);

	if (it == data.end() || it->first != key) {
		// Key does not exist in map.
		it = data.emplace_hint(it, key, data.size());
	}

	return it->second;
}
