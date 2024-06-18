//
//  show_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef show_frame_h
#define show_frame_h

#include <iostream>

using namespace std;




//class which defines a functor which shows a frame with arbitrary type F
template<class F> class ShowFrame{
    
public:
    
    //the frame to be closed
    F* frame;
    ShowFrame(F*);
    
    template<class T> void operator()(T&);
    
};


#endif
