//
//  close_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "close_frame.h"

#include "destroy_frame.h"
#include "list_frame.h"
#include "position_frame.h"
#include "sight_frame.h"


template<class F> CloseFrame<F>::CloseFrame(F* frame_in) {

    frame = frame_in;

}

//explicit instantiations
template class CloseFrame<ListFrame>;
template class CloseFrame<MessageFrame<UnsetIdling<ListFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<RouteFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<SightFrame>>>;
template class CloseFrame<MessageFrame<UnsetIdling<PositionFrame>>>;

//closes a frame of type F
template<class F> template <class T> void CloseFrame<F>::operator()([[maybe_unused]] T& event) {

    DestroyFrame<F>* destroy_frame;

    destroy_frame = new DestroyFrame<F>(frame);

    //destroys frame
    frame->CallAfter(*destroy_frame);

    event.Skip(true);

}

//explicit instantiations
template void CloseFrame<ListFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<ListFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<ListFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<RouteFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<RouteFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<SightFrame>>>::operator()<wxKeyEvent>(wxKeyEvent&);
template void CloseFrame<MessageFrame<UnsetIdling<SightFrame>>>::operator()<wxCommandEvent>(wxCommandEvent&);
