#include "math/matrix.h"

#include "gtest/gtest.h"

constexpr double eps = 1e-9;

TEST(MatrixTest, ScaleRow) {
	math::Matrix m{{
	    {1.0, 2.5, 3.0},
	    {3.0, 6.0, 9.0},
	    {1.5, 6.7, 4.2},
	}};

	std::vector<std::vector<double>> expected = {
	    {2.0, 5.0, 6.0},
	    {1.0, 2.0, 3.0},
	    {0.75, 3.35, 2.1},
	};

	m.scaleRow(0, 2.0);
	m.scaleRow(1, 1.0 / 3.0);
	m.scaleRow(2, 0.5);

	for (std::size_t r = 0; r < m.getRows(); r++)
		for (std::size_t c = 0; c < m.getCols(); c++)
			EXPECT_NEAR(expected[r][c], (m[r, c]), eps);
}
