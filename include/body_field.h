//
//  bodyfield.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef body_field_h
#define body_field_h

#include <iostream>

#include <vector>
#include "body.h"
#include "check_body.h"
#include "multiple_item_field.h"

using namespace std;

template<class P> class CheckBody;


//P is the type of the frame which hosts *this
template<class P> class BodyField : public MultipleItemField<P, Body, CheckBody<P>>{
    
public:
    
    BodyField(wxPanel*, Body*, vector<int>*);
    void set(void);
    template<class T> void get(T&);
    template<class E> void OnEdit(E&);
    
};


#endif
