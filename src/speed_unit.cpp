//
//  speed_unit.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "speed_unit.h"


SpeedUnit::SpeedUnit(void) : String() {}


SpeedUnit::SpeedUnit(const String& input) : String(input.value) {}


SpeedUnit::SpeedUnit(const string& input) : String(input) {}
