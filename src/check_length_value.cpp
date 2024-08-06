//
//  check_length_value.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_length_value.h"

#include "generic.h"
#include "my_app.h"
#include "my_string.h"
#include "sight_frame.h"

template<class P> CheckLengthValue<P>::CheckLengthValue(DynamicLengthField<P>* p_in) {

    p = p_in;

}

template class CheckLengthValue<RouteFrame>;
template class CheckLengthValue<SightFrame>;


//checks the value in the GUI field in DynamicLengthField
template<class P> template <class T> void CheckLengthValue<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

        if (check || (((p->value->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->value->GetValue()).ToStdString())) == String("")))) {
            //p->value either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set value_ok to true (false)
            (p->value_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in value
            p->value->SetForegroundColour(wxGetApp().foreground_color);
            p->value->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->value), String("Entered value is not valid!"), String("Lengths must be floating-point numbers >= 0 m"), (wxGetApp().path_file_error_icon));

            (p->value_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckLengthValue<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckLengthValue<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckLengthValue<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckLengthValue<SightFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
