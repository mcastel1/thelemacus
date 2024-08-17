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



//constructor of a EditableLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> DynamicLengthField<P>::DynamicLengthField(wxPanel* panel_of_parent, Length* p, Length* recent_value_in, vector<int>* recent_units_in) : NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>(panel_of_parent, p, recent_value_in, recent_units_in) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::unit = new LengthUnitField<P>(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::parent->panel, NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::object->unit, &(wxGetApp().list_frame->data->recent_length_units));

    
    //initialize check
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::check = new CheckLength<P>(this);
    
    
    //I bind unit to NumercialField::Check because when the unit of measure is changed, I want to update also the unit of measure of recent_object. I need to put this Bind() here rather than in the constructor of NumericalField, because otherwise unit->name would be NULL when thi Bind() is called
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::unit->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::template Check<wxFocusEvent>, this);
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::unit->Bind(wxEVT_COMBOBOX, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::template OnEditUnit<wxCommandEvent>, this);
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::unit->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::template OnEditUnit<wxKeyEvent>, this);

    
    
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::value->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::template OnEditValue<wxKeyEvent>, this);

    
    //this method has to be here and not in the parent class because otherwise it would call non-allocated objects
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::FillInRecentValue();
    
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::unit->InsertIn(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::sizer_h, flags);


}

template class DynamicLengthField<SightFrame>;
template class DynamicLengthField<RouteFrame>;
