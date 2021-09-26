#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


using namespace std;

const  int N_max = 100; 
int        N;

struct XYZ { float x, y, z; };
XYZ    Point[N_max + 1];

char*  type[] = {
  "                     ",
  "           adj='xyz' ",
  "           adj='xy'  ",
  "           adj='z'   ",
  "           adj='XYZ' ",
  "           adj='XY'  ",
  "           adj='Z'   ",
  "           adj='xyz' ",
  "           adj='xy'  ",
  "           adj='z'   ",
  "           adj='XYZ' ",
  "           adj='XY'  ",
  "           adj='Z'   ",
  "           adj='xyz' ",
  "           adj='xy'  ",
  "           adj='z'   ",
  "           adj='XYZ' ",
  "           adj='XY'  ",
  "           adj='Z'   ",
  "           adj='xyz' ",
  "           adj='xy'  ",
  "           adj='z'   ",
  "           adj='XYZ' ",
  "           adj='XY'  ",
  "           adj='Z'   ",
  " fix='xyz'           ",
  " fix='xyz' adj='xyz' ",
  " fix='xyz' adj='xy'  ",
  " fix='xyz' adj='z'   ",
  " fix='xyz' adj='XYZ' ",
  " fix='xyz' adj='XY'  ",
  " fix='xyz' adj='Z'   ",
  " fix='xy'            ",
  " fix='xy'  adj='xyz' ",
  " fix='xy'  adj='xy'  ",
  " fix='xy'  adj='z'   ",
  " fix='xy'  adj='XYZ' ",
  " fix='xy'  adj='XY'  ",
  " fix='xy'  adj='Z'   ",
  " fix='z'             ",
  " fix='z'   adj='xyz' ",
  " fix='z'   adj='xy'  ",
  " fix='z'   adj='z'   ",
  " fix='z'   adj='XYZ' ",
  " fix='z'   adj='XY'  ",
  " fix='z'   adj='Z'   "
};



typedef void (*PF)(int);

void observations(int);
void heights(int);
void coordinates(int);
void vectors(int);

PF cluster[] = {observations, observations, observations,
                heights,      coordinates,  vectors};
const int cluster_size = sizeof(cluster)/sizeof(PF);



inline float xyz() 
{ 
  return float(rand())/RAND_MAX*1000.0; 
} 

inline int rand_type() 
{ 
  return int( float(rand())/RAND_MAX * (sizeof(type)/sizeof(char*)) ); 
}

inline int index(int M) 
{
  return int( float(rand())/RAND_MAX * M );
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
  float dx = Point[to].x - Point[from].x;
  float dy = Point[to].y - Point[from].y;
  float d  = sqrt(dx*dx + dy*dy);
  cout << "<distance from='" << from << "' to='" << to << "'"
       << " val='" << d << "' stdev='4.0' />\n";
}

void sdist(int from)
{
  int to = target(from);
  cout.precision(2);
  float dx = Point[to].x - Point[from].x;
  float dy = Point[to].y - Point[from].y;
  float dz = Point[to].z - Point[from].z;
  float d  = sqrt(dx*dx + dy*dy + dz*dz);
  cout << "<s-distance from='" << from << "' to='" << to << "'"
       << " val='" << d << "' stdev='5.0' />\n";
}

void dir(int from)
{
  cout.precision(4);
  for (int i=0; i<=1+index(4); i++)
    {
      int to = target(from);
      float dx = Point[to].x - Point[from].x;
      float dy = Point[to].y - Point[from].y;
      float s  = atan2(dy, dx)/M_PI*200;
      if (s < 0) s += 400;
      cout << "<direction to='" << to << "'"
           << " val='" << s << "' stdev='6.0' />\n";
    }
}

void zen(int from)
{
  int   to;
  float dx, dy, dz, s;
  do
    {
      to = target(from);
      cout.precision(4);
      dx = Point[to].x - Point[from].x;
      dy = Point[to].y - Point[from].y;
      dz = Point[to].z - Point[from].z;
      s  = sqrt(dx*dx + dy*dy + dz*dz);
    }
  while(s == 0);
  float z  = acos(dz/s)/M_PI*200;
  cout << "<z-angle from='" << from << "' to='" << to << "'"
       << " val='" << z << "' stdev='7.0' />\n";
}

PF obs[] = {  dir, dist, sdist, zen, dh };
const int obs_size = sizeof(obs)/sizeof(PF);

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
  int random_seed = time(0);
  if (argv[1]) random_seed = atoi(argv[1]);
  srand(random_seed);

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
       << "gen-test 1.0 :   random seed " << random_seed << endl
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

