add_library(simplex STATIC
	simplex.cpp
)

target_include_directories(simplex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(simplex PRIVATE
	math
)

if (BUILD_TESTING)
	target_compile_definitions(simplex PRIVATE BUILD_TESTING)
endif()
