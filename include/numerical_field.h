//
//  numerical_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef numerical_field_h
#define numerical_field_h

#include <iostream>

using namespace std;


//a GUI field used to store a numerical value (e.g. a Length, a Speed, etc)
template<class P, class NON_GUI, class CHECK> class NumericalField{

public:

    //the wxFrame parent of *this
    P* parent;
    //the non-GUI object related to *this: for example, if I create a  class LengthField derived from Numerical, then *object will be a Length object
    NON_GUI* object;
    //the value
    wxTextCtrl *value;
    //a pointer to the functor that will be used to check whether *this is valid
    CHECK* check;
    //this points to an external vector<NON_GUI> where the recent values of *this are stored
    NON_GUI* recent_value;
    double value_before_editing;
    bool ok, /*this is true if *this is being edited, false otherwise*/ editing;
    wxBoxSizer *sizer_h, *sizer_v;

    NumericalField(wxPanel*, NON_GUI*, NON_GUI*);

    void FillInRecentValue(void);
    template<class E> void Check(E&);
    template<class T> void InsertIn(T*);
    template<class T> void InsertIn(T*, wxSizerFlags&);

};

#endif
