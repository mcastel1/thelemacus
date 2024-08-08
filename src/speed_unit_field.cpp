//
//  speed_unit_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "speed_unit_field.h"

#include "constants.h"
#include "multiple_item_field.h"
#include "route_frame.h"
#include "speed_field.h"


//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> SpeedUnitField<P>::SpeedUnitField(wxPanel* panel_of_parent, SpeedUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, SpeedUnit, CheckUnit<P, SpeedField<P>> >(panel_of_parent, object_in, SpeedUnit_types, recent_items_in) {

}

template class SpeedUnitField<RouteFrame>;
