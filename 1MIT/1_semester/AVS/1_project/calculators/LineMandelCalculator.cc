/**
 * @file LineMandelCalculator.cc
 * @author Erika Do <xdoeri00@stud.fit.vutbr.cz>
 * @brief Implementation of Mandelbrot calculator that uses SIMD paralelization over lines
 * @date 2024-11-08
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <mm_malloc.h>

#include "LineMandelCalculator.h"


LineMandelCalculator::LineMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "LineMandelCalculator")
{
	data = (int *)_mm_malloc(height * width * sizeof(int), 64);
	zReal = (float *)_mm_malloc(width * sizeof(float), 64);
	zImag = (float *)_mm_malloc(width * sizeof(float), 64);

	const int half_height = height / 2;
	for (int i = 0; i < half_height; i++) {
		#pragma omp simd aligned(data: 64)
		for (int j = 0; j < width; j++) {
			data[i * width + j] = limit;
		}
	}
}

LineMandelCalculator::~LineMandelCalculator() {
	_mm_free(data);
	_mm_free(zReal);
	_mm_free(zImag);
}

int * LineMandelCalculator::calculateMandelbrot () {
	int *pdata = data;
	float *pzReal = zReal;
	float *pzImag = zImag;

	const float x_start = this->x_start;
	const float y_start = this->y_start;
	const float dx = this->dx;
	const float dy = this->dy;
	const int half_height = height / 2;

	for (int i = 0; i < half_height; i++) {
		float y = y_start + i * dy; // current imaginary value

		#pragma omp simd aligned(pzReal, pzImag: 64) // initializes the pzReal and pzImag array
		for (int j = 0; j < width; j++) {
			pzReal[j] = x_start + j * dx;
			pzImag[j] = y;
		}

		int escaped = 1;
		for (int k = 0; k < limit && escaped; ++k) {
			escaped = 0;
			#pragma omp simd aligned(pdata, pzReal, pzImag: 64) reduction(+: escaped) simdlen(64)
            for (int j = 0; j < width; j++) {
                float r2 = pzReal[j] * pzReal[j];
                float i2 = pzImag[j] * pzImag[j];

                pzImag[j] = 2.0f * pzReal[j] * pzImag[j] + y; 
                pzReal[j] = r2 - i2 + (x_start + j * dx);

				int pos = i * width + j;
				int limit_reached = pdata[pos] == limit;

				if (r2 + i2 > 4.0f && limit_reached) {
					pdata[pos] = k;
				}
				
				escaped += limit_reached;
            }
		}
	}

	for (int i = 0; i < half_height; i++) { // copies upper part of matrix to the corresponding positions in the lower half
		#pragma omp simd aligned(pdata: 64)
		for (int j = 0; j < width; j++) {
			pdata[(height - i - 1) * width + j] = pdata[i * width + j];
		}
	}

	return data;
}
