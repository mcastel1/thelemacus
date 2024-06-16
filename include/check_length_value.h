//
//  check_length_value.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_length_value_h
#define check_length_value_h

#include <iostream>

using namespace std;


template<class P> class DynamicLengthField;


template<class P> class CheckLengthValue{
    
public:
    
    DynamicLengthField<P>* p;
    
    CheckLengthValue(DynamicLengthField<P>*);
    template<class T> void operator()(T&);
    
};

#endif
