//
//  check_unit.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_unit.h"

#include "my_app.h"
#include "sight_frame.h"


template<class P, class GUI> CheckUnit<P, GUI>::CheckUnit(GUI* p_in, const vector<String>& unit_types_in) {

    p = p_in;
    unit_types = unit_types_in;

}

template class CheckUnit<RouteFrame, DynamicLengthField<RouteFrame>>;
template class CheckUnit<SightFrame, DynamicLengthField<SightFrame>>;
template class CheckUnit<RouteFrame, SpeedField<RouteFrame>>;


//check the unit in the GUI field in LengthField
template<class P, class GUI> template <class T> void CheckUnit<P, GUI>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        
        p->unit->CheckInCatalog(&check, &i);

        if (check || (((p->unit->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String((p->unit->name->GetValue().ToStdString())) == String("")))) {

            //if check is true (false) -> set unit->ok to true (false)
            (p->unit->ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            p->unit->name->SetForegroundColour(wxGetApp().foreground_color);
            p->unit->name->SetFont(wxGetApp().default_font);


        }else {

            stringstream temp;

            temp.str("");
            temp << "Available units are: ";
            for (i = 0; i < unit_types.size(); i++) {
                temp << (unit_types[i]).value << ((i < unit_types.size() - 1) ? ", " : ".");
            }

            f->print_error_message->SetAndCall((p->unit->name), String("Unit not found in list!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            (p->unit->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckUnit<RouteFrame, DynamicLengthField<RouteFrame>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckUnit<SightFrame, DynamicLengthField<SightFrame>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckUnit<RouteFrame, SpeedField<RouteFrame> >::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckUnit<RouteFrame, SpeedField<RouteFrame> >::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckUnit<ChartFrame, DynamicLengthField<ChartFrame> >::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckUnit<RouteFrame, DynamicLengthField<RouteFrame> >::operator()<wxFocusEvent>(wxFocusEvent&);
template void CheckUnit<SightFrame, DynamicLengthField<SightFrame> >::operator()<wxFocusEvent>(wxFocusEvent&);
