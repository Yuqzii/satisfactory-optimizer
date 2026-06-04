#include "simplex/simplex.h"

#include <gtest/gtest.h>

namespace simplex {

class TableauTest : public ::testing::Test {};

TEST_F(TableauTest, FindPivotColumn) {
	Tableau tableau{{}, {}, {{}}};
}

}  // namespace simplex
