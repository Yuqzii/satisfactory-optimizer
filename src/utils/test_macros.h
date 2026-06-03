#pragma once

#ifndef FRIEND_TEST
#	ifdef BUILD_TESTING
#		define FRIEND_TEST(test_case_name, test_name) \
			friend class test_case_name##_##test_name##_Test
#	else
#		define FRIEND_TEST(test_case_name, test_name)
#	endif
#endif
