//
//  animate_to_object.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "animate_to_object.h"

#include "list_frame.h"


template<class P, class T, class F> AnimateToObject<P, T, F>::AnimateToObject(P* parent_in, T* object_in, F* f_in) : parent(parent_in), object(object_in), f(f_in) {
    
}

