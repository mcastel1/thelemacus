//
//  check_arc_degree.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_arc_degree.h"

#include "my_app.h"
#include "position_frame.h"
#include "sight_frame.h"



template<class P> CheckArcDegree<P>::CheckArcDegree(AngleField<P>* p_in) {

    p = p_in;

}

template class CheckArcDegree<RouteFrame>;
template class CheckArcDegree<SightFrame>;
template class CheckArcDegree<PositionFrame>;


template<class P> template<class T> void CheckArcDegree<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->deg->GetValue()).ToStdString(), NULL, true, 0, 360);

        if (check || (((p->deg->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->deg->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set deg_ok to true (false)
            (p->deg_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            p->deg->SetForegroundColour(wxGetApp().foreground_color);
            p->deg->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->deg), String("Entered value is not valid!"), String("Arcdegrees must be unsigned integer numbers between 0 and 359").append(wxGetApp().degree_symbol), (wxGetApp().path_file_error_icon));

            (p->deg_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckArcDegree<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckArcDegree<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckArcDegree<SightFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckArcDegree<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckArcDegree<PositionFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
