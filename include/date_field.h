//
//  date_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef date_field_h
#define date_field_h

#include <iostream>

using namespace std;

#include "color.h"
#include "date.h"
#include "static_text.h"


template<class P> class DateField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    wxArrayString days, months;
    //year, month and day boxes
    wxTextCtrl *year;
    wxComboBox *month, *day;
    //texts
    StaticText* text_hyphen_1, *text_hyphen_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Date* date;
    //year_ok = true if the year is formatted properly and set to the same value as date->Y, and similarly for the other variables
    bool year_ok, month_ok, day_ok;
    CheckDate<P>* check;
    
    DateField(wxPanel*, Date*);
    void set(Date);
    void SetBackgroundColor(Color);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditYear(E&);
    template<class E> void OnEditMonth(E&);
    template<class E> void OnEditDay(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
};



#endif
