//
//  set_string_field_to_current_time.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "set_string_field_to_current_time.h"

#include "constants.h"
#include "my_time.h"
#include "position_frame.h"
#include "route_frame.h"
#include "sight_frame.h"
#include "string_field.h"


template<class P> template <class T> void SetStringFieldToCurrentTime<P>::operator()(T& event) {

    //if the label is empty, I replace it with the local time and date
    if ((p->value->GetValue()).IsEmpty()) {

        Time now;

        now.set_current_utc();
        //I write in the non-GUI object (p->string)
        (*(p->string)) = String(now.to_string(data_precision.get(), true));

        p->set();

    }

    event.Skip(true);

}

template void SetStringFieldToCurrentTime<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void SetStringFieldToCurrentTime<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void SetStringFieldToCurrentTime<PositionFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
