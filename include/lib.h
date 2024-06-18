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
