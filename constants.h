//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal
//this is the high precision used for storing data and making calculations with it
#define data_precision 32
//this is the low precision used for displaying data
#define display_precision 4
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
#define epsilon_double (1e-15)
#define max_iter (1e3)
//one nautical mile in kilometers
#define nm 1.852
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
#define n_points_minor_ticks 3
#define path_file_utc_date_and_time "utc.txt"
#define path_file_init "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/init.txt"
#define path_file_recent "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/recent.txt"
#define path_file_catalog "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/catalog.txt"
#define path_file_temp "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/temp.txt"
//maximal and minimal latitude of the points in file define path_file_coastlines
#define max_lat (83.6664731)
#define min_lat (-78.7290778)
//for debug
//#define max_lat (-69.5)
//#define min_lat (-78.7290778)
//for debug
#define floor_min_lat (floor(min_lat))
#define floor_max_lat (floor(max_lat))
#define ceil_min_lat (ceil(min_lat))
//latitude span
#define span_lat ((floor_max_lat-floor_min_lat+1)
//the ratio between the width (height) of the plot area and the width (height) of the chart.
#define length_plot_area_over_length_chart 0.8
#define length_chart_over_length_chart_frame 0.8
//this is the ratio between (the length of the borders drawn around the widgets) and (the length of the frame in which the widgets are located)
#define length_border_over_length_frame 0.01
#define outfile_precision 16
//the maximum allowed value of the slider in ChartFrame
//#define wxDEBUG_LEVEL 0


//this string defines the width of GUI fields hosting floating-point numbers
#define sample_width_floating_point_field "0.000000000"
#define sample_width_string_field "Et repellat optio nam iste voluptatum in magnam?"
#define path_file_app_icon "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/jolly_rogers_png.png"
#define path_file_pencil_icon "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/pencil_icon.png"
#define path_file_coastline_data_blocked "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
#define path_file_n_line "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked.txt"
//for debug
//#define path_file_coastline_data_blocked "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked_toy.csv"
//#define path_file_n_line "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked_toy.txt"
//for debug


