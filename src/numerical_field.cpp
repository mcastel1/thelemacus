//
//  numerical_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "numerical_field.h"

#include <sstream>

using namespace std;

template<class P, class NON_GUI, class CHECK> NumericalField<P, NON_GUI, CHECK>::NumericalField(wxPanel* panel_of_parent, NON_GUI* object_in, vector<NON_GUI>* recent_items_in){
    
    unsigned int i;

    //set the non-GUI object
    object = object_in;
    
    //I just created *this, thus it is not being edited
    editing = false;
    //to be conservative, I set
    ok = false;

    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));
    recent_values = recent_values_in;


    name = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(name);
    Fill();
    name->SetValue(items[0]);
    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value_before_editing = name->GetValue();
    AdjustWidth(name);
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call Check to check whether what has been entered in name is valid
    //    name->Bind(wxEVT_COMBOBOX, &NumericalField::OnEdit<wxCommandEvent>, this);
    //    name->Bind(wxEVT_KEY_UP, &NumericalField::OnEdit<wxKeyEvent>, this);
    //    name->Bind(wxEVT_KILL_FOCUS, *check);
    name->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, NON_GUI, CHECK>::Check<wxFocusEvent>, this);
    //THIS LINE CAUSES AN ERROR
    //    name->Bind(wxEVT_COMBOBOX, &NumericalField<P, NON_GUI, CHECK>::Check<wxCommandEvent>, this);

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);

}

