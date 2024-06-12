//
//  check_angle.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_angle_h
#define check_angle_h

#include "angle_field.h"


template<class P> class CheckAngle{
    
public:
    
    //p is the AngleField which is parent of the CheckAngle object: the CheckAngle object checks the validity of the entries in AngleField
    AngleField<P>* p;
    CheckSign<P>* check_sign;
    CheckArcDegree<P>* check_arc_degree;
    CheckArcMinute<P>* check_arc_minute;
    
    CheckAngle(AngleField<P>*);
    template <class T> void operator()(T&);
    
};


#endif
