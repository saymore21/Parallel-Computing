#include <mpi.h>


int main (int argc, char* argv[]) {
MPI_Init (&argc, &argv);
int wrank;
int wsize;
MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
MPI_Comm_size(MPI_COMM_WORLD, &wsize);
MPI_Comm splitcomm;
MPI_Comm_split (MPI_COMM_WORLD, wrank%(wsize/2), wrank, &splitcomm);
int crank;
int csize;
MPI_Comm_rank(splitcomm, &crank);
MPI_Comm_size(splitcomm, &csize);
std::cout<<"I am "<<wrank<<" out of "<<wsize<<" in COMM_WORLD and "<<crank<<" out of "<<csize<<" in split comm"<<std::endl;
MPI_Finalize();
return 0;
}