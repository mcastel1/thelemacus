//
//  tabulate_days.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "tabulate_days.h"

#include "constants.h"
#include "sight_frame.h"


template<class P> TabulateDays<P>::TabulateDays(DateField<P>* p_in) {

    p = p_in;

}



template<class P> template<class T> void TabulateDays<P>::operator()(T& event) {

    unsigned int i;
    SightFrame* f = (p->parent_frame);

    if ((p->year_ok) && (p->month_ok)) {

        wxString temp;

        //save the old value of p->day into temp
        temp = (p->day->GetValue());

        //read the year
        ((((f->sight)->master_clock_date_and_hour).date).Y) = ((unsigned int)wxAtoi(p->year->GetValue()));
        (((f->sight)->master_clock_date_and_hour).date).check_leap_year();

        //read the month
        ((((f->sight)->master_clock_date_and_hour).date).M) = ((unsigned int)wxAtoi(p->month->GetValue()));

        if (((f->sight)->master_clock_date_and_hour).date.Y_is_leap_year) {
            //in this case the year is a leap year: I fill the list of days from days_per_month_leap

            for ((p->days).Clear(), i = 0; i < days_per_month_leap[(((f->sight)->master_clock_date_and_hour).date.M) - 1]; i++) {
                (p->days).Add(wxString::Format(wxT("%i"), i + 1));
            }

        }
        else {
            //in this case the year is a common year: I fill the list of days from days_per_month_common

            for ((p->days).Clear(), i = 0; i < days_per_month_common[(((f->sight)->master_clock_date_and_hour).date.M) - 1]; i++) {
                (p->days).Add(wxString::Format(wxT("%i"), i + 1));
            }
            //
        }

        p->day->Set(p->days);

        if (!(wxAtoi(temp) <= wxAtoi((p->days)[(p->days).GetCount() - 1]))) {
            //if the value in p->day is does not lie between the boundaries of the newly set days list (list of days of the month, then I reset it by setting it to 1

            p->day->SetValue(wxString("1"));

        }
        else {

            p->day->SetValue(temp);

        }

        p->day->Enable(true);

    }
    else {

        p->day->Enable(false);

    }

    event.Skip(true);

}
