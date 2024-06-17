//
//  save_and_reset.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef save_and_reset_h
#define save_and_reset_h

#include <iostream>

using namespace std;


template<class F> class SaveAndReset{
    
public:
    
    //the frame to be closed
    F* frame;
    SaveAndReset(F*);
    
    template<class T> void operator()(T&);
    
};

#endif
