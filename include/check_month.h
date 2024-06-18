//
//  check_month.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_month_h
#define check_month_h

#include <iostream>

using namespace std;

template<class P> class DateField;
template<class P> class TabulateDays;


template<class P> class CheckMonth{
    
public:
    
    
    DateField<P>* p;
    TabulateDays<P>* tabulate_days;
    
    CheckMonth(DateField<P>*);
    template<class T> void operator()(T&);
    
};



#endif
