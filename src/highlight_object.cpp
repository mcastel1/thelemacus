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

#include "do_nothing.h"
#include "list_frame.h"


//initialize this by setting parent equal to parent_in, higlighted_object_before equal to higlighted_object_before_in and higlighted_object_now equal to higlighted_object_now_in
template<class P, class F> HighlightObject<P, F>::HighlightObject(P* parent_in, int* highlighted_object_before_in, int* highlighted_object_now_in, F* f_in):  parent(parent_in), f(f_in), higlighted_object_before(highlighted_object_before_in), highlighted_object_now(highlighted_object_now_in) {


}

template class HighlightObject<ListFrame, DoNothing>;
template class HighlightObject<ListFrame, DisconnectSight>;
template class HighlightObject<ListFrame, UnsetIdling<ListFrame>>;


template<class P, class F> void HighlightObject<P, F>::set_value(const int& i){
    
    value = i;
    
}

template void HighlightObject<ListFrame, DoNothing>::set_value(int const&);
template void HighlightObject<ListFrame, DisconnectSight>::set_value(int const&);
template void HighlightObject<ListFrame, UnsetIdling<ListFrame>>::set_value(int const&);


//store the value of the previoudly highlighted object in *highlighted_object_before, set *highlighted_object_now to value and refresh all to show the newly highlighted object
template<class P, class F> template<class E> void HighlightObject<P, F>::operator()(E& event){
    
    (*higlighted_object_before) = (*highlighted_object_now);
    (*highlighted_object_now) = value;
    
    if ((*higlighted_object_before) != (*highlighted_object_now)) {
        //the highlighted Route or Position has changed -> update the charts
        
        parent->changing_highlighted_object = true;
        parent->MyRefreshAll();
        parent->changing_highlighted_object = false;
        
    }
    
    if(f != NULL){
        //*this has been constructed with f_in != NULL -> call the action to be executed after *this is called
        
        (*f)(event);
    }

    event.Skip(true);

}

template void HighlightObject<ListFrame, DisconnectSight>::operator()();


//same as HighlightObject<P, F>::operator()(E& event but with no argument
template<class P, class F> void HighlightObject<P, F>::operator()(void){
    
    wxCommandEvent dummy;

    (*this)(dummy);

}

template void HighlightObject<ListFrame, DoNothing>::operator()();
template void HighlightObject<ListFrame, UnsetIdling<ListFrame>>::operator()();
