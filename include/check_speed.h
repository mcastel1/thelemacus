//
//  check_speed.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_speed_h
#define check_speed_h

#include <iostream>

using namespace std;




template<class P> CheckSpeed<P>::CheckSpeed(SpeedField<P>* p_in) {

    p = p_in;

    check_speed_value = new CheckSpeedValue<P>(p);
    check_speed_unit = new CheckSpeedUnit<P>(p);

}


//this functor checks the whole Speed field by calling the check on its value and unit
template<class P> template <class T> void CheckSpeed<P>::operator()(T& event) {

    (*check_speed_value)(event);
    (*check_speed_unit)(event);

    event.Skip(true);

}






#endif
