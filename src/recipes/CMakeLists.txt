add_library(recipes STATIC
	recipes.cpp
)

find_package(nlohmann_json 3.12.0 REQUIRED)

target_include_directories(recipes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(recipes PRIVATE 
	nlohmann_json::nlohmann_json
)