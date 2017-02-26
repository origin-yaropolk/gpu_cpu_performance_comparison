#pragma once

#ifdef API_EXPORT

#define GPU_API_EXPORT __declspec(dllexport)

#else

#define GPU_API_EXPORT __declspec(dllimport)

#endif

#include "matrix.h"

#ifdef __cplusplus
extern "C"
{
#endif

GPU_API_EXPORT void GPU_test(PerfComparison::Matrix<float> const& A, PerfComparison::Matrix<float> const& B, PerfComparison::Matrix<float> &C, float& ComputingTime);
GPU_API_EXPORT void DisplayMatrix(double const*, size_t,char*);
	
#ifdef __cplusplus
}
#endif
