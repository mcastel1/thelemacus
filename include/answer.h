//
//  answer.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef answer_h
#define answer_h

#include <iostream>


#include "my_string.h"

using namespace std;

class Answer{
    
public:
    char value;
    
    Answer();
    Answer(char, String);
    bool set(String, char, String);
    //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
    void print(String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    bool operator==(const Answer&), operator !=(const Answer&);
    
};


#endif
