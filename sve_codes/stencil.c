#include <stdlib.h>
typedef float real_t;
//float real_t;
typedef real_t* arr_t;
typedef unsigned long long index_t;
typedef index_t* arr_index_t;
// #define VERSION 1 // Select version 
#define MAX_TIME 1000
#if VERSION == 0
void stencil_1d(arr_t A, const int nx) {   
    for(int timestep = 0; timestep < MAX_TIME; ++timestep)
        for(int i = 1; i < nx - 1; ++i)
            A[i] = (A[i - 1] + A[i] + A[i + 1]) / 3.0;
}
#elif VERSION == 1
void stencil_1d(arr_t A, const int nx) {   
    for(int timestep = 0; timestep < MAX_TIME; ++timestep)
        for(int i = 0; i < nx - 2; ++i)
            A[i] = (A[i] + A[i + 1] + A[i + 2]) / 3.0;
}
#endif

int main() {
  int sz = 1024;
  arr_t A = (arr_t) malloc(sz * sizeof(real_t));
  //arr_t A = new real_t[sz];
  stencil_1d(A, sz);
  free(A);
  //delete[] A; 
  return 0;
}
