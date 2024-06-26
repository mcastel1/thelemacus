//
//  check_month.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_month.h"

#include "date_field.h"
#include "my_app.h"
#include "sight_frame.h"
#include "tabulate_days.h"


template<class P> CheckMonth<P>::CheckMonth(DateField<P>* p_in) {

    p = p_in;
    tabulate_days = new TabulateDays<P>(p);

}

template class CheckMonth<SightFrame>;



template<class P> template<class T> void CheckMonth<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->month->GetValue()).ToStdString(), NULL, true, 1, 12 + 1);

        if (check || (((p->month->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->month->GetValue()).ToStdString())) == String("")))) {
            //p->month either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set month_ok to true (false)
            (p->month_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in month
            p->month->SetForegroundColour(wxGetApp().foreground_color);
            p->month->SetFont(wxGetApp().default_font);

            if (check && (p->year_ok)) {

                (*tabulate_days)(event);

            }

            p->day->Enable(check && (p->year_ok));

        }
        else {

            f->print_error_message->SetAndCall((p->month), String("Entered value is not valid!"), String("Month must be an unsigned integer >= 1 and <= 12"), (wxGetApp().path_file_error_icon));

            (p->month_ok) = false;
            p->day->Enable(false);

        }


        f->AllOk();

    }

    event.Skip(true);

}

template void CheckMonth<SightFrame>::operator()<wxFocusEvent>(wxFocusEvent&);
