/*
 
 g++ sight_gui.cpp -o sight_gui.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -Wall -Wno-c++11-extensions
 
 */

#include "wx/wxprec.h"
#include "wx/checkbox.h"
#include "wx/combobox.h"
#include "wx/dc.h"
#include "wx/display.h"
#include "wx/listctrl.h"





#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

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


#include "gsl_rng.h"
// #include "gsl_randist.h"
// #include "gsl_vector.h"
// #include "gsl_matrix.h"
// #include "gsl_eigen.h"
#include "gsl_blas.h"
#include "gsl_sf_pow_int.h"
#include "gsl_sf_exp.h"
#include "gsl_errno.h"
#include "gsl_math.h"
#include "gsl_spline.h"
#include "gsl_integration.h"
#include "gsl_roots.h"
// #include "gsl_complex.h"
// #include "gsl_complex_math.h"



using namespace std;


#include "lib.h"
#include "lib_gui.h"

/*
 notes: set parents to constructors of all frames that you defined
- when you print out index error, print it with its sign rather than normalized
 - when you click on modify and then press reduce, the code should not insert a new item in listcontrol, but replace the old one
- if you modify a field in SightFrame, do not kill the focus on the field and press Reduce, the content of the field is not written in the non-GUI object, while it should be -> fix it. 
- add recent items to dropdown menus
 */


