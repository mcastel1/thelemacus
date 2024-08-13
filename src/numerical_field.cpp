//
//  numerical_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "numerical_field.h"

#include "date_field.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/textctrl.h>

#include "check_speed.h"
#include "constants.h"
#include "generic.h"
#include "my_app.h"
#include "sight_frame.h"

#include <sstream>

using namespace std;

template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::NumericalField(wxPanel* panel_of_parent, NON_GUI* object_in, NON_GUI* recent_object_in, vector<int>* recent_units_in){
    
    //set the non-GUI object
    object = object_in;
    
    //I just created *this, thus it is not being edited
    editing = false;
    //to be conservative, I set
    ok = false;
    value_ok = false;

    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));
    recent_object = recent_object_in;

    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetValue(wxString(""));

    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value->GetValue().ToDouble(&(object_before_editing.value));
    //    AdjustWidth(value);
  
    value->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Check<wxFocusEvent>, this);
  
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    
}

template class NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >;
template class NumericalField<ChartFrame, Length, LengthUnit, CheckLength<ChartFrame>, CheckUnit<ChartFrame, LengthField<ChartFrame> > >;
template class NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >;
template class NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >;


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class E> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Check(E& event) {

    //I proceed only if the progam is not is indling mode
    if (!(parent->idling)) {

        (*check)(event);

        if (is_ok() ||
            (
             ((value->GetForegroundColour()) != (wxGetApp().error_color)) &&
             ((unit->name->GetForegroundColour()) != (wxGetApp().error_color)) &&
             (String((value->GetValue().ToStdString())) == String("")) &&
             (String((unit->name->GetValue().ToStdString())) == String(""))
             )
            ) {
            //the GUI field  contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            if (is_ok()) {
                //the content of the GUI field is valid  -> I insert it into recent_value, which points to a suitable location (initialized when *this was constructed)

                //write the value written in *this into *recent_object
                value->GetValue().ToDouble(&(recent_object->value));
                recent_object->unit->set(String(unit->name->GetValue().ToStdString()));
                
                //I update p->name according to the content of recent_itmes
                FillInRecentValue();

            }


            //if check is true (false) -> set ok to true (false)
            ok = is_ok();
            //the background color of value and of unit->name is set to wxGetApp().foreground_color and the font to default_font, because in this case there is no erroneous value in value nor in unit. I call Reset to reset the font colors of the items in the list of unit to their default values
            value->SetForegroundColour(wxGetApp().foreground_color);
            value->SetFont(wxGetApp().default_font);
            unit->name->SetForegroundColour(wxGetApp().foreground_color);
            unit->name->SetFont(wxGetApp().default_font);

            Reset(unit->name);
            
            //if the value written in name is correct, I store it in value_before_editing
            if(ok){
 
                value->GetValue().ToDouble(&(object_before_editing.value));
                object_before_editing.unit->set(String(unit->name->GetValue().ToStdString()));
 
            }

        }else{
            //the GUI field  does not contain a valid text,  it is not empty and with a red background color-> I prompt an error message frame


            parent->print_error_message->SetAndCall(value, String("Value is not valid!"), String("Value must be a floating-point number."), (wxGetApp().path_file_error_icon));

            ok = false;

        }

        if (!ok) {
            //the entered value is not valid: I set the value back to the value before the editing process had started
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, object_before_editing.value));

            unit->name->SetValue(wxString(object_before_editing.unit->value));
            ok = true;

            value->SetForegroundColour(wxGetApp().foreground_color);
            value->SetFont(wxGetApp().default_font);
            unit->name->SetForegroundColour(wxGetApp().foreground_color);
            unit->name->SetFont(wxGetApp().default_font);
            
            Reset(unit->name);
        }
        
        parent->AllOk();

    }
    
    event.Skip(true);

}

template void NumericalField<ChartFrame, Length, LengthUnit, CheckLength<ChartFrame>, CheckUnit<ChartFrame, LengthField<ChartFrame> > >::Check<wxFocusEvent>(wxFocusEvent&);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::Check<wxFocusEvent>(wxFocusEvent&);


//update the value of the GUI (both the value and the unit)  in NumericalField according to recent_value and to the recent units in such a way that the recent value appears in the GUI field
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::FillInRecentValue(void) {
   
    //write *recent_value into the NON_GUI field *object
    object->set((*recent_object));
    
    //write the value in *object into the GUI field *this
    set();
    
//    unit->FillInRecentItems();
    
}

template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::FillInRecentValue();
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::FillInRecentValue();



//update the value of the GUI  in NumericalField according to NuericalField<P, NON_GUI>::recent_value in such a way that the recent value appears in the GUI field
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Enable(bool is_enabled) {
   
    value->Enable(is_enabled);
    unit->Enable(is_enabled);


}
template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::Enable(bool);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::Enable(bool);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::Enable(bool);


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}

template void NumericalField<ChartFrame, Length, LengthUnit, CheckLength<ChartFrame>, CheckUnit<ChartFrame, LengthField<ChartFrame> > >::InsertIn<wxBoxSizer>(wxBoxSizer*, wxSizerFlags&);


//set the value in the GUI object value equal to the value in the non-GUI object speed
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::set(void) {
        
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, object->value));
    value_ok = true;
    
    unit->MultipleItemField<P, NON_GUI_UNIT, CHECK_UNIT>::set();
  
}

template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::set();


template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> bool NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::is_ok(void) {

    return(value_ok && (unit->ok));

}

template bool NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::is_ok();


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class E> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::OnEditValue(E& event) {

    bool success;

    success = check_double((value->GetValue().ToStdString()), NULL, true, 0.0, DBL_MAX);

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

template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::OnEditValue<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<ChartFrame, Length, LengthUnit, CheckLength<ChartFrame>, CheckUnit<ChartFrame, LengthField<ChartFrame> > >::OnEditValue<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::OnEditValue<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::OnEditValue<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::OnEditValue<wxCommandEvent>(wxCommandEvent&);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::OnEditValue<wxCommandEvent>(wxCommandEvent&);


//this method is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template<class E> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::OnEditUnit(E& event) {

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


template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::OnEditUnit<wxCommandEvent>(wxCommandEvent&);
template void NumericalField<ChartFrame, Length, LengthUnit, CheckLength<ChartFrame>, CheckUnit<ChartFrame, LengthField<ChartFrame> > >::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<ChartFrame, Length, LengthUnit, CheckLength<ChartFrame>, CheckUnit<ChartFrame, LengthField<ChartFrame> > >::OnEditUnit<wxCommandEvent>(wxCommandEvent&);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::OnEditUnit<wxCommandEvent>(wxCommandEvent&);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::OnEditUnit<wxCommandEvent>(wxCommandEvent&);


template <class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template <typename EventTag, typename Method, typename Object> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}

template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void NumericalField<SightFrame, Length, LengthUnit, CheckLength<SightFrame>, CheckUnit<SightFrame, LengthField<SightFrame> > >::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);



//write the value and the unit of the GUI field in SpeedField into the non-GUI field speed
template <class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> template <class T> void NumericalField<P, NON_GUI, NON_GUI_UNIT, CHECK, CHECK_UNIT>::get(T& event) {

    if(is_ok()){
        
        double x;
        
        value->GetValue().ToDouble(&x);
        object->set(x, NON_GUI_UNIT((unit->name->GetValue().ToStdString())));
        
    }

    event.Skip(true);

}

template void NumericalField<RouteFrame, Speed, SpeedUnit, CheckSpeed<RouteFrame>, CheckUnit<RouteFrame, SpeedField<RouteFrame> > >::get<wxCommandEvent>(wxCommandEvent&);
template void NumericalField<RouteFrame, Length, LengthUnit, CheckLength<RouteFrame>, CheckUnit<RouteFrame, LengthField<RouteFrame> > >::get<wxCommandEvent>(wxCommandEvent&);
