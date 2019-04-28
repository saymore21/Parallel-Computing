#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <omp.h>
#include <chrono>
#include <string.h>


using namespace std;
using seconds = chrono::seconds;
using check_time = std::chrono::high_resolution_clock;


#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);


  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  // get string data
  char *X = new char[n];
  char *Y = new char[m];
  char *parent = new char[max(n,m)];
  generateLCS(X, m, Y, n);
  
  int C[n+1][m+1];

 #pragma omp parallel for
 for(int i=0;i<=m;i++)
 {
   C[0][i]=0;
 }

 #pragma omp parallel for
 for(int i=0;i<=n;i++)
 {
   C[i][0]=0;
 }


 
 for(int x=1; x<=max(n,m);x++)
 {

    if(x == min(n,m))
    {
    if (X[x-1] == Y[x-1])
            C[x][x] = C[x-1][x-1] + 1;
    else
        C[x][x] = max(C[x][x-1],C[x-1][x]);
    }    

     #pragma omp parallel for
    for(int j = x; j<=m;j++)
    {
        if (X[x-1] == Y[j-1])
                C[x][j] = C[x-1][j-1] + 1;
        else
            C[x][j] = max(C[x][j-1],C[x-1][j]);
    }
 
     #pragma omp parallel for
    for(int i = x+1; i<=n; i++)
    {
        if (X[i-1] == Y[x-1])
                C[i][x] = C[i-1][x-1] + 1;
        else
            C[i][x] = max(C[i][x-1],C[i-1][x]);
    }
 }

 
  result = C[n][m];

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkLCS(X, m, Y, n, result);

  std::cerr<<elpased_seconds.count()<<std::endl;

  delete[] X;
  delete[] Y;
  return 0;
}