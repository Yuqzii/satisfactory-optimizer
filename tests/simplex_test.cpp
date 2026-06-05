#include "simplex/simplex.h"

#include <gtest/gtest.h>

#include "math/matrix.h"

namespace simplex {

class TableauTest : public ::testing::Test {};

TEST_F(TableauTest, FindPivotColumn) {
	std::vector<double> objective = {7.0, 6.0, 0, 0};
	math::Matrix matrix{{
	    {0, 8.0 / 3.0, 1.0, -2.0 / 3.0},
	    {1.0, 2.0 / 3.0, 0, 1.0 / 3.0},
	}};
	std::vector<double> rhs(matrix.getRows());

	Tableau tableau{std::move(objective), std::move(rhs), std::move(matrix)};
	tableau.basis[1] = 0;
	tableau.calculateContribution();

	std::size_t res = tableau.findPivotColumn().value();
	EXPECT_EQ(res, 1);
}

}  // namespace simplex
