//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal
Int /*this is the low precision used for displaying data*/ display_precision, /*this is the high precision used for storing data and making calculations with it*/ data_precision;
Double /*the ratio between the width (height) of the plot area and the width (height) of the chart*/length_plot_area_over_length_chart, length_chart_over_length_chart_frame, /*this is the ratio between (the length of the borders drawn around the widgets) and (the length of the screen)*/length_border_over_length_screen;
//maximal and minimal latitude of the points in file path_file_coastlines
Angle max_lat, min_lat;

#define k (2.0*M_PI/360.0)
#define K (1.0/k)
//one arcminute in units of radians
#define arcmin_radians (k/60.0)
//one tenth of arcminute in units of radians
#define tenth_arcmin_radians (arcmin_radians/10.0)
//MJD_min corresponds to Jan 1 2016 00-00-26.00 TAI, i.e., Jan 1 2016 00-00-00.00 UTC
#define MJD_min 57388.00041666666666666664
//NASA's webgeocalc datafiles show L lines per day
#define L 24.0
//the time window in which interpolation is made has a width of N lines in NASA's webgeocalc files
#define N 24.0
#define epsrel (1e-12)
#define epsilon_double (1e-14)
#define max_iter (1e3)
//one in uint64_t format
#define one_uint64_t ((uint64_t)1)
#define accuracy_equal_approx 52
//one nautical mile in kilometers
#define nm 1.852
//one nautical mile in feet
#define nm_ft (nm*1e3*3937.0/1200.0)
//earth radius in nautical miles
#define Re (60.0*360.0/(2.0*M_PI))
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
//the types of Routes are stored here, so I don't have to re-type them everywhere in the codde
#define Route_types (vector<String> {String("loxordome"), String("orthodrome"), String("circle of equal altitude")})
#define floor_min_lat (floor(K*((min_lat.normalize_pm_pi_ret()).value)))
#define floor_max_lat (floor(K*((max_lat.normalize_pm_pi_ret()).value)))
#define ceil_min_lat (ceil(K*((min_lat.normalize_pm_pi_ret()).value)))
//latitude span
#define span_lat ((floor_max_lat-floor_min_lat+1)
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

static const int days_per_month_leap_temp[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_leap(days_per_month_leap_temp, days_per_month_leap_temp + sizeof(days_per_month_leap_temp)/sizeof(days_per_month_leap_temp[0]));

static const int days_per_month_common_temp[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_common(days_per_month_common_temp, days_per_month_common_temp + sizeof(days_per_month_common_temp)/sizeof(days_per_month_common_temp[0]));


//these are the color codes in kml file format for a few populat colors (red, etc...);
string kml_colors[] = {"ff0000ff", "ffff0000", "ff336699", "ff00ff00", "ff0080ff", "ffff00ff"};
string hex_colors[] = {"#000000", "#0000FF", "#00FF00", "#663300", "#3399FF", "#0000CC"};
