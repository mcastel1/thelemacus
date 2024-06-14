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












#include "units.h"
#include "constants.h"



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
