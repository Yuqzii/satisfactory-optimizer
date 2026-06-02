#include "math/matrix.h"

#include "gtest/gtest.h"

constexpr double eps = 1e-9;

TEST(MatrixTest, ScaleRow) {
	math::Matrix m{{
	    {1.0, 2.5, 3.0},
	    {3.0, 6.0, 9.0},
	    {1.5, 6.7, 4.2},
	}};

	m.scaleRow(0, 2.0);
	std::array<double, 3> expect{{2.0, 5.0, 6.0}};
	for (std::size_t i = 0; i < m.getCols(); i++)
		EXPECT_NEAR(expect[i], (m[0, i]), eps);

	m.scaleRow(1, 1.0 / 3.0);
	expect = {1.0, 2.0, 3.0};
	for (std::size_t i = 0; i < m.getCols(); i++)
		EXPECT_NEAR(expect[i], (m[1, i]), eps);

	m.scaleRow(2, 0.5);
	expect = {0.75, 3.35, 2.1};
	for (std::size_t i = 0; i < m.getCols(); i++)
		EXPECT_NEAR(expect[i], (m[2, i]), eps);
}
