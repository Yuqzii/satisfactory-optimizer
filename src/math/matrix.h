#pragma once

#include <cstddef>
#include <vector>

namespace math {

class Matrix {
public:
	Matrix(std::size_t rows, std::size_t cols) : rows{rows}, cols{cols}, data(rows * cols) {}

	void scaleRow(std::size_t r, double x);

	double& operator[](std::size_t r, std::size_t c) {
		return data[r * cols + c];
	}

	double operator[](std::size_t r, std::size_t c) const {
		return data[r * cols + c];
	}

private:
	std::size_t rows = 0;
	std::size_t cols = 0;
	std::vector<double> data;
};

}  // namespace math
