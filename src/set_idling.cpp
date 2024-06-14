//
//  set_idling.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "set_idling.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


template<class P> SetIdling<P>::SetIdling(P* parent_in) {

    parent = parent_in;

}

template<class P> void SetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = true;

    event.Skip(true);

}

//this is the same as template<class P> void SetIdling<P>::operator()(wxCommandEvent& event){ but without the event argument
template<class P> void SetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}
