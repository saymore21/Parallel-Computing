#include <mpi.h>
#include <iostream>
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

    MPI_Init(&argc, &argv);

    int wrank;
    int wsize;
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
    MPI_Comm_size(MPI_COMM_WORLD, &wsize);

    long _id = stol(argv[1]);
    float temp_val = 0.0;
    float val = 0.0;
    float a = stof(argv[2]);
    float b = stof(argv[3]);
    long n= stol(argv[4]);
    int intensity = stoi(argv[5]);
    float range = (b-a)/n;
    long granularity = ((n/wsize)/8);
    int count = 0;
    MPI_Status status;

    if (argc < 6) {
        std::cerr<<"usage: mpirun "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
        return -1;
    }


    auto initiated = check_time::now();

    if(wrank == 0)
    {
        for(int i = 1; i< wsize; i++)
        {
            if(count != n)
            {
//cout <<" the granularity is"<< granularity <<"\n";
// cout <<" I am in first for loop \n";
              //  cout << "I am sending "<< count << " to id "<< i<<"\n";
                // current = count;
                MPI_Send(&count, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
                count = count + (3 * granularity);
              //  cout << "The count value is now :" << count << "\n" ;
                if(count > n)
                    count  = n;
            }
        }
    }

    int rank;
    int term_res = 0;
    while(count <= n)
    {
        if(wrank == 0)
        {
            float local_sum = 0.0;
           
          //  cout << "The first while count is  " <<count << "\n";
            MPI_Recv(&local_sum, 1, MPI_FLOAT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
           // cout << "I am 0 and I received " << local_sum <<"\n";
            if(local_sum != 1)
                temp_val += local_sum;
            else
            {
                term_res += 1;
                if(term_res == (wsize - 1))
 {
//cout << "I am in if for term_res \n";
//count += 5;
 //cout << " THe countfor 0 is " << count << "\n";
 break;
}
                   // break;
            }

            // MPI_Get_count(&status, MPI_FLOAT, &numofelem);
            rank = status.MPI_SOURCE;
           // cout << " I got rank as " << rank << "\n";
            // current = count;
           // << "Count before sending .." << count << "\n";
            if(count < n)
   	    {
               MPI_Send(&count, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
            //   cout << " I am " << wrank << " and i am sending count "<< count <<" to rank "<< rank << "\n";
               count += (3 * granularity);
	    }
            else if(count >= n)
            {
                count = n;
                MPI_Send(&count, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
            }
            if(count > n)
                count = n;
        }
        else
        {
           // cout << "I am waiting to receibve \n";
            MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            //cout << "I  am  " << wrank << " and I have got my current as " << count <<"\n";
            if(count != n)
            { 
   		for(int l = 0; l < 3; l++)
		{
                int start_iteration = count;
                int end_iteration = start_iteration + granularity;
                if(end_iteration > n)
                    end_iteration = n;

                float local_sum = 0.0;
                if (_id == 1)
                {
                  //  cout<< "I am "<< wrank << " and I entered " << _id <<" with start as " << start_iteration << " and end as " << end_iteration << "\n";
                    for(int i= start_iteration; i< end_iteration; i++)
                    {
                        float firstparam = a+(i+0.5)*(range);
                        local_sum += f1(firstparam,  intensity);
                    }
		   
                  // cout<< "I am "<<_id<< " and my local_sum val is "<< local_sum << "\n";

                }
                else if (_id == 2)
                {

                   // cout<< "I entered " << _id <<" with start as" << start_iteration << "and end as " << end_iteration << "\n";
                    for(float i= start_iteration; i < end_iteration; i++)
                    {
                        float firstparam = a+(i+0.5)*(range);
                        local_sum += f2(firstparam, intensity);
                    }
                 //   cout<< "My local_sum val is "<< local_sum << "\n";

                }
                else if (_id == 3)
                {
                   // cout<< "I entered " << _id <<" with start as" << start_iteration << "and end as " << end_iteration << "\n";
                    for(int i= start_iteration; i< end_iteration; i++)
                    {
                        float firstparam = a+(i+0.5)*(range);
                        local_sum += f3(firstparam, intensity);
                    }
                  //  cout<< "My local_sum val is "<< local_sum << "\n";

                }
                else if (_id == 4)
                {
                   // cout<< "I entered " << _id <<" with start as" << start_iteration << "and end as " << end_iteration << "\n";
                    for(int i= start_iteration; i<end_iteration; i++)
                    {
                        float firstparam = a+(i+0.5)*(range);
                        local_sum += f4(firstparam, intensity);
                    }
                   // cout<< "My local val is "<< local_sum << "\n";
                }
                MPI_Send(&local_sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
		count = end_iteration;
		}
                //current = local_sum;
                //count  = n;
                //if(end_iteration <= n)
               //     MPI_Send(&local_sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
               /* else if(start_iteration > n)
                {
                    local_sum = 1;
                    MPI_Send(&local_sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                }*/
            }
            else if(count == n)
             {
              float local_sum = 1;
              MPI_Send(&local_sum, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            //  cout << "I entered else \n";
              count += 5;
     	      //cout << "The count is now " << count << "\n";
     	      break;
           }

       }
   // cout << "I am going to check while condition again and for me count is now " << count << " and n is " << n << "\n";  
    }

    auto end = check_time::now();
    auto time_elapsed = end - initiated;
    auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);



    if(wrank == 0)
    {
       // cout << "YThe temp_val at end " << temp_val << "\n";
       // cout << "the range " << range << "\n";
        val = temp_val*range;
        cout<<val;
        cerr<<secs.count();
    }

    MPI_Finalize();
    return 0;
}
