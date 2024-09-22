//
//  set_highlighted_object.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef set_highlighted_object_h
#define set_highlighted_object_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

using namespace std;

//this functor sets a highlighted object (i.e. a Route or a Position) to a given value and stores the id of the old highlighted object.  f is the functor that will be called at the end of the highlight operation,and it will be entered into the constructor. P is the type of the parent of *this and F the type of f
template<class P, class F> class HighlightObject{
    
public:
        
    //the frame which called this struct
    P* parent;
    //the functor to be called at the end of the animation
    F* f;
    
    int /*higlighted_object_before and highlighted_object_now are the addresses where are stored the ids of the object highlighted in the past and the object hihglighted now: these adresses are needed for this functor to write in them when the highlighted object changes */*higlighted_object_before, *highlighted_object_now, /*the value that will be set when the functor is called*/ value;
    
    HighlightObject(P*, int*, int*, F*);
    
    void set_value(const int&);
    template<class E> void operator()(E&);
    void operator()(void);
    
};


#endif
