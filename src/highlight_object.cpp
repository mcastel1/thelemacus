//
//  highlight_object.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "highlight_object.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "list_frame.h"


//initialize this by setting parent equal to parent_in, higlighted_object_before equal to higlighted_object_before_in and higlighted_object_now equal to higlighted_object_now_in
template<class P> HighlightObject<P>::HighlightObject(P* parent_in, int* highlighted_object_before_in, int* highlighted_object_now_in):  parent(parent_in), higlighted_object_before(highlighted_object_before_in), highlighted_object_now(highlighted_object_now_in) {


}

template class HighlightObject<ListFrame>;


template<class P> void HighlightObject<P>::set_value(const int& i){
    
    value = i;
    
}

template void HighlightObject<ListFrame>::set_value(int const&);


//store the value of the previoudly highlighted object in *highlighted_object_before and set *highlighted_object_now to value
template<class P> template<class E> void HighlightObject<P>::operator()(E& event){
    
    (*higlighted_object_before) = (*highlighted_object_now);
    (*highlighted_object_now) = value;
    
    if ((*higlighted_object_before) != (*highlighted_object_now)) {
        //the highlighted Route or Position has changed -> update the charts
        
        parent->changing_highlighted_object = true;
        parent->MyRefreshAll();
        parent->changing_highlighted_object = false;
        
    }

    event.Skip(true);

}

template void HighlightObject<ListFrame>::operator()<wxCommandEvent>(wxCommandEvent&);


//same as HighlightObject<P>::operator()(E& event but with no argument
template<class P> void HighlightObject<P>::operator()(void){
    
    wxCommandEvent dummy;

    (*this)(dummy);

}

template void HighlightObject<ListFrame>::operator()();
