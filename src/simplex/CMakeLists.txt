add_library(simplex STATIC
	simplex.cpp
)

target_include_directories(simplex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
