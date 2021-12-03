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

#define max_lat (83.6664731)
#define min_lat (-78.7290778)
#define outfile_precision 16


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
  unsigned int count;

  /*
    ifstream is("text.txt");
    unsigned int l;

    if(is){
    is.seekg (0, is.end);
    l = is.tellg();

    // allocate memory:
    char * buffer = new char [l];

    // read data as a block:
    is.seekg(3, is.beg);
    is.read(buffer, 2);

    cout.write(buffer, 2);

    is.close();
    }else{

    cout << "Could not open file!\n";
    }
    cout << "\n";
    return 0;
  */

  //infile.open("/Users/mcastellana/Documents/navigational_astronomy/sight_reduction_program/sample.csv");
  infile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_sorted_by_latitude.csv");

  system("rm /Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv");
  outfile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv");
  outfile.precision(outfile_precision);

  floor_old = floor(min_lat);
  for(i=0; i<((int)p.size()); i++){(p[i]).clear();}

  while(!infile.eof()){

    line.clear();
    ins.clear();

    getline(infile, line);
    ins << line;
    ins >> (t.lon) >> (t.lat);

    floor_new = floor((t.lat));

    if(floor_new != floor_old){

      if(floor_new > floor_old + 1){cout << "\nJump is > 1!"; return 0;}
      
      cout << "\nSwitching to latitude " << floor_new;

      for(i=0; i<((int)p.size()); i++){
	//-lon+180-e = i , lon = 180-i-e, floor(lon) = 180-i-1
	//outfile << "\np[" << -i+180-1 << "]:\t\t";
	//outfile << "\n" << i << "\t";
	
	if((p[i]).size() != 0){
	  for(j=0; j<(int)(p[i]).size(); j++){
	    //outfile << "{" << ((p[i][j]).lat) << "," << ((p[i][j]).lon) << "},";
	    outfile << ((p[i][j]).lat) << " " << ((p[i][j]).lon) << " ";
	  }
	}
	outfile << "\n";
      }

      for(i=0; i<((int)p.size()); i++){(p[i]).clear();}
    }
    p[floor(-(t.lon)+180.0)].push_back(t);
    //cout << "\nPushing back {" << (t.lat) << "," << (t.lon) << "} to p[" << floor(-(t.lon)+180.0) << "]";

    floor_old = floor_new;

  }


  for(i=0; i<((int)p.size()); i++){
    //-lon+180-e = i , lon = 180-i-e, floor(lon) = 180-i-1
    //outfile << "\np[" << -i+180-1 << "]:\t\t";
    //outfile << "\n" << i << "\t";
	
    if((p[i]).size() != 0){
      for(j=0; j<(int)(p[i]).size(); j++){
	//outfile << "{" << ((p[i][j]).lat) << "," << ((p[i][j]).lon) << "},";
	outfile << ((p[i][j]).lat) << " " << ((p[i][j]).lon) << " ";
      }
    }
    outfile << "\n";
  }


  
  infile.close();
  p.clear();
  outfile.close();

  cout << "\n";
  return(0);
	
}
