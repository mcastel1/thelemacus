//
//  length_unit_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef length_unit_field_h
#define length_unit_field_h

#include <iostream>

using namespace std;

//GUI field to specify the unit of measure of a Length, which is inserted into a parent panel of type P
template<class P> class LengthUnitField: public MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >{
    
public:
    
    LengthUnitField(wxPanel*, LengthUnit*, vector<int>*);
    
};


#endif
