#pragma once

#include "matrix.h"

#ifdef __cplusplus
extern "C"
{
#endif

void GPU_test(
	PerfComparison::Matrix<float> const& A, 
	PerfComparison::Matrix<float> const& B, 
	PerfComparison::Matrix<float> &C, float& ComputingTime
);

void DisplayMatrix(double const*, size_t, char*);
	
#ifdef __cplusplus
}
#endif
