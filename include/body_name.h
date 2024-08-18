//
//  body_name.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef body_name_h
#define body_name_h

#include <iostream>

#include "my_string.h"

using namespace std;

//a derived class from String, which stores the name of Body 
class BodyName: public String{
    
public:

    BodyName();
    BodyName(const String&);
    BodyName(const string&);
    
    bool check();
    
};


#endif
