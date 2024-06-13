//
//  check_arc_minute.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_arc_minute_h
#define check_arc_minute_h

#include "angle_field.h"

template<class P> class AngleField;

template<class P> class CheckArcMinute{
    
public:
    
    AngleField<P>* p;
    
    CheckArcMinute(AngleField<P>*);
    template <class T> void operator()(T&);
    
};


#endif
