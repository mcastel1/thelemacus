//
//  destroy_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "destroy_frame.h"

#include "list_frame.h"
#include "position_frame.h"
#include "route_frame.h"
#include "sight_frame.h"


template<class F> DestroyFrame<F>::DestroyFrame(F* frame_in) {

    frame = frame_in;

}

template class DestroyFrame<ListFrame>;
template class DestroyFrame<MessageFrame<UnsetIdling<ListFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<RouteFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<SightFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<PositionFrame>>>;
template class DestroyFrame<MessageFrame<UnsetIdling<DrawPanel>>>;


//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}

template void DestroyFrame<ListFrame>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<ListFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<RouteFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<SightFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<PositionFrame>>>::operator()();
template void DestroyFrame<MessageFrame<UnsetIdling<DrawPanel>>>::operator()();
