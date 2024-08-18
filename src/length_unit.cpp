//
//  length_unit.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "length_unit.h"

#include "constants.h"
#include "generic.h"


LengthUnit::LengthUnit(void) : String() {}


LengthUnit::LengthUnit(const String& input) : String(input.value) {}


LengthUnit::LengthUnit(const string& input) : String(input) {}


//return true if *this is one element in LengthUnit_types (i.e. *this is a valid Length unit) and false otherwise)
//inline 
bool LengthUnit::check(void){
    
    return(position_in_vector((*this), LengthUnit_types) != (LengthUnit_types.size())) ;
    
}

