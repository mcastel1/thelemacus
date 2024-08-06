//
//  check_unit.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_unit.h"

#include "my_app.h"
#include "sight_frame.h"


template<class P> CheckUnit<P>::CheckUnit(DynamicLengthField<P>* p_in) {

    p = p_in;

}

template class CheckUnit<RouteFrame>;
template class CheckUnit<SightFrame>;


//check the unit in the GUI field in LengthField
template<class P> template <class T> void CheckUnit<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        
        p->unit->CheckInCatalog(&check, &i);

        if (check || (((p->unit->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->unit->name->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set unit->ok to true (false)
            (p->unit->ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            p->unit->name->SetForegroundColour(wxGetApp().foreground_color);
            p->unit->name->SetFont(wxGetApp().default_font);


        }else {

            stringstream temp;

            temp.str("");
            temp << "Available units are: ";
            for (i = 0; i < LengthUnit_types.size(); i++) {
                temp << (LengthUnit_types[i]).value << ((i < LengthUnit_types.size() - 1) ? ", " : ".");
            }

            f->print_error_message->SetAndCall((p->unit->name), String("Unit not found in list!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            (p->unit->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template void CheckUnit<RouteFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckUnit<SightFrame>::operator()<wxCommandEvent>(wxCommandEvent&);
