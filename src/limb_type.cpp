//
//  limb_type.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "limb_type.h"

#include "constants.h"
#include "generic.h"


LimbType::LimbType(void) : String() {}


LimbType::LimbType(const String& input) : String(input.value) {}


LimbType::LimbType(const string& input) : String(input) {}
