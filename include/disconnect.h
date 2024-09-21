//
//  disconnect.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef disconnect_h
#define disconnect_h

#include <iostream>


#include "list_frame.h"

using namespace std;

//this functor disconnects a Sight from a Route in ListFrame *parent
class Disconnect{
    
public:
        
    //the ListFrame that called this functor
    ListFrame* parent;
    int sight_id;
    
    Disconnect(ListFrame*, const int&);
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


#endif
