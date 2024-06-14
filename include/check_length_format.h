//
//  check_length_format.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_length_format_h
#define check_length_format_h

#include <iostream>

#include "length_format_field.h"


using namespace std;

//a functor that checks if a LengthFormat  is valid
template<class P> class CheckLengthFormat{
    
public:
    
    //the LengthFormat  field containing the Route type
    LengthFormatField<P>* p;
    
    CheckLengthFormat(LengthFormatField<P>*);
    template<class T> void operator()(T&);
    
};



#endif
