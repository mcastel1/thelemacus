//
//  projection.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef projection_h
#define projection_h

#include <iostream>

#include "my_string.h"

using namespace std;


//a derived class from String, which stores the type of a Projection ("Mercator", "3D", ...)
class Projection: public String{
    
public:

    Projection();
    Projection(string);
    
};


#endif
