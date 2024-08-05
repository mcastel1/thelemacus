//
//  body_name_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "body_name_field.h"

#include "body.h"
#include "check_body_name.h"
#include "multiple_item_field.h"
#include "my_app.h"
#include "sight_frame.h"


//constructor of a BodyNameField object, based on panel_of_parent, which is the panel of the frame (of type P) which hosts *this
template<class P> BodyNameField<P>::BodyNameField(wxPanel* panel_of_parent, BodyName* p, vector<int>* recent_items_in) : MultipleItemField<P, BodyName, CheckBodyName<P> >(panel_of_parent, p, wxGetApp().catalog->get_names(), recent_items_in){
 
    MultipleItemField<P, BodyName, CheckBodyName<P> >::check = new CheckBodyName<P>(this);

    MultipleItemField<P, BodyName, CheckBodyName<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, BodyName, CheckBodyName<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, BodyName, CheckBodyName<P> >::name->Bind(wxEVT_COMBOBOX, &BodyNameField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, BodyName, CheckBodyName<P> >::name->Bind(wxEVT_KEY_UP, &BodyNameField::OnEdit<wxKeyEvent>, this);


}

template class BodyNameField<SightFrame>;


//sets the value in the GUI object name equal to the value in the non-GUI Body object body_name
template<class P> void BodyNameField<P>::set(void) {

    MultipleItemField<P, BodyName, CheckBodyName<P> >::name->SetValue(MultipleItemField<P, BodyName, CheckBodyName<P> >::object->value);
    MultipleItemField<P, BodyName, CheckBodyName<P> >::ok = true;

}

template void BodyNameField<SightFrame>::set();


//sets the value in the non-GUI object body equal to the value in the GUI  object name
template<class P> template<class T> void BodyNameField<P>::get(T& event) {

    unsigned int i;
    bool success;

    if (MultipleItemField<P, BodyName, CheckBodyName<P> >::ok) {
        //If the GUI field's content is ok...

        //I find the position of the content of the GUI field in the list of  the body names in catalog
        for (success = false, i = 0; (i < (wxGetApp().catalog->list.size())) && (!success); i++) {
            if (String(MultipleItemField<P, BodyName, CheckBodyName<P> >::name->GetValue().ToStdString()) == (*(((wxGetApp().catalog->list)[i]).name))) {
                success = true;
            }
        }
        i--;

        //I set the value of the non-GUI object body to the value obtained from the GUI object.
        (*(MultipleItemField<P, BodyName, CheckBodyName<P> >::object)) = (wxGetApp().catalog->list)[i];

    }

    event.Skip(true);

}

template void BodyNameField<SightFrame>::get<wxCommandEvent>(wxCommandEvent&);


//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void BodyNameField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success;


    //I check whether the name in the GUI field body matches one of the valid body names
    find_and_replace_case_insensitive(MultipleItemField<P, BodyName, CheckBodyName<P> >::name, MultipleItemField<P, BodyName, CheckBodyName<P> >::items, &success, &i);

    if (success) {
        //the text entered in name is valid

        //I enable the limb field if and only if the selected body allows for a field and I run check on the existing text in the limb field
        MultipleItemField<P, BodyName, CheckBodyName<P> >::parent->limb->name->Enable(((MultipleItemField<P, BodyName, CheckBodyName<P> >::items)[i] == wxString("sun")) || ((MultipleItemField<P, BodyName, CheckBodyName<P> >::items)[i] == wxString("moon")));
        (*(MultipleItemField<P, BodyName, CheckBodyName<P> >::parent->limb->check))(event);

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, BodyName, CheckBodyName<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, BodyName, CheckBodyName<P> >::name->SetFont(wxGetApp().default_font);

    }
    else {
        //the text entered in name is not valid: disable parent_frame->limb and set limb->ok to false because the body related to limb is invalid

        MultipleItemField<P, BodyName, CheckBodyName<P> >::parent->limb->name->Enable(false);
        MultipleItemField<P, BodyName, CheckBodyName<P> >::parent->limb->ok = false;

    }


    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, BodyName, CheckBodyName<P> >::ok = success;
    //tries to enable button_reduce
    MultipleItemField<P, BodyName, CheckBodyName<P> >::parent->AllOk();

    event.Skip(true);

}
