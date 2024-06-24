//
//  check_chrono.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_chrono.h"

#include "check_hour.h"
#include "check_minute.h"
#include "check_second.h"
#include "route_frame.h"
#include "sight_frame.h"

template<class P> CheckChrono<P>::CheckChrono(ChronoField<P>* p_in) {

    p = p_in;

    check_hour = new CheckHour<P>(p);
    check_minute = new CheckMinute<P>(p);
    check_second = new CheckSecond<P>(p);

}



//this function writes into sight.stopwatch the value written into the respective GUI box
template<class P> template <class T> void CheckChrono<P>::operator()(T& event) {

    (*check_hour)(event);
    (*check_minute)(event);
    (*check_second)(event);

    event.Skip(true);

}

//explicit instantiations
template void CheckChrono<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckChrono<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
