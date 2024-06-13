//
//  catalog.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef catalog_h
#define catalog_h


#include "body.h"

class Body;

class Catalog{
    
public:
    vector<Body> list;
    Catalog(String, String);
    void add(String, String, double);
    vector<String> get_names(void);
    void print(String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    
};


#endif
