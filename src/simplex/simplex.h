#pragma once

#include <vector>

#include "math/matrix.h"

class Tableau {
public:
	Tableau(std::size_t cols, std::size_t rows, const std::vector<double>& objective)
	    : objective{objective}, rhs(rows), contribution(cols), basis(rows), matrix{cols, rows} {}

private:
	std::vector<double> objective;
	std::vector<double> rhs;
	std::vector<double> contribution;
	std::vector<std::size_t> basis;
	math::Matrix matrix;

	// @return The variable with the maximum objective - contribution. -1 if none are positive.
	std::size_t findPivotColumn() const;
	// @param col The pivot column.
	// @return The row with the minimum rhs / matrix value.
	std::size_t findPivotRow(std::size_t col) const;

	void pivot(std::size_t col, std::size_t row);
};
