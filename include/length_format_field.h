//
//  length_format_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef length_format_field_h
#define length_format_field_h

#include <iostream>

#include "check_length_format.h"
#include "length_format.h"
#include "multiple_item_field.h"

using namespace std;

template<class P> class CheckLengthFormat;


//this class defines a dropdown menu (wxComboBox) that lets the user choose in what format to express lengths, i.e., simply as a DynamicLengthField or as a ChronoField + a SpeedField (l = t * v). P is the type of parent in which *this is inserted
template<class P> class LengthFormatField: public MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >{
    
public:

    LengthFormatField(wxPanel*, LengthFormat*, vector<int>*);

    template<class E> void OnEdit(E&);
   
};

#endif
