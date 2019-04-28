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
	
float val = 0.0;
int _id = 0;
float a = 0;
float b = 0;
int n= 0;
int intensity =0;
int nbthreads = 0;
int granularity = 0 ;
int main_begin = 0;
int main_end = 0;
int rangeleft =0 ;
float range =0.0;
pthread_mutex_t mut;
pthread_mutex_t mut1;


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
int remain = 0;
//cout << " while entering done rangeleft " << rangeleft  << "\n";
pthread_mutex_lock(&mut1);
if(rangeleft == 0)
{
//pthread_mutex_unlock(&mut);
//cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
pthread_mutex_unlock(&mut1);
return true;
}
else
{
//cout << "got it!" << "\n";
//remain = (n-rangeleft);
if(rangeleft >= granularity)
{
//pthread_mutex_lock(&mut1);
rangeleft = rangeleft - granularity;
//cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
}
else
{
//pthread_mutex_lock(&mut1);
rangeleft = 0;
} pthread_mutex_unlock(&mut1);
//cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
//cout << "while leaving rangeleft " << rangeleft << "\n";
return false;
}
//cout << "while leaving rangeleft " << rangeleft << "\n";
}


void getnext (int *begin, int *end)
{
pthread_mutex_lock(&mut1);
//cout<< "entered getnext " << "\n" ;
//*begin = 2;
//*end = 20;
*begin = main_begin;
*end = main_end;
int lb =  main_end;
//cout << " i am lb" << lb << "i am main_begin" << main_begin << " i am main_end" << main_end << "\n";
int remain = n - lb;
if(remain >= granularity)
{
:: main_begin = *end;
:: main_end = main_begin + granularity;
}
/*else
{
pthread_mutex_lock(&mut1);
:: main_begin = *end +1;
:: main_end = n;
pthread_mutex_unlock(&mut1);
} */

//cout <<"Leaving getnext() begin " << *begin << " end : " << *end << "\n" ;
pthread_mutex_unlock(&mut1);
}



void* call_iter_function(void* j)
{
     //cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
     //cout << " the value of n " << n << " main end " << :: main_end << "rangeleft " << rangeleft <<"\n";
      float cnstnt = range;
      int i, begin_index, end_index;
      //std::cout << "hi am thread "<< i << " with begin : " <<begin << " end : " << endloop;
      // long i = (long)j;
      //std::cout << "entered iter ";
      //cout<< " calling done";
       while(!done())
	{
        //cout<< " calling getnext";
	getnext(&begin_index, &end_index);
        //pthread_mutex_unlock(&mut);
	//std::cout << "hi am thread "<< i << " with begin : " <<begin << " end : " << endloop << "\n";
        //}
        //cout << "a : " << a <<" b :" << b <<" n : " << n << " inten : " << 
        //intensity <<" nbthreads : "<<nbthreads << "range : " << range << " \n" ;
        //std::cout << "begin" <<begin << "end" << end  << " \n" ;

         if (_id == 1)
		{

			for(i=begin_index; i<end_index; i++)
				{
//cout << "range " << cnstnt << "\n";
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*cnstnt;	
					val += f1(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}	
	else if (_id == 2)
		{
			for(i=begin_index; i<end_index; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*cnstnt;	
					val += f2(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}
	else if (_id == 3)
		{
			for(i=begin_index; i<end_index; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*cnstnt;
					val += f3(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}
	else if (_id == 4)
		{
		 	for(i=begin_index; i<end_index; i++)
				{
					pthread_mutex_lock(&mut);
					float firstparam = a+(i+0.5)*cnstnt;
					val += f4(firstparam, intensity);
					pthread_mutex_unlock(&mut);
				}
		}
}
         //cout << "Val : "<< val;
}



void* call_chunk_function(void* j)
{
     // float cnstnt = range;
      long thread= (long) j;
      float tempry;
      int i, begin_index, end_index;

      while(!done())
	{
	getnext(&begin_index, &end_index);
	float firstparam = 0.0;
	tempry = 0.0;
float cnstnt = range;
      if (_id == 1)
		{
//cout << " started with the value of begin and end "<< begin_index << "and" << end_index  <<  "\n";
			for(i=begin_index; i<end_index; i++)
				{					
					float firstparam = a+(i+0.5)*cnstnt;
					tempry += f1(firstparam, intensity);					
				}
		}	
	else if (_id == 2)
		{
			for(i=begin_index; i<end_index; i++)
				{				
					float firstparam = a+(i+0.5)*cnstnt;
					tempry += f2(firstparam, intensity);
				}
		}
	else if (_id == 3)
		{
//cout << " for thread " << thread <<" value of begin and end "<< begin_index << "and" << end_index  <<  "\n";
			for(i=begin_index; i<end_index; i++)
				{
					float firstparam = a+(i+0.5)*cnstnt;
					tempry += f3(firstparam, intensity);
				}
		}
	else if (_id == 4)
		{
		 	for(i=begin_index; i<end_index; i++)
				{
					float firstparam = a+(i+0.5)*cnstnt;
					tempry += f4(firstparam, intensity);
				}
		}
//cout << " calculated local result as  "<< tempry  << "by thread" << thread << "\n";
	pthread_mutex_lock(&mut);
//cout << "calculate VAL before adding "<< val  << "\n";
	val = val + tempry;
//cout << "after entering the val "<< val << "\n";
	pthread_mutex_unlock(&mut);
//cout << "Val : "<< val << " \n";
}
}


void* call_thread_function(void* j)
{

	long thread = (long)j;
     // cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
      int i,begin_index, end_index;
      float local_res = 0.0;
      float cnstnt = range;
       while(!done())
	{
//cout<< "i am working" ;
	getnext(&begin_index, &end_index);

        if (_id == 1)
		{
//cout << " for value of begin and end"<< begin_index << "and" << end_index  <<  "\n";
/*cout << "a : " << a << " n : " << n << " inten : " << intensity << "cons" << cnstnt
<< "begin " << begin_index << "end " << end_index << "\n";*/
//cout << " for value of begin and end "<< begin_index << "and" << end_index  <<  "\n";
			for(i=begin_index; i<end_index; i++)
				{
					float firstparam = a+(i+0.5)*cnstnt;	
					local_res += f1(firstparam, intensity);

				}
//cout << " calculated local result as  "<< local_res  << "\n";
		}	
	else if (_id == 2)
		{
			for(i=begin_index; i<end_index; i++)
				{
					float firstparam = a+(i+0.5)*cnstnt;	
					local_res += f2(firstparam, intensity);
				}
		}
	else if (_id == 3)
		{
			for(i=begin_index; i<end_index; i++)
				{
					float firstparam = a+(i+0.5)*cnstnt;
					local_res += f3(firstparam, intensity);
				}
		}
	else if (_id == 4)
		{
		 	for(i=begin_index; i<end_index; i++)
				{
					float firstparam = a+(i+0.5)*cnstnt;
					local_res += f4(firstparam, intensity);
				}
		}
}

//cout << " calculated local result as  "<< local_res  << "\n";
	pthread_mutex_lock(&mut);
//cout << " calculated sum for a whole chunk to add "<< local_res  << "\n";
//cout << "calculated VAL before adding "<< val  << "\n";
	val = val + local_res;
//cout << "entering the val "<< val << "\n";
	pthread_mutex_unlock(&mut);
//cout << "Val : "<< val;
}



int main (int argc, char* argv[])
{
  // std :: cout << "Started main";
   auto initiated = check_time::now();

  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }	
  	int i,j = 0;
	:: _id = stoi(argv[1]);
	:: a = stoi(argv[2]);
	:: b = stof(argv[3]);
	:: n= stof(argv[4]);
	:: intensity = stoi(argv[5]);
	:: nbthreads = stoi(argv[6]);
	:: string sync = argv[7];
	:: granularity = stoi(argv[8]);
	:: main_end = granularity;
	:: range = (b-a)/n;
	transform(sync.begin(), sync.end(), sync.begin(), ::tolower);
	pthread_t thread[nbthreads];
	//cout << "a : " << a <<" b :" << b <<" n : " << n << " inten : " << 
       // intensity <<" nbthreads : "<<nbthreads << " sync : " << sync;
	//struct range_t *range_thread;
	//struct range_t range_thread_array[nbthreads];
        ::rangeleft =  n;
	//cout << "rangeleft " << rangeleft << "granularity " << granularity << "\n";
	pthread_mutex_init(&mut, NULL); 
	pthread_mutex_init(&mut1, NULL); 
        // std::cout << "going to iterate threads ";

	if(sync == "iteration")
	{
	for(j= 0; j<nbthreads;j++)
	{
	 pthread_create(&thread[j], NULL, call_iter_function, NULL);
	}	
	}
	else if(sync == "chunk")
	{
	for(j= 0; j<nbthreads;j++)
	{
	 pthread_create(&thread[j], NULL, call_chunk_function, NULL);
	}
	}
	else if(sync == "thread")
	{
	for(j= 0; j<nbthreads;j++)
	{
	 pthread_create(&thread[j], NULL, call_thread_function,NULL);
	}
	}

	for(int x=0; x <nbthreads; x++)
	{
	 pthread_join(thread[x], NULL);
	}
        
	pthread_mutex_destroy(&mut);
pthread_mutex_destroy(&mut1);
//cout<<"value final " << val << "\n";
	//cout << "range at the end : " << range << "\n";
 	val = val*range;

	cout<<val;

	auto end = check_time::now();
	auto time_elapsed = end - initiated;
	auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);

	cerr<<secs.count()<< "\n";
  
  return 0;
}
