//
//  speed_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "speed_field.h"


#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/textctrl.h>

#include "check_speed.h"
#include "check_speed_value.h"
#include "constants.h"
#include "generic.h"
#include "speed_unit_field.h"
#include "my_app.h"


//constructor of a SpeedField object, based on the parent frame frame
template<class P> SpeedField<P>::SpeedField(wxPanel* panel_of_parent, Speed* object_in, double* recent_value_in, vector<int>* recent_units_in) : NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >(panel_of_parent, object_in, recent_value_in, recent_units_in){

    wxSizerFlags flags;

    flags.Center();

    
//    parent = ((P*)(panel_of_parent->GetParent()));
//    speed = p;
    //these flags will be used in the method InsertIn below, to insert this->unit
    //    wxSizerFlags flags;
    
    //unit is allocated here and not in NumericalField constructor because it needs the argument SpeedUnit_types, which is specifit to SpeedField
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit =
    new SpeedUnitField<P>(
                                                                     NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::parent->panel,
                                                                     &(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::object->unit),
                                                recent_units_in);
    
    //allocate check
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::check = new CheckSpeed<P>(this);

//    flags.Center();

//    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->SetInitialSize(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetSizeFromTextSize(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Speed object
//    value->SetValue(wxString(""));
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::check->check_speed_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>>>::value->Bind(wxEVT_KEY_UP, &SpeedField::OnEditValue<wxKeyEvent>, this);


//    (unit->name) = new wxComboBox((parent->panel), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, units, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
//    unit = new SpeedUnitField<P>(parent->panel, &(speed->unit), &(wxGetApp().list_frame->data->recent_speed_units));

    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->Bind(wxEVT_COMBOBOX, &SpeedField::OnEditUnit<wxCommandEvent>, this);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->Bind(wxEVT_KEY_UP, &SpeedField::OnEditUnit<wxKeyEvent>, this);

    
    //    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    //    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    //    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
    //    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    //    unit->InsertIn(sizer_h, flags);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->InsertIn(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::sizer_h, flags);

    
}

//explicit instantiations
template class SpeedField<RouteFrame>;


//set the value in the GUI object value equal to the value in the non-GUI object speed
template<class P> void SpeedField<P>::set(void) {
        
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, SpeedField<P>::object->value));
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value_ok = true;
    
    SpeedField<P>::unit->set();
  
}

//explicit instantiations
template void SpeedField<RouteFrame>::set();


//write the value and the unit of the GUI field in SpeedField into the non-GUI field speed
template<class P> template <class T> void SpeedField<P>::get(T& event) {

    if(is_ok()){
        
        double x;
        
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetValue().ToDouble(&x);
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::object->set(x, SpeedUnit((NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);

}

//explicit instantiations
template void SpeedField<RouteFrame>::get<wxCommandEvent>(wxCommandEvent&);


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditValue(E& event) {

    bool success;

    success = check_double((NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->SetForegroundColour(wxGetApp().foreground_color);
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value_ok = success;
    //tries to enable button_reduce
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::parent->AllOk();

    event.Skip(true);

}

//explicit instantiations
template void SpeedField<RouteFrame>::OnEditValue<wxKeyEvent>(wxKeyEvent&);

//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in (unit->catalog)
    find_and_replace_case_insensitive(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->name, (NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->catalog), &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->name->SetForegroundColour(wxGetApp().foreground_color);
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->name->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    (NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->ok) = success;
    //tries to enable button_reduce
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::parent->AllOk();

    event.Skip(true);

}

//explicit instantiations
template void SpeedField<RouteFrame>::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void  SpeedField<RouteFrame>::OnEditUnit<wxCommandEvent>(wxCommandEvent&);


//enable/disable the SpeedField
//template<class P> void SpeedField<P>::Enable(bool is_enabled) {
//
//    value->Enable(is_enabled);
//    unit->Enable(is_enabled);
//
//}

//explicit instantiations


//template<class P> template<class T> void SpeedField<P>::InsertIn(T* host) {
//
//    host->Add(sizer_v);
//
//}

//explicit instantiations


template<class P> template <typename EventTag, typename Method, typename Object> void SpeedField<P>::Bind(EventTag tag, Method method, Object object) {

    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}

//explicit instantiations
template void SpeedField<RouteFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);


template<class P> bool SpeedField<P>::is_ok(void) {

    return(value_ok && (unit->ok));

}

