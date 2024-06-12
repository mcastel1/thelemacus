//
//  limb.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef limb_h
#define limb_h

#include "my_string.h"

class Limb{
    
public:
    char value;
    void print(String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    
    bool operator == (const Limb&);
    
};


#endif
