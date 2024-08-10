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
template<class P> SpeedField<P>::SpeedField(wxPanel* panel_of_parent, Speed* object_in, Speed* recent_value_in) : NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >(panel_of_parent, object_in, recent_value_in){

    parent = ((P*)(panel_of_parent->GetParent()));
    speed = p;
    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    
    //initialize check
    check = new CheckSpeed<P>(this);

    flags.Center();

    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Speed object
    value->SetValue(wxString(""));
    value_ok = false;
    value->Bind(wxEVT_KILL_FOCUS, (*(check->check_speed_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    value->Bind(wxEVT_KEY_UP, &SpeedField::OnEditValue<wxKeyEvent>, this);


//    (unit->name) = new wxComboBox((parent->panel), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, units, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    unit = new SpeedUnitField<P>(parent->panel, &(speed->unit), &(wxGetApp().list_frame->data->recent_speed_units));
    
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    unit->Bind(wxEVT_COMBOBOX, &SpeedField::OnEditUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &SpeedField::OnEditUnit<wxKeyEvent>, this);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    unit->InsertIn(sizer_h, flags);
    
}

//explicit instantiations


//set the value in the GUI object value equal to the value in the non-GUI object speed
template<class P> void SpeedField<P>::set(void) {
        
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, SpeedField<P>::speed->value));
    value_ok = true;
    
    SpeedField<P>::unit->set();
  
}

//explicit instantiations


//write the value and the unit of the GUI field in SpeedField into the non-GUI field speed
template<class P> template <class T> void SpeedField<P>::get(T& event) {

    if(is_ok()){
        
        double x;
        
        value->GetValue().ToDouble(&x);
        speed->set(x, SpeedUnit((unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);

}

//explicit instantiations


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditValue(E& event) {

    bool success;

    success = check_double((value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        value->SetForegroundColour(wxGetApp().foreground_color);
        value->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    value_ok = success;
    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}

//explicit instantiations


//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in (unit->catalog)
    find_and_replace_case_insensitive(unit->name, (unit->catalog), &success, NULL);


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

//explicit instantiations


//enable/disable the SpeedField
//template<class P> void SpeedField<P>::Enable(bool is_enabled) {
//
//    value->Enable(is_enabled);
//    unit->Enable(is_enabled);
//
//}

//explicit instantiations


template<class P> template<class T> void SpeedField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

//explicit instantiations


template<class P> template <typename EventTag, typename Method, typename Object> void SpeedField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}

//explicit instantiations


template<class P> bool SpeedField<P>::is_ok(void) {

    return(value_ok && (unit->ok));

}

