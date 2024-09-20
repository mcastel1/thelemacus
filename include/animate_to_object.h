//
//  animate_to_object.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef animate_to_object_h
#define animate_to_object_h

#include <iostream>


using namespace std;

template<class P> class AnimateToObject{
    
public:
        
    //the frame which called *this
    P* parent;
    
    AnimateToObject(P*);
    
};


#endif
