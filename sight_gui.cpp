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

#include "chartdir.h"


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
#include <stdio.h>


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
struct CheckMouseEnter;
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

/*
 notes: set parents to constructors of all frames that you defined
 - when you click on modify and then press reduce, the code should not insert a new item in listcontrol, but replace the old one
- add recent items to dropdown menus
 - add instrumental error
 - add condition that an error message is prompted only if the GUI field is enabled not only to CheckLength, but also to all other Check* classes
 - make sure that aspect ratio = 1 in the plotted charts
 */


