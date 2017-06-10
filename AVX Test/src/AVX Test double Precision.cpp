//============================================================================
// Name        : AVX.cpp
// Author      : rumi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <emmintrin.h> // for _m128
#include <immintrin.h> // for _m256
#include <math.h>
#include <chrono>

// compile making sure you have support for AVX2 : compiler option -mavx2
void normal(double* a, int N) {
	for (int i = 0; i < N; ++i)
		a[i] = sqrt(a[i]);
}

void sse128(double* a, int N) {
	// We assume N % 4 == 0.
	int nb_iters = N / 2;
	__m128* ptr = (__m128*)a;

	for (int i = 0; i < nb_iters; ++i, ++ptr, a += 2)
		_mm_store_pd(a, _mm_sqrt_pd(*ptr));
}
void sse256(double* a, int N) {
	// We assume N % 4 == 0.
	int nb_iters = N / 4;
	__m256* ptr = (__m256*)a;

	for (int i = 0; i < nb_iters; ++i, ++ptr, a += 4)
		_mm256_store_pd(a, _mm256_sqrt_pd(*ptr));
}

int main(int argc, char** argv) {
	if (argc != 2)
		return 1;
	int N = atoi(argv[1]);
	cout << "!!!Hello World OF Advanced Vector Extensions : Double Precision!!!"
			<< endl; // prints !!!Hello World!!!
	double* a;

	auto alloc_start = chrono::high_resolution_clock::now();

	posix_memalign((void**) &a, 32, N * sizeof(double));
	auto alloc_end = chrono::high_resolution_clock::now();

	for (int i = 0; i < N; ++i)
		a[i] = 3141592.65358;
	auto normal_start = chrono::high_resolution_clock::now();
	clock_t c_normal_start = clock();
	{

		normal(a, N);

	}
	auto normal_end = chrono::high_resolution_clock::now();
	clock_t c_normal_end = clock();

	auto init_start = chrono::high_resolution_clock::now();

	for (int i = 0; i < N; ++i)
		a[i] = 3141592.65358;

	auto init_end = chrono::high_resolution_clock::now();

	auto sse128_start = chrono::high_resolution_clock::now();
	clock_t c_sse128_start = clock();

	{
		sse128(a, N);

	}
	auto sse128_end = chrono::high_resolution_clock::now();
	clock_t c_sse128_end = clock();

	auto sse256_start = chrono::high_resolution_clock::now();
	clock_t c_sse256_start = clock();

	{
		sse256(a, N);

	}
	auto sse256_end = chrono::high_resolution_clock::now();
	clock_t c_sse256_end = clock();

	printf("Effective Threads Runtime\n");
	printf("%8.5f\tseconds normal\n",
			(double) (c_normal_end - c_normal_start) / CLOCKS_PER_SEC);
	printf("%8.5f\tseconds sse128\n",
			(double) (c_sse128_end - c_sse128_start) / CLOCKS_PER_SEC);
	printf("%8.5f\tseconds sse256\n",
			(double) (c_sse256_end - c_sse256_start) / CLOCKS_PER_SEC);

	printf("Effective clock Runtime\n");
	/*
	 printf("Memory %lld.%lld s\n", chrono::duration_cast<chrono::seconds>(alloc_end - alloc_start).count(),
	 chrono::duration_cast<chrono::microseconds>(alloc_end - alloc_start).count());
	 printf("Init   %lld.%lld s\n", chrono::duration_cast<chrono::seconds>(init_end - init_start).count(),
	 chrono::duration_cast<chrono::microseconds>(init_end - init_start).count());

	 */
	printf("Normal %lld.%lld s\n",
			chrono::duration_cast<chrono::seconds>(normal_end - normal_start).count(),
			chrono::duration_cast<chrono::microseconds>(
					normal_end - normal_start).count());

	printf("SSE128 %lld.%lld s\n",
			chrono::duration_cast<chrono::seconds>(sse128_end - sse128_start).count(),
			chrono::duration_cast<chrono::microseconds>(
					sse128_end - sse128_start).count());

	printf("SSE256 %lld.%lld s\n",
			chrono::duration_cast<chrono::seconds>(sse256_end - sse256_start).count(),
			chrono::duration_cast<chrono::microseconds>(
					sse256_end - sse256_start).count());

	/*  printf("total  %lld.%lld s\n", chrono::duration_cast<chrono::seconds>(sse256_end - alloc_start).count(),
	 chrono::duration_cast<chrono::microseconds>(sse256_end - alloc_start).count());
	 */
}

