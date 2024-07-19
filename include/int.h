//
//  int.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef int_h
#define int_h

#include <iostream>

#include "my_string.h"

using namespace std;


class Int{
    
public:
    
    int value;
    
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void set(const Int&);
    void set(String, int, String);
    void set(int);
    void my_round(Int precision);
    String to_string_spaces(void);
    void print(String, String, ostream&);
    
    bool operator == (const Int&), operator != (const Int&), operator == (const int&), operator != (const int&), operator > (const Int&), operator > (const int&);
    
};


#endif
