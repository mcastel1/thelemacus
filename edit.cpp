#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <strstream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <list>


//mac
/*
  clear; clear; g++ edit.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o edit.o -Wall -DHAVE_INLINE
  clear; clear; g++ edit.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o edit.o -Wall -DHAVE_INLINE -g

  ./edit.o
  valgrind ./edit.o
  valgrind --leak-check=full ./edit.o


*/

// #include <gsl_rng.h>
// #include <gsl_randist.h>
// #include <gsl_vector.h>
// #include <gsl_matrix.h>
// #include <gsl_eigen.h>
#include <gsl_blas.h>
#include <gsl_sf_pow_int.h>
#include <gsl_sf_exp.h>
#include <gsl_errno.h> 
#include <gsl_math.h>
#include <gsl_spline.h>
#include <gsl_integration.h>
#include <gsl_roots.h>
// #include <gsl_complex.h>
// #include <gsl_complex_math.h>




using namespace std;



#include "lib.h"

class position{

public:
  double lat, lon;

};



int main(int argc, char *argv[]){


  ifstream infile;
  ofstream outfile;
  string line;
  int floor_old, floor_new;
  vector< vector<position> > p(360);
  stringstream line_ins;
  position t;
  int i;


  infile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_sorted_by_latitude.csv");
  outfile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_block.csv");


  for(i=0; i<(int)(p.size()); i++){(p[i]).resize(0);}

  
  line.clear();
  line_ins.clear();
  getline(infile, line);

  floor_old = -1;

  while(!infile.eof()){

    line_ins << line;
    line_ins >> (t.lon) >> (t.lat);

    cout << t.lon << "\t" << t.lat << "\n";

    floor_new = floor(t.lat);

    (p[floor_new]).push_back(t);

    if(floor_new != floor_old){
      for(i=0; i<(int)(p.size()); i++){(p[i]).clear();}
      outfile << "\n\n";
    }
    
    line.clear();
    line_ins.clear();
    getline(infile, line);

    floor_old = floor_new;

  }


  
  infile.close();
  p.clear();
  outfile.close();

  cout << "\n";
  return(0);
	
}
