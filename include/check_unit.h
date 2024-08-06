//
//  check_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_unit_h
#define check_unit_h

#include <iostream>

using namespace std;

template<class P> class DynamicLengthField;



template<class P> class CheckUnit{
    
public:
    
    DynamicLengthField<P>* p;
    
    CheckUnit(DynamicLengthField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
