//
//  check_route_type.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_route_type_h
#define check_route_type_h

#include <iostream>

using namespace std;

//a functor that checks if a Route type is valid
template<class P> class CheckRouteType{
    
public:
    
    //the RouteType  field containing the Route type
    RouteTypeField<P>* p;
    
    CheckRouteType(RouteTypeField<P>*);
    template<class T> void operator()(T&);
    
};



#endif
