//
//  check_minute.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_minute_h
#define check_minute_h

#include <iostream>

using namespace std;

template <class P> class ChronoField;


template<class P> class CheckMinute{
    
public:
    
    ChronoField<P>* p;
    
    CheckMinute(ChronoField<P>*);
    template<class T> void operator()(T&);
    
    
};


#endif
