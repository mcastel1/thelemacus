//
//  check_length_format.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_length_format.h"

#include "constants.h"
#include "length_format_field.h"
#include "route_frame.h"



template<class P> CheckLengthFormat<P>::CheckLengthFormat(LengthFormatField<P>* p_in) {

    p = p_in;

}

template class CheckLengthFormat<RouteFrame>;



template<class P> template<class T> void CheckLengthFormat<P>::operator()(T& event) {
    
    P* f = (p->parent);
    
    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field
    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode


        f->EnableDisableRouteTypeFields(event);
        f->EnableDisableLengthFormatFields(event);
        
    }
    
     
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}

template void CheckLengthFormat<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
