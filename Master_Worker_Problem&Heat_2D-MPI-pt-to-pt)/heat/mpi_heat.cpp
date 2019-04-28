#include <mpi.h>
#include <cmath>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>

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

  if (argc < 3) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <N> <K>"<<std::endl;
    return -1;
  }

    // declare and init command line params
    //long N, K;
  const long N = atol(argv[1]);
  long K = atol(argv[2]);
  MPI_Init(&argc, &argv);

  int wrank, wsize;

  MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
  MPI_Comm_size(MPI_COMM_WORLD, &wsize);

  int square = sqrt(wsize);
  int block_size = N/square;
  int div = wrank / square;
  int mod  = fmod(wrank, square);
  int rstart = (wrank - (wrank - div)) * block_size;
  /*if(wrank == 2){
    cout<<" "<<rstart<<" = ("<<wrank<<" - ("<<wrank<<" - ("<<wrank/sqrt(wsize)<<"))) * "<<block_size<<endl;
  }*/
  int rend = rstart + (block_size-1);
  int cstart = (wrank - (wrank - mod)) * block_size;
  int cend = cstart + (block_size-1);
  int count_req = 0;
 
 

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

//cout<< "GENERATED \n";
std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  for(long it = 0; it < K; it++) {
   
    double *sb1  = new double[square];
    double *sb2  = new double[square];
    double *sb3  = new double[square];
    double *sb4  = new double[square];

    double *rb1  = new double[square];
    double *rb2  = new double[square];
    double *rb3  = new double[square];
    double *rb4  = new double[square];
    int r_size = 0;
    int rcount = 0;
   
    if(cstart != 0){
      r_size+=2;
    }
    if(rstart != 0){
      r_size+=2;
    }
    if(cend != N-1){
      r_size+=2;
    }
    if(rend != N-1){
      r_size+=2;
    }
    MPI_Request req[r_size];
    MPI_Status status[4];

    if(cstart != 0){
      int r = 0;
      for(int i =0; i<square; i++) {
        sb1[i]= H_K[r];
        r+=block_size;     
      }
      //cout<<"in wrank "<<wrank<<" wrank - 1: "<<wrank-1<<endl;
      MPI_Isend(sb1, square, MPI_DOUBLE, wrank - 1, 0, MPI_COMM_WORLD, &req[rcount]);
      rcount++;
      MPI_Irecv(rb1, square, MPI_DOUBLE, wrank - 1, 0, MPI_COMM_WORLD, &req[rcount]);
      count_req+=2;
      rcount++;
    }
    if(rstart != 0){
      int c = 0;
      for(int i =0; i<square; i++) {
        sb2[i]= H_K[c];
        c++;     
      }
      //cout<<"in wrank "<<wrank<<" rstart:: "<<rstart<<" wrank - square: "<<wrank-square<<endl;
      MPI_Isend(sb2, square, MPI_DOUBLE, wrank - square, 0, MPI_COMM_WORLD, &req[rcount]);
      rcount++;
      MPI_Irecv(rb2, square, MPI_DOUBLE, wrank - square, 0, MPI_COMM_WORLD, &req[rcount]);
      count_req+=2;
      rcount++;
    }
    if(cend != N-1){
      int r = cend;
      for(int i =0; i<square; i++) {
        sb3[i]= H_K[r];
        r+=block_size;     
      }
      //cout<<"in wrank "<<wrank<<" cend:: "<<cend<<" wrank + 1: "<<wrank+1<<endl;
      MPI_Isend(sb3, square, MPI_DOUBLE, wrank + 1, 0, MPI_COMM_WORLD, &req[rcount]);
      rcount++;
      MPI_Irecv(rb3, square, MPI_DOUBLE, wrank + 1, 0, MPI_COMM_WORLD, &req[rcount]);
      rcount++;
      count_req+=2;
    }
    if(rend != N-1){
      int c = rend;
      for(int i =0; i<square; i++) {
        sb4[i]= H_K[c];
        c++;     
      }
      //cout<<"in wrank "<<wrank<<"wrank + square: "<<wrank+square<<endl;
      MPI_Isend(sb4, square, MPI_DOUBLE, wrank + square, 0, MPI_COMM_WORLD, &req[rcount]);
      rcount++;
      MPI_Irecv(rb4, square, MPI_DOUBLE, wrank + square, 0, MPI_COMM_WORLD, &req[rcount]);
      count_req+=2;
      rcount++;
    }
    //if((rstart == 0 && cstart == 0) || (rend == N-1 && cend == N-1) || (rstart == 0 && cend == N-1) || rend == N-1 && cstart == 0){
   
      MPI_Waitall(r_size, req, MPI_STATUSES_IGNORE);
   
    int new_track = 0;
    for(int i = 0; i< block_size; i++) {
      for(int  j = 0; j<block_size; j++) {

        if(i == 0 && j==0) {
          if(rstart == 0 && cstart ==0){
            H[new_track] = (H_K[1] + H_K[(i+1)*block_size+j] +  (3 * H_K[i*block_size+j])) / 5;
          }else{
            H[new_track] = (H_K[1] + H_K[(i+1)*block_size+j] +  H_K[i*block_size+j] + rb1[0] + rb2[0]) / 5;
          }
         
        }
       else if(i==block_size-1 && j==block_size-1) {

       if(rend == N-1 && cend ==N-1){
        H[new_track]= (H_K[i*block_size+(j-1)] + H_K[(i-1)*block_size+j] + (3 * H_K[i*block_size+j]))/5;

      }else{
        H[new_track]= (H_K[i*block_size+(j-1)] + H_K[(i-1)*block_size+j] + H_K[i*block_size+j] + rb3[i] + rb4[i])/5;

      }
    } else if(i==0 && j!=block_size-1) {
      if(rstart == 0 && cstart !=N-1){
        H[new_track]= (H_K[i*block_size+(j+1)] + H_K[i*block_size+(j-1)] + H_K[(i+1)*block_size+j]
         + (2 * H_K[i*block_size+j]))/5;

      }else{
        H[new_track]= (H_K[i*block_size+(j+1)] + H_K[i*block_size+(j-1)] + H_K[(i+1)*block_size+j]
         + H_K[i*block_size+j] + rb2[j])/5;

      }
     
    } else if(i!=0 && j==0) {
     if(rstart != 0 && cstart ==0){
      H[new_track]= (H_K[i*block_size+(j+1)] + H_K[(i-1)*block_size+j] + H_K[(i+1)*block_size+j]
       + (2 * H_K[i*block_size+j]))/5;               
    }else{
      H[new_track]= (H_K[i*block_size+(j+1)] + H_K[(i-1)*block_size+j] + H_K[(i+1)*block_size+j]
       + H_K[i*block_size+j] + rb1[i])/5;  
    }
  } else if(i!=N-1 && j==N-1) {
    if(rend != N-1 && cend ==N-1){
      H[new_track]= (H_K[i*block_size+(j-1)] + H_K[(i-1)*block_size+j] + H_K[(i+1)*block_size+j]
       + (2 * H_K[i*block_size+j]))/5;              
    }else{
      H[new_track]= (H_K[i*block_size+(j-1)] + H_K[(i-1)*block_size+j] + H_K[(i+1)*block_size+j]
       + H_K[i*block_size+j] + rb3[i])/5; 
    }
  } else if(i==N-1 && j!=N-1) {
    if(rend == N-1 && cend !=N-1){
      H[new_track]= (H_K[i*block_size+(j+1)] + H_K[i*block_size+(j-1)] + H_K[(i-1)*block_size+j]
       + (2 * H_K[i*block_size+j]))/5;          
    }else{
      H[new_track]= (H_K[i*block_size+(j+1)] + H_K[i*block_size+(j-1)] + H_K[(i-1)*block_size+j]
       + H_K[i*block_size+j] + rb4[j])/5;
    }
  } else if(i==0 && j==N-1) {
    if(rstart == 0 && cend ==N-1){
      H[new_track]= (H_K[i*block_size+(j-1)] + H_K[(i+1)*block_size+j] + (3 * H_K[i*block_size+j]))/5;
    }else{
      H[new_track]= (H_K[i*block_size+(j-1)] + H_K[(i+1)*block_size+j] + H_K[i*block_size+j] + rb2[j] + rb3[i])/5;
    }
  } else if(i==N-1 && j==0) {

    if(rend == N-1 && cstart ==0){
      H[new_track]= (H_K[i*block_size+(j+1)] + H_K[(i-1)*block_size+j] + (3 * H_K[i*block_size+j]))/5;
    }else{
      H[new_track]= (H_K[i*block_size+(j+1)] + H_K[(i-1)*block_size+j] + H_K[i*block_size+j] + rb1[i] + rb4[j])/5;
    }
  } else {
    H[new_track]= (H_K[i*block_size+(j+1)] + H_K[(i-1)*block_size+j] + H_K[i*block_size+j] +
     H_K[(i+1)*block_size+j] + H_K[i*block_size+(j-1)])/5;
  }
  new_track++;
}
}
        /*for (int row = 0; row<N; row++) {
         for (int col=0; col<N; col++) {
           std::cout<<H[row*N+col]<<" ";
         }
         std::cout<<std::endl;
        }*/
double **ptr = &H;

//cout<<check2DHeat(p, N, wrank, wsize, it)<<endl;
H_K = H;
}
if(wrank == 0){
     
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cerr<<elapsed_seconds.count()<<std::endl;
}
MPI_Finalize();
return 0;
}

