//
//  check_speed.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_speed.h"

#include "check_speed_value.h"
#include "check_speed_unit.h"
#include "route_frame.h"

template<class P> CheckSpeed<P>::CheckSpeed(SpeedField<P>* p_in) {

    p = p_in;

    check_speed_value = new CheckSpeedValue<P>(p);
    check_speed_unit = new CheckSpeedUnit<P>(p);

}

//explicit instantiations
template class CheckSpeed<RouteFrame>;


//this functor checks the whole Speed field by calling the check on its value and unit
template<class P> template <class T> void CheckSpeed<P>::operator()(T& event) {

    (*check_speed_value)(event);
    (*check_speed_unit)(event);

    event.Skip(true);

}

//explicit instantitation
template void CheckSpeed<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
