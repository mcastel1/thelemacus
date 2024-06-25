//
//  length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "dynamic_length_field.h"
#include "length.h"
#include "length_field.h"
#include "sight_frame.h"




//set the value and unit of measure in the GUI field *this equal to the value and the unit of measure in the non-GUI object *input
template<class P> void DynamicLengthField<P>::set(Length input) {
        
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, input.value));
    LengthField<P>::unit->set(input.unit);
    
}




//constructor of a LengthField object, based on the parent frame frame
template<class P> LengthField<P>::LengthField(wxPanel* panel_of_parent, Length* p){

    parent = ((P*)(panel_of_parent->GetParent()));
    length = p;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);

}


//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template<class E> void LengthField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in units
    find_and_replace_case_insensitive(unit->name, unit->catalog, &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        unit->name->SetForegroundColour(wxGetApp().foreground_color);
        unit->name->SetFont(wxGetApp().default_font);

    }

    (unit->ok) = success;

    event.Skip(true);

}

template  void LengthField<RouteFrame>::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void LengthField<RouteFrame>::OnEditUnit<wxCommandEvent>(wxCommandEvent&);
template void LengthField<SightFrame>::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void LengthField<SightFrame>::OnEditUnit<wxCommandEvent>(wxCommandEvent&);


//insert *this in *host
template<class P> template<class T> void LengthField<P>::InsertIn(T* host) {

    host->Add(LengthField<P>::sizer_v);

}

template void LengthField<RouteFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template  void LengthField<SightFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);


//same asLengthField<P>::InsertIn(T* host) but with flags to be provided
template<class P> template<class T> void LengthField<P>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}

template void LengthField<ChartFrame>::InsertIn<wxBoxSizer>(wxBoxSizer*, wxSizerFlags&);
