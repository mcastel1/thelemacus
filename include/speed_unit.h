//
//  speed_unit.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef speed_unit_h
#define speed_unit_h

#include <iostream>

#include "my_string.h"

using namespace std;


//a derived class from String, which stores the unit of measuer of Speed ("kt", etc...)
class SpeedUnit: public String{
    
public:

    SpeedUnit();
    SpeedUnit(const String&);
    SpeedUnit(const string&);
    
};



#endif
