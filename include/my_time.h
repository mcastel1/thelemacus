//
//  time.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef my_time_h
#define my_time_h

#include <iostream>

#include "date.h"
#include "chrono.h"
#include "my_string.h"

using namespace std;


class Time{
    
public:
    
    Date date;
    Chrono chrono;
    double MJD;
    
    void set(const Time&);
    void print(String, String, ostream&);
    void set_current_utc(void);
    template<class S> bool read_from_stream(String, S*, bool, String);
    
    void to_MJD(void);
    void to_TAI(void);
    
    string to_string(unsigned int, bool);
    bool operator==(const Time&), operator!=(const Time&), operator> (const Time&);
    void operator += (const Chrono&);
    void operator -= (const Chrono&);
    
};





#endif
