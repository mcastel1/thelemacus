#include "wx/wxprec.h"
#include "wx/checkbox.h"
#include "wx/combobox.h"
#include "wx/dc.h"
#include "wx/display.h"
#include "wx/listctrl.h"
#include "wx/slider.h"
#include "wx/mstream.h"
#include "wx/timer.h"
#include "wx/datetime.h"
#include "wx/settings.h"
#include "wx/kbdstate.h"
#include "wx/wfstream.h"
#include "wx/graphics.h"
#include "wx/dcgraph.h"
#include "wx/progdlg.h"





#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//#include "chartdir.h"


template<class P> class BodyField;
template<class P> class ProjectionField;
template<class P> class LimbField;
template<class P, class T> class CheckField;
template<class P> class AngleField;
template<class P> class LengthField;
template<class P> class DateField;
template<class P> class ChronoField;
class RouteTypeField;
template<class P> class StringField;
class MyApp;
template<class F> class CloseFrame;
template<class F> class ShowFrame;
class ShowAll;
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
class Data;
class File;
class FileRW;
class FileR;
class Time;
class Date;
class Chrono;
class Route;
class Sight;
class Atmosphere;
class Answer;
class Body;
class String;
template<class P> class CheckProjection;
template<class P> class CheckArcDegree;
template<class P> class CheckArcMinute;
template<class P> class TabulateDays;
template<typename FF_OK> class MessageFrame;


//a class for color objects
class Color: public wxColour{
    
public:
    Color();
    Color(wxColour);
    Color(unsigned char, unsigned char, unsigned char);
    Color(unsigned char, unsigned char, unsigned char, unsigned char);
    
    void read_from_file(String, String, String, String);
    int ToRGB(void);
    
};

//a class for Bitmap images
class Bitmap: public wxBitmap{
    
public:
    Bitmap();
    Bitmap(String, wxSize);
    
};

//a class for image objects
class StaticBitmap: public wxStaticBitmap{
    
public:
    StaticBitmap(wxWindow*, String, wxSize);
    
};

class String{
    
public:
    string value;
    
    String();
    String(string);
    String(char);
    void print(String, bool, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void write_to_file(String, FileRW&, String);
    void set(String, String, String);
    void set(String);
    void set_to_current_time(void);
    String append(String);
    void appendto(String);
    String prepend(String);
    bool split_file_path(String*, String*, String*, String);
    String filename_without_folder_nor_extension(String);
    bool is_a_file_path(String);
    wxSize get_size(wxWindow* p);
    
    bool operator==(const String&), operator!=(const String&);
    
};


class Int{
    
public:
    int value;
    
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void set(String, int, String);
    void print(String, String, ostream&);
    
    bool operator==(const Int&), operator!=(const Int&), operator>(const int&);
    
};

class Chrono{
    
public:
    unsigned int h, m;
    double s;
    
    void print(String, String, ostream&);
    bool set(String, double, String);
    void set_current(void);
    template<class S> bool read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    string to_string(unsigned int, bool);
    
    bool operator==(const Chrono&), operator!=(const Chrono&), operator<(const Chrono&), operator>(const Chrono&);
    
};



class Double{
    
public:
    double value;
    
    bool equal_approx(Double);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void set(String, double, String);
    void print(String, String, ostream&);
    Double operator+(const Double&);
    
};


class Angle{
    
public:
    
    double value;
    
    Angle();
    Angle(double);
    Angle(unsigned int, double);
    Angle(String, double, String);
    void normalize(void);
    Angle normalize_ret(void);
    void normalize_pm_pi(void);
    Angle normalize_pm_pi_ret(void);
    Angle span(Angle);
    void enter(String, String);
    void set(String, double, String);
    void print(String, String, ostream&);
    void to_deg_min(unsigned int*, double*);
    void from_sign_deg_min(char, unsigned int, double);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void to_deg_min_string(stringstream, stringstream);
    string to_string(String, unsigned int, bool);
    string deg_to_string(String, unsigned int);
    string min_to_string(String, unsigned int);
    
    bool operator == (const Angle&), operator == (const double&), operator != (const double&), operator > (const Angle&), operator > (const double&);
    Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&), operator * (const double&);
    Angle& operator +=(const Angle&), &operator +=(const double&), &operator -=(const Angle&), &operator -=(const double&);
    
};



#include "constants.h"


inline double cot(double x){
    
    return 1.0/tan(x);
    
}

inline double csc(double x){
    
    return 1.0/sin(x);
    
}

//returns alpha = arctan(y/x), with alpha lying in the same quadrant as the vector x, y
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

//given a wxListCtrl* list_control, if * list_control has at least one item, it finds on what element of *list_control the mouse is currently on and writes the id of this element in i. Otherwise, it writes wxNOT_FOUND in *i
void MousePositionOnListControl(wxListCtrl* list_control, int* i){
    
    if((list_control->GetItemCount()) > 0){
        //*list_control has a non-zero size -> write in *i the number of the item on which the mouse is hovering
        
        wxPoint p;
        wxRect r;
        int hit_test_flag;
        
        hit_test_flag = wxLIST_HITTEST_ONITEM;
        
        p = (list_control->ScreenToClient(wxGetMousePosition()));
        
        //obtain the position of the rectangle of the first ivisible item in *this and store it in r
        list_control->GetItemRect(list_control->GetTopItem(), r, wxLIST_RECT_BOUNDS);
        
        //decrease the mouse position with respect to the origin of *this located on the bottom edge of the gray band on the top by r.y on the y axis -> now p is the mouse position with respect to the top-left origin of the white area of *this
        (p.y) -= r.y;
        
        //store in i the # of the item on which the mouse is hovering
        (*i) = ((int)(list_control->HitTest(p, hit_test_flag)));
        
        //    cout << "\nMouse is on item # " << (*i);
        
    }else{
        //*list_control has zero size -> write in *t wxNOT_FOUND
        
        (*i) = wxNOT_FOUND;
        
    }
    
}



//this is a wxFrame designed to show a message to the GUI user. FF_OK is the type of the functor class which will be called when the button ok is pressed. This type is variable, so it has been 'templated'
template<class FF_OK> class MessageFrame: public wxFrame{
    
public:
    
    wxPanel *panel;
    wxBoxSizer *frame_sizer, *sizer_v;
    wxGridSizer* sizer_grid;
    wxButton* button_ok;
    StaticBitmap* image;
    //    wxBitmap* m_bitmap;
    //pointer to the class containing the functor which will be called when the button ok is pressed
    FF_OK* f_ok;
    
    MessageFrame(wxWindow*, FF_OK*, const wxString&, const wxString&, String, const wxPoint&, const wxSize&, String);
    //initialize the functor to close this MessageFrame when button_ok will be pressed
    CloseFrame<MessageFrame>* close_frame;
    void KeyDown(wxKeyEvent&);
    //    void OnPaint(wxPaintEvent&);
    //    void OnPressOk(wxCommandEvent&);
    
    //    DECLARE_EVENT_TABLE();
    
};


//this is a wxFrame designed to ask a  yes/no question to the GUI user. F_A is the type of the functor class which will be called when the button yes is pressed. This type is variables, so it has been 'templated'. Same for F_B. If the user presses enter (esc), f_a  (f_b) are called
template<typename F_A, typename F_B> class QuestionFrame: public wxFrame{
    
public:
    QuestionFrame(wxWindow*, F_A*, String, F_B*, String, const wxString&, const wxString&,  String, const wxPoint&, const wxSize&, String);
    
    //the non-GUI object connected to the GUI object MessageFrame
    wxPanel *panel;
    wxBoxSizer *sizer_v, *sizer_buttons;
    wxGridSizer* sizer_grid;
    wxButton* button_a, *button_b;
    StaticBitmap* image;
    //initialize the functor to close thie QuestionFrame when button_a or button_b will be pressed
    CloseFrame<QuestionFrame>* close_frame;
    //pointer to the class containing the functor which will be called when the button yes is pressed
    F_A* f_a;
    //pointer to the class containing the functor which will be called when the button no is pressed
    F_B* f_b;
    String string_a, string_b;
    
    template<class E> void KeyDown(E&);
    
};


//this function checks whether the  unsigned int in string s is formatted correctly and, if check_interval = true, it also checks whether the this unsigned int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_unsigned_int(string s, unsigned int* i, bool check_interval, unsigned int min, unsigned int sup){
    
    bool check;
    unsigned int j = 0;
    
    check = false;
    
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
    int j = 0;
    
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
    double y = 0.0;
    
    check = false;
    
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

//a general class for File objects
class File{
    
public:
    
    String  /*the name of the file without the folder path (before it) and without the file extension (after it)*/name_without_folder_nor_extension;
    unsigned int number_of_lines;
    
    File();
    
};

//a inherited class from File class, for files on disk that can be read and written to
class FileRW: public File{
    
public:
    
    //the strean for reading and writing
    fstream* value;
    String /*the full path of the file, including the folder, filename and extension*/name, /*the path of the folder where the file is located*/folder, /*the file extension, without the '.'*/extension;
    
    
    
    FileRW();
    void set_name(String);
    bool open(String, String);
    void close(String);
    void remove(String);
    void count_lines(String);
    bool check_if_exists(String);
    
    
    
    //    wxDECLARE_EVENT_TABLE();
    
};

//an inherited class from File class, for files that can be read only
class FileR: public File{
    
public:
    
#ifdef __APPLE__
    //I am on APPLE operating system: the file will be read from a path in the .app package and read with the ifstream value (read only) -> I include also ame, folder and extension as members of this class
    ifstream* value;
    String /*the full path of the file, including the folder, filename and extension*/name, /*the path of the folder where the file is located*/folder, /*the file extension, without the '.'*/extension;
    
#endif
#ifdef _WIN32
    //I am on WIN32 operating system: the file will be loaded from a resource incoporeated in the .exe file and read with the istringstream value
    istringstream* value;
#endif
    
    
    FileR();
    void set_name(String);
    bool open(String);
    void close(String);
    bool check_if_exists(String);
    void count_lines(String);
    //istringstream* create_istringstream(String);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

class Length{
    
public:
    double value;
    
    Length();
    Length(double);
    
    void set(String, double, String);
    void enter(String, String, String);
    void print(String, String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    bool check_valid(String, String);
    string to_string(String, unsigned int);
    bool operator> (const Length&), operator<= (const Length&), operator> (const double&), operator< (const double&), operator==(const Length&), operator!=(const Length&);
    Length operator + (const Length&), operator - (const Length&);
    Length &operator += (const Length&), &operator -= (const Length&);
    
};


class Speed{
    
public:
    
    double value;
    
    Speed();
    Speed(double);
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
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    bool operator==(const Answer&), operator !=(const Answer&);
    
};


//this class denotes a rigid rotation R_z(c).R_x(-b).R_z(a) with Euler angles a, b, c
class Rotation{
    
public:
    
    //R[3*i+j] is the element of the j-th column, i-th row of the rotation matrix of this
    Angle a, b, c;
    gsl_matrix* matrix;
    
    Rotation();
    Rotation(Angle, Angle, Angle);
    Rotation(Position, Position);
    Rotation operator *(const Rotation&);
    Rotation inverse(void);
    void set(Angle, Angle, Angle);
    void set(gsl_matrix*);
    void print(String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    
    
};

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
    
    Position();
    Position(Angle, Angle);
    void set_cartesian(String, const gsl_vector*, String);
    void get_cartesian(String, gsl_vector*, String);
    void print(String, String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    bool transport(Route, String);
    string to_string(unsigned int);
    bool distance(Position, Length*, String, String);
    bool is_in(Route, String);
    bool operator==(const Position&);
    bool operator!=(const Position&);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    void rotate(String, Rotation, Position*, String);
    
};

//this class defines a 'rectangular' area on the Eearth's surface: it is a rectangle with vertices p_NW, p_SE in the Mercator projection
class MyRectangle{
    
public:
    
    //thw two Positions which constitute the vertices of the rectangle: p is the NW vertex, p_SE is the SE vertex
    Position p_NW, p_SE;
    
    MyRectangle();
    MyRectangle(Position, Position, String);
    bool Contains(Position);
    
};


//if type = l or o, the parameters specifying the route are reference_position (which in this case is the start position of the Route), alpha, l. if type = c, the parameters specifying the route are reference_position (which in this case is the GP of the circle of equal altitude) and omega.
class Route{
    
public:
    String type, label, temp_prefix;
    //if type = l or o -> reference_position = start position, if type = c -> reference_position = ground position
    Position reference_position, end;
    //alpha: the angle that the vector tangent to the route describes with the local meridian at start; omega: the aperture angle of the cone for circles of equal altitude
    Angle /*this is equal to alpha_notes: it is the azimuth of the vector tangent to the Route at reference_position*/Z, omega;
    //the length of the route
    Length l;
    Speed sog;
    //this is the position in sight_list of the sight linked to route. If there is no sight linked to route, then related_sight = -1.
    Int related_sight;
    //a list of points containing the geo coordinates of points on the Route this
    Position* points;
    
    Route(void);
    Route(String, Position, Angle, Length);
    Route(String, Position, Angle);
    void print(String, String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    void compute_end(String);
    bool compute_end(Length, String);
    int crossing(Route, vector<Position>*, double*, String);
    static double lambda_minus_pi(double, void*);
    int inclusion(Route, bool, vector<Angle>*, String);
    int inclusion(MyRectangle, bool, vector<Angle>*, String);
    int intersection(Route, bool, vector<Angle>*, String);
    bool closest_point_to(Position*, Angle*, Position, String);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    
    void DrawOld(unsigned int, Color, int, wxDC*, DrawPanel*);
    void DrawOld(unsigned int, wxDC*, DrawPanel*, String);
    void DrawOld(unsigned int, DrawPanel*, vector< vector<wxPoint> >*, String);
    
    void Draw(unsigned int, Color, int, wxDC*, DrawPanel*, String);
    void Draw(unsigned int, wxDC*, DrawPanel*, String);
    void Draw(unsigned int, DrawPanel*, vector< vector<wxPoint> >*, String);
    
    bool lambda_min_max(Angle*, Angle*, String);
    void compute_l_ends(vector<Length>*, bool*, DrawPanel*, String);
    
};

class Date{
    
public:
    unsigned int Y, M, D;
    bool Y_is_leap_year;
    vector<unsigned int> days_per_month;
    
    void print(String, String, ostream&);
    void enter(String, String);
    template<class S> bool read_from_stream(String, S*, bool, String);
    string to_string(void);
    void check_leap_year(void);
    void set_current(void);
    
};


class Time{
    
public:
    Date date;
    Chrono chrono;
    //is s used?
    double s, MJD;
    void enter(String, String);
    void print(String, String, ostream&);
    void set_current(void);
    template<class S> bool read_from_stream(String, S*, bool, String);
    
    void to_MJD(void);
    void to_TAI(void);
    
    string to_string(unsigned int, bool);
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
    template<class S> bool read_from_stream(String, S*, bool, String);
    
    bool operator==(const Body&);
    
    
};

class Limb{
    
public:
    char value;
    void print(String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    
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
    
    void print(String, String, ostream&);
    template<class S> bool read_from_stream(String, S*, bool, String);
    bool reduce(Route*, String);
    bool check_time_interval(String);
    
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    
};

class Catalog{
    
public:
    vector<Body> list;
    Catalog(String, String);
    void add(String, String, double);
    void print(String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    
};


class Data{
    
public:
    Catalog* catalog;
    //    FileR file_init;
    int job_id;
    stringstream command, plot_command;
    vector<Sight> sight_list;
    vector<Position> position_list;
    vector<Route> route_list;
    vector<String> choices;
    vector<Route> crossing_route_list;
    Int n_points_plot_coastline_Mercator, n_points_plot_coastline_3D, n_points_routes, /*the preferred numer of ticks per axis in the chart plot: if there is enough space, I will put this number of ticks. If not, I will plot a nuber of ticks as cloase as possible to the preferred one*/n_intervals_ticks_preferred;
    //the recent bodies selected by the user are stored here
    vector<int> recent_bodies;
    
    Data(Catalog*, String);
    //~Data();
    bool add_sight_and_reduce(Sight*, String);
    void add_route(Route*, String);
    void remove_sight(unsigned int, Answer, String);
    void remove_position(unsigned int, String);
    void remove_route(unsigned int, Answer, String);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    void print(bool, String, ostream&);
    void print_to_kml(String);
    void print_sights(String, ostream&);
    void print_positions(String, ostream&);
    void print_routes(bool, String, ostream&);
    //    void show(bool, String);
    
    void write_recent_bodies(void);
    
    void menu(String);
    int compute_position(String);
    
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

//class which defines a functor which shows a frame with arbitrary type F
template<class F> class ShowFrame{
    
public:
    
    //the frame to be closed
    F* frame;
    ShowFrame(F*);
    
    template<class T> void operator()(T&);
    
};

//class which defines a functor which shows ListFrame and all ChartFrames
class ShowAll{
    
public:
    
    ListFrame* frame;
    ShowAll(ListFrame*);
    
    template<class T> void operator()(T&);
    
};

//class which defines a functor which destroys a frame with arbitrary type F
template<class F> class DestroyFrame{
    
public:
    
    //the frame to be destroyd
    F* frame;
    DestroyFrame(F*);
    
    void operator()(void);
    
};

class ResetListFrame{
    
public:
    
    //the frame to be reset
    ListFrame* p;
    ResetListFrame(ListFrame*);
    
    template<class E> void operator()(E&);
    
};


template<class F> class SaveAndReset{
    
public:
    
    //the frame to be closed
    F* frame;
    SaveAndReset(F*);
    
    template<class T> void operator()(T&);
    
};


//this checks if an element of the BodyField class is valid
template<class P> class CheckBody{
    
public:
    
    BodyField<P>* p;
    
    CheckBody(BodyField<P>*);
    template<class T> void operator()(T&);
    
    
};

//this checks if an element of the ProjectionField class is valid
template<class P> class CheckProjection{
    
public:
    
    ProjectionField<P>* p;
    
    CheckProjection(ProjectionField<P>*);
    template<class T> void operator()(T&);
    
};


//this checks the value of LimbField
template<class P> class CheckLimb{
    
public:
    
    LimbField<P>* p;
    
    CheckLimb(LimbField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P, class T> class CheckCheck{
    
public:
    
    CheckField<P,T>* p;
    
    CheckCheck(CheckField<P,T>*);
    //this functor checks whether a GUI Check field is filled correctly and writes its value into the relative non-GUI field
    template<class R> void operator()(R&);
    
};



template<class P> class CheckSign{
    
public:
    
    AngleField<P>* p;
    
    CheckSign(AngleField<P>*);
    template <class T> void operator()(T&);
    
    
};

template<class P> class CheckArcDegree{
    
public:
    
    AngleField<P>* p;
    
    CheckArcDegree(AngleField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckArcMinute{
    
public:
    
    AngleField<P>* p;
    
    CheckArcMinute(AngleField<P>*);
    template <class T> void operator()(T&);
    
};


template<class P> class CheckAngle{
    
public:
    
    //p is the AngleField which is parent of the CheckAngle object: the CheckAngle object checks the validity of the entries in AngleField
    AngleField<P>* p;
    CheckSign<P>* check_sign;
    CheckArcDegree<P>* check_arc_degree;
    CheckArcMinute<P>* check_arc_minute;
    
    CheckAngle(AngleField<P>*);
    template <class T> void operator()(T&);
    
};

template<class P> class TabulateDays{
    
public:
    
    DateField<P>* p;
    
    TabulateDays(DateField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckYear{
    
public:
    
    DateField<P>* p;
    TabulateDays<P>* tabulate_days;
    
    CheckYear(DateField<P>*);
    template<class T> void operator()(T&);
    
};

template<class P> class CheckMonth{
    
public:
    
    
    DateField<P>* p;
    TabulateDays<P>* tabulate_days;
    
    CheckMonth(DateField<P>*);
    template<class T> void operator()(T&);
    
};

template<class P> class CheckDay{
    
public:
    
    DateField<P>* p;
    
    CheckDay(DateField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckDate{
    
public:
    
    //the parent DateField
    DateField<P>* p;
    CheckYear<P>* check_year;
    CheckMonth<P>* check_month;
    CheckDay<P>* check_day;
    
    //constructor, which sets the parent
    CheckDate(DateField<P>*);
    template <class T> void operator()(T&);
    
};

template<class P> class CheckLengthValue{
    
public:
    
    LengthField<P>* p;
    
    CheckLengthValue(LengthField<P>*);
    template<class T> void operator()(T&);
    
};

template<class P> class CheckLengthUnit{
    
public:
    
    LengthField<P>* p;
    
    CheckLengthUnit(LengthField<P>*);
    template<class T> void operator()(T&);
    
};

template<class P> class CheckLength{
    
public:
    
    //p is the LengthField which is parent of the CheckLength object: the CheckLength object checks the validity of the entries in LengthField
    LengthField<P>* p;
    CheckLengthValue<P>* check_length_value;
    CheckLengthUnit<P>* check_length_unit;
    
    CheckLength(LengthField<P>*);
    template <class T> void operator()(T&);
    
};


template<class P> class CheckString{
    
public:
    
    StringField<P>* p;
    
    CheckString(StringField<P>*);
    template<class T> void operator()(T&);
    
};

template<class P> class CheckHour{
    
public:
    
    ChronoField<P>* p;
    
    CheckHour(ChronoField<P>*);
    template<class T> void operator()(T&);
    
    
};

template<class P> class CheckMinute{
    
public:
    
    ChronoField<P>* p;
    
    CheckMinute(ChronoField<P>*);
    template<class T> void operator()(T&);
    
    
};

template<class P> class CheckSecond{
    
public:
    
    ChronoField<P>* p;
    
    CheckSecond(ChronoField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckChrono{
    
public:
    
    //the parent field of the CheckChrono object
    ChronoField<P>* p;
    CheckHour<P>* check_hour;
    CheckMinute<P>* check_minute;
    CheckSecond<P>* check_second;
    
    CheckChrono(ChronoField<P>*);
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


//my own derived class of wxListCtrl
class ListControl : public wxListCtrl{
    
public:
    
    //a vector containing pointers to the buttons of *this (modify button, etc ...) which may be disabled if some event occurs
    vector<wxButton*> disableable_buttons;
    //header_width[i] = width of the header of the i-th column
    vector<int> header_width;
    
    ListControl(wxWindow*, vector<wxButton*>, const wxPoint&, const wxSize&);
    
    void SetColumns(vector<wxString>);
    void PushBackColumn(wxString);
    void DeselectAll(void);
    template<class T> void set(vector<T>, bool);
    void EnableButtons(bool);
    void Resize(void);
    void GetSelectedItems(vector<long>*);
    
};



//if an item is selected/deselected in caller, enable/disable the disableable buttons in caller
class OnChangeSelectionInListControl{
    
public:
    
    ListControl* caller;
    //the type of listcontrol: "sight" if caller = listcontrol_sights, "position" if caller = listcontrol_positions, "route" if caller = listcontrol_routes
    String type;
    
    OnChangeSelectionInListControl(ListControl*, String);
    template<class T> void operator()(T&);
    
};

//if an item is selected/deselected in caller, call operator() to make sure that only one item is selected at a time
template<class P> class OnChangeSelectionInLimbField{
    
public:
    
    LimbField<P>* caller;
    
    OnChangeSelectionInLimbField(LimbField<P>*);
    template<class T> void operator()(T&);
    
};


class SetStringToCurrentTime{
    
public:
    
    String* p;
    
    SetStringToCurrentTime(String*);
    template<class T> void operator()(T&);
    
};

template<class P> class SetStringFieldToCurrentTime{
    
public:
    
    StringField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

//this class defines the functor () used to remove a sight from the non-GUI object data
class DeleteSight{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the sight to be removed
    long i_sight_to_remove;
    //this is equal to 'y' if the route related to the removed sight has to be removed too, and 'n' otherwise
    Answer remove_related_route;
    
    DeleteSight(ListFrame*, Answer);
    void operator()(wxCommandEvent&);
    
};

//this class defines a functor () used to modify an existing Route
class ExistingRoute{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    ExistingRoute(ListFrame*);
    void operator()(wxCommandEvent&);
    
};


//this class defines a functor () used to create a new Route
class NewRoute{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    NewRoute(ListFrame*);
    void operator()(wxCommandEvent&);
    
};

//this class defines a functor () used to select all Routes to compute the astronomical position
class AllRoutes{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    AllRoutes(ListFrame*);
    void operator()(wxCommandEvent&);
    
};

//this class defines a functor () used to select some specific Routes to compute the astronomical position
class SomeRoutes{
    
public:
    
    //the frame which called this struct
    ListFrame* f;
    
    SomeRoutes(ListFrame*);
    void operator()(wxCommandEvent&);
    
};


//this functor sets idling -> true in parent
template<class P> class SetIdling{
    
public:
    
    //the frame which called this struct
    P* parent;
    
    SetIdling(P*);
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


//this functor sets idling -> false in parent
template<class P> class UnsetIdling{
    
public:
    
    UnsetIdling(P*);
    
    //the frame which called this struct
    P* parent;
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


//this functor pops up a QuestionFrame and asks the user whether he wants to remove the a Sight related to a Route
class AskRemoveRelatedSight{
    
public:
    
    AskRemoveRelatedSight(ListFrame*);
    
    //the frame which called this struct
    ListFrame* parent;
    
    void operator()(wxCommandEvent&);
    
};


//this functor pops up a QuestionFrame and asks the user whether he wants to remove the a Route related to a Sight
class AskRemoveRelatedRoute{
    
public:
    
    AskRemoveRelatedRoute(ListFrame*);
    
    //the frame which called this struct
    ListFrame* parent;
    
    void operator()(wxCommandEvent&);
    
};


//this functor allows the user to select a Route in *parent frame
class SelectRoute{
    
public:
    
    SelectRoute(ListFrame*);
    
    //the ListFrame which called this struct
    ListFrame* parent;
    
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


//this class defines the functor () used to remove a Route from the non-GUI object data
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

//this class defines the functor () used to remove a Position from the non-GUI object data
class DeletePosition{
    
public:
    
    DeletePosition(ListFrame*);
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the Position to be removed
    long i_position_to_remove;
    
    
    void operator()(wxCommandEvent&);
    
};


//this is a GUI field contaning a binary checkbox, which is either checked or unchecked
template<class P, class T> class CheckField{
    
public:
    
    //the parent frame to which this object is attached
    P* parent;
    Answer* answer;
    //related_field is a GUI field (such as ChronoField, etc) related to this CheckField, such that: if direct_reverse = true->  when the checkbox in this CheckFIeld is checked (unchecked), related_field is active (inactive). If direct_reverse = false ->  when the checkbox in this CheckFIeld is unchecked (checked), related_field is active (inactive).
    T* related_field;
    bool direct_reverse;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxCheckBox with the name of the bodies
    wxCheckBox* checkbox;
    CheckCheck<P,T>* check;
    
    CheckField(wxPanel*, Answer*, T*, bool);
    
    template<class R> void InsertIn(R*);
    template<class S> void get(S&);
    void set(void);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
    
};

//this functor pops out a message window with title tile and  message message, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this class adaptable
template<class T, typename FF_OK> class PrintMessage{
    
public:
    
    T* f;
    wxControl* control;
    String title, message, image_path;
    FF_OK* f_ok;
    MessageFrame<FF_OK>* message_frame;
    
    PrintMessage(T*, FF_OK*);
    
    void SetAndCall(wxControl*, String, String, String);
    void operator()(void);
    
    
};

//this functor pops out a question window with title tile, quesiton question, and answers answer_y, answer_n, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this class adaptable. If the user answers yes/no to the question, f_yes/f_no are called
template<class T, typename FF_YES, typename FF_NO> class PrintQuestion{
    
public:
    
    T* f;
    wxControl* control;
    String title, message, question, answer_y, answer_n;
    FF_YES* f_yes;
    FF_NO* f_no;
    QuestionFrame<FF_YES, FF_NO>* question_frame;
    
    PrintQuestion(T*, FF_YES*, FF_NO*);
    
    void SetAndCall(wxControl*, String, String, String, String);
    void operator()(void);
    
    
};


//my own derived class of wxStaticText
class StaticText : public wxStaticText{
    
public:
    
    
    StaticText(wxWindow*, const wxString&, const wxPoint&, const wxSize&, long);
    
};


class DrawPanel : public wxPanel{
    
public:
    
    DrawPanel(ChartPanel*, const wxPoint& position_in, const wxSize& size_in);
    ChartFrame* parent;
    PrintMessage<DrawPanel, UnsetIdling<DrawPanel> >* print_error_message;
    wxPoint position_draw_panel, position_plot_area, position_start_selection, position_end_selection, /*the instantaneous positions of the mouse with respect to the screen/draw-panel origin*/position_screen_now, position_start_drag, position_end_drag, position_now_drag, position_draw_panel_now;
    unsigned int size_label_vertical, size_label_horizontal;
    /*x_min, x_max, y_min, y_max do correspond to lambda_min, lambda_max, etc... They are ordered in such a way that x_min <= x_max and y_min <= y_max always. */
    double /*min and max values of the Mercator or 3D projections x, y*/x_min, x_max, y_min, y_max, x_min_start_drag, x_max_start_drag, y_min_start_drag, y_max_start_drag, x_span_start_drag,
    /*these are the values of x_min .. y_max and x_span when the plot is first drawn*/
    x_min_0, x_max_0, y_min_0, y_max_0, x_span_0,
    /*the coordinates of the center of the projection when scrolling starts*/ x_center_scrolling, y_center_scrolling,
    /*these are the values of x_min, ... y_max after each sliding event, corresponding to lambda_min, ... , phi_max read from file*/x_min_old, x_max_old, y_min_old, y_max_old, /*these are the angular separations in latitude and longitude between meridians and parallels, respectively */delta_lambda, delta_phi,     /*this is a quantity resulting from the 3D projection: if and only if it is >= 0, then the point under consideration in the 3d projeciton lies on the visible surface of the earth */ arg_sqrt, /*increments in longitude/latitude to draw minor ticks*/delta_lambda_minor, delta_phi_minor, lambda_span, phi_span;;
    //the precision used to print out labels
    Int label_precision;
    
    
    //the euler angles which specify the orientation of the earth for the 3d representation
    //two auxiliary vectors which will be used later
    gsl_vector *r, /*vector position in the x'y'z' reference frame used for multiple purposes*/*rp, /*vector position in the x'y'z' reference frame at the beginning, end and current time of mouse drag*/*rp_start_drag, *rp_end_drag, *rp_now_drag;
    Rotation /*the orientation of the Earth at the beginning / current time / end of a drag*/rotation_start_drag, rotation_now_drag, rotation_end_drag, /*the rotation representing the current / initial orientation of the earth*/rotation, rotation_0;
    Double /*the distance between the plane of the 2d projection and the eye of the observer for the 3d plot, and its initial value when this is constructedd, d_0,*/ /*if the mouse hovers over a route and its y coordinate is equal to the y of the route +- (length sceen) * thickness_route_selection_over_length_screen /2, then the relative Route is highlighted in ListFrame*/thickness_route_selection_over_length_screen;
    StaticText *text_position_start, *text_position_end, /*this is used to display on the chart the coordinates of a Position that is being dragged or of the reference position of a Route that is being dragged*/*text_geo_position;
    bool /*this is true if the mouse is dragging with the left button pressed*/mouse_dragging, idling, /*if re_draw = true (false), then one has to draw the non-highglighteable stuff in DrawPanel (coastlines, paralles, meridians ...  but not Routes nor Positions)*/re_draw;
    Position /*I store in this position the starting point (ground position) of a Route if the Route is a loxodrome or orthodrome (circle of equal altitude) that I want to drag, at the beginning of the dragging process*/route_position_start_drag, /*current, starting and ending geographic position in a mouse drag process*/ geo_now_drag, geo_start_drag, geo_end_drag, /*the position on the sphere such that the vector between the center of the sphere and the position equals the direction of the rotation axis relative to a mouse drag*/rotation_axis, /*the geographic positions corresponding to the NW (SE) boundary of of the plot area, moved to the interior of the plot area by one pixel. These will be used to plot parallels and meridians in such a way that they don't hit the boundary of the plot area*/p_NW, p_SE;
    Angle rotation_angle, /*an angle containing the middle longitude/latitude of the current 3D projection, rounded up to the closest value which is a multiple of delta_lambda/phi, used for drawing things in the middle of the projection*/lambda_middle, phi_middle, /*lambda/phi_start/end are the start/end values of longidue/latitude adapted in the right form ro the loopws which draw meridians/parallels*/ lambda_start, lambda_end, phi_start, phi_end, lambda_saved, phi_saved, Z_saved;
    Projection /*the values of (x, y) at the beginning/end of the selection process with a rectangle*/start_selection, end_selection;
    Route /*this is a circle of equal altitude which is the intersection between the earth's surface and the visibility cone of the observer, whose vertex is at the observer. circle_oberserver.omega is the aperture angle at the center of the earth which specifies the part of the earth surface visible by the observer. circle_observer.rerefence_position is the ground position of the observer: the intersection between the line between the center of the earth and the observer, and the surface of the earth. These exist for the 3D projection only*/circle_observer, /*the same as circle_observer, but at the initial configuration fo the chart*/circle_observer_0;
    //this rectangle represents the rectangle x_min ... y_max in the Mercator projection
    MyRectangle rectangle_observer;
    wxBitmap m_bgbuffer;
    
    
    
    wxSizer* sizer_h, *sizer_v;
    //the i-th element of point_route_list is a list: in this list there are as many elements as the number of connected curves in which the route is cut (because of the meridian lambda = pi). In each of these elements there are the  points, with respect to the origin of DrawPanel, which represent the route_list[i] discretized
    vector< vector< vector<wxPoint> > > points_route_list;
    //the i-th element of this vector contains a list of critical values of the parametric angle (t) of the i-th route. At these critical values, route #i crosses the meridian lambda = pi
    //the chart contains the plot area, and the following quantities are the width and height of chart and plot area
    wxSize size_chart, size_plot_area;
    unsigned int  /*these are the values of width/height_chart when the chart is first drawn*/width_chart_0, height_chart_0, tick_length, /* gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes when setting the ticks on the x axis, and similarly for gamma_phi*/gamma_lambda, gamma_phi;
    //this is true if the label which is being drawn is the first among the parallel/meridian labels, false otherwise
    bool first_label;
    wxMemoryInputStream * memory_input_stream;
    UnsetIdling<DrawPanel>* unset_idling;
    vector<wxString> labels_lambda, labels_phi;
    vector<wxPoint> positions_labels_lambda, positions_labels_phi;
    
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither DrawPanel::Draw_Mercator or DrawPanel::Draw_3D, according to my needs, and similarly for the other pointers
    void (DrawPanel::*Draw)(void);
    bool (DrawPanel::*ScreenToProjection)(wxPoint, Projection*);
    bool (DrawPanel::*ScreenToGeo)(wxPoint, Position*);
    bool (DrawPanel::*GeoToProjection)(Position, Projection*, bool);
    void (DrawPanel::*Render)(wxDC*);
    void (DrawPanel::*ProjectionToDrawPanel)(Projection, wxPoint*);
    void (DrawPanel::*Set_x_y_min_max)(void);
    void (DrawPanel::*Set_lambda_phi_min_max)(void);
    void (DrawPanel::*Set_size_chart)(void);
    
    
    void SetIdling(bool);
    void Draw_Mercator(void);
    void Draw_3D(void);
    void TabulateRoutes(void);
    void PaintEvent(wxPaintEvent & evt);
    void FitAll();
    
    bool ScreenToDrawPanel(wxPoint, wxPoint*);
    void GeoToScreen(Position, wxPoint*);
    bool DrawPanelToGeo(wxPoint, Position*);
    bool ScreenToMercator(wxPoint, Projection*);
    bool ScreenTo3D(wxPoint, Projection*);
    bool GeoToDrawPanel(Position, wxPoint*, bool);
    bool GeoTo3D(Position, Projection*, bool);
    bool GeoToMercator(Position, Projection*, bool);
    bool ScreenToGeo_Mercator(wxPoint, Position*);
    bool ScreenToGeo_3D(wxPoint, Position*);
    void ProjectionToDrawPanel_Mercator(Projection, wxPoint*);
    void ProjectionToDrawPanel_3D(Projection, wxPoint*);
    void ShowCoordinates(Position, wxStaticText*);
    void ShowCoordinates(wxPoint, wxStaticText*);
    void SetCoordinateLabel(Position, wxPoint, wxStaticText*);
    void Set_lambda_phi_min_max_Mercator(void);
    void Set_lambda_phi_min_max_3D(void);
    void Set_x_y_min_max_Mercator(void);
    void Set_x_y_min_max_3D(void);
    void Set_size_chart_Mercator(void);
    void Set_size_chart_3D(void);
    bool PutBackIn(wxPoint, wxPoint*);
    bool check(const Projection);
    bool check_x(const Projection);
    double x_span(void);
    Rotation rotation_start_end(wxPoint, wxPoint);
    
    void Render_Mercator(wxDC*);
    void Render_3D(wxDC*);
    void WriteLabel(const Position&, Angle, Angle, Int, String, wxString*);
    void DrawLabel(const Position&, Angle, Angle, Int, String);
    
    bool GetMouseGeoPosition(Position*);
    void OnMouseMovement(wxMouseEvent&);
    void OnMouseLeftDown(wxMouseEvent&);
    void OnMouseLeftUp(wxMouseEvent&);
    void OnMouseRightDown(wxMouseEvent&);
    void OnMouseDrag(wxMouseEvent&);
    void OnMouseWheel(wxMouseEvent&);
    template<class E> void OnChooseProjection(E&);
    void KeyDown(wxKeyEvent&);
    
    DECLARE_EVENT_TABLE()
};

template<class P> class ProjectionField{
    
public:
    P* parent;
    
    wxBoxSizer *sizer_h, *sizer_v;
    wxArrayString  /*this is equal to a standard list of the available graphical types*/types;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    CheckProjection<P>* check;
    
    vector<int> recent_items;
    
    bool ok;
    
    ProjectionField(wxPanel*);
    void read_recent_items(void);
    void write_recent_items(void);
    template<class S> void write_recent_items_to_stream(S*);
    template<class T> void InsertIn(T*, wxSizerFlags&);
    template<class E> void OnEdit(E&);
    
};

//P is the type of the frame which hosts *this
template<class P> class BodyField{
    
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
    CheckBody<P>* check;
    
    bool ok;
    
    BodyField(wxPanel*, Body*, Catalog*);
    void read_recent_bodies(void);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEdit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


template<class P> class LimbField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    
    //this is the wxComboBox with the name of the limbs
    wxCheckListBox* name;
    wxArrayInt checked_items;
    wxArrayString limbs;
    //this points to a Limn object, which contains the data written in the GUI field of this
    Limb* limb;
    wxBoxSizer *sizer_h, *sizer_v;
    bool ok;
    CheckLimb<P>* check;
    OnChangeSelectionInLimbField<P>* change_selection;
    
    LimbField(wxPanel*, Limb*);
    void set(void);
    void Enable(bool);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEdit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


//class for graphical object: a field to enter an angle, composed of a box for the sign, a box for the degrees, a degree text symbol, another box for minutes and a minute text symbol
template<class P> class AngleField{
    
public:
    //the parent frame to which this object is attached
    P* parent;
    wxArrayString signs, degrees;
    //the format of the AngleField: String("") for angles  between 0 and 2*pi, String("+-") for angles with a sign between 0 and pi, String("NS") for latitudes between -pi/2 and pi/2, and String("EW") for longitudes between -pi and pi
    String format;
    //degrees and minutes boxes
    wxComboBox*sign, * deg;
    wxTextCtrl *min;
    //texts
    StaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    Angle* angle;
    //deg_ok = true if the degrees part of this angle is formatted properly and set to the same value as the degree part of angle, and simiarly for min
    bool sign_ok, deg_ok, min_ok;
    CheckAngle<P>* check;
    
    
    AngleField(wxPanel*, Angle*, String);
    void Enable(bool);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditSign(E&);
    template<class E> void OnEditArcDegree(E&);
    template<class E> void OnEditArcMinute(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
};

//class for graphical object: a field to enter a length, composed of a box. P is the type of the parent which hosts the LengthField object
template<class P> class LengthField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    //degrees and minutes boxes
    wxTextCtrl *value;
    //units of measure of the length
    wxComboBox* unit;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;
    //an array containing all possible units of measure
    wxArrayString units;
    //the units of measure of the length in this GUI field
    String unit_value;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool value_ok, unit_ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    CheckLength<P>* check;
    
    LengthField(wxPanel*, Length*, String);
    void set(void);
    template<class T> void get(T&);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditValue(E&);
    template<class E> void OnEditUnit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};

//class for graphical object: a field to enter a String, composed of a box. P is the type of the object in which this StringField will be inserted
template<class P> class StringField{
    
public:
    //the parent where this StringField object will be inserted
    P* parent;
    //label box
    wxTextCtrl *value;
    wxBoxSizer *sizer_h, *sizer_v;
    //non-GUI object related to this
    String* string;
    CheckString<P>* check;
    SetStringFieldToCurrentTime<P> set_to_current_time;
    
    StringField(wxPanel*, String*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


template<class P> class DateField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    wxArrayString days, months;
    //year, month and day boxes
    wxTextCtrl *year;
    wxComboBox *month, *day;
    //texts
    StaticText* text_hyphen_1, *text_hyphen_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Date* date;
    //year_ok = true if the year is formatted properly and set to the same value as date->Y, and similarly for the other variables
    bool year_ok, month_ok, day_ok;
    CheckDate<P>* check;
    
    DateField(wxPanel*, Date*);
    void set(Date);
    void SetBackgroundColor(Color);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditYear(E&);
    template<class E> void OnEditMonth(E&);
    template<class E> void OnEditDay(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
};


template<class P> class ChronoField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    wxArrayString hours, minutes;
    //hour and minute  boxes
    wxComboBox *hour, *minute;
    //second text control
    wxTextCtrl *second;
    //texts
    StaticText* text_colon_1, *text_colon_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Chrono* chrono;
    //hour_ok = true if the hour is formatted properly and set to the same value as chrono->h, and similarly for the other variables
    bool hour_ok, minute_ok, second_ok;
    CheckChrono<P>* check;
    
    ChronoField(wxPanel*, Chrono*);
    void set(Chrono);
    void Enable(bool);
    void SetBackgroundColor(Color);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditHour(E&);
    template<class E> void OnEditMinute(E&);
    template<class E> void OnEditSecond(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
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
    template<class E> void OnEdit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


//this class is to define the functor template<class T> void operator()(T&), which is called when the user selects a route with which he wants to transport a sight
class OnSelectRouteInListControlRoutesForTransport{
    
public:
    
    //parent frame
    ListFrame* f;
    //the object which is being transported : a "sight" or a "position"
    String transported_object;
    
    //constructor, which sets the parent frame
    OnSelectRouteInListControlRoutesForTransport(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};

//this class is to define the functor template<class T> void operator()(T&), which is called when the user crates a new route with which he wants to transport a sight
class OnNewRouteInListControlRoutesForTransport{
    
public:
    
    //parent frame
    ListFrame* f;
    //the object which is being transported : a "sight" or a "position"
    String transported_object;
    
    //constructor, which sets the parent frame
    OnNewRouteInListControlRoutesForTransport(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};



//this is a wxFrame designed to contain the list of sights, routes, etc...
class ListFrame: public wxFrame{
    
public:
    
    Data /*pointer to the non-GUI object Data which is related to the GUI object this*/*data;
    //point to the child frame of this
    vector<ChartFrame*> chart_frames;
    //this is a pointer to a Catalog object which will be used by data
    Catalog *catalog;
    vector<Route> /*when I transport by using an existing Route, I will store the list of Routes which can be used for transport here*/route_list_for_transport, /*when I transport by using an existing Route, I will save data->list_routes temporarily here and recover it from here after the transport */route_list_saved;
    wxMenuBar *menu_bar;
    wxMenu *menu_app, *menu_file, *menu_chart, *menu_new_chart, *menu_item_mercator, *menu_item_3d;
    ListControl *listcontrol_sights, *listcontrol_routes, *listcontrol_positions;
    //extract_colors is created only to detect the background and foreground colors set by the operating systems, it has no other use. As a result, its size will be set to zero.
    wxTextCtrl* extract_colors;
    wxPanel *panel;
    //a dummy RouteFrame which will be used for various purposes
    RouteFrame* route_frame;
    wxButton  *button_delete_position, *button_delete_route, *button_show_map, *button_compute_position;
    wxBitmapButton *button_add_sight, *button_add_position, *button_add_route, *button_modify_sight, *button_delete_sight, *button_transport_sight, *button_transport_position, *button_modify_position, *button_modify_route, *button_transport_route, *button_disconnect_sight, *button_disconnect_route;
    wxBoxSizer *sizer_all, *sizer_h, *sizer_v;
    wxSizer *sizer_buttons_sight, *sizer_buttons_position, *sizer_buttons_route;
    wxStaticBoxSizer* sizer_box_sight, *sizer_box_position, *sizer_box_route;
    DeleteSight *delete_sight, *delete_sight_and_related_route;
    DeleteRoute *delete_route, *delete_route_and_related_sight;
    DeletePosition *delete_position;
    OnChangeSelectionInListControl *on_change_selection_in_listcontrol_sights, *on_change_selection_in_listcontrol_routes, *on_change_selection_in_listcontrol_positions;
    ExistingRoute *existing_route;
    NewRoute *create_route;
    //a temporary value of data->crossing_route_list
    vector<Route> crossing_route_list_temp;
    bool selection_rectangle, /*this is true/false if highlighting of routes and sights is enabled/disables*/enable_highlight, /*idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false*/ idling, /*this is equal to true if file has been modified, false otherwise*/file_has_been_modified, /*this is equal to true if the file has no name, false otherwise*/file_is_untitled, /*this is true if I am computing the astronomical position, false otherwise*/selecting_route_for_position, /*this is equal to true (false) if the user has (has not) pressed cancel while charts were loading */abort;
    Answer /*if this is y/n, the coastlines are shown/not shown*/show_coastlines, /*if this is y/n, sample_sight.nav is loaded/not loaded at startup*/ load_sample_sight;
    //the file where the data is read and written
    FileRW data_file;
    
    unsigned int margin;
    int /*the # of the sight/route/position which is highlighted because the mouse is hovering over it in listcontrol_sights/routes/positions*/highlighted_sight, highlighted_route, highlighted_position, /*# of the object to transport or disconnect */i_object_to_transport, i_object_to_disconnect;
    /*map[i] is the position in data->route_list of the i-th Route in route_list_for_transport*/
    vector<int> map;
    //data_x[i][j] is a vector which contains the (x-value of) the datapoints within the block at (shifted) latitude i and longitude j in file path_file_coastline_data_blocked
    vector< vector< vector<Position> > > p_coastline;
    Position /*these are the positions where the right mouse button is clicked at the beginning, current time and at the end of the drawing process for the selection rectangle on the world's chart*/p_start, p_now, p_end;
    
    //a functor to set/unset idling mode in *this
    SetIdling<ListFrame>* set_idling;
    UnsetIdling<ListFrame>* unset_idling;
    CloseFrame<ListFrame>* close;
    //a functor to let the user select a Route in listcontrol_routes
    SelectRoute* select_route;
    PrintMessage<ListFrame, UnsetIdling<ListFrame> >* print_warning_message, *print_error_message;
    PrintMessage<ListFrame, SelectRoute >* print_info_message;
    
    OnSelectRouteInListControlRoutesForTransport* on_select_route_in_listcontrol_routes_for_transport;
    OnNewRouteInListControlRoutesForTransport* on_new_route_in_listcontrol_routes_for_transport;
    AskRemoveRelatedSight* ask_remove_related_sight;
    AskRemoveRelatedRoute* ask_remove_related_route;
    
    ListFrame(const wxString&, const wxString&, const wxPoint&, const wxSize&, String);
    
    void set(void);
    void DrawAll(void);
    void Resize(void);
    
    void GetAllCoastLineData(String);
    
    void OnAddSight(wxCommandEvent&);
    template<class E> void OnModifySight(E&);
    void OnTransportSight(wxCommandEvent&);
    void OnDisconnectSight(wxCommandEvent&);
    template<class E> void OnPressDeleteSight(E&);
    
    void OnAddPosition(wxCommandEvent& event);
    template<class E> void OnModifyPosition(E&);
    void OnTransportPosition(wxCommandEvent&);
    template<class E> void OnPressDeletePosition(E&);
    
    void OnAddRoute(wxCommandEvent&);
    void OnAddRouteForTransport(wxCommandEvent&);
    template<class E> void OnModifyRoute(E&);
    void OnTransportRoute(wxCommandEvent&);
    void OnDisconnectRoute(wxCommandEvent&);
    template<class E> void OnPressDeleteRoute(E&);
    
    template<class E> void Disconnect(E&);
    
    void OnAddChartFrame(wxCommandEvent&);
    void OnCloseActiveChartFrame(wxCommandEvent&);
    void OnCloseAllChartFrames(wxCommandEvent&);
    void OnComputePosition(void);
    
    //    void OnMouseOnListControlSights(wxMouseEvent&);
    void OnMouseMovement(wxMouseEvent&);
    //    void OnMouseOnListControlPositions(wxMouseEvent&);
    
    void OnModifyFile(void);
    void OnSaveFile(void);
    template<class E> void OnPressCtrlO(E&);
    template<class E> void OnPressCtrlW(E&);
    template<class E> void OnPressCtrlS(E&);
    template<class E> void OnPressCtrlShiftS(E&);
    template<class E> void KeyDown(E&);
    template<class T> void ComputePosition(T&);
    
};




class SightFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    Catalog* catalog;
    Sight* sight;
    //this long represents the position in the list (this->GetParent())->listcontrol_sights of sight. If list_position = -1, then sight is not in that list
    long position_in_listcontrol_sights;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling, /*this is true if all the GUI fileds which specify sight->time are ok and the resulting sight->time falls within the time covered by ephemerids' data files, and false otherwise*/time_interval_ok;
    //this messages displays a text on SightFrame if the time interval entered by the user does not lie within the interval covered by the ephemerides' data
    StaticText* text_time_interval_not_ok;
    //image shown on SightFrame if the time interval entered by the user does not lie within the interval covered by the ephemerides' data
    wxStaticBitmap* image_time_interval_status;
    wxImage* image_time_interval_not_ok;
    
    SetIdling<SightFrame>* set_idling;
    UnsetIdling<SightFrame>* unset_idling;
    PrintMessage<SightFrame, UnsetIdling<SightFrame> >* print_error_message;
    
    BodyField<SightFrame>* body;
    LimbField<SightFrame>* limb;
    CheckField<SightFrame, LengthField<SightFrame> >* artificial_horizon_check;
    CheckField<SightFrame, ChronoField<SightFrame> >* stopwatch_check;
    AngleField<SightFrame>* H_s, *index_error;
    LengthField<SightFrame>* height_of_eye;
    DateField<SightFrame>* master_clock_date;
    ChronoField<SightFrame>* master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    StringField<SightFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_time, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    wxStaticBoxSizer *sizer_box_measurement, *sizer_box_time;
    
    wxButton* button_reduce, *button_cancel;
    wxMenuBar *menuBar;
    
    SightFrame(ListFrame*, Sight*, long, const wxString&, const wxPoint&, const wxSize&, String);
    void set(void);
    bool is_ok();
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void AllOk(void);
    //    template<class T> void OnEditTime(T&);
    void TimeIntervalOk(String);
    void KeyDown(wxKeyEvent&);
    template<class E> void check(E&);
    
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
    
    SetIdling<PositionFrame>* set_idling;
    UnsetIdling<PositionFrame>* unset_idling;
    PrintMessage<PositionFrame, UnsetIdling<PositionFrame> >* print_error_message;
    
    AngleField<PositionFrame>* lat, *lon;
    StringField<PositionFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2;
    wxStaticBoxSizer *sizer_box_measurement;
    
    wxButton* /*button_ok denotes a button which triggers either an 'Add' command, when one is adding a new position, or a 'Modify' command, when one is modifying a position whih already exists*/button_ok, *button_cancel;
    wxMenuBar *menuBar;
    
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    bool is_ok(void);
    void AllOk(void);
    void KeyDown(wxKeyEvent&);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

class RouteFrame: public wxFrame{
    
public:
    RouteFrame(ListFrame*, Route*, bool, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Route* route;
    //this long represents the position in the list (this->GetParent())->listcontrol_routes of route. If list_position = -1, then route is not in that list
    long position_in_listcontrol_routes;
    wxPanel *panel;
    bool /*idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false*/idling, /*for_transport = true (false) means that the Route is meant to transport objects*/for_transport;
    
    SetIdling<RouteFrame> * set_idling;
    UnsetIdling<RouteFrame> * unset_idling;
    PrintMessage<RouteFrame, UnsetIdling<RouteFrame> >* print_error_message;
    
    RouteTypeField *type;
    AngleField<RouteFrame> *Z, *omega, *start_phi, *start_lambda, *GP_phi, *GP_lambda;
    LengthField<RouteFrame> *l;
    StringField<RouteFrame> *label;
    
    wxFlexGridSizer *sizer_grid_type, *sizer_grid_Z, *sizer_grid_l,  *sizer_grid_omega, *sizer_grid_start, *sizer_grid_GP, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer;
    wxStaticBoxSizer *sizer_box_data, *sizer_box_start, *sizer_box_GP;
    
    wxButton* /*this button triggers either the addition of a new Route, or the modification of an existing one*/button_ok, *button_cancel;
    
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    bool is_ok(void);
    void AllOk(void);
    void KeyDown(wxKeyEvent&);
    
    //    wxDECLARE_EVENT_TABLE();
    
};



class ChartFrame: public wxFrame{
    
public:
    ChartFrame(ListFrame*, String, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    StaticText *text_position_now, *text_slider;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxGridSizer* sizer_buttons;
    wxStaticBitmap* image;
    wxSlider* slider;
    wxBitmapButton* button_show_list;
    wxButton* button_up, *button_down, *button_left, *button_right, *button_reset;
    ProjectionField<ChartFrame>* projection;
    PrintMessage<ChartFrame, UnsetIdling<ChartFrame> >* print_error_message;
    /*the latitude/longitude setting the boundaries of the plotted area on earth*/Angle phi_min, phi_max, /*it is not necessarily true that lambda_min < lambda_max: lambda_min(max) correspond to the left(right) edge of the plot area*/lambda_min, lambda_max;
    //in p_coastline_draw, I store the 2d coordindates  in DrawPanel coordinates of coastline data p_coastline
    vector<wxPoint> p_coastline_draw;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling, /*this is true if the user is currently scrolling*/mouse_scrolling, /*this is true if the chart is being dragged, and thus the size of *this must not change across multiple Draw(s), and false otherwise*/ dragging_chart;
    //This is the actual value of the maximal zoom factor allowed
    Double /*the zoom factor relative to the default configuration of either projection, the zoom factor  is not necessarily equal to the numerical value (slider->GetValue()) shown on the slider*/zoom_factor;
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither ChartFrame::UpdateSliderLabel_Mercator or ChartFrame::UpdateSliderLabel_3D, according to my needs, and similarly for the other pointers
    void (ChartFrame::*UpdateSliderLabel)(void);
    
    void GetCoastLineData_Mercator(void);
    void GetCoastLineData_3D(void);
    void UpdateSlider(void);
    bool ComputeZoomFactor_Mercator(double);
    bool ComputeZoomFactor_3D(void);
    void UpdateSliderLabel_Mercator(void);
    void UpdateSliderLabel_3D(void);
    template<class T> void OnMouseLeftDownOnSlider(T&);
    template<class T> void OnMouseLeftUpOnSlider(T&);
    template<class T> void OnScroll(T&);
    bool SetSlider(unsigned int);
    template<class T> void OnPressCtrlW(T&);
    void SetIdling(bool);
    void AllOk(void);
    
    template<class T> void MoveNorth(T&);
    template<class T> void MoveSouth(T&);
    template<class T> void MoveWest(T&);
    template<class T> void MoveEast(T&);
    template<class T> void Reset(T&);
    
    //this is created for testing
    UnsetIdling<ChartFrame> *unset_idling;
    
    
    
    
};

class ChartPanel : public wxPanel{
    
public:
    
    ChartFrame* parent;
    
    ChartPanel(ChartFrame*, const wxPoint&, const wxSize&);
    
};
