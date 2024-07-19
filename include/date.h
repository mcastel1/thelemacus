//
//  date.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef date_h
#define date_h

#include <iostream>

using namespace std;

#include "my_string.h"

class String;

class Date{
    
public:
    
    unsigned int Y, M, D;
    bool Y_is_leap_year;
    vector<unsigned int> days_per_month;
    
    void set(const Date&);
    void print(String, String, ostream&);
    template<class S> bool read_from_stream(String, S*, bool, String);
    string to_string(void);
    void check_leap_year(void);
    void set_current(void);
    
};


#endif
