//
//  graphical_feature_transport_handler.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef graphical_feature_transport_handler_h
#define graphical_feature_transport_handler_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "my_string.h"

using namespace std;

class ListFrame;
class Route;
class String;


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




#endif
