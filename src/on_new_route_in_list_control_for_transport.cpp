//
//  on_new_route_in_list_control_for_transport.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "on_new_route_in_list_control_for_transport.h"

#include "graphical_feature_transport_handler.h"
#include "to_do_at_end_of_transport.h"



OnNewRouteInListControlRoutesForTransport::OnNewRouteInListControlRoutesForTransport(ListFrame* f_in) {

    parent = f_in;

}



//if a new item listcontrol_routes is created, I transport the sight/position under consideration with such Route
template<class T> void OnNewRouteInListControlRoutesForTransport::operator()(T& event) {
    
    //the id of the Route that will do the transport: it is the last item in listcontrol_routes, because it is the item of the newly added Route
    (parent->i_transporting_route) = ((parent->listcontrol_routes)->GetItemCount()) - 1;
    //do the tasks tha need to be done at the end of the transport
    ToDoAtEndOfTransport<Route, ListFrame>* to_do_at_end_of_transport;

    to_do_at_end_of_transport = new ToDoAtEndOfTransport<Route, ListFrame>(
                                                                  NULL,
                                                                  NULL,
                                                                  parent
                                                                  );


    if (((parent->transported_object_type) == String("sight")) || ((parent->transported_object_type) == String("route"))) {
        //I am transporting a Sight or the Route related to it: allocate transport_handler with template NON_GUI = Route
        
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* transport_handler;

        
        if ((parent->transported_object_type) == String("sight")) {
            
            
            //the id of the Route that will be transported
            (parent->i_object_to_transport) = (((((parent->data)->sight_list)[(parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value);
            
            
        }
        
        
        transport_handler = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(
                                                                                                                 parent,
                                                                                                                 &((parent->data->route_list)[(parent->i_object_to_transport)]),
                                                                                                                 (parent->transported_object_type),
                                                                                                                 ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                 to_do_at_end_of_transport
                                                                                                                 );
        
        //start the transport
        (*transport_handler)();
        
    }
    
    if ((parent->transported_object_type) == String("position")) {
        //I am transporting a Position: allocate transport_handler with template NON_GUI = Position
        
        GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame> >* transport_handler;
        
        
        //the id of the Route or Position that will be transported
        (parent->i_object_to_transport) = ((int)(parent->listcontrol_positions)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
        
        transport_handler = new GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                                    &((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                    (parent->transported_object_type),
                                                                                                                    ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                    to_do_at_end_of_transport
                                                                                                                    );
        
        //start the transport
        (*transport_handler)();
    }
    
    event.Skip(true);
    
}

template void OnNewRouteInListControlRoutesForTransport::operator()<wxCommandEvent>(wxCommandEvent&);
