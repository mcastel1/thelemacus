//
//  sight_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "sight_frame.h"

#include "check_check.h" 
#include "check_date.h"
#include "check_length.h"
#include "constants.h"
#include "date_field.h"
#include "limb_field.h"
#include "string_field.h"



SightFrame::SightFrame(ListFrame* parent_input, Sight* sight_in, long position_in_listcontrol_sights_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    parent = parent_input;

    //pointer to init.txt to read fixed sight data from in there
    String new_prefix;
    unsigned int common_width;
    bool check = true;


    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    (*(parent->set_idling))();

    set_idling = new SetIdling<SightFrame>(this);
    unset_idling = new UnsetIdling<SightFrame>(this);
    (*unset_idling)();

    print_error_message = new PrintMessage<SightFrame, UnsetIdling<SightFrame> >(this, unset_idling);


    catalog = new Catalog((wxGetApp().path_file_catalog), String(""));

    //if this SightFrame has been constructed with sight_in = NULL, then I allocate a new Sight object with the pointer this->sight and set position_in_listcontrol_sights to a 'NULL' value (position_in_listcontrol_sights = -1). Otherwise, the pointer sight_in points to a valid Sight object -> I let this->sight point to sight_in, and set position_in_listcontrol_sights to position_in_listcontrol_sights_in.
    if (sight_in != NULL) {
        sight = sight_in;
        position_in_listcontrol_sights = position_in_listcontrol_sights_in;
    }
    else {
        sight = new Sight();
        position_in_listcontrol_sights = -1;
    }

    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    sizer_grid_measurement = new wxFlexGridSizer(6, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_time = new wxFlexGridSizer(5, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_label = new wxFlexGridSizer(1, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer = new wxBoxSizer(wxVERTICAL);

    //allocate buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);
    button_reduce = new wxButton(panel, wxID_ANY, "Reduce", wxDefaultPosition, wxDefaultSize);


    //First off, I need to set TAI_minus_UTC, which will be used in the following. If sight_in = NULL,  I read it from from file_init
    if (sight_in == NULL) {

        sight->TAI_minus_UTC.read_from_file_to(String("TAI - UTC at time of master-clock synchronization with UTC"), (wxGetApp().path_file_init), String("R"), new_prefix);

    }


    StaticText* text_combo_body = new StaticText(panel, wxT("Celestial body"), wxDefaultPosition, wxDefaultSize, 0);
    body = new BodyField<SightFrame>(panel, &(sight->body), catalog, &(wxGetApp().list_frame->data->recent_bodies));

    StaticText* text_limb = new StaticText(panel, wxT("Limb"), wxDefaultPosition, wxDefaultSize, 0);
    limb = new LimbField<SightFrame>(panel, &(sight->limb));
    (limb->name)->Enable(false);

    //sextant altitude
    StaticText* text_H_s = new StaticText(panel, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0);
    H_s = new AngleField<SightFrame>(panel, &(sight->H_s), String(""));

    //index error
    StaticText* text_index_error = new StaticText(panel, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0);
    //If sight_in = NULL, read index error from init file
    if (sight_in == NULL) {
        sight->index_error.read_from_file_to(String("index error"), (wxGetApp().path_file_init), String("R"), new_prefix);
    }
    index_error = new AngleField<SightFrame>(panel, &(sight->index_error), String("+-"));

    //artificial horizon
    StaticText* text_artificial_horizon_check = new StaticText(panel, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0);
    artificial_horizon_check = new CheckField<SightFrame, DynamicLengthField<SightFrame> >(panel, &(sight->artificial_horizon), NULL, false);

    //height of eye
    StaticText* text_height_of_eye = new StaticText(panel, wxT("Height of eye"), wxDefaultPosition, wxDefaultSize, 0);
    height_of_eye = new DynamicLengthField<SightFrame>(panel, &(sight->height_of_eye)/*, LengthUnit_types[1]*/);
    
    //this is how to properly bind the DynamicLengthField height_of_eye when it is inserted into a frame and I want a modification of the DynamicLengthField to trigger AllOk() in the frame. Given that I am including height_of_eye in a frame, I want that every time value or unit is changed, SightFrame::AllOk() is triggered : 1. I first bind OnEditValue and OnEditUnit to height_of_eye->value and height_of_eye->unit 2. every time height_of_eye is changed, OnEditValue and OnEditUnit will be called and set to true/false the value_ok and unit_ok variables 3. AllOk() will be called later, read the value_ok and unit_ok variables, and enable/disable button_reduce  accordingly
    height_of_eye->Bind(wxEVT_COMBOBOX, &SightFrame::AllOk<wxCommandEvent>, this);
    height_of_eye->Bind(wxEVT_KEY_UP, &SightFrame::AllOk<wxKeyEvent>, this);
    height_of_eye->value->Bind(wxEVT_COMBOBOX, &DynamicLengthField<SightFrame>::OnEditValue<wxCommandEvent>, height_of_eye);
    height_of_eye->value->Bind(wxEVT_KEY_UP, &DynamicLengthField<SightFrame>::OnEditValue<wxKeyEvent>, height_of_eye);
    height_of_eye->unit->Bind(wxEVT_COMBOBOX, &DynamicLengthField<SightFrame>::OnEditUnit<wxCommandEvent>, height_of_eye);
    height_of_eye->unit->Bind(wxEVT_KEY_UP, &DynamicLengthField<SightFrame>::OnEditUnit<wxKeyEvent>, height_of_eye);

    
    
    if (sight_in == NULL) {
        //given that the height of eye may be often the same, I write a default value in sight->height_of_eye and fill in the height of eye DynamicLengthField with this value, so the user won't have to enter the same value all the time
        sight->height_of_eye.read_from_file_to(String("default height of eye"), (wxGetApp().path_file_init), String("R"), String(""));
        height_of_eye->set();

    }
    //now that height_of_eye has been allocatd, I link artificial_horizon_check to height_of_eye
    (artificial_horizon_check->related_field) = height_of_eye;

    //master-clock date
    //sets  sight.master_clock_date_and_hour.date and sight.time.date to the current UTC date if this constructor has been called with sight_in = NULL
    if (sight_in == NULL) {
        (sight->master_clock_date_and_hour).date.set_current();
        (sight->master_clock_date_and_hour).chrono.set_current();
        (sight->time).date.set_current();
        (sight->time).chrono.set_current();
    }
    StaticText* text_date = new StaticText(panel, wxT("Master-clock UTC date and hour of sight"), wxDefaultPosition, wxDefaultSize, 0);
    master_clock_date = new DateField<SightFrame>(panel, &(sight->master_clock_date_and_hour.date));
    master_clock_date->set((sight->master_clock_date_and_hour).date);
    //    (master_clock_date->year)->SetFont((wxGetApp().error_font));
    //I bind master_clock_date->year/month/day to OnEditTime in such a way that, if the user enters a master_clock_date such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (master_clock_date->year)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (master_clock_date->month)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);


    //master-clock chrono
    StaticText* text_space_1 = new StaticText(panel, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0);
    master_clock_chrono = new ChronoField<SightFrame>(panel, &(sight->master_clock_date_and_hour.chrono));
    //I bind master_clock_chrono->hour/minute/second to OnEditTime in such a way that, if the user enters a master_clock_chrono such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (master_clock_chrono->hour)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (master_clock_chrono->minute)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (master_clock_chrono->second)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);

    //I initialize the GUI filed master_clock_chrono with the one written in sight_in.
    //    if(sight_in != NULL){
    master_clock_chrono->set(sight->master_clock_date_and_hour.chrono);
    //    }


    //check/uncheck stopwatch
    StaticText* text_stopwatch_check = new StaticText(panel, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0);
    stopwatch_check = new CheckField<SightFrame, ChronoField<SightFrame> >(panel, &(sight->use_stopwatch), NULL, true);
    //I bind stopwatch_check to OnEditTime in such a way that, if the user enters a stopwatch_check such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (stopwatch_check->checkbox)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //if stopwatch_check is checked/unchecked, then I runm OnEditTime to verify that the time of sight lies within the ephemerides' time span
    //    (stopwatch_check->checkbox)->Bind(wxEVT_CHECKBOX, &SightFrame::OnEditTime<wxCommandEvent>, this);


    //stopwatch reading
    StaticText* text_stopwatch_reading = new StaticText(panel, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0);
    //    stopwatch_reading = new ChronoField(this, &(sight.stopwatch));
    stopwatch_reading = new ChronoField<SightFrame>(panel, &(sight->stopwatch));
    //now that stopwatch_reading has been allocatd, I link stopwatch_check to stopwatch_reading
    (stopwatch_check->related_field) = stopwatch_reading;
    //I bind stopwatch_reading->hour/minute/second to OnEditTime in such a way that, if the user enters a stopwatch_reading such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (stopwatch_reading->hour)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (stopwatch_reading->minute)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (stopwatch_reading->second)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);


    //initialize stopwatch_check and stopwatch_reading
    (stopwatch_check->checkbox)->SetValue(false);
    stopwatch_reading->Enable(false);

    StaticText* text_TAI_minus_UTC = new StaticText(panel, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0);
    TAI_minus_UTC = new ChronoField<SightFrame>(panel, &(sight->TAI_minus_UTC));
    TAI_minus_UTC->set(sight->TAI_minus_UTC);

    //message and image shown if the time entered by the user is not covered by ephemerides' data. Both are set to empty at the construction of SightFrame
//    text_time_interval_not_ok = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
//    image_time_interval_status = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize);

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<SightFrame>(panel, &(sight->label));


    button_cancel->Bind(wxEVT_BUTTON, &SightFrame::OnPressCancel, this);

    //I bind reduce button to label->set_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_reduce->Bind(wxEVT_BUTTON, &SightFrame::OnPressReduce, this);
    button_reduce->Bind(wxEVT_BUTTON, label->set_to_current_time);
    button_reduce->Bind(wxEVT_BUTTON, &SightFrame::update_recent_items<wxCommandEvent>, this);

    //If I press reduce, I want all the fields in this SightFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_reduce->Bind(wxEVT_BUTTON, &BodyField<SightFrame>::get<wxCommandEvent>, body);
    button_reduce->Bind(wxEVT_BUTTON, &LimbField<SightFrame>::get<wxCommandEvent>, limb);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, H_s);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, index_error);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<SightFrame, DynamicLengthField<SightFrame> >::get<wxCommandEvent>, artificial_horizon_check);
    button_reduce->Bind(wxEVT_BUTTON, &DynamicLengthField<SightFrame>::get<wxCommandEvent>, height_of_eye);
    button_reduce->Bind(wxEVT_BUTTON, &DateField<SightFrame>::get<wxCommandEvent>, master_clock_date);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField<SightFrame>::get<wxCommandEvent>, master_clock_chrono);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<SightFrame, ChronoField<SightFrame> >::get<wxCommandEvent>, stopwatch_check);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField<SightFrame>::get<wxCommandEvent>, stopwatch_reading);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField<SightFrame>::get<wxCommandEvent>, TAI_minus_UTC);
    button_reduce->Bind(wxEVT_BUTTON, &StringField<SightFrame>::get<wxCommandEvent>, label);

    //bind the function SightFrame::KeyDown to the event where a keyboard dey is pressed down in panel, body, ... and all fields
    panel->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    body->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    limb->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    H_s->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    index_error->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    artificial_horizon_check->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    stopwatch_check->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    height_of_eye->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    master_clock_date->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    master_clock_chrono->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    stopwatch_reading->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    TAI_minus_UTC->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);


    //I enable the reduce button only if sight_in is a valid sight with the entries propely filled, i.e., only if sight_in != NULL
    button_reduce->Enable((sight_in != NULL));

    sizer_grid_measurement->Add(text_combo_body);
    body->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_limb);
    limb->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_H_s);
    H_s->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_index_error);
    index_error->InsertIn<wxBoxSizer>(box_sizer_3);
    sizer_grid_measurement->Add(box_sizer_3);

    sizer_grid_measurement->Add(text_artificial_horizon_check);
    artificial_horizon_check->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_height_of_eye);
    height_of_eye->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_time->Add(text_date);
    master_clock_date->InsertIn<wxBoxSizer>(box_sizer_4);
    box_sizer_4->Add(text_space_1);
    master_clock_chrono->InsertIn<wxBoxSizer>(box_sizer_4);
    sizer_grid_time->Add(box_sizer_4);

    sizer_grid_time->Add(text_stopwatch_check);
    stopwatch_check->InsertIn<wxFlexGridSizer>(sizer_grid_time);

    sizer_grid_time->Add(text_stopwatch_reading);
    stopwatch_reading->InsertIn<wxFlexGridSizer>(sizer_grid_time);

    sizer_grid_time->Add(text_TAI_minus_UTC);
    TAI_minus_UTC->InsertIn<wxFlexGridSizer>(sizer_grid_time);

//    sizer_grid_time->Add(text_time_interval_not_ok);
//    sizer_grid_time->Add(image_time_interval_status);

    sizer_grid_label->Add(text_label);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL);
    box_sizer_2->Add(button_reduce, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL);

    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Measurement");
    sizer_box_time = new wxStaticBoxSizer(wxVERTICAL, panel, "Time");

    sizer_box_measurement->Add(sizer_grid_measurement);
    sizer_box_time->Add(sizer_grid_time);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the StaticText containing "Master-clock UTC date and hour of sight"
    common_width = GetTextExtent(wxS("Master-clock UTC date and hour of sight   ")).GetWidth();
    text_combo_body->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_date->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_box_time, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    panel->Fit();
    Fit();


    //Maximize();


    if (!check) {
        cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
    }
    if (sight_in != NULL) {
        set();
    }

    //runs checl to write into the `ok` boolean variable of each field, and then AllOk to enable/disable button_reduce according to these `ok` variables
    Check();
    AllOk();

    Centre();

}


//if a key is pressed in the keyboard, I call this function
void SightFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        // the use pressed escape -> I do as if the user pressed button_cancel

        wxCommandEvent dummy;

        OnPressCancel(dummy);

    }
    else {

        if (((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
            //the user pressed return or numpad return

            /*
             //example of a lambda!
            CallAfter(
                      [  ] (  ) -> void
                      {
                          cout << "Lambda ! \n";
                      }
                      );
             */

            if (is_ok()) {
                //if all fields are ok, I do as if the user presssed button_reduce

                wxCommandEvent dummy;

                OnPressReduce(dummy);

            }

        }

    }

    event.Skip(true);

}

//run check on all the GUI fields that are members of SightFrame
template<class E> void SightFrame::Check(E& event) {

    (*(body->check))(event);
    if (((body->name->GetValue()) == wxString("sun")) || ((body->name->GetValue()) == wxString("moon"))) {
        //I check limb only if the body has a limb -> only if the body is either sun or moon
        (*(limb->check))(event);
    }
    (*(artificial_horizon_check->check))(event);
    (*(stopwatch_check->check))(event);
    (*(H_s->check))(event);
    (*(index_error->check))(event);
    if (!((artificial_horizon_check->checkbox)->GetValue())) {
        //I check height_of_eye only if the artificial horizon checkbox has been checked
        (*(height_of_eye->check))(event);
    }
    (*(master_clock_date->check))(event);
    (*(master_clock_chrono->check))(event);
    if ((stopwatch_check->checkbox)->GetValue()) {
        //I check stopwatch_reading only if the stopwatch checkbox has been checked
        (*(stopwatch_reading->check))(event);
    }
    (*(TAI_minus_UTC->check))(event);
    (*(label->check))(event);

    event.Skip(true);

}


//same as void SightFrame::Check(E& event)  but without event as an argument
void SightFrame::Check(void){
    
    wxCommandEvent dummy;
    
    Check(dummy);
    
    
}

    
template<class E> void SightFrame::update_recent_items(E& event) {

    unsigned int i;
    bool check;

    //I check whether the name in the GUI field body matches one of the body names in catalog, and store its id in i
    for (check = false, i = 0; (i < (body->catalog->list).size()) && (!check); i++) {
        if (String((body->name->GetValue().ToStdString())) == (((body->catalog->list)[i]).name)) {
            check = true;
        }
    }
    i--;

    //insert body #i into data->recent_bodies
//    wxGetApp().list_frame->data->insert_recent_body(i);
    wxGetApp().list_frame->data->insert_recent_item(i, &(wxGetApp().list_frame->data->recent_bodies));

    
    
    event.Skip(true);

}



//write into all the non-GUI objects the values of the GUI fields
template<class T> void SightFrame::get(T& event) {

    body->get(event);
    limb->get(event);
    artificial_horizon_check->get(event);
    H_s->get(event);
    index_error->get(event);

    if (!((artificial_horizon_check->checkbox)->GetValue())) {
        height_of_eye->get(event);
    }

    master_clock_date->get(event);
    master_clock_chrono->get(event);

    //while setting the non-GUI fields equal to the values in the GUI fields, I set the value of sight->time: I firs set time to master_clock_date_and_hour ...
    (sight->time) = (sight->master_clock_date_and_hour);

    stopwatch_check->get(event);

    //.. then I add to it sight->stopwatch, if any ....
    if ((sight->use_stopwatch) == Answer('y', String(""))) {
        (sight->time) += (sight->stopwatch);
    }


    if (((stopwatch_check->checkbox)->GetValue())) {
        stopwatch_reading->get(event);
    }

    TAI_minus_UTC->get(event);

    //... then I add to it TAI_minus_UTC, to convert it from the UTC to the TAI scale.
    (sight->time) += (sight->TAI_minus_UTC);


    label->get(event);

    event.Skip(true);

}

//set all the GUI fields in this equal to those in the non-GUI object this->sight
void SightFrame::set(void) {

    //    Time temp;

    body->set();

    if(((body->name)->GetValue() == wxString("sun") || (body->name)->GetValue() == wxString("moon"))) {
        //if  body is sun or moon, then I write the value in the non-GUI field Limb into the GUI LimbField

        limb->Enable(true);
        limb->set();

    }else {
        //if  body is not sun or moon, then I set the limb GUI field to empty and disable it

        long i;

        for (i = 0; i < 3; i++) { limb->name->Check(((unsigned int)i), false); }
        limb->checked_items.Clear();

        limb->Enable(false);
        
    }


    artificial_horizon_check->set();
    H_s->set();
    index_error->set();

    if(!(artificial_horizon_check->checkbox->GetValue())) {
        
        height_of_eye->Enable(true);
        height_of_eye->set();
        
    }else{
        
        height_of_eye->Enable(false);
        
    }

    //(sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    //    temp = (sight->master_clock_date_and_hour);
    //    temp += (sight->TAI_minus_UTC);

    master_clock_date->set(sight->master_clock_date_and_hour.date);
    master_clock_chrono->set(sight->master_clock_date_and_hour.chrono);

    stopwatch_check->set();

    if((stopwatch_check->checkbox->GetValue())){
        
        stopwatch_reading->Enable(true);
        stopwatch_reading->set(sight->stopwatch);
        
    }else{
        
        stopwatch_reading->Enable(false);
        
    }

    TAI_minus_UTC->set(sight->TAI_minus_UTC);
    label->set();

}




//write the content in the GUI fields into the non=GUI fields, and check whether all the fields in SightFrame are ok and whether the time of sight lies within the ephemerides' time span:
bool SightFrame::is_ok(void) {

    bool time_interval_ok_before;
    
    wxCommandEvent dummy;

    get(dummy);

    //becore time_inverval_ok is recomputed, store its value into time_interval_ok_before
    time_interval_ok_before = time_interval_ok;

    
    //runs TimeIntervalOk to compute time_interval_ok, which will be used to determine whether button_reduce is enabled or not
    TimeIntervalOk(String(""));
    
    if((!time_interval_ok) && time_interval_ok_before){
        //the time interval lies outside the interval of ephemerides' data and it was ok before -> prompt an error message
        
        print_error_message->SetAndCall(NULL, String("Error"), String("Time not included in ephemerides' data!"), (wxGetApp().path_file_error_icon));

    }
    
    return(
        (body->is_ok()) &&
        ((!(((body->name->GetValue()) == wxString("sun")) || ((body->name->GetValue()) == wxString("moon")))) || (limb->is_ok())) &&
        (H_s->is_ok()) &&
        (index_error->is_ok()) &&
        ((((artificial_horizon_check->checkbox)->GetValue())) || (height_of_eye->is_ok())) &&
        (master_clock_date->is_ok()) &&
        (master_clock_chrono->is_ok()) &&
        ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) &&
        (TAI_minus_UTC->is_ok()) && time_interval_ok);


}

//calls CheckAllOk: if it returns true/false, it enables/disables the button_reduce
void SightFrame::AllOk(void) {

    button_reduce->Enable(is_ok());

}


// same as SightFrame::AllOk(void)  but with an event as an argument, so this method can be triggered from an event
template<class E> void SightFrame::AllOk([[maybe_unused]] E& event) {

    AllOk();

}

//compute time_interval_ok
void SightFrame::TimeIntervalOk([[maybe_unused]] String prefix) {

    if (
        (master_clock_date->is_ok()) &&
        (master_clock_chrono->is_ok()) &&
        ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) &&
        (TAI_minus_UTC->is_ok())
        )
    {
        //the fields that specify the time of the sight are all ok ...

        //        wxCommandEvent dummy;
        //
        //        master_clock_date->get(dummy);
        //        master_clock_chrono->get(dummy);
        //        stopwatch_check->get(dummy);
        //        if(((stopwatch_check->checkbox)->GetValue())){
        //            stopwatch_reading->get(dummy);
        //        }
        //        TAI_minus_UTC->get(dummy);

        //... compute if sight->time lies within the ephemerids' data time interval
        time_interval_ok = (sight->check_time_interval(String("")));


    }
    else {

        time_interval_ok = false;

    }


}


void SightFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to true
    (*(parent->unset_idling))();

    Close(TRUE);

}



void SightFrame::OnPressReduce(wxCommandEvent& event) {

    stringstream s;

    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }


    //writes the values of the GUI fields in the non-GUI fields
    get(event);

    //    sight->print(String("sight entered via GUI"), String(""), cout);


    if (position_in_listcontrol_sights == -1) {
        //if the constructor of SightFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of sight_list and reduce it

        parent->data->add_sight_and_reduce(sight, String(""));

        //add the sight and the related route to the GUI object listconstrol_sights and listcontrol_routes, respectively
        //        sight->add_to_wxListCtrl(position_in_listcontrol_sights, ((this->parent)->listcontrol_sights));
        //        (((this->parent->data)->route_list)[(sight->related_route).value]).add_to_wxListCtrl(position_in_listcontrol_sights, ((this->parent)->listcontrol_routes));

    }
    else {
        //if the constructor of SightFrame has been called with sight_in != NULL, then I am modifying an existing sight, and I reduce it and write the result in the related route, which already exists


        if ((sight->related_route.value) != -1) {
            //sight has a related Route -> reduce sight and write the resulting Route into such related Route

            sight->reduce(&((parent->data->route_list)[(sight->related_route).value]), String(""));

        }
        else {
            //sight has no related Route -> create a related Route and add it to listcontrol_routes

            ptrdiff_t sight_position;

            sight_position = sight - (&((parent->data->sight_list)[0]));

            (parent->data->route_list).resize((parent->data->route_list).size() + 1);
            sight->reduce(&((parent->data->route_list)[(parent->data->route_list).size() - 1]), String(""));

            //I link the Sight to the Route, and the Route to the Sight
            (sight->related_route.value) = ((int)(parent->data->route_list).size()) - 1;
            ((((parent->data->route_list)[(parent->data->route_list).size() - 1]).related_sight).value) = ((int)sight_position);


        }

    }

    parent->listcontrol_sights->set(parent->data->sight_list, false);
    //I call listcontrol_routes->set with true because I want to keep the selection in listcontrol_routes
    parent->listcontrol_routes->set(parent->data->route_list, true);

    //given that I have reset the content of listcontrol_sights and listcontrol_routes, now no items will be selected in these ListControls -> I call:
    (*(parent->on_change_selection_in_listcontrol_sights))(event);
    (*(parent->on_change_selection_in_listcontrol_routes))(event);


    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    
//    parent->PreRenderAll();
    //animate the charts to bring them to the Route related to the newly reduced Sight
    parent->AnimateToObject<Route, UnsetIdling<ListFrame> >(&((parent->data->route_list)[(sight->related_route).value]), parent->unset_idling);
    
    event.Skip(true);

    Close(TRUE);

}

