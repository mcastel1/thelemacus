#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <strstream>
#include <string.h>
#include <sstream>
//#include <iomanip>
#include <algorithm>
#include <list>

#define max_lat (83.6664731)
#define min_lat (-78.7290778)
#define floor_min_lat (floor(min_lat))
#define floor_max_lat (floor(max_lat))
#define span_lat ((floor_max_lat-floor_min_lat+1)
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


  ifstream infile, infile_n_line;
  ofstream outfile_n_line, outfile;
  string line;
  vector< vector<position> > p(360);
  stringstream ins, ins_outfile;
  int i, j;
  //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
  vector<unsigned int> n_line(360*(floor_max_lat-floor_min_lat+1));

 
  //
  //read n_line.txt and store it into vector n_line
  infile_n_line.open("n_line.txt");
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


  
  //this chunk of code reads in map_conv_blocked.csv the points with latitude floor(lat_min)+i, and longitude j
  ifstream is("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv");
  unsigned int l;
  char* buffer = NULL;
  string data;
  int i_min, i_max, j_min, j_max;

  

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
      is.seekg(n_line[360*i+j], is.beg);

      
      l = n_line[360*i+j + 1] - n_line[360*i+j] - 1; 
      if(buffer != NULL){delete [] buffer;}
      buffer = new char [l];

      is.read(buffer, l);
      string dummy(buffer, l);
      data.append(dummy);
      dummy.clear();

      if(is){
      
	cout << "\nall characters read successfully.";
      
      }else{
      
	cout << "\nerror: only " << is.gcount() << " could be read";
	is.close();

      }

    }

  }
  
  cout << "\nResult of reading n_line.txt: " << data;
 
  //make substitutions to plot with gnuplot
  replace(data.begin(), data.end(), ' ', '\n');
  replace(data.begin(), data.end(), ',', ' ');

  system("rm -rf result.txt");
  outfile.open("result.txt");
  outfile << data;
  outfile.close();
  is.close();

  

  //

  //this part of the code creates map_conv_blocked.csv 
  /*
    int floor_old = 0, floor_new = 0;
    position t;
    long int n;

  
    //infile.open("/Users/mcastellana/Documents/navigational_astronomy/sight_reduction_program/sample.csv");
    infile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_sorted_by_latitude.csv");

    system("rm /Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv");
    outfile.open("/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv");
    outfile.precision(outfile_precision);
    ins_outfile.precision(outfile_precision);

    system("rm n_line.txt");
    outfile_n_line.open("n_line.txt");

    floor_old = floor_min_lat;
    for(i=0; i<((int)p.size()); i++){(p[i]).clear();}
    n = 0;
    while(!infile.eof()){

    line.clear();
    ins.clear();

    getline(infile, line);
    ins << line;
    ins >> (t.lon) >> (t.lat);
    //convert longitude according to convention where W longitudes are positive and normalize it in such a way that 0 <= lon < 360.0
    (t.lon) = (-(t.lon)) - 360.0*floor((-(t.lon))/(360.0*M_PI));

    floor_new = floor((t.lat));

    if(floor_new != floor_old){

    if(floor_new > floor_old + 1){cout << "\nJump is > 1!"; return 0;}
      
    cout << "\nSwitching to latitude " << floor_new;

    for(i=0; i<((int)p.size()); i++){

    n_line[360*(floor_new-floor_min_lat)+i] = n;	
    outfile_n_line << n_line[360*(floor_new-floor_min_lat)+i] << "\n";

    ins_outfile.str("");
    if((p[i]).size() != 0){
	  
    for(j=0; j<(int)(p[i]).size(); j++){
    ins_outfile << ((p[i][j]).lat) << "," << ((p[i][j]).lon) << " ";
    }
    outfile << ins_outfile.str();
    n += (ins_outfile.str()).size();
    //add to n the character corresponding to the '\n' found at the end of the line
    n++;
    }else{
    //add to n the character corresponding to the '\n' found at the end of the line
    n++;
    }

    outfile <<  "\n";
    }

    for(i=0; i<((int)p.size()); i++){(p[i]).clear();}
    }
    p[(t.lon)].push_back(t);

    floor_old = floor_new;

    }



    for(i=0; i<((int)p.size()); i++){

    n_line[360*(floor_new-floor_min_lat)+i] = n;	
    outfile_n_line << n_line[360*(floor_new-floor_min_lat)+i] << "\n";

    ins_outfile.str("");
    if((p[i]).size() != 0){
	  
    for(j=0; j<(int)(p[i]).size(); j++){
    ins_outfile << ((p[i][j]).lat) << "," << ((p[i][j]).lon) << " ";
    }
    outfile << ins_outfile.str();
    n += (ins_outfile.str()).size();
    //add to n the character corresponding to the '\n' found at the end of the line
    n++;
    }else{
    //add to n the character corresponding to the '\n' found at the end of the line
    n++;
    }

    outfile <<  "\n";
    }
  
    infile.close();
    p.clear();
    n_line.clear();
    outfile.close();
    outfile_n_line.close();

  */

  cout << "\n";
  return(0);
	
}
