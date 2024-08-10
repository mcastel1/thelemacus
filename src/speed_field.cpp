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
#include "double.h"
#include "generic.h"
#include "speed_unit_field.h"
#include "my_app.h"


//constructor of a SpeedField object, based on the parent frame frame
template<class P> SpeedField<P>::SpeedField(wxPanel* panel_of_parent, Speed* object_in, Speed* recent_value_in, vector<int>* recent_units_in) : NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >(panel_of_parent, object_in, recent_value_in, recent_units_in){

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

    //this method has to be here and not in the parent class because otherwise it would call non-allocated objects
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::FillInRecentValue();

//    flags.Center();

//    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->SetInitialSize(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetSizeFromTextSize(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Speed object
//    value->SetValue(wxString(""));
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::check->check_speed_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>>>::value->Bind(wxEVT_KEY_UP, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::template OnEditValue<wxKeyEvent>, this);


//    (unit->name) = new wxComboBox((parent->panel), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, units, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
//    unit = new SpeedUnitField<P>(parent->panel, &(speed->unit), &(wxGetApp().list_frame->data->recent_speed_units));

    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->Bind(wxEVT_COMBOBOX, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::template OnEditUnit<wxCommandEvent>, this);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->Bind(wxEVT_KEY_UP, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::template OnEditUnit<wxKeyEvent>, this);

    
    //    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    //    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    //    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
    //    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    //    unit->InsertIn(sizer_h, flags);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->InsertIn(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::sizer_h, flags);

    
}

//explicit instantiations
template class SpeedField<RouteFrame>;




//write the value and the unit of the GUI field in SpeedField into the non-GUI field speed
template<class P> template <class T> void SpeedField<P>::get(T& event) {

    if(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::is_ok()){
        
        double x;
        
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->GetValue().ToDouble(&x);
        NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::object->set(x, SpeedUnit((NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);

}

//explicit instantiations
template void SpeedField<RouteFrame>::get<wxCommandEvent>(wxCommandEvent&);


template<class P> template <typename EventTag, typename Method, typename Object> void SpeedField<P>::Bind(EventTag tag, Method method, Object object) {

    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::value->Bind(tag, method, object);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>> >::unit->Bind(tag, method, object);

}

//explicit instantiations
template void SpeedField<RouteFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);




