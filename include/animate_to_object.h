//
//  animate_to_object.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef animate_to_object_h
#define animate_to_object_h

#include <iostream>

#include "list_frame.h"

using namespace std;

class ListFrame;

//this functor makes an animation which centers the chart on the object *object (which may be a Route, Position, ...) and adjust the chart zoom factor in such a way that *object is nicely visible at the end of the animation. Here f is the functor that will be called at the end of the animation, and it will be entered into the constructor of ChartTransrportHandler. If no functor is to be called at the end of the animation, one may let f point to a UnsetIdling<> functor. The template parameters are : T (type of the object to which animate), and F (the type of the functor to be called at the end of the animation). The animation will be triggered by calling template<class E> void operator()(E&);
template<class T, class F> class AnimateToObject{
    
public:
        
    //the parent of *this
    ListFrame* parent;
    //the object to which animate
    T* object;
    //the functor to be called at the end of the animation
    F* f;
    
    AnimateToObject(ListFrame*, T*, F*);
    
    template<class E> void operator()(E&);
    void operator()(void);

};


#endif
