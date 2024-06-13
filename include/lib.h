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

//REMOVE AT THE END IF NOT NEEDED
#include "angle.h"
#include "body.h"
#include "static_text.h"
#include "double.h"
#include "int.h"
#include "file.h"
#include "file_r.h"
#include "file_rw.h"
#include "limb.h"
#include "position.h"
//REMOVE AT THE END IF NOT NEEDED


//#include "chartdir.h"


template<class P> class BodyField;
template<class P, class NON_GUI, class CHECK> class MultipleItemField;
template<class P> class ProjectionField;
template<class P> class LengthFormatField;
template<class P> class LengthUnitField;
template<class P> class SpeedUnitField;
template<class P, class T> class CheckField;
template<class P> class AngleField;
template<class P> class LengthField;
template<class P> class DynamicLengthField;
template<class P> class StaticLengthField;
template<class P> class SpeedField;
template<class P> class DateField;
template<class P> class ChronoField;
template<class P> class RouteTypeField;
template<class P> class StringField;
class MyApp;
template<class P> class LimbField;
template<class F> class CloseFrame;
template<class F> class ShowFrame;
class ShowAll;
template<class F_A, class F_B, class F_ABORT> class QuestionFrame;
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
class Int;
class PositionProjection;
//template<class P> class CheckProjection;
template<class P> class CheckArcDegree;
template<class P> class CheckArcMinute;
template<class P> class CheckLengthFormat;
template<class P> class TabulateDays;
template<typename FF_OK> class MessageFrame;

template<class F> class MotionHandler;
template<class F> class ChartTransportHandler;


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








#include "units.h"
#include "constants.h"




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
template<typename F_A, typename F_B, typename F_ABORT> class QuestionFrame: public wxFrame{
    
public:
        //the non-GUI object connected to the GUI object MessageFrame
    wxPanel *panel;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_buttons;
    wxGridSizer* sizer_grid;
    wxButton* button_a, *button_b;
    bool enable_button_a, enable_button_b, /*if this is true, then pressing esc will be equivalent to pressing button b, if this is false, pressing esc will call *f_abort*/bind_esc_to_button_b;
    StaticBitmap* image;
    //initialize the functor to close thie QuestionFrame when button_a or button_b will be pressed
    CloseFrame<QuestionFrame>* close_frame;
    //pointer to the class containing the functor which will be called when the button yes is pressed
    F_A* f_a;
    //pointer to the class containing the functor which will be called when the button no is pressed
    F_B* f_b;
    //pointer to the class containing the functor which will be called to abort
    F_ABORT* f_abort;
    String string_a, string_b;
    
    QuestionFrame(wxWindow*, F_A*, String, F_B*, String, F_ABORT*, bool, bool, bool, const wxString&, const wxString&,  String, const wxPoint&, const wxSize&, String);
    template<class E> void KeyDown(E&);
    
};











////a non-GUI object containing a Length
//class Length{
//    
//public:
//    //the value of the Length
//    double value;
//    //the unit of measure of the Length
//    LengthUnit unit;
//    
//    Length();
//    Length(double);
//    Length(double, const LengthUnit&);
//    Length(Chrono, Speed);
//    
//    void set(String, double, String);
//    void set(double);
//    void set(double, const LengthUnit&);
//    string to_string(const LengthUnit&, unsigned int);
//    string to_string(unsigned int);
//    void print(String, String, ostream&);
//    void convert_to(const LengthUnit&);
//    Length convert(const LengthUnit&);
//    template<class S> void read_from_stream(String, S*, bool, String);
//    void read_from_file_to(String, String, String, String);
//    bool check(String, String);
//    bool operator > (const Length&), operator >= (const Length&), operator <= (const Length&), operator > (const double&), operator >= (const double&), operator <= (const double&), operator < (const Length&), operator < (const double&), operator == (const Length&), operator != (const Length&);
//    Length operator + (const Length&), operator - (const Length&),  operator * (const double&), operator / (const double&);
//    void operator += (const Length&), operator -= (const Length&), operator *= (const double&), operator /= (const double&);
//    
//};



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
    void set(const Rotation&);
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





//this class defines a 'rectangular' area on the Eearth's surface: it is a rectangle with vertices p_NW, p_SE in the Mercator projection
class PositionRectangle{
    
public:
    
    //thw two Positions which constitute the vertices of the rectangle: p is the NW vertex, p_SE is the SE vertex
    Position p_NW, p_SE;
    
    PositionRectangle();
    PositionRectangle(Position, Position, String);
    bool Contains(Position);
    bool SizeMercator(PositionProjection*);
    
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
    vector<Length> /*heights delimiting the level of the US 1976 atmosphere model, see https://en.wikipedia.org/wiki/U.S._Standard_Atmosphere, expressed in units of LengthUnit_types[0] */h;
    vector<double> /*dT/dz, ecpressed in units of K / nm*/lambda, t;
    void set(void);
    double T(Length), n(Length), dTdz(Length), dndz(Length);
    
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
    PositionProjection operator + (const PositionProjection&), operator - (const PositionProjection&), operator / (const double &), operator / (const Double&), operator * (const double &);
    void operator += (const PositionProjection&), operator -= (const PositionProjection&), operator *= (const double&), operator /= (const double&);
        
};

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








template<class P, class T> class CheckCheck{
    
public:
    
    CheckField<P,T>* p;
    
    CheckCheck(CheckField<P,T>*);
    //this functor checks whether a GUI Check field is filled correctly and writes its value into the relative non-GUI field
    template<class R> void operator()(R&);
    
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
    
    DynamicLengthField<P>* p;
    
    CheckLengthValue(DynamicLengthField<P>*);
    template<class T> void operator()(T&);
    
};

template<class P> class CheckLengthUnit{
    
public:
    
    DynamicLengthField<P>* p;
    
    CheckLengthUnit(DynamicLengthField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckLength{
    
public:
    
    //p is the DynamicLengthField which is parent of the CheckLength object: the CheckLength object checks the validity of the entries in DynamicLengthField
    DynamicLengthField<P>* p;
    CheckLengthValue<P>* check_length_value;
    CheckLengthUnit<P>* check_length_unit;
    
    CheckLength(DynamicLengthField<P>*);
    template <class T> void operator()(T&);
    
};


template<class P> class CheckSpeedValue{
    
public:
    
    SpeedField<P>* p;
    
    CheckSpeedValue(SpeedField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckSpeedUnit{
    
public:
    
    SpeedField<P>* p;
    
    CheckSpeedUnit(SpeedField<P>*);
    template<class T> void operator()(T&);
    
};


template<class P> class CheckSpeed{
    
public:
    
    //p is the SpeedField which is parent of the CheckSpeed object: the CheckSpeed object checks the validity of the entries in SpeedField
    SpeedField<P>* p;
    CheckSpeedValue<P>* check_speed_value;
    CheckSpeedUnit<P>* check_speed_unit;
    
    CheckSpeed(SpeedField<P>*);
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


//a functor that checks if a Route type is valid
template<class P> class CheckRouteType{
    
public:
    
    //the RouteType  field containing the Route type
    RouteTypeField<P>* p;
    
    CheckRouteType(RouteTypeField<P>*);
    template<class T> void operator()(T&);
    
};


//a functor that checks if a LengthFormat  is valid
template<class P> class CheckLengthFormat{
    
public:
    
    //the LengthFormat  field containing the Route type
    LengthFormatField<P>* p;
    
    CheckLengthFormat(LengthFormatField<P>*);
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
template<class T, typename F_YES, typename F_NO, typename F_ABORT> class ShowQuestionFrame{
    
public:
    
    T* f;
    wxControl* control;
    String title, message, question, answer_y, answer_n;
    F_YES* f_yes;
    F_NO* f_no;
    F_ABORT* f_abort;
    QuestionFrame<F_YES, F_NO, F_ABORT>* question_frame;
    bool /*these are true/false if the yes/no button are enabled/disabled, respectively*/ enable_button_a, enable_button_b, /*if this is true, then pressing esc will be equivalent to pressing button b, if this is false, pressing esc will call *f_abort*/ bind_esc_to_button_b;
    
    ShowQuestionFrame(T*, F_YES*, F_NO*, F_ABORT*);
    
    void SetAndCall(wxControl*, String, String, String, String);
    void SetAndCall(wxControl*, String, String, String, String, bool, bool, bool);
    void EnableDisableButtons(void);
    void operator()(void);
    
    
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


//this class defines a dropdown menu (wxComboBox) that lets the user choose in what format to express lengths, i.e., simply as a DynamicLengthField or as a ChronoField + a SpeedField (l = t * v). P is the type of parent in which *this is inserted
template<class P> class LengthFormatField: public MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >{
    
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





//this class is for a GUI field including a length
template<class P> class LengthField{
    
public:
    
    //the parent frame to which this object is attached
    P* parent;
    //unit of measure of the Length written in *this
    LengthUnitField<P>* unit;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;

    LengthField(wxPanel*, Length*);
    
    template<class E> void OnEditUnit(E&);
    template<class T> void InsertIn(T*);
    template<class T> void InsertIn(T*, wxSizerFlags&);
    
};


//an editable GUI field to enter a Length, composed of a box and a dropdown menu to enter the units of measure of the length. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class DynamicLengthField: public LengthField<P>{
    
public:

    //the length value
    wxTextCtrl *value;
    CheckLength<P>* check;
    bool value_ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    
    DynamicLengthField(wxPanel*, Length*);
    
    void set(Length);
    void set(void);
    template<class T> void get(T&);
    void Enable(bool);
    bool is_ok(void);
    template<class E> void OnEditValue(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};


//a non-editable GUI field to display a Length, composed of a static text and a dropdown menu to enter the units of measure of the length. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class StaticLengthField: public LengthField<P>{
    
public:
    
    //the length value
    StaticText* value;
    
    StaticLengthField(wxPanel*, Length*/*, String*/);
    
    void set(const Length&);
    void set(void);
    void set_value_keep_unit(void);
    template<class E> void ConvertUnit(E&);

};


//class for graphical object: a field to enter a speed, composed of a box and a dropdown menu to enter the units of measure of the speed. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class SpeedField{
    
public:
    //the parent frame to which this object is attached
    P* parent;
    //the speed value
    wxTextCtrl *value;
    //unit of measure of the Speed written in *this
    SpeedUnitField<P>* unit;
    wxBoxSizer *sizer_h, *sizer_v;
    Speed* speed;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool value_ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    CheckSpeed<P>* check;

    
    SpeedField(wxPanel*, Speed*);
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
template<class P> class SpeedUnitField: public MultipleItemField<P, SpeedUnit, CheckLengthUnit<P> >{
    
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
    //this long represe5nts the position in the list (this->GetParent())->listcontrol_routes of route. If list_position = -1, then route is not in that list
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
    DynamicLengthField<RouteFrame> *length;
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
    template<class E> void AllOk(E&);
    void KeyDown(wxKeyEvent&);
    template<class E> void Check(E&);
    void Check(void);
        
};



class ChartFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    StaticText *chart_scale;
    //a static text displaying the height of the observer 
    StaticLengthField<ChartFrame>* observer_height;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxGridSizer* sizer_buttons;
    wxStaticBitmap* image;
    wxSlider* slider;
    wxBitmapButton *button_show_list, *button_reset;
    wxButton* button_up, *button_down, *button_left, *button_right;
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
    template<class T> void AllOk(T&);
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
template<class F> class MotionHandler{
    
public:
    
    //the functor that calls *this
    ListFrame* parent;
    //the eventual functor to be called at the end of tha motion animation
    F* f;

    wxTimer* timer;
    //the position during the transport process at 'time' t
    Position start;
    //transporting_route is the Route used to do the transport. At each step of the transport, transporting_route_temp is set to be a part of the full Route (transporting_route) used to do the transport
    Route transporting_route, transporting_route_temp;

    //a counter of the step in the animation, running from 0 to n_animation_steps
    long t;
    
    MotionHandler(ListFrame*, const Route&, F*);
    
};


//a hanlder to transport a non-GUI object of type NON_GUI (NON_GUI may be equal to Position, Route, ...)  with an animation. When the transport is over, this may exectued a functor of type F. If nothing is supposed to be exectued at the end of the transport, set F = void
template <class NON_GUI, class F> class GraphicalFeatureTransportHandler: public MotionHandler<F>{
    
public:
    
    NON_GUI* transported_object;
    //the type of the transported_object that is being transported (String("position") or String("route"), ...)
    String type_of_transported_object;
    
    GraphicalFeatureTransportHandler(ListFrame*, NON_GUI*, const String&, const Route&, F*);
    void operator()(void);

    void OnTimer(wxTimerEvent&);
    
};


//a hanlder to move the chart of a given ChartFrame with an animation
template<class F> class ChartTransportHandler: public MotionHandler<F>{
    
public:
    
    //a poitner to the ChartFrame whose chart is being moved
    ChartFrame* chart_frame;
    
    PositionProjection /*the size (in Mercator projection x,y) of the projection in the mercator projection: this is stored and used during the transport in such a way that the size of the projection stays the same through the transport*/projection_size, /*same as projection_size, but for the initial/final step of the animation */projection_size_start, projection_size_end;
    //the NS, SW and center Position of the chart, used for the Mercator projections
    Position p_NE, p_SW, p_center;;
    //for the 3d projectionb: the aperture angles of circle_observer  at the beginning and at the end of the transport, respectively
    Angle omega_start, omega_end;
    Double zoom_factor;

    ChartTransportHandler(ChartFrame*, const Route&, const Double&, F*);
    void operator()(void);
    void OnTimer(wxTimerEvent&);
    
};
