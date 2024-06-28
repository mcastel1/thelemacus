//
//  destroy_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "destroy_frame.h"

#include "list_frame.h"


template<class F> DestroyFrame<F>::DestroyFrame(F* frame_in) {

    frame = frame_in;

}

template class DestroyFrame<ListFrame>;
template class DestroyFrame<MessageFrame<UnsetIdling<ListFrame>>>;


//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}

template void DestroyFrame<ListFrame>::operator()();
