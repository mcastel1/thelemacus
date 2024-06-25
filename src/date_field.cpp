//
//  date_field.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "date_field.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/textctrl.h>


#include "date.h"

#include "check_date.h"
#include "check_day.h"
#include "check_month.h"
#include "check_year.h"
#include "color.h"
#include "generic.h"
#include "my_app.h"
#include "sight_frame.h"
#include "static_text.h"
#include "tabulate_days.h"


//constructor of a DateField object, based on the parent frame frame
template<class P> DateField<P>::DateField(wxPanel* panel_of_parent, Date* p) {

    unsigned int i;
    parent_frame = ((P*)(panel_of_parent->GetParent()));
    date = p;


    //initialize check and its objects
    check = new CheckDate<P>(this);
    //    (check.p) = this;
    //    (check->check_year->p) = this;
    //    ((check->check_year->tabulate_days).p) = this;
    //    (check->check_month->p) = this;
    //    ((check->check_month->tabulate_days).p) = this;
    //    (check->check_day->p) = this;

    for (months.Clear(), months.Add(wxT("")), i = 0; i < 12; i++) {
        months.Add(wxString::Format(wxT("%i"), i + 1));
    }

    year = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(year);
    year->SetInitialSize(year->GetSizeFromTextSize(year->GetTextExtent(wxS("0000"))));
    year->SetValue(wxString(""));
    year_ok = false;
    year->Bind(wxEVT_KILL_FOCUS, *(check->check_year));
    //as text is changed year by the user with the keyboard, call OnEditYear
    year->Bind(wxEVT_KEY_UP, &DateField::OnEditYear<wxKeyEvent>, this);


    text_hyphen_1 = new StaticText((parent_frame->panel), wxT("-"), wxDefaultPosition, wxDefaultSize, 0);
    month = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(month);
    AdjustWidth(month);
    month->SetValue(wxString(""));
    month_ok = false;
    month->Bind(wxEVT_KILL_FOCUS, *(check->check_month));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    month->Bind(wxEVT_COMBOBOX, &DateField::OnEditMonth<wxCommandEvent>, this);
    month->Bind(wxEVT_KEY_UP, &DateField::OnEditMonth<wxKeyEvent>, this);

    text_hyphen_2 = new StaticText((parent_frame->panel), wxT("-"), wxDefaultPosition, wxDefaultSize, 0);

    day = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(day);
    //I create a temporary days list to set the size of the wxComboBox day with AdjustWidth, and then destroy this temporary days list
    for (days.Clear(), i = 0; i < 31; i++) {
        days.Add(wxString::Format(wxT("%i"), i + 1));
    }
    day->Set(days);
    AdjustWidth(day);
    days.Clear();
    day->SetValue(wxString(""));
    day_ok = false;
    day->Bind(wxEVT_KILL_FOCUS, *(check->check_day));
    //as text is changed in day from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEditDay
    day->Bind(wxEVT_COMBOBOX, &DateField::OnEditDay<wxCommandEvent>, this);
    day->Bind(wxEVT_KEY_UP, &DateField::OnEditDay<wxKeyEvent>, this);

    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(year, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_1, wxALIGN_CENTER);
    sizer_h->Add(month, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_2, wxALIGN_CENTER);
    sizer_h->Add(day, 0, wxALIGN_CENTER);

}

template class DateField<SightFrame>;



template<class P> bool DateField<P>::is_ok(void) {

    return(year_ok && month_ok && day_ok);

}

template bool DateField<SightFrame>::is_ok();



//this function is called every time a keyboard button is lifted in this->year: it checks whether the text entered so far in year is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditYear(E& event) {

    bool success;

    success = check_unsigned_int((year->GetValue()).ToStdString(), NULL, false, 0, 0);

    if (success) {

        year->SetForegroundColour(wxGetApp().foreground_color);
        year->SetFont(wxGetApp().default_font);

    }

    //year_ok is true/false is the text enteres is valid/invalid
    year_ok = success;

    (*(((this->check)->check_month)->tabulate_days))(event);

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->month: it checks whether the text entered so far in month is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditMonth(E& event) {

    bool success;

    success = check_unsigned_int((month->GetValue()).ToStdString(), NULL, true, 1, 12 + 1);

    if (success) {

        month->SetForegroundColour(wxGetApp().foreground_color);
        month->SetFont(wxGetApp().default_font);

    }

    //month_ok is true/false is the text enteres is valid/invalid
    month_ok = success;

    (*(((this->check)->check_month)->tabulate_days))(event);

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->day: it checks whether the text entered so far in day is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditDay(E& event) {

    bool success;

    date->check_leap_year();

    if (date->Y_is_leap_year) {

        success = check_unsigned_int((day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi(month->GetValue())) - 1] + 1);

    }
    else {

        success = check_unsigned_int((day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi(month->GetValue())) - 1] + 1);

    }

    if (success) {

        day->SetForegroundColour(wxGetApp().foreground_color);
        day->SetFont(wxGetApp().default_font);

    }

    //day_ok is true/false is the text enteres is valid/invalid
    day_ok = success;

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}



template<class P> template <typename EventTag, typename Method, typename Object> void DateField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind year, month and day to method
    year->Bind(tag, method, object);
    month->Bind(tag, method, object);
    day->Bind(tag, method, object);

}

template void DateField<SightFrame>::Bind<wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*>(wxEventTypeTag<wxKeyEvent>, void (SightFrame::*)(wxKeyEvent&), SightFrame*);


//set color as the background color in all fields of *this
template<class P> void DateField<P>::SetBackgroundColor(Color color) {

    year->SetBackgroundColour(color);
    month->SetBackgroundColour(color);
    day->SetBackgroundColour(color);

}



template<class P> template<class T> void DateField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template void DateField<SightFrame>::InsertIn<wxBoxSizer>(wxBoxSizer*);


//this functor writes the values written inthe whole GUI date field (year, month and day) in the respective non-GUI object date->D, date->M, date->D
template<class P> template <class T> void DateField<P>::get(T& event) {

    if (year_ok && (year->IsEnabled()) && month_ok && (month->IsEnabled()) && day_ok && (month->IsEnabled())) {

        (date->Y) = (unsigned int)wxAtoi(year->GetValue());
        (date->M) = (unsigned int)wxAtoi(month->GetValue());
        (date->D) = (unsigned int)wxAtoi(day->GetValue());

    }

    event.Skip(true);

}

//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date_in
template<class P> void DateField<P>::set(Date date_in) {

    //    Time time_UTC;
    //
    //    //(parent_frame->sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    //    time_UTC = (parent_frame->sight->time);
    //    time_UTC -= (parent_frame->sight->TAI_minus_UTC);

    year->SetValue(wxString::Format(wxT("%i"), date_in.Y));
    month->SetValue(wxString::Format(wxT("%i"), date_in.M));
    day->SetValue(wxString::Format(wxT("%i"), date_in.D));

    year_ok = true;
    month_ok = true;
    day_ok = true;

}
