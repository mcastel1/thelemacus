//
//  check_length.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_length.h"

#include "check_length_value.h"
#include "check_unit.h"
#include "route_frame.h"
#include "sight_frame.h"


template<class P> CheckLength<P>::CheckLength(DynamicLengthField<P>* p_in) {

    p = p_in;

    check_length_value = new CheckLengthValue<P>(p);
    check_length_unit = new CheckUnit<P>(p);

}

template class CheckLength<RouteFrame>;
template class  CheckLength<SightFrame>;


//this functor checks the whole Length field by calling the check on its value and unit
template<class P> template <class T> void CheckLength<P>::operator()(T& event) {

    (*check_length_value)(event);
    (*check_length_unit)(event);

    event.Skip(true);

}

//explicit instantiations
template void CheckLength<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckLength<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
