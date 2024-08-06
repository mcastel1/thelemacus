//
//  length.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef length_h
#define length_h

#include <iostream>

#include "chrono.h"
//#include "constants.h"
#include "length_unit.h"
#include "my_string.h"
#include "speed.h"

using namespace std;

class Chrono;
class LengthUnit;
class Speed;
class String;

//a non-GUI object containing a Length
class Length{
    
public:
    //the value of the Length
    double value;
    //the unit of measure of the Length
    LengthUnit* unit;
    
    Length();
    Length(double);
    Length(double, const LengthUnit&);
    Length(Chrono, Speed);
    
    void set(String, double, String);
    void set(double);
    void set(double, const LengthUnit&);
    void set(const Length&);
    string to_string(const LengthUnit&, unsigned int);
    string to_string(unsigned int);
    void print(String, String, ostream&);
    void convert_to(const LengthUnit&);
    Length convert(const LengthUnit&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    bool check(String, String);
    bool operator > (const Length&), operator >= (const Length&), operator <= (const Length&), operator > (const double&), operator >= (const double&), operator <= (const double&), operator < (const Length&), operator < (const double&), operator == (const Length&), operator != (const Length&);
    Length operator + (const Length&), operator - (const Length&),  operator * (const double&), operator / (const double&);
    void operator += (const Length&), operator -= (const Length&), operator *= (const double&), operator /= (const double&);
    
};

#endif
