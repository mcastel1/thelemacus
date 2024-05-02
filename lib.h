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
#include <wx/tooltip.h>






#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//#include "chartdir.h"


template<class P> class BodyField;
template<class P, class NON_GUI, class CHECK> class MultipleItemField;
template<class P> class ProjectionField;
template<class P> class LengthFormatField;
template<class P> class LimbField;
template<class P> class LengthUnitField;
template<class P> class SpeedUnitField;
template<class P, class T> class CheckField;
template<class P> class AngleField;
template<class P> class LengthField;
template<class P> class SpeedField;
template<class P> class DateField;
template<class P> class ChronoField;
template<class P> class RouteTypeField;
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
class Cartesian;
template <class NON_GUI, class F> class GraphicalFeatureTransportHandler;
class LengthFormat;
class RouteType;
class Projection;
class LengthUnit;
class SpeedUnit;

class Catalog;
class Limb;
class Length;
class Speed;
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
//template<class P> class CheckProjection;
template<class P> class CheckArcDegree;
template<class P> class CheckArcMinute;
template<class P> class TabulateDays;
template<typename FF_OK> class MessageFrame;

class MotionHandler;
class ChartTransportHandler;


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
    String(const string&);
    String(const char&);
    void print(String, bool, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
//    template<class S> void read_ints_from_stream(String, S*, bool, vector<int>*);
    void read_from_file_to(String, String, String, String);
    void write_to_file(String, FileRW&, String);
    void set(String, String, String);
    void set(String);
    void set_to_current_time(void);
    String append(String);
    void appendto(String);
    String prepend(String);
    void replace_to(const char&, const char&);
    String subString(size_t, size_t);
    bool split_file_path(String*, String*, String*, String);
    String filename_without_folder_nor_extension(String);
    bool is_a_file_path(String);
    wxSize get_size(wxWindow*);
    wxSize get_size(wxDC*);
    unsigned int get_length(void);
    vector<String> split(void);
    int position_in_list(const vector<String>&);
    int position_in_list(const wxArrayString&);

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
    double get(void);
    void set_current(void);
    template<class S> bool read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    string to_string(unsigned int, bool);
    
    bool operator==(const Chrono&), operator!=(const Chrono&), operator<(const Chrono&), operator>(const Chrono&);
    
};



class Double{
    
public:
    double value;
    
    Double();
    Double(const double&);
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
    Angle(const double&);
    Angle(const unsigned int&, const double&);
    Angle(String, const double&, const String&);
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
    
    bool operator == (const Angle&), operator == (const double&), operator != (const double&), operator !=(const Angle&), operator > (const Angle&), operator > (const double&);
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
        
     
#ifdef __APPLE__
        //on APPLE operating system I need to correct the Mouse position in the following way

        //obtain the position of the rectangle of the first visible item in *this and store it in r
        list_control->GetItemRect(list_control->GetTopItem(), r, wxLIST_RECT_BOUNDS);

		//decrease the mouse position with respect to the origin of *this located on the bottom edge of the gray band on the top by r.y on the y axis -> now p is the mouse position with respect to the top-left origin of the white area of *this
		(p.y) -= r.y;
#endif
        
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
        //the non-GUI object connected to the GUI object MessageFrame
    wxPanel *panel;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_buttons;
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
    
    QuestionFrame(wxWindow*, F_A*, String, F_B*, String, const wxString&, const wxString&,  String, const wxPoint&, const wxSize&, String);
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

//a derived class from String, which stores the unit of measuer of Length ("nm", "m", "ft", ...)
class LengthUnit: public String{
    
public:

    LengthUnit();
    LengthUnit(const String&);
    
};


//a derived class from String, which stores the unit of measuer of Speed ("kt", etc...)
class SpeedUnit: public String{
    
public:

    SpeedUnit();
    SpeedUnit(const String&);
    
};


//a non-GUI object containing a Length
class Length{
    
public:
    //the value of the Length
    double value;
    //the unit of measure of the Length
    LengthUnit unit;
    
    Length();
    Length(double);
    Length(double, const LengthUnit&);
    Length(Chrono, Speed);
    
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
    
    //the value of the Speed
    double value;
    //the unit of measure of the Speed
    SpeedUnit unit;

    
    Speed();
    Speed(double);
    Speed(double, const SpeedUnit&);
    void set(String, double, String);
    void print(String, String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
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
    
    Rotation inverse(void);
    void set(Angle, Angle, Angle);
    void set(gsl_matrix*);
    void print(String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    
    Rotation operator *(const Rotation&);

};

//this function returns the longitude value (expressed in degrees, positive towards W) of the inverse spherical Mercator projection from the rectangular x value
inline double lambda_mercator(double x){
    
    return (-x*K - 360.0*floor((x-(-M_PI))/(2.0*M_PI)));
    
}

//this function returns the latitude value (expressed in degrees) of the inverse spherical Mercator projection from the rectangular y value
inline double phi_mercator(double y){
    
    return(K*atan(sinh(y)));
    
}


//a geographic Position
class Position{
    
public:
    //latitude and longitude of the position
    Angle phi, lambda;
    //label to add a note about the position
    String label;
    
    Position();
    Position(Angle, Angle);
    void setCartesian(String, const Cartesian, String);
    void getCartesian(Cartesian*);
    Cartesian getCartesian(void);
    void print(String, String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    bool transport(Position*, Route, String);
    bool transport_to(Route, String);
    Position antipode(void);
    Position antipode_lambda(void);
    string to_string(unsigned int);
    bool distance(Position, Length*, String, String);
    bool is_in(Route, String);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    void rotate(const String&, const Rotation&, Position*, const String&);
    
    bool operator==(const Position&);
    bool operator!=(const Position&);

};



// a three-dimensional cartesian position in the reference frame centered at the earth's origin, with the x-y axes lying on the equator and the x axis aligned with Greenwich meridian
class Cartesian{
    
public:
    
    gsl_vector* r;
    
    Cartesian();
    Cartesian(const Position&);
    void setPosition(const Position&);
    void print(String, String, ostream&);
    void operator = (const Cartesian&);
    
};


//this class defines a 'rectangular' area on the Eearth's surface: it is a rectangle with vertices p_NW, p_SE in the Mercator projection
class PositionRectangle{
    
public:
    
    //thw two Positions which constitute the vertices of the rectangle: p is the NW vertex, p_SE is the SE vertex
    Position p_NW, p_SE;
    
    PositionRectangle();
    PositionRectangle(Position, Position, String);
    bool Contains(Position);
    
};

//a derived class of String, which stores the type of a Length 
class LengthFormat: public String{
    
public:

    LengthFormat();
    LengthFormat(string);
    
};


//a derived class from String, which stores the type of a Route ("loxodrome", "orthodrome", ...)
class RouteType: public String{
    
public:

    RouteType();
    RouteType(const String&);
    RouteType(const string&);
    
};


//a derived class from String, which stores the type of a Projection ("Mercator", "3D", ...)
class Projection: public String{
    
public:

    Projection();
    Projection(string);
    
};




//if type = l or o, the parameters specifying the route are reference_position (which in this case is the start position of the Route), alpha, l. if type = c, the parameters specifying the route are reference_position (which in this case is the GP of the circle of equal altitude) and omega.
class Route{
    
public:
    RouteType type;
    String label, temp_prefix;
    //the length of the Route is expressed as the length l (if length_format.value == "length"), or as the product of the Speed v and the time t (if length_format.value == "time and speed")
    LengthFormat length_format;
    //if type = l or o -> reference_position = start position, if type = c -> reference_position = ground position
    Position reference_position, end;
    //alpha: the angle that the vector tangent to the route describes with the local meridian at start; omega: the aperture angle of the cone for circles of equal altitude
    Angle /*this is equal to alpha_notes: it is the azimuth of the vector tangent to the Route at reference_position*/Z, /*the aprture angle, valid only if *this is a circle of equal altitude*/omega;
    //the length of the route
  
    Length length;
    Speed speed;
    Chrono time;
    //this is the position in sight_list of the sight linked to route. If there is no sight linked to route, then related_sight = -1.
    Int related_sight;
    //a list of points containing the geo coordinates of points on the Route this
    Position* points;
    
    Route(void);
    Route(RouteType, Position, Angle, Length);
    Route(const RouteType&, Position, Position);
    Route(RouteType, Position, Angle);
    void print(String, String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    void set_length(double);
    void compute_end(String);
    bool compute_end(Length, String);
    int crossing(Route, vector<Position>*, double*, String);
    static double lambda_minus_pi(double, void*);
    int inclusion(Route, bool, vector<Angle>*, String);
    int inclusion(PositionRectangle, bool, vector<Angle>*, String);
    int intersection(Route, bool, vector<Angle>*, String);
    bool closest_point_to(Position*, Angle*, Position, String);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    
    void DrawOld(unsigned int, Color, int, wxDC*, DrawPanel*);
    void DrawOld(unsigned int, wxDC*, DrawPanel*, String);
    void DrawOld(unsigned int, DrawPanel*, vector< vector<wxPoint> >*, String);
    
    void Draw(unsigned int, Color, Color, int, wxDC*, DrawPanel*, String);
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
//    void enter(String, Catalog, String);
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
    vector<String> get_names(void);
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
    //the recent bodies, projections, ... selected by the user are stored here
    vector<int> recent_bodies, recent_projections, recent_length_formats, recent_route_types, recent_route_for_transport_types, recent_length_units, recent_speed_units;

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
    
    void print_recent_items(const vector<int>&, String, String, ostream&);
    void insert_recent_item(unsigned int, vector<int>*);

//    void insert_recent_body(unsigned int);
//    void print_recent_bodies(String, ostream&);
//    void insert_recent_projection(unsigned int);
//    void print_recent_projections(String, ostream&);
//    void insert_recent_length_format(unsigned int);
//    void print_recent_length_formats(String, ostream&);
//    void insert_recent_route_type(unsigned int);
//    void print_recent_route_types(String, ostream&);

    void menu(String);
    int compute_position(String);
    
};


//this class denotes a point in a generic 2D projection of the sphere
class PositionProjection{
    
public:
    //the x, y coordinates of the projected point.
    double x, y;
    
    PositionProjection();
    PositionProjection(const double, const double);
    void SetMercator(const Position&);
    bool CheckMercator(DrawPanel*);
    void NormalizeAndSetMercator(const Position&);
    PositionProjection operator+(const PositionProjection&), operator-(const PositionProjection&);
    
    
}
;

//this class defines a functor to close the app properly
class CloseApp{
    
public:
    
    MyApp* app;
    
    CloseApp(MyApp*);
    template<class T>void operator()(T&);
    
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


////this checks if an element of the ProjectionField class is valid
//template<class P> class CheckProjection{
//    
//public:
//    
//    ProjectionField<P>* p;
//    
//    CheckProjection(ProjectionField<P>*);
//    template<class T> void operator()(T&);
//    
//};


////this checks if an element of the LengthFormatField class is valid
//template<class P> class CheckLengthFormat{
//    
//public:
//    
//    LengthFormatField<P>* p;
//    
//    CheckLengthFormat(LengthFormatField<P>*);
//    template<class T> void operator()(T&);
//    
//};


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


//this functor does all the necessary tasks to be done at the end of an animated transporty: sets the non-GUI object *object_a  (for example, a Position, Route...) of type NON_GUI equal to *object_b, sets and redraws everything in the parent of type P, ...
template<class NON_GUI, class P> class ToDoAtEndOfTransport{
    
public:
    NON_GUI *object_a, *object_b;
    P* parent;

    ToDoAtEndOfTransport(NON_GUI*, NON_GUI*, P*);
    
    void operator()(void);
    
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


template<class P> class CheckSpeedValue{
    
public:
    
    SpeedField<P>* p;
    
    CheckSpeedValue(SpeedField<P>*);
    template<class T> void operator()(T&);
    
};

//template<class P> class CheckSpeedUnit{
//    
//public:
//    
//    SpeedField<P>* p;
//    
//    CheckSpeedUnit(SpeedField<P>*);
//    template<class T> void operator()(T&);
//    
//};

//template<class P> class CheckSpeed{
//    
//public:
//    
//    //p is the SpeedField which is parent of the CheckSpeed object: the CheckSpeed object checks the validity of the entries in SpeedField
//    SpeedField<P>* p;
//    CheckSpeedValue<P>* check_speed_value;
////    CheckSpeedUnit<P>* check_speed_unit;
//    
//    CheckSpeed(SpeedField<P>*);
//    template <class T> void operator()(T&);
//    
//};


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
template<class P> class CheckRouteType{
    
public:
    
    //the RouteType  field containing the Route type
    RouteTypeField<P>* p;
    
    CheckRouteType(RouteTypeField<P>*);
    template<class T> void operator()(T&);
    
    
};


//my own derived class of wxListCtrl, which contains objects of type S
template<class S> class ListControl : public wxListCtrl{
    
public:
    
    //a vector containing pointers to the buttons of *this (modify button, etc ...) which may be disabled if some event occurs
    vector<wxButton*> disableable_buttons;
    //header_width[i] = width of the header of the i-th column
    vector<int> header_width;
    
    ListControl(wxWindow*, vector<wxButton*>, const wxPoint&, const wxSize&, long);
    
    void SetColumns(vector<wxString>);
    void PushBackColumn(wxString);
    void DeselectAll(void);
    void set(vector<S>, bool);
    void EnableButtons(bool);
    void Resize(vector<S>);
    void GetSelectedItems(vector<long>*);
    
};



//if an item is selected/deselected in caller, enable/disable the disableable buttons in caller
template<class S> class OnChangeSelectionInListControl{
    
public:
    
    ListControl<S>* caller;
    //the type of listcontrol: "sight" if caller = listcontrol_sights, "position" if caller = listcontrol_positions, "route" if caller = listcontrol_routes
    String type;
    
    OnChangeSelectionInListControl(ListControl<S>*, String);
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


//this functor is called when the user has chosen that he/she wants to condfirm transporting an object
template<class P> class ConfirmTransport{
    
public:
    
    ConfirmTransport(P*);
    
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
    
    ChartFrame* parent;
    PrintMessage<DrawPanel, UnsetIdling<DrawPanel> >* print_error_message;
    wxPoint draw_panel_origin, position_plot_area_now, position_plot_area_before, drawpanel_position_end, position_start_drag, position_end_drag, /*the positions where the will be placed the labels start_label_selection_rectangle, end_label_selection_rectangle_now of the start and end point of selection_rectangle. position_end_label_selection_rectangle_now is the position of the end point of the selection rectangle now, while position_end_label_selection_rectangle_before is the position of the end point of selection rectangle at the preceeding mouse position*/ position_start_label_selection_rectangle, position_end_label_selection_rectangle_now, position_end_label_selection_rectangle_before, position_now_drag, position_draw_panel_now, /*the position of label_position_now*/ position_label_position_now, /*the position of label_dragged_object at the current step of the drag process and at the preceeding step of the drag process, respectively*/ position_label_dragged_object_now, position_label_dragged_object_before;

    //size, in pixels, of vertical and horizontal labels
    unsigned int size_label_vertical, size_label_horizontal;
    /*x_min, x_max, y_min, y_max do correspond to lambda_min, lambda_max, etc... They are ordered in such a way that x_min <= x_max and y_min <= y_max always. */
    double /*min and max values of the Mercator or 3D projections x, y*/x_min, x_max, y_min, y_max, x_min_start_drag, x_max_start_drag, y_min_start_drag, y_max_start_drag, x_span_start_drag,
    /*these are the values of x_min .. y_max and x_span when the plot is first drawn*/
    x_min_0, x_max_0, y_min_0, y_max_0, x_span_0,
    /*the coordinates of the center of the projection when scrolling starts*/ x_center_scrolling, y_center_scrolling,
    /*these are the values of x_min, ... y_max after each sliding event, corresponding to lambda_min, ... , phi_max read from file*/x_min_old, x_max_old, y_min_old, y_max_old, /*these are the angular separations in latitude and longitude between meridians and parallels, respectively */delta_lambda, delta_phi,     /*this is a quantity resulting from the 3D projection: if and only if it is >= 0, then the point under consideration in the 3d projeciton lies on the visible surface of the earth */ arg_sqrt, /*increments in longitude/latitude to draw minor ticks*/delta_lambda_minor, delta_phi_minor, lambda_span, phi_span;
    //the precision used to print out labels
    Int label_precision;
    
    //the euler angles which specify the orientation of the earth for the 3d representation
    //two auxiliary vectors which will be used later
     Cartesian r, /*vector position in the x'y'z' reference frame used for multiple purposes*/rp, /*vector position in the x'y'z' reference frame at the beginning, end and current time of mouse drag*/rp_start_drag, rp_end_drag, rp_now_drag;
    Rotation /*the orientation of the Earth at the beginning / current time / end of a drag*/rotation_start_drag, rotation_now_drag, rotation_end_drag, /*the rotation representing the current / initial orientation of the earth*/rotation, rotation_0;
    Double d, /*the distance between the plane of the 2d projection and the eye of the observer for the 3d plot, and its initial value when this is constructedd, d_0,*/ /*if the mouse hovers over a route and its y coordinate is equal to the y of the route +- (length sceen) * thickness_route_selection_over_length_screen /2, then the relative Route is highlighted in ListFrame*/thickness_route_selection_over_length_screen;
    String /*this is used to display on the chart the coordinates of a Position that is being dragged or of the reference_position of a Route that is being dragged at the current step of the drag process (label_dragged_object_now) or at the preceeding step (label_dragged_object_before)*/ label_dragged_object_now, label_dragged_object_before, /*text showing the coordinates of the current mouse position on draw_panel*/ label_position_now, label_position_before;

    bool /*this is true if the mouse is dragging with the left button pressed*/mouse_dragging, idling, /*if re_draw = true (false), then one has to draw the non-highglighteable stuff in DrawPanel (coastlines, paralles, meridians ...  but not Routes nor Positions)*/re_draw, /*this is true if the current mouse position lies in the plot area, false otherwise*/mouse_in_plot_area;
    Position /*the starting Position (ground position) of a Route if the Route is a loxodrome or orthodrome (circle of equal altitude) that I want to drag, at the beginning of the drag process (route_reference_position_drag_start), at the preceeding step of the drag process (route_reference_position_drag_start) and at the current step of the drag process (route_reference_position_drag_now) */route_reference_position_drag_start, route_reference_position_drag_before, route_reference_position_drag_now, /*starting and ending geographic position in a mouse drag process*/  geo_start_drag, geo_end_drag, /*the position on the sphere such that the vector between the center of the sphere and the position equals the direction of the rotation axis relative to a mouse drag*/rotation_axis, /*the geographic positions corresponding to the NW (SE) boundary of of the plot area, moved to the interior of the plot area by one pixel. These will be used to plot parallels and meridians in such a way that they don't hit the boundary of the plot area*/p_NW, p_SE;
    Angle rotation_angle, /*an angle containing the middle longitude/latitude of the current 3D projection, rounded up to the closest value which is a multiple of delta_lambda/phi, used for drawing things in the middle of the projection*/lambda_middle, phi_middle, /*lambda/phi_start/end are the start/end values of longidue/latitude adapted in the right form ro the loopws which draw meridians/parallels*/ lambda_start, lambda_end, phi_start, phi_end, lambda_saved, phi_saved, Z_saved;
    PositionProjection /*the values of (x, y) at the beginning/end of the selection process with a rectangle*/projection_start, projection_end;
    Route /*this is a circle of equal altitude which is the intersection between the earth's surface and the visibility cone of the observer, whose vertex is at the observer. circle_oberserver.omega is the aperture angle at the center of the earth which specifies the part of the earth surface visible by the observer. circle_observer.rerefence_position is the ground position of the observer: the intersection between the line between the center of the earth and the observer, and the surface of the earth. These exist for the 3D projection only*/circle_observer, /*the same as circle_observer, but at the initial configuration fo the chart*/circle_observer_0;
    //this rectangle represents the rectangle x_min ... y_max in the Mercator projection
    PositionRectangle rectangle_observer;
    wxBitmap m_bgbuffer;
    
    wxSizer* sizer_h, *sizer_v;
    //the i-th element of point_route_list is a list: in this list there are as many elements as the number of connected curves in which the route is cut (because of the meridian lambda = pi). In each of these elements there are the  points, with respect to the origin of DrawPanel, which represent the route_list[i] discretized
    vector< vector< vector<wxPoint> > > points_route_list_now, points_route_list_before;
    //I store in reference_positions_route_list_now (reference_positions_route_list_before) the coordinates, with respect to the origin of DrawPanel, of the reference positions of the Routes at the current (preceeding) step of a drag process
    vector<wxPoint> reference_positions_route_list_now, reference_positions_route_list_before, points_position_list_now, points_position_list_before;
    //the i-th element of this vector contains a list of critical values of the parametric angle (t) of the i-th route. At these critical values, route #i crosses the meridian lambda = pi
    //the chart contains the plot area, and the following quantities are the width and height of chart and plot area
    wxSize size_chart, size_plot_area;
    unsigned int  /*these are the values of width/height_chart when the chart is first drawn*/width_chart_0, height_chart_0, tick_length, /* gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes when setting the ticks on the x axis, and similarly for gamma_phi*/gamma_lambda, gamma_phi;
    //this is true if the label which is being drawn is the first among the parallel/meridian labels, false otherwise
    bool first_label;
    wxMemoryInputStream * memory_input_stream;
    UnsetIdling<DrawPanel>* unset_idling;
    //labels of parallels and meridians at the current and preceeding chart configuration, respectively
    vector<wxString> parallels_and_meridians_labels_now, parallels_and_meridians_labels_before;
    //positions of labels of parallels and meridians at the current and preceeding chart configuration, respectively
    vector<wxPoint> positions_parallels_and_meridians_labels_now, positions_parallels_and_meridians_labels_before;
    vector< vector< vector<wxPoint> > > /*parallels and meridians are stored in this vectors at the current step of the drag process of the chart: the i-th entry of this vector is a vector of chunks of the Route*/grid_now, /*parallels and meridians are stored in these vectors at the preceeding step in the drag process of the chart: the i-th entry of this vector is a vector of chunks of the Route*/grid_before, ticks_now, ticks_before;
    
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither DrawPanel::PreRenderMercator or DrawPanel::PreRender3D, according to my needs, and similarly for the other pointers
    void (DrawPanel::*PreRender)(void);
    bool (DrawPanel::*ScreenToProjection)(const wxPoint&, PositionProjection*);
    bool (DrawPanel::*CartesianToProjection)(const Cartesian&, PositionProjection*, bool);
    bool (DrawPanel::*ScreenToGeo)(const wxPoint&, Position*);
    bool (DrawPanel::*GeoToProjection)(const Position&, PositionProjection*, bool);
    void (DrawPanel::*Render)(wxDC*, 
                              const wxPoint&, 
                              const vector< vector< vector<wxPoint> > >&,
                              const vector< vector< vector<wxPoint> > >&,
                              const vector<wxString>&,
                              const vector<wxPoint>&,
                              const vector<unsigned long long int>&,
                              const vector<wxPoint>&,
                              wxColor,
                              wxColor,
                              double);
    bool (DrawPanel::*ProjectionToDrawPanel)(PositionProjection&, wxPoint*, bool);
    void (DrawPanel::*ProjectionToGeo)(const PositionProjection&, Position*);
    void (DrawPanel::*Set_x_y_min_max)(void);
    void (DrawPanel::*Set_lambda_phi_min_max)(void);
    void (DrawPanel::*Set_size_chart)(void);
    
    DrawPanel(ChartPanel*, const wxPoint& position_in, const wxSize& size_in);

    void SetIdling(bool);
    void PreRenderMercator(void);
    void PreRender3D(void);
    void PaintEvent(wxPaintEvent&);
//    void PaintNow(void);
    void RenderAll(wxDC&);
    void MyRefresh(void);
    void RefreshWIN32(void);
    void RenderPolygons(wxDC*,
                        const vector<unsigned long long int>&,
                        const vector<wxPoint>&,
                        wxColor,
                        wxColor,
                        double 
                        );
    void RenderBackground(wxDC&,
                          const wxPoint&, 
                          const vector< vector< vector<wxPoint> > >&,
                          const vector< vector< vector<wxPoint> > >&,
                          const vector<wxString>&,
                          const vector<wxPoint>&,
                          const vector<unsigned long long int>&,
                          const vector<wxPoint>&,
                          wxColor,
                          wxColor,
                          double);
    void RenderRoutes(wxDC&,
                      const vector< vector< vector<wxPoint> > >&,
                      const vector<wxPoint>&,
                      int,
                      wxColor
                      );
    void RenderPositions(
                         wxDC&,
                         const vector<wxPoint>&,
                         int,
                         wxColor);
    void RenderMousePositionLabel(wxDC&, const String&, const wxPoint&, wxColor, wxColor);
    void RenderDraggedObjectLabel(wxDC&, const wxPoint&, const String&, wxColor, wxColor);
    void RenderSelectionRectangle(wxDC&, const wxPoint&, const wxPoint&, const String&, wxColor, wxColor);
    void RenderSelectionRectangle(wxDC&, const Position&, const wxPoint&, const String&, wxColor, wxColor);
    void CleanAndRenderAll(void);
    void TabulateRoutes(void);
    void TabulatePositions(void);
    void FitAll();
    
    bool AdjustLatitudeLongitude3D(const int&, const int&, int*, int*);
    bool ScreenToDrawPanel(const wxPoint&, wxPoint*);
    void GeoToScreen(const Position&, wxPoint*);
    bool DrawPanelToGeo(const wxPoint&, Position*);
    bool ScreenToMercator(const wxPoint&, PositionProjection*);
    bool ScreenTo3D(const wxPoint&, PositionProjection*);
    bool GeoToDrawPanel(const Position&, wxPoint*, bool);
    bool CartesianToDrawPanel(const Cartesian&, wxPoint*, bool);
    bool GeoTo3D(const Position&, PositionProjection*, bool);
    bool CartesianTo3D(const Cartesian&, PositionProjection*, bool);
    bool CartesianToMercator(const Cartesian&, PositionProjection*, bool);
    bool GeoToMercator(const Position&, PositionProjection*, bool);
    bool ScreenToGeo_Mercator(const wxPoint&, Position*);
    bool ScreenToGeo_3D(const wxPoint&, Position*);
    bool ProjectionToDrawPanel_Mercator(PositionProjection&, wxPoint*, bool);
    bool ProjectionToDrawPanel_3D(PositionProjection&, wxPoint*, bool);
    void ProjectionToGeo_Mercator(const PositionProjection&, Position*);
    void ProjectionToGeo_3D(const PositionProjection&, Position*);
    void ShowCoordinates(const Position&, String*);
    void SetLabelAndPosition(const wxPoint&, wxPoint*, String*);
    void SetLabelAndPosition(const Position&, wxPoint*, String*);
    void SetLabelAndAdjustPosition(const Position&, wxPoint*, String*);
    void Set_lambda_phi_min_max_Mercator(void);
    void Set_lambda_phi_min_max_3D(void);
    void Set_x_y_min_max_Mercator(void);
    void Set_x_y_min_max_3D(void);
    void Set_size_chart_Mercator(void);
    void Set_size_chart_3D(void);
    bool PutBackIn(wxPoint, wxPoint*);
    double x_span(void);
    Rotation rotation_start_end(const wxPoint&, const wxPoint&);

    void Render_Mercator(wxDC*, 
                         const wxPoint&,
                         const vector< vector< vector<wxPoint> > >&,
                         const vector< vector< vector<wxPoint> > >&,
                         const vector<wxString>&,
                         const vector<wxPoint>&,
                         const vector<unsigned long long int> &,
                         const vector<wxPoint>&,
                         wxColor,
                         wxColor,
                         double);
    void Render_3D(wxDC*, 
                   const wxPoint&,
                   const vector< vector< vector<wxPoint> > >&,
                   const vector< vector< vector<wxPoint> > >&,
                   const vector<wxString>&,
                   const vector<wxPoint>&,
                   const vector<unsigned long long int> &,
                   const vector<wxPoint>&,
                   wxColor,
                   wxColor,
                   double);
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
    template<class E> void SetProjection(E&);
    void KeyDown(wxKeyEvent&);
    
//    DECLARE_EVENT_TABLE()
};

//this class is the parent of all classes for GUI fields which include a list of multiple items to  be selected (such as ProjectionField, LengthFormatField, ...). All members and methods which are common to these children classes are written in this parent class. P is the type of the frame that hosts *this. NON_GUI is the type of the non-GUI field related to *this: For example, if I create a  class BodyField derived from MultipleItemField, then NON_GUI = Body; if I create a derived class LengthFormatField derived from MultipleItemField, NON_GUI = LengthFormat, etc. If there is no non-GUI object associated with *this, I set NON_GUI = void and call the constructor of MultipleItemField with second argument equal to NULL. CHECK is the type of the functor that will be used to check whetehr *this is valid: for example, for a BodyField CHECK = CheckBody<P>. If the check functor is not defined -> CHECK = void
template<class P, class NON_GUI, class CHECK> class MultipleItemField{
    
public:
    
    P* parent;
    //the non-GUI object related to *this: for example, if I create a  class BodyField derived from MultipleItemField, then *object will be a Body object
    NON_GUI* object;
    //a pointer to the functor that will be used to check whether *this is valid
    CHECK* check;
    wxBoxSizer *sizer_h, *sizer_v;
    wxArrayString  /*this is equal to a standard list of the available types*/items, /*a fixed catalog of the items in this MultipleChoiceField*/catalog;
    //I store the value in name, before name starts being edited, in value_before_editing
    wxString value_before_editing;
    //this is the wxComboBox with the name of the projections
    wxComboBox* name;
    //this points to an external vector<int> where the recent items are stored: the Fill() method  reads from *recent_items
    vector<int>* recent_items;
    bool ok, /*this is true if *this is being edited, false otherwise*/editing;
//    template<class T> void (MultipleItemField::*OnChooseItem)(T&);


    MultipleItemField(wxPanel*, NON_GUI*, const vector<String>&, vector<int>*);
    void Fill(void);
    void set(void);
    template<class E> void Get(E&);
    void Enable(bool);
    bool is_ok(void);
    template<class E> void Check(E&);
//    void read_recent_projections(void);
    template<class T> void InsertIn(T*);
    template<class T> void InsertIn(T*, wxSizerFlags&);
//    template<class E> void OnEdit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    void SetToolTip(const String&);
    
    
};


template<class P> class ProjectionField: public MultipleItemField<P, Projection, void>{
    
public:
    
//    P* parent;
//    wxBoxSizer *sizer_h, *sizer_v;
//    wxArrayString  /*this is equal to a standard list of the available graphical types*/items, /*same as projections, but it is fixed and never wrote to: it is a fixed catalog*/catalog;
    //this is the wxComboBox with the name of the projections
//    wxComboBox* name;
//    vector<int> recent_items;
//    bool ok;
//    CheckProjection<P>* check;

    ProjectionField(wxPanel*, vector<int>*);
//    void Fill(void);
//    void read_recent_projections(void);
//    template<class T> void InsertIn(T*, wxSizerFlags&);
    template<class E> void OnEdit(E&);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


//this class defines a dropdown menu (wxComboBox) that lets the user choose in what format to express lengths, i.e., simply as a LengthField or as a ChronoField + a SpeedField (l = t * v). P is the type of parent in which *this is inserted
template<class P> class LengthFormatField: public MultipleItemField<P, LengthFormat, void>{
    
public:
    
//    P* parent;
//    wxBoxSizer *sizer_h, *sizer_v;
//    wxArrayString  /*this is equal to a standard list of the available formats for a length*/items, /*same as length_formats, but it is fixed and never wrote to: it is a fixed catalog*/catalog;
    //this is the wxComboBox with the name of the length formats
//    wxComboBox* name;
    //this points to a LengthFormat object, which contains the date written in the GUI field of this
//    vector<int> recent_items;
//    bool ok;
//    LengthFormat* length_format;
//    CheckLengthFormat<P>* check;

    LengthFormatField(wxPanel*, LengthFormat*, vector<int>*);
//    void Fill(void);
//    void set(void);
//    template<class T> void InsertIn(T*);
    template<class E> void OnEdit(E&);
//    void Enable(bool);
//    bool is_ok(void);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


//P is the type of the frame which hosts *this
template<class P> class BodyField : public MultipleItemField<P, Body, CheckBody<P>>{
    
public:
    //the parent frame to which this object is attached
//    SightFrame* parent;
//    wxArrayString  /*this is equal to a standard list of the available bodies, except that on top of the list are the recently selected bodies*/items;
    //this points to a Body object, which contains the date written in the GUI field of this
//    Body* object;
    Catalog* catalog;
//    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxComboBox with the name of the bodies
//    wxComboBox* name;
//    CheckBody<P>* check;
//    bool ok;
    
    BodyField(wxPanel*, Body*, Catalog*, vector<int>*);
//    void fill_bodies(void);
//    void read_recent_bodies(void);
//    void update_recent_bodies(void);
    void set(void);
    template<class T> void get(T&);
//    template<class T> void InsertIn(T*);
//    bool is_ok(void);
    template<class E> void OnEdit(E&);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
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

//class for graphical object: a field to enter a length, composed of a box and a dropdown menu to enter the units of measure of the length. P is the type of the parent which hosts the LengthField object
template<class P> class LengthField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    //the length value
    wxTextCtrl *value;
    //units of measure of the Length
    LengthUnitField<P>* unit;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;
    //the units of measure of the length in this GUI field
    String unit_value;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool value_ok, unit_ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    CheckLength<P>* check;
    
    LengthField(wxPanel*, Length*, String);
    void set(Length);
    void set(void);
    template<class T> void get(T&);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditValue(E&);
    template<class E> void OnEditUnit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


//class for graphical object: a field to enter a speed, composed of a box and a dropdown menu to enter the units of measure of the speed. P is the type of the parent which hosts the LengthField object
template<class P> class SpeedField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    //the speed value
    wxTextCtrl *value;
    //units of measure of the Length
    SpeedUnitField<P>* unit;
    wxBoxSizer *sizer_h, *sizer_v;
    Speed* speed;
    //an array containing all possible units of measure
//    wxArrayString units;
    //the units of measure of the length in this GUI field
    String unit_value;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool value_ok, unit_ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
//    CheckSpeed<P>* check;
    CheckSpeedValue<P>* check_speed_value;

    
    SpeedField(wxPanel*, Speed*, String);
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


//a GUI field containing a time, which is contained by an object of type P
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
    StaticText* static_text_1, *static_text_2, *static_text_3;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Chrono* chrono;
    //hour_ok = true if the hour is formatted properly and set to the same value as chrono->h, and similarly for the other variables
    bool hour_ok, minute_ok, second_ok;
    CheckChrono<P>* check;
    
    ChronoField(wxPanel*, Chrono*);
    void set(void);
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


//GUI field to specify the RouteType, which is inserted into a parent panel of type P
template<class P> class RouteTypeField: public MultipleItemField<P, RouteType, CheckRouteType<P> >{
    
public:
    
    
    RouteTypeField(wxPanel*, RouteType*, const vector<String>&, vector<int>*);
    void set(void);
    template<class E> void OnEdit(E&);
    void OnEdit();
    
};


//GUI field to specify the unit of measure of a Length, which is inserted into a parent panel of type P
template<class P> class LengthUnitField: public MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >{
    
public:
    
    LengthUnitField(wxPanel*, LengthUnit*, vector<int>*);
    
};

//GUI field to specify the unit of measure of a Speed, which is inserted into a parent panel of type P
template<class P> class SpeedUnitField: public MultipleItemField<P, SpeedUnit, void >{
    
public:
    
    SpeedUnitField(wxPanel*, SpeedUnit*, vector<int>*);
    
};




//this class is to define the functor template<class T> void operator()(T&), which is called when the user selects a route with which he wants to transport a sight
class OnSelectRouteInListControlRoutesForTransport{
    
public:
    
    //parent frame
    ListFrame* parent;
    //    UnsetIdling<ListFrame>* unset_idling;
    
    //constructor, which sets the parent frame
    OnSelectRouteInListControlRoutesForTransport(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};

//this class is to define the functor template<class T> void operator()(T&), which is called when the user crates a new route with which he wants to transport a sight
class OnNewRouteInListControlRoutesForTransport{
    
public:
    
    //parent frame
    ListFrame* parent;
    
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
    Route transporting_route_saved;
    wxMenuBar *menu_bar;
    wxMenu *menu_app, *menu_file, *menu_chart, *menu_new_chart, *menu_item_mercator, *menu_item_3d;
    ListControl<Sight> *listcontrol_sights;
    ListControl<Route>* listcontrol_routes;
    ListControl<Position>* listcontrol_positions;
//    //this is used to transport objects listed in *this
//    GraphicalFeatureTransportHandler* transport_handler;
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
    wxPoint /*the instantaneous positions of the mouse with respect to the screen: this position is kept to the right value by DrawPanel::OnMouseMovement method, that is called every time the mouse moves. This variable belongs to ListFrame rather than to single ChartFrames or DrawPanel, because there is a unique screen position for all of them*/ screen_position_now, screen_position_before;
    OnChangeSelectionInListControl<Sight>* on_change_selection_in_listcontrol_sights; OnChangeSelectionInListControl<Route>* on_change_selection_in_listcontrol_routes; OnChangeSelectionInListControl<Position>* on_change_selection_in_listcontrol_positions;
    ExistingRoute *existing_route;
    NewRoute *new_route;
    //a temporary value of data->crossing_route_list
    vector<Route> crossing_route_list_temp;
    bool /*this is true if the mouse is moving, and false otherwise*/mouse_moving, /*this is true if a selection rectangle is being drawn, and false otherwise */selection_rectangle, /*this is true/false if highlighting of routes and sights is enabled/disables*/enable_highlight, /*idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false*/ idling, /*this is equal to true if file has been modified, false otherwise*/file_has_been_modified, /*this is equal to true if the file has no name, false otherwise*/file_is_untitled, /*this is true if I am computing the astronomical position, false otherwise*/selecting_route_for_position, /*this is equal to true (false) if the user has (has not) pressed cancel while charts were loading */abort, /*if this is true, I am transporting an object with a new Route, otherwise it is false*/ transporting_with_new_route, /*if this is true, I am transporting an object with an existing Route, otherwise it is false*/ transporting_with_selected_route, /*this is true if a Route or Position is being dragged, and false otherwise*/ dragging_object,    /*this is true if the highlighted Route / Position is being changed and false otherwise */  changing_highlighted_object;

    Answer /*if this is y/n, the coastlines are shown/not shown*/show_coastlines, /*if this is y/n, sample_sight.nav is loaded/not loaded at startup*/ load_sample_sight;
    //the file where the data is read and written
    FileRW data_file;
    unsigned int margin;
    int /*the # of the sight/route/position which is highlighted at the current (_now) or preceeding (_before) step of mouse movement, because the mouse is hovering over it in listcontrol_sights/routes/positions*/highlighted_sight_now, highlighted_route_now, highlighted_route_before,  highlighted_position_now, highlighted_position_before, /*# of the object to transport or disconnect */i_object_to_transport, i_object_to_disconnect, i_transporting_route;
    /*map[i] is the position in data->route_list of the i-th Route in route_list_for_transport*/
    vector<int> map;
    //coastline_polygons_Position/Cartesian/Mercator[i] is a vector which contains the the coastline datapoints (in Position/Cartesian/Mercator projection format) of polygon #i
    vector< vector<Position> > coastline_polygons_Position;
    vector< vector<Cartesian> > coastline_polygons_Cartesian;
    vector< vector<PositionProjection> > coastline_polygons_Mercator;
    //coastline_polygons_map[i][j] contans a list of polygon ids (i.e., an unsigned int denoting an entry in coastline_polygons_Position) that denote all polygons that fall within latitude / longitude i + floor_min_lat, j
    vector< vector< vector<unsigned long long int> > > coastline_polygons_map;
    //the total number of coastline points present in the coastline data file
    unsigned long long int n_all_coastline_points;
    //this vector containts the ids (as written in coastline_polygons_Position/Cartesian) of the polygons which have at least one point falling into rectangle_observer (Mercator projection) or circle_observer (3D projection), and it is updated every time rectangle_observer changes
    vector<unsigned long long int> coastline_polygons_area_observer;
    Position /*these are the geographic Positions where the right mouse button is clicked at the beginning, the current and preceeding Position during mouse movement, and the mouse Position and at the end of the drawing process for the selection rectangle*/geo_position_start, geo_position_before, geo_position_now, position_end;
    //the object which is being transported : a "sight" or a "position"
    String transported_object_type, /*the labels that will be drawn on position_start_label_selection_rectangle and position_end_label_selection_rectangle_now, respectively. end_label_selection_rectangle_now is the label of the end point of selection rectangle now, while end_label_selection_rectangle_before is the label of the end point of selection rectangle at the preceeding mouse position*/start_label_selection_rectangle, end_label_selection_rectangle_now, end_label_selection_rectangle_before;
    
    //a functor to set/unset idling mode in *this
    SetIdling<ListFrame>* set_idling;
    UnsetIdling<ListFrame>* unset_idling;
    ConfirmTransport<ListFrame>* confirm_transport;
    CloseFrame<ListFrame>* close;
    //a functor to let the user select a Route in listcontrol_routes
    SelectRoute* select_route;
    PrintMessage<ListFrame, UnsetIdling<ListFrame> >* print_warning_message, *print_error_message;
    PrintMessage<ListFrame, SelectRoute >* print_info_message;
    PrintQuestion< ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame> >* print_question_message;
    
    OnSelectRouteInListControlRoutesForTransport* on_select_route_in_listcontrol_routes_for_transport;
    OnNewRouteInListControlRoutesForTransport* on_new_route_in_listcontrol_routes_for_transport;
    AskRemoveRelatedSight* ask_remove_related_sight;
    AskRemoveRelatedRoute* ask_remove_related_route;
    
    ListFrame(const wxString&, const wxString&, const wxPoint&, const wxSize&, String);
    
    void set(void);
    void DrawAll(void);
    void MyRefreshAll(void);
    void RefreshAll(void);
    void TabulatePositionsAll(void);
    void TabulateRoutesAll(void);
    void Resize(void);
    
    void LoadCoastLineData(String);
    
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
    template<class E> void DisconnectAndPromptMessage(E&);
    
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
//    StaticText* text_time_interval_not_ok;
    //image shown on SightFrame if the time interval entered by the user does not lie within the interval covered by the ephemerides' data
//  ?  wxStaticBitmap* image_time_interval_status;
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
    template<class E> void update_recent_items(E&);

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
    
    ListFrame* parent;
    Route* route;
    //this long represents the position in the list (this->GetParent())->listcontrol_routes of route. If list_position = -1, then route is not in that list
    long position_in_listcontrol_routes;
    wxPanel *panel;
    bool /*idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false*/idling, /*for_transport = true (false) means that the Route is meant to transport objects*/for_transport;
    
    SetIdling<RouteFrame> * set_idling;
    UnsetIdling<RouteFrame> * unset_idling;
    PrintMessage<RouteFrame, UnsetIdling<RouteFrame> >* print_error_message;
    RouteTypeField<RouteFrame>* type;
    AngleField<RouteFrame> *Z, *omega, *start_phi, *start_lambda, *GP_phi, *GP_lambda;
    //l_format tells whether the length of Route is written simply as a Length, or as a Speed x a Chrono (a time)
    LengthFormatField<RouteFrame>* length_format;
    //if the length of the Route is written directly as a length, this field is used
    LengthField<RouteFrame> *length;
    //if the lenght of the Route is written in terms of a speed multiplied by a time, the following two fields are used
    ChronoField<RouteFrame> *time;
    SpeedField<RouteFrame> *speed;
    StringField<RouteFrame> *label;
    
    wxFlexGridSizer *sizer_grid_type, *sizer_grid_Z, *sizer_grid_t_v, *sizer_grid_l, *sizer_grid_omega, *sizer_grid_start, *sizer_grid_GP, *sizer_grid_label;
    wxBoxSizer *sizer, /*this sizer containts the Length format GUI field, the Length GUI field and the speed and time GUI fields*/*sizer_l_format_l_t_v, *box_sizer;
    wxStaticBoxSizer *sizer_box_data, *sizer_box_l_format_l_t_v, *sizer_box_l, *sizer_box_t_v, *sizer_box_start, *sizer_box_GP;
    //static texts for the GUI fields containing the Route length, speed and time
    wxStaticText *text_time, *text_speed, *text_length;
    wxButton* /*this button triggers either the addition of a new Route, or the modification of an existing one*/button_ok, *button_cancel;
    
    RouteFrame(ListFrame*, Route*, bool, long, const wxString&, const wxPoint&, const wxSize&, String);
    void set(void);
    template<class T> void get(T&);
    template<class E> void OnChooseLengthFormatField(E&);
    template<class E> void UpdateLength(E&);
    void OnChooseLengthFormatField();
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    bool is_ok(void);
    void AllOk(void);
    void KeyDown(wxKeyEvent&);
    
    //    wxDECLARE_EVENT_TABLE();
    
};



class ChartFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    StaticText *text_slider;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxGridSizer* sizer_buttons;
    wxStaticBitmap* image;
    wxSlider* slider;
    wxBitmapButton* button_show_list;
    wxButton* button_up, *button_down, *button_left, *button_right, *button_reset;
    ProjectionField<ChartFrame>* projection;
    PrintMessage<ChartFrame, UnsetIdling<ChartFrame> >* print_error_message;
    /*the latitude/longitude setting the boundaries of the plotted area on earth*/Angle phi_min, phi_max, /*it is not necessarily true that lambda_min < lambda_max: lambda_min(max) correspond to the left(right) edge of the plot area*/lambda_min, lambda_max;
    //a vector of the points of the coastlines in DrawPanel coordinates: these are points of polygons concatenated in the linear arrays coastline_polygons_now/ before. The i-th polygon is stored n coastline_polygons_now/before[polygon_position_now/before[i]], coastline_polygons_now[polygon_position_now/before[i]+1], ..., coastline_polygons_now[polygon_position_now/before[i+1]-1]
    vector<wxPoint> coastline_polygons_now, coastline_polygons_before;
    vector<unsigned long long int> polygon_position_now, polygon_position_before;

    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling, /*this is true if the user is currently scrolling*/mouse_scrolling, /*this is true if the chart is being dragged, and thus the size of *this must not change across multiple Draw(s), and false otherwise*/ dragging_chart;
    //This is the actual value of the maximal zoom factor allowed
    Double /*the zoom factor relative to the default configuration of either projection, the zoom factor  is not necessarily equal to the numerical value (slider->GetValue()) shown on the slider*/zoom_factor;
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither ChartFrame::UpdateSliderLabel_Mercator or ChartFrame::UpdateSliderLabel_3D, according to my needs, and similarly for the other pointers
    void (ChartFrame::*UpdateSliderLabel)(void);
    //the transport handler used to transport the chart in *this
    ChartTransportHandler* chart_transport_handler;
    
    ChartFrame(ListFrame*, Projection, const wxString&, const wxPoint&, const wxSize&, String);

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
    void EnableAll(bool);
    
    template<class T> void MoveNorth(T&);
    template<class T> void MoveSouth(T&);
    template<class T> void MoveWest(T&);
    template<class T> void MoveEast(T&);
    template<class T> void KeyDown(T&);
    template<class T> void Reset(T&);
    template<class T> void ResetRender(T&);
    template<class T> void ResetRenderAnimate(T&);
    void Animate(void);
    
    //this is created for testing
    UnsetIdling<ChartFrame> *unset_idling;
    
    
    
    
};

class ChartPanel : public wxPanel{
    
public:
    
    ChartFrame* parent;
    
    ChartPanel(ChartFrame*, const wxPoint&, const wxSize&);
    
};

//a hanlder to make a general motion (i.e., drag the chart, transport a Position, ...) with an animation
class MotionHandler{
    
public:
    
    //the functor that calls *this
    ListFrame* parent;
    wxTimer* timer;
    //the position during the transport process at 'time' t
    Position start;
    //transporting_route is the Route used to do the transport. At each step of the transport, transporting_route_temp is set to be a part of the full Route (transporting_route) used to do the transport
    Route transporting_route, transporting_route_temp;

    //a counter of the step in the animation, running from 0 to n_animation_steps
    long t;
    
    MotionHandler(ListFrame*);
    
};


//a hanlder to transport a non-GUI object of type NON_GUI (NON_GUI may be equal to Position, Route, ...)  with an animation. When the transport is over, this may exectued a functor of type F. If nothing is supposed to be exectued at the end of the transport, set F = void
template <class NON_GUI, class F> class GraphicalFeatureTransportHandler: public MotionHandler{
    
public:
    
    NON_GUI* transported_object;
    //the type of the transported_object that is being transported (String("position") or String("route"), ...)
    String type_of_transported_object;
    F* f;
    
    GraphicalFeatureTransportHandler(ListFrame*, NON_GUI*, const String&, const Route&, F*);
    void operator()(void);

    void OnTimer(wxTimerEvent&);
    
};


//a hanlder to move the chart of a given ChartFrame with an animation
class ChartTransportHandler: public MotionHandler{
    
public:
    
    //a poitner to the ChartFrame whose chart is being moved
    ChartFrame* chart_frame;
    //the size (in Mercator projection x,y) of the projection in the mercator projection: this is stored and used during the transport in such a way that the size of the projection stays the same through the transport
    PositionProjection projection_size;
    Position p_NE, p_SW;
    
    ChartTransportHandler(ChartFrame*, const Route&, const Double&);
    void operator()(void);
    void OnTimer(wxTimerEvent&);
    
};
