//
//  length_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef length_unit_h
#define length_unit_h

#include <iostream>

#include "my_string.h"

using namespace std;

//a derived class from String, which stores the unit of measuer of Length ("nm", "m", "ft", ...)
class LengthUnit: public String{
    
public:

    LengthUnit();
    LengthUnit(const String&);
    LengthUnit(const string&);
    
    bool check();
    
};


#endif
