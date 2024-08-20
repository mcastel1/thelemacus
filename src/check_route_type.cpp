//
//  check_route_type.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_route_type.h"

#include "constants.h"
#include "route_frame.h"


template<class P> CheckRouteType<P>::CheckRouteType(RouteTypeField<P>* p_in) {

    p = p_in;


}

template class CheckRouteType<RouteFrame>;


//this functor checks the wxComboBox containing the Route type, and if it is equal to loxodrome or orthodrome, it enables length, Z and start_* fields in RouteFrame (the latter if for_transport = false, becasue I don't need a start position if I am using the Route for transport). If it is equal to circle of equal altitude, it enables only the GP and omege fields.
template<class P> template<class T> void CheckRouteType<P>::operator()(T& event) {

    P* f = (p->parent);
//    bool enable;
    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field

    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode
        
        f->EnableDisableRouteTypeFields(event);
        f->EnableDisableLengthFormatFields();

    }
    
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}

template void CheckRouteType<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckRouteType<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
