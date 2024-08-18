//
//  speed_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef speed_field_h
#define speed_field_h

#include <iostream>


#include "check_unit.h"
#include "numerical_field.h"
#include "speed.h"
#include "speed_unit_field.h"

using namespace std;


template<class P> class CheckSpeed;
class Speed;
template<class P> class SpeedUnitField;


//class for graphical object: a field to enter a speed, composed of a box and a dropdown menu to enter the units of measure of the speed. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class SpeedField: public NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedUnit, SpeedField<P>>>{
    
public:
    
    SpeedField(wxPanel*, Speed*, Speed*, vector<int>*);
    
};



#endif
