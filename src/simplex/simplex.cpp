#include "simplex/simplex.h"

#include <limits>

std::size_t Tableau::findPivotColumn() const {
	std::size_t best = -1;
	double bestContrib = 0;

	for (std::size_t i = 0; i < contribution.size(); i++) {
		double cur = objective[i] - contribution[i];
		if (cur > bestContrib) {
			best = i;
			bestContrib = cur;
		}
	}

	return best;
}

std::size_t Tableau::findPivotRow(std::size_t col) const {
	std::size_t best = -1;
	double minRatio = std::numeric_limits<double>::max();

	for (std::size_t i = 0; i < rhs.size(); i++) {
		double cur = rhs[i] / matrix[col, i];
		if (cur < minRatio) {
			best = i;
			minRatio = cur;
		}
	}

	return best;
}

void Tableau::pivot(std::size_t col, std::size_t row) {
	// Variable at pivot column becomes basic, the one in the pivot row becomes non-basic.
	basis[row] = col;

	// Scale row such that [col, row] is 1.
	matrix.scaleRow(row, 1.0 / matrix[col, row]);
	rhs[row] /= matrix[col, row];

	// Make other values in pivot column 0.
	for (std::size_t r = 0; r < rhs.size(); r++) {
		if (r == row)
			continue;

		double scale = matrix[col, r] / matrix[col, row];
		matrix.addScaledRow(r, row, -scale);
		rhs[r] -= rhs[row] * scale;
	}

	// Update contribution row.
	for (std::size_t c = 0; c < contribution.size(); c++) {
		contribution[c] = 0;
		for (std::size_t r = 0; r < rhs.size(); r++)
			contribution[c] += objective[basis[r]] * matrix[c, r];
	}
}
