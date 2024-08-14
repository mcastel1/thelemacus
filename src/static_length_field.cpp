//
//  static_length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "static_length_field.h"

#include "chart_frame.h"

//constructor of a StaticLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> StaticLengthField<P>::StaticLengthField(wxPanel* panel_of_parent, Length* length_in){

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    //set the non-GUI object
    length = length_in;
    
    value = new StaticText(parent->panel,  "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetLabel(wxString(""));
    

    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    unit->Bind(wxEVT_COMBOBOX, &StaticLengthField::OnEditUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &StaticLengthField::OnEditUnit<wxKeyEvent>, this);

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

//this method is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template <class E> void StaticLengthField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in (unit->catalog)
    find_and_replace_case_insensitive(unit->name, unit->catalog, &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        unit->name->SetForegroundColour(wxGetApp().foreground_color);
        unit->name->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    (unit->ok) = success;
    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


template<class P> template<class T> void StaticLengthField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template<class P> template<class T> void StaticLengthField<P>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}


template <class P> template <typename EventTag, typename Method, typename Object> void StaticLengthField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}
