#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>


using namespace std;
int n =0;
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void merge(int * arr, int l, int m, int r) 
{ 

        cout << "l : " << l << "\n";
        cout << "m : " << m << "\n";
        cout << "r : " << r << "\n";
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 =  r - m; 
  
    /* create temp arrays */
    int * L = new int[n1];
    int * R = new int[n2]; 
  
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for (j = 0; j < n2; j++) 
        R[j] = arr[m + 1+ j]; 
  
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray 
    j = 0; // Initial index of second subarray 
    k = l; // Initial index of merged subarray 
    while (i < n1 && j < n2) 
    { 
        cout << " in while 1... " << "\n";
        if (L[i] <= R[j]) 
        { 
            arr[k] = L[i]; 
            i++; 
        } 
        else
        { 
            arr[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
  
    /* Copy the remaining elements of L[], if there 
       are any */
    while (i < n1) 
    { 
        cout << " in while 2 ... " << "\n";
        arr[k] = L[i]; 
        i++; 
        k++; 
    } 
  
    /* Copy the remaining elements of R[], if there 
       are any */
    while (j < n2) 
    { 
        cout << " in while 3 ... " << "\n";
        arr[k] = R[j]; 
        j++; 
        k++; 
    } 

delete[] L;
delete[] R;
} 
  

void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        int m = l+(r-l)/2; 
  
        // Sort first and second halves 
        #pragma omp task
        mergeSort(arr, l, m); 
#pragma omp taskwait
#pragma omp task
        mergeSort(arr, m+1, r); 
  #pragma omp taskwait
  }
        merge(arr, l, m, r);
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

  n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  // get arr data
  int * arr = new int [n];
  int * temparr = new int [n];
  generateMergeSortData (arr, n);
  int chunk = n/nbthreads;

/*for(int i =0; i<n; i++)
{
 cout << " in main arr[i]" << arr[i] << "\n";
}*/


  omp_set_num_threads(nbthreads);

    //Variables to capture execution start/end time instances
  struct timespec begin, end;
    //#pragma omp threadprivate(temparr)
  clock_gettime(CLOCK_MONOTONIC_RAW, &begin);


#pragma omp parallel
{
#pragma omp single
{

   mergeSort(arr, 0, n-1, chunk);
}
}

  checkMergeSortResult (arr, n);

 /* for(int i =0; i<n; i++)
{
 cout << "arr[i]" << arr[i] << "\n";
}*/


clock_gettime(CLOCK_MONOTONIC_RAW, &end);
 float exeTimeInSec = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);

  cerr << exeTimeInSec << endl;
  
  delete[] arr;
 //delete[] L;
 //delete[] R;

  return 0;
}