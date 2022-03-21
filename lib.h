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

//class CheckBody;
//struct CheckLimb;
//template<class T> struct CheckCheck;
//struct CheckChrono;
//template<class P> class CheckAngle;
//template<class P> struct CheckSign;
//template<class P> struct CheckArcDegree;
//template<class P> struct CheckArcMinute;
//struct CheckLength;
//class CheckDate;
//class CheckYear;
//class CheckMonth;
//class CheckDay;
//struct CheckHour;
//struct CheckMinute;
//struct CheckSecond;
//template<class P> struct CheckString;
//template<class P> struct SetStringToCurrentTime;
//struct TabulateDays;
//template<class T> class PrintErrorMessage;
//struct OnSelectInListBox;

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


//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal
//this is the high precision used for storing data and making calculations with it 
#define data_precision 32
//this is the low precision used for displaying data
#define display_precision 4
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
//MJD_min corresponds to Jan 1 2016 00-00-26.00 TAI, i.e., Jan 1 2016 00-00-00.00 UTC  
#define MJD_min 57388.00041666666666666664
//NASA's webgeocalc datafiles show L lines per day
#define L 24.0
//the time window in which interpolation is made has a width of N lines in NASA's webgeocalc files
#define N 24.0
#define epsrel (1e-12)
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

static const int days_per_month_leap_temp[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_leap(days_per_month_leap_temp, days_per_month_leap_temp + sizeof(days_per_month_leap_temp)/sizeof(days_per_month_leap_temp[0]));

static const int days_per_month_common_temp[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_common(days_per_month_common_temp, days_per_month_common_temp + sizeof(days_per_month_common_temp)/sizeof(days_per_month_common_temp[0]));


#define path_file_utc_date_and_time "utc.txt"
#define path_file_init "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/init.txt"
#define path_file_catalog "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/catalog.txt"
//these are the color codes in kml file format for a few populat colors (red, etc...);
string kml_colors[] = {"ff0000ff", "ffff0000", "ff336699", "ff00ff00", "ff0080ff", "ffff00ff"};
string hex_colors[] = {"#000000", "#0000FF", "#00FF00", "#663300", "#3399FF", "#0000CC"};

//maximal and minimal latitude of the points in file define path_file_coastlines
#define max_lat (83.6664731)
#define min_lat (-78.7290778)
#define floor_min_lat (floor(min_lat))
#define floor_max_lat (floor(max_lat))
//latitude span
#define span_lat ((floor_max_lat-floor_min_lat+1)
//the ratio between the width (height) of the plot area and the width (height) of the chart.
#define length_plot_area_over_length_chart 0.8
#define length_chart_over_length_chart_frame 0.8
//this is the ratio between (the length of the borders drawn around the widgets) and (the length of the frame in which the widgets are located)
#define length_border_over_length_frame 0.01
#define outfile_precision 16
//the maximum allowed value of the slider in ChartFrame
#define value_slider_max 1000
//#define wxDEBUG_LEVEL 0


//this string defines the width of GUI fields hosting floating-point numbers
#define sample_width_floating_point_field "0.000000000"
#define sample_width_string_field "Et repellat optio nam iste voluptatum in magnam?"
#define path_file_app_icon "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/jolly_rogers_png.png"
#define path_file_chart "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/chart.png"
#define path_file_pencil_icon "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/pencil_icon.png"
#define path_file_coastlines "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_toy.csv"
#define path_file_coastline_data_blocked "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
#define path_file_n_line "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked.txt"
//#define path_file_selected_coastline_data "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt"



//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal


//this function returns the rectangular x value of the spherical Mercator projection from the longitude lambda (expressed in degrees, positive towards W).
inline double x_mercator(double lambda){
    
    double lambda_temp;
    
    lambda_temp = lambda;
    
    //puts the lambda_temp value in the range [-180.0, +180.0]
    if(lambda < -180.0){lambda_temp+=360.0;}
    if(lambda > +180.0){lambda_temp-=360.0;}
    
    return(-k*lambda_temp);
    
}

//this function returns the rectangular y value of the spherical Mercator projection from the latitude phi (expressed in degrees).
inline double y_mercator(double phi){
    
    double phi_temp;
    
    phi_temp = phi;
    
    //puts the phi_temp value in the range [-180.0, +180.0]
    if(phi > +180.0){phi_temp-=360.0;}
    
    return log(1./cos(phi_temp*k) + tan(phi_temp*k));
    
}

//this function returns the longitude value (expressed in degrees, positive towards W) of the inverse spherical Mercator projection from the rectangular x value
inline double lambda_mercator(double x){
    
    double output;
    
    if((x<=0.0) && (x>-M_PI)){output = -x*K;}
    else{output = -x*K+360.0;}
    
    return output;
    
}

//this function returns the latitude value (expressed in degrees) of the inverse spherical Mercator projection from the rectangular y value
inline double phi_mercator(double y){
    
    return(K*atan(sinh(y)));
    
}



inline double cot(double x){
    
    return 1.0/tan(x);
    
}

inline double csc(double x){
    
    return 1.0/sin(x);
    
}

inline double atan(double x, double y){
    
    if(x > 0.0){
        
        return atan(y/x);
        
    }else{
        
        return(atan(y/x)+M_PI);
    }
    
}



class String{
    
public:
    string value;
    
    String();
    String(string);
    void enter(String, String);
    void print(String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    void set(String, String, String);
    String append(String);
    String prepend(String);
    
    bool operator==(const String&), operator!=(const String&);
    
};

//this is a wxFrame designed to show a message to the GUI user
class MessageFrame: public wxFrame{
    
public:
    MessageFrame(wxWindow*, const wxString&, const wxString&, const wxPoint&, const wxSize&, String);
    
    wxPanel *panel;
    wxBoxSizer *sizer_h, *sizer_v, *sizer_buttons;
    wxGridSizer* sizer_grid;
    wxButton* button_ok;
    wxStaticBitmap* image;
    
    void OnPressOk(wxCommandEvent&);

};

//this is a wxFrame designed to ask a  yes/no question to the GUI user. F_YES is the type of the functor struct which will be called when the button yes is pressed. This type is variables, so it has been 'templated'. Same for F_NO.
template<typename F_YES, typename F_NO> class QuestionFrame: public wxFrame{
    
public:
    QuestionFrame(wxWindow*, F_YES*, F_NO*, const wxString&, const wxString&, const wxPoint&, const wxSize&, String);
    
    //the non-GUI object connected to the GUI object MessageFrame
    wxPanel *panel;
    wxBoxSizer *sizer_h, *sizer_v, *sizer_buttons;
    wxGridSizer* sizer_grid;
    wxButton* button_yes, *button_no;
    wxStaticBitmap* image;
    //pointer to the struct containing the functor which will be called when the button yes is pressed
    F_YES* f_yes;
    //pointer to the struct containing the functor which will be called when the button no is pressed
    F_NO* f_no;

    void OnPressYes(wxCommandEvent&);
    void OnPressNo(wxCommandEvent&);

};


//this function checks whether the  unsigned int in string s is formatted correctly and, if check_interval = true, it also checks whether the this unsigned int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_unsigned_int(string s, unsigned int* i, bool check_interval, unsigned int min, unsigned int sup){
    
    bool check;
    unsigned int j;
    
    if(/*here I check that the string s is not empty*/(!s.empty()) && /*here I check whether the quantity entered in s is an unsigned integer, i.e., it contains only the characters 0123456789*/ ((s.find_first_not_of(chars_unsigned_int)) == (std::string::npos))){
        
        j = stoi(s, NULL, 10);
        
        if(check_interval){
            
            check = ((j >= min) && (j < sup));
            
        }else{
            
            check = true;
        }
        
    }else{
        
        check = false;
        
    }
    
    if((i != NULL) && check){
        
        (*i) = j;
        
    }
    
    return check;
    
}



//this function checks whether the int in string s is formatted correctly and, if check_interval = true, it also checks whether the this  int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_int(string s, int* i, bool check_interval, int min, int sup){
    
    bool check;
    int j;
    
    if(/*here I check whether the quantity entered in s is an integer, i.e., it contains only the characters +-0123456789*/ ((s.find_first_not_of(chars_int)) == (std::string::npos))){
        
        j = stoi(s, NULL, 10);
        
        if(check_interval){
            
            check = ((j >= min) && (j < sup));
            
        }else{
            
            check = true;
            
        }
        
    }else{
        
        check = false;
        
    }
    
    if((i != NULL) && check){
        
        (*i) = j;
        
    }
    
    return check;
    
}


//this function asks the user to enter an unsigned int from keyboard and checks whether the entered value is an unsigned int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_unsigned_int(unsigned int* i, bool check_interval, unsigned int min, unsigned int sup, String name, String prefix){
    
    string s;
    bool check;
    
    do{
        
        s.clear();
        
        cout << prefix.value << "Enter " << name.value << ":";
        getline(cin >> ws, s);
        
        check = check_unsigned_int(s, i, check_interval, min, sup);
        
        if(!check){
            
            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
            
        }
        
    }while(!check);
    
}



//this function asks the user to enter an  int from keyboard and checks whether the entered value is an  int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_int(int* i, bool check_interval, int min, int sup, String name, String prefix){
    
    string s;
    bool check;
    
    do{
        
        s.clear();
        
        cout << prefix.value << "Enter " << name.value << ":";
        getline(cin >> ws, s);
        
        check = check_int(s, i, check_interval, min, sup);
        
        if(!check){
            
            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
            
        }
        
    }while(!check);
    
}


//this function checks whether the double in string s is formatted correctly and, if check_interval = true, it also checks whether the this double lies in [min, sup). If x != NULL and the check is ok, it also writes the value of the double read from s into (*x)
bool check_double(string s, double* x, bool check_interval, double min, double sup){
    
    bool check;
    double y;
    
    if(/*here I check that the string s is not empty*/(!s.empty()) && (/*here I check whether the quantity in s contains the allowed chars for double, i.e., it contains only the characters in chars_double*/ ((s.find_first_not_of(chars_double)) == (std::string::npos))) && /*here I count whether the '.' occurs zero or one time*/(count(s.begin(), s.end(), '.') <= 1) && /*here I count whether the '+' occurs zero or one time*/(count(s.begin(), s.end(), '+') <= 1)  && /*here I count whether the '-' occurs zero or one time*/(count(s.begin(), s.end(), '-') <= 1)){
        
        //if the check above passed, then I proceed and write s into y
        y = stod(s);
        
        if(check_interval){
            
            check = ((y >= min) && (y < sup));
            
        }else{
            
            check = true;
            
        }
        
    }else{
        
        check = false;
        
    }
    
    //if x == NULL, then this function is meant to be used to check the correct format of s only, not to write its value to x.
    if((x != NULL) && check){
        
        (*x) = y;
        
    }
    
    return check;
    
}


//this function asks the user to enter a double from keyboard and checks whether the entered value contains the allowed chars for double and, if check_interval = true, that the entered value lies in [min, sup)
void enter_double(double* x, bool check_interval, double min, double sup, String name, String prefix){
    
    string s;
    bool check;
    
    do{
        
        s.clear();
        
        cout << prefix.value << "Enter " << name.value << ":";
        getline(cin >> ws, s);
        
        check = check_double(s, x, check_interval, min, sup);
        
        if(!check){
            
            cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
            
        }
        
    }while(!check);
    
}






class Int{
    
public:
    int value;
    
    void read_from_file(String, File&, bool, String);
    void enter(String, String);
    void set(String, int, String);
    void print(String, String, ostream&);
    
    bool operator==(const Int&), operator!=(const Int&);
    
};





class Double{
    
public:
    double value;
    
    void read_from_file(String, File&, bool, String);
    void print(String, String, ostream&);
    
};

class File{
    
public:
    fstream value;
    String name;
    unsigned int number_of_lines;
    
    File();
    void set_name(String);
    void enter_name(String);
    bool open(String, String);
    void close(String);
    void remove(String);
    void count_lines(String);
    bool check_if_exists(String);
    
};



bool get_date_hour(String &line, String prefix){
    
    File file;
    stringstream command;
    bool check;
    
    check = true;
    
    command.str("");
    command << "rm -rf output.out; date \"+%Y-%m-%d %H:%M:%S\" >> output.out";
    system(command.str().c_str());
    
    ((file.name).value) = "output.out";
    check &= file.open(String("in"), prefix);
    line.value.clear();
    getline(file.value, line.value);
    file.close(prefix);
    
    command.str("");
    command << "rm -rf output.out";
    system(command.str().c_str());
    
    if(!check){
        cout << prefix.value << RED << "\tI could not get hour and date!\n" << RESET;
    }
    
    return check;
    
}





class Length{
    
public:
    double value;
    
    void set(String, double, String);
    void enter(String, String, String);
    void print(String, String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    bool check_valid(String, String);
    string to_string(String, unsigned int);
    bool operator> (const Length&), operator==(const Length&), operator!=(const Length&);
    Length operator + (const Length&);
    
};



class Speed{
    
public:
    double value;
    void enter(String, String);
    bool check_valid(String, String);
    void print(String, String, ostream&);
    
    
};




class Answer{
    
public:
    char value;
    
    Answer();
    Answer(char, String);
    void enter(String, String);
    bool set(String, char, String);
    //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
    void print(String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    bool operator==(const Answer&), operator !=(const Answer&);
    
};



class Angle{
    
public:
    
    double value;
    void normalize(void);
    void normalize_pm_pi(void);
    void enter(String, String);
    void set(String, double, String);
    void print(String, String, ostream&);
    void to_deg_min(unsigned int*, double*);
    void from_sign_deg_min(char, unsigned int, double);
    void read_from_file(String, File&, bool, String);
    string to_string(String, unsigned int);
    string deg_to_string(String, unsigned int);
    string min_to_string(String, unsigned int);

    bool operator==(const Angle&), operator>(const Angle&);
    Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
    
};




class Position{
    
public:
    //latitude and longitude of the position
    Angle phi, lambda;
    //label to add a note about the position
    String label;
    //this is a list of the items which are part of a Position object (phi, lambda, ..)
    vector<String> items;
    
    Position();
    void enter(String, String);
    void modify(String);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void print(String, String, ostream&);
    void read_from_file(File&, String);
    //this function transports the position and returns the Route with which it has been transported
    Route transport(String);
    string to_string(unsigned int);
    bool distance(Position, Length*, String, String);
    bool operator==(const Position&);
    
    
};



//if type = l or o, the parameters specifying the route are start, alpha, l. if type = c, the parameters specifying the route are GP and omega. 
class Route{
    
public:
    String type, label, temp_prefix;
    //starting position of the route
    Position start, end, GP;
    //alpha: the angle that the vector tangent to the route describes with the local meridian at start; omega: the aperture angle of the cone for circles of equal altitude
    Angle alpha, omega;
    //the length of the route
    Length l;
    Speed sog;
    //this is the position in sight_list of the sight linked to route. If there is no sight linked to route, then related_sight = -1.
    int related_sight;
    
    void enter(String, String);
    void print(String, String, ostream&);
    void read_from_file(File&, String);
    void compute_end(String);
    bool crossing(Route, vector<Position>*, double*, String);
    void transport(String);
    static double lambda_minus_pi(double, void*);
    bool t_crossing(Route, vector<Angle>*, String);
    bool closest_point_to(Position*, Angle*, Position, String);
    
};



class Date{
    
public:
    unsigned int Y, M, D;
    bool Y_is_leap_year;
    vector<unsigned int> days_per_month;
    
    void print(String, String, ostream&);
    void enter(String, String);
    bool read_from_file(String, File&, bool, String);
    string to_string(void);
    void check_leap_year(void);
    bool set_current(String);
    
};

class Chrono{
    
public:
    unsigned int h, m;
    double s;
    
    void print(String, String, ostream&);
    bool set(String, double, String);
    bool set_current(String);
    void enter(String, String);
    bool read_from_file(String, File&, bool, String);
    string to_string(unsigned int);
    
    bool operator==(const Chrono&), operator!=(const Chrono&);
    
};



class Time{
    
public:
    Date date;
    Chrono chrono;
    //is s used?
    double s, MJD;
    void enter(String, String);
    void print(String, String, ostream&);
    bool set_current(String);
    bool read_from_file(String, File&, String);
    
    void to_MJD(void);
    void to_TAI(void);
    
    string to_string(unsigned int);
    bool operator==(const Time&), operator!=(const Time&), operator> (const Time&);
    void operator += (const Chrono&);
    void operator -= (const Chrono&);
    
};



class Atmosphere{
    
public:
    Length earth_radius;
    unsigned int n_layers;
    double A, B, P_dry_0, alpha, beta, gamma, T0;
    vector<double> h, lambda, t;
    void set(void);
    double T(Length), n(Length), dTdz(Length), dndz(Length);
    
};

class Body{
    
public:
    String name, type;
    Length radius;
    Angle RA, d;
    void enter(String, Catalog, String);
    bool check(unsigned int*, Catalog, String);
    void print(String, String, ostream&);
    void read_from_file(String, File&, String);
    
    bool operator==(const Body&);
    
    
};



class Limb{
    
public:
    char value;
    void enter(String, String);
    void print(String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    
    bool operator==(const Limb&);
    
};

class Sight{
    
public:
    Time master_clock_date_and_hour, /*this is the time of the observation, in the TAI time scale*/time;
    //stopwatch is the reading [hh:mm:ss.s] on the stopwatch
    Chrono TAI_minus_UTC, stopwatch;
    Angle index_error, H_s, H_a, H_o, H_i, DH_refraction, DH_dip, DH_parallax_and_limb;
    Length r, height_of_eye;
    Atmosphere atmosphere;
    Body body;
    Limb limb;
    // use_stopwatch = 'n' -> time is in format "UTC" time. use_stopwatch  = 'y' -> master clock UTC time + stopwatch reading
    Answer artificial_horizon, use_stopwatch;
    //label to add a note about the sight
    String label;
    //this is the position in route_list of the route linked to Sight. If there is no route linked to Sight, then related_route = -1.
    Int related_route;
    //all_items is a list of all the possible items which are part of a Sight object (master_clock_date_and_hour, body, ...). items is the list of items specific to a given Sight object: items may not include all the elements of all_items
    vector<String> items, all_items;
    
    
    Sight();
    static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
    bool get_coordinates(Route*, String);
    void compute_DH_dip(String);
    bool compute_DH_refraction(String);
    void compute_DH_parallax_and_limb(String);
    
    void compute_H_a(String);
    bool compute_H_o(String);
    
    bool enter(Catalog, String, String);
    bool modify(Catalog, String);
    void print(String, String, ostream&);
    bool read_from_file(File&, String);
    bool reduce(Route*, String);
    bool check_data_time_interval(String);
    
    void add_to_wxListCtrl(long, wxListCtrl*);
    
};

class Catalog{
    
public:
    vector<Body> list;
    Catalog(String, String);
    void add(String, String, double);
    void print(String, ostream&);
    
};



class Plot{
    
public:
    Catalog* catalog;
    File file_init, file_id, file_gnuplot, file_kml, file_boundary;
    int job_id;
    stringstream command, plot_command;
    vector<Sight> sight_list;
    vector<Position> position_list;
    vector<Route> route_list;
    vector<String> choices;
    vector<unsigned int> crossing_route_list;
    Angle phi_min, phi_max, lambda_min, lambda_max;
    Int n_points_plot_coastline, n_intervals_tics;
    
    Plot(Catalog*, String);
    //~Plot();
    bool add_sight_and_reduce(Sight*, String);
    bool modify_sight(unsigned int, String);
    void transport_route(unsigned int, String);
    void add_position(String);
    void add_route(String);
    void remove_sight(unsigned int, Answer, String);
    void transport_position(unsigned int, String);
    void remove_position(unsigned int, String);
    void remove_route(unsigned int, String);
    bool read_from_file(File&, String);
    void print(bool, String, ostream&);
    void print_to_kml(String);
    void print_sights(String, ostream&);
    void print_positions(String, ostream&);
    void print_routes(bool, String, ostream&);
    void show(bool, String);
    void menu(String);
    void compute_crossings(String);
    
};



//class which defines a functor which closes a frame with arbitrary type F
template<class F> class CloseFrame{
    
public:
    
    //the frame to be closed
    F* frame;
    CloseFrame(F*);
    
    template<class T> void operator()(T&);
    
};

//this checks if an element of the Body class is valid
class CheckBody{
    
public:
    
    BodyField* p;
    
    CheckBody(BodyField*);
    template<class T> void operator()(T&);
    
    
};

struct CheckLimb{
    
    LimbField* p;
    
    template<class T> void operator()(T&);
    
    
    
};


template<class T> struct CheckCheck{
    
    CheckField<T>* p;
    
    //this functor checks whether a GUI Check field is filled correctly and writes its value into the relative non-GUI field
    template<class R> void operator()(R&);
    
};



template<class P> struct CheckSign{
    
    AngleField<P>* p;
    
    template <class T> void operator()(T&);
    
    
};

template<class P> struct CheckArcDegree{
    
    AngleField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

template<class P> struct CheckArcMinute{
    
    AngleField<P>* p;
    
    template <class T> void operator()(T&);
    
    
};


template<class P> class CheckAngle{
    
public:
    
    //p is the AngleField which is parent of the CheckAngle object: the CheckAngle object checks the validity of the entries in AngleField
    AngleField<P>* p;
    CheckSign<P> check_sign;
    CheckArcDegree<P> check_arc_degree;
    CheckArcMinute<P> check_arc_minute;
    
    CheckAngle(AngleField<P>*);
    
    template <class T> void operator()(T&);
    
};

struct TabulateDays{
    
    DateField* p;
    
    template<class T> void operator()(T&);
    
    
};


class CheckYear{
    
public:
    
    DateField* p;
    TabulateDays tabulate_days;
    
    CheckYear(DateField*);
    template<class T> void operator()(T&);
    
    
};

class CheckMonth{
    
public:

    
    DateField* p;
    TabulateDays tabulate_days;
    
    CheckMonth(DateField*);
    template<class T> void operator()(T&);
    
    
};

class CheckDay{
    
public:

    DateField* p;
    
    CheckDay(DateField*);
    template<class T> void operator()(T&);
    
    
};


class CheckDate{
    
public:
    
    //the parent DateField
    DateField* p;
    CheckYear *check_year;
    CheckMonth *check_month;
    CheckDay *check_day;
    
    //constructor, which sets the parent
    CheckDate(DateField*);
    
    template <class T> void operator()(T&);
    
};


struct CheckLength{
    
    LengthField* p;
    
    template<class T> void operator()(T&);
    
    
};

template<class P> struct CheckString{
    
    StringField<P>* p;
    
    template<class T> void operator()(T&);
    
};

class CheckHour{
    
public:
    
    ChronoField* p;
    
    CheckHour(ChronoField*);
    template<class T> void operator()(T&);
    
    
};

class CheckMinute{

public:
    
    ChronoField* p;
    
    CheckMinute(ChronoField*);
    template<class T> void operator()(T&);
    
    
};

class CheckSecond{
    
public:
    
    ChronoField* p;
    
    CheckSecond(ChronoField*);
    template<class T> void operator()(T&);
    
};


class CheckChrono{
    
public:
    
    //the parent field of the CheckChrono object
    ChronoField* p;
    CheckHour *check_hour;
    CheckMinute *check_minute;
    CheckSecond *check_second;
    
    CheckChrono(ChronoField*);
    template<class T> void operator()(T&);
    
};


template<class P> struct SetStringToCurrentTime{
    
    StringField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

//this class defines the functor () used to remove a sight from the non-GUI object plot
class DeleteSight{
    
public:
    
    DeleteSight(ListFrame*, Answer);
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the sight to be removed
    long i_sight_to_remove;
    //this is equal to 'y' if the route related to the removed sight has to be removed too, and 'n' otherwise
    Answer remove_related_route;
    
    void operator()(wxCommandEvent&);
    
};


//this is a GUI field contaning a binary checkbox, which is either checked or unchecked
template<class T> class CheckField{
    
public:
    
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    Answer* answer;
    //related_field is a GUI field (such as ChronoField, etc) related to this CheckField, such that: if direct_reverse = true->  when the checkbox in this CheckFIeld is checked (unchecked), related_field is active (inactive). If direct_reverse = false ->  when the checkbox in this CheckFIeld is unchecked (checked), related_field is active (inactive).
    T* related_field;
    bool direct_reverse;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxCheckBox with the name of the bodies
    wxCheckBox* checkbox;
    CheckCheck<T> check;
    
    CheckField(SightFrame*, Answer*, T*, bool);
    
    template<class R> void InsertIn(R*);
    template<class S> void get(S&);
    void set(void);
    
    
};



class DrawPanel : public wxPanel{
    
public:
    DrawPanel(ChartPanel*);
    ChartFrame* parent;
    XYChart* c;
    wxPoint position_draw_panel, position_plot_area, position_start_selection, position_end_selection, position_screen_now, position_start_drag, position_end_drag, position_now_drag;
    wxSize size_plot_area;
    wxSlider* slider;
    /*x_min, x_max, y_min, y_max do correspond to lambda_min, lambda_max, etc... They are ordered in such a way that x_min <= x_max and y_min <= y_max always. */
    double x_min, x_max, y_min, y_max,
    /*these are the values of x_min .. y_max when the plot is first drawn*/
    x_min_0, x_max_0, y_min_0, y_max_0,
    /*these are the values of x_min, ... y_max after each sliding event, corresponding to lambda_min, ... , phi_max read from file*/x_min_old, x_max_old, y_min_old, y_max_old, /*this is the ratio between the length of the tics on both axes, and the width of the plot area*/tic_length_over_width_plot_area, /* gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes when setting the tics on the x axis, and similarly for gamma_phi*/gamma_lambda, gamma_phi, /*these are the angular separations in latitude and longitude between meridians and parallels, respectively */delta_lambda, delta_phi;
    wxStaticText*text_position_start, *text_position_end;
    bool selection_rectangle, /*this is true if the mouse is dragging with the left button pressed*/mouse_dragging;
    //these are the positions where the right mouse button is clicked at the beginning and at the end of the drawing process for the selection rectangle on the world's chart
    Position p_start, p_end;
    wxSizer* sizer_h, *sizer_v;
    //the chart contains the plot area, and the following quantities are the width and height of chart and plot area
    unsigned int width_chart, height_chart, /*these are the values of width/height_chart when the chart is first drawn*/width_chart_0, height_chart_0, width_plot_area, height_plot_area, tic_length;
    
    void Draw(void);
    void PaintEvent(wxPaintEvent & evt);
    void PaintNow();
    void ScreenToGeo(wxPoint, Position*);
    void GeoToScreen(Position, wxPoint*);
    void Update_lambda_phi_min_max(void);
    void Update_x_y_min_max(void);
    
    void Render(wxDC& dc);
    
    // some useful events
    void GetMouseGeoPosition(Position*);
    void OnMouseMovement(wxMouseEvent&);
    void OnMouseLeftDown(wxMouseEvent&);
    void OnMouseLeftUp(wxMouseEvent&);
    void OnMouseRightDown(wxMouseEvent&);
    void OnMouseDrag(wxMouseEvent&);
    void OnScroll(wxScrollEvent&);
    
    DECLARE_EVENT_TABLE()
};

class BodyField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString bodies;
    //this points to a Body object, which contains the date written in the GUI field of this
    Body* body;
    Catalog* catalog;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    CheckBody* check;
    
    bool ok;
    
    BodyField(SightFrame*, Body*, Catalog*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};


class LimbField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString limbs;
    //this points to a Limn object, which contains the data written in the GUI field of this
    Limb* limb;
    wxBoxSizer *sizer_h, *sizer_v;
    bool ok;
    CheckLimb check;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    
    LimbField(SightFrame*, Limb*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};



//class for graphical object: a field to enter an angle, composed of a box for the sign, a box for the degrees, a degree text symbol, another box for minutes and a minute text symbol
template<class P> class AngleField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    wxArrayString signs, degrees;
    //the format of the AngleField: String("") for angles  between 0 and 2*pi, String("+-") for angles with a sign between 0 and pi, String("NS") for latitudes between -pi/2 and pi/2, and String("EW") for longitudes between -pi and pi
    String format;
    //degrees and minutes boxes
    wxComboBox*sign, * deg;
    wxTextCtrl *min;
    //texts
    wxStaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    Angle* angle;
    //deg_ok = true if the degrees part of this angle is formatted properly and set to the same value as the degree part of angle, and simiarly for min
    bool sign_ok, deg_ok, min_ok;
    CheckAngle<P>* check_angle;
    
    
    AngleField(P*, Angle*, String);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a length, composed of a box
class LengthField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    //degrees and minutes boxes
    wxTextCtrl *value;
    //texts
    wxStaticText* text;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    CheckLength check;
    
    LengthField(SightFrame*, Length*);
    void set(void);
    template<class T> void get(T&);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a String, composed of a box. P is the type of the object in which this StringField will be inserted
template<class P> class StringField{
    
public:
    //the parent where this StringField object will be inserted
    P* parent_frame;
    //label box
    wxTextCtrl *value;
    wxBoxSizer *sizer_h, *sizer_v;
    //non-GUI object related to this
    String* string;
    CheckString<P> check;
    SetStringToCurrentTime<P> set_string_to_current_time;
    
    StringField(P*, String*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    
};



class DateField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString days, months;
    //year, month and day boxes
    wxTextCtrl *year;
    wxComboBox *month, *day;
    //texts
    wxStaticText* text_hyphen_1, *text_hyphen_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Date* date;
    //year_ok = true if the year is formatted properly and set to the same value as date->Y, and similarly for the other variables
    bool year_ok, month_ok, day_ok;
    CheckDate *check;
    
    DateField(SightFrame*, Date*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};


class ChronoField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString hours, minutes;
    //hour and minute  boxes
    wxComboBox *hour, *minute;
    //second text control
    wxTextCtrl *second;
    //texts
    wxStaticText* text_colon_1, *text_colon_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Chrono* chrono;
    //hour_ok = true if the hour is formatted properly and set to the same value as chrono->h, and similarly for the other variables
    bool hour_ok, minute_ok, second_ok, /*this variable = true if this has been just enabled, and false otherwise*/just_enabled;
    CheckChrono* check;
    
    ChronoField(SightFrame*, Chrono*);
    void set(Chrono);
    void Enable(bool);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};









//this functor pops out an error-message window with title tile and error message message, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this struct adaptable
template<class T> class PrintErrorMessage{
    
public:
    
    T* f;
    wxControl* control;
    String title, message;
    
    PrintErrorMessage(T*);
    
    void operator()(void);
    
    
};



//this is a wxFrame designed to contain the list of sights, routes, etc...
class ListFrame: public wxFrame{
    
public:
    //this frame has no parent, because it is supposed to be the main frame of the appplication
    ListFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix);
    
    //this is a pointer to the non-GUI object Plot which is related to the GUI object this
    Plot* plot;
    //this is a pointer to a Catalog object which will be used by plot
    Catalog *catalog;
    //    wxListBox* listbox;
    wxListCtrl* listcontrol_sights, *listcontrol_positions, *listcontrol_routes;
    wxPanel *panel;
    wxButton *button_add_sight, *button_delete_sight, *button_add_position, *button_delete_position, *button_add_route, *button_delete_route;
    wxBitmapButton *button_modify_sight, *button_modify_position, *button_modify_route;
    wxSizer* sizer_h, *sizer_v, *sizer_buttons_sight, *sizer_buttons_position, *sizer_buttons_route;
    wxStaticBoxSizer* sizer_box_sight, *sizer_box_position, *sizer_box_route;
    DeleteSight *delete_sight, *delete_sight_and_related_route;
    
    void OnAddSight(wxCommandEvent& event);
    void OnModifySight(wxCommandEvent& event);
    void OnPressDeleteSight(wxCommandEvent& event);
    
    void OnAddPosition(wxCommandEvent& event);
    void OnModifyPosition(wxCommandEvent& event);
    void OnDeletePosition(wxCommandEvent& event);
    
};



class SightFrame: public wxFrame{
    
public:
    SightFrame(ListFrame*, Sight*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Catalog* catalog;
    Sight* sight;
    //this long represents the position in the list (this->GetParent())->listcontrol_sights of sight. If list_position = -1, then sight is not in that list
    long list_position;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    PrintErrorMessage<SightFrame>* printerrormessage;
    
    BodyField* body;
    LimbField* limb;
    CheckField<LengthField>* artificial_horizon_check;
    CheckField<ChronoField>* stopwatch_check;
    AngleField<SightFrame>* H_s, *index_error;
    LengthField* height_of_eye;
    DateField *master_clock_date;
    ChronoField *master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    StringField<SightFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_time, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    wxStaticBoxSizer *sizer_box_measurement, *sizer_box_time;
    
    wxArrayString bodies, limbs;
    wxButton* button_reduce, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void TryToEnableReduce(void);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

class PositionFrame: public wxFrame{
    
public:
    PositionFrame(ListFrame*, Position*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Position* position;
    //this long represents the position in the list (this->GetParent())->listcontrol_positions of position. If list_position = -1, then sight is not in that list
    long list_position;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    PrintErrorMessage<PositionFrame>* printerrormessage;
    
    AngleField<PositionFrame>* lat, *lon;
    StringField<PositionFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2;
    wxStaticBoxSizer *sizer_box_measurement;
    
    wxButton* button_add, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressAdd(wxCommandEvent& event);
    void TryToEnableReduce(void);
    
    // The Path to the file we have open
    wxString CurrentDocPath;
    
    //    wxDECLARE_EVENT_TABLE();
    
};


class ChartFrame: public wxFrame{
    
public:
    ChartFrame(ListFrame*, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    wxStaticText *text_position_now, *text_slider;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxStaticBitmap* image;
    wxDisplay display;
    wxRect rectangle_display;
    TextBox* box;
    wxSlider* slider;
    PrintErrorMessage<ChartFrame>* print_error_message;
    //this variable is true if the user has started drawing a selection rectangle on image, by right-clicking on image and thus forming one of the corners of the rectangle, and zero otherwise.
    unsigned int /*this stores the value of slider*/value_slider_old;
    vector<double> x, y;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    void GetCoastLineData(void);
    bool UpdateSlider(void);
    void UpdateSliderLabel(void);
    void SetIdling(bool);
    
};

class ChartPanel : public wxPanel{
    
public:
    
    ChartFrame* parent;
    
    ChartPanel(ChartFrame*, const wxPoint&, const wxSize&);
    
};


class OnSelectInListControlSights{
    
public:
    
    //the parent frame
    ListFrame* f;
    
    //the constructor, setting the parent frame
    OnSelectInListControlSights(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};

class OnSelectInListControlPositions{
    
public:
    
    //parent frame
    ListFrame* f;
    
    //constructor, which sets the parent frame
    OnSelectInListControlPositions(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};

OnSelectInListControlSights::OnSelectInListControlSights(ListFrame* f_in){
    
    f = f_in;
    
}

OnSelectInListControlPositions::OnSelectInListControlPositions(ListFrame* f_in){
    
    f = f_in;
    
}

