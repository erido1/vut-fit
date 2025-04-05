/**
 * @file BatchMandelCalculator.cc
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

#include "BatchMandelCalculator.h"


BatchMandelCalculator::BatchMandelCalculator (unsigned matrixBaseSize, unsigned limit) :
	BaseMandelCalculator(matrixBaseSize, limit, "BatchMandelCalculator")
{
	block_size = 64;
	data = (int *)_mm_malloc(height * width * sizeof(int), 64);
	zReal = (float *)_mm_malloc(block_size * sizeof(float), 64);
	zImag = (float *)_mm_malloc(block_size * sizeof(float), 64);

	const int half_height = height / 2;
	for (int i = 0; i < half_height; i++) {
		#pragma omp simd aligned(data: 64)
		for (int j = 0; j < width; j++) {
			data[i * width + j] = limit;
		}
	}
}

BatchMandelCalculator::~BatchMandelCalculator() {
	_mm_free(data);
	_mm_free(zReal);
	_mm_free(zImag);
}

int * BatchMandelCalculator::calculateMandelbrot () {
	int *pdata = data;
	float *pzReal = zReal;
	float *pzImag = zImag;

	const float x_start = this->x_start;
	const float y_start = this->y_start;
	const float dx = this->dx;
	const float dy = this->dy;
	const int half_height = height / 2;
	const float ratio = width / block_size;

	for (int i = 0; i < half_height; i++) {
		float y = y_start + i * dy; // current imaginary value

		for (int block = 0; block < ratio; block++) {
			int offset = block * block_size;

			#pragma omp simd aligned(pzReal, pzImag: 64) // initializes the pzReal and pzImag array
			for (int j = 0; j < block_size; j++) {
				pzReal[j] = x_start + (j + offset) * dx;
				pzImag[j] = y;
			}

			int escaped = block_size;
			for (int k = 0; k < limit && escaped; ++k) {
				escaped = 0;
				#pragma omp simd aligned(pdata, pzReal, pzImag: 64) reduction(+: escaped) simdlen(64)
				for (int j = 0; j < block_size; j++) {
					float r2 = pzReal[j] * pzReal[j];
					float i2 = pzImag[j] * pzImag[j];

					pzImag[j] = 2.0f * pzReal[j] * pzImag[j] + y; 
					pzReal[j] = r2 - i2 + (x_start + (j + offset) * dx);

					int pos = i * width + j + offset;
					int limit_reached = pdata[pos] == limit;

					if (r2 + i2 > 4.0f && limit_reached) {
						pdata[pos] = k;
					} 

					escaped += limit_reached;
				}
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
