//
//  dynamic_length_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef dynamic_length_field_h
#define dynamic_length_field_h

#include <iostream>

#include "length.h"
#include "length_field.h"

using namespace std;

template<class P> class CheckLength;


//an editable GUI field to enter a Length, composed of a box and a dropdown menu to enter the units of measure of the Length. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class DynamicLengthField: public LengthField<P>{
    
public:

    //the length value
//    wxTextCtrl *value;
//    CheckLength<P>* check;
//    bool value_ok;
    
    DynamicLengthField(wxPanel*, Length*, Length*, vector<int>*);
    
//    void set(Length);
//    void set(void);
//    template<class T> void get(T&);
//    void Enable(bool);
//    bool is_ok(void);
//    template<class E> void OnEditValue(E&);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};




#endif
