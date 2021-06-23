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
g++ lib.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o lib.o -Wall -DHAVE_INLINE
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


class Time{

public:  int Y, M, D, h, m;
  double s;
  bool check_M(void), check_D(void), check_h(void), check_m(void), check_s(void), bool print(void);
  
};


class Sight{

public: Time t;
  void set_time(void);
};

bool Time::check_M(void){

  if((M >= 1) && (M <= 12)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.";
    flush(cout);
    return false;
  }

};

bool Time::check_D(void){

  if((D >= 1) && (D <= 31)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.";
    flush(cout);
    return false;
  }

};

bool Time::check_h(void){

  if((h >= 0) && (h < 24)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.";
    flush(cout);
    return false;
  }

};

bool Time::check_m(void){

  if((m >= 0) && (m < 60)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.";
    flush(cout);
    return false;
  }

};

bool Time::check_s(void){

  if((s >= 0.0) && (s < 60.0)){return true;}
  else{
    cout << "\nEntered value is not valid! Try again.";
    flush(cout);
    return false;
  }

};

void Time::print(void){

  cout << "\nTime is" << Y << " " << M << " " << D << " " << h << "-" << m << "-" << s << "UTC";
  flush(cout);

};




void Sight::set_time (void) {

  cout << "\nEnter UTC time YYYY MM DD hh-mm-s:";

  cout << "\nEnter YYYY: ";
  cin >> t.Y;

  do{
    cout << "\nEnter MM: ";
    cin >> t.M;
  }while(!t.check_M());

  do{
    cout << "\nEnter DD: ";
    cin >> t.D;
  }while(!(t.check_D()));

  do{
    cout << "\nEnter hh: ";
    cin >> t.h;
  }while(!(t.check_h()));

  do{
    cout << "\nEnter mm: ";
    cin >> t.m;
  }while(!(t.check_m()));

  do{
    cout << "\nEnter ss: ";
    cin >> t.s;
  }while(!(t.check_s()));

}





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

  sight.set_time();
  
  cout << "\n";
  return(0);
	
}
