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
#include "braid.h"
#include "utils.h"

NTL_CLIENT

using namespace std;

const long MAX_LINE_SIZE=1048576;


void display_help()
{
  cout << endl;
  cout << "htop computes the topological entropy of braids\n" << endl;
  cout << "Usage : htop [-e eps] [-f fmt] file\n" << endl;
  cout << "-e eps \t Stops the computation when 10^-eps precision is reached," << endl;
  cout << "\t default is eps = 4.\n" << endl;  
  cout << "-f fmt \t Specifies braid format. If fmt=0 (default)," << endl;
  cout << "\t 2 -1 1 1 denotes the braid s1 s2^-1.\n" << endl;
  cout << "\t If fmt=1 the braid is alternated:" << endl;
  cout << "\t 1 2 3 2 denotes the braid s1 s2^-1 s3 s2^-1.\n" << endl;
  cout << "file \t Contains one braid per line." << endl;
  cout << endl;
}

int main(int argc, char** argv)
{
  ZZ a;
  char* braid_filename;
  string prec_opt,form_opt;
  unsigned int braid_format=0;
  double eps=1e-4;
  
  if(argc==1)
    {
      display_help();
      exit(0);
    }
  if(argc==2) braid_filename=argv[1]; 
  if(argc==3) 
    {
      display_help();
      exit(0);
    }
  if(argc==4)
    {
      prec_opt=argv[1];
      double epsexp;
      if (prec_opt=="-e")
	{
	  epsexp=atof(argv[2]);
	  eps=pow(10.,-epsexp);
	}
      else
	{
	  display_help();
	  exit(0);
      	}
      braid_filename=argv[3]; 
    }
  if(argc==6)
    {
      prec_opt=argv[1];
      form_opt=argv[3];
      double epsexp;
      
      if (prec_opt=="-e" && form_opt=="-f")
	{
	  epsexp=atof(argv[2]);
	  eps=pow(10.,-epsexp);
	  braid_format=atoi(argv[4]);
	  if(braid_format!=0 && braid_format!=1)
	    {
	      display_help();
	      exit(0);
	    }
	}
      else
	{
	  display_help();
	  exit(0);
      	}
      braid_filename=argv[5]; 
    }
  if(argc>6) 
    {
      display_help();
      exit(0);
    }

  ifstream bstream(braid_filename);
  char bstring[MAX_LINE_SIZE];
  
  if(!bstream)
    { 
      display_help();
      exit(0);
    }

  while(bstream)
    {
      bstream.getline(bstring, MAX_LINE_SIZE);      
      if(bstream.gcount()>=2) 
	{
	  braid br(bstring, braid_format);
	  cout << entropy(br, eps) << endl;
	  //print(entropies(br,eps));
	}
   }
  return 0;
}
