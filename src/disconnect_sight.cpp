//
//  disconnect_sight.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "disconnect_sight.h"


#include "on_change_selection_in_list_control.h"

//constructor of the DisconnectSight functor: parent_in is the ListFrame parent of *this, sight_id_in is the # of the Sight to be disconnected from Route # route_id_in
DisconnectSight::DisconnectSight(ListFrame* parent_in, const int& sight_id_in) : parent(parent_in), sight_id(sight_id_in) {
    
    
}


//if sight_id is valid and Sight #sight_id has a related Route contained in parent->data->route_list, disconnect Sight i_sight from its related Route. Otherwise, do nothing.
template <class E> void DisconnectSight::operator()(E& event) {
    
    if(
       (sight_id >= 0)
       && (sight_id < (parent->data->sight_list.size()))
       && (((parent->data->sight_list)[sight_id]).related_route.value >= 0)
       && (((parent->data->sight_list)[sight_id]).related_route.value < (parent->data->route_list.size()))
       ){
        
        int i_route;
        
        i_route = ((parent->data->sight_list)[sight_id]).related_route.value;
        
        //disconnect route and sight
        ((parent->data->sight_list)[sight_id]).related_route.set(-1);
        ((parent->data->route_list)[i_route]).related_sight.set(-1);
        
        //update the related wxListCtrls in ListFrame
        ((parent->data->sight_list)[sight_id]).update_ListControl(sight_id, parent->listcontrol_sights);
        ((parent->data->route_list)[i_route]).update_ListControl(i_route, parent->listcontrol_routes);
        
        //set the background color of the related sight to white
        parent->listcontrol_sights->SetItemBackgroundColour(sight_id, wxGetApp().background_color);
        
        //if an item is selected in listcontrol_sights, enable /disable button_transport_sight and button_disconnect_sight if the selected sight is related / unrelated to a Route
        if ((parent->listcontrol_sights->GetSelectedItemCount()) != 0) {
            
            bool enable;
            
            enable = ((((parent->data->sight_list)[parent->listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value != -1);
            
            parent->button_transport_sight->Enable(enable);
            parent->button_disconnect_sight->Enable(enable);
            
        }
        
        //call listcontrol_sights->set with true because I want to keep the selection in listcontrol_sights
        parent->listcontrol_sights->set((parent->data->sight_list), true);
        parent->listcontrol_positions->set((parent->data->position_list), true);
        //THIS CORRUPTS THE UNITS OF MEASURE OF route_list[i].length
        parent->listcontrol_routes->set((parent->data->route_list), false);
        //THIS CORRUPTS THE UNITS OF MEASURE OF route_list[i].length
        
        //given that I have reset the content of listcontrol_sights and listcontrol_routes, now no items will be selected in these ListControls -> I call:
        (*(parent->on_change_selection_in_listcontrol_sights))(event);
        (*(parent->on_change_selection_in_listcontrol_routes))(event);
        
        (*(parent->unset_idling))();
        parent->Resize();
        
        //the Route has been disconnected from the sight -> a new Route which is not connected to any Sight is created -> the data of the file has been modified
        parent->OnModifyFile();
        
    }
    
    event.Skip(true);
    
}

template void DisconnectSight::operator()<wxMouseEvent>(wxMouseEvent&);
template void DisconnectSight::operator()<wxTimerEvent>(wxTimerEvent&);


// same as ConnectDisconnect::operator()(wxCommandEvent& event) but without the event argument
void DisconnectSight::operator()(void) {
    
    wxCommandEvent dummy;
    
    (*this)(dummy);
    
}
