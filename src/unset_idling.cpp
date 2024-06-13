//
//  unset_idling.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "unset_idling.h"

template<class P> UnsetIdling<P>::UnsetIdling(P* parent_in) {

    parent = parent_in;

}


template<class P> void UnsetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = false;

    event.Skip(true);

}

//this is the same as template<class P> void UnsetIdling<P>::operator()(void){ but without the event argument
template<class P> void UnsetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}

