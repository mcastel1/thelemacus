#ifndef constants_h
#define constants_h

#include <iostream>


#include "angle.h"
#include "double.h"
#include "int.h"

using namespace std;

//class Length;


//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal
extern Int /*this is the low precision used for displaying data*/ display_precision, /*this is the high precision used for storing data and making calculations with it*/ data_precision;
extern Double /*the ratio between the width (height) of the plot area and the width (height) of the chart*/length_plot_area_over_length_chart, length_chart_over_length_chart_frame, /*this is the ratio between (the length of the borders drawn around the widgets) and (the length of the screen)*/length_border_over_length_screen;
//maximal and minimal latitude of the points in file path_file_coastlines
extern Angle max_lat, min_lat;
#define two_M_PI (2.0*M_PI)
#define deg_to_rad (two_M_PI/360.0)
#define rad_to_deg (1.0/deg_to_rad)
//one arcminute in units of radians
#define arcmin_radians (deg_to_rad/60.0)
//one tenth of arcminute in units of radians
#define tenth_arcmin_radians (arcmin_radians/10.0)
//MJD_min corresponds to Jan 1 2016 00-00-26.00 TAI, i.e., Jan 1 2016 00-00-00.00 UTC
#define MJD_min 57388.00041666666666666664
//NASA's webgeocalc datafiles show L lines per day
//#define L 24.0
#define n_lines_per_day_ephemerides 24.0
//the time window in which interpolation is made has a width of N lines in NASA's webgeocalc files
#define n_lines_ephemerides 24.0
#define epsilon_real (1e-12)
#define epsilon_double (1e-14)
#define max_iter (1e3)
//one in uint64_t format
#define one_uint64_t ((uint64_t)1)
#define accuracy_equal_approx 52
//earth radius in nautical miles: this is a temporary value of the radius of the Earth which will be written into the Length Re by MyApp::OnInit
#define Re_temp (60.0*360.0/two_M_PI)
#define RED     "\033[1;31m"
#define YELLOW     "\033[1;33m"
#define CYAN "\033[1;36m"
#define RESET   "\033[0m"
#define BOLD     "\033[1m"
//all possible chars that can enter in a signed integer
#define chars_int "+-0123456789"
//all possible chars that can enter in an unsigned integer
#define chars_unsigned_int "0123456789"
//all possible chars that can enter in a non-negative double
#define chars_double "+-0123456789."
#define color_white  (wxColour(255,255,255))
//the types of Routes, Projections, Length units of measure are stored here, so I don't have to re-type them everywhere in the codde
#define Route_types (vector<RouteType> {RouteType("loxodrome"), RouteType("orthodrome"), RouteType("circle of equal altitude")})
//the type of Routes used to transport a graphical object (i.e., a Position, a Route, etc...)
#define Route_types_for_transport (vector<RouteType> {Route_types[0], Route_types[1]})
#define Projection_types (vector<Projection> {Projection("Mercator"), Projection("3D")})
#define Limb_types (vector<LimbType> {LimbType("upper"), LimbType("center"), LimbType("lower")})
#define LengthFormat_types (vector<LengthFormat> {LengthFormat("time and speed"), LengthFormat("length")})
#define LengthUnit_types (vector<LengthUnit> {LengthUnit("nm"), LengthUnit("m"), LengthUnit("ft")})
#define SpeedUnit_types (vector<SpeedUnit> {SpeedUnit("kt"), SpeedUnit("km/h"), SpeedUnit("m/s")})
#define ListControl_types (vector<ListControlType> {ListControlType("sight"), ListControlType("position"), ListControlType("route")})
#define floor_min_lat (floor(rad_to_deg*((min_lat.normalize_pm_pi_ret().value))))
#define floor_max_lat (floor(rad_to_deg*((max_lat.normalize_pm_pi_ret().value))))
#define ceil_min_lat (ceil(rad_to_deg*((min_lat.normalize_pm_pi_ret().value))))
#define ceil_max_lat (ceil(rad_to_deg*((max_lat.normalize_pm_pi_ret().value))))
//latitude span
#define outfile_precision 16
//#define wxDEBUG_LEVEL 0
#define WXK_PLUS 61
#define WXK_MINUS 45
#define max_dialog 100


//this string defines the width of GUI fields hosting floating-point numbers
#define sample_width_floating_point_field "0.000000000"
#define sample_width_string_field "This is a sample string?"

//for debug
//#define path_file_coastline_data_blocked "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked_toy.csv"
//#define path_file_n_line "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked_toy.txt"
//for debug
//this number equals 11111111 in binary, and thus it will be used to pin out some parts of hexadecimal numbers 
#define hexadecimal_mask 0x0000ff

//static const int days_per_month_leap_temp[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
extern vector<unsigned int> days_per_month_leap;

//static const int days_per_month_common_temp[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//vector<unsigned int> days_per_month_common(days_per_month_common_temp, days_per_month_common_temp + sizeof(days_per_month_common_temp)/sizeof(days_per_month_common_temp[0]));

extern vector<unsigned int> days_per_month_common;


//these are the color codes in kml file format for a few populat colors (red, etc...);
extern vector<string> kml_colors;
extern vector<string> hex_colors;



#endif
