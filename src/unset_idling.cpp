//
//  unset_idling.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "unset_idling.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "chart_frame.h"
#include "list_frame.h"
#include "position_frame.h"
#include "route_frame.h"
#include "sight_frame.h"


template<class P> UnsetIdling<P>::UnsetIdling(P* parent_in) {

    parent = parent_in;

}

template class UnsetIdling<ChartFrame>;
template class UnsetIdling<RouteFrame>;
template class UnsetIdling<SightFrame>;
template class UnsetIdling<PositionFrame>;


template<class P> void UnsetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = false;

    event.Skip(true);

}

//this is the same as template<class P> void UnsetIdling<P>::operator()(void){ but without the event argument
template<class P> void UnsetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}

template void UnsetIdling<ListFrame>::operator()();
