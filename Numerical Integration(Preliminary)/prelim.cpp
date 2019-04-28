#include <unistd.h>
#include <iostream>

using namespace std;

int main () {

char hostname[256];

gethostname(hostname, sizeof(hostname));
cout<<"Host Name for this pc is : "<<hostname;

  return 0;
}
