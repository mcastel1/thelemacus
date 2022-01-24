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
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE
  clear; clear; g++ sight.cpp -llapack  -lgsl -lcblas -lm -O3 -Wno-deprecated -I/usr/local/include/gsl/ -I ./ -o sight.o -Wall -DHAVE_INLINE -g

  ./sight.o
  valgrind ./sight.o
  valgrind --leak-check=full ./sight.o


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
- include sight reduction for meridian passage!
- make sure that running code cannot enter into infinite loop if weird values are entered from keyboard
- include keyboard escape sequence to terminate code at any time
- if phi_span < 1 degree, add one label on the y axis where you show both degrees and arcminutes
- all functions ..read_from_file shouuld return a bool to signal whether the reading was successful or not
- check whether H_o > 0, and if it is not, restart the procedure to enter the sight
- adjust the number of points of coastlines as a function of the plot scale: the larger the scale, the lesser points
- 360 deg E in xlabel -> 0 deg
- make sure that code does not crash if special characters are entered in position/sight labels
- remove horizontal lines in gnuplot plot
- modify structure of catalog.txt so angles and distances are read with Angle::read_from_file, etc...
- include computation of crossing points between circles of equal altitude
- export plots in .kml formats, so they can be plotted on the nautical charts on data.shom
- include reading of dummy line in Position::read
- add modify sight, route  in menu
- export data to gpx file and open Chrome authomatically to data.shom page to plot the gpx file
- set position of the gnuplot plot window when this is opened
 - when entering angles, replace +- with N/S and E/W
- find out why increasing too much number of points coastline in init file yields plots with few points
- set output to log file
- centralizza tutti i percorsi dei file in modo che se devi cambiarlo basta cambiarlo in un posto solo
- in all functions ::set and ::print, print out only if the associated name of the quantity that you are printing is different from ""
- the way you compute the error on the astronomical position is wrong: it does not go to zero as the number of sights goes to infinity
- make sure that when you remove the Position astronomical position, the circle of error Route is also removed
- when you delete a sight, make sure that all the crossing Positions involving it are also deleted
- enter body + limb directly in one line
- add != operators in all classes
- finisci di programmare Sight::modify (mancano le modifiche ad alcuni item)
- solve issue that sed crashes if replaced string is too long
- check check_double function for sign of double
- type of related_sight should be int -> Int
- when you enter positive angles, revise Angle::enter in such a way that you don't have to enter '+' from keyborard
- in print_to_kml include also loxodromes and orthodromes
- make file_init a variable of the Sight class
- the line width of gnuplot curves should be a variable in the init_file
*/



using namespace std;



#include "lib.h"





int main(int argc, char *argv[]){

  Int width_terminal_window, height_terminal_window;
  File file_init;
  stringstream command;

  cout.precision(display_precision);
  //turn off the GSL error handler, so the GSL routines will return an error message if they fail, and this error message can be handled by my code
  gsl_set_error_handler_off();

  /*
  double x;
  enter_double(&x, true, -3., 2., String("xxx"), String("---"));
  return 0;
  */
  
  /*
  Route r;
  Position q, p;
  r.enter(String("testing route"), String(""));
  q.enter(String("position"), String(""));
  r.closest_point_to(&p, q, String(""));
  */

  /*
  Chrono c;
  c.set(String("my chronometer"), 2.342546574, String("\t"));
  return 0;
  */
  
  /*
  Date date;
  date.enter(String("test date of new year"), String(""));
  date.print(String("entered date"), String(""), cout);
  return 0;
  */

  /*
  Answer a, b;

  a.set(String("answer a"), 'y', String(""));
  b.set(String("answer b"), 'n', String(""));
  bool C = (a!=b);
  
  cout << "a != b = " << C;
  return 0;
  */
  
  /*
  char c[] = "fgdfjhttjerioyje434678ghfd";
  cout << "\tint = " << strtol (c, NULL, 32) << "\n";
  return 0;
  */

  
  /*
  String s;
  s.enter(String("test string to convert"), String(""));
  cout << "Int = " << s.to_long_int(String("\t")) << "\n";
  return 0;
  */
  
  /*
  Answer a;
  a.set(String("what an answer!"), 'c', String("\t"));
  return 0;
  */

  /*
  Position a,b;
  Length l;
  a.enter(String("a"), String(""));
  b.enter(String("b"), String(""));
  a.distance(b, &l, String("\t"));
  */

  /*
  Angle x;
  x.enter(String("instrumental altitude"), String("\t"));
  x.print(String("instrumental altitude"), String("\t"), cout);
  return 0;
  */

  /*
  Route r1, r2;
  vector<Position> c(2);
  Catalog catalog_temp(String(path_file_catalog));

  Plot plot_temp(&catalog_temp);

  r1.enter(String("route 1"), String("\t"));
  r2.enter(String("route 2"), String("\t"));
  if(r1.crossing(r2, &c, String("\t"))){

    plot_temp.position_list.push_back(c[0]);
    plot_temp.position_list.push_back(c[1]);

  }
  
  plot_temp.show(String(""));
  return 0;
  */
  /*
  Int x;
  File f;
  f.set_name(String("data/init.txt"));
  f.open(String("in"), String("\t"));
  x.read_from_file(String("plot coastline every"), f, true, String("\t"));
  f.close(String("\t"));
  return 0;
  */

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

  /*
  Time t;
  t.enter("sample", "");
  t.to_MJD();
  cout << "MJD = " << t.MJD << "\n";
  */
  
  /*
  Position a;
  a.enter("Position", "");
  a.print("Entered position", "", cout);
  a.transport("");
  return 0;
  */

  /*
  Route route;
  route.enter("path to cape horn", "");
  route.compute_end("");
  route.print("path to cape horn", "", cout);
  return 0;
  */

  //read height and width of terminal window from file_init and writes them to width_terminal_window and height_terminal_window
  file_init.set_name(String(path_file_init));
  file_init.open(String("in"), String(""));
  width_terminal_window.read_from_file(String("width of terminal window"), file_init, true, String("\t")); 
  height_terminal_window.read_from_file(String("height of terminal window"), file_init, true, String("\t")); 
  file_init.close(String(""));
  //run the command that sets the width and height of terminal window, and moves it to the top left corner of the screen
  command.str("");
  command << "printf '\e[8;" << (height_terminal_window.value) << ";" << (width_terminal_window.value) << "t'\n printf '\e[3;0;0t'";
  system(command.str().c_str());

  
  Catalog catalog(String(path_file_catalog), String(""));
  Plot plot(&catalog, String(""));

  catalog.print(String(""), cout);
  

  plot.menu(String(""));


  // sight.enter(catalog); 
  // sight.reduce();

  //restore the terminal window to full screen
  system("printf '\e[9;1t'");


  cout << "\n";
  return(0);
	
}
