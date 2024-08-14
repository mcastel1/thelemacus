//
//  static_length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "static_length_field.h"

#include "chart_frame.h"

//constructor of a StaticLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> StaticLengthField<P>::StaticLengthField(wxPanel* panel_of_parent, Length* p){

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    
    value = new StaticText(parent->panel,  "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetLabel(wxString(""));
    

    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    unit->Bind(wxEVT_COMBOBOX, & OnEditUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, & OnEditUnit<wxKeyEvent>, this);

    unit->Bind(wxEVT_COMBOBOX, &StaticLengthField<P>:: ConvertUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &StaticLengthField<P>:: ConvertUnit<wxKeyEvent>, this);
    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    unit-> template InsertIn<wxBoxSizer>(sizer_h, flags);

}

template class StaticLengthField<ChartFrame>;


//set the value and the unit of the GUI field *this equal to the value and the unit in the non-GUI object input
template<class P> void StaticLengthField<P>::set(const Length& l) {
    
    value->SetLabel(wxString::Format(wxT("%.*f"), display_precision.value, l.value));
    LengthField<P>::unit->set((*(l.unit)));
    
}


//set the value and the unit in the GUI object value equal to the value and the unit in the non-GUI object length
template<class P> void StaticLengthField<P>::set(void) {

    set(*(LengthField<P>::object));

    (LengthField<P>::unit->ok) = true;

}

template void StaticLengthField<ChartFrame>::set();


//convert *length to the unis of measure in the GUI field *this and write the result in *this
template<class P> void StaticLengthField<P>::set_value_keep_unit(void) {
    
    Length temp;
    
    temp = (LengthField<P>::object->convert(LengthUnit(LengthField<P>::unit->name->GetValue().ToStdString())));
    
    set(temp);

}

template void StaticLengthField<ChartFrame>::set_value_keep_unit();


//convert the numerical value stored into value according to the length unit unit
template<class P> template<class E>  void StaticLengthField<P>::ConvertUnit(E& event) {
    
  
    event.Skip(true);

}
