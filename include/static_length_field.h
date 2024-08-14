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

class Length;

//a non-editable GUI field to display a Length, composed of a static text and a dropdown menu to enter the units of measure of the length. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class StaticLengthField{
    
public:
    
    //the wxFrame parent of *this
    P* parent;
    //the NON_GUI Length related to *this
    Length* length;
    
    //the length value
    StaticText* value;
    //the GUI field for the unit of measure of *this
    LengthUnitField<P>* unit;
    wxBoxSizer *sizer_h, *sizer_v;

    
    StaticLengthField(wxPanel*, Length*, vector<int>*);
    
    void set(const Length&);
    void set(void);
    void set_value_keep_unit(void);
    template<class E> void ConvertUnit(E&);
    template<class E> void OnEditUnit(E&);
    template<class T> void InsertIn(T*);
    template<class T> void InsertIn(T*, wxSizerFlags&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);


};



#endif
