#include <iostream>
#include <cmath>
#include <random>
#include <string>

using namespace std;

random_device r;
mt19937 rgen(r());
uniform_real_distribution<> rdis(0,1);

const  int N_max = 100;
int        N {};

struct XYZ { double x, y, z; };
XYZ    Point[N_max + 1];

  string type[] = {
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",
  " adj='xyz' ",
  " adj='xy'  ",
  " adj='z'   ",
  " adj='XYZ' ",
  " adj='XY'  ",
  " adj='Z'   ",

  " fix='xyz' ",
  " fix='xy' ",
  " fix='xy' adj='z' ",
  " fix='xy' adj='z' ",
  " fix='xy' adj='Z' ",
  " fix='xy' adj='Z' ",
  " fix='z' ",
  " fix='z' adj='xy' ",
  " fix='z' adj='xy' ",
  " fix='z' adj='XY' ",
  " fix='z' adj='XY' ",
};



using Function = void (*)(int);

void observations(int);
void heights(int);
void coordinates(int);
void vectors(int);

Function cluster[] = {observations, observations, observations,
                      heights,      coordinates,  vectors};
const int cluster_size = sizeof(cluster)/sizeof(Function);



inline double xyz()
{
  return rdis(rgen)*1000.0;
}

inline int rand_type()
{
  return int( rdis(rgen) * type->size());
}

inline int index(int M) 
{
  return int( rdis(rgen) * M );
}

int target(int from)
{
  int to;
  do {
    to = index(N) + 1;
  } while (from == to);
  return to;
}



void dh(int from)
{
  int to = target(from);
  cout.precision(3);
  cout << "<dh from='" << from << "' to='" << to << "'"
       << " val='" << Point[to].z -Point[from].z << "'"
       << " stdev='2.0' />\n";
}

void dist(int from)
{
  int to = target(from);
  cout.precision(2);
  double dx = Point[to].x - Point[from].x;
  double dy = Point[to].y - Point[from].y;
  double d  = sqrt(dx*dx + dy*dy);
  cout << "<distance from='" << from << "' to='" << to << "'"
       << " val='" << d << "' stdev='4.0' />\n";
}

void sdist(int from)
{
  double from_dh {0}, to_dh {0};
  double rf = rdis(rgen);
  if (rf < 0.5) from_dh = 0.5 +  rf*2;
  double rt = rdis(rgen);
  if (rt < 0.5) to_dh = 0.5 +  rt*2;

  int to = target(from);
  cout.precision(2);
  double dx = Point[to].x - Point[from].x;
  double dy = Point[to].y - Point[from].y;
  double dz = Point[to].z - Point[from].z + to_dh - from_dh;
  double d  = sqrt(dx*dx + dy*dy + dz*dz);
  cout << "<s-distance from='" << from << "' to='" << to << "'"
       << " val='" << d << "'";
  if (from_dh != 0 || to_dh != 0)
    cout << " from_dh='" << from_dh << "' to_dh='" << to_dh << "'";
  cout << " stdev='5.0' />\n";
}

void dir(int from)
{
  cout.precision(4);
  for (int i=0; i<=1+index(4); i++)
    {
      int to = target(from);
      double dx = Point[to].x - Point[from].x;
      double dy = Point[to].y - Point[from].y;
      double s  = atan2(dy, dx)/M_PI*200;
      if (s < 0) s += 400;
      cout << "<direction to='" << to << "'"
           << " val='" << s << "' stdev='6.0' />\n";
    }
}

void zen(int from)
{
  double from_dh {0}, to_dh {0};
  double rf = rdis(rgen);
  if (rf < 0.5) from_dh = 0.5 +  rf*2;
  double rt = rdis(rgen);
  if (rt < 0.5) to_dh = 0.5 +  rt*2;

  int   to;
  double dx, dy, dz, s;
  do
    {
      to = target(from);
      cout.precision(4);
      dx = Point[to].x - Point[from].x;
      dy = Point[to].y - Point[from].y;
      dz = Point[to].z - Point[from].z + to_dh - from_dh;
      s  = sqrt(dx*dx + dy*dy + dz*dz);
    }
  while(s == 0);
  double z  = acos(dz/s)/M_PI*200;
  cout << "<z-angle from='" << from << "' to='" << to << "'"
       << " val='" << z << "'";
  if (from_dh != 0 || to_dh != 0)
    cout << " from_dh='" << from_dh << "' to_dh='" << to_dh << "'";
  cout << " stdev='7.0' />\n";
}

Function obs[] = {  dir, dist, sdist, zen };
const int obs_size = sizeof(obs)/sizeof(Function);

void observations(int from)
{
  cout << "<obs from='" << from << "'>\n";
  for (int i=0; i<=index(5); i++) obs[index(obs_size)](from);
  cout << "</obs>\n\n";
}

void heights(int from)
{
  cout << "<height-differences>\n";
  for (int i=0; i<=index(5); i++) dh(from);
  cout << "</height-differences>\n\n";
}

void coordinates(int from)
{
  cout.precision(3);
  int p[] = {0, 0, 0};

  int test=0;
  do
    {
      p[0] = target(from);
      p[1] = target(from);
      p[2] = target(from);
      if (test++ > N_max) return;
    }
  while (p[0] == p[1] || p[0] == p[2] || p[1] == p[2]);

  int dim=0;

  cout << "<coordinates>\n";

  for (int i=0; i<=index(3); i++)
    {
      cout << "<point id='" << p[i] << "'";
      int k = index(3);
      switch (k)
        {
        case 0:
          dim += 3;
          cout << " x='" << Point[p[i]].x << "'";
          cout << " y='" << Point[p[i]].y << "'";
          cout << " z='" << Point[p[i]].z << "'";
          break;
        case 1:
          dim += 2;
          cout << " x='" << Point[p[i]].x << "'";
          cout << " y='" << Point[p[i]].y << "'";
          break;
        case 2:
          dim += 1;
          cout << " z='" << Point[p[i]].z << "'";
          break;
        };
      cout << " />\n";
    }

  cout << "<cov-mat dim='" << dim << "' band='" << dim-1 << "'>\n";
  for (int s=1, i=1; i<=dim; i++)
    {
      cout << "25";
      for (int k=1; k<=dim-1 && i+k<=dim; k++, s=-s) cout << " " << s;
      cout << endl;
    }
  cout << "</cov-mat>\n";

  cout << "</coordinates>\n\n";
}

void vectors(int from)
{
  cout.precision(3);
  int p[] = {0, 0, 0};

  int test=0;
  do
    {
      p[0] = target(from);
      p[1] = target(from);
      p[2] = target(from);
      if (test++ > N_max) return;
    }
  while (p[0] == p[1] || p[0] == p[2] || p[1] == p[2]);

  int dim=0;

  cout << "<vectors>\n";

  for (int i=0; i<=index(3); i++)
    {
      dim += 3;
      int to = target(p[i]);
      cout << "<vec from='" << p[i] << "' to='" << to << "'"
           << " dx='" << Point[to].x - Point[p[i]].x << "'"
           << " dy='" << Point[to].y - Point[p[i]].y << "'"
           << " dz='" << Point[to].z - Point[p[i]].z << "'"
           << " />\n";
    }

  cout << "<cov-mat dim='" << dim << "' band='" << dim-1 << "'>\n";
  for (int s=1, i=1; i<=dim; i++)
    {
      cout << "25";
      for (int k=1; k<=dim-1 && i+k<=dim; k++, s=-s) cout << " " << s;
      cout << endl;
    }
  cout << "</cov-mat>\n";

  cout << "</vectors>\n\n";
}

// #######################################################################

int main(int argc, char* argv[])
{
  cout.setf(ios::fixed, ios::floatfield);

  do
    N = index(N_max);
  while (N < 2);

  for (int i=1; i<=N; i++)
    {
      Point[i].x = xyz();
      Point[i].y = xyz();
      Point[i].z = xyz();
    }

  cout << "<?xml version='1.0' ?>\n"
       << "<!DOCTYPE gama-xml SYSTEM 'gama-xml.dtd'>\n"
       << "<gama-xml version='2.0'>\n"
       << "<network>\n\n"

       << "<description>\n"
       << "gen-test 2.00\n"
       << "</description>\n\n"

       << "<points-observations>\n\n";

  cout.precision(2);
  for (int i=1; i<=N; i++)
    cout << "<point id='" << i << "'"
         << " x='" << Point[i].x << "'"
         << " y='" << Point[i].y << "'"
         << " z='" << Point[i].z << "'"
         << type[rand_type()]
         << "/>\n";
  cout << endl;

  for (int i=1; i<=N; i++)
    for (int j=1; j<=1+index(3); j++)
      {
        cluster[index(cluster_size)](i);
      }

  cout << "</points-observations>\n"
       << "</network>\n"
       << "</gama-xml>\n\n";
}

