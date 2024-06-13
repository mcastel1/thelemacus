//
//  route_type.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef route_type_h
#define route_type_h

//a derived class from String, which stores the type of a Route ("loxodrome", "orthodrome", ...)
class RouteType: public String{
    
public:

    RouteType();
    RouteType(const String&);
    RouteType(const string&);
    
};


#endif
