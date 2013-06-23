#include "braid.h"

braid::braid(const char* bstring, const int format)
{
  istringstream bs(bstring);
  strand=1;
  if(format==BRAID_FORMAT_0)
    {
      long i,k;
      while(bs)
	{
	  bs >> i >> k;
	  if(bs) push(i,k);
	}
    }
  if(format==BRAID_FORMAT_1)
    {
      long i,k;
      k=1;
      while(bs)
	{
	  bs >> i;
	  if(bs) push(i,k);
	  k=-k;
	}
    }
}

void braid::push(const long i, const long k)
{
  vector<long> x(2);
  x[0]=i; x[1]=k;
  word.push_back(x);
  if(strand <= i) strand=i+1;
}

void braid::read(const char* fname)
{
  ifstream bs(fname);
  long i,k;
  if(bs)
    {
      while(bs)
	{
	  bs >> i >> k;
	  if(bs) push(i,k);
	}
      bs.close();
    }
}
 
void braid::print(const int format)
{
  if(format==BRAID_FORMAT_0)
    {
      for(int i=0; i<word.size()-1;i++)
	cout << word[i][0] << " " << word[i][1] << " ";
      cout << word[word.size()-1][0] << " " << word[word.size()-1][1];
    }
  if(format==BRAID_FORMAT_1)
    {
      for(int i=0; i<word.size()-1;i++)
	cout << word[i][0] << " ";
      cout << word[word.size()-1][0];
    }
}

lamination::lamination(long n)
{
  a.resize(n);
  b.resize(n);
  for(long i=0; i<n; i++)
    {
      a[i]=0; b[i]=1;
    }
}

void lamination::print()
{
  long n=punct();
  for(long i=0; i<n; i++)
    {
      cout << " " << a[i] << " " << b[i]; 
    }
  cout << endl;
}

void lamination::twist(const long k, const int sign)
{
  long n=a.size();
  long i=k-1;
  ZZ zero=to_ZZ(0);
  if(0<=i && i<(n-1) && sign==1)
    {
      ZZ c = a[i] - min(b[i], zero) - a[i+1] + max(b[i+1], zero);
      ZZ x,y,z,t;
      x = a[i] + max(b[i], zero) + max(max(b[i+1], zero) - c, zero);
      y = b[i+1] - max(c,zero);
      z = a[i+1] + min(b[i+1], zero) + min(min(b[i], zero)+c, zero);
      t = b[i] + max(c, zero);
      a[i]=x;
      b[i]=y;
      a[i+1]=z;
      b[i+1]=t;
    }
  if(0<=i && i<(n-1) && sign==-1)
    {
      ZZ d = a[i] + min(b[i], zero) - a[i+1] - max(b[i+1], zero);
      ZZ x,y,z,t;
      x = a[i] - max(b[i], zero) - max(max(b[i+1], zero) + d, zero);
      y = b[i+1] + min(d, zero);
      z = a[i+1] - min(b[i+1], zero) - min(min(b[i], zero) - d, zero);
      t = b[i] - min(d, zero);
      a[i]=x;
      b[i]=y;
      a[i+1]=z;
      b[i+1]=t;
    }
}

void lamination::twist(const braid br)
{
  for(long i=0; i<br.size(); i++)
    {
      long g=br.get_gen(i);
      long p=br.get_pow(i);
      if(p>0)
	{
	  for(long j=0; j<p; j++) 
	    twist(g,1);
	}
      if(p<0)
	{
	  for(long j=0; j<-p; j++) 
	    twist(g,-1);
	}
    }
}

void lamination::twist(const braid br, const long n)
{
  for(long i=0; i<n; i++)
    twist(br);
}

ZZ intersection(const lamination &l)
{
  long n=l.punct();
  vector<ZZ> g(n);
  vector<ZZ> d(n);
  vector<ZZ> a=l.get_a();
  vector<ZZ> b=l.get_b();
  ZZ zero=to_ZZ(0);

  for(long i=0; i<n; i++)
    {
      g[i] = 2*abs(a[i]) + 2*max(b[i], zero);
      d[i] = 2*abs(a[i]) + 2*max(-b[i], zero);
    }
  
  ZZ first_nu;
  first_nu=g[0];
  for(long i=0; i<n-1; i++)
    first_nu=first_nu + max(g[i+1] - d[i], zero);
  
  ZZ last_nu;
  last_nu=d[n-1];
  for(long i=0; i<n-1; i++)
    last_nu=last_nu + max(d[i] - g[i+1], zero);

  ZZ inter;
  inter = first_nu/2 + last_nu/2 + abs(a[0]) + abs(a[n-1]);
  for(long i=0; i<n; i++)
    inter+=abs(b[i]);
  
  for(long i=0; i<n-1; i++)
    inter+=abs(a[i+1] - a[i]);
  
  return inter;
}

vector<ZZ> intersections(const lamination &l)
{
  long n=l.punct();
  vector<ZZ> g(n);
  vector<ZZ> d(n);
  vector<ZZ> a=l.get_a();
  vector<ZZ> b=l.get_b();
  ZZ zero=to_ZZ(0);
  
  for(long i=0; i<n; i++)
    {
      g[i] = 2*abs(a[i]) + 2*max(b[i], zero);
      d[i] = 2*abs(a[i]) + 2*max(-b[i], zero);
    }

  vector<ZZ> inter;
  inter.resize(n+1);
  
  inter[0]=abs(a[0]) - min(b[0], zero);
  inter[n]=abs(a[n-1]) + max(b[n-1], zero);

  for(long j=1; j<=n-1;j++)
    inter[j] = abs(a[j]-a[j-1]) + max(b[j-1], zero)-min(b[j], zero);

  return inter;
}

double entropy(braid b, const double eps)
{
  long n=b.get_strand();
  lamination l(n);
  
  long p=1;
  int cont=1;
  RR h=to_RR(1.);
  RR hbak=h;
  while(cont)
    {
      l.twist(b);
      hbak=h; 
      ZZ inter=intersection(l);
      if(inter>=2)
	h=log(to_RR(inter))/p;
      p+=1;
      if(abs(h-hbak)<eps) cont=0;
    }
  return to_double(h);
}

vector<RR> entropies(braid b, const double eps)
{
  long n=b.get_strand();
  lamination l(n);
  
  long p=1;
  int cont=1;
  vector<RR> h(n+1,to_RR(0.));
  vector<RR> hbak=h;
  while(cont)
    {
      l.twist(b);
      vector<ZZ> inter=intersections(l);
      hbak=h;
      for(long j=0; j<inter.size();j++)
	{
	  if(inter[j]>=2)
	    h[j]=log(to_RR(inter[j]))/p;
	}
      p+=1;
      cont=0;
      for(long j=0; j<inter.size();j++)
	{
	  if(abs(h[j]-hbak[j])>eps) cont=1;
	}
    }
  return h;
}
