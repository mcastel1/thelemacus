//
//  check_speed_value.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_speed_value_h
#define check_speed_value_h

#include <iostream>

using namespace std;

template <class P> class SpeedField;


template<class P> class CheckSpeedValue{
    
public:
    
    SpeedField<P>* p;
    
    CheckSpeedValue(SpeedField<P>*);
    template<class T> void operator()(T&);
    
};

#endif
