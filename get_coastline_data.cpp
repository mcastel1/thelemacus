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
#define span_lat (floor_max_lat-floor_min_lat+1)
#define outfile_precision 16
#define path_file_coastline_data_blocked "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
#define path_file_n_line "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked.txt"
#define path_file_selected_coastline_data "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt"

//mac
/*
  clear; clear; g++ get_coastline_data.cpp `wx-config --cxxflags --libs` -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -I/Applications/ChartDirector/include -L/Applications/ChartDirector/lib -lchartdir -rpath /Applications/ChartDirector/lib -o get_coastline_data.o -Wall -DHAVE_INLINE
  
  ./get_coastline_data.o 
  valgrind ./get_coastline_data.o   -p 45 -P 50 -l 1 -L 4 -N 1000
  valgrind --leak-check=full ./get_coastline_data.o    -p 45 -P 50 -l 1 -L 4 -N 1000


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

#include "wx/wxprec.h"
#include "wx/checkbox.h"
#include "wx/combobox.h"
#include "wx/dc.h"
#include "wx/display.h"
#include "wx/listctrl.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "chartdir.h"



using namespace std;

class BodyField;
class LimbField;
template<class T> class CheckField;
class AngleField;
class LengthField;
class DateField;
class ChronoField;
class StringField;
class MyApp;
class MessageFrame;
class SightFrame;
class ChartFrame;
class PlotFrame;

struct CheckBody;
struct CheckLimb;
template<class T> struct CheckCheck;
struct CheckChrono;
struct CheckAngle;
struct CheckSign;
struct CheckArcDegree;
struct CheckArcMinute;
struct CheckLength;
struct CheckDate;
struct CheckYear;
struct CheckMonth;
struct CheckDay;
struct CheckHour;
struct CheckMinute;
struct CheckSecond;
struct CheckString;
struct SetStringToCurrentTime;
struct TabulateDays;
struct PrintErrorMessage;
struct OnSelectInListBox;


class Catalog;
class Limb;
class Angle;
class Length;
class String;
class Plot;
class File;
class Time;
class Date;
class Chrono;
class Route;
class Sight;
class Atmosphere;
class Answer;
class Body;


#include "lib.h"
#include "lib_gui.h"


//this code efficiently reads coastline data stored in path_file_coastline_data_blocked from latitudes p to P and longitudes l to L, and writes this data into path_file_selected_coastline_data, writing n_points_coastline points max 
int main(int argc, char *argv[]){

  File file_n_line, file_coastline_data_blocked, outfile_selected_coastline_data;
  int options;
  string data, line;
  stringstream ins;
  int i, j, i_min = 0, i_max = 0, j_min = 0, j_max = 0, j_normalized = 0;
  //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
  vector<unsigned int> n_line(360*(floor_max_lat-floor_min_lat+1));
  unsigned int l, n_points_coastline = 0, n = 0, every = 0, n_points_grid = 0;
  char* buffer = NULL;
  size_t pos_beg, pos_end;
  bool check;

  while ((options = getopt(argc, argv, ":l:L:p:P:N:")) != -1) {
		
    switch (options) {

    case 'p':
      i_min = (atoi(optarg)) - floor_min_lat;
      break;

    case 'P':
      i_max = (atoi(optarg)) - floor_min_lat;
      break;	

    case 'l':
      j_min = atoi(optarg);
      break;	

    case 'L':
      j_max = atoi(optarg);
      break;	

    case 'N':
      n_points_coastline = (unsigned int)atoi(optarg);
      break;	
 
      
    }
    
  }

  
  cout << "Coordinates: " << i_min << " " << i_max << " " << j_min << " " << j_max << "\n";


  n_points_grid = (i_max - i_min + 1 ) * (j_max - j_min + 1);
  
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
  //open a new file selected coastline data and write into it the new data
  outfile_selected_coastline_data.remove(String(""));  
  outfile_selected_coastline_data.open(String("out"), String(""));

  check = true;
  for(i=i_min; i<=i_max; i++){
    
    for(j=j_min; j<=j_max; j++){

      j_normalized = (j % 360);
      
      // read data as a block:
      file_coastline_data_blocked.value.seekg(n_line[360*i+j_normalized], file_coastline_data_blocked.value.beg);

      l = n_line[360*i+j_normalized + 1] - n_line[360*i+j_normalized] - 1; 
      if(buffer != NULL){delete [] buffer;}
      buffer = new char [l];

      file_coastline_data_blocked.value.read(buffer, l);
      string data(buffer, l);

      if(!(file_coastline_data_blocked.value)){

	check = false;
      
      }

      //count how many datapoints are in data
      n = count(data.begin(), data.end(), ',');

      every = (unsigned int)(((double)n)/((double)n_points_coastline)*((double)n_points_grid));
      if(every == 0){every = 1;}

      l=0;
      pos_beg = 0;
      pos_end = data.find(" ", pos_beg);
      while(pos_end != (string::npos)){

	//I write points in data to outfile_selected_coastline_data in such a way to write n_points_coastline points to the most
	if((l % every) == 0){
    
	  line.clear();
	  line = data.substr(pos_beg, pos_end - pos_beg + 1).c_str();

	  replace(line.begin(), line.end(), ' ', '\n');
	  replace(line.begin(), line.end(), ',', ' ');

	  (outfile_selected_coastline_data.value) << line;

	}

	pos_beg = pos_end+1;
	pos_end = data.find(" ", pos_beg);

	l++;
    
      };


      data.clear();

    }

  }
  
  if(check){

    cout << "All characters read successfully\n";
      
  }else{
      
    cout << RED << "Error: only " << file_coastline_data_blocked.value.gcount() << " characters could be read\n" << RESET;
    return 0;
	
  }

  
  outfile_selected_coastline_data.close(String(""));  
  file_coastline_data_blocked.close(String(""));

  return(0);
	
}
