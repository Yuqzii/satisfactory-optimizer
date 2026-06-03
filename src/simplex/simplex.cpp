#include "simplex/simplex.h"

#include <limits>
#include <numeric>

namespace simplex {

Tableau::Tableau(const std::vector<double>& objective, const std::vector<double>& rhs,
                 const math::Matrix& constraints)
    : objective{objective},
      rhs{rhs},
      contribution(constraints.getCols()),
      basis(constraints.getRows()),
      matrix{constraints} {
	assert(objective.size() == matrix.getCols());
	assert(rhs.size() == matrix.getRows());

	// Set all the slack variables to be basic.
	std::iota(basis.begin(), basis.end(), matrix.getCols() - matrix.getRows());

	calculateContribution();
}

void Tableau::optimize() {
	while (const auto pivotCol = findPivotColumn()) {
		const std::size_t pivotRow = findPivotRow(*pivotCol);

		pivot(*pivotCol, pivotRow);
	}
}

std::optional<std::size_t> Tableau::findPivotColumn() const {
	std::size_t best = -1;
	double bestContrib = 0;

	for (std::size_t i = 0; i < contribution.size(); i++) {
		double cur = objective[i] - contribution[i];
		if (cur > bestContrib) {
			best = i;
			bestContrib = cur;
		}
	}

	if (best == -1)
		return std::nullopt;

	return best;
}

std::size_t Tableau::findPivotRow(std::size_t col) const {
	std::size_t best = -1;
	double minRatio = std::numeric_limits<double>::max();

	for (std::size_t i = 0; i < rhs.size(); i++) {
		double cur = rhs[i] / matrix[i, col];
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
	matrix.scaleRow(row, 1.0 / matrix[row, col]);
	rhs[row] /= matrix[row, col];

	// Make other values in pivot column 0.
	for (std::size_t r = 0; r < rhs.size(); r++) {
		if (r == row)
			continue;

		double scale = matrix[r, col] / matrix[row, col];
		matrix.addScaledRow(r, row, -scale);
		rhs[r] -= rhs[row] * scale;
	}

	calculateContribution();
}

void Tableau::calculateContribution() {
	for (std::size_t c = 0; c < contribution.size(); c++) {
		contribution[c] = 0;
		for (std::size_t r = 0; r < rhs.size(); r++)
			contribution[c] += objective[basis[r]] * matrix[r, c];
	}
}

}  // namespace simplex
