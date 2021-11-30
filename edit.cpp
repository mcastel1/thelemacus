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





int main(int argc, char *argv[]){


  ifstream infile;
  string line;
  double lon, lat;
  stringstream line_ins;


  infile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_sorted_by_latitude.csv");


  line.clear();
  line_ins.clear();
  getline(infile, line);

  while(!infile.eof()){

    line_ins << line;
    line_ins >> lon >> lat;

    cout << lon << "\t" << lat << "\n";
    
    line.clear();
    line_ins.clear();
    getline(infile, line); 

  }


  
  infile.close();

  cout << "\n";
  return(0);
	
}
