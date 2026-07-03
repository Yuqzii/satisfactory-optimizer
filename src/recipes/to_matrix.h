#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <string_view>

class ItemManager {
public:
	std::size_t operator[](std::string_view key);

	auto begin() {
		return data.begin();
	}
	auto end() {
		return data.end();
	}

private:
	std::map<std::string, std::size_t, std::less<>> data;
};
