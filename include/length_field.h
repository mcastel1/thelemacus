//
//  length_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef length_field_h
#define length_field_h

#include <iostream>

#include "check_length.h"
#include "length.h"
#include "length_unit_field.h"
#include "numerical_field.h"

using namespace std;

class Length;


//this class is for a GUI field containing a Length
template<class P> class LengthField : public NumericalField<P, Length, LengthUnit, CheckLength<P>, CheckUnit<P, LengthField<P>>>{
    
public:
    
    //the parent frame to which this object is attached
//    P* parent;
    //unit of measure of the Length written in *this
//    LengthUnitField<P>* unit;
//    wxBoxSizer *sizer_h, *sizer_v;
//    Length* length;

    LengthField(wxPanel*, Length*, Length*, vector<int>*);

//    template<class E> void OnEditUnit(E&);
//    template<class T> void InsertIn(T*);
//    template<class T> void InsertIn(T*, wxSizerFlags&);
    
};


#endif
