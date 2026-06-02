#pragma once

#include <numeric>
#include <optional>
#include <vector>

#include "math/matrix.h"

class Tableau {
public:
	Tableau(const std::vector<double>& objective, const std::vector<double>& rhs,
	        const math::Matrix& constraints)
	    : objective{objective},
	      rhs{rhs},
	      contribution(constraints.getCols()),
	      basis(constraints.getRows()),
	      matrix{constraints} {
		// Set all the slack variables to be basic.
		std::iota(basis.begin(), basis.end(), matrix.getCols() - matrix.getRows());

		calculateContribution();
	}

	// Runs the simplex algorithm and stops when it finds an optimum.
	// The state of the tableau after running this is the optimal state.
	void optimize();

private:
	std::vector<double> objective;
	std::vector<double> rhs;
	std::vector<double> contribution;
	std::vector<std::size_t> basis;
	math::Matrix matrix;

	// @return The variable with the maximum objective - contribution. Nothing if none are positive.
	std::optional<std::size_t> findPivotColumn() const;
	// @param col The pivot column.
	// @return The row with the minimum rhs / matrix value.
	std::size_t findPivotRow(std::size_t col) const;

	void pivot(std::size_t col, std::size_t row);

	void calculateContribution();
};
