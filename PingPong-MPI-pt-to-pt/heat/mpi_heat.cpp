#include <mpi.h>
#include <math.h>
#include <iostream>


#ifdef __cplusplus
extern "C" {
#endif

  int check2DHeat(double** H, long n, long rank, long P, long k); //this assumes array of array and grid block decomposition

#ifdef __cplusplus
}
#endif

/***********************************************
 *         NOTES on check2DHeat.
 ***********************************************
 *         
 *  First of, I apologize its wonky. 
 *
 *  Email me ktibbett@uncc.edu with any issues/concerns with this. Dr. Saule or the other
 *    TA's are not familiar with how it works. 
 *
 * Params:
 *  n - is the same N from the command line, NOT the process's part of N
 *  P - the total amount of processes ie what MPI_Comm_size gives you.
 *  k - assumes n/2 > k-1 , otherwise may return false negatives.
 *
 *   
 * Disclaimer:
 ***
 *** Broken for P is 9. Gives false negatives, for me it was always
 ***  ranks 0, 3, 6. I have not found issues with 1, 4, or 16, and these
 ***  are what `make test` will use.
 ***
 *
 * Usage:
 *  When code is WRONG returns TRUE. Short example below
 *  if (check2DHeat(...)) {
 *    // oh no it is (maybe) wrong  
 *    std::cout<<"rank: "<<rank<<" is incorrect"<<std::endl;
 *  }
 *
 *
 *
 *  I suggest commenting this out when running the bench
 *
 *
 * - Kyle
 *
 *************/


// Use similarily as the genA, genx from matmult assignment.
double genH0(long row, long col, long n) {
  double val = (double)(col == (n/2));
  return val;
}



int main(int argc, char* argv[]) {

  if (argc < 3) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <N> <K>"<<std::endl;
    return -1;
  }

  // declare and init command line params
  long N, K;
  N = atol(argv[1]);
  K = atol(argv[2]);

  // use double for heat 2d 

  // write code here


  return 0;
}

