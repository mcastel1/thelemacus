//
//  check_angle.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_angle.h"

#include "route_frame.h"


template<class P> CheckAngle<P>::CheckAngle(AngleField<P>* p_in) {

    p = p_in;

    check_sign = new CheckSign<P>(p);
    check_arc_degree = new CheckArcDegree<P>(p);
    check_arc_minute = new CheckArcMinute<P>(p);

}

//explicit instantiation
template class CheckAngle<RouteFrame>;


//this functor checks the whole angle field by calling the check on its sign, arcdegree and arcminute parts‰
template<class P> template <class T> void CheckAngle<P>::operator()(T& event) {

    //    if((p->format) != String("")){
    (*check_sign)(event);
    //    }
    (*check_arc_degree)(event);
    (*check_arc_minute)(event);

    event.Skip(true);

}

//explicit instantiation
template void CheckAngle<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
