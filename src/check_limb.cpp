//
//  check_limb.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_limb.h"
#include "my_app.h"


template<class P> CheckLimb<P>::CheckLimb(LimbField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckLimb<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        
        check = ((p->checked_items.GetCount()) == 1);

        if (check || (((p->name->GetForegroundColour()) != (wxGetApp().error_color)) && ((p->checked_items.GetCount()) == 0))) {
            //p->name either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set ok to true (false)
            (p->ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in name
            p->name->SetForegroundColour(wxGetApp().foreground_color);
            p->name->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall(p->name, String("Error"), String("Limb not valid! Limb must be upper, center or lower."), (wxGetApp().path_file_error_icon));
            (p->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}
