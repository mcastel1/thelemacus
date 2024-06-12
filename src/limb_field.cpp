//
//  limb_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "limb_field.h"
#include "constants.h"
#include "string.h"

//constructor of a LimbField object, based on the parent frame frame
template<class P> LimbField<P>::LimbField(wxPanel* panel_of_parent, Limb* p) {

    long i;

    parent_frame = ((P*)(panel_of_parent->GetParent()));
    //I link the internal pointers p the respective Limb object
    limb = p;

    //initialize check
    check = new CheckLimb<P>(this);
    (check->p) = this;

    
    for(limbs.Clear(), i=0; i<Limb_types.size(); i++){limbs.push_back(wxString((Limb_types[i]).value));}

    name = new wxCheckListBox(parent_frame->panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, limbs, 0, wxDefaultValidator, wxString(""));
    name->Set(limbs);

    change_selection = new OnChangeSelectionInLimbField<P>(this);

    name->SetForegroundColour(parent_frame->GetForegroundColour());
    name->SetBackgroundColour(parent_frame->GetBackgroundColour());
    //SetColor(name);

    //uncheck all items and empty cecked_items accordingly
    for (i = 0; i < 3; i++) { name->Check(((unsigned int)i), false); }
    checked_items.Clear();
    ok = false;

    name->Bind(wxEVT_CHECKLISTBOX, (*check));
    //whenever an item is selected/deselected in name, I call change_selection->operator
    name->Bind(wxEVT_CHECKLISTBOX, *change_selection);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);

}


template<class P> bool LimbField<P>::is_ok(void) {

    return(ok);

}

template<class P> template <typename EventTag, typename Method, typename Object> void LimbField<P>::Bind(EventTag tag, Method method, Object object) {

    name->Bind(tag, method, object);

}


//writes the value contained in the GUI field into the non-GUI field
template<class P> template<class T> void LimbField<P>::get(T& event) {

    if (ok) {

        if (name->IsEnabled()) {
            //if the limb is ok and the limb wxComboBox is enabled, I set the char in (limb->value) to the first letter in the string contained in the GUI field

            //            (limb->value) = ((String((name->GetValue().ToStdString()))).value)[0];
            (limb->value) = ((String(((limbs[checked_items.Item(0)]).ToStdString()))).value)[0];

        }
        else {
            //if the limb is ok and the limb wxComboBox is disabled, then the limb is irrelevant, and I set the char in limb->value to the null char.

            (limb->value) = '\0';

        }
    }

    event.Skip(true);

}


//sets the value in the GUI object name equal to the value in the non-GUI limb object limb
template<class P> void LimbField<P>::set(void) {

    checked_items.Clear();

    if ((limb->value) == 'u') {

        name->Check(0, true);
        name->Check(1, false);
        name->Check(2, false);
        checked_items.Add(0, 1);

    }

    if ((limb->value) == 'l') {

        name->Check(0, false);
        name->Check(1, false);
        name->Check(2, true);

        checked_items.Add(2, 1);

    }

    if ((limb->value) == 'c') {

        name->Check(0, false);
        name->Check(1, true);
        name->Check(2, false);

        checked_items.Add(1, 1);

    }

    ok = true;

}

//this function enables/disable the LimbField
template<class P> void LimbField<P>::Enable(bool is_enabled) {

    name->Enable(is_enabled);

}


template<class P> template<class T> void LimbField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}
