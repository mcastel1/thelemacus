//
//  check_sign.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_sign.h"
#include "my_app.h"


template<class P> CheckSign<P>::CheckSign(AngleField<P>* p_in) {

    p = p_in;

}

//checks the value of the sign in the GUI field
template<class P> template <class T> void CheckSign<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is in idling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        //I check whether the name in the GUI field sign matches one of the sign values in the list signs
        if ((p->format) == String("")) {
            //if the AngleField p has no sign, the check is ok

            check = true;

        }
        else {
            //if the AngleField p has a sign, I check it

            for (check = false, i = 0; (i < ((p->signs).GetCount())) && (!check); i++) {
                if ((p->sign->GetValue()) == (p->signs)[i]) {
                    check = true;
                }
            }

        }


        if (check || (((p->sign->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->sign->GetValue()).ToStdString())) == String("")))) {
            //p->sign either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set sign_ok to true (false)
            (p->sign_ok) = check;

            if ((p->format) != String("")) {
                //there exists a p->sign field

                //the background color is set to white, because in this case there is no erroneous value in sign
                p->sign->SetForegroundColour(wxGetApp().foreground_color);
                p->sign->SetFont(wxGetApp().default_font);
            }

        }
        else {

            f->print_error_message->SetAndCall((p->sign), String("Error"), String("Sign is not valid! Sign must be +-, NS or EW."), (wxGetApp().path_file_error_icon));
            (p->sign_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}



