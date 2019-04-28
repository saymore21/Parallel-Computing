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

void merge(int * arr, int left, int mid, int right, int * temparr){
	int i = left, j = mid;
cout << " in if left " << left << "\n";
cout << " in if mid " << mid << "\n";
cout << " in if right " << right << "\n";

cout << " -----------------------------------------"<< "\n";

/*for (int i =0; i<sizeof(arr); i++)
{
std ::cout << "in merge arr["<<i<< "] " <<arr[i] << "\n"; 
}*/

//	for(int k = left; k<right; k++){
        int k =left;
		while(i < mid && j < right)
{

cout << " in while 1 " << "\n";
		if(arr[i] <= arr[j]){
			temparr[k] = arr[i];

cout << " in if 1 " << "\n";
cout << " in if arr[i]" <<arr[i] << "\n";
cout << " in if temparr[k]" << temparr[k] << "\n";
/*
cout << " in if arr[i]" <<arr[i] << "\n";
cout << " in if arr[j]" <<arr[j] << "\n";
cout << " in if temparr[k]" <<temparr[k] << "\n";*/
			i++;
k++;
}		else {

			temparr[k] = arr[j];
cout << " in else 1 " << "\n";
cout << " in else 1 arr[j]" <<arr[j] << "\n";
cout << " in else 1  temparr[k]" << temparr[k] << "\n";
			j++;
k++;

//cout << " in else temparr[k]" <<temparr[k] << "\n";
		}
	}
	 while (i < mid) 
    { 

cout << " in while 2 " << "\n";
        temparr[k] = arr[i];
cout << " in while 2 arr[i]" <<arr[i] << "\n";
cout << " in while 2 temparr[k]" << temparr[k] << "\n";
        i++;
k++;

//cout << " in i while temparr[k]" <<temparr[k] << "\n";
    } 
   
    while (j < right) 
    { 

cout << " in while 3" << "\n";
        temparr[k] = arr[j]; 
cout << " in while 3 arr[j]" <<arr[j] << "\n";
cout << " in while 3 temparr[k]" << temparr[k] << "\n";
        j++; 
k++;

//cout << " in j while temparr[k]" <<temparr[k] << "\n";
    } 


/*for(int i =0; i<n; i++)
{
 //arr[i] = temparr[i];
cout << "temp arr[i]" << temparr[i] << "\n";
}*/



 for(int i =0; i<n; i++)
{
 arr[i] = temparr[i];
 //cout << "arr[i]" << arr[i] << "\n";
}

/*for(int i =0; i<n; i++)
{
 //arr[i] = temparr[i];
cout << "arr[i]" << arr[i] << "\n";
} */


}

 
/* l is for left index and r is right index of the 
   sub-array of arr to be sorted */
void mergeSort(int * arr, int l, int r, int chunk, int * temparr) 
{ 
    int m = n/2;


    if (m > chunk) 
    { 
        // Same as (l+r)/2, but avoids overflow for 
        // large l and h 
        //int m = l+(r-l)/2; 
  
	#pragma omp task
        mergeSort(arr, l, m, chunk, temparr); 
#pragma omp taskwait
#pragma omp task
        mergeSort(arr, m+1, r, chunk, temparr); 
  #pragma omp taskwait
  }
        merge(arr, l, m, r, temparr); 
   // } 
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
   
  int chunk = (n/nbthreads);
/*
    for(int i =0; i<n; i++)
{
 cout << " in main arr[i]" << arr[i] << "\n";
}

*/
  omp_set_num_threads(nbthreads);

    //Variables to capture execution start/end time instances
  struct timespec begin, end;
    //#pragma omp threadprivate(temparr)
  clock_gettime(CLOCK_MONOTONIC_RAW, &begin);


#pragma omp parallel
{
#pragma omp single
{
//#pragma omp task
   mergeSort(arr, 0, n-1,chunk, temparr);
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

  return 0;
}