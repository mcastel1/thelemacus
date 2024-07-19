//
//  close_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef close_frame_h
#define close_frame_h

#include <iostream>

using namespace std;




//class which defines a functor which closes a frame with arbitrary type F
template<class F> class CloseFrame{
    
public:
    
    //the frame to be closed
    F* frame;
    CloseFrame(F*);
    
    template<class T> void operator()(T&);
    
};


#endif
