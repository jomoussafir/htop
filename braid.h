#include <string>
#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <fstream>

#include <math.h>
#include <complex.h>
#include <NTL/ZZ.h>
#include <NTL/mat_ZZ.h> 
#include <NTL/RR.h> 
#include "utils.h"

#ifndef CLASS_BRAID
#define CLASS_BRAID

NTL_CLIENT
using namespace std;
//using namespace NTL;

const int BRAID_FORMAT_0=0;
const int BRAID_FORMAT_1=1;

class braid
{
public:
  braid(){ strand=1; word.clear();}
  braid(const char* bstring, const int format=BRAID_FORMAT_0);
  // push(i,k) adds s_i^k at the beginning of 
  // the word coding the braid 
  void push(const long i, const long k);
  // size() returns the number of terms s_i^k
  long size() const {return word.size();}
  // get_strand() returns the number of strands
  // NB : there are n+1 stands for Br(n)
  long get_strand() const {return strand;}
  // A braid is coded as a sequence of s_i^k ( (i_1, k_1), ..., (i_m, k_m))
  // get_gen(j) returns i_j
  // get_pow(j) returns k_j
  long get_gen(const long i) const {return word[i][0];}
  long get_pow(const long i) const {return word[i][1];}
  // read() reads a file with braids in it
  // There are two formats
  // 2 -1 3 2 5 1 stands for s2^-1 s3^2 s5^-1 in format BRAID_FORMAT_0
  // 3 4 3 2 stands for 2^1 3^-1 4^1 3^-1 in format BRAID_FORMAT_1
  void read(const char* fname);
  // print() prints
  void print(const int format=BRAID_FORMAT_0);
  
private:
  long strand;
  vector<vector<long> > word;
};

class lamination
{
  // Laminations are coded using I. Dynnikov's 
  // coordinates ((a_0, b_0),...,(a_(n-1), b_(n-1))
  // NB : If the coding of a lamination uses 2*n
  // integers, there are n curves. The braid group acting 
  // on it is Br(n-1), there are n+2 points in the complex plane
  // and one additional point at infinity
public:
  lamination(){a.resize(0);b.resize(0);}
  // lamination(n) creates the lamination (0,1,0,1...,0,1)
  lamination(long n);
  // punct() returns the number of strands corresponding
  // to the lamination
  long punct() const {return a.size();}
  // get_a() and get_b() return a and b respectively.
  // These vectors are indexed from 0 to punct()-1.
  vector<ZZ> get_a() const {return a;}
  vector<ZZ> get_b() const {return b;}
  void print();
  // twist(k,sign) modifies the Dynnikov's coding
  // according to the action of s_k^sign
  void twist(const long k, const int sign);
  // twist(br) modifies the Dynnikov's coding
  // according to the action of br
  void twist(const braid br);
  // twist(br,n) modifies the Dynnikov's coding
  // according to the action of br^n
  void twist(const braid br, const long n);

private:
  vector<ZZ> a;
  vector<ZZ> b;
};

// intersection() computes the cardinal of the intersection
// of the lamination with the real axis. 
ZZ intersection(const lamination &l);

// intersections() compute all cardinal of intersections 
// of the lamination with the real axis.
// If there are n strands, then the returned vector
// is of size n+1.
vector<ZZ> intersections(const lamination &l);

// entropy() returns the entropy of a braid. The algorithm
// computes the sequence log ( intersection ( l.twist(br^k) ) )/k.
// It stops when two terms in the sequence differ less than eps.
double entropy(braid b, const double eps=1e-4);

// entropies() returns the n+1 dimensionnal vector
// of entropies corresponding to the n+1 dimmensional
// vector intersections().
vector<RR> entropies(braid b, const double eps=1e-4);

#endif
