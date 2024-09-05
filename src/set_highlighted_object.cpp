//
//  set_highlighted_object.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "set_highlighted_object.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


template<class P> SetHighlightedObject<P>::SetHighlightedObject(P* parent_in) {

    parent = parent_in;

}


template<class P> void SetHighlightedObject<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = false;

    event.Skip(true);

}

//this is the same as template<class P> void SetHighlightedObject<P>::operator()(void){ but without the event argument
template<class P> void SetHighlightedObject<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}
