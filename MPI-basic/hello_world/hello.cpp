#include <mpi.h>
#include <unistd.h>
#include <iostream>

int main(int argc, char*argv[]) 
{

int rank, size,flag;
char hostname[100];

MPI_Init(& argc, & argv);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

flag = gethostname(hostname, sizeof(hostname));
printf("I am process %d out of %d. I am running on machine %s\n", rank, size, hostname);


MPI_Finalize();

return 0;
}
