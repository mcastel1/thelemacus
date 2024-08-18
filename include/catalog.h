//
//  catalog.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef catalog_h
#define catalog_h

#include <iostream>

#include "body.h"
#include "body_name.h"

using namespace std;


class Body;

class Catalog{
    
public:
    
    vector<Body> list;
    //number_of_lines[i] is the number of lines in the ephemerides file relative to Bosy list[i]
    vector<unsigned long long int> number_of_lines_bodies;
    
    Catalog(String, String);
    
    void set(const Catalog&);
    void add(String, String, double);
    vector<BodyName> get_body_names(void);
    void print(String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    unsigned int position_in_list(const BodyName&);
    
};


#endif
