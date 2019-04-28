#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <string.h>


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

int divideconq(int*, int);

int divideconq (int* arr,int n){
int i,j, sum =0;
int chunk = n/2;
if(n==0) return 0;
if(n == 1) return arr[0];
else {

 #pragma omp task 
i = divideconq( arr, chunk);

#pragma omp taskwait
sum += i;
return sum;
}
}

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
  generateReduceData (arr, atoi(argv[1]));

  omp_set_num_threads(nbthreads);

  //insert reduction code here
  #pragma omp parallel reduction(+:result_sum)
  {
 #pragma omp single 
  result_sum = divideconq(arr, n);
}

/*for(int i = 0; i<n; i++)
{
cout<< arr[i]<<"\n";
}  */


  delete[] arr;

  std::cout << result_sum; //Printing the result sum
  auto end = check_time::now();
  auto time_elapsed = end - initiated;
  auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);

  std::cerr<<secs.count();

  return 0;
}
