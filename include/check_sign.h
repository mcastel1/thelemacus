//
//  check_sign.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_sign_h
#define check_sign_h

#include "angle_field.h"

template<class P> class AngleField;


template<class P> class CheckSign{
    
public:
    
    AngleField<P>* p;
    
    CheckSign(AngleField<P>*);
    template <class T> void operator()(T&);
    
    
};

#endif
