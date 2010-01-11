// g++ -fopenmp openmp.cpp -o test

#include <omp.h>
#include <iostream>
int main (int argc, char *argv[]) {
 int th_id, nthreads;
#pragma omp parallel private(th_id)
 {
  th_id = omp_get_thread_num();
  std::cout << "[" << th_id << "]\n";
#pragma omp barrier
 if ( th_id == 0 ) {
   nthreads = omp_get_num_threads();
   std::cout << "thread count: " << nthreads << "\n";
  }
 }
 return 0;
}
