#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <mpi.h>

using namespace std;

float genA (int row, int col) {
  if (row > col)
    return 1.;
  else
    return 0.;
}

float genx0 (int i) {
  return 1.;
}


void checkx (int iter, long i, float xval) {
  if (iter == 1) {
    float shouldbe = i;
    if (fabs(xval/shouldbe) > 1.01 || fabs(xval/shouldbe) < .99 )
      std::cout<<"incorrect : x["<<i<<"] at iteration "<<iter<<" should be "<<shouldbe<<" not "<<xval<<std::endl;
  }

  if (iter == 2) {
    float shouldbe =(i-1)*i/2;
    if (fabs(xval/shouldbe) > 1.01 || fabs(xval/shouldbe) < .99)
      std::cout<<"incorrect : x["<<i<<"] at iteration "<<iter<<" should be "<<shouldbe<<" not "<<xval<<std::endl;
  }
}

int main (int argc, char*argv[]) {
  MPI_Init(&argc, &argv);

  if (argc < 3) {
    std::cout<<"usage: "<<argv[0]<<" <n> <iteration>"<<std::endl;
  }

  int rank;
  int wsize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &wsize);

  bool check = true;
  
  long n = atol(argv[1]);

  long iter = atol(argv[2]);

  long* temp_x = new long[n/2];


  MPI_Comm row_communicator, column_communicator;
  MPI_Comm_split(MPI_COMM_WORLD,  (rank / n), rank, &column_communicator);
  MPI_Comm_split(MPI_COMM_WORLD,  (rank % n), rank, &row_communicator);

  int rrank, rsize;
  MPI_Comm_rank(row_communicator, &rrank);
  MPI_Comm_size(row_communicator, &rsize);

  int crank, csize;
  MPI_Comm_rank(column_communicator, &crank);
  MPI_Comm_size(column_communicator, &csize);

   int square = sqrt(wsize);
   long block_size = n/square;
   long div = rank / square;
   long mod  = rank % square;
   long rstart = (rank - (rank - div)) * block_size;
   long rend = rstart + block_size;
   long cstart = (rank - (rank - mod)) * block_size;
   long cend = cstart + block_size;

  long* A = new long[block_size*block_size];
  long* x = new long[block_size];
  long* y = new long[block_size];
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();


for(long it = 0; it<iter; it++){
 
  for(long r =rstart; r<rend;r++){
   // long local_sum = 0.0;
    for(long c = cstart; c<cend; c++){         
      y[c] += A[r*n+c] * x[c];
    }
  

  }

  MPI_Reduce(y, x, block_size, MPI_FLOAT, MPI_SUM, rank/n, row_communicator);

 

        /*if (check){
          for (long i = 0; i<n; ++i){
        checkx (it+1, i, x[i]);
          }*/
        //}
   MPI_Bcast(x, block_size, MPI_FLOAT, rank%n , column_communicator);
  }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;

  
  

  delete[] A;
  delete[] x;
  delete[] y;

  MPI_Finalize();
  return 0;
}
	