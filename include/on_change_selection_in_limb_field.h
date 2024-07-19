//
//  on_change_selection_in_limb_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef on_change_selection_in_limb_field_h
#define on_change_selection_in_limb_field_h

#include <iostream>

using namespace std;

template<class P> class LimbField;


//if an item is selected/deselected in caller, call operator() to make sure that only one item is selected at a time
template<class P> class OnChangeSelectionInLimbField{
    
public:
    
    LimbField<P>* caller;
    
    OnChangeSelectionInLimbField(LimbField<P>*);
    template<class T> void operator()(T&);
    
};


#endif
