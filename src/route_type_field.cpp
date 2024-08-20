//
//  route_type_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "route_type_field.h"

#include "check_route_type.h"
#include "constants.h"
#include "generic.h"
#include "route_type.h"
#include "my_string.h"
#include "my_app.h"


//constructor of a RouteTypeField object, based on the parent frame frame
template<class P> RouteTypeField<P>::RouteTypeField(wxPanel* panel_of_parent, RouteType* object_in, const vector<RouteType>& route_types_in, vector<int>* recent_items_in) : MultipleItemField<P, RouteType, CheckRouteType<P> >(panel_of_parent, object_in, route_types_in, recent_items_in) {

    MultipleItemField<P, RouteType, CheckRouteType<P> >::check = new CheckRouteType<P>(this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, RouteType, CheckRouteType<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_COMBOBOX, &RouteTypeField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KEY_UP, &RouteTypeField::OnEdit<wxKeyEvent>, this);

}

template class RouteTypeField<RouteFrame>;


//sets the value in the GUI object equal to the value in the non-GUI  object 
template<class P> void RouteTypeField<P>::set(void) {
    
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString(
                                                                                 (Route_types[
                                                                                              position_in_vector((*(MultipleItemField<P, RouteType, CheckRouteType<P> >::object)), Route_types)
                                                                                              ]).value
                                                                                 ));
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = true;
    
}

template void RouteTypeField<RouteFrame>::set();


//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void RouteTypeField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success;

    //I check whether the name in the GUI field  matches one of the entries  of catalog
    find_and_replace_case_insensitive(MultipleItemField<P, RouteType, CheckRouteType<P> >::name, MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog, &success, &i);
    
    if (success) {
        //the text entered in name is valid

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetFont(wxGetApp().default_font);

    }

    //ok is true/false is the text entered is valid/invalid
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = success;
    
    //try to enable fields related to Route type
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->EnableDisableRouteTypeFields(event);
    //try to enable fields related to Length format
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->EnableDisableLengthFormatFields(event);
    //try to enable button_reduce
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->AllOk();

    event.Skip(true);

}


template<class P> void RouteTypeField<P>::OnEdit(void){
    
    wxCommandEvent dummy;
    
    OnEdit(dummy);
    
}

template void RouteTypeField<RouteFrame>::OnEdit();
