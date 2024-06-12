//
//  body.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef body_h
#define body_h

#include "string.h"
#include "length.h"
#include "angle.h"

using namespace std;

class Body{
    
public:

    String name, type;
    Length radius;
    Angle RA, d;

    bool check(unsigned int*, Catalog, String);
    void print(String, String, ostream&);
    template<class S> bool read_from_stream(String, S*, bool, String);
    
    bool operator == (const Body&);
    
};


#endif
