//
//  projection_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "projection_field.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "constants.h"
#include "generic.h"
#include "my_app.h"
#include "my_string.h"
#include "projection.h"


//constructor of a ProjectionField object, based on the parent frame frame
template<class P> ProjectionField<P>::ProjectionField(wxPanel* panel_of_parent, Projection* object_in, vector<int>* recent_items_in) : MultipleItemField<P, Projection, CheckProjection<P> >(panel_of_parent, object_in, Projection_types, recent_items_in){

    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, Projection, CheckProjection<P>>::name->Bind(wxEVT_COMBOBOX, &ProjectionField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, Projection, CheckProjection<P>>::name->Bind(wxEVT_KEY_UP, &ProjectionField::OnEdit<wxKeyEvent>, this);

}


//this function is called every time the user modifies the text in this->name: it checks whether the text entered so far in name is valid, if name is valid, it calls OnChooseProjection to select the projection written in name
template<class P> template<class E> void ProjectionField<P>::OnEdit(E& event) {

    String s;
    bool success;
    
    if(!(MultipleItemField<P, Projection, CheckProjection<P> >::editing)){
        //*the user has started editing *this
        (MultipleItemField<P, Projection, CheckProjection<P> >::editing) = true;
    }

    //I check whether the name in the GUI field body_name matches one of the body names in catalog
    find_and_replace_case_insensitive(MultipleItemField<P, Projection, CheckProjection<P> >::name, MultipleItemField<P, Projection, CheckProjection<P> >::items, &success, NULL);

    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, Projection, CheckProjection<P> >::ok = success;

    if (success) {

        MultipleItemField<P, Projection, CheckProjection<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, Projection, CheckProjection<P> >::name->SetFont(wxGetApp().default_font);

    }


    event.Skip(true);

}

template class ProjectionField<ChartFrame>;
