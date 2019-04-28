#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <mpi.h>
#include <chrono>

using namespace std;
using seconds = chrono::seconds;
using check_time = std::chrono::high_resolution_clock;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

  
int main (int argc, char* argv[]) {

int i, rank, size;
 MPI_Init(&argc, &argv);  
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

  
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }
	
	

  	
	long _id = stol(argv[1]);
	float temp_val = 0;	



        float val = 0;
        float a = stof(argv[2]);
	float b = stof(argv[3]);
	long n= stol(argv[4]);
	int intensity = stoi(argv[5]);
	float range = (b-a)/n;
	long nofiterations = (n/size);

	
       
	/*long *sendbuf;
	sendbuf = (long *)malloc(n*sizeof(long));
    
	for(long j = 0; j<n;j++)
	{
	 sendbuf[j] = j;
	}*/
	long start_iteration , end_iteration;
	start_iteration = rank*nofiterations;
	end_iteration = (rank+1)*nofiterations;
	
	if(rank == (size -1))
	{
	if(n%size != 0)
	{
	end_iteration = n;
	}
	}

	
	auto initiated = check_time::now();
	
	if (argc < 6)
 		{
    
  			cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<endl;
 
 			return -1;

 		 }
 
	else if (_id == 1)
		{
//printf( "Range is %d to %d for ranke %d", rank*nofiterations, (rank+1)*nofiterations, rank);
			for(i= start_iteration; i< end_iteration; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					temp_val += f1(firstparam,  intensity);
				}
//printf(" Iam rank %d with raange %d %d and mt temp_val is %f", rank, rank*nofiterations,(rank+1)*nofiterationstemp_val
		}	
	else if (_id == 2)
		{
			for(i= start_iteration; i< end_iteration; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					temp_val += f2(firstparam, intensity);
				}
		}
	else if (_id == 3)
		{
			for(i= start_iteration; i< end_iteration; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					temp_val += f3(firstparam, intensity);
				}
		}
	else if (_id == 4)
		{
		 	for(i= start_iteration; i<end_iteration; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					temp_val += f4(firstparam, intensity);
				}
		}

	
	MPI_Reduce(&temp_val, &val, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);


	auto end = check_time::now();
	auto time_elapsed = end - initiated;
	auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);
 

	if(rank == 0)
	{
		val = val*range;
		cout<<val;
	}

	if(rank == 0)
	{
		cerr<<secs.count();
	}

	MPI_Finalize();
	return 0;
}
