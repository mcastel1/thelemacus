//
//  on_select_route_in_list_control_for_transport.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef on_select_route_in_list_control_for_transport_h
#define on_select_route_in_list_control_for_transport_h

#include <iostream>

#include "list_frame.h"

using namespace std;


class ListFrame;


//this class is to define the functor template<class T> void operator()(T&), which is called when the user selects a route with which he wants to transport a sight
class OnSelectRouteInListControlRoutesForTransport{
    
public:
    
    //parent frame
    ListFrame* parent;
    //    UnsetIdling<ListFrame>* unset_idling;
    
    //constructor, which sets the parent frame
    OnSelectRouteInListControlRoutesForTransport(ListFrame*);
    
    template<class T> void operator()(T&);
    
    
};

#endif
