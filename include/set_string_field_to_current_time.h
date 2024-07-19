//
//  set_string_field_to_current_time.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef set_string_field_to_current_time_h
#define set_string_field_to_current_time_h

#include <iostream>

using namespace std;

template<class P> class StringField;


template<class P> class SetStringFieldToCurrentTime{
    
public:
    
    StringField<P>* p;
    
    template<class T> void operator()(T&);
    
};


#endif
