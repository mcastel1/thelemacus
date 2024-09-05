//
//  set_highlighted_object.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef set_highlighted_object_h
#define unset_idling_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

using namespace std;

//this functor sets a highlighted object (i.e. a Route or a Position) to a given value and stores the id of the old highlighted object 
template<class P> class SetHighlightedObject{
    
public:
        
    //the frame which called this struct
    P* parent;
    
    SetHighlightedObject(P*);
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


#endif
