#include "math/matrix.h"

#include <cassert>

namespace math {

Matrix::Matrix(const std::vector<std::vector<double>>& matrix)
    : rows{matrix.size()}, cols{(!matrix.empty()) ? matrix[0].size() : 0}, data(rows * cols) {
	// All rows must have same amount of columns.
	for (std::size_t i = 1; i < matrix.size(); i++)
		assert(matrix[i].size() == cols);

	// Flatten 2D matrix param to 1D vector.
	for (std::size_t r = 0; r < rows; r++)
		for (std::size_t c = 0; c < cols; c++)
			data[r * cols + c] = matrix[r][c];
}

void Matrix::scaleRow(std::size_t r, double x) {
	const std::size_t rowStart = r * cols;
	for (std::size_t c = 0; c < cols; c++)
		data[rowStart + c] *= x;
}

void Matrix::addScaledRow(std::size_t targetRow, std::size_t sourceRow, double x) {
	const std::size_t targetRowStart = targetRow * cols;
	const std::size_t sourceRowStart = sourceRow * cols;

	for (std::size_t c = 0; c < cols; c++)
		data[targetRowStart + c] += data[sourceRowStart + c] * x;
}

}  // namespace math
