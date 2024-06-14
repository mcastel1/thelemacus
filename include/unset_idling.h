//
//  unset_idling.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef unset_idling_h
#define unset_idling_h

#include <iostream>

using namespace std;

//this functor sets idling -> false in parent
template<class P> class UnsetIdling{
    
public:
    
    UnsetIdling(P*);
    
    //the frame which called this struct
    P* parent;
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


#endif
