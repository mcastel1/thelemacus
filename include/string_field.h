//
//  string_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef string_field_h
#define string_field_h

#include <iostream>


#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/textctrl.h>

#include "my_string.h"
#include "set_string_field_to_current_time.h"

using namespace std;


template<class P> class CheckString;
template<class P> class SetStringFieldToCurrentTime;
class String;


//class for graphical object: a field to enter a String, composed of a box. P is the type of the object in which this StringField will be inserted
template<class P> class StringField{
    
public:
    //the parent where this StringField object will be inserted
    P* parent;
    //label box
    wxTextCtrl *value;
    wxBoxSizer *sizer_h, *sizer_v;
    //non-GUI object related to this
    String* string;
    CheckString<P>* check;
    SetStringFieldToCurrentTime<P> set_to_current_time;
    
    StringField(wxPanel*, String*);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};



#endif
