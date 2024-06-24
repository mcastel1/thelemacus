//
//  check_check.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_check.h"

#include "check_chrono.h"
#include "check_length.h"
#include "chrono_field.h"
#include "sight_frame.h"



template<class P, class T>  CheckCheck<P, T>::CheckCheck(CheckField<P, T>* p_in) {

    p = p_in;

}

//this function reads the value in the GUI box checkbox, and enables/disables the related_field accordingly
template<class P, class T> template<class R> void CheckCheck<P, T>::operator()(R& event) {

    //I enable/disable related_field according to whether checkbox is checked or not, and according to the value of direct_reverse
    if (((p->checkbox->GetValue()) ^ (!(p->direct_reverse)))) {
        
        p->related_field->Enable(true);
        
    }else {
        
        p->related_field->Enable(false);
        
    }

    (*(p->related_field->check))(event);

    p->parent->AllOk();

    event.Skip(true);

}

//explicit instantiation
template void CheckCheck<SightFrame, ChronoField<SightFrame>>::operator()<wxCommandEvent>(wxCommandEvent&);
template void CheckCheck<SightFrame, DynamicLengthField<SightFrame>>::operator()<wxCommandEvent>(wxCommandEvent&);
