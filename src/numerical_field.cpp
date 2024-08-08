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


    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetValue(wxString(""));

    //SetColor(name);
    Fill();
    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value_before_editing = value->GetValue();
    AdjustWidth(value);
  
    name->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, NON_GUI, CHECK>::Check<wxFocusEvent>, this);
  
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);

}

