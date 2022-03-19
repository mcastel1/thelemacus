/*
 
 g++ sight_gui.cpp -o sight_gui.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib
 */

#include "wx/wxprec.h"
#include "wx/checkbox.h"
#include "wx/combobox.h"
#include "wx/dc.h"
#include "wx/display.h"
#include "wx/listctrl.h"
#include "wx/slider.h"


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
template<class P> class AngleField;
class LengthField;
class DateField;
class ChronoField;
template<class P> class StringField;
class MyApp;
class MessageFrame;
template<class F_YES, class F_NO> class QuestionFrame;
class ListFrame;
class SightFrame;
class ChartFrame;
class PlotFrame;
class ChartPanel;

struct CheckBody;
struct CheckLimb;
template<class T> struct CheckCheck;
struct CheckChrono;
template<class P> class CheckAngle;
template<class P> struct CheckSign;
template<class P> struct CheckArcDegree;
template<class P> struct CheckArcMinute;
struct CheckLength;
struct CheckDate;
struct CheckYear;
struct CheckMonth;
struct CheckDay;
struct CheckHour;
struct CheckMinute;
struct CheckSecond;
template<class P> struct CheckString;
template<class P> struct SetStringToCurrentTime;
struct TabulateDays;
template<class T> class PrintErrorMessage;
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
 - add error message if phi_min, phi_max etc... exceed the min and max latitudes in the data file.
 - all quantities in pixels must be relative to screen size, not absolute pixel size.
 - re-arrange class definitions properly
 - add lambert projection and 3D sphere with no projection
 - make sure you can recover a correct chart after the print error message has been prompted because the zoom factor is too large
 - make sure that LengthField allows to enter lengths in both meters and nm
 - change the initialization of structs (such as     (check.p) = this;) by transforming the structs into classes, creating their constructor, transforming check into a pointer and replacing     (check.p) = this; with check = new Check(this); do this for all objects 
 */


