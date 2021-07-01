#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <list>


//mac
/*
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE -g

  ./sight.o
  valgrind ./sight.o
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

  Catalog catalog;
  Sight sight;
  stringstream command, plot;

  catalog.add("sun","sun",695700.0/nm);
  catalog.add("moon","moon",1737.4/nm);
  catalog.add("planet","jupiter",0.0/nm);
  catalog.add("star","vega",0.0/nm);

 
  sight.body.enter(catalog);



  sight.H_s.enter("sextant altitude");
  sight.index_error.enter("index error");
  sight.height_of_eye.enter("height of eye");

  sight.compute_H_a();

  sight.t.enter("UTC time of sight");
  sight.limb.enter("limb");
  sight.get_coordinates();

  sight.compute_H_o();

  command << "sed 's/dummy_line/"
	  << "replot [0.:2.*pi] xe(K*Lambda(t, " << sight.d.value << ", " << sight.GHA.value << ", " << M_PI/2.0 - sight.H_o.value << ")), ye(K*Phi(t, " << sight.d.value << ", " << sight.GHA.value << ", " << M_PI/2.0 - sight.H_o.value << "))"
	  << "/g' plot_dummy.plt";
  
  system(command.str().c_str());

  
  
  cout << "\n";
  return(0);
	
}
