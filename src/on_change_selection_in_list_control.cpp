//
//  on_change_selection_in_list_control.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "on_change_selection_in_list_control.h"

#include "generic.h"
#include "listcontrol_type.h"


template<class S> OnChangeSelectionInListControl<S>::OnChangeSelectionInListControl(ListControl<S>* caller_in, String type_in) {

    caller = caller_in;
    type = type_in;

}

template class OnChangeSelectionInListControl<Route>;
template class OnChangeSelectionInListControl<Sight>;
template class OnChangeSelectionInListControl<Position>;


//check whether there are some selected items in ListControl *caller, and enables/disables disableable_buttons accordingly
template<class S> template<class T>void OnChangeSelectionInListControl<S>::operator()(T& event) {
    
    long i;
    //a pointer to the ListFrame
    ListFrame* list_frame_pointer;
    
    list_frame_pointer = ((ListFrame*)(caller->GetParent()->GetParent()));
    
    for (i = 0; i < (caller->disableable_buttons).size(); i++) {
        (caller->disableable_buttons)[i]->Enable(caller->GetSelectedItemCount() != 0);
    }
    
    i = (caller->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
    
    if (i != -1) {
        //one item is selected
        
        switch (position_in_vector(type, ListControl_types)) {
                
            case 0: {
                //type.value = "sight"->caller is listcontrol_sights
                
                //the selected Sight has/does not have a related Route -> enable/disable the buttons to disconnect and transport the selected Sight
                list_frame_pointer->button_disconnect_sight->Enable(((((list_frame_pointer->data->sight_list)[i]).related_route.get()) != -1));
                list_frame_pointer->button_transport_sight->Enable(((((list_frame_pointer->data->sight_list)[i]).related_route.get()) != -1));
                
                break;
                
            }
                
            case 2: {
                //type.value = "route" -> caller is listcontrol_routes
                
                //the selected Route has/does not have a related Sight -> enable/disable the button to disconnect the selected Route
                list_frame_pointer->button_disconnect_route->Enable(((((list_frame_pointer->data->route_list)[i]).related_sight.get()) != -1));
                
                break;
                
            }
                
        }
        
    }
    else {
        //no item is selected -> disable the button to disconnect
        
        switch(position_in_vector(type, ListControl_types)) {
                
            case 0: {
                //type.value = "sight"->caller is listcontrol_sights
                
                list_frame_pointer->button_disconnect_sight->Enable(false);
                
                break;
                
            }
                
            case 2: {
                //type.value = "route" -> caller is listcontrol_routes
                
                list_frame_pointer->button_disconnect_route->Enable(false);
                
                break;
                
            }
                
        }
        
        
    }
    
    event.Skip(true);
    
}

template void OnChangeSelectionInListControl<Route>::operator()<wxCommandEvent>(wxCommandEvent&);
template void OnChangeSelectionInListControl<Position>::operator()<wxListEvent>(wxListEvent&);
template void OnChangeSelectionInListControl<Position>::operator()<wxCommandEvent>(wxCommandEvent&);
template void OnChangeSelectionInListControl<Route>::operator()<wxListEvent>(wxListEvent&);
template void OnChangeSelectionInListControl<Sight>::operator()<wxCommandEvent>(wxCommandEvent&);
template void OnChangeSelectionInListControl<Sight>::operator()<wxListEvent>(wxListEvent&);
template void OnChangeSelectionInListControl<Route>::operator()<wxMouseEvent>(wxMouseEvent&);
template void OnChangeSelectionInListControl<Route>::operator()<wxTimerEvent>(wxTimerEvent&);
template void  OnChangeSelectionInListControl<Sight>::operator()<wxMouseEvent>(wxMouseEvent&);
template void OnChangeSelectionInListControl<Sight>::operator()<wxTimerEvent>(wxTimerEvent&);


//same as template<class S> template<class T>void OnChangeSelectionInListControl<S>::operator()(T& event) but with no event argument
template<class S> void OnChangeSelectionInListControl<S>::operator()(void){
    
    wxCommandEvent dummy;
    
    (*this)(dummy);

}

template void OnChangeSelectionInListControl<Route>::operator()();
template void OnChangeSelectionInListControl<Sight>::operator()();
template void OnChangeSelectionInListControl<Position>::operator()();
