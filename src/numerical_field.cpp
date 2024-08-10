//
//  numerical_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "numerical_field.h"

#include <sstream>

using namespace std;

template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::NumericalField(wxPanel* panel_of_parent, NON_GUI* object_in, NON_GUI* recent_value_in){
    
    wxSizerFlags flags;

    flags.Center();

    //set the non-GUI object
    object = object_in;
    
    //I just created *this, thus it is not being edited
    editing = false;
    //to be conservative, I set
    ok = false;

    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));
    recent_value = recent_value_in;

    //initialize check
    check = new CHECK(this);


    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
   
    
    value->SetValue(wxString(""));

    FillInRecentValue();
    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value_before_editing = value->GetValue();
    AdjustWidth(value);
  
    value->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Check<wxFocusEvent>, this);
  
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    unit->InsertIn(sizer_h, flags);
    
}


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class E> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Check(E& event) {


    //I proceed only if the progam is not is indling mode
    if (!(parent->idling)) {



    }
    
    event.Skip(true);

}


//update the value of the GUI  in NumericalField according to NuericalField<P, NON_GUI>::recent_value in such a way that the recent value appears in the GUI field
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::FillInRecentValue(void) {
   
    

}


//update the value of the GUI  in NumericalField according to NuericalField<P, NON_GUI>::recent_value in such a way that the recent value appears in the GUI field
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Enable(bool is_enabled) {
   
    value->Enable(is_enabled);
    unit->Enable(is_enabled);


}


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::InsertIn(T* host) {

    host->Add(sizer_v);

}



template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}
