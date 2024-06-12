//
//  check_limb.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_limb_h
#define check_limb_h

#include "limb_field.h"

template<class P> class LimbField;


//this checks the value of LimbField
template<class P> class CheckLimb{
    
public:
    
    LimbField<P>* p;
    
    CheckLimb(LimbField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
