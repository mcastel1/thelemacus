//
//  length_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "dynamic_length_field.h"
#include "generic.h"
#include "length.h"
#include "length_field.h"
#include "sight_frame.h"



//constructor of a LengthField object, based on the parent frame frame
template<class P> LengthField<P>::LengthField(wxPanel* panel_of_parent, Length* object_in, Length* recent_value_in, vector<int>* recent_units_in) : NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >(panel_of_parent, object_in, recent_value_in, recent_units_in){

    wxSizerFlags flags;

    flags.Center();

    
//    //allocate check
//    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::check = new CheckLength<P>(this);
    
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>>>::unit = new LengthUnitField<P>(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>>>::parent->panel, NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>>>::object->unit, &(wxGetApp().list_frame->data->recent_length_units));

    //I bind unit to NumercialField::Check because when the unit of measure is changed, I want to update also the unit of measure of recent_object. I need to put this Bind() here rather than in the constructor of NumericalField, because otherwise unit->name would be NULL when thi Bind() is called
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::unit->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::template Check<wxFocusEvent>, this);

    
    //this method has to be here and not in the parent class because otherwise it would call non-allocated objects
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::FillInRecentValue();

    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::value->SetInitialSize(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::value->GetSizeFromTextSize(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::value->GetTextExtent(wxS(sample_width_floating_point_field))));
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>>>::value->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::template OnEditValue<wxKeyEvent>, this);

    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::unit->Bind(wxEVT_COMBOBOX, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::template OnEditUnit<wxCommandEvent>, this);
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::unit->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::template OnEditUnit<wxKeyEvent>, this);

    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::unit->InsertIn(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>> >::sizer_h, flags);

}

template class LengthField<ChartFrame>;
template class LengthField<RouteFrame>;
template class LengthField<SightFrame>;


////this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
//template<class P> template<class E> void LengthField<P>::OnEditUnit(E& event) {
//
//    bool success;
//
//    //I check whether the name in the GUI field unit matches one of the unit names in units
//    find_and_replace_case_insensitive(unit->name, unit->catalog, &success, NULL);
//
//
//    if (success) {
//
//        //because the text in value is valid, I set the background color of unit to white
//        unit->name->SetForegroundColour(wxGetApp().foreground_color);
//        unit->name->SetFont(wxGetApp().default_font);
//
//    }
//
//    (unit->ok) = success;
//
//    event.Skip(true);
//
//}
//
//template void LengthField<RouteFrame>::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
//template void LengthField<RouteFrame>::OnEditUnit<wxCommandEvent>(wxCommandEvent&);
//template void LengthField<SightFrame>::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
//template void LengthField<SightFrame>::OnEditUnit<wxCommandEvent>(wxCommandEvent&);
//template void LengthField<ChartFrame>::OnEditUnit<wxKeyEvent>(wxKeyEvent&);
//template void LengthField<ChartFrame>::OnEditUnit<wxCommandEvent>(wxCommandEvent&);


//insert *this in *host
//template<class P> template<class T> void LengthField<P>::InsertIn(T* host) {
//
//    host->Add(LengthField<P>::sizer_v);
//
//}
//
//template void LengthField<RouteFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
//template  void LengthField<SightFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
//
//
////same asLengthField<P>::InsertIn(T* host) but with flags to be provided
//template<class P> template<class T> void LengthField<P>::InsertIn(T* host, wxSizerFlags& flag) {
//
//    host->Add(sizer_v, flag);
//
//}
//
//template void LengthField<ChartFrame>::InsertIn<wxBoxSizer>(wxBoxSizer*, wxSizerFlags&);
