//
//  check_chrono.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_chrono_h
#define check_chrono_h

#include <iostream>

using namespace std;

template<class P> class ChronoField;
template<class P> class CheckHour;
template<class P> class CheckMinute;
template<class P> class CheckSecond;


template<class P> class CheckChrono{
    
public:
    
    //the parent field of the CheckChrono object
    ChronoField<P>* p;
    CheckHour<P>* check_hour;
    CheckMinute<P>* check_minute;
    CheckSecond<P>* check_second;
    
    CheckChrono(ChronoField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
