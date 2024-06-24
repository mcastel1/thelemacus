//
//  check_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "check_field.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/checkbox.h>

#include "check_check.h"


#include "answer.h"
#include "chrono_field.h"
#include "sight_frame.h"


//constructor of a CheckField object, based on the parent frame frame
template<class P, class T> CheckField<P, T>::CheckField(wxPanel* panel_of_parent, Answer* p, T* related_field_in, bool direct_reverse_in) {

    parent = ((P*)(panel_of_parent->GetParent()));
    //I link the internal pointers p and c to the respective Answer object
    answer = p;
    related_field = related_field_in;
    direct_reverse = direct_reverse_in;

    check = new CheckCheck<P, T>(this);

    checkbox = new wxCheckBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    //SetColor(checkbox);
    checkbox->Bind(wxEVT_CHECKBOX, (*check));

    checkbox->SetValue(false);

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(checkbox, 0, wxALIGN_CENTER);

}


//explicit instantiation
template class  CheckField<SightFrame, ChronoField<SightFrame>>;


//this function writes into the non-GUI field answer the value entered in the GUI box
template<class P, class T> template<class S> void CheckField<P, T>::get(S& event) {

    //I set p->answer to the value entered in the GUI checkbox
    if (checkbox->GetValue()) {
        (answer->value) = 'y';
    }
    else {
        (answer->value) = 'n';
    }

    event.Skip(true);

}

//explicit instantiation
template void CheckField<SightFrame, ChronoField<SightFrame>>::get<wxCommandEvent>(wxCommandEvent&);
template void CheckField<SightFrame, DynamicLengthField<SightFrame>>::get<wxCommandEvent>(wxCommandEvent&);


//sets the value in the GUI object check equal to the value in the non-GUI limb object answer
template<class P, class T> void CheckField<P, T>::set(void) {

    if ((answer->value) == 'y') {
        checkbox->SetValue(true);
    }

    if ((answer->value) == 'n') {
        checkbox->SetValue(false);
    }

}

//explicit instantiation
template void CheckField<SightFrame, ChronoField<SightFrame>>::set();
template void CheckField<SightFrame, DynamicLengthField<SightFrame>>::set();


template<class P, class T> template <typename EventTag, typename Method, typename Object> void CheckField<P, T>::Bind(EventTag tag, Method method, Object object) {

    checkbox->Bind(tag, method, object);

}

//explicit instantiation
template void CheckField<SightFrame, DynamicLengthField<SightFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);


//explicit instantiation
template void CheckField<SightFrame, ChronoField<SightFrame>>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);


template<class P, class T> template<class R> void CheckField<P, T>::InsertIn(R* host) {

    host->Add(sizer_v);

}

//explicit instantiation
template void CheckField<SightFrame, ChronoField<SightFrame>>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void CheckField<SightFrame, DynamicLengthField<SightFrame>>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);

