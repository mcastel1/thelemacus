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
//#endif

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

#include <boost/algorithm/string.hpp>



using namespace std;

#include "lib.h"





class MyApp: public wxApp{
public:

    ListFrame *list_frame;
    
    //this rectangle contains the size of the display on which the *this is run
    wxRect rectangle_display;
    //the size of small and large buttons of the app
    wxSize size_small_button, size_large_button;
    //The default fore/background color for day/night vision of the app of all objects in *this. background_color also represents the default, plain background color of fields, such as BodyFields, and it is thus conceptally equivalent to 'white' color
    Color foreground_color, background_color, error_color, /*the color with which an item is highlighted in ListFrame, when mouse hovers over the related route or position in the plot area*/ color_selected_item,  /*the color of the horizon circle for the 3D projection*/ color_horizon;
    vector<Color> color_list;
    //the default font used by the operating system, and a bold font obtained by the defalult font by setting its weight to wxFONTWEIGHT_BOLD
    wxFont default_font, error_font;
    Int time_zone, zoom_factor_max, n_points_minor_ticks;
    Double  /*the displacemenet over the x (y) axis divided by the length of the x (y) axis used by the functions MoveUp, ..., MoveRight*/ relative_displacement, /*the default thickness of the line with which routes are drawn*/ standard_thickness_over_length_screen, /*the thickness with which highlighted routes are drawn*/ large_thickness_over_length_screen, /*the numerical coefficients which define the function which relates the slider value to the zoom factor*/a_zoom, b_zoom, e_zoom,  /*this is the ratio between the length of the ticks on both axes, and the width of the plot area (for the mercator projection)/angle of aperture of the observer circle (for the 3d projection)*/tick_length_over_width_plot_area, tick_length_over_aperture_circle_observer, /*the ration between the size of small/large buttons and the width of the screen*/ size_small_button_over_width_screen, size_large_button_over_width_screen;
;

    wxTimer* timer;
    wxSystemSettings* settings;
    //the code checks every time_check whether the operating system has changed its light/dark mode
    Chrono time_check;
    //this is true/false if the system is in dark/light mode, respectively
    bool dark_mode;
    //the rectangle used to measure the size of the display
    wxDisplay display;
    //this referrs to the init file
    File file_init;


    virtual bool OnInit();
    void OnTimer(wxTimerEvent&);
    void ShowChart(wxCommandEvent&);
    void ShowList(wxCommandEvent&);

};

wxDECLARE_APP(MyApp);
wxIMPLEMENT_APP(MyApp);
