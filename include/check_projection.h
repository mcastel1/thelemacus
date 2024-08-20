//
//  check_projection.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_projection_h
#define check_projection_h

#include <iostream>

#include "projection_field.h"

template<class P> class ProjectionField;


using namespace std;

//a functor that checks if a Projection  is valid
template<class P> class CheckProjection{
    
public:
    
    //the Projection  field containing the Route type
    ProjectionField<P>* p;
    
    CheckProjection(ProjectionField<P>*);
    template<class T> void operator()(T&);
    
};

#endif
