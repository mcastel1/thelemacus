//
//  projection_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef projection_field_h
#define projection_field_h

#include <iostream>
#include <vector>

#include "check_projection.h"
#include "multiple_item_field.h"


using namespace std;


template<class P> class CheckProjection;
class Projection;


template<class P> class ProjectionField: public MultipleItemField<P, Projection, CheckProjection<P> >{
    
public:

    ProjectionField(wxPanel*, Projection*, vector<int>*);
    
    template<class E> void OnEdit(E&);
   
};


#endif
