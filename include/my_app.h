//
//  my_app.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef my_app_h
#define my_app_h

#include <iostream>

#include <wx/display.h>

#include <boost/date_time.hpp>


#include "angle.h"
#include "catalog.h"
#include "chrono.h"
#include "color.h"
#include "double.h"
#include "int.h"
#include "length.h"
#include "list_frame.h"

using namespace std;

class Catalog;
class CloseApp;
class ListFrame;
template<class F_A, class F_B, class F_ABORT> class QuestionFrame;
class ShowAll;


class MyApp: public wxApp{
public:

    ListFrame *list_frame;
    //this is a pointer to a Catalog object which will be used by data
    Catalog *catalog;    
    //this rectangle contains the size of the display on which the *this is run
    wxRect rectangle_display;
    //the size of small and large buttons of the app
    wxSize size_small_button, size_large_button;
    //The default fore/background color for day/night vision of the app of all objects in *this. background_color also represents the default, plain background color of fields, such as BodyNameFields, and it is thus conceptally equivalent to 'white' color
    Color foreground_color, background_color, error_color, highlight_color, /*the color with which an item is highlighted in ListFrame, when mouse hovers over the related route or position in the plot area*/ color_selected_item,  /*the color of the horizon circle for the 3D projection*/ color_horizon;
    vector<Color> color_list;
    //the default font used by the operating system, and a bold font obtained by the defalult font by setting its weight to wxFONTWEIGHT_BOLD
    wxFont default_font, error_font, highlight_font;
    Length Re;
    Int time_zone, n_points_minor_ticks, /*the size of borders in pixels*/border, /*the size in pixels of the points used, for example, to draw coastlines */point_size, /*the number of snapshots used in animations*/n_animation_steps, n_points_plot_coastline_Mercator, n_points_plot_coastline_3D, n_points_routes, /*the preferred numer of ticks per axis in the chart plot: if there is enough space, I will put this number of ticks. If not, I will plot a nuber of ticks as cloase as possible to the preferred one*/n_intervals_ticks_preferred;
    Double  /*the displacemenet over the x (y) axis divided by the length of the x (y) axis used by the functions MoveUp, ..., MoveRight*/ relative_displacement, /*the default thickness of the line with which routes are drawn*/ standard_thickness_over_length_screen, /*the thickness with which highlighted routes are drawn*/ large_thickness_over_length_screen, /*the numerical coefficients which define the function which relates the slider value to the zoom factor*/a_zoom, b_zoom, e_zoom,  /*this is the ratio between the length of the ticks on both axes, and the width of the plot area (for the mercator projection)/angle of aperture of the observer circle (for the 3d projection)*/tick_length_over_width_plot_area, tick_length_over_aperture_circle_observer, /*the ration between the size of small/large buttons, icons and images in message frmes, and the width of the screen*/ size_small_button_over_width_screen, size_large_button_over_width_screen, size_icon_over_width_screen, size_message_image_over_width_screen, standard_thickness, large_thickness, /*when ChartTansportHandler zooms onto an object (e.g. a Route) I set the zoom factor at the end of the animation to be equal to be multiplied by chart_transport_zoom_factor_coefficient, in order to leave some space around the object for a nice visual outcome*/chart_transport_zoom_factor_coefficient, zoom_factor_max, /*when one calls AnimateToObject and the observer region (i.e., *rectangle_observer in the Mercator projection or circle_observer in the 3D projection) is centered to a Poisition very close to the target Position of the animation, I do the animation only if the distance between these two positions, divided by the size of the observer region, is larger than minimal_animation_distance_over_size_of_observer_region. This is to avoir NaNs in the transporting Route*/minimal_animation_distance_over_size_of_observer_region;
    String default_open_directory, data_directory, image_directory, path_file_init, path_file_recent, path_file_catalog, path_file_sample_sight, path_file_reset_icon, path_file_app_icon, path_file_error_icon, path_file_warning_icon, path_file_question_icon, path_file_info_icon, path_file_plus_icon, path_file_list_icon, path_file_position_icon, path_file_map_icon, path_file_pencil_icon, path_file_trash_icon, path_file_transport_icon, path_file_disconnect_icon, path_file_michele_icon, path_coastline_file, path_file_n_line, /*the root directory of the application*/root_directory;
    //the minimal angle at which two Routes must cross for them to be used to compute the position
    Angle min_crossing_angle;
    String degree_symbol;

    wxTimer* timer;
    //the disclaimer window shown at the beginning of the app
    QuestionFrame<ShowAll, CloseApp, CloseApp>* disclaimer;
    //type of the Boost library which contains the local time, intended as the time of the machine where the app is running
    boost::posix_time::ptime utc_time;
    wxSystemSettings* settings;
    Chrono /*the code checks every time_check whether the operating system has changed its light/dark mode*/time_check_light_dark, /*the time step between two subsequent animation frames */animation_time;
#ifdef WIN32
    Chrono /*when a graphical object is dragged, two Refresh() calls are spaced bv a time larger or equal to time_refresh. This is used only on WIN32, where the drag of a graphical object generates some ugly flashes if time_refresh is not used*/time_refresh;
#endif
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
    void set_icon_paths(void);

};

wxDECLARE_APP(MyApp);


#endif
