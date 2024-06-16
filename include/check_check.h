//
//  check_check.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_check_h
#define check_check_h

#include <iostream>

using namespace std;

template<class P, class T> class CheckField;


template<class P, class T> class CheckCheck{
    
public:
    
    CheckField<P,T>* p;
    
    CheckCheck(CheckField<P,T>*);
    //this functor checks whether a GUI Check field is filled correctly and writes its value into the relative non-GUI field
    template<class R> void operator()(R&);
    
};


#endif
