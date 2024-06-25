//
//  chrono_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "chrono_field.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "check_chrono.h"
#include "check_hour.h"
#include "check_minute.h"
#include "check_second.h"
#include "color.h"
#include "constants.h"
#include "chrono.h"
#include "generic.h"
#include "my_app.h"
#include "route_frame.h"
#include "sight_frame.h"
#include "static_text.h"



//constructor of a ChronoField object, based on the parent frame frame
template<class P> ChronoField<P>::ChronoField(wxPanel* panel_of_parent, Chrono* p) {

    unsigned int i;
    parent_frame = ((P*)(panel_of_parent->GetParent()));
    chrono = p;

    check = new CheckChrono<P>(this);
    //    (check.p) = this;

    for (hours.Clear(), hours.Add(wxT("")), i = 0; i < 24; i++) {
        hours.Add(wxString::Format(wxT("%i"), i));
    }
    for (minutes.Clear(), minutes.Add(wxT("")), i = 0; i < 60; i++) {
        minutes.Add(wxString::Format(wxT("%i"), i));
    }

    hour = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(hour);
    //    hour->SetInitialSize(hour->GetSizeFromTextSize(hour ->GetTextExtent(wxS("00"))));
    AdjustWidth(hour);
    hour->SetValue(wxString(""));
    hour_ok = false;
    hour->Bind(wxEVT_KILL_FOCUS, *(check->check_hour));
    //as text is changed in hour from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    hour->Bind(wxEVT_COMBOBOX, &ChronoField::OnEditHour<wxCommandEvent>, this);
    hour->Bind(wxEVT_KEY_UP, &ChronoField::OnEditHour<wxKeyEvent>, this);

    static_text_1 = new StaticText((parent_frame->panel), wxT("h"), wxDefaultPosition, wxDefaultSize, 0);

    minute = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(minute);
    AdjustWidth(minute);
    //    minute->SetInitialSize(minute->GetSizeFromTextSize(minute->GetTextExtent(wxS("00"))));
    minute->SetValue(wxString(""));
    minute_ok = false;
    minute->Bind(wxEVT_KILL_FOCUS, *(check->check_minute));
    //as text is changed in minute from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    minute->Bind(wxEVT_COMBOBOX, &ChronoField::OnEditMinute<wxCommandEvent>, this);
    minute->Bind(wxEVT_KEY_UP, &ChronoField::OnEditMinute<wxKeyEvent>, this);

    static_text_2 = new StaticText((parent_frame->panel), wxT("m"), wxDefaultPosition, wxDefaultSize, 0);

    second = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    second->SetInitialSize(second->GetSizeFromTextSize(second->GetTextExtent(wxS(sample_width_floating_point_field))));
    //SetColor(second);
    second->SetValue(wxString(""));
    second_ok = false;
    second->Bind(wxEVT_KILL_FOCUS, *(check->check_second));
    //as text is changed in second from the user with either a keyboard button, call OnEditSecond
    second->Bind(wxEVT_KEY_UP, &ChronoField::OnEditSecond<wxKeyEvent>, this);
    static_text_3 = new StaticText((parent_frame->panel), wxT("s"), wxDefaultPosition, wxDefaultSize, 0);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(hour, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_1, 0, wxALIGN_CENTER);
    sizer_h->Add(minute, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_2, 0, wxALIGN_CENTER);
    sizer_h->Add(second, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_3, 0, wxALIGN_CENTER);

}

template class ChronoField<RouteFrame>;

//this function writes into the non-GUI fields in chrono the value written into the respective GUI fields hour, minute and second
template<class P> template <class T> void ChronoField<P>::get(T& event) {

    if (hour_ok && (hour->IsEnabled()) && minute_ok && (minute->IsEnabled()) && second_ok && (second->IsEnabled())) {
        //I write only if hour, minute and second are ok and enabled

        double s_temp;

        (chrono->h) = ((unsigned int)wxAtoi(hour->GetValue()));
        ((chrono)->m) = ((unsigned int)wxAtoi(minute->GetValue()));
        ((second)->GetValue()).ToDouble(&s_temp);
        ((chrono)->s) = s_temp;

    }

    event.Skip(true);

}

template void ChronoField<RouteFrame>::get<wxCommandEvent>(wxCommandEvent&);
template void ChronoField<SightFrame>::get<wxCommandEvent>(wxCommandEvent&);


//set the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object *chrono
template<class P> void ChronoField<P>::set(void) {

    set(*chrono);

}

template void ChronoField<RouteFrame>::set();
template void ChronoField<SightFrame>::set(Chrono);


//set the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object chrono_in
template<class P> void ChronoField<P>::set(Chrono chrono_in) {

    hour->SetValue(wxString::Format(wxT("%i"), chrono_in.h));
    minute->SetValue(wxString::Format(wxT("%i"), chrono_in.m));
    second->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, chrono_in.s));

    hour_ok = true;
    minute_ok = true;
    second_ok = true;

}

//set color as the background color in all fields of *this
template<class P> void ChronoField<P>::SetBackgroundColor(Color color) {

    hour->SetBackgroundColour(color);
    minute->SetBackgroundColour(color);
    second->SetBackgroundColour(color);

}



template<class P> bool ChronoField<P>::is_ok(void) {

    return(hour_ok && minute_ok && second_ok);

}

template bool ChronoField<RouteFrame>::is_ok();
template bool ChronoField<SightFrame>::is_ok();


//this function is called every time a keyboard button is lifted in this->hour: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditHour(E& event) {

    bool success;

    success = check_unsigned_int((hour->GetValue()).ToStdString(), NULL, true, 0, 24);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        hour->SetForegroundColour(wxGetApp().foreground_color);
        hour->SetFont(wxGetApp().default_font);

    }


    //hour_ok is true/false is the text entered is valid/invalid
    hour_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

template void ChronoField<RouteFrame>::OnEditHour<wxKeyEvent>(wxKeyEvent&);
template void ChronoField<RouteFrame>::OnEditHour<wxCommandEvent>(wxCommandEvent&);


//this function is called every time a keyboard button is lifted in this->minute: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditMinute(E& event) {

    bool success;

    success = check_unsigned_int((minute->GetValue()).ToStdString(), NULL, true, 0, 60);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        minute->SetForegroundColour(wxGetApp().foreground_color);
        minute->SetFont(wxGetApp().default_font);

    }


    //minute_ok is true/false is the text entered is valid/invalid
    minute_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

template void ChronoField<RouteFrame>::OnEditMinute<wxKeyEvent>(wxKeyEvent&);
template void ChronoField<RouteFrame>::OnEditMinute<wxCommandEvent>(wxCommandEvent&);


//this function is called every time a keyboard button is lifted in this->second: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditSecond(E& event) {

    bool success;

    success = check_double((second->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        second->SetForegroundColour(wxGetApp().foreground_color);
        second->SetFont(wxGetApp().default_font);

    }


    //second_ok is true/false is the text entered is valid/invalid
    second_ok = success;
    //tries to enable button_ok
    parent_frame->AllOk();

    event.Skip(true);

}

template void ChronoField<RouteFrame>::OnEditSecond<wxKeyEvent>(wxKeyEvent&);



template<class P> template <typename EventTag, typename Method, typename Object> void ChronoField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind hour, minute and second to method
    hour->Bind(tag, method, object);
    minute->Bind(tag, method, object);
    second->Bind(tag, method, object);

}

template  void ChronoField<RouteFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*>(wxEventTypeTag<wxKeyEvent>, void (RouteFrame::*)(wxKeyEvent&), RouteFrame*);
template void ChronoField<RouteFrame>::Bind<wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*>(wxEventTypeTag<wxCommandEvent>, void (RouteFrame::*)(wxCommandEvent&), RouteFrame*);
template void ChronoField<SightFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);


//this function enables/disable the whole ChronoField
template<class P> void ChronoField<P>::Enable(bool is_enabled) {

    hour->Enable(is_enabled);
    minute->Enable(is_enabled);
    second->Enable(is_enabled);

}

template void ChronoField<SightFrame>::Enable(bool);


template void ChronoField<RouteFrame>::Enable(bool);


template<class P> template<class T> void ChronoField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template void ChronoField<RouteFrame>::InsertIn<wxFlexGridSizer>(wxFlexGridSizer*);
