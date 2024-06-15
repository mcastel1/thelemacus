//
//  chrono_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef chrono_field_h
#define chrono_field_h

#include <iostream>

#include "chrono.h"
#include "color.h"

using namespace std;

class Color;


//a GUI field containing a time, which is contained by an object of type P
template<class P> class ChronoField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    wxArrayString hours, minutes;
    //hour and minute  boxes
    wxComboBox *hour, *minute;
    //second text control
    wxTextCtrl *second;
    //texts
    StaticText* static_text_1, *static_text_2, *static_text_3;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Chrono* chrono;
    //hour_ok = true if the hour is formatted properly and set to the same value as chrono->h, and similarly for the other variables
    bool hour_ok, minute_ok, second_ok;
    CheckChrono<P>* check;
    
    ChronoField(wxPanel*, Chrono*);
    void set(void);
    void set(Chrono);
    void Enable(bool);
    void SetBackgroundColor(Color);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditHour(E&);
    template<class E> void OnEditMinute(E&);
    template<class E> void OnEditSecond(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
};


#endif
