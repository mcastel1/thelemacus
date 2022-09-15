//#include "wx/wxprec.h"
////#include "wx/checkbox.h"
//#include "wx/combobox.h"
//#include "wx/dc.h"
//#include "wx/display.h"
//#include "wx/listctrl.h"
//#include "wx/slider.h"
//
//
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/app.h"
#include "wx/datetime.h"
//#endif

//#include "chartdir.h"


//#include <iostream>
//#include <cstdio>
//#include <cmath>
//#include <vector>
//#include <fstream>
//#include <strstream>
//#include <string.h>
//#include <sstream>
//#include <algorithm>
//#include <list>
//#include <stdio.h>


//#include "gsl_rng.h"
// #include "gsl_randist.h"
// #include "gsl_vector.h"
// #include "gsl_matrix.h"
// #include "gsl_eigen.h"
//#include "gsl_blas.h"
//#include "gsl_sf_pow_int.h"
//#include "gsl_sf_exp.h"
//#include "gsl_errno.h"
//#include "gsl_math.h"
//#include "gsl_spline.h"
//#include "gsl_integration.h"
//#include "gsl_roots.h"
// #include "gsl_complex.h"
// #include "gsl_complex_math.h"

class BodyField;
class LimbField;
template<class T> class CheckField;
template<class P> class AngleField;
template<class P> class LengthField;
class DateField;
class ChronoField;
class RouteTypeField;
template<class P> class StringField;
class MyApp;
template<class F> class CloseFrame;
template<class F_A, class F_B> class QuestionFrame;
class ListFrame;
class SightFrame;
class ChartFrame;
class PlotFrame;
class ChartPanel;
class RouteFrame;
class DrawPanel;
class Position;

class Catalog;
class Limb;
class Length;
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
class String;


//a class for color objects
class Color: public wxColour{
    
public:
    Color();
    Color(unsigned char, unsigned char, unsigned char);
    Color(unsigned char, unsigned char, unsigned char, unsigned char);
    
    void read_from_file(String, String, String);

};

class MyApp: public wxApp{
public:

    //this rectangle contains the size of the display on which the *this is run
    wxRect rectangle_display;
    //the background color for day/night vision of the app of all frames in *this
    Color day_background_color, night_background_color;

    virtual bool OnInit();

};

wxDECLARE_APP(MyApp);
wxIMPLEMENT_APP(MyApp);
