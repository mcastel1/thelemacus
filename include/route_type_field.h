//
//  route_type_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef route_type_field_h
#define route_type_field_h

#include <vector>

#include "route_type.h"
#include "my_string.h"

using namespace std;

//GUI field to specify the RouteType, which is inserted into a parent panel of type P
template<class P> class RouteTypeField: public MultipleItemField<P, RouteType, CheckRouteType<P> >{
    
public:
    
    
    RouteTypeField(wxPanel*, RouteType*, const vector<String>&, vector<int>*);
    void set(void);
    template<class E> void OnEdit(E&);
    void OnEdit();
    
};


#endif
