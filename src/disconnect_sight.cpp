//
//  disconnect_sight.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "disconnect_sight.h"


//constructor of the DisconnectSight functor: parent_in is the ListFrame parent of *this, sight_id_in is the # of the Sight to be disconnected from Route # route_id_in
DisconnectSight::DisconnectSight(ListFrame* parent_in, const int& sight_id_in) : parent(parent_in), sight_id(sight_id_in) {
    
    
}


//disconnects sight i_sight from its related Route
void DisconnectSight::operator()(wxCommandEvent& event) {
    
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
    
    //the Route has been disconnected from the sight -> a new Route which is not connected to any Sight is created -> the data of the file has been modified
    parent->OnModifyFile();
    
    event.Skip(true);
    
}


// same as ConnectDisconnect::operator()(wxCommandEvent& event) but without the event argument
void DisconnectSight::operator()(void) {
    
    wxCommandEvent dummy;
    
    (*this)(dummy);
    
}
