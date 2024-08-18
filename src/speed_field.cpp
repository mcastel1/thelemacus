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
template<class P> SpeedField<P>::SpeedField(wxPanel* panel_of_parent, Speed* object_in, Speed* recent_value_in, vector<int>* recent_units_in) : NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >(panel_of_parent, object_in, recent_value_in, recent_units_in){

    wxSizerFlags flags;

    flags.Center();

    //unit is allocated here and not in NumericalField constructor because it needs the argument SpeedUnit_types, which is specifit to SpeedField
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::unit =
    new SpeedUnitField<P>(
                          NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::parent->panel,
                          NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::object->unit,
                          recent_units_in);
    
    //allocate check
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::check = new CheckSpeed<P>(this);

    //I bind unit to NumercialField::Check because when the unit of measure is changed, I want to update also the unit of measure of recent_object. I need to put this Bind() here rather than in the constructor of NumericalField, because otherwise unit->name would be NULL when thi Bind() is called 
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::unit->Bind(wxEVT_KILL_FOCUS, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::template Check<wxFocusEvent>, this);

    
    //this method has to be here and not in the parent class because otherwise it would call non-allocated objects
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::FillInRecentValue();

    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::value->SetInitialSize(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::value->GetSizeFromTextSize(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::value->GetTextExtent(wxS(sample_width_floating_point_field))));
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>>>::value->Bind(wxEVT_KILL_FOCUS, (*(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::check->check_speed_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>>>::value->Bind(wxEVT_KEY_UP, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::template OnEditValue<wxKeyEvent>, this);

    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::unit->Bind(wxEVT_COMBOBOX, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::template OnEditUnit<wxCommandEvent>, this);
    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::unit->Bind(wxEVT_KEY_UP, &NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::template OnEditUnit<wxKeyEvent>, this);

    NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::unit->InsertIn(NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>> >::sizer_h, flags);

}

//explicit instantiations
template class SpeedField<RouteFrame>;
