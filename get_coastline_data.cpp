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
#include <unistd.h>

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
  valgrind ./get_coastline_data.o   -p 45 -P 50 -l 1 -L 4
  valgrind --leak-check=full ./get_coastline_data.o    -p 45 -P 50 -l 1 -L 4


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

  File file_n_line, file_coastline_data_blocked, outfile_selected_coastline_data;
  int options;
  string data, line;
  stringstream ins;
  int i, j, i_min = 0, i_max = 0, j_min = 0, j_max = 0;
  //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
  vector<unsigned int> n_line(360*(floor_max_lat-floor_min_lat+1));
  unsigned int l;
  char* buffer = NULL;
  size_t pos_beg, pos_end;


  while ((options = getopt(argc, argv, ":l:L:p:P:")) != -1) {
		
    switch (options) {

    case 'p':
      i_min = ((unsigned long long int)atoi(optarg)) - floor_min_lat;
      break;

    case 'P':
      i_max = ((unsigned long long int)atoi(optarg)) - floor_min_lat;
      break;	

    case 'l':
      j_min = (unsigned long long int)atoi(optarg);
      break;	

    case 'L':
      j_max = (unsigned long long int)atoi(optarg);
      break;	
      
    }
    
  }

  cout << "Coordinates: " << i_min << " " << i_max << " " << j_min << " " << j_max << "\n";


  file_n_line.set_name(String(path_file_n_line));
  file_coastline_data_blocked.set_name(String(path_file_coastline_data_blocked));
  outfile_selected_coastline_data.set_name(String(path_file_selected_coastline_data));

 
  //read file n_line and store it into vector n_line
  file_n_line.open(String("in"), String(""));
  i=0;
  while(!(file_n_line.value.eof())){

    line.clear();
    ins.clear();

    getline(file_n_line.value, line);
    ins << line;
    ins >> (n_line[i++]);

    //cout << "\nn_line[" << i-1 << "] = " << n_line[i-1];

  }
  file_n_line.close(String(""));


  
  //read in map_conv_blocked.csv the points with i_min <= latitude <= i_max, and j_min <= longitude <= j_max
  file_coastline_data_blocked.open(String("in"), String(""));

  data.clear();
  for(i=i_min; i<=i_max; i++){
    
    for(j=j_min; j<=j_max; j++){
      
      // read data as a block:
      file_coastline_data_blocked.value.seekg(n_line[360*i+j], file_coastline_data_blocked.value.beg);

      l = n_line[360*i+j + 1] - n_line[360*i+j] - 1; 
      if(buffer != NULL){delete [] buffer;}
      buffer = new char [l];

      file_coastline_data_blocked.value.read(buffer, l);
      string dummy(buffer, l);
      data.append(dummy);
      dummy.clear();

      if(file_coastline_data_blocked.value){
      
	cout << "\nAll characters read successfully.";
      
      }else{
      
	cout << "\nError: only " << file_coastline_data_blocked.value.gcount() << " characters could be read";
	file_coastline_data_blocked.close(String(""));

      }

    }

  }
  
  //cout << "\nResult of reading n_line.txt: " << data;
 
 
  //open a new file selected coastline data and write into it the new data
  outfile_selected_coastline_data.remove(String(""));  
  outfile_selected_coastline_data.open(String("out"), String(""));

  i=0;
  pos_beg = 0;
  pos_end = data.find(" ", pos_beg);
  while(pos_end != (string::npos)){
    
    line.clear();
    line = data.substr(pos_beg, pos_end - pos_beg + 1).c_str();

    replace(line.begin(), line.end(), ' ', '\n');
    replace(line.begin(), line.end(), ',', ' ');

    (outfile_selected_coastline_data.value) << line;

    pos_beg = pos_end+1;
    pos_end = data.find(" ", pos_beg);
    
  };
  
  outfile_selected_coastline_data.close(String(""));
  
  file_coastline_data_blocked.close(String(""));

  cout << "\n";
  return(0);
	
}
