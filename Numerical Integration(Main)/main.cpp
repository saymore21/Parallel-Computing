#include <iostream>
#include <cmath>
#include <cstdlib>
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

  
int main (int argc, char* argv[]) 
{
	
	auto initiated = check_time::now();

  	int i;
	int _id = stoi(argv[1]);	
	float val = 0;
        float a = stof(argv[2]);
	float b = stof(argv[3]);
	int n= stoi(argv[4]);
	int intensity = stoi(argv[5]);
	float range = (b-a)/n;
	
	if (argc < 6)
 		{
    
  			cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<endl;
 
 			return -1;

 		 }
 
	else if (_id == 1)
		{
			for(i=0; i<=n-1; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					val += f1(firstparam, intensity);
				}
		}	
	else if (_id == 2)
		{
			for(i=0; i<=n-1; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					val += f2(firstparam, intensity);
				}
		}
	else if (_id == 3)
		{
			for(i=0; i<=n-1; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					val += f3(firstparam, intensity);
				}
		}
	else if (_id == 4)
		{
		 	for(i=0; i<=n-1; i++)
				{
					float firstparam = a+(i+0.5)*(range);	
					val += f4(firstparam, intensity);
				}
		}

 	val = val*range;

	cout<<val;
	auto end = check_time::now();
	auto time_elapsed = end - initiated;
	auto secs = std::chrono::duration_cast<std::chrono::duration<float>>(time_elapsed);

	cerr<<secs.count();
	
	return 0;
}
