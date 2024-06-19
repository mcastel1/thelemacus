//
//  check_hour.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_hour.h"

#include "my_app.h"
#include "my_string.h"

template<class P> CheckHour<P>::CheckHour(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckHour<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24);


        if (check || ((((p->hour)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->hour)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set hour_ok to true (false)
            (p->hour_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            (p->hour)->SetForegroundColour(wxGetApp().foreground_color);
            (p->hour)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->hour), String("Entered value is not valid!"), String("Hours must be unsigned integer numbers >= 0 and < 24"), (wxGetApp().path_file_error_icon));

            (p->hour_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}
