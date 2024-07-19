//
//  route_type.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "route_type.h"


RouteType::RouteType(void) : String() {}

RouteType::RouteType(const String& input) : String(input.value) {}

RouteType::RouteType(const string& input) : String(input) {}

