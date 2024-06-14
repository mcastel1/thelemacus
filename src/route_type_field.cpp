//
//  route_type_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "route_type_field.h"

#include "constants.h"
#include "route_type.h"
#include "my_string.h"


//constructor of a RouteTypeField object, based on the parent frame frame
template<class P> RouteTypeField<P>::RouteTypeField(wxPanel* panel_of_parent, RouteType* object_in, const vector<String>& route_types_in, vector<int>* recent_items_in) : MultipleItemField<P, RouteType, CheckRouteType<P> >(panel_of_parent, object_in, route_types_in, recent_items_in) {

    MultipleItemField<P, RouteType, CheckRouteType<P> >::check = new CheckRouteType<P>(this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, RouteType, CheckRouteType<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_COMBOBOX, &RouteTypeField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KEY_UP, &RouteTypeField::OnEdit<wxKeyEvent>, this);

}



//sets the value in the GUI object equal to the value in the non-GUI  object string
template<class P> void RouteTypeField<P>::set(void) {
    
    switch (/*( (MultipleItemField<P, RouteType, CheckRouteType<P> >::object->value)[0])*/ (MultipleItemField<P, RouteType, CheckRouteType<P> >::object)->position_in_list(Route_types)) {
            
        case 0: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[0]).value));
            
            break;
            
        }
            
        case 1: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[1]).value));
            
            break;
            
        }
            
        case 2: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[2]).value));
            
            break;
            
        }
            
    }
    
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = true;
    
}


//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void RouteTypeField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success, enable;


    //I check whether the name in the GUI field  matches one of the entries  of catalog
    find_and_replace_case_insensitive(MultipleItemField<P, RouteType, CheckRouteType<P> >::name, MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog, &success, &i);

    if (success) {
        //the text entered in name is valid

        //enable/disable the related fields in RouteFrame f
        enable = ((((MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog)[i]) == wxString(((Route_types[0]).value))) || (((MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog)[i]) == wxString(((Route_types[1]).value))));

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->Z->Enable(enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_phi->Enable(enable && (!(MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->for_transport)));
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_lambda->Enable(enable && (!(MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->for_transport)));

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_phi->Enable(!enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_lambda->Enable(!enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->omega->Enable(!enable);

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetFont(wxGetApp().default_font);

    }
    else {

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->Z->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_phi->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_lambda->Enable(false);

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_phi->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_lambda->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->omega->Enable(false);

    }

    //ok is true/false is the text entered is valid/invalid
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = success;

    //try to enable time, speed and length
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->OnChooseLengthFormatField(event);

    //try to enable button_reduce
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->AllOk();

    event.Skip(true);

}

template<class P> void RouteTypeField<P>::OnEdit(void){
    
    wxCommandEvent dummy;
    
    OnEdit(dummy);
    
    
}
