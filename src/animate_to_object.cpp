//
//  animate_to_object.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "animate_to_object.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "chart_frame.h"
#include "draw_panel.h"
#include "list_frame.h"
#include "position_frame.h"
#include "route_frame.h"
#include "sight_frame.h"


template<class P> AnimateToObject<P>::AnimateToObject(P* parent_in) {

    parent = parent_in;

}



template<class P> void AnimateToObject<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = false;

    event.Skip(true);

}

//this is the same as template<class P> void AnimateToObject<P>::operator()(void){ but without the event argument
template<class P> void AnimateToObject<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}

template void AnimateToObject<ListFrame>::operator()();
template void AnimateToObject<PositionFrame>::operator()();
template void AnimateToObject<RouteFrame>::operator()();
template void AnimateToObject<SightFrame>::operator()();
template void AnimateToObject<DrawPanel>::operator()();
template void AnimateToObject<ChartFrame>::operator()();
