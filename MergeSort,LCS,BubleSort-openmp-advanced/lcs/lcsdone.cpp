#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <omp.h>
#include <vector>

#define DEBUG 0

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
  int nbt=atoi(argv[3]);

  omp_set_num_threads(nbt);

 //  get string data
  char *X = new char[m];
  char *Y = new char[n];

  generateLCS(X, m, Y, n);

  int** C = new int*[m+1];

    for (int i=0; i<=m; ++i) {
      C[i] = new int[n+1];
    }

  //result to store length of LCS 
  int result = 0;

  //initiating clock
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

//code to create edit distance matrix
  #pragma omp parallel
  {
  #pragma omp parallel for
  for (int i=0; i<=m; ++i) 
  {
      C[i][0] = 0;
  }
  #pragma omp parallel for
  for (int j=1; j<=n; ++j) 
  {
      C[0][j] = 0;
   }

    for (int a=1; a<=m; ++a) 
    {
    #pragma omp parallel for
    for (int b=1; b<=n; ++b)
    {
    if (X[a-1] == Y[b-1]) 
    {
       C[a][b] = C[a-1][b-1] + 1;
    }
     else 
    {
       C[a][b] = std::max(C[a][b-1], C[a-1][b]);
     }
      
     }

    }
  }

    //the last element of the matrix stores the final length of lcs 
    //storing that result in result
    result=C[m][n];


  //stoping the clock
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  checkLCS(X, m, Y, n, result);
  std::cerr<<elpased_seconds.count()<<std::endl;

  delete[] X;
  delete[] Y;

  return 0;
}