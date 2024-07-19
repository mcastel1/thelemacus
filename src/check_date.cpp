//
//  check_date.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_date.h"

#include "check_day.h"
#include "check_month.h"
#include "check_year.h"
#include "sight_frame.h"



template<class P> CheckDate<P>::CheckDate(DateField<P>* p_in) {

    p = p_in;

    check_year = new CheckYear<P>(p);
    check_month = new CheckMonth<P>(p);
    check_day = new CheckDay<P>(p);

}

template class CheckDate<SightFrame>;


//this functor checks the whole date field by calling the check on its year, month and day parts
template<class P> template <class T> void CheckDate<P>::operator()(T& event) {

    (*check_year)(event);
    (*check_month)(event);
    (*check_day)(event);

    event.Skip(true);

}

template void CheckDate<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
