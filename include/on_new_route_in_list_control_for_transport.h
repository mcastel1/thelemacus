//
//  on_new_route_in_list_control_for_transport.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef on_new_route_in_list_control_for_transport_h
#define on_new_route_in_list_control_for_transport_h

#include <iostream>

#include "list_frame.h"

using namespace std;


class ListFrame;


//this class is to define the functor template<class T> void operator()(T&), which is called when the user crates a new route with which he wants to transport a sight
class OnNewRouteInListControlRoutesForTransport{
    
public:
    
    //parent frame
    ListFrame* parent;
    
    //constructor, which sets the parent frame
    OnNewRouteInListControlRoutesForTransport(ListFrame*);
    template<class T> void operator()(T&);
    
};




#endif
