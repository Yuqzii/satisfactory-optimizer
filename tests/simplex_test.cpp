#include "simplex/simplex.h"

#include <gtest/gtest.h>

#include <optional>

#include "gtest/gtest.h"
#include "math/matrix.h"

namespace simplex {

struct TableauPivotTestCase {
	std::size_t expectedRow;
	std::optional<std::size_t> expectedCol;

	std::string name;

	std::vector<double> objective;
	std::vector<double> rhs;
	std::vector<std::size_t> basis;
	math::Matrix matrix;
};

class TableauPivotTest : public ::testing::TestWithParam<TableauPivotTestCase> {};

TEST_P(TableauPivotTest, FindPivot) {
	const auto& testCase = GetParam();

	Tableau tableau{
	    std::move(testCase.objective),
	    std::move(testCase.rhs),
	    std::move(testCase.matrix),
	};

	if (!testCase.basis.empty()) {
		tableau.basis = std::move(testCase.basis);
		tableau.calculateContribution();
	}

	const auto col = tableau.findPivotColumn();
	ASSERT_EQ(testCase.expectedCol.has_value(), col.has_value())
	    << "Wrongly claims optimality / non-optimality";

	if (!col.has_value()) {
		SUCCEED();
		return;
	}

	ASSERT_EQ(testCase.expectedCol.value(), col.value());

	const std::size_t row = tableau.findPivotRow(col.value());
	EXPECT_EQ(testCase.expectedRow, row);
}

INSTANTIATE_TEST_SUITE_P(
    Tableau, TableauPivotTest,
    ::testing::Values(
        TableauPivotTestCase{
            .expectedRow = 1,
            .expectedCol = 0,
            .name = "InitialState",
            .objective{7, 6, 0, 0},
            .rhs{16, 12},
            .matrix{{
                {2, 4, 1, 0},
                {3, 2, 0, 1},
            }},
        }
    ),
    [](const ::testing::TestParamInfo<TableauPivotTestCase>& info) { return info.param.name; }
);

}  // namespace simplex
