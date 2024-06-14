//
//  dynamic_length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "dynamic_length_field.h"

#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/string.h>
#include <wx/textctrl.h>


#include "constants.h"
#include "length.h"



//constructor of a EditableLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> DynamicLengthField<P>::DynamicLengthField(wxPanel* panel_of_parent, Length* p) : LengthField<P>( panel_of_parent, p) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    
    //initialize check
    check = new CheckLength<P>(this);
    
    value = new wxTextCtrl((LengthField<P>::parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Length object
    value->SetValue(wxString(""));
    value_ok = false;
    value->Bind(wxEVT_KILL_FOCUS, (*(check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
//    value->Bind(wxEVT_KEY_UP, &DynamicLengthField::OnEditValue<wxKeyEvent>, this);
    
    LengthField<P>::unit = new LengthUnitField<P>((LengthField<P>::parent->panel), &(LengthField<P>::length->unit), &(wxGetApp().list_frame->data->recent_length_units));
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
//    LengthField<P>::unit->name->Bind(wxEVT_COMBOBOX, &LengthField<P>::template OnEditUnit<wxCommandEvent>, this);
//    LengthField<P>::unit->name->Bind(wxEVT_KEY_UP, &LengthField<P>::template OnEditUnit<wxKeyEvent>, this);

    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    LengthField<P>::sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    LengthField<P>::unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::template InsertIn<wxBoxSizer>(LengthField<P>::sizer_h, flags);

}

//write the value and the unit of the GUI field in LengthField into the non-GUI field length
template<class P> template <class T> void DynamicLengthField<P>::get(T& event) {
    
    if(is_ok()){
        
        double x;
        
        value->GetValue().ToDouble(&x);
        LengthField<P>::length->set(x, LengthUnit((LengthField<P>::unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);
    
}


//set the value in the GUI object value equal to the value in the non-GUI object length
template<class P> void DynamicLengthField<P>::set(void) {
    
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, LengthField<P>::length->value));
    value_ok = true;
    
    LengthField<P>::unit->set();

}



//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date_in
template<class P> void DateField<P>::set(Date date_in) {

    //    Time time_UTC;
    //
    //    //(parent_frame->sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    //    time_UTC = (parent_frame->sight->time);
    //    time_UTC -= (parent_frame->sight->TAI_minus_UTC);

    year->SetValue(wxString::Format(wxT("%i"), date_in.Y));
    month->SetValue(wxString::Format(wxT("%i"), date_in.M));
    day->SetValue(wxString::Format(wxT("%i"), date_in.D));

    year_ok = true;
    month_ok = true;
    day_ok = true;

}

template<class P> bool DynamicLengthField<P>::is_ok(void) {

    return(value_ok && (LengthField<P>::unit->ok));

}


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void DynamicLengthField<P>::OnEditValue(E& event) {

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
//    LengthField<P>::parent->AllOk();

    event.Skip(true);

}


//this function enables/disable the DynamicLengthField
template<class P> void DynamicLengthField<P>::Enable(bool is_enabled) {

    value->Enable(is_enabled);
    LengthField<P>::unit->Enable(is_enabled);

}


template<class P> template <typename EventTag, typename Method, typename Object> void DynamicLengthField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    LengthField<P>::unit->Bind(tag, method, object);

}
