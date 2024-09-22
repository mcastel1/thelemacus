//
//  do_nothing.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef do_nothing_h
#define do_nothing_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

using namespace std;

//this functor sets idling -> false in parent
template<class P> class UnsetIdling{
    
public:
        
    //the frame which called this struct
    P* parent;
    
    UnsetIdling(P*);
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


#endif
