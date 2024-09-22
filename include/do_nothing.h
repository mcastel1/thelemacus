//
//  do_nothing.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef do_nothing_h
#define do_nothing_h

#include <iostream>

using namespace std;

//this is a dummy functor thad does nothing. It exists because the structure of the code of Thelemacus requires, for example, an animation to be provided with a functor that specifies what to do when the animation is over, and DoNothing is used when nothing needs to be done at the end of the animation.
class DoNothing{
    
public:

    DoNothing();
    
    template <class E> void operator()(E&);
    
};

#endif
