//
//  set_idling.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef set_idling_h
#define set_idling_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/event.h>

using namespace std;

//this functor sets idling -> true in parent
template<class P> class SetIdling{
    
public:
    
    //the frame which called this struct
    P* parent;
    
    SetIdling(P*);
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};

#endif
