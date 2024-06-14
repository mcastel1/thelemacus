//
//  speed.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef speed_h
#define speed_h

#include <iostream>

#include "my_string.h"
#include "speed_unit.h"
#include "generic.h"

using namespace std;


class Speed{
    
public:
    
    //the value of the Speed
    double value;
    //the unit of measure of the Speed
    SpeedUnit unit;

    
    Speed();
    Speed(double);
    Speed(double, const SpeedUnit&);
    
    void set(double);
    void set(double, const SpeedUnit&);
    void set(String, double, String);
    void print(String, String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void print(String, String, ostream&);
    
};


#endif
