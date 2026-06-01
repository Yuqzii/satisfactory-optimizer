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
