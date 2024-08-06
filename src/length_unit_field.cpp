//
//  length_unit_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "length_unit_field.h"


#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "chart_frame.h"
#include "constants.h"
#include "sight_frame.h"


//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> LengthUnitField<P>::LengthUnitField(wxPanel* panel_of_parent, LengthUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, LengthUnit, CheckUnit<P> >(panel_of_parent, object_in, LengthUnit_types, recent_items_in) {

}
 

template class LengthUnitField<ChartFrame>;
template class LengthUnitField<RouteFrame>;
template class LengthUnitField<SightFrame>;
