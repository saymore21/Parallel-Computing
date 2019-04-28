#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>


using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void mergesort(int arr[], int left, int mid, int right, int temparr[]){
	int i = left, j = mid;
/*
cout << " in if left " << left << "\n";
cout << " in if mid " << mid << "\n";
cout << " in if right " << right << "\n";

cout << " -----------------------------------------"<< "\n";*/

/*for (int i =0; i<sizeof(arr); i++)
{
std ::cout << "in merge arr["<<i<< "] " <<arr[i] << "\n"; 
}*/

//	for(int k = left; k<right; k++){
        int k =left;
		while(i < mid && j < right)
{
		if(arr[i] <= arr[j]){
			temparr[k] = arr[i];

/*cout << " in if i" << i << "\n";
cout << " in if j" << j << "\n";
cout << " in if arr[i]" <<arr[i] << "\n";
cout << " in if arr[j]" <<arr[j] << "\n";
cout << " in if temparr[k]" <<temparr[k] << "\n";*/
			i++;
k++;
}		else {
			temparr[k] = arr[j];
			j++;
k++;

//cout << " in else temparr[k]" <<temparr[k] << "\n";
		}
	}
	 while (i < mid) 
    { 
        temparr[k] = arr[i]; 
        i++;
k++;

//cout << " in i while temparr[k]" <<temparr[k] << "\n";
    } 
   
    while (j < right) 
    { 
        temparr[k] = arr[j]; 
        j++; 
k++;

//cout << " in j while temparr[k]" <<temparr[k] << "\n";
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

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
  // get arr data
  int * arr = new int [n];
  int * temparr = new int [n];
  generateMergeSortData (arr, n);
 

/*for (int i =0; i<n; i++)
{
std ::cout << "arr["<<i<< "] " <<arr[i] << "\n"; 
}
*/
  omp_set_num_threads(nbthreads);

    //Variables to capture execution start/end time instances
  struct timespec begin, end;
    //#pragma omp threadprivate(temparr)
  clock_gettime(CLOCK_MONOTONIC_RAW, &begin);


/*#pragma omp parallel
{
#pragma omp single
{*/
  for(int i = 1; i< n; i =  2*i)
{ 
//#pragma omp task //private(temparr)
  for(int j=0; j < n ; j =j+2*i)
{
 mergesort(arr, j, std::min(j+i, n), std::min(j+2*i, n), temparr);
//break;
}
for(int i = 0; i<n; i++)
{
arr[i] = temparr[i];
}
//break;
}
//}
//}

/*
for (int i =0; i<n; i++)
{
std ::cout << "temparr["<<i<< "] " <<temparr[i] << "\n"; 
}
*/



  checkMergeSortResult (arr, n);
/*
for (int i =0; i<n; i++)
{
std ::cout << "arr["<<i<< "] " <<arr[i] << "\n"; 
}
*/

clock_gettime(CLOCK_MONOTONIC_RAW, &end);
 float exeTimeInSec = (end.tv_nsec - begin.tv_nsec) / 1000000000.0 + (end.tv_sec - begin.tv_sec);

  cerr << exeTimeInSec << endl;
  
  delete[] arr;

  return 0;
}
