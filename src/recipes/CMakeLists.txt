add_library(recipes STATIC
	recipes.cpp
)

find_package(nlohmann_json 3.12.0 QUIET)

if (NOT nlohmann_json_FOUND)
	message(STATUS "nlohmann/json not found locally, fetching from GitHub")

	include(FetchContent)
	FetchContent_Declare(
		json
		URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
	)
	FetchContent_MakeAvailable(json)
endif()

target_include_directories(recipes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(recipes PRIVATE 
	nlohmann_json::nlohmann_json
)
