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
    
//    P* parent;
//    wxBoxSizer *sizer_h, *sizer_v;
//    wxArrayString  /*this is equal to a standard list of the available formats for a length*/items, /*same as length_formats, but it is fixed and never wrote to: it is a fixed catalog*/catalog;
    //this is the wxComboBox with the name of the length formats
//    wxComboBox* name;
    //this points to a LengthFormat object, which contains the date written in the GUI field of this
//    vector<int> recent_items;
//    bool ok;
//    LengthFormat* length_format;
//    CheckLengthFormat<P>* check;

    LengthFormatField(wxPanel*, LengthFormat*, vector<int>*);
//    void Fill(void);
//    void set(void);
//    template<class T> void InsertIn(T*);
    template<class E> void OnEdit(E&);
//    void Enable(bool);
//    bool is_ok(void);
//    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};




#endif
