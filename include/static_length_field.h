//
//  static_length_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef static_length_field_h
#define static_length_field_h

#include <iostream>

#include "length.h"
#include "static_text.h"
#include "length_field.h"

using namespace std;


//a non-editable GUI field to display a Length, composed of a static text and a dropdown menu to enter the units of measure of the length. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class StaticLengthField: public LengthField<P>{
    
public:
    
    //the length value
    StaticText* value;
    
    StaticLengthField(wxPanel*, Length*/*, String*/);
    
    void set(const Length&);
    void set(void);
    void set_value_keep_unit(void);
    template<class E> void ConvertUnit(E&);

};



#endif
