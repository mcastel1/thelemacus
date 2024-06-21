//
//  angle_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include "angle_field.h"
#include "check_angle.h"
#include "constants.h"
#include "generic.h"
#include "my_string.h"
#include "route_frame.h"
#include "position_frame.h"
#include "sight_frame.h"




//constructor of an AngleField object, based on the parent frame 'frame'. Here format_in is the format of the angle
template <class P> AngleField<P>::AngleField(wxPanel* panel_of_parent, Angle* p, String format_in) {

    unsigned int i;
    parent = ((P*)(panel_of_parent->GetParent()));
    angle = p;

    format = format_in;

    signs.Clear();
    degrees.Clear();
    if (format == String("+-")) {
        //tabulate signs
        signs.Add(wxString("+"));
        signs.Add(wxString("-"));
    }
    if (format == String("NS")) {
        signs.Add(wxString("N"));
        signs.Add(wxString("S"));
    }
    if (format == String("EW")) {
        //here W <-> + sign, E <-> - sign
        signs.Add(wxString("E"));
        signs.Add(wxString("W"));
    }

    //tabuate degrees
    if ((format == String(""))) {
        for (i = 0; i < 360; i++) {
            degrees.Add(wxString::Format(wxT("%i"), i));
        }
    }
    else {
        if (format == String("NS")) {
            for (i = 0; i <= 90; i++) {
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }
        else {
            for (i = 0; i <= 180; i++) {
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }
    }



    //initialize check and its objects
    check = new CheckAngle<P>(this);

    //here the allocation of sign is inserted in the code in such a way that if format = "+-" the sign is allocated before deg, text_deg, min, text_min: In this way, when the user tabs through the fields in PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if (format == String("+-")) {
        sign = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    }

    deg = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    //SetColor(deg);
    AdjustWidth(deg);
    deg->SetValue(wxString(""));
    deg_ok = false;
    deg->Bind(wxEVT_KILL_FOCUS, (*(check->check_arc_degree)));
    //as text is changed in deg from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    deg->Bind(wxEVT_COMBOBOX, &AngleField::OnEditArcDegree<wxCommandEvent>, this);
    deg->Bind(wxEVT_KEY_UP, &AngleField::OnEditArcDegree<wxKeyEvent>, this);


    text_deg = new StaticText((parent->panel), wxString(wxGetApp().degree_symbol.append(String(" ")).value), wxDefaultPosition, wxDefaultSize, 0);

    min = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS(sample_width_floating_point_field))));
    //SetColor(min);
    min->SetValue(wxString(""));
    min_ok = false;
    min->Bind(wxEVT_KILL_FOCUS, (*(check->check_arc_minute)));
    //as text is changed min by the user with the keyboard, call OnEditArcMinute
    min->Bind(wxEVT_KEY_UP, &AngleField::OnEditArcMinute<wxKeyEvent>, this);


    text_min = new StaticText((parent->panel), wxT("' "), wxDefaultPosition, wxDefaultSize, 0);

    //here the allocation of sign is inserted in the code in such a way that if format = "NS" || "EW" the sign is allocated after deg, text_deg, min, text_min: In this way, when the user tabs through the fields in the PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if ((format == String("NS")) || (format == String("EW"))) {
        sign = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    }

    if (format != String("")) {
        //if the AngleField format is either NS, EW or +-, the sign field is used -> I set its background colour, width and bind it to check->check_sign

        //SetColor(sign);
        AdjustWidth(sign);
        sign->SetValue(wxString(""));
        sign_ok = false;
        sign->Bind(wxEVT_KILL_FOCUS, (*(check->check_sign)));
        sign->Bind(wxEVT_COMBOBOX, (*(check->check_sign)));
        //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEditSign
        sign->Bind(wxEVT_COMBOBOX, &AngleField::OnEditSign<wxCommandEvent>, this);
        sign->Bind(wxEVT_KEY_UP, &AngleField::OnEditSign<wxKeyEvent>, this);


    }



    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    if (format != String("")) {

        if (format == String("+-")) {

            //in this case I display the sign before the numerical value of the angle
            sizer_h->Add(sign, 0, wxALIGN_LEFT);

            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);

        }
        else {

            //in this case I display the sign after the numerical value of the angle
            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);

            sizer_h->Add(sign, 0, wxALIGN_LEFT);

        }
    }
    else {

        //in this case I display only the numerical value of the angle
        sizer_h->Add(deg, 0, wxALIGN_CENTER);
        sizer_h->Add(text_deg);
        sizer_h->Add(min, 0, wxALIGN_CENTER);
        sizer_h->Add(text_min);

    }

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);

}


//writes to the non-GUI field angle the values written in the GUI fields sign, deg and min
template<class P> template <class T> void AngleField<P>::get(T& event) {

    if (is_ok()) {


        double min_temp;
        char c;

        //set a value to c to avoid uninitialized-variable warning
        c = ' ';

        if (format == String("")) {
            //in this case there is no sign in AngleField->this:

            c = '+';

        }
        else {
            //in this case there is a sign in AngleField->this: I write the sign in c

            wxString s;
            s = (sign->GetValue());

            if (format == String("+-")) {

                if (s == wxString("+")) { c = '+'; }
                else { c = '-'; }

            }

            if (format == String("EW")) {

                if (s == wxString("W")) { c = '+'; }
                else { c = '-'; }

            }

            if (format == String("NS")) {

                if (s == wxString("N")) { c = '+'; }
                else { c = '-'; }

            }

        }


        (min->GetValue()).ToDouble(&min_temp);

        angle->from_sign_deg_min(c, wxAtoi(deg->GetValue()), min_temp);

    }

    event.Skip(true);

}

// explicit instantiations
template void AngleField<PositionFrame>::get<wxCommandEvent>(wxCommandEvent&);
template void AngleField<RouteFrame>::get<wxCommandEvent>(wxCommandEvent&);
template void AngleField<SightFrame>::get<wxCommandEvent>(wxCommandEvent&);



//sets the value in the GUI objects deg and min equal to the value in the non-GUI Angle object angle
template <class P> void AngleField<P>::set(void) {

    unsigned int deg_temp;
    double min_temp;
    Angle angle_temp;


    if (format == String("")) {

        angle->to_deg_min(&deg_temp, &min_temp, display_precision.value);

    }
    else {
        //in this case format = +-, EW or NS

        if ((angle->value) < M_PI) {

            if (format == String("+-")) { sign->SetValue(wxString("+")); }
            if (format == String("EW")) { sign->SetValue(wxString("W")); }
            if (format == String("NS")) { sign->SetValue(wxString("N")); }

            angle->to_deg_min(&deg_temp, &min_temp, display_precision.value);

        }
        else {

            if (format == String("+-")) { sign->SetValue(wxString("-")); }
            if (format == String("EW")) { sign->SetValue(wxString("E")); }
            if (format == String("NS")) { sign->SetValue(wxString("S")); }

            (angle_temp.value) = 2.0 * M_PI - (angle->value);
            angle_temp.to_deg_min(&deg_temp, &min_temp, display_precision.value);

        }

    }

    //all the cases above must share these lines, so I put them here
    deg->SetValue(wxString::Format(wxT("%i"), deg_temp));
    min->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, min_temp));

    sign_ok = true;
    deg_ok = true;
    min_ok = true;

}

//checks whether the contents of the GUI fiels in AngleField are valid
template<class P> bool AngleField<P>::is_ok(void) {

    bool output;

    if (format == String("")) {
        //if the angle is in format "", the sign field is not used, so there is no need to check whether it is ok

        output = (deg_ok && min_ok);

    }
    else {
        //if the angle is in format "NS", "EW" or "+-" the sign field is  used, so I check that it is ok

        output = (sign_ok && deg_ok && min_ok);


    }

    return(output);

}

//this function is called every time a keyboard button is lifted in this->sign: it checks whether the text entered so far in this->sign is valid and runs AllOk
template<class P> template<class E> void AngleField<P>::OnEditSign(E& event) {

    bool success;

    find_and_replace_case_insensitive(sign, signs, &success, NULL);

    if (success) {

        //because the text in sign is valid, I set the background color of sign to white
        sign->SetForegroundColour(wxGetApp().foreground_color);
        sign->SetFont(wxGetApp().default_font);

    }

    //sign_ok is true/false is the text entered in sign is valid/invalid
    sign_ok = success;

    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->deg: it checks whether the text entered so far in deg is valid and runs AllOk
template<class P> template<class E> void AngleField<P>::OnEditArcDegree(E& event) {

    bool success;

    success = check_unsigned_int((deg->GetValue()).ToStdString(), NULL, true, 0, 360);

    if (success) {

        //because the text in sign is valid, I set the background color of deg to white
        deg->SetForegroundColour(wxGetApp().foreground_color);
        deg->SetFont(wxGetApp().default_font);

    }

    //sign_ok is true/false is the text entered in sign is valid/invalid
    deg_ok = success;

    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->min: it checks whether the text entered so far in min is valid and runs AllOk
template<class P> template<class E> void AngleField<P>::OnEditArcMinute(E& event) {

    bool success;

    success = check_double((min->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

    if (success) {

        //because the text in sign is valid, I set the background color of min to white
        min->SetForegroundColour(wxGetApp().foreground_color);
        min->SetFont(wxGetApp().default_font);

    }

    //min_ok is true/false is the text entered in min is valid/invalid
    min_ok = success;

    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}

//bind all GUI windows in *this to method
template<class P> template <typename EventTag, typename Method, typename Object> void AngleField<P>::Bind(EventTag tag, Method method, Object object) {


    if (format != String("")) {
        //the angle format contains a non-empty sign

        //I bind the sign to method
        sign->Bind(tag, method, object);
    }

    //I bind deg and min to method
    deg->Bind(tag, method, object);
    min->Bind(tag, method, object);

}

// explicit instantiations
template void AngleField<PositionFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (PositionFrame::*)(wxKeyEvent&), PositionFrame*>(wxEventTypeTag<wxKeyEvent>, void (PositionFrame::*)(wxKeyEvent&), PositionFrame*);
template void AngleField<RouteFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void AngleField<SightFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);



//this function enables/disable all the fields in AngleField
template<class P> void AngleField<P>::Enable(bool is_enabled) {

    if (format != String("")) {
        sign->Enable(is_enabled);
    }
    deg->Enable(is_enabled);
    min->Enable(is_enabled);

}


template<class P> template<class T> void AngleField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}
// explicit instantiations
template void AngleField<PositionFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void AngleField<RouteFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void AngleField<SightFrame>::InsertIn<wxBoxSizer>(wxBoxSizer*);
template void  AngleField<SightFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);


// explicit instantiations
template class AngleField<PositionFrame>;
template class AngleField<RouteFrame>;
template class AngleField<SightFrame>;
