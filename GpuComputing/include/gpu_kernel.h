#pragma once

#ifdef API_EXPORT

#define GPU_API_EXPORT __declspec(dllexport)

#else

#define GPU_API_EXPORT __declspec(dllimport)

#endif

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "matrix.h"

#ifdef __cplusplus
extern "C"
{
#endif

GPU_API_EXPORT void GPU_test(PerfComparison::Matrix<float> const& A, PerfComparison::Matrix<float> const& B, PerfComparison::Matrix<float> &C, float& ComputingTime);
GPU_API_EXPORT void DisplayMatrix(float const*, size_t,char*);
	
#ifdef __cplusplus
}
#endif
