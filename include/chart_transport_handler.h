//
//  chart_transport_handler.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef chart_transport_handler_h
#define chart_transport_handler_h

#include <iostream>

using namespace std;

class Angle;
class ChartFrame;
class Double;
class Position;
class PositionProjection;


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


#endif
