//
//  body.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef body_h
#define body_h

#include <iostream>

#include "my_string.h"
#include "length.h"
#include "angle.h"
#include "catalog.h"

using namespace std;


class Angle;
class Catalog;
class Length;
class String;

class Body{
    
public:

    String *name, *type;
    Length *radius;
    Angle *RA, *d;

    bool check(unsigned int*, Catalog, String);
    void print(String, String, ostream&);
    template<class S> bool read_from_stream(String, S*, bool, String);
    
    bool operator == (const Body&);
    
};


#endif
