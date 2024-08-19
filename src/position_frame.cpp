//
//  position_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "position_frame.h"

#include "generic.h"
#include "on_change_selection_in_list_control.h"
#include "string_field.h"


PositionFrame::PositionFrame(ListFrame* parent_input, Position* position_in, long position_in_listcontrol_positions_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    parent = parent_input;

    String new_prefix, /*the label which appears on button_ok*/label_button_ok;
    unsigned int common_width;


    bool check = true;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    //SetColor(this);

    set_idling = new SetIdling<PositionFrame>(this);
    unset_idling = new UnsetIdling<PositionFrame>(this);
    (*unset_idling)();

    print_error_message = new PrintMessage<PositionFrame, UnsetIdling<PositionFrame> >(this, unset_idling);

    //if this PositionFrame has been constructed with position_in = NULL, then I allocate a new Position object with the pointer this->position and set position_in_listcontrol_positions to a 'NULL' value (position_in_listcontrol_positions = -1). Otherwise, the pointer position_in points to a valid Position object -> I let this->position point to position_in, and set position_in_listcontrol_positions to position_in_listcontrol_positions_in.
    if (position_in != NULL) {
        position = position_in;
        position_in_listcontrol_positions = position_in_listcontrol_positions_in;
    }
    else {
        position = new Position();
        position_in_listcontrol_positions = -1;
    }

    //if I am adding a brand new position, I name button_ok 'Add'. Otherwise I name it "Modify"
    if (position_in == NULL) {
        label_button_ok.set(String("Add"));
    }
    else {
        label_button_ok.set(String("Modify"));
    }


    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value, wxDefaultPosition, wxDefaultSize);
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);

    sizer_grid_measurement = new wxFlexGridSizer(2, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_label = new wxFlexGridSizer(1, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);


    //latitude
    StaticText* text_lat = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    lat = new AngleField<PositionFrame>(
                                        panel,
                                        &(position->phi),
                                        NULL,
                                        String("NS")
                                        );

    //longitude
    StaticText* text_lon = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    lon = new AngleField<PositionFrame>(
                                        panel,
                                        &(position->lambda),
                                        NULL,
                                        String("EW")
                                        );

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<PositionFrame>(panel, &(position->label));


    //I bind  button_ok to label->set_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_ok->Bind(wxEVT_BUTTON, label->set_to_current_time);
    //If I press button_ok, I want all the fields in this PositionFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lat);
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lon);
    button_ok->Bind(wxEVT_BUTTON, &StringField<PositionFrame>::get<wxCommandEvent>, label);
    button_ok->Bind(wxEVT_BUTTON, &::PositionFrame::OnPressOk, this);
    button_cancel->Bind(wxEVT_BUTTON, &PositionFrame::OnPressCancel, this);


    panel->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    lat->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    lon->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);


    //I enable the reduce button only if position_in is a valid position with the entries propely filled, i.e., only if position_in != NULL
    button_ok->Enable((position_in != NULL));

    sizer_grid_measurement->Add(text_lat, 0, wxALIGN_CENTER_VERTICAL);
    lat->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_lon, 0, wxALIGN_CENTER_VERTICAL);
    lon->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL
    );
    box_sizer_2->Add(button_ok, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL
    );

    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Coordinates");

    sizer_box_measurement->Add(sizer_grid_measurement);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the StaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_lat->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_lon->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    //add the various elements to sizer, by inserting a border of (wxGetApp().rectangle_display.GetSize().GetWidth())*(length_border_over_length_screen.value) in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));


    //panel->SetSizer(sizer);

    //    CreateStatusBar();
    //    SetStatusText( "Welcome to UnsetIdling's text editor!" );

    if (!check) {
        cout << prefix.value << RED << "Cannot read position!\n" << RESET;
    }

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    //Maximize();
    panel->Fit();
    Fit();


    if (position_in != NULL) { set(); }

    Centre();

}


//set all the GUI fields in this equal to those in the non-GUI object this->position
void PositionFrame::set(void) {

    lat->set();
    lon->set();
    label->set();

}

//set all the non-GUI objects in this->position  equal to those in the GUI fields
template<class T> void PositionFrame::get(T& event) {

    lat->get(event);
    lon->get(event);
    label->get(event);

}


void PositionFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to false
    (*(parent->unset_idling))();

    Close(TRUE);

}


//this function is triggered when button_ok is pressed
void PositionFrame::OnPressOk(wxCommandEvent& event) {

    unsigned int i;
    stringstream s;

    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }

    //if I am adding a new Position, I resize points_position_list to add a new element to it
    if (position_in_listcontrol_positions == -1) {

        for (i = 0; i < (parent->chart_frames.size()); i++) {

            ((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).resize(((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).size() + 1);
            ((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).resize(((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).size() + 1);

        }
    }


    //writes the values of the GUI fields in the non-GUI fields
    get(event);

    position->print(String("position entered via GUI"), String(""), cout);

    //if the constructor of PositionFrame has been called with sight_in = NULL, then I push back the newly allocated Sight to the end of position_list
    if (position_in_listcontrol_positions == -1) {
        my_push_back(&(this->parent->data->position_list), *position);
    }

    (parent->listcontrol_positions)->set((parent->data)->position_list, false);

    //given that I have reset the content of listcontrol_positions, now no items are selected in this ListControl -> I call:
    (*(parent->on_change_selection_in_listcontrol_positions))(event);

    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    
//    parent->PreRenderAll();
    parent->AnimateToObject<Position, UnsetIdling<ListFrame> >(position, parent->unset_idling);
    

    event.Skip(true);

    Close(TRUE);

}


//write all the content in the GUI fields into the non-GUI objects, checks whether all the fields in PositionFrame are ok and if they are it returns true and false otherwise
bool PositionFrame::is_ok(void) {

    wxCommandEvent dummy;

    get(dummy);

    return((lat->is_ok()) && (lon->is_ok()));


}

//if all_ok() returns turue it enables  button_add, and it disables it otherwise
void PositionFrame::AllOk(void) {

    button_ok->Enable(is_ok());

}



//if a key is pressed in the keyboard, I call this function
void PositionFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        // the use pressed escape -> I do as if the user pressed button_cancel

        wxCommandEvent dummy;

        OnPressCancel(dummy);

    }
    else {

        if (((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
            //the user pressed return or numpad return

            if (is_ok()) {
                //if all fields are ok, I do as if the user presssed button_ok

                wxCommandEvent dummy;

                OnPressOk(dummy);

            }

        }

    }

    event.Skip(true);

}
