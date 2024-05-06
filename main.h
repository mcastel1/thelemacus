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
#include "wx/artprov.h"
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
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#ifdef _WIN32
#include <Windows.h>
#include <regex>
#endif


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
#include "gsl_sf_psi.h"
// #include "gsl_complex.h"
// #include "gsl_complex_math.h"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/dll.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>
#include "boost/date_time/local_time/local_time.hpp"


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
    Color foreground_color, background_color, error_color, highlight_color, /*the color with which an item is highlighted in ListFrame, when mouse hovers over the related route or position in the plot area*/ color_selected_item,  /*the color of the horizon circle for the 3D projection*/ color_horizon;
    vector<Color> color_list;
    //the default font used by the operating system, and a bold font obtained by the defalult font by setting its weight to wxFONTWEIGHT_BOLD
    wxFont default_font, error_font, highlight_font;
    Int time_zone, n_points_minor_ticks, /*the size of borders in pixels*/border, /*the size in pixels of the points used, for example, to draw coastlines */point_size, /*the number of snapshots used in animations*/n_animation_steps, n_points_plot_coastline_Mercator, n_points_plot_coastline_3D, n_points_routes, /*the preferred numer of ticks per axis in the chart plot: if there is enough space, I will put this number of ticks. If not, I will plot a nuber of ticks as cloase as possible to the preferred one*/n_intervals_ticks_preferred;
    Double  /*the displacemenet over the x (y) axis divided by the length of the x (y) axis used by the functions MoveUp, ..., MoveRight*/ relative_displacement, /*the default thickness of the line with which routes are drawn*/ standard_thickness_over_length_screen, /*the thickness with which highlighted routes are drawn*/ large_thickness_over_length_screen, /*the numerical coefficients which define the function which relates the slider value to the zoom factor*/a_zoom, b_zoom, e_zoom,  /*this is the ratio between the length of the ticks on both axes, and the width of the plot area (for the mercator projection)/angle of aperture of the observer circle (for the 3d projection)*/tick_length_over_width_plot_area, tick_length_over_aperture_circle_observer, /*the ration between the size of small/large buttons, icons and images in message frmes, and the width of the screen*/ size_small_button_over_width_screen, size_large_button_over_width_screen, size_icon_over_width_screen, size_message_image_over_width_screen, standard_thickness, large_thickness, /*when ChartTansportHandler zooms onto an object (e.g. a Route) I set the zoom factor at the end of the animation to be equal to be multiplied by chart_transport_zoom_factor_coefficient, in order to leave some space around the object for a nice visual outcome*/chart_transport_zoom_factor_coefficient, zoom_factor_max;
    String default_open_directory, data_directory, image_directory, path_file_init, path_file_recent, path_file_catalog, path_file_sample_sight, path_file_app_icon, path_file_error_icon, path_file_warning_icon, path_file_question_icon, path_file_plus_icon, path_file_list_icon, path_file_position_icon, path_file_map_icon, path_file_pencil_icon, path_file_trash_icon, path_file_arrow_icon, path_file_disconnect_icon, path_file_michele_icon, path_coastline_file, path_file_n_line, /*the root directory of the application*/root_directory;
    //the minimal angle at which two Routes must cross for them to be used to compute the position
    Angle min_crossing_angle;
    String degree_symbol;

    wxTimer* timer;
    //the disclaimer window shown at the beginning of the app
    QuestionFrame< ShowAll, CloseApp >* disclaimer;
    //type of the Boost library which contains the local time, intended as the time of the machine where the app is running
    boost::posix_time::ptime local_time;
    wxSystemSettings* settings;
    Chrono /*the code checks every time_check whether the operating system has changed its light/dark mode*/time_check_light_dark, /*the time step between two subsequent animation frames */animation_time;
    //this is true/false if the system is in dark/light mode, respectively
    bool dark_mode, idling;
    //the rectangle used to measure the size of the display
    wxDisplay display;
    //this referrs to the init file
    FileRW file_init;
    //a functor to properly close *this
    CloseApp* close_app;


    virtual bool OnInit();
    template<class T> void OnPressCtrlQ(T&);
    void OnTimer(wxTimerEvent&);
    template<class T> void ShowCharts(T&);
    void ShowCharts(void);
    void AnimateCharts(void);
    void ShowList(wxCommandEvent&);
    void where_am_I(String);

};

wxDECLARE_APP(MyApp);
wxIMPLEMENT_APP(MyApp);
