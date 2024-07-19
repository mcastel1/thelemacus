//
//  check_year.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_year_h
#define check_year_h

#include <iostream>

using namespace std;

template<class P> class DateField;
template<class P> class TabulateDays;


template<class P> class CheckYear{
    
public:
    
    DateField<P>* p;
    TabulateDays<P>* tabulate_days;
    
    CheckYear(DateField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
