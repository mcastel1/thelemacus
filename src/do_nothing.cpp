//
//  do_nothing.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "do_nothing.h"




template<class P> UnsetIdling<P>::UnsetIdling(P* parent_in) {

    parent = parent_in;

}

template class UnsetIdling<ChartFrame>;
template class UnsetIdling<RouteFrame>;
template class UnsetIdling<SightFrame>;
template class UnsetIdling<PositionFrame>;
template class UnsetIdling<DrawPanel>;
template class UnsetIdling<ListFrame>;

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
template void UnsetIdling<PositionFrame>::operator()();
template void UnsetIdling<RouteFrame>::operator()();
template void UnsetIdling<SightFrame>::operator()();
template void UnsetIdling<DrawPanel>::operator()();
template void UnsetIdling<ChartFrame>::operator()();
