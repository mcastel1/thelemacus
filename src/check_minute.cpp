//
//  check_minute.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_minute.h"



template<class P> CheckMinute<P>::CheckMinute(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P>  template<class T> void CheckMinute<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60);

        if (check || ((((p->minute)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->minute)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set minute_ok to true (false)
            (p->minute_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in minute
            (p->minute)->SetForegroundColour(wxGetApp().foreground_color);
            (p->minute)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->minute), String("Entered value is not valid!"), String("Minutes must be unsigned integer numbers >= 0 and < 60"), (wxGetApp().path_file_error_icon));

            (p->minute_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}
