//
//  route_type_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef route_type_field_h
#define route_type_field_h


//GUI field to specify the RouteType, which is inserted into a parent panel of type P
template<class P> class RouteTypeField: public MultipleItemField<P, RouteType, CheckRouteType<P> >{
    
public:
    
    
    RouteTypeField(wxPanel*, RouteType*, const vector<String>&, vector<int>*);
    void set(void);
    template<class E> void OnEdit(E&);
    void OnEdit();
    
};


#endif
