//
//  numerical_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef numerical_field_h
#define numerical_field_h

#include <iostream>
#include <vector>


using namespace std;


template<class P, class NON_GUI, class CHECK> class NumericalField{

public:

    //the wxFrame parent of *this
    P* parent;
    //the non-GUI object related to *this: for example, if I create a  class LengthField derived from Numerical, then *object will be a Length object
    NON_GUI* object;
    //a pointer to the functor that will be used to check whether *this is valid
    CHECK* check;

    NumericalField(wxPanel*, NON_GUI*, vector<NON_GUI>*);

};

#endif
