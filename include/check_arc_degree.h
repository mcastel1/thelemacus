//
//  check_arc_degree.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_arc_degree_h
#define check_arc_degree_h

#include "angle_field.h"

template<class P> class AngleField;

template<class P> class CheckArcDegree{
    
public:
    
    AngleField<P>* p;
    
    CheckArcDegree(AngleField<P>*);
    template<class T> void operator()(T&);
    
};

#endif
