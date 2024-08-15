//
//  static_length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "static_length_field.h"

#include "chart_frame.h"
#include "generic.h"


//constructor of a StaticLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> StaticLengthField<P>::StaticLengthField(wxPanel* panel_of_parent, Length* length_in, vector<int>* recent_units_in){

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    //set the non-GUI object
    length = length_in;
    
    
    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));

    
    value = new StaticText(parent->panel,  "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetLabel(wxString(""));
    

    unit =
    new MultipleItemField<P, LengthUnit, CheckUnit<P, StaticLengthField<P>>>(
                                                                             parent->panel,
                                                                             length->unit,
                                                                             LengthUnit_types,
                                                                             recent_units_in);
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    unit->Bind(wxEVT_COMBOBOX, &StaticLengthField::OnEditUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &StaticLengthField::OnEditUnit<wxKeyEvent>, this);

    unit->Bind(wxEVT_COMBOBOX, &StaticLengthField<P>:: ConvertUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &StaticLengthField<P>:: ConvertUnit<wxKeyEvent>, this);
//    unit->Bind(wxEVT_KILL_FOCUS, &StaticLengthField::Check<wxFocusEvent>, this)
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    
    unit-> template InsertIn<wxBoxSizer>(sizer_h, flags);

}

template class StaticLengthField<ChartFrame>;


//set the value and the unit of the GUI field *this equal to the value and the unit in the non-GUI object input
template<class P> void StaticLengthField<P>::set(const Length& l) {
    
    value->SetLabel(wxString::Format(wxT("%.*f"), display_precision.value, l.value));
    unit->set((*(l.unit)));
    
}


//set the value and the unit in the GUI object value equal to the value and the unit in the non-GUI object length
template<class P> void StaticLengthField<P>::set(void) {

    set(*(length));

}

template void StaticLengthField<ChartFrame>::set();


//convert write into the GUI field a Length equal to *length but converted to the most recent unit of measure stored in unit->recent_items->front()
template<class P> void StaticLengthField<P>::SetValueInMostRecentUnit(void) {

    set((length->convert(LengthUnit_types[unit->recent_items->front()])));

}

template void StaticLengthField<ChartFrame>::SetValueInMostRecentUnit();


//convert *length to the unis of measure in the GUI field *this and write the result in *this
template<class P> void StaticLengthField<P>::SetValueKeepUnit(void) {
    
    Length temp;
    
    temp = (length->convert(LengthUnit(unit->name->GetValue().ToStdString())));
    
    set(temp);

}

template void StaticLengthField<ChartFrame>::SetValueKeepUnit();


//convert the numerical value stored into value according to the length unit unit
template<class P> template<class E>  void StaticLengthField<P>::ConvertUnit(E& event) {
    
    bool success;
    unsigned int i;
    
    find_and_replace_case_insensitive(unit->name, unit->catalog, &success, &i);
    if(success){
                
        set((length->convert(LengthUnit_types[i])));
        
    }

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

template void StaticLengthField<ChartFrame>::InsertIn<wxBoxSizer>(wxBoxSizer*, wxSizerFlags&);


template <class P> template <typename EventTag, typename Method, typename Object> void StaticLengthField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}
