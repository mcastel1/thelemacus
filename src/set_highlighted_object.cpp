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


//initialize this by setting parent equal to parent_in, higlighted_object_before equal to higlighted_object_before_in and higlighted_object_now equal to higlighted_object_now_in
template<class P> SetHighlightedObject<P>::SetHighlightedObject(P* parent_in, int* highlighted_object_before_in, int* highlighted_object_now_in):  parent(parent_in), higlighted_object_before(highlighted_object_before_in), highlighted_object_now(highlighted_object_now_in) {


}


template<class P> void SetHighlightedObject<P>::operator()(wxCommandEvent& event) {

\
    event.Skip(true);

}

//this is the same as template<class P> void SetHighlightedObject<P>::operator()(void){ but without the event argument
template<class P> void SetHighlightedObject<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}
