//
//  speed_unit_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef speed_unit_field_h
#define speed_unit_field_h

#include <iostream>

using namespace std;

//GUI field to specify the unit of measure of a Speed, which is inserted into a parent panel of type P
template<class P> class SpeedUnitField: public MultipleItemField<P, SpeedUnit, CheckLengthUnit<P> >{
    
public:
    
    SpeedUnitField(wxPanel*, SpeedUnit*, vector<int>*);
    
};



#endif
