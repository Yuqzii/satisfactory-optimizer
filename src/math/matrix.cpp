#include "math/matrix.h"

namespace math {

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
