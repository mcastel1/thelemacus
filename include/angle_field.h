//
//  angle_field.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//



#ifndef angle_field_h
#define angle_field_h

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/app.h"
#include "wx/artprov.h"
#endif

#include "static_text.h"

//class for graphical object: a field to enter an angle, composed of a box for the sign, a box for the degrees, a degree text symbol, another box for minutes and a minute text symbol
template<class P> class AngleField{
    
public:
    //the parent frame to which this object is attached
    P* parent;
    wxArrayString signs, degrees;
    //the format of the AngleField: String("") for angles  between 0 and 2*pi, String("+-") for angles with a sign between 0 and pi, String("NS") for latitudes between -pi/2 and pi/2, and String("EW") for longitudes between -pi and pi
    String format;
    //degrees and minutes boxes
    wxComboBox*sign, * deg;
    wxTextCtrl *min;
    //texts
    StaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    Angle* angle;
    //deg_ok = true if the degrees part of this angle is formatted properly and set to the same value as the degree part of angle, and simiarly for min
    bool sign_ok, deg_ok, min_ok;
    CheckAngle<P>* check;
    
    
    AngleField(wxPanel*, Angle*, String);
    void Enable(bool);
    void set(void);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    template<class E> void OnEditSign(E&);
    template<class E> void OnEditArcDegree(E&);
    template<class E> void OnEditArcMinute(E&);
    template <typename EventTag, typename Method, typename Object> void Bind(EventTag, Method, Object);
    
    
};


#endif
