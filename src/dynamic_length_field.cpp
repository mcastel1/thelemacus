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
#include "numerical_field.h"
#include "sight_frame.h"



//constructor of a editable Length field object
template<class P> DynamicLengthField<P>::DynamicLengthField(wxPanel* panel_of_parent, Length* p, Length* recent_value_in) : NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>(panel_of_parent, p, recent_value_in) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>> >::unit =
    new MultipleItemField<P, LengthUnit, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>(
                                                                              NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>> >::parent->panel,
                                                                              NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>> >::object->unit,
                                                                              LengthUnit_types,
                                                                              &(wxGetApp().list_frame->data->recent_length_units)
                                                                              );
    
    //initialize check
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::check = new CheckLength<P>(this);
    
    
    //I bind unit to NumercialField::Check because when the unit of measure is changed, I want to update also the unit of measure of recent_object. I need to put this Bind() here rather than in the constructor of NumericalField, because otherwise unit->name would be NULL when thi Bind() is called
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::unit->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::template Check<wxFocusEvent>, this);
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::unit->Bind(wxEVT_COMBOBOX, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::template OnEditUnit<wxCommandEvent>, this);
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::unit->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::template OnEditUnit<wxKeyEvent>, this);

    
    
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::value->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::template OnEditValue<wxKeyEvent>, this);
    
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::unit->InsertIn(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthUnit, DynamicLengthField<P>>>::sizer_h, flags);


}

template class DynamicLengthField<SightFrame>;
template class DynamicLengthField<RouteFrame>;
