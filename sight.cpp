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
  valgrind --leak-check==full ./sight.o


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
- how do the jpl data files deal with the potential insertion of leap seconds in the future?
- this code does not work if there are leapseconds in the time lapse covered by the ephemeride files
- in Time::enter(), you should include check that D lies in the right interval .This requires a knowledge of bisextile years
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

  Time A;
  Chrono a;
  A.enter("Time A");
  a.enter("Chrono a");
  A.add(a);
  A.print("Time A+a");
  return 0;

  
  Answer answer;
  Catalog catalog("data/catalog.txt");
  Plot plot;

  catalog.print();
  
 
  do{
    plot.add(catalog);
    cout << "Do you want to add another sight? [y/n]";
    cin >> answer.value;
    answer.print("answer");
  }while((answer.value)=='y');


  // sight.enter(catalog); 
  // sight.reduce();

  plot.print();
  plot.show();
  
  //plot.~Plot();
  
  cout << "\n";
  return(0);
	
}
