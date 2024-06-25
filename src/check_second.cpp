//
//  check_second.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_second.h"

#include "my_app.h"
#include "my_string.h"
#include "route_frame.h"
#include "sight_frame.h"


template<class P> CheckSecond<P>::CheckSecond(ChronoField<P>* p_in) {

    p = p_in;

}

template class CheckSecond<RouteFrame>;
template class CheckSecond<SightFrame>;


template<class P> template<class T> void CheckSecond<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

        if (check || ((((p->second)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->second)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set second_ok to true (false)
            (p->second_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            (p->second)->SetForegroundColour(wxGetApp().foreground_color);
            (p->second)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->second), String("Entered value is not valid!"), String("Seconds must be floating-point numbers >= 0.0 and < 60.0"), (wxGetApp().path_file_error_icon));

            (p->second_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckSecond<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckSecond<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckSecond<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
