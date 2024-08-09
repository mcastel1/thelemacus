//
//  numerical_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "numerical_field.h"

#include <sstream>

using namespace std;

template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK> NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK>::NumericalField(wxPanel* panel_of_parent, NON_GUI* object_in, NON_GUI* recent_value_in){
    

    //set the non-GUI object
    object = object_in;
    
    //I just created *this, thus it is not being edited
    editing = false;
    //to be conservative, I set
    ok = false;

    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));
    recent_value = recent_value_in;


    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetValue(wxString(""));

    FillInRecentValue();
    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value_before_editing = value->GetValue();
    AdjustWidth(value);
  
    value->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK>::Check<wxFocusEvent>, this);
  
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);

}


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK> template<class E> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK>::Check(E& event) {


    //I proceed only if the progam is not is indling mode
    if (!(parent->idling)) {



    }
    
    event.Skip(true);

}


//update the value of the GUI  in NumericalField according to NuericalField<P, NON_GUI>::recent_value in such a way that the recent value appears in the GUI field
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK>::FillInRecentValue(void) {
   
    

}


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK> template<class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK>::InsertIn(T* host) {

    host->Add(sizer_v);

}



template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK> template<class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}
