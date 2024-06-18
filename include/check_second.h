//
//  check_second.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_second_h
#define check_second_h

#include <iostream>

using namespace std;

template<class P> class ChronoField;


template<class P> class CheckSecond{
    
public:
    
    ChronoField<P>* p;
    
    CheckSecond(ChronoField<P>*);
    template<class T> void operator()(T&);
    
};



#endif
