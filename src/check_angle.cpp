//
//  check_angle.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_angle.h"

#include "angle.h"
#include "position_frame.h"
#include "route_frame.h"
#include "sight_frame.h"


template<class P> CheckAngle<P>::CheckAngle(AngleField<P>* p_in) {

    p = p_in;

    check_sign = new CheckSign<P>(p);
    check_arc_degree = new CheckArcDegree<P>(p);
    check_arc_minute = new CheckArcMinute<P>(p);
    
    if(p->is_ok()){
        
        //write the value written in *this into *recent_angle
        p->recent_angle->
        
    }

}

//explicit instantiation
template class CheckAngle<PositionFrame>;
template class CheckAngle<RouteFrame>;
template class CheckAngle<SightFrame>;


//this functor checks the whole angle field by calling the check on its sign, arcdegree and arcminute parts‰
template<class P> template <class T> void CheckAngle<P>::operator()(T& event) {

    (*check_sign)(event);
    (*check_arc_degree)(event);
    (*check_arc_minute)(event);

    event.Skip(true);

}

//explicit instantiation
template void CheckAngle<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckAngle<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
