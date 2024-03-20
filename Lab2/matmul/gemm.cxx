#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <iostream>
#include <omp.h>
using namespace std;


#if defined(SINGLE)
typedef float type_t;
#else
typedef double type_t;
#endif
int tile_size = 64;
void print ( const vector <type_t>& m, int n_rows, int n_columns ) {
    
    /*  "Couts" the input vector as n_rows x n_columns matrix.
     Inputs:
     m: vector, matrix of size n_rows x n_columns
     n_rows: int, number of rows in the left matrix m1
     n_columns: int, number of columns in the left matrix m1
     */
    
    for( int i = 0; i != n_rows; ++i ) {
        for( int j = 0; j != n_columns; ++j ) {
            cout << m[ i * n_columns + j ] << " ";
        }
        cout << '\n';
    }
    cout << endl;
}

int argmax ( const vector <type_t>& m ) {
    return distance(m.begin(), max_element(m.begin(), m.end()));
}

vector<type_t> random_vector(const int size)
{
    /*  Generates a random vector with uniform distribution
     Inputs:
     size: the vector size
     */
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distribution(0.01, 0.95);
    static default_random_engine generator;

    vector<type_t> data(size);
    generate(data.begin(), data.end(), [&]() { return distribution(generator); });
    return data;
}

vector <type_t> operator+(const vector <type_t>& m1, const vector <type_t>& m2){
    
    /*  Returns the elementwise sum of two vectors.
     Inputs:
     m1: a vector
     m2: a vector
     Output: a vector, sum of the vectors m1 and m2.
     */
    
    const unsigned long VECTOR_SIZE = m1.size();
    vector <type_t> sum (VECTOR_SIZE);
    
    for (unsigned i = 0; i != VECTOR_SIZE; ++i){
        sum[i] = m1[i] + m2[i];
    };
    
    return sum;
}

vector <type_t> operator-(const vector <type_t>& m1, const vector <type_t>& m2){
    
    /*  Returns the difference between two vectors.
     Inputs:
     m1: vector
     m2: vector
     Output: vector, m1 - m2, difference between two vectors m1 and m2.
     */
    
    const unsigned long VECTOR_SIZE = m1.size();
    vector <type_t> difference (VECTOR_SIZE);
    
    for (unsigned i = 0; i != VECTOR_SIZE; ++i){
        difference[i] = m1[i] - m2[i];
    }
    
    return difference;
}

vector <type_t> operator*(const vector <type_t>& m1, const vector <type_t>& m2){
    
    /*  Returns the product of two vectors (elementwise multiplication).
     Inputs:
     m1: vector
     m2: vector
     Output: vector, m1 * m2, product of two vectors m1 and m2
     */
    
    const unsigned long VECTOR_SIZE = m1.size();
    vector <type_t> product (VECTOR_SIZE);
    
    for (unsigned i = 0; i != VECTOR_SIZE; ++i){
        product[i] = m1[i] * m2[i];
    };
    
    return product;
}

vector <type_t> operator*(const type_t m1, const vector <type_t>& m2){
    
    /*  Returns the product of a type_t and a vectors (elementwise multiplication).
     Inputs:
     m1: type_t
     m2: vector
     Output: vector, m1 * m2, product of two vectors m1 and m2
     */
    
    const unsigned long VECTOR_SIZE = m2.size();
    vector <type_t> product (VECTOR_SIZE);
    
    for (unsigned i = 0; i != VECTOR_SIZE; ++i){
        product[i] = m1 * m2[i];
    };
    
    return product;
}

vector <type_t> operator/(const vector <type_t>& m2, const type_t m1){
    
    /*  Returns the product of a type_t and a vectors (elementwise multiplication).
     Inputs:
     m1: type_t
     m2: vector
     Output: vector, m1 * m2, product of two vectors m1 and m2
     */
    
    const unsigned long VECTOR_SIZE = m2.size();
    vector <type_t> product (VECTOR_SIZE);
    
    for (unsigned i = 0; i != VECTOR_SIZE; ++i){
        product[i] = m2[i] / m1;
    };
    
    return product;
}

vector <type_t> transpose (type_t *m, const int C, const int R) {
    
    /*  Returns a transpose matrix of input matrix.
     Inputs:
     m: vector, input matrix
     C: int, number of columns in the input matrix
     R: int, number of rows in the input matrix
     Output: vector, transpose matrix mT of input matrix m
     */
    
    vector <type_t> mT (C*R);
    
    for(int n = 0; n < C*R; n++) {
        int i = n/C;
        int j = n%C;
        mT[n] = m[R*j + i];
    }
    
    return mT;
}

vector <type_t> dot (const vector <type_t>& m1, const vector <type_t>& m2, const int m1_rows, const int m1_columns, const int m2_columns) {
    
    /*  Returns the product of two matrices: m1 x m2.
     Inputs:
     m1: vector, left matrix of size m1_rows x m1_columns
     m2: vector, right matrix of size m1_columns x m2_columns (the number of rows in the right matrix
     must be equal to the number of the columns in the left one)
     m1_rows: int, number of rows in the left matrix m1
     m1_columns: int, number of columns in the left matrix m1
     m2_columns: int, number of columns in the right matrix m2
     Output: vector, m1 * m2, product of two vectors m1 and m2, a matrix of size m1_rows x m2_columns
     */
    
    vector <type_t> output (m1_rows*m2_columns, 0);
#if defined(BLOCK_TILE)
    const int block_size = tile_size / sizeof(type_t); // 64 = common cache line size
    int N = m1_rows;
    int M = m2_columns; 
    int K = m1_columns;
#if defined(ENABLE_THREADING)
#pragma omp parallel for
#endif
    for (int i0 = 0; i0 < N; i0 += block_size) {
        int imax = i0 + block_size > N ? N : i0 + block_size;

        for (int j0 = 0; j0 < M; j0 += block_size) {
            int jmax = j0 + block_size > M ? M : j0 + block_size;

            for (int k0 = 0; k0 < K; k0 += block_size) {
                int kmax = k0 + block_size > K ? K : k0 + block_size;
                
                for(int i1 = i0; i1 < imax; ++i1) {
                
                  for(int j1 = j0; j1 < jmax; ++j1) {
                    
                    for(int k1 = k0; k1 < kmax; ++k1) {
                      output[i1 * M + j1] += m1[i1 * K + k1] * m2[k1 * M + j1];
                    }

                  } 
                
                }
        }
    }
                    
                    for(int k1 = k0; k1 < kmax; ++k1) {
                      output[i1 * M + j1] += m1[i1 * K + k1] * m2[k1 * M + j1];
                    }

                  } 
                
                }
#if defined(ENABLE_THREADING)
#pragma omp parallel for
#endif
    for( int row = 0; row < m1_rows; ++row ) {
        for( int col = 0; col < m2_columns; ++col ) {
            for( int k = 0; k < m1_columns; ++k ) {
                output[ row * m2_columns + col ] += m1[ row * m1_columns + k ] * m2[ k * m2_columns + col ];
            }
        }
    }
#endif
  return output;
}

vector <type_t> dot_transpose (const vector <type_t>& m1, const vector <type_t>& m2, const int m1_rows, const int m1_columns, const int m2_columns) {
    
    /*  Returns the product of two matrices: m1 x m2.
     Inputs:
     m1: vector, left matrix of size m1_rows x m1_columns
     m2: vector, right matrix of size m1_columns x m2_columns (the number of rows in the right matrix
     must be equal to the number of the columns in the left one)
     m1_rows: int, number of rows in the left matrix m1
     m1_columns: int, number of columns in the left matrix m1
     m2_columns: int, number of columns in the right matrix m2
     Output: vector, m1 * m2, product of two vectors m1 and m2, a matrix of size m1_rows x m2_columns
     */
    
    vector <type_t> output (m1_rows*m2_columns, 0);
#if defined(BLOCK_TILE)
    const int block_size = tile_size / sizeof(type_t); // 64 = common cache line size
    int N = m1_rows;
    int M = m2_columns; 
    int K = m1_columns;
#if defined(ENABLE_THREADING)
#pragma omp parallel for
#endif
    for (int i0 = 0; i0 < N; i0 += block_size) {
        int imax = i0 + block_size > N ? N : i0 + block_size;

        for (int j0 = 0; j0 < M; j0 += block_size) {
            int jmax = j0 + block_size > M ? M : j0 + block_size;

            for (int k0 = 0; k0 < K; k0 += block_size) {
                int kmax = k0 + block_size > K ? K : k0 + block_size;
                
                for(int i1 = i0; i1 < imax; ++i1) {
                
                  for(int j1 = j0; j1 < jmax; ++j1) {
                    
                    for(int k1 = k0; k1 < kmax; ++k1) {
                      output[i1 * M + j1] += m1[i1 * K + k1] * m2[j1 * M + k1];
                    }

                  } 
                
                }
            }
        }
    }
#else
#if defined(ENABLE_THREADING)
#pragma omp parallel for
#endif
    for( int row = 0; row < m1_rows; ++row ) {
        for( int col = 0; col < m2_columns; ++col ) {
            for( int k = 0; k < m1_columns; ++k ) {
                output[ row * m2_columns + col ] += m1[ row * m1_columns + k ] * m2[ col * m2_columns + k ];
            }
        }
    }
#endif
  return output;
}




int main(int argc, char** argv) { 
  if(argc < 3) { std::cout << "Usage: ./gemm dim_size is_b_transpose[0,1] <tile_size>" << std::endl; exit(0); }
  int dim_size = atoi(argv[1]);
  int transpose = atoi(argv[2]);
  tile_size = (argc > 3)? atoi(argv[3]) : tile_size; 
#ifdef ENABLE_THREADING
  int thread_num =  (argc > 4)? atoi(argv[4]) : 1; 
  omp_set_num_threads(thread_num);
  std::cout << "Threading is enabled" << std::endl;
#else 
  std::cout << "Threading is disabled" << std::endl;
#endif
#ifdef BLOCK_TILE
  std::cout << "Tiling is enabled with tile_size: " << tile_size << std::endl;
#else 
  std::cout << "Tiling is disabled" << std::endl;
#endif
//#ifdef SINGLE
//  std::cout << "Running in single precision mode" << std::endl;
//#else 
//  std::cout << "Running in double precision mode" << std::endl;
//#endif
  vector<type_t> A = random_vector(dim_size * dim_size); 
  vector<type_t> B = random_vector(dim_size * dim_size); 
  if(transpose) {
    std::cout << "Started multiplying " << dim_size << " x " << dim_size << " with transpose enabled" << std::endl;
    dot_transpose(A, B, dim_size, dim_size, dim_size);
  } else { 
    std::cout << "Started multiplying " << dim_size << " x " << dim_size << " with transpose disabled" << std::endl;
    dot(A, B, dim_size, dim_size, dim_size);
  }
  std::cout << "finished execution" << std::endl;
  return 0;
} 
