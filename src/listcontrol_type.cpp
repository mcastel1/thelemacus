//
//  listcontrol_type.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "listcontrol_type.h"

#include "constants.h"
#include "generic.h"


ListControlType::ListControlType(void) : String() {}


ListControlType::ListControlType(const String& input) : String(input.value) {}


ListControlType::ListControlType(const string& input) : String(input) {}
