cmake_minimum_required(VERSION 3.22)

project(satisfactory-optimizer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src/math)
add_subdirectory(src/simplex)
add_subdirectory(src/recipes)
add_subdirectory(src/utils)

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE
	math
	recipes
)

option(BUILD_TESTING "Build the test suite" OFF)
if (BUILD_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif()
