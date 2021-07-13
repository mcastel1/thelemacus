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
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE -g

  ./sight.o
  valgrind ./sight.o
  valgrind --leak-check=full ./sight.o


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

/*notes
- add rotation of sight
- split Time into Date and Chrono objects
- change date and time format do YYYY-DD-MM hh:mm:ss.s, which is nicer.
*/

using namespace std;



#include "lib.h"





int main(int argc, char *argv[]){

  cout.precision(my_precision);

  //unsigned int i;
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

  /*
  Time t;
  t.enter("sample", "");
  t.to_MJD();
  cout << "MJD = " << t.MJD << "\n";
  */
  
  /*
  Point a;
  a.enter("Position", "");
  a.print("Entered position", "");
  */
  
  Catalog catalog("data/catalog.txt");
  Plot plot(&catalog);

  catalog.print("\t", cout);
  

  plot.menu();


  // sight.enter(catalog); 
  // sight.reduce();

  

  cout << "\n";
  return(0);
	
}
