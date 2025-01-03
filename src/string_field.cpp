//
//  string_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "string_field.h"

#include "check_string.h"
#include "constants.h"
#include "position_frame.h"
#include "route_frame.h"
#include "sight_frame.h"


//constructor of a StringField object, based on the parent frame frame
template<class P> StringField<P>::StringField(wxPanel* panel_of_parent, String* p) {

    parent = ((P*)(panel_of_parent->GetParent()));
    string = p;

    //initialize check
    check = new CheckString<P>(this);

    (set_to_current_time.p) = this;

    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_string_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, (*check));

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);

}

template class StringField<RouteFrame>;
template class StringField<SightFrame>;
template class StringField<PositionFrame>;


template<class P> template<class T> void StringField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template void StringField<RouteFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
template void StringField<SightFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);

//bind this -> value to method
template <class P> template <typename EventTag, typename Method, typename Object> void StringField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind value to method
    value->Bind(tag, method, object);

}

template void StringField<RouteFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void StringField<SightFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);
template void StringField<PositionFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (PositionFrame::*)(wxKeyEvent&), PositionFrame*>(wxEventTypeTag<wxKeyEvent>, void (PositionFrame::*)(wxKeyEvent&), PositionFrame*);
template void StringField<PositionFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);


//I write in the non-GUI object string the value entered in the GUI object value
template<class P> template<class T> void StringField<P>::get(T& event) {

    //here I don't check whether the StringField is ok, because any value in the string field is ok
    (*string) = String((value->GetValue().ToStdString()));

    event.Skip(true);

}

template void StringField<RouteFrame>::get<wxCommandEvent>(wxCommandEvent&);
template void StringField<SightFrame>::get<wxCommandEvent>(wxCommandEvent&);
template void StringField<PositionFrame>::get<wxCommandEvent>(wxCommandEvent&);


//sets the value in the GUI object value equal to the value in the non-GUI String object string
template<class P> void StringField<P>::set(void) {

    value->SetValue(wxString(string->value));

}

template void StringField<RouteFrame>::set();
template void StringField<SightFrame>::set();
template void StringField<PositionFrame>::set();
