#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <bits/stdc++.h>
#include <string.h>
#include  <omp.h>
using namespace std;
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void bubbleSort(int arr[], int n)
{
  for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            int temp = arr[i];
    arr[i] = arr[i+1];
    arr[i+1] = temp;
        }
    }
    if (n - 1 > 1) {
 #pragma omp task
        bubbleSort(arr, n - 1);
    }
}
 
int main (int argc, char* argv[]) {
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC_RAW, &start);

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

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  omp_set_num_threads(nbthreads);
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);
/*for(int i=0;i<n;i++)
  {
      printf("\n arr[%i]:%i",i,arr[i]);
  }*/
  //insert sorting code here.
#pragma omp parallel
{
#pragma omp single
bubbleSort(arr,n);
}
           

  /*for(int i=0;i<n;i++)
  {
      printf("\n arr[%i]:%i",i,arr[i]);
  }*/
  checkMergeSortResult (arr, n);
 
  delete[] arr;
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);
  float elapsedTime = (end.tv_nsec - start.tv_nsec) / 1000000000.0 + (end.tv_sec - start.tv_sec);
  cerr << elapsedTime << endl;
  return 0;
}

