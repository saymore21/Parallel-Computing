#include <mpi.h>
#include <math.h>
#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

int check2DHeat(double** H, long n, long rank, long P, long k);
//this assumes array of array and grid block decomposition

#ifdef __cplusplus
}
#endif

/***********************************************
 *         NOTES on check2DHeat.
 ***********************************************
 *
 *  First of, I apologize its wonky.
 *
 *  Email me ktibbett@uncc.edu with any issues/concerns with this.
Dr. Saule or the other
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
     MPI_Init(&argc, &argv);


    if (argc < 3) {
        std::cerr<<"usage: mpirun "<<argv[0]<<" <N> <K>"<<std::endl;
        return -1;
    }

    // declare and init command line params
    //long N, K;
    const long N = atol(argv[1]);
    long K = atol(argv[2]);

    int wrank, wsize, row_rank, col_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);
    

    long n = atol(argv[1]);
    int square = sqrt(wsize);
    int block_size = n/square;
    long div = wrank/ square;
    long mod  = wrank % square;
    long rstart = (wrank - (wrank - div)) * block_size;
    long rend = rstart + block_size;
    long cstart = (wrank - (wrank - mod)) * block_size;
    long cend = cstart + block_size;

    
    double *H = new double[block_size*block_size];
    double *H_K = new double[block_size*block_size];
    /*double **H = (double **)malloc(N * sizeof(double *));
    for (long i=0; i<N; i++)
         H[i] = (double *)malloc(N * sizeof(double));

    double **H_K = (double **)malloc(N * sizeof(double *));
    for (long i=0; i<N; i++)
         H_K[i] = (double *)malloc(N * sizeof(double)); */

    // use double for heat 2d

    // write code here
    int track = 0;
    for(int r =rstart; r<rend; r++) {
        // long local_sum = 0.0;
        for(int c = cstart; c<cend; c++) {
            H_K[track]= genH0(r, c, N);
   	    track++;
        }

    }

//int *left, *right, *up, *down ;

if(rstart == 0 && cstart == 0 || rstart == 0 && cend == n || cstart == 0 && rend == n || cend == n && rend == n)
{
  //cout << " I am "<<wrank<< " and I am in! \n"; 
 MPI_Request request[2];
 if(rstart == 0)
 {
  cout << " I am "<<wrank<< " and I will send ! \n"; 
  int* down = new int[block_size];
  int* up = new int[block_size];
  int comrank = wrank+square;
  for(int t =0; t < block_size; t++)
  {
   down[t] = H_K[((block_size * block_size) - block_size) + t];
  }
  MPI_Isend(down, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[1]);
  MPI_Waitall(block_size, request, MPI_STATUSES_IGNORE);
  MPI_Irecv(up, block_size, MPI_INT,comrank, 0, MPI_COMM_WORLD, &request[1]);
  }
 else if( rend  == n)
 {
  int* up = new int[block_size];
  int* down = new int[block_size];
  for(int t =0; t < block_size; t++)
  {
   up[t] = H_K[t];
  }
  int comrank = wrank-square;
  MPI_Isend(up, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[1]);
  MPI_Waitall(block_size, request, MPI_STATUSES_IGNORE);
  MPI_Irecv(down, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[1]);
  }
   
if(cstart == 0)
{
  int* right = new int[block_size];
  int* left = new int[block_size];
  for(int t =0; t < block_size; t++)
  {
   right[t] = H_K[((block_size - 1) + (t * (block_size - 1)))];
  }
  int comrank = wrank+1;
  MPI_Isend(right, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[2]);
  MPI_Waitall(block_size, request, MPI_STATUSES_IGNORE);
  MPI_Irecv(left, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[2]);
 }
else if(cend == n)
{
 int* left = new int[block_size];
 int* right = new int[block_size];
 for(int t =0; t < block_size; t++)
  {
   left[t] = H_K[t * block_size];
  }
  int comrank = wrank-1;
  MPI_Isend(left, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[2]);
  MPI_Waitall(block_size, request, MPI_STATUSES_IGNORE);
  MPI_Irecv(right, block_size, MPI_INT, comrank, 0, MPI_COMM_WORLD, &request[2]);
}
}

//}
//else if (rstart == 0 && cend != n || cstart == 0 && rstart != 0 || cend == n && rstart != 0 ||  

 /*if(rstart == 0 || rend 
 int size = 2*blocksize;
 temp = new int[size];

  MPI_Request request[size];
  int c = cend;
  int rid = 0;
  int start = square - 1; 
  for(int r =rstart; r<rend; r++) {
	MPI_ISend(H_K[], 1 , MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &request[rid]);

{
   int size = 2*square;
   temp = new int[size];
   if(cstart
   MPI_ISend(   

    for(long iter = 0; iter < K; iter++) {
        for(double i = 0; i< partition; i++) {
            for(double  j = 0; j<partition; j++) {

                if(i == 0 && j==0) {
                    H[i*N+j] = (H_K[i*N+(j+1)] + H_K[(i+1)*N+j] +  (3 * H_K[i*N+j])) / 5;
                } else if(i==N-1 && j==N-1) {
                    H[i*N+j]= (H_K[i*N+(j-1)] + H_K[(i-1)*N+j] + (3 * H_K[i*N+j]))/5;
                } else if(i==0 && j!=0) {
                    H[i*N+j]= (H_K[i*N+(j+1)] + H_K[i*N+(j-1)] + H_K[(i+1)*N+j]
                               + (2 * H_K[i*N+j]))/5;
                } else if(i!=0 && j==0) {
                    H[i*N+j]= (H_K[i*N+(j+1)] + H_K[(i-1)*N+j] + H_K[(i+1)*N+j]
                               + (2 * H_K[i*N+j]))/5;
                } else if(i!=N-1 && j==N-1) {
                    H[i*N+j]= (H_K[i*N+(j-1)] + H_K[(i-1)*N+j] + H_K[(i+1)*N+j]
                               + (2 * H_K[i*N+j]))/5;
                } else if(i==N-1 && j!=N-1) {
                    H[i*N+j]= (H_K[i*N+(j+1)] + H_K[i*N+(j-1)] + H_K[(i-1)*N+j]
                               + (2 * H_K[i*N+j]))/5;
                } else if(i==0 && j==N-1) {
                    H[i*N+j]= (H_K[i*N+(j-1)] + H_K[(i+1)*N+j] + (3 * H_K[i*N+j]))/5;
                } else if(i==N-1 && j==0) {
                    H[i*N+j]= (H_K[i*N+(j+1)] + H_K[(i-1)*N+j] + (3 * H_K[i*N+j]))/5;
                } else {
                    H[i*N+j]= (H_K[i*N+(j+1)] + H_K[(i-1)*N+j] + H_K[i*N+j] +
                               H_K[(i+1)*N+j] + H_K[i*N+(j-1)])/5;
                }

            }
        }
        /*for (int row = 0; row<N; row++) {
         for (int col=0; col<N; col++) {
           std::cout<<H[row*N+col]<<" ";
         }
         std::cout<<std::endl;
        }*/
        //double **p = &H;
        //cout<<check2DHeat(p, N, 0, 1, iter)<<endl;
       // H_K = H;
  //  }
//}
    MPI_Finalize();
    return 0;
}
