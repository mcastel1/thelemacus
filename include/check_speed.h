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




template<class P> class SpeedField;
template<class P> class CheckSpeedValue;
template<class P> class CheckSpeedUnit;

template<class P> class CheckSpeed{
    
public:
    
    //p is the SpeedField which is parent of the CheckSpeed object: the CheckSpeed object checks the validity of the entries in SpeedField
    SpeedField<P>* p;
    CheckSpeedValue<P>* check_speed_value;
    CheckSpeedUnit<P>* check_speed_unit;
    
    CheckSpeed(SpeedField<P>*);
    template <class T> void operator()(T&);
    
};





#endif
