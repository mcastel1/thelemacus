//
//  limb_type.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef limb_type_h
#define limb_type_h

#include <iostream>

#include "my_string.h"

using namespace std;

//a derived class from String, which stores the type of the Limb of a Body ("upper", "center", ...)
class LimbType: public String{
    
public:

    LimbType();
    LimbType(const String&);
    LimbType(const string&);
        
};


#endif
