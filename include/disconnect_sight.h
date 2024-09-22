//
//  disconnect_sight.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef disconnect_sight_h
#define disconnect_sight_h

#include <iostream>


#include "list_frame.h"

using namespace std;

//this functor disconnects a Sight from a Route in ListFrame *parent
class DisconnectSight{
    
public:
        
    //the ListFrame that called this functor
    ListFrame* parent;
    int sight_id;
    
    DisconnectSight(ListFrame*, const int&);
    
    template <class E> void operator()(E&);
    void operator()(void);
    
};

#endif
