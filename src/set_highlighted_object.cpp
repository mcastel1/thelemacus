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

#include "list_frame.h"


//initialize this by setting parent equal to parent_in, higlighted_object_before equal to higlighted_object_before_in and higlighted_object_now equal to higlighted_object_now_in
template<class P> SetHighlightedObject<P>::SetHighlightedObject(P* parent_in, int* highlighted_object_before_in, int* highlighted_object_now_in):  parent(parent_in), higlighted_object_before(highlighted_object_before_in), highlighted_object_now(highlighted_object_now_in) {


}

template class SetHighlightedObject<ListFrame>;


template<class P> void SetHighlightedObject<P>::set_value(const int& i){
    
    value = i;
    
}


//store the value of the previoudly highlighted object in *highlighted_object_before and set *highlighted_object_now to value
template<class P>  template<class E> void SetHighlightedObject<P>::operator()(E& event){
    
    (*higlighted_object_before) = (*highlighted_object_now);
    (*highlighted_object_now) = value;

    event.Skip(true);

}

