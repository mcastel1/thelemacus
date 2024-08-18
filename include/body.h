//
//  body.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef body_h
#define body_h

#include <iostream>

#include "angle.h"
#include "body_name.h"
#include "catalog.h"
#include "length.h"
#include "my_string.h"

using namespace std;


class Angle;
class Catalog;
class Length;
class String;

class Body{
    
public:

    BodyName* name;
    String *type;
    Length *radius;
    Angle *RA, *d;
    
    Body();

    bool check(unsigned int*, Catalog, String);
    void print(String, String, ostream&);
    template<class S> bool read_from_stream(String, S*, bool, String);
    void set(const Body&);
    void set_from_name(const BodyName&);
    
    bool operator == (const Body&);
    
};


#endif
