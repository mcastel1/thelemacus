//
//  all_routes.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "all_routes.h"


AllRoutes::AllRoutes(ListFrame* f_in) {

    f = f_in;

}



void AllRoutes::operator()(wxCommandEvent& event) {

    unsigned int j;

    //there need to be at list two routes of type ((Route_types[2]).value) to compute crossings. Here I include all routes of type ((Route_types[2]).value) whhich are related to a Sight into crossing_route_list by writing their index into crossing_route_list
    for ((f->data->crossing_route_list).clear(), j = 0; j < (f->data->route_list).size(); j++) {

        if (((((f->data->route_list))[j]).type.value) == ((Route_types[2]).value)) {
//            f->data->crossing_route_list.push_back(((f->data->route_list)[j]));
            my_push_back(&(f->data->crossing_route_list), ((f->data->route_list)[j]));
            
        }

    }

    f->OnComputePosition();

    event.Skip(true);

}

