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



class SetStringToCurrentTime{
    
public:
    
    String* p;
    
    SetStringToCurrentTime(String*);
    template<class T> void operator()(T&);
    
};







//this functor pops up a QuestionFrame and asks the user whether he wants to remove the a Sight related to a Route
class AskRemoveRelatedSight{
    
public:
    
    AskRemoveRelatedSight(ListFrame*);
    
    //the frame which called this struct
    ListFrame* parent;
    
    void operator()(wxCommandEvent&);
    
};





























