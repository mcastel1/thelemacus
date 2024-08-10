//
//  numerical_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef numerical_field_h
#define numerical_field_h

#include <iostream>

#include "multiple_item_field.h"

using namespace std;


//a GUI field used to store a numerical value (e.g. a Length, a Speed, etc). Here NON_GUI is the type of the NON_GUI field related to *this, e.g. Speed, and NON_GUI_VALUE is the type of the unit of the NON_GUI field, e.g. SpeedUnit. CHECK is the type of the functor that checks the entire field, and CHECK_UNIT is the type of the functor that checks the unit only 
template<class P, class NON_GUI, class NON_GUI_UNIT, class CHECK, class CHECK_UNIT> class NumericalField{

public:

    //the wxFrame parent of *this
    P* parent;
    //the non-GUI object related to *this: for example, if I create a  class LengthField derived from Numerical, then *object will be a Length object
    NON_GUI* object;
    //the GUI field for the value of *this
    wxTextCtrl *value;
    //the GUI field for the unit of measure of *this
    MultipleItemField<P, NON_GUI_UNIT, CHECK_UNIT>* unit;
    //a pointer to the functor that will be used to check whether *this is valid
    CHECK* check;
    //this points to an external vector<NON_GUI> where the recent values of *this are stored
    double* recent_value;
    double value_before_editing;
    bool ok, value_ok, /*this is true if *this is being edited, false otherwise*/ editing;
    wxBoxSizer *sizer_h, *sizer_v;

    NumericalField(wxPanel*, NON_GUI*, double*, vector<int>*);

    void FillInRecentValue(void);
    void Enable(bool);
    template<class E> void Check(E&);
    template<class T> void InsertIn(T*);
    template<class T> void InsertIn(T*, wxSizerFlags&);

};

#endif
