//
//  check_string.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_string.h"

template<class P> class StringField;


template<class P> class CheckString{
    
public:
    
    StringField<P>* p;
    
    CheckString(StringField<P>*);
    template<class T> void operator()(T&);
    
};
