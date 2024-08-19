//
//  check_arc_minute.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_arc_minute.h"

#include "angle_field.h"
#include "generic.h"
#include "my_app.h"
#include "position_frame.h"
#include "sight_frame.h"


template<class P> CheckArcMinute<P>::CheckArcMinute(AngleField<P>* p_in) {

    p = p_in;

}

template class CheckArcMinute<RouteFrame>;
template class CheckArcMinute<SightFrame>;
template class CheckArcMinute<PositionFrame>;


template<class P> template <class T> void CheckArcMinute<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->min->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

        if (check || (((p->min->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->min->GetValue()).ToStdString())) == String("")))) {
            //p->min either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set min_ok to true (false)
            (p->min_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in min
            p->min->SetForegroundColour(wxGetApp().foreground_color);
            p->min->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->min), String("Entered value is not valid!"), String("Arcminutes must be floating-point numbers >= 0' and < 60'"), (wxGetApp().path_file_error_icon));

            (p->min_ok) = false;


        }
        
        //if p is valid and p->recent_angle is not NULL, I write the content of the GUI field p into the non-GUI field *recent_angle so this content will be stored for the next time the user uses p
        if((p->is_ok()) && ((p->recent_angle) != NULL)){
            
            //write the value written in *p into *recent_angle
            p->get_to_Angle((p->recent_angle));
  
        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckArcMinute<RouteFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckArcMinute<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckArcMinute<SightFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckArcMinute<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckArcMinute<PositionFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
