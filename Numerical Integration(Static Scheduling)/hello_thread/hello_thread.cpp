#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string>
#include <chrono>

int numthreads = 0;

void* print(void* nbthreads)
{
long thread_id = (long) nbthreads;
std::cout << "I am thread "<< thread_id <<" in "<< numthreads << "\n";
//pthread_exit(0);
}


int main (int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
    return -1;
  }

:: numthreads = std::stoi(argv[1]);
pthread_t thread[(numthreads - 1)];

for(long i = 0 ; i < numthreads; i++)
{
	pthread_create(&thread[i], NULL, print, (void *)i );
}

for(int j=0; j < numthreads; j++)
{
	 pthread_join(thread[j], NULL);
}

  return 0;
}
