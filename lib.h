#include "wx/wxprec.h"
#include "wx/checkbox.h"
#include "wx/combobox.h"
#include "wx/dc.h"
#include "wx/display.h"
#include "wx/listctrl.h"
#include "wx/slider.h"
#include "wx/mstream.h"



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
template<class P> class LengthField;
class DateField;
class ChronoField;
class RouteTypeField;
template<class P> class StringField;
class MyApp;
template<class F> class CloseFrame;
template<class F_YES, class F_NO> class QuestionFrame;
class ListFrame;
class SightFrame;
class ChartFrame;
class PlotFrame;
class ChartPanel;
class RouteFrame;
class DrawPanel;
class Position;

//class CheckBody;
//struct CheckLimb;
//template<class T> struct CheckCheck;
//struct CheckChrono;
//template<class P> class CheckAngle;
//template<class P> struct CheckSign;
//template<class P> struct CheckArcDegree;
//template<class P> struct CheckArcMinute;
//struct CheckLengthValue;
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


static const int days_per_month_leap_temp[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_leap(days_per_month_leap_temp, days_per_month_leap_temp + sizeof(days_per_month_leap_temp)/sizeof(days_per_month_leap_temp[0]));

static const int days_per_month_common_temp[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_common(days_per_month_common_temp, days_per_month_common_temp + sizeof(days_per_month_common_temp)/sizeof(days_per_month_common_temp[0]));


#define path_file_utc_date_and_time "utc.txt"
#define path_file_init "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/init.txt"
#define path_file_recent "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/recent.txt"
#define path_file_catalog "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/catalog.txt"
#define path_file_temp "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/temp.txt"
//these are the color codes in kml file format for a few populat colors (red, etc...);
string kml_colors[] = {"ff0000ff", "ffff0000", "ff336699", "ff00ff00", "ff0080ff", "ffff00ff"};
string hex_colors[] = {"#000000", "#0000FF", "#00FF00", "#663300", "#3399FF", "#0000CC"};

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

//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal





inline double cot(double x){
    
    return 1.0/tan(x);
    
}

inline double csc(double x){
    
    return 1.0/sin(x);
    
}

inline double atan(double x, double y){
    
    if(x != 0.0){
        
        if(x > 0.0){
            
            return atan(y/x);
            
        }else{
            
            return(atan(y/x)+M_PI);
        }
        
    }else{
        
        if(y > 0.0){
            
            return(M_PI/2.0);
            
        }else{
            
            return(3.0*M_PI/2.0);
            
        }
        
    }
    
}

//the floor of the exponential of x
inline int floor_exp(double x){
    
    return(floor(exp(x)));
    
}

//given a wxListCtrl* list_control, it finds on what element of it the mouse is currently on and writes the id of this element in i
void MousePositionOnListControl(wxListCtrl* list_control, int* i){
    
    wxPoint p;
    wxRect r;
    int hit_test_flag;
    
    hit_test_flag = wxLIST_HITTEST_ONITEM;
    
    p = (list_control->ScreenToClient(wxGetMousePosition()));
    
    list_control->GetItemRect(0, r, wxLIST_RECT_BOUNDS);
    (p.y) -= r.y;
    
    (*i) = (list_control->HitTest(p, hit_test_flag));
    
    //    cout << "\nMouse is on item # " << (*i);
    
}

class String{
    
public:
    string value;
    
    String();
    String(string);
    void enter(String, String);
    void print(String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    void read_from_file(String, String, String);
    void write_to_file(String, File&, String);
    void set(String, String, String);
    String append(String);
    String prepend(String);
    
    bool operator==(const String&), operator!=(const String&);
    
};

//this is a wxFrame designed to show a message to the GUI user. FF_OK is the type of the functor struct which will be called when the button ok is pressed. This type is variable, so it has been 'templated'
template<typename FF_OK> class MessageFrame: public wxFrame{
    
public:
    MessageFrame(wxWindow*, FF_OK*, const wxString&, const wxString&, const wxPoint&, const wxSize&, String);
    
    wxPanel *panel;
    wxBoxSizer *sizer_h, *sizer_v, *sizer_buttons;
    wxGridSizer* sizer_grid;
    wxButton* button_ok;
    wxStaticBitmap* image;
    //initialize the functor to close this MessageFrame when button_ok will be pressed
    CloseFrame<MessageFrame>* close_frame;
  
    
    //pointer to the struct containing the functor which will be called when the button ok is pressed
    FF_OK* f_ok;
    
    //    void OnPressOk(wxCommandEvent&);

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
    //initialize the functor to close thie QuestionFrame when button_yes or button_no will be pressed
    CloseFrame<QuestionFrame>* close_frame;
    //pointer to the struct containing the functor which will be called when the button yes is pressed
    F_YES* f_yes;
    //pointer to the struct containing the functor which will be called when the button no is pressed
    F_NO* f_no;

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
    void read_from_file(String, String, String);
    void enter(String, String);
    void set(String, int, String);
    void print(String, String, ostream&);
    
    bool operator==(const Int&), operator!=(const Int&);
    
};





class Double{
    
public:
    double value;
    
    void read_from_file(String, File&, bool, String);
    void read_from_file(String, String, String);
    void set(String, double, String);
    void print(String, String, ostream&);
    Double operator+(const Double&);
    
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
    void read_from_file(String, String, String);
    void to_deg_min_string(stringstream, stringstream);
    string to_string(String, unsigned int, bool);
    string deg_to_string(String, unsigned int);
    string min_to_string(String, unsigned int);

    Angle();
    Angle(String, double, String);
    bool operator == (const Angle&), operator > (const Angle&), operator < (const Angle&);
    Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
    
};

//this class denotes a rigid rotation R_z(c).R_x(-b).R_z(a) with Euler angles a, b, c
class Rotation{
  
public:
    
    //R[3*i+j] is the element of the j-th column, i-th row of the rotation matrix of this
    gsl_matrix* matrix;
    
    Rotation();
    Rotation(Angle, Angle, Angle);
    Rotation operator *(const Rotation&);
    Rotation inverse(void);
    void set(Angle, Angle, Angle);
    void print(String, String, ostream&);
    void read_from_file(String, String, String);
    
};



//this function returns the rectangular x value of the spherical Mercator projection from the longitude lambda (expressed in degrees, positive towards W).
inline double x_mercator(double lambda){
    
    //construct lambda_temp and set it to lambda, in order to report lambda in the interval [0, 2pi)
    Angle lambda_temp;
    
    lambda_temp.set(String(""), lambda*k, String(""));
    
    //return x by taking into account the periodicity of the projection
    return( -((lambda_temp.value) - floor((lambda_temp.value)/M_PI)*2.0*M_PI) );
    
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
    
    return (-x*K - 360.0*floor((x-(-M_PI))/(2.0*M_PI)));
    
}

//this function returns the latitude value (expressed in degrees) of the inverse spherical Mercator projection from the rectangular y value
inline double phi_mercator(double y){
    
    return(K*atan(sinh(y)));
    
}




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
    void set(String, gsl_vector*, String);
    void modify(String);
    void print(String, String, ostream&);
    void read_from_file(File&, String);
    //this function transports the position and returns the Route with which it has been transported
    Route transport(String);
    string to_string(unsigned int);
    bool distance(Position, Length*, String, String);
    bool operator==(const Position&);
    bool operator!=(const Position&);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    void rotate(String, Rotation, Position*, String);
    
};



//if type = l or o, the parameters specifying the route are reference_position (which in this case is the start position of the Route), alpha, l. if type = c, the parameters specifying the route are reference_position (which in this case is the GP of the circle of equal altitude) and omega.
class Route{
    
public:
    String type, label, temp_prefix;
    //if type = l or o -> reference_position = start position, if type = c -> reference_position = ground position
    Position reference_position, end;
    //alpha: the angle that the vector tangent to the route describes with the local meridian at start; omega: the aperture angle of the cone for circles of equal altitude
    Angle alpha, omega;
    //the length of the route
    Length l;
    Speed sog;
    //this is the position in sight_list of the sight linked to route. If there is no sight linked to route, then related_sight = -1.
    Int related_sight;
    //a list of points containing the geo coordinates of points on the Route this
    Position* points;
    
    Route(void);
    void enter(String, String);
    void print(String, String, ostream&);
    void read_from_file(File&, String);
    void compute_end(String);
    bool crossing(Route, vector<Position>*, double*, String);
    void transport(String);
    static double lambda_minus_pi(double, void*);
    bool t_crossing(Route, vector<Angle>*, String);
    bool closest_point_to(Position*, Angle*, Position, String);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    void draw(unsigned int, int, int, DrawPanel*);
    bool lambda_min_max(Angle*, Angle*, String);

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
    void update_wxListCtrl(long, wxListCtrl*);

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
    Int n_points_plot_coastline, n_points_routes, /*the preferred numer of ticks per axis in the chart plot: if there is enough space, I will put this number of ticks. If not, I will plot a nuber of ticks as cloase as possible to the preferred one*/n_intervals_ticks_preferred;
    
    Plot(Catalog*, String);
    //~Plot();
    bool add_sight_and_reduce(Sight*, String);
    bool modify_sight(unsigned int, String);
    void transport_route(unsigned int, String);
    void add_position(String);
    void add_route(Route*, String);
    void remove_sight(unsigned int, Answer, String);
    void transport_position(unsigned int, String);
    void remove_position(unsigned int, String);
    void remove_route(unsigned int, Answer, String);
    bool read_from_file(File&, String);
    void print(bool, String, ostream&);
    void print_to_kml(String);
    void print_sights(String, ostream&);
    void print_positions(String, ostream&);
    void print_routes(bool, String, ostream&);
    //    void show(bool, String);
    void menu(String);
    void compute_crossings(String);
    
};

//this class denotes a point in a 2D projection of the sphere
class Projection{
    
public:
    //the x, y coordinates of the projected point.
    double x, y;
    
    Projection();
    Projection(const double, const double);
    Projection operator+(const Projection&), operator-(const Projection&);

    
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

template<class P> struct CheckLengthValue{
    
    LengthField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

template<class P> struct CheckLengthUnit{
    
    LengthField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

template<class P> class CheckLength{
    
public:
    
    //p is the LengthField which is parent of the CheckLength object: the CheckLength object checks the validity of the entries in LengthField
    LengthField<P>* p;
    CheckLengthValue<P> check_length_value;
    CheckLengthUnit<P> check_length_unit;
    
    CheckLength(LengthField<P>*);
    
    template <class T> void operator()(T&);
    
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

//this checks if a Route type is valid
class CheckRouteType{
    
public:
    
    //the RouteType  field containing the Route type
    RouteTypeField* p;
    
    CheckRouteType(RouteTypeField*);
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

//this functor sets idling -> false in the parent of parent
template<class P> class UnsetIdling{
    
public:
    
    UnsetIdling(P*);
    
    //the frame which called this struct
    P* parent;

    void operator()(wxCommandEvent&);
    
};


//this class defines the functor () used to ...
template<class P> class FunctionOnPressOk{
    
public:
    
    FunctionOnPressOk(P*);
    
    //the frame which called this struct
    P* parent;
     
    void operator()(wxCommandEvent&);
    
};

//this class defines the functor () used to remove a route from the non-GUI object plot
class DeleteRoute{
    
public:
    
    DeleteRoute(ListFrame*, Answer);
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the route to be removed
    long i_route_to_remove;
    //this is equal to 'y' if the sight related to the removed route has to be removed too, and 'n' otherwise
    Answer remove_related_sight;
    
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

//this functor pops out an error-message window with title tile and error message message, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this struct adaptable
template<class T, typename FF_OK> class PrintErrorMessage{
    
public:
    
    T* f;
    wxControl* control;
    String title, message;
    FF_OK* f_ok;
    MessageFrame<FF_OK>* message_frame;
    
//    FunctionOnPressOk<T> * function_on_press_ok;
    
    PrintErrorMessage(T*, FF_OK*);
    
    void operator()(void);
    
    
};

//my own derived class of wxListCtrl
class ListControl : public wxListCtrl{
  
public:
    
    ListControl(wxWindow*, const wxPoint&, const wxSize&);
    
    void PushBackColumn(wxString);
    
};




class DrawPanel : public wxPanel{
    
public:
    
    DrawPanel(ChartPanel*);
    ChartFrame* parent;
    PrintErrorMessage<DrawPanel, UnsetIdling<DrawPanel> >* print_error_message;
    XYChart *chart;
    //a ChartDirector layer to draw splines on chart
    SplineLayer *spline_layer;
    wxPoint position_draw_panel, position_plot_area, position_start_selection, position_end_selection, /*the instantaneous positions of the mouse with respect to the screen/draw-panel origin*/position_screen_now, position_start_drag, position_end_drag, position_now_drag, position_draw_panel_now;
    /*x_min, x_max, y_min, y_max do correspond to lambda_min, lambda_max, etc... They are ordered in such a way that x_min <= x_max and y_min <= y_max always. */
    double /*min and max values of the Mercator or 3D projections x, y*/x_min, x_max, y_min, y_max, x_min_start_drag, x_max_start_drag, y_min_start_drag, y_max_start_drag,
    /*these are the values of x_min .. y_max and x_span when the plot is first drawn*/
    x_min_0, x_max_0, y_min_0, y_max_0, x_span_0,
    /*the coordinates of the center of the projection when scrolling starts*/ x_center_scrolling, y_center_scrolling,
    /*these are the values of x_min, ... y_max after each sliding event, corresponding to lambda_min, ... , phi_max read from file*/x_min_old, x_max_old, y_min_old, y_max_old, /*this is the ratio between the length of the ticks on both axes, and the width of the plot area*/tic_length_over_width_plot_area, /* gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes when setting the ticks on the x axis, and similarly for gamma_phi*/gamma_lambda, gamma_phi, /*these are the angular separations in latitude and longitude between meridians and parallels, respectively */delta_lambda, delta_phi;
    //the euler angles which specify the orientation of the earth for the 3d representation
    //two auxiliary vectors which will be used later
    gsl_vector *r, /*vector position in the x'y'z' reference frame used for multiple purposes*/*rp, /*vector position in the x'y'z' reference frame at the beginning, end and current time of mouse drag*/*rp_start_drag, *rp_end_drag, *rp_now_drag;
    Rotation /*the orientation of the Earth at the beginning / current time / end of a drag*/rotation_start_drag, rotation_now_drag, rotation_end_drag, /*the rotation representing the current / initial orientation of the earth*/rotation, rotation_0;
    Double /*the distance between the plane of the 2d projection and the eye of the observer for the 3d plot, and its initial value when this is constructed*/d, d_0, /*if the mouse hovers over a route and its y coordinate is equal to the y of the route +- (length sceen) * thickness_route_selection_over_length_screen /2, then the relative Route is highlighted in ListFrame*/thickness_route_selection_over_length_screen;
    wxStaticText*text_position_start, *text_position_end;
    bool selection_rectangle, /*this is true if the mouse is dragging with the left button pressed*/mouse_dragging, idling;
    Position /*these are the positions where the right mouse button is clicked at the beginning and at the end of the drawing process for the selection rectangle on the world's chart*/p_start, p_end, /*I store in this position the starting point (ground position) of a Route if the Route is a loxodrome or orthodrome (circle of equal altitude) that I want to drag, at the beginning of the dragging process*/route_position_start_drag, /*current, starting and ending geographic position in a mouse drag process*/ geo_now_drag, geo_start_drag, geo_end_drag, /*the position on the sphere such that the vector between the center of the sphere and the position equals the direction of the rotation axis relative to a mouse drag*/rotation_axis;
    Angle rotation_angle;
    Projection /*the values of (x, y) at the beginning/end of the selection process with a rectangle*/start_selection, end_selection;
    Route /*this is a circle of equal altitude which is the intersection between the earth's surface and the visibility cone of the observer, whose vertex is at the observer. circle_oberserver.omega is the aperture angle at the center of the earth which specifies the part of the earth surface visible by the observer. circle_observer.rerefence_position is the ground position of the observer: the intersection between the line between the center of the earth and the observer, and the surface of the earth. These exist for the 3D projection only*/circle_observer;
    
 
    
    wxSizer* sizer_h, *sizer_v;
    //the i-th element of point_route_list is a list: in this list there are as many elements as the number of connected curves in which the route is cut (because of the meridian lambda = pi). In each of these elements there are the  points, with respect to the origin of DrawPanel, which represent the route_list[i] discretized
    vector< vector< vector< wxPoint > > > points_route_list;
    //the i-th element of this vector contains a list of critical values of the parametric angle (t) of the i-th route. At these critical values, route #i crosses the meridian lambda = pi
    //the chart contains the plot area, and the following quantities are the width and height of chart and plot area
    unsigned int width_chart, height_chart, /*these are the values of width/height_chart when the chart is first drawn*/width_chart_0, height_chart_0, width_plot_area, height_plot_area, tic_length;
    Plot* plot;
    MemBlock mem_block;
    wxMemoryInputStream * memory_input_stream;
    wxBitmap* bitmap_image; 
    UnsetIdling<DrawPanel>* unset_idling;

 
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither DrawPanel::Draw_Mercator or DrawPanel::Draw_3D, according to my needs, and similarly for the other pointers
    void (DrawPanel::*Draw)(void);
    void (DrawPanel::*TabulateRoutes)(void);
    bool (DrawPanel::*ScreenToGeo)(wxPoint, Position*);
    bool (DrawPanel::*GeoToProjection)(Position, Projection*);
    void (DrawPanel::*Render)(wxDC&);
    bool (DrawPanel::*GeoToDrawPanel)(Position, wxPoint*);
    void (DrawPanel::*Set_x_y_min_max)(void);

    
    void SetIdling(bool);
    void Draw_Mercator(void);
    void Draw_3D(void);
    void TabulateRoutes_Mercator(void);
    void TabulateRoutes_3D(void);
    void PaintEvent(wxPaintEvent & evt);
    void PaintNow();
    
    void GeoToScreen(Position, wxPoint*);
    bool DrawPanelToGeo(wxPoint, Position*);
    bool ScreenToMercator(wxPoint, Projection*);
    bool GeoTo3D(Position, Projection*);
    bool GeoToMercator(Position, Projection*);
    bool ScreenToGeo_Mercator(wxPoint, Position*);
    bool ScreenToGeo_3D(wxPoint, Position*);
    bool GeoToDrawPanel_Mercator(Position, wxPoint*);
    bool GeoToDrawPanel_3D(Position, wxPoint*);
    void Update_lambda_phi_min_max(void);
    void Set_x_y_min_max_Mercator(void);
    void Set_x_y_min_max_3D(void);
    bool check(const Projection);
    bool check_x(const Projection);
    double x_span(void);
    Rotation rotation_start_end(wxPoint, wxPoint);
    
    void Render_Mercator(wxDC&);
    void Render_3D(wxDC&);

    bool GetMouseGeoPosition(Position*);
    void OnMouseMovement(wxMouseEvent&);
    void OnMouseLeftDown(wxMouseEvent&);
    void OnMouseLeftUpOnDrawPanel(wxMouseEvent&);
    void OnMouseRightDown(wxMouseEvent&);
    void OnMouseDrag(wxMouseEvent&);
    void OnChooseProjection(wxCommandEvent&);

    
    void ArrowDown(wxKeyEvent&);
    
    
    DECLARE_EVENT_TABLE()
};

class ProjectionField{
    
public:
    ChartFrame* parent;
    
    wxBoxSizer *sizer_h, *sizer_v;
    wxArrayString  /*this is equal to a standard list of the available graphical types*/types;

    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;

    ProjectionField(ChartFrame*);
    template<class T> void InsertIn(T*);
    
};

class BodyField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString  /*this is equal to a standard list of the available bodies, except that on top of the list are the recently selected bodies*/bodies;
    //this points to a Body object, which contains the date written in the GUI field of this
    Body* body;
    Catalog* catalog;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    CheckBody* check;
    File file_recent;
    vector<int> recent_items;
    
    bool ok;
    
    BodyField(SightFrame*, Body*, Catalog*);
    void read_recent_items(void);
    void write_recent_items(void);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};


class LimbField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    wxArrayString limbs;
    //this points to a Limn object, which contains the data written in the GUI field of this
    Limb* limb;
    wxBoxSizer *sizer_h, *sizer_v;
    bool ok;
    CheckLimb check;
    
     
    LimbField(SightFrame*, Limb*);
    void set(void);
    void Enable(bool);
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
    void Enable(bool);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a length, composed of a box. P is the type of the parent which hosts the LengthField object
template<class P> class LengthField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    //degrees and minutes boxes
    wxTextCtrl *value;
    //units of measure of the length
    wxComboBox* box_unit;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;
    //an array containing all possible units of measure
    wxArrayString units;
    //the units of measure of the length in this GUI field
    String unit;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool value_ok, box_unit_ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    CheckLength<P>* check;
    
    LengthField(P*, Length*, String);
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

class RouteTypeField{
    
public:
    //the parent frame to which this object is attached
    RouteFrame* parent_frame;
    wxArrayString types;
    //this points to a non-GUI String object, which contains the Route type written in the GUI object this
    String* type;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    CheckRouteType* check;
    
    bool ok;
    
    RouteTypeField(RouteFrame*, String*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};






//a class for color objects
class Color: public wxColour{
    
public:
    Color();
    Color(unsigned char, unsigned char, unsigned char);
    Color(unsigned char, unsigned char, unsigned char, unsigned char);
    
    void read_from_file(String, String, String);

};



//this is a wxFrame designed to contain the list of sights, routes, etc...
class ListFrame: public wxFrame{
    
public:
    //this frame has no parent, because it is supposed to be the main frame of the appplication
    ListFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix);
    
    //this is a pointer to the non-GUI object Plot which is related to the GUI object this
    Plot* plot;
    //point to the child frame of this
    vector<ChartFrame*> chart_frames;
    //this is a pointer to a Catalog object which will be used by plot
    Catalog *catalog;
    //    wxListBox* listbox;
    wxMenuBar *menu_bar;
    wxMenu *menu_chart, *menu_new_chart, *menu_item_mercator, *menu_item_3d;
    ListControl *listcontrol_sights, *listcontrol_routes, *listcontrol_positions;
    wxPanel *panel;
    wxButton *button_add_sight, *button_delete_sight, *button_add_position, *button_delete_position, *button_add_route, *button_delete_route;
    wxBitmapButton *button_modify_sight, *button_modify_position, *button_modify_route;
    wxSizer* sizer_h, *sizer_v, *sizer_buttons_sight, *sizer_buttons_position, *sizer_buttons_route;
    wxStaticBoxSizer* sizer_box_sight, *sizer_box_position, *sizer_box_route;
    DeleteSight *delete_sight, *delete_sight_and_related_route;
    DeleteRoute *delete_route, *delete_route_and_related_sight;
    int /*the # of the sight/route/position which is highlighted because the mouse is hovering over it in listcontrol_sights/routes/positions*/highlighted_sight, highlighted_route, highlighted_position;
    //data_x[i][j] is a vector which contains the (x-value of) the datapoints within the block at (shifted) latitude i and longitude j in file path_file_coastline_data_blocked
    vector< vector< vector<float> > > data_x, data_y;
    vector< vector< vector<Position> > > data_3d;
    //a vector which contains the Positions of the coastline profiles of the entire world
//    vector<Position> data_3d;
    vector<Color> color_list;
    //the color with which an item is highlighted in ListFrame, when mouse hovers over the related route or position in the plot area
    Color color_selected_item;
    //the rectangle used to measure the size of the display
    wxDisplay display;
    wxRect rectangle_display;
    //this referrs to the init file
    File file_init;
    
    void GetAllCoastLineData(void);
    void UpdateRelatedSightsAndRoutes(void);
    
    void OnAddSight(wxCommandEvent& event);
    void OnModifySight(wxCommandEvent& event);
    void OnPressDeleteSight(wxCommandEvent& event);
    
    void OnAddPosition(wxCommandEvent& event);
    void OnModifyPosition(wxCommandEvent& event);
    void OnDeletePosition(wxCommandEvent& event);

    void OnAddRoute(wxCommandEvent& event);
    void OnModifyRoute(wxCommandEvent& event);
    void OnPressDeleteRoute(wxCommandEvent& event);

    void OnAddChartFrame(wxCommandEvent& event);

    void OnMouseOnListControlSights(wxMouseEvent&);
    void OnMouseMovement(wxMouseEvent&);
    void OnMouseOnListControlPositions(wxMouseEvent&);
    
    void DrawRoutes(void);

    
};




class SightFrame: public wxFrame{
    
public:
    SightFrame(ListFrame*, Sight*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Catalog* catalog;
    Sight* sight;
    //this long represents the position in the list (this->GetParent())->listcontrol_sights of sight. If list_position = -1, then sight is not in that list
    long position_in_listcontrol_sights;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    UnsetIdling<SightFrame>* unset_idling;
    PrintErrorMessage<SightFrame, UnsetIdling<SightFrame> >* print_error_message;
    
    BodyField* body;
    LimbField* limb;
    CheckField< LengthField<SightFrame> >* artificial_horizon_check;
    CheckField<ChronoField>* stopwatch_check;
    AngleField<SightFrame>* H_s, *index_error;
    LengthField<SightFrame>* height_of_eye;
    DateField *master_clock_date;
    ChronoField *master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    StringField<SightFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_time, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    wxStaticBoxSizer *sizer_box_measurement, *sizer_box_time;
    
    wxButton* button_reduce, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void AllOk(void);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

class PositionFrame: public wxFrame{
    
public:
    PositionFrame(ListFrame*, Position*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Position* position;
    //this long represents the position in the list (this->GetParent())->listcontrol_positions of position. If list_position = -1, then sight is not in that list
    long position_in_listcontrol_positions;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    
    
    //these are the functors needed to check whether GUI fields, such as arcdegrees and arcminutes,  are entered  correctly
    UnsetIdling<PositionFrame>* unset_idling;
    PrintErrorMessage<PositionFrame, UnsetIdling<PositionFrame> >* print_error_message;
    
    AngleField<PositionFrame>* lat, *lon;
    StringField<PositionFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2;
    wxStaticBoxSizer *sizer_box_measurement;
    
    wxButton* /*button_ok denotes a button which triggers either an 'Add' command, when one is adding a new position, or a 'Modify' command, when one is modifying a position whih already exists*/button_ok, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    void AllOk(void);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

class RouteFrame: public wxFrame{
    
public:
    RouteFrame(ListFrame*, Route*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Route* route;
    //this long represents the position in the list (this->GetParent())->listcontrol_routes of route. If list_position = -1, then route is not in that list
    long position_in_listcontrol_routes;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether GUI fields, such as arcdegrees and arcminutes,  are entered  correctly
    UnsetIdling<RouteFrame> * unset_idling;
    PrintErrorMessage<RouteFrame, UnsetIdling<RouteFrame> >* print_error_message;
    
    RouteTypeField *type;
    AngleField<RouteFrame> *alpha, *omega, *start_phi, *start_lambda, *GP_phi, *GP_lambda;
    LengthField<RouteFrame> *l;
    StringField<RouteFrame> *label;
    
    wxFlexGridSizer *sizer_grid_type, *sizer_grid_alpha, *sizer_grid_l,  *sizer_grid_omega, *sizer_grid_start, *sizer_grid_GP, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer;
    wxStaticBoxSizer *sizer_box_data, *sizer_box_start, *sizer_box_GP;
    
    wxButton* /*this button triggers either the addition of a new Route, or the modification of an existing one*/button_ok, *button_cancel;
//    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    void AllOk(void);
    
    //    wxDECLARE_EVENT_TABLE();
    
};



class ChartFrame: public wxFrame{
    
public:
    ChartFrame(ListFrame*, String, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    wxStaticText *text_position_now, *text_slider;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxGridSizer* sizer_buttons;
    wxStaticBitmap* image;
    TextBox* box;
    wxSlider* slider;
    wxButton* button_up, *button_down, *button_left, *button_right, *button_reset;
    //the color of the horizon circle for the 3D projection
    wxColor color_horizon;
    ProjectionField* projection;
    PrintErrorMessage<ChartFrame, UnsetIdling<ChartFrame> >* print_error_message;
    //in these vectors, I store the 2d coordindates of the Mercator projection and of the 3D rendering, respectively
    vector<double> x, y, x_3d, y_3d;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling, /*this is true if the user is currently scrolling*/mouse_scrolling;
    //This is the actual value of the maximal zoom factor allowed
    Int zoom_factor_max;
    
    Double  /*the displacemenet over the x (y) axis divided by the length of the x (y) axis used by the functions MoveUp, ..., MoveRight*/ relative_displacement, /*the default thickness of the line with which routes are drawn*/ standard_thickness_over_length_screen, /*the thickness with which highlighted routes are drawn*/ large_thickness_over_length_screen, /*the zoom factor relative to the default configuration of either projection, the zoom factor  is not necessarily equal to the numerical value (slider->GetValue()) shown on the slider*/zoom_factor;
    
    
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither ChartFrame::ZoomFactor_Mercator or ChartFrame::ZoomFactor_3D, according to my needs, and similarly for the other pointers
//    bool (ChartFrame::*ZoomFactor)(double);
 
    void GetCoastLineData_Mercator(void);
    void GetCoastLineData_3D(void);
    void UpdateSlider(void);
    bool ZoomFactor_Mercator(double);
    bool ZoomFactor_3D(void);
    void UpdateSliderLabel(void);
    void OnMouseLeftDownOnSlider(wxMouseEvent&);
    void OnMouseLeftUpOnSlider(wxMouseEvent&);
    void OnScroll(wxScrollEvent&);
    void OnClose(wxCloseEvent&);
    void SetIdling(bool);
    void AllOk(void);
    
    template<class T> void MoveUp(T&);
    template<class T> void MoveDown(T&);
    template<class T> void MoveLeft(T&);
    template<class T> void MoveRight(T&);
    template<class T> void Reset(T&);
    
    //this is created for testing
    UnsetIdling<ChartFrame> *unset_idling;
    


    
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

class OnSelectInListControlRoutes{
    
public:
    
    //parent frame
    ListFrame* f;
    
    //constructor, which sets the parent frame
    OnSelectInListControlRoutes(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};
