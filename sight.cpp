#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>


//mac
/*
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE
  ./lib.o
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
// #include <gsl_roots.h>
// #include <gsl_complex.h>
// #include <gsl_complex_math.h>


using namespace std;
#include "lib.h"





int main(int argc, char *argv[]){


  /*
    int options;

    while ((options = getopt(argc, argv, ":L:B:")) != -1) {
		
    switch (options) {

    case 'L':
    L = (unsigned long long int) atoi(optarg) ;
    break;


    }
		
    }
  */

  Sight sight;

  sight.t.set();
  sight.t.print();

  cout << "Set the index error:\n";
  sight.index_error.set();
  
  cout << "\n";
  return(0);
	
}
