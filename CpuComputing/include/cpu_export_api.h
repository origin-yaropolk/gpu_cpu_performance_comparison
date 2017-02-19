#pragma once

#ifdef DLLAPI_EXPORT

#define CPU_API_EXPORT __declspec(dllexport)

#else

#define CPU_API_EXPORT __declspec(dllimport)

#endif

#include "matrix.h"

CPU_API_EXPORT bool multiply_matrices(
	PerfComparison::Matrix<double> const& first,
	PerfComparison::Matrix<double> const& second,
	PerfComparison::Matrix<double>& result
);

CPU_API_EXPORT void cpu_api_init();
CPU_API_EXPORT void cpu_api_destroy();