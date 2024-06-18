//
//  check_string.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_string_h
#define check_string_h

#include <iostream>

using namespace std;


template<class P> class StringField;


template<class P> class CheckString{
    
public:
    
    StringField<P>* p;
    
    CheckString(StringField<P>*);
    template<class T> void operator()(T&);
    
};




#endif
