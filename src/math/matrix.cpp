#include "math/matrix.h"

namespace math {

void Matrix::scaleRow(std::size_t r, double x) {
	const std::size_t rowStart = r * cols;
	for (std::size_t c = 0; c < cols; c++)
		data[rowStart + c] *= x;
}

}  // namespace math
