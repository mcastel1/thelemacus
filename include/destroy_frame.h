//
//  destroy_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef destroy_frame_h
#define destroy_frame_h

#include <iostream>

using namespace std;



//class which defines a functor which destroys a frame with arbitrary type F
template<class F> class DestroyFrame{
    
public:
    
    //the frame to be destroyd
    F* frame;
    DestroyFrame(F*);
    
    void operator()(void);
    
};



#endif
