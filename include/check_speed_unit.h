//
//  check_speed_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_speed_unit_h
#define check_speed_unit_h

#include <iostream>

using namespace std;

template <class P> class SpeedField;


template<class P> class CheckSpeedUnit{
    
public:
    
    SpeedField<P>* p;
    
    CheckSpeedUnit(SpeedField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
