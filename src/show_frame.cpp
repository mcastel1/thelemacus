//
//  show_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "show_frame.h"



template<class F> ShowFrame<F>::ShowFrame(F* frame_in) {

    frame = frame_in;

}


//show a frame of type F
template<class F> template <class T> void ShowFrame<F>::operator()(T& event) {

    frame->Show(true);

    event.Skip(true);


}
