//
//  tabulate_days.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef tabulate_days_h
#define tabulate_days_h

#include <iostream>

using namespace std;

template <class P> class DateField;


template<class P> class TabulateDays{
    
public:
    
    DateField<P>* p;
    
    TabulateDays(DateField<P>*);
    
    template<class T> void operator()(T&);
    
};


#endif
