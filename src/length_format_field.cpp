//
//  length_format_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <wx/event.h>
#include <wx/panel.h>

#include "length_format.h"
#include "length_format_field.h"
#include "multiple_item_field.h"
#include "my_app.h"


//constructor of a LengthFormatField object, based on the parent frame frame
template<class P> LengthFormatField<P>::LengthFormatField(wxPanel* panel_of_parent, LengthFormat* object_in, vector<int>* recent_items_in)  : MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >(panel_of_parent, object_in, LengthFormat_types, recent_items_in){

    
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::check = new CheckLengthFormat<P>(this);
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::check));

    
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_COMBOBOX, &LengthFormatField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_KEY_UP, &LengthFormatField::OnEdit<wxKeyEvent>, this);

}

template class LengthFormatField<RouteFrame>;


//this function is called every time the user modifies the text in this->name: it checks whether the text entered so far in name is valid, if name is valid, it calls OnChooseLengthFormat to select the projection written in name
template<class P> template<class E> void LengthFormatField<P>::OnEdit(E& event) {

    String s;
    bool success;

    //I check whether the name in the GUI field body matches one of the body names in catalog
    find_and_replace_case_insensitive(MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name, MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::items, &success, NULL);

    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::ok = success;

    if (success) {

        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->SetFont(wxGetApp().default_font);
        //choses the length format entered in name button_reduce
        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::parent->OnChooseLengthFormatField(event);

    }

    event.Skip(true);

}

template void LengthFormatField<RouteFrame>::OnEdit<wxKeyEvent>(wxKeyEvent&);
template void LengthFormatField<RouteFrame>::OnEdit<wxCommandEvent>(wxCommandEvent&);
