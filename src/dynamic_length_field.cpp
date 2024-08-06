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


#include "check_length.h"
#include "check_length_value.h"
#include "constants.h"
#include "generic.h"
#include "length.h"
#include "sight_frame.h"



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
    
    LengthField<P>::unit = new LengthUnitField<P>((LengthField<P>::parent->panel), (LengthField<P>::length->unit), &(wxGetApp().list_frame->data->recent_length_units));
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
//    LengthField<P>::unit->name->Bind(wxEVT_COMBOBOX, &LengthField<P>::template OnEditUnit<wxCommandEvent>, this);
//    LengthField<P>::unit->name->Bind(wxEVT_KEY_UP, &LengthField<P>::template OnEditUnit<wxKeyEvent>, this);

    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    LengthField<P>::sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    LengthField<P>::unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::template InsertIn<wxBoxSizer>(LengthField<P>::sizer_h, flags);

}

template class DynamicLengthField<SightFrame>;
template class DynamicLengthField<RouteFrame>;


//write the value and the unit of the GUI field in LengthField into the non-GUI field length
template<class P> template <class T> void DynamicLengthField<P>::get(T& event) {
    
    if(is_ok()){
        
        double x;
        
        value->GetValue().ToDouble(&x);
        LengthField<P>::length->set(x, LengthUnit((LengthField<P>::unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);
    
}

template void DynamicLengthField<RouteFrame>::get<wxCommandEvent>(wxCommandEvent&);
template void DynamicLengthField<SightFrame>::get<wxCommandEvent>(wxCommandEvent&);


//set the value in the GUI object value equal to the value in the non-GUI object length
template<class P> void DynamicLengthField<P>::set(void) {
    
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, LengthField<P>::length->value));
    value_ok = true;
    
    LengthField<P>::unit->set();

}

template void DynamicLengthField<RouteFrame>::set();
template void DynamicLengthField<SightFrame>::set();


template<class P> bool DynamicLengthField<P>::is_ok(void) {

    return(value_ok && (LengthField<P>::unit->ok));

}

template bool DynamicLengthField<SightFrame>::is_ok();


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

template void DynamicLengthField<RouteFrame>::OnEditValue<wxKeyEvent>(wxKeyEvent&);
template void DynamicLengthField<RouteFrame>::OnEditValue<wxCommandEvent>(wxCommandEvent&);
template void DynamicLengthField<SightFrame>::OnEditValue<wxKeyEvent>(wxKeyEvent&);
template void DynamicLengthField<SightFrame>::OnEditValue<wxCommandEvent>(wxCommandEvent&);


//this function enables/disable the DynamicLengthField
template<class P> void DynamicLengthField<P>::Enable(bool is_enabled) {

    value->Enable(is_enabled);
    LengthField<P>::unit->Enable(is_enabled);

}

template void DynamicLengthField<RouteFrame>::Enable(bool);
template void DynamicLengthField<SightFrame>::Enable(bool);


template<class P> template <typename EventTag, typename Method, typename Object> void DynamicLengthField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    LengthField<P>::unit->Bind(tag, method, object);

}

template void DynamicLengthField<RouteFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void DynamicLengthField<RouteFrame>::Bind<wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*>(wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*);
template void DynamicLengthField<SightFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);
template void DynamicLengthField<SightFrame>::Bind<wxEventTypeTag<wxCommandEvent>, void (SightFrame::*)(wxCommandEvent&), SightFrame*>(wxEventTypeTag<wxCommandEvent>, void (SightFrame::*)(wxCommandEvent&), SightFrame*);


//set the value and unit of measure in the GUI field *this equal to the value and the unit of measure in the non-GUI object *input
template<class P> void DynamicLengthField<P>::set(Length input) {
        
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, input.value));
    LengthField<P>::unit->set((*(input.unit)));
    
}

template void DynamicLengthField<RouteFrame>::set(Length);
