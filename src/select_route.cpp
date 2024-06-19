//
//  select_route.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "select_route.h"

#include "list_frame.h"


SelectRoute::SelectRoute(ListFrame* parent_in) {

    parent = parent_in;

}


void SelectRoute::operator()(wxCommandEvent& event) {

    (parent->idling) = true;

    //temporarily unbind listcontrol_routes from &ListFrame::OnChangeSelectionInListControl
    (parent->listcontrol_routes)->Unbind(wxEVT_LIST_ITEM_SELECTED, *(parent->on_change_selection_in_listcontrol_routes));
    (parent->listcontrol_routes)->Unbind(wxEVT_LIST_ITEM_DESELECTED, *(parent->on_change_selection_in_listcontrol_routes));



    //brings parent to front
    parent->Raise();

    //deselect all previously selected items in listcontrol_routes to allow the user to properly select an item
    (parent->listcontrol_routes)->DeselectAll();

    //    (parent->listcontrol_routes)->Bind(wxEVT_LIST_ITEM_SELECTED, *(parent->on_select_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}
