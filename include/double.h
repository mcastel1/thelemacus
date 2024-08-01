//
//  double.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef double_h
#define double_h

#include <iostream>

#include "my_string.h"

using namespace std;


class Double{
    
public:
    
    double value;
    
    Double();
    Double(const double&);
    bool equal_approx(Double);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void set(double);
    void set(String, double, String);
    string to_string(unsigned int);
    void print(String, String, ostream&);
    Double operator + (const Double&);
    bool operator > (const double&), operator > (const Double&), operator < (const double&), operator < (const Double&);
    
};



#endif
