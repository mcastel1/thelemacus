//
//  disconnect_sight_and_unset_idling.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef disconnect_sight_and_unset_idling_h
#define disconnect_sight_and_unset_idling_h

#include <iostream>

#include "int.h"
#include "list_frame.h"

using namespace std;


//this functor disconnects a Sight from a Route in ListFrame *parent
class DisconnectSightAndUnsetIdling{
    
public:
        
    //the ListFrame that called this functor
    ListFrame* parent;
    Int sight_id;
    
    DisconnectSightAndUnsetIdling(ListFrame*, const Int&);
    
    template <class E> void operator()(E&);
    void operator()(void);
    
};

#endif
