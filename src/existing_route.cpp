//
//  existing_route.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "existing_route.h"

#include "list_frame.h"
#include "my_string.h"
#include "show_question_frame.h"


ExistingRoute::ExistingRoute(ListFrame* f_in) {

    f = f_in;

}



void ExistingRoute::operator()(wxCommandEvent& event) {

    //by setting this to true, when the user presses enter, the method ListFrame::KeyDown will call on_select_route_in_listcontrol_routes_for_transport
    (f->transporting_with_selected_route) = true;

    //save data->route_list into route_list_saved
    f->route_list_saved.resize(((f->data)->route_list).size());
    copy((f->data->route_list).begin(), (f->data->route_list).end(), (f->route_list_saved).begin());

    //print an info message
    (f->print_question_message)->SetAndCall(NULL, String(""), String("You are about to transport with an existing route. Select the Route and press enter.\nDo you want to continue?"), String("Yes"), String("No, I want to cancel"));

    event.Skip(true);

}
