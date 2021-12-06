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
#define floor_min_lat (floor(min_lat))
#define floor_max_lat (floor(max_lat))
#define span_lat ((floor_max_lat-floor_min_lat+1)
#define outfile_precision 16
#define path_file_coastline_data_blocked "/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
#define path_file_n_line "/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked.txt"
#define path_file_selected_coastline_data "/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt"

//mac
/*
  clear; clear; g++ get_coastline_data.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o get_coastline_data.o -Wall -DHAVE_INLINE
  clear; clear; g++ get_coastline_data.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o get_coastline_data.o -Wall -DHAVE_INLINE -g

  ./get_coastline_data.o
  valgrind ./get_coastline_data.o
  valgrind --leak-check=full ./get_coastline_data.o


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

#include <lib.h>


int main(int argc, char *argv[]){

  ifstream infile_n_line, infile_coastline_data_blocked;
  ofstream outfile_selected_coastline_data;
  string data, line;
  stringstream ins;
  int i, j, i_min, i_max, j_min, j_max;
  //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
  vector<unsigned int> n_line(360*(floor_max_lat-floor_min_lat+1));
  unsigned int l;
  char* buffer = NULL;

 
  //read file n_line and store it into vector n_line
  infile_n_line.open(path_file_n_line);
  i=0;
  while(!infile_n_line.eof()){

    line.clear();
    ins.clear();

    getline(infile_n_line, line);
    ins << line;
    ins >> (n_line[i++]);

    //cout << "\nn_line[" << i-1 << "] = " << n_line[i-1];

  }
  infile_n_line.close();


  
  //read in map_conv_blocked.csv the points with i_min <= latitude <= i_max, and j_min <= longitude <= j_max
  infile_coastline_data_blocked.open(path_file_coastline_data_blocked);
  cout << "\nEnter minimal latitude:";
  cin >> i_min;
  i_min -= floor_min_lat;

  cout << "\nEnter maximal latitude:";
  cin >> i_max;
  i_max -= floor_min_lat;
  
  cout << "\nEnter minimal longitude:";
  cin >> j_min;

  cout << "\nEnter maximal longitude:";
  cin >> j_max;

  data.clear();
  for(i=i_min; i<=i_max; i++){
    
    for(j=j_min; j<=j_max; j++){
      
      // read data as a block:
      infile_coastline_data_blocked.seekg(n_line[360*i+j], infile_coastline_data_blocked.beg);

      l = n_line[360*i+j + 1] - n_line[360*i+j] - 1; 
      if(buffer != NULL){delete [] buffer;}
      buffer = new char [l];

      infile_coastline_data_blocked.read(buffer, l);
      string dummy(buffer, l);
      data.append(dummy);
      dummy.clear();

      if(infile_coastline_data_blocked){
      
	cout << "\nAll characters read successfully.";
      
      }else{
      
	cout << "\nError: only " << infile_coastline_data_blocked.gcount() << " characters could be read";
	infile_coastline_data_blocked.close();

      }

    }

  }
  
  cout << "\nResult of reading n_line.txt: " << data;
 
  //replace ' ' with newline and ',' with a space, so the file will be in a format which is readable from gnuplot
  replace(data.begin(), data.end(), ' ', '\n');
  replace(data.begin(), data.end(), ',', ' ');

  ins.str("");
  ins << "rm -rf " << path_file_selected_coastline_data;

  //remove file selected coastline data
  system(ins.str().c_str());
  //open a new file selected coastline data and write into it the new data
  outfile_selected_coastline_data.open(path_file_selected_coastline_data);
  outfile_selected_coastline_data << data;
  outfile_selected_coastline_data.close();
  
  infile_coastline_data_blocked.close();

  cout << "\n";
  return(0);
	
}
