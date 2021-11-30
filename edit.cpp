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
// #include <gsl_blas.h>
// #include <gsl_sf_pow_int.h>
// #include <gsl_sf_exp.h>
// #include <gsl_errno.h> 
// #include <gsl_math.h>
// #include <gsl_spline.h>
// #include <gsl_integration.h>
// #include <gsl_roots.h>
// #include <gsl_complex.h>
// #include <gsl_complex_math.h>




using namespace std;

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
  stringstream ins;
  position t;
  int i, j;


  infile.open("/Users/mcastellana/Documents/navigational_astronomy/sight_reduction_program/sample.csv");
  outfile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_block.csv");

  floor_old = -90-1;
  
  while(!infile.eof()){

    line.clear();
    ins.clear();

    getline(infile, line);
    ins << line;
    ins >> (t.lon) >> (t.lat);

    floor_new = floor((t.lat));

    if(floor_new != floor_old){
      cout << "\nSwitching to a new latitude";

      for(i=0; i<((int)p.size()); i++){
	if((p[i]).size() != 0){
	  //-lon+180-e = i , lon = 180-i-e, floor(lon) = 180-i-1
	  cout << "\np[" << -i+180-1 << "]:\t\t";
	  for(j=0; j<(int)(p[i]).size(); j++){
	    cout << "{" << ((p[i][j]).lat) << "," << ((p[i][j]).lon) << "} ";
	  }

	}

      }

      
      for(i=0; i<((int)p.size()); i++){(p[i]).clear();}
    }
    p[floor(-(t.lon)+180.0)].push_back(t);
    cout << "\nPushing back {" << (t.lat) << "," << (t.lon) << "} to p[" << floor(-(t.lon)+180.0) << "]";

    floor_old = floor_new;

  }


  for(i=0; i<((int)p.size()); i++){
    if((p[i]).size() != 0){
      //-lon+180-e = i , lon = 180-i-e, floor(lon) = 180-i-1
      cout << "\np[" << -i+180-1 << "]:\t\t";
      for(j=0; j<(int)(p[i]).size(); j++){
	cout << "{" << ((p[i][j]).lat) << "," << ((p[i][j]).lon) << "} ";
      }

    }

  }

  
  infile.close();
  p.clear();
  outfile.close();

  cout << "\n";
  return(0);
	
}
