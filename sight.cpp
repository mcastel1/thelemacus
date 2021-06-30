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
// #include <gsl_roots.h>
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

  catalog.add("sun","sun",695700.0/nm);
  catalog.add("moon","moon",1737.4/nm);
  catalog.add("planet","jupiter",0.0/nm);
  catalog.add("star","vega",0.0/nm);

  Body body;
  body.enter(catalog);






  /*
    ofstream outfile;
    outfile.open("data_test.dat");
    outfile.precision(20);
    for(double z=0.0; z<sight.atmosphere.h[sight.atmosphere.n_layers]-.1; z+=.01){
    outfile << "\n" << z << " " << (sight.atmosphere.T(z+0.01)-sight.atmosphere.T(z))/0.01 << "\t" << sight.atmosphere.dTdz(z)
    << " " << (sight.atmosphere.n(z+0.01)-sight.atmosphere.n(z))/0.01 << "\t" << sight.atmosphere.dndz(z);
    }
    outfile.close();
  */
  // Angle a, b;
  // a.set("First angle", 5.6 );
  // b.set("Second angle", 4.2);
  // (a/.1).print("Sum angle");


  sight.height_of_eye.enter("height of eye");
  sight.correct_for_dip();
  
  sight.H_a.enter("apparent altitude");
  sight.correct_for_refraction();

  // sight.t.enter("UTC time of sight");
  // sight.index_error.enter("index error");
  // sight.limb.enter("limb");

  
  // sight.get_coordinates();

  
  
  
  //sight.body.enter();
  
  cout << "\n";
  return(0);
	
}
