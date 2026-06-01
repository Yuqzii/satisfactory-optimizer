#include "simplex/simplex.h"

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
