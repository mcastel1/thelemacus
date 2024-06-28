//
//  bodyfield.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "body_field.h"
#include "body.h"
#include "check_body.h"
#include "multiple_item_field.h"
#include "my_app.h"
#include "sight_frame.h"


//constructor of a BodyField object, based on panel_of_parent, which is the panel of the frame (of type P) which hosts *this
template<class P> BodyField<P>::BodyField(wxPanel* panel_of_parent, Body* p, Catalog* c, vector<int>* recent_items_in) : MultipleItemField<P, Body, CheckBody<P> >(panel_of_parent, p, c->get_names(), recent_items_in){

    catalog = c;

 
    MultipleItemField<P, Body, CheckBody<P> >::check = new CheckBody<P>(this);

    MultipleItemField<P, Body, CheckBody<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, Body, CheckBody<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, Body, CheckBody<P> >::name->Bind(wxEVT_COMBOBOX, &BodyField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, Body, CheckBody<P> >::name->Bind(wxEVT_KEY_UP, &BodyField::OnEdit<wxKeyEvent>, this);


}

template class BodyField<SightFrame>;


//sets the value in the GUI object name equal to the value in the non-GUI Body object body
template<class P> void BodyField<P>::set(void) {

    MultipleItemField<P, Body, CheckBody<P> >::name->SetValue(MultipleItemField<P, Body, CheckBody<P> >::object->name.value);
    MultipleItemField<P, Body, CheckBody<P> >::ok = true;

}

template void BodyField<SightFrame>::set();


//sets the value in the non-GUI object body equal to the value in the GUI  object name
template<class P> template<class T> void BodyField<P>::get(T& event) {

    unsigned int i;
    bool success;

    if (MultipleItemField<P, Body, CheckBody<P> >::ok) {
        //If the GUI field's content is ok...

        //I find the position of the content of the GUI field in the list of  the body names in catalog
        for (success = false, i = 0; (i < (catalog->list).size()) && (!success); i++) {
            if (String(MultipleItemField<P, Body, CheckBody<P> >::name->GetValue().ToStdString()) == (*(((catalog->list)[i]).name))) {
                success = true;
            }
        }
        i--;

        //I set the value of the non-GUI object body to the value obtained from the GUI object.
        (*(MultipleItemField<P, Body, CheckBody<P> >::object)) = (catalog->list)[i];

    }

    event.Skip(true);

}

template void BodyField<SightFrame>::get<wxCommandEvent>(wxCommandEvent&);


//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void BodyField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success;


    //I check whether the name in the GUI field body matches one of the valid body names
    find_and_replace_case_insensitive(MultipleItemField<P, Body, CheckBody<P> >::name, MultipleItemField<P, Body, CheckBody<P> >::items, &success, &i);

    if (success) {
        //the text entered in name is valid

        //I enable the limb field if and only if the selected body allows for a field and I run check on the existing text in the limb field
        MultipleItemField<P, Body, CheckBody<P> >::parent->limb->name->Enable(((MultipleItemField<P, Body, CheckBody<P> >::items)[i] == wxString("sun")) || ((MultipleItemField<P, Body, CheckBody<P> >::items)[i] == wxString("moon")));
        (*(MultipleItemField<P, Body, CheckBody<P> >::parent->limb->check))(event);

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, Body, CheckBody<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, Body, CheckBody<P> >::name->SetFont(wxGetApp().default_font);

    }
    else {
        //the text entered in name is not valid: disable parent_frame->limb and set limb->ok to false because the body related to limb is invalid

        MultipleItemField<P, Body, CheckBody<P> >::parent->limb->name->Enable(false);
        MultipleItemField<P, Body, CheckBody<P> >::parent->limb->ok = false;

    }


    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, Body, CheckBody<P> >::ok = success;
    //tries to enable button_reduce
    MultipleItemField<P, Body, CheckBody<P> >::parent->AllOk();

    event.Skip(true);

}
