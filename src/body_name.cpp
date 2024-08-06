//
//  body_name.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "body_name.h"

#include "my_app.h"


BodyName::BodyName(void) : String() {}


BodyName::BodyName(const String& input) : String(input.value) {}


//return true if *this is one element in wxGetApp().catalog->list (i.e. *this is a valid Body name) and false otherwise
bool BodyName::check(void){
    
    return((wxGetApp().catalog->position_in_list((*this))) != (wxGetApp().catalog->list.size()));
        
}

