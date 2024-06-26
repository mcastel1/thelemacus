//
//  check_string.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_string.h"

#include "route_frame.h"
#include "sight_frame.h"
#include "string_field.h"


template<class P> CheckString<P>::CheckString(StringField<P>* p_in) {

    p = p_in;

}

template class CheckString<RouteFrame>;


//this functor does nothing, delete it in the future
template<class P> template<class T> void CheckString<P>::operator()(T& event) {

    P* f = (p->parent);

    f->AllOk();

    event.Skip(true);

}

template void CheckString<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckString<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckString<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
