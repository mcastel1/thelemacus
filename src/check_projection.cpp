//
//  check_projection.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_projection.h"

#include "projection_field.h"



template<class P> CheckProjection<P>::CheckProjection(ProjectionField<P>* p_in) {

    p = p_in;

}


template<class P> template<class T> void CheckProjection<P>::operator()(T& event) {
        
    P* f = (p->parent);

    
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}

