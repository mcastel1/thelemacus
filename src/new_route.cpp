//
//  new_route.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "list_frame.h"
#include "new_route.h"


NewRoute::NewRoute(ListFrame* f_in) {

    f = f_in;

}



void NewRoute::operator()(wxCommandEvent& event) {

    (f->transporting_with_new_route) = true;
    //call OnAddRoute to add a new Route
    (f->OnAddRouteForTransport)(event);

    //when button_ok in f->route_fram will be pressed, I call on_new_route_in_listcontrol_routes_for_transport to execute the transport with this Route
    //    f->route_frame->button_ok->Bind(wxEVT_BUTTON, *(f->on_new_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}

