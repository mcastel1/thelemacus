//
//  check_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef check_field_h
#define check_field_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "answer.h"

using namespace std;


//this is a GUI field contaning a binary checkbox, which is either checked or unchecked
template<class P, class T> class CheckField{
    
public:
    
    //the parent frame to which this object is attached
    P* parent;
    Answer* answer;
    //related_field is a GUI field (such as ChronoField, etc) related to this CheckField, such that: if direct_reverse = true->  when the checkbox in this CheckFIeld is checked (unchecked), related_field is active (inactive). If direct_reverse = false ->  when the checkbox in this CheckFIeld is unchecked (checked), related_field is active (inactive).
    T* related_field;
    bool direct_reverse;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxCheckBox with the name of the bodies
    wxCheckBox* checkbox;
    CheckCheck<P,T>* check;
    
    CheckField(wxPanel*, Answer*, T*, bool);
    
    template<class R> void InsertIn(R*);
    template<class S> void get(S&);
    void set(void);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
    
};



#endif
