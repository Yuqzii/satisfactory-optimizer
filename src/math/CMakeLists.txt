add_library(math STATIC
	matrix.cpp
)

target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
