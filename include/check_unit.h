//
//  check_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_unit_h
#define check_unit_h

#include <iostream>

#include "length_unit.h"
#include "my_string.h"

using namespace std;

template<class P> class DynamicLengthField;


//this class checks the unit of measure in frame of type P, for a non-GUI object of type NON_GUI_UNIT, related to a GUI field of type GUI
template<class P, class NON_GUI_UNIT, class GUI> class CheckUnit{
    
public:
    
    //the GUI field relative to *this
    GUI* p;
    //a vector of unit types, e.g., LengthUnit_types, which will be used to list the units of measures relative to *this
    vector<NON_GUI_UNIT> unit_types;
    
    CheckUnit(GUI*, const vector<NON_GUI_UNIT>&);
    template<class T> void operator()(T&);
    
};


#endif
