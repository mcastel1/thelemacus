//
//  check_day.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_day_h
#define check_day_h

#include <iostream>

using namespace std;

template<class P, class T> class CheckField;
template<class P> class DateField;


template<class P> class CheckDay{
    
public:
    
    DateField<P>* p;
    
    CheckDay(DateField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
