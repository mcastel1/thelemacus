//
//  animate_to_object.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "animate_to_object.h"

#include "list_frame.h"


template<class P> AnimateToObject<P>::AnimateToObject(P* parent_in) : parent(parent_in) {
    
    
    
    
}

template class AnimateToObject<ListFrame>;
