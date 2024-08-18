//
//  listcontrol_type.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef listcontrol_type_h
#define listcontrol_type_h

#include <iostream>

#include "my_string.h"

using namespace std;

//a derived class from String, which stores the unit of measuer of Length ("nm", "m", "ft", ...)
class ListControlType: public String{
    
public:

    ListControlType();
    ListControlType(const String&);
    ListControlType(const string&);
        
};


#endif
