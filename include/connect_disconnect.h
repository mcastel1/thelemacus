//
//  connect_disconnect.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef connect_disconnect_h
#define connect_disconnect_h

#include <iostream>


#include "list_frame.h"

using namespace std;

//this functor either connects or disconnects a Sight and a Route in ListFrame *parent
class ConnectDisconnect{
    
public:
        
    //the ListFrame that called this functor
    ListFrame* parent;
    int sight_id;
    
    ConnectDisconnect(ListFrame*, const int&, const bool&);
    
    void operator()(wxCommandEvent&);
    void operator()(void);
    
};


#endif
