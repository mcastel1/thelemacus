//
//  motion_handler.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef motion_handler_h
#define motion_handler_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

class ListFrame;
class Position;
class Route;


//a hanlder to make a general motion (i.e., drag the chart, transport a Position, ...) with an animation
template<class F> class MotionHandler{
    
public:
    
    //the functor that calls *this
    ListFrame* parent;
    //the eventual functor to be called at the end of tha motion animation
    F* f;

    wxTimer* timer;
    //the position during the transport process at 'time' t
    Position *start;
    //transporting_route is the Route used to do the transport. At each step of the transport, transporting_route_temp is set to be a part of the full Route (transporting_route) used to do the transport
    Route transporting_route, transporting_route_temp;

    //a counter of the step in the animation, running from 0 to n_animation_steps
    long t;
    
    MotionHandler(ListFrame*, const Route&, F*);
    
};


#endif
