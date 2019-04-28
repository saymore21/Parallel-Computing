#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

void generateReduceData (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

long int partial_sum = 0;

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

  int n 	= atoi(argv[1]);
  int * arr 	= new int [n];
  int nbThreads = atoi(argv[2]);

  generateReduceData (arr, atoi(argv[1]));

  //Set the number of threads
  omp_set_dynamic(0);
  omp_set_num_threads(nbThreads);

  //insert reduction code here
  
  long int sum = 0;

  //Variables to capture execution start/end time instances
  struct timespec begin, end;

#if 1

  int num_threads = omp_get_num_threads();

  clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

  long int chunkSize = n / nbThreads;

  int max_threads = omp_get_max_threads();

  #pragma omp threadprivate(partial_sum)

  #pragma omp parallel reduction(+: sum)
  {
	  #pragma omp single
	  {
  		for(int counter = 0; counter < max_threads; counter++)
		{ 
			long int localStart;
			long int localEnd;
			int k;

			#pragma omp task private(localStart, localEnd, k)
			{
				localStart 	= counter * chunkSize;
				localEnd 	= localStart + chunkSize;
				
				for(k = localStart; k < localEnd; k++)
				{
					partial_sum += arr[k];
				}
			}
		}

	  }

	  sum += partial_sum;
  }

  int remainder = n % nbThreads;

  if(remainder > 0)
  {
	long int localStart = chunkSize * nbThreads;
	long int localEnd   = localStart + remainder;

	for(int l = localStart; l < localEnd; l++)
	{
		sum += arr[l];
	}
  }

  //Capture the current (execution stop) time in end
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

#endif
  
  //Calculate the total execution time in seconds
  //Add the time reported in nano seconds to the total time
  //after converting it to seconds
  float exeTimeInSec = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);

  //Print the sum value to the output stream
  cout <<endl<< sum << endl;
  
  //Print the total execution time (in sec) to the error stream
  cerr << exeTimeInSec << endl;
  
  delete[] arr;

  return 0;
}
