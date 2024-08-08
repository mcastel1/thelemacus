//
//  check_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_unit_h
#define check_unit_h

#include <iostream>

#include "my_string.h"

using namespace std;

template<class P> class DynamicLengthField;



template<class P, class GUI> class CheckUnit{
    
public:
    
    //the GUI field relative to *this
    GUI* p;
    //a vector of unit types, e.g., LengthUnit_types, which will be used to list the units of measures relative to *this
    vector<String> unit_types;
    
    CheckUnit(GUI*, const vector<String>&);
    template<class T> void operator()(T&);
    
};


#endif
