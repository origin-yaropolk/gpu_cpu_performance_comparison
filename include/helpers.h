#pragma once

#include "matrix.h"
#include <iostream>
#include <memory>

#define UNUSED_NAME(name) (void)name

namespace Helpers
{

inline void waitInput()
{
	std::cin.get();
}

template <typename Tp>
std::unique_ptr<Tp[]>
convertToPointerToMatrix(PerfComparison::Matrix<Tp> const& matrix)
{
	std::unique_ptr<Tp[]> pointerToMatrix{ new Tp[matrix.rows() * matrix.columns()] };

	for (size_t i = 0; i < matrix.rows(); ++i)
	{
		for (size_t j = 0; j < matrix.columns(); ++j)
		{
			pointerToMatrix.get()[i * matrix.rows() + j] = matrix[i][j];
		}
	}

	return pointerToMatrix;
}

}
