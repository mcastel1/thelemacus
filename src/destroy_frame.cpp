//
//  destroy_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "destroy_frame.h"


template<class F> DestroyFrame<F>::DestroyFrame(F* frame_in) {

    frame = frame_in;

}

//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}

