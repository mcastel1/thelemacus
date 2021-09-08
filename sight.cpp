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
- include sight reduction for meridian passage!
- make sure that running code cannot enter into infinite loop if weird values are entered from keyboard
- include keyboard escape sequence to terminate code at any time
- if phi_span < 1 degree, add one label on the y axis where you show both degrees and arcminutes
- replace string with String everywhere
- all functions ..read_from_file shouuld return a bool to signal whether the reading was successful or not
- check whether H_o > 0, and if it is not, restart the procedure to enter the sight
- adjust the number of points of coastlines as a function of the plot scale: the larger the scale, the lesser points
*/

using namespace std;



#include "lib.h"





int main(int argc, char *argv[]){

  cout.precision(display_precision);
  //turn off the GSL error handler, so the GSL routines will return an error message if they fail, and this error message can be handled by my code
  gsl_set_error_handler_off();


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

  Route route;
  route.enter("path to cape horn", "");
  route.end("");
  return 0;
  
  Catalog catalog("data/catalog.txt");
  Plot plot(&catalog);

  catalog.print("\t", cout);
  

  plot.menu();


  // sight.enter(catalog); 
  // sight.reduce();

  

  cout << "\n";
  return(0);
	
}
