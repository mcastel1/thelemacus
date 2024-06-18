//
//  check_hour.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_hour_h
#define check_hour_h

#include <iostream>

using namespace std;

template <class P> class ChronoField;



template<class P> class CheckHour{
    
public:
    
    ChronoField<P>* p;
    
    CheckHour(ChronoField<P>*);
    template<class T> void operator()(T&);
    
    
};

#endif
