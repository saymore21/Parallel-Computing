#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <string.h>
#include <math.h>


using namespace std;
using seconds = chrono::seconds;
using check_time = std::chrono::high_resolution_clock;


#ifdef __cplusplus
extern "C" {
#endif

  void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {
  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

 
  //Starting the clock time
  auto initiated = check_time::now();

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int nbthreads = atoi(argv[2]);
  int result_sum = 0;

  generateReduceData (arr, n);

  omp_set_num_threads(nbthreads);
   
   int chunk = (n/nbthreads);
   int * temp = new int [nbthreads];


  #pragma omp parallel //reduction(+:result_sum)
  #pragma omp single 
// This loop runs the inner loop for the number of threads times
  for(int i = 0; i < nbthreads; i++)
{
int jlimit = ((i+1) * chunk);
if(i == nbthreads) jlimit = n; // If its the last thread all the last chunk is givne to the last thread 
int j;
  #pragma omp task
{
//loop for local chunk specific summation of values
 for(j= i*chunk;j<jlimit; j++)
{
//cout<< "j from "<< i*chunk << "\n";
//cout<< "j till "<< jlimit << "\n";
 temp[i] += arr[j];
//if(j >= n-1) break;
//cout<< "temp"<< temp[i] << "\n";
}
//Adding result after every chunk execution
#pragma omp critical
result_sum += temp[i];
}
}



  delete[] arr;

  std::cout << result_sum; //Printing the result sum
  auto end = check_time::now();
  auto time_elapsed = end - initiated;
  auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);

  std::cerr<<secs.count();

  return 0;
}
