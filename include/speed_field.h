//
//  speed_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef speed_field_h
#define speed_field_h

#include <iostream>

using namespace std;

#include "check_unit.h"
#include "numerical_field.h"
#include "speed.h"
#include "speed_unit_field.h"

template<class P> class CheckSpeed;
class Speed;
template<class P> class SpeedUnitField;


//class for graphical object: a field to enter a speed, composed of a box and a dropdown menu to enter the units of measure of the speed. P is the type of the parent which hosts the DynamicLengthField object
template<class P> class SpeedField: public NumericalField<P, Speed, SpeedUnit, CheckSpeed<P>, CheckUnit<P, SpeedField<P>>>{
    
public:
    //the parent frame to which this object is attached
//    P* parent;
    //the speed value
//    wxTextCtrl *value;
    //unit of measure of the Speed written in *this
    SpeedUnitField<P>* unit;
//    wxBoxSizer *sizer_h, *sizer_v;
//    Speed* speed;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool value_ok;
//    bool /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
//    CheckSpeed<P>* check;

    
    SpeedField(wxPanel*, Speed*, Speed*);
    void set(void);
    template<class T> void get(T&);
//    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditValue(E&);
    template<class E> void OnEditUnit(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
};



#endif
