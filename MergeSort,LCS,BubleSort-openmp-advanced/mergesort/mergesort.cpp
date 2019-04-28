#include <stdio.h>
#include <omp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void merge(int * arr, int n, int * temparr) 
{

  int i = 0;
  int j = n/2;
  int k = 0;

  while (i<n/2 && j<n) {
     if (arr[i] < arr[j] ) {
       temparr[k++] = arr[i++];
     } else {
       temparr[k++] = arr[j++];
     }
  }

  while (i<n/2) {
    temparr[k++] = arr[i++];
  }
  while (j<n) {
     temparr[k++] = arr[j++];
   }
  for (int h=0;h<n;h++){
	  arr[h]=temparr[h];
  }
}


void mergesort(int * arr, int n, int * temparr, int chunksize) {

	if(n<2)
	{
	   return;
	}

	int mid = n/2;
	if(n>=chunksize)
	{

#pragma omp task
		{
			mergesort(arr,mid,temparr,chunksize);

		}
#pragma omp taskwait

#pragma omp task
		{
		mergesort(arr+mid,n-mid,temparr,chunksize);
	}

#pragma omp taskwait

		merge(arr,n,temparr);
	}
	else
	{
		mergesort(arr,mid,temparr,chunksize);
		mergesort(arr+mid,n-mid,temparr,chunksize);
		merge(arr,n,temparr);
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

  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int * arr = new int [n];
  int * temparr=new int [n];

  int n = atoi(argv[1]);
  int nbthreads= atoi(argv[2]);

  //generating array
  generateMergeSortData (arr, n);

  int chunksize = n/nbthreads; // getting the chunk size depending a]]upon available number of threads

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  omp_set_num_threads(nbthreads);


  #pragma omp parallel
  {
  #pragma omp single
  { 
   mergesort(arr, n,temparr,chunksize);
  }
  }

    checkMergeSortResult (arr, n);

    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    delete[] arr;
    delete[] temparr;

    std::cerr<<elapsed_seconds.count()<<std::endl;

    return 0;
  }

