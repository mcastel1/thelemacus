//
//  animate_to_object.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef animate_to_object_h
#define animate_to_object_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

using namespace std;

//this functor sets idling -> false in parent
template<class P> class AnimateToObject{
    
public:
        
    //the frame which called this struct
    P* parent;
    
    AnimateToObject(P*);
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


#endif
