//
//  body_name_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef body_name_field_h
#define body_name_field_h

#include <iostream>

#include <vector>
#include "body.h"
#include "check_body_name.h"
#include "multiple_item_field.h"

using namespace std;

template<class P> class CheckBodyName;


//P is the type of the frame which hosts *this
template<class P> class BodyNameField : public MultipleItemField<P, Body, CheckBodyName<P>>{
    
public:
    
    BodyNameField(wxPanel*, Body*, vector<int>*);
    void set(void);
    template<class T> void get(T&);
    template<class E> void OnEdit(E&);
    
};


#endif
