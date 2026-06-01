#pragma once

#include <vector>

#include "math/matrix.h"

class Tableau {
public:
	Tableau(std::size_t cols, std::size_t rows, const std::vector<double>& objective)
	    : objective{objective}, rhs(cols), contribution(rows), matrix{cols, rows} {}

private:
	std::vector<double> objective;
	std::vector<double> rhs;
	std::vector<double> contribution;
	math::Matrix matrix;

	// @return The variable with the maximum objective - contribution. -1 if none are positive.
	std::size_t findPivotColumn() const;
};
