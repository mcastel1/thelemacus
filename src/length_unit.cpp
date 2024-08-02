//
//  length_unit.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "length_unit.h"
#include "constants.h"


LengthUnit::LengthUnit(void) : String() {}


LengthUnit::LengthUnit(const String& input) : String(input.value) {}


//return true if *this is one element in LengthUnit_types (i.e. *this is a valid Length unit) and false otherwise)
//inline 
bool LengthUnit::check(void){
    
    return(position_in_list(LengthUnit_types) != (LengthUnit_types.size())) ;
    
}

