//
//  check_length_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_length_unit_h
#define check_length_unit_h

#include <iostream>

using namespace std;

template<class P> class DynamicLengthField;



template<class P> class CheckLengthUnit{
    
public:
    
    DynamicLengthField<P>* p;
    
    CheckLengthUnit(DynamicLengthField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
