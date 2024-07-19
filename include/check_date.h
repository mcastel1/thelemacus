//
//  check_date.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_date_h
#define check_date_h

#include <iostream>

using namespace std;


template<class P> class DateField;
template<class P> class CheckYear;
template<class P> class CheckMonth;
template<class P> class CheckDay;

template<class P> class CheckDate{
    
public:
    
    //the parent DateField
    DateField<P>* p;
    CheckYear<P>* check_year;
    CheckMonth<P>* check_month;
    CheckDay<P>* check_day;
    
    //constructor, which sets the parent
    CheckDate(DateField<P>*);
    template <class T> void operator()(T&);
    
};


#endif
