//
//  check_body_name.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_body_name_h
#define check_body_name_h

#include <iostream>

#include "body_name_field.h"

using namespace std;

template<class P> class BodyNameField;


//this checks if an element of the BodyNameField class is valid
template<class P> class CheckBodyName{
    
public:
    
    BodyNameField<P>* p;
    
    CheckBodyName(BodyNameField<P>*);
    template<class T> void operator()(T&);
    
    
};



#endif
