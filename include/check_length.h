//
//  check_length.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_length_h
#define check_length_h

#include <iostream>

using namespace std;

template<class P> class DynamicLengthField;
template<class P> class CheckLengthValue;
template<class P, class GUI> class CheckUnit;


template<class P> class CheckLength{
    
public:
    
    //p is the DynamicLengthField which is parent of the CheckLength object: the CheckLength object checks the validity of the entries in DynamicLengthField
    DynamicLengthField<P>* p;
    CheckLengthValue<P>* check_length_value;
    CheckUnit<P, DynamicLengthField<P>>* check_length_unit;
    
    CheckLength(DynamicLengthField<P>*);
    template <class T> void operator()(T&);
    
};


#endif
