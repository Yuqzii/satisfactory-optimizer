#pragma once

#ifdef BUILD_TESTING
#	include <gtest/gtest_prod.h>
#else
#	ifndef FRIEND_TEST
#		define FRIEND_TEST(test_case_name, test_name)
#	endif
#endif
