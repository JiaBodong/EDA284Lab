#include<arm_sve.h>
#include <stdlib.h>
#include<stdio.h>
#include<time.h>
typedef float real_t;
typedef real_t* arr_t;
// #define VERSION 2
#if VERSION == 0
arr_t matmul_basic(const arr_t A, const arr_t B, const int M, const int N, const int K) {
    arr_t C = (arr_t) malloc(M * N * sizeof(real_t)); 
    for( int m = 0; m < M; ++m ) {
        for( int n = 0; n < N; ++n ) {
            for( int k = 0; k < K; ++k ) {
                C[ m * M + n ] += A[ m * M + k ] * B[ k * K + n ];
            }
        }
    }
  return C;
}
#elif VERSION == 1
arr_t matmul_basic(const arr_t A, const arr_t B, const int M, const int N, const int K) {
    arr_t C = (arr_t) malloc(M * N * sizeof(real_t)); 
    for( int m = 0; m < M; ++m ) {
        for( int k = 0; k < K; ++k ) {
            real_t _a = A[m * M + k];
            for( int n = 0; n < N; ++n ) {
                C[ m * M + n ] += _a * B[ k * K + n ];
            }
        }
    }
  return C;
}
#elif VERSION == 2
arr_t matmul_basic(const arr_t A, const arr_t B, const int M, const int N, const int K) {
    arr_t C = (arr_t) malloc(M * N * sizeof(real_t)); 
    svfloat32_t vc;
    for( int n = 0; n < N; n+=svcntw() ) {//svcntw means vector length
    	svbool_t pg = svwhilelt_b32(n,N);
        for( int m = 0; m < M; ++m ) {
		    svfloat32_t c_vec = svld1(pg, &C[m * M + n]);  //load C matrix
        	for( int k = 0; k < K; ++k ) 
		    {
                real_t a = A[m * M + k];  // A matrix element
                svfloat32_t a_vec = svdup_f32(a);
			    svfloat32_t b_vec = svld1(pg, &B[k * K + n]);  //load B matrix, stencil flow

			    vc =  svmla_m(pg, c_vec,a_vec, b_vec);  // Multiply-addition (Vector, Vector, Scalar)
                //C[ m * M + n ] += A[m * M + K] * B[ k * K + n ];
            }
	        svst1(pg, &C[m * M + n], vc);  // strore the result in C matrix
        }
    }
    return C;
}
#endif

int main() { 
    const int N = 64;
    arr_t A = (arr_t) malloc(N * N * sizeof(real_t)); 
    arr_t B = (arr_t) malloc(N * N * sizeof(real_t)); 
    clock_t start = clock();
    arr_t C = matmul_basic(A, B, N, N, N);
    clock_t end = clock();
    printf("time is",((double)(end - start))/CLOCKS_PER_SEC);
    return 0;
}

