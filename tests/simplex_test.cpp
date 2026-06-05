#include "simplex/simplex.h"

#include <gtest/gtest.h>

#include <optional>

#include "gtest/gtest.h"
#include "math/matrix.h"

namespace simplex {

struct TableauPivotTestCase {
	std::optional<std::size_t> expectedRow;
	std::optional<std::size_t> expectedCol;

	std::string name;

	std::vector<double> objective;
	std::vector<double> rhs;
	std::vector<std::size_t> basis;
	math::Matrix matrix;
};

void PrintTo(const TableauPivotTestCase& testCase, std::ostream* os) {
	const auto toStr = [](std::size_t a) -> std::optional<std::string> {
		return std::to_string(a);
	};

	const auto col = testCase.expectedCol.and_then(toStr).value_or("none");
	const auto row = testCase.expectedRow.and_then(toStr).value_or("none");

	*os << '"' << testCase.name << "\" (Expected Row: " << row << ", Col: " << col << ')';
}

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

	const auto row = tableau.findPivotRow(col.value());
	ASSERT_EQ(testCase.expectedRow.has_value(), row.has_value());

	if (!row.has_value()) {
		SUCCEED();
		return;
	}

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
        },
        TableauPivotTestCase{
            .expectedRow = 0,
            .expectedCol = 1,
            .name = "AfterOneIteration",
            .objective{7, 6, 0, 0},
            .rhs{8, 4},
            .basis{2, 0},
            .matrix{{
                {0, 8.0 / 3.0, 1, -2.0 / 3.0},
                {1, 2.0 / 3.0, 0, 1.0 / 3.0},
            }},
        },
        TableauPivotTestCase{
            .expectedRow = 0,
            .expectedCol = 0,
            .name = "IgnoreNegAndZeroFindingRow",
            .objective{5.0, 2.0, 0, 0, 0},
            .rhs{4, 6, 5},
            .matrix{{
                {2.0, 1.0, 1.0, 0.0, 0.0},
                {-1.0, 3.0, 0.0, 1.0, 0.0},
                {0.0, 2.0, 0.0, 0.0, 1.0},
            }},
        },
        TableauPivotTestCase{
            .expectedRow = std::nullopt,
            .expectedCol = 1,
            .name = "NoRowWhenNoPositive",
            .objective{2.0, 5.0, 0, 0, 0},
            .rhs{4, 6, 5},
            .matrix{{
                {2.0, -1.0, 1.0, 0.0, 0.0},
                {-1.0, -3.0, 0.0, 1.0, 0.0},
                {0.0, -2.0, 0.0, 0.0, 1.0},
            }},
        }
    ),
    [](const ::testing::TestParamInfo<TableauPivotTestCase>& info) { return info.param.name; }
);

}  // namespace simplex
