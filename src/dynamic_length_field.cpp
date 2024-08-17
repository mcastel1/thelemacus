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

    
    //initialize check
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::check = new CheckLength<P>(this);
    
//    value = new wxTextCtrl((LengthField<P>::parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::value->Bind(wxEVT_KEY_UP, &NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::template OnEditValue<wxKeyEvent>, this);

    
    //this method has to be here and not in the parent class because otherwise it would call non-allocated objects
    NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>::FillInRecentValue();

}

template class DynamicLengthField<SightFrame>;
template class DynamicLengthField<RouteFrame>;
