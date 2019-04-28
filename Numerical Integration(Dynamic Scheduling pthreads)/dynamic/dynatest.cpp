#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>
#include <chrono>
#include <cmath>
#include <algorithm>


using namespace std;
using seconds = chrono::seconds;
using check_time = std::chrono::high_resolution_clock;
	
float val = 0;
int _id = 0;
float a = 0;
float b = 0;
int n= 0;
int intensity =0;
int nbthreads = 0;
int granularity = 10 ;
int main_begin = 0;
int main_end = granularity;
int rangeleft =0 ;
pthread_mutex_t mut;
pthread_mutex_t mut1;



struct range_t {

float begin;
float end;
float range;

};


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

bool done()
{
//cout << " while entering done rangeleft " << rangeleft << "granularity " << granularity << "\n";
//pthread_mutex_lock(&mut);
int remain = 0;
if(rangeleft == 0)
{
cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
return true;
}
else
{
//cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
//remain = (n-rangeleft);
if(rangeleft >= granularity)
{
pthread_mutex_lock(&mut);
rangeleft = rangeleft - granularity;
pthread_mutex_unlock(&mut);
//cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
}
else
{
pthread_mutex_lock(&mut);
rangeleft = 0;
pthread_mutex_unlock(&mut);
} 
cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
return false;
}
}


void getnext (int *begin, int *end)
{
cout<< "entered getnext " << "\n" ;
//*begin = 2;
//*end = 20;
*begin = ::main_begin;
*end = ::main_end;
int lb = :: main_end;
//cout << " i am lb" << lb << "i am main_begin" << main_begin << " i am main_end" << main_end << "\n";
int remain = n - lb;
if(remain >= granularity)
{
pthread_mutex_lock(&mut1);
:: main_begin = *end;
:: main_end = :: main_begin + granularity;
pthread_mutex_unlock(&mut1);
}
else
{
pthread_mutex_lock(&mut1);
:: main_begin = *end +1;
:: main_end = n;
pthread_mutex_unlock(&mut1);
} 
}



void* call_iter_function(void* j)
{
cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";

//cout << " the value of n " << n << " main end " << :: main_end << "rangeleft " << rangeleft <<"\n";
      long i = (long)j;
      int begin, endloop;
//std::cout << "hi am thread "<< i << " with begin : " <<begin << " end : " << endloop;
      float range;
     // long i = (long)j;
      //std::cout << "entered iter ";
      /*range_t* my_range = (range_t*)range_thread;
      begin = my_range -> begin;
      end = my_range -> end;
      range  = my_range -> range;*/
//cout<< " calling done";
       while(!done())
	{
//cout<< " calling getnext";
	getnext(&begin, &endloop);
//pthread_mutex_unlock(&mut);
	std::cout << "hi am thread "<< i << " with begin : " <<begin << " end : " << endloop << "\n";
        //}
      //cout << "a : " << a <<" b :" << b <<" n : " << n << " inten : " << 
        //intensity <<" nbthreads : "<<nbthreads << "range : " << range << " \n" ;
      //std::cout << "begin" <<begin << "end" << end  << " \n" ;

     /* if (_id == 1)
		{

			for(i=begin; i<=end; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*range;	
					val += f1(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}	
	else if (_id == 2)
		{
			for(i=begin; i<=end; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*range;	
					val += f2(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}
	else if (_id == 3)
		{
			for(i=begin; i<=end; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*range;
					val += f3(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}
	else if (_id == 4)
		{
		 	for(i=begin; i<=end; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*range;
					val += f4(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}*/
}
//cout << "Val : "<< val;
}



void* call_thread_function(void* j)
{
     /* int begin, end;
      float range;
      long i = (long)j;
     // std::cout << "entered thread ";
      /*range_t* my_range = (range_t*)range_thread;
      begin = my_range -> begin;
      end = my_range -> end;
      range  = my_range -> range;*/
      //std::cout << "begin" <<begin << "end" << end;
     // float firstparam = 0.0;
     // float tempry = 0;
     // pthread_mutex_lock(&mut1);
     /* while(!done())
	{
	getnext(&begin , &end);
        pthread_mutex_lock(&mut1);
	std::cout << "hi am thread "<< i << " with begin : " <<begin << " end : " << end;
        }
     /* if (_id == 1)
		{

			for(i=begin; i<=end; i++)
				{					
					float firstparam = a+(i+0.5)*range;
					tempry += f1(firstparam, intensity);					
				}
		}	
	else if (_id == 2)
		{
			for(i=begin; i<=end; i++)
				{				
					float firstparam = a+(i+0.5)*range;
					tempry += f2(firstparam, intensity);
				}
		}
	else if (_id == 3)
		{
			for(i=begin; i<=end; i++)
				{
					float firstparam = a+(i+0.5)*range;
					tempry += f3(firstparam, intensity);
				}
		}
	else if (_id == 4)
		{
		 	for(i=begin; i<=end; i++)
				{
					float firstparam = a+(i+0.5)*range;
					tempry += f4(firstparam, intensity);
				}
		}

	pthread_mutex_lock(&mut);
        val = val + tempry;
        pthread_mutex_unlock(&mut);
*/
}


int main (int argc, char* argv[]) {
  // std :: cout << "Started main";

  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }

 // std :: cout << "Started main ";
  // auto initiated = check_time::now();

	
  	long i,j = 0;
	:: _id = stoi(argv[1]);
	:: a = stoi(argv[2]);
	:: b = stof(argv[3]);
	:: n= stof(argv[4]);
	:: intensity = stoi(argv[5]);
	:: nbthreads = stoi(argv[6]);
	:: string sync = argv[7];
	float range = (b-a)/n;
	//:: granularity = n /nbthreads;
	/*int begin_loop= 0;
	int end_loop= (begin_loop +(thread_limit - 1));
	transform(sync.begin(), sync.end(), sync.begin(), ::tolower);*/
	pthread_t thread[nbthreads];
	//cout << "a : " << a <<" b :" << b <<" n : " << n << " inten : " << 
       // intensity <<" nbthreads : "<<nbthreads << " sync : " << sync;
	struct range_t *range_thread;
	struct range_t range_thread_array[nbthreads];
        ::rangeleft =  n;
	cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
	pthread_mutex_init(&mut, NULL); 
	pthread_mutex_init(&mut1, NULL); 

        // std::cout << "going to iterate threads ";

	if(sync == "iteration")
	{
	for(j= 0; j<nbthreads;j++)
	{
	 /*range_thread_array[j].begin = begin_loop;
	 range_thread_array[j].end = end_loop;
	 range_thread_array[j].range = range;*/
	 pthread_create(&thread[j], NULL, call_iter_function, (void *)j);
	/* begin_loop =(end_loop +1);
	 end_loop = (begin_loop + (thread_limit - 1));*/
	}	
	}
	else if(sync == "thread")
	{
	for(j= 0; j<nbthreads;j++)
	{
	 /*range_thread_array[j].begin = begin_loop;
	 range_thread_array[j].end = end_loop;
	 range_thread_array[j].range = range;*/
	 pthread_create(&thread[j], NULL, call_thread_function, (void *)j);
	 /*begin_loop =(end_loop +1);
	 end_loop = (begin_loop + (thread_limit - 1));*/
	}
	}

	for(int j=0; j < nbthreads; j++)
	{
	 pthread_join(thread[j], NULL);
	}
        
	pthread_mutex_destroy(&mut);
        pthread_mutex_destroy(&mut1);
//cout << "range at the end : " << range;
 	val = val*range;

	//cout<<val;

	//auto end = check_time::now();
	//auto time_elapsed = end - initiated;
	//auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);

	//cerr<<secs.count();
  
  return 0;
}
