#include <vector>
#include <NTL/ZZ.h>

NTL_CLIENT
using namespace std;

#ifndef UTILS
#define UTILS

template <class T>
void print(const vector<T> &u)
{
    long n=u.size();
  for(long i=0; i<n; i++)
    cout << "[" << u[i] << "]";
  cout << endl;
}

#endif
