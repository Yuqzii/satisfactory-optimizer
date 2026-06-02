include (FetchContent)

FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG v1.17.0
)

# For Windows compatibility
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(tests
	# Test files here
)

target_link_libraries(tests PRIVATE
	gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
