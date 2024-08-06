//
//  some_routes.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "some_routes.h"

#include "generic.h"
#include "list_frame.h"
#include "my_string.h"


SomeRoutes::SomeRoutes(ListFrame* f_in) {

    f = f_in;

}


void SomeRoutes::operator()(wxCommandEvent& event) {


    int i;

    f->print_info_message->SetAndCall(NULL, String(""), String("Select the routes that you want to use to compute the astronomical position and press enter when done"), (wxGetApp().path_file_info_icon));

    //Given that a sight must be transported only with a Route that does not come from a Sight and a Route that is not a circle of equal altitude (it would not make sense), I store in route_list_for_transport the Routes in route_list which are not related to any sight and that are not circles of equal altitude, show route_list_for_transport in listcontrol_routes, and let the user select one item in route_list_for_transport to transport the Sight
    for ((f->crossing_route_list_temp.clear()), i = 0; i < (f->data->route_list.size()); i++) {

        if (((f->data->route_list)[i]).type == (Route_types[2])) {
            
            //            f->crossing_route_list_temp.push_back(((f->data->route_list)[i]));
            my_push_back(&(f->crossing_route_list_temp), ((f->data->route_list)[i]));
            
        }

    }

    //setting this to true, now when the enter key is pressed the selected Routes are used to compute the position
    (f->selecting_route_for_position) = true;
    (f->listcontrol_routes)->set(f->crossing_route_list_temp, false);
    //I bind listcontrol_routes to on_select_route_in_listcontrol_routes_for_position in such a way that when the user will select an item in listcontrol, I perform the computation of the position
    //    (f->listcontrol_routes)->Bind(wxEVT_LIST_ITEM_SELECTED, *(f->on_select_route_in_listcontrol_routes_for_position));




    event.Skip(true);

}
