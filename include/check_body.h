//
//  check_body.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_body_h
#define check_body_h

#include "body_field.h"


template<class P> class BodyField;


//this checks if an element of the BodyField class is valid
template<class P> class CheckBody{
    
public:
    
    BodyField<P>* p;
    
    CheckBody(BodyField<P>*);
    template<class T> void operator()(T&);
    
    
};



#endif
