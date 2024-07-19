//
//  chrono.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef chrono_h
#define chrono_h

#include <iostream>

#include "my_string.h"

using namespace std;

class Chrono{
    
public:
    
    unsigned int h, m;
    double s;
    
    void print(String, String, ostream&);
    bool set(String, double, String);
    void set(const Chrono&);
    double get(void);
    void set_current(void);
    template<class S> bool read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    string to_string(unsigned int, bool);
    
    bool operator==(const Chrono&), operator!=(const Chrono&), operator<(const Chrono&), operator>(const Chrono&);
    
};

#endif
