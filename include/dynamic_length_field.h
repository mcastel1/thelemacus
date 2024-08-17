//
//  dynamic_length_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef dynamic_length_field_h
#define dynamic_length_field_h

#include <iostream>

#include "length.h"

using namespace std;

template<class P> class CheckLength;


//an editable GUI field to enter a Length, composed of a box and a dropdown menu to enter the units of measure of the Length. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class DynamicLengthField: public NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, DynamicLengthField<P>>>{
    
public:
    
    DynamicLengthField(wxPanel*, Length*, Length*, vector<int>*);
    
};




#endif
