//
//  lib_h.h
//  
//
//  Created by MacBook Pro on 16/02/2022.
//
class AngleField;
class MyApp;
class MyFrame;

//class for graphical object: a field to enter an angle, composed of a box for degrees, a degree symbol, another box for minutes and a minute symbol
class AngleField{
    
    
    public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    //degrees and minutes boxes
    wxComboBox* deg;
    wxTextCtrl *min;
    //texts
    wxStaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    
    AngleField(MyFrame*);
    template<class T> void InsertIn(T*);
    
    
};


class MyApp: public wxApp{
public:
    virtual bool OnInit();
};

class MyFrame: public wxFrame{
    
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, String prefix);
    
    Catalog* catalog;
    Sight sight;
    wxPanel *panel;
    
    AngleField* H_s, *index_error;
    
    wxGridSizer *grid_sizer;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4, *box_sizer_5, *box_sizer_6;
    
    wxArrayString bodies, limbs, signs, months, days, hours, minutes;
    wxTextCtrl *box_year, *box_second_masterclock, *box_second_stopwatch, *box_second_TAI_minus_UTC;
    wxCheckBox *artificial_horizon, *stopwatch;
    wxComboBox* combo_body, *combo_limb, *combo_sign_index_error, *combo_month, *combo_day, *combo_hour_masterclock, *combo_minute_masterclock, *combo_hour_stopwatch, *combo_minute_stopwatch, *combo_sign_TAI_minus_UTC, *combo_hour_TAI_minus_UTC, *combo_minute_TAI_minus_UTC;
    wxButton* button_ok, *button_cancel;
    wxMenuBar *menuBar;
    
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void OnCheckArtificialHorizon(wxCommandEvent& event);
    void OnSelectBody(wxFocusEvent& event);
    void TabulateDays(wxFocusEvent& event);
    void OnCheckStopwatch(wxCommandEvent& event);
    void PrintErrorMessage(wxControl*, String);
    void CheckDegrees(wxFocusEvent& event);
    void CheckMinutes(wxFocusEvent& event);

    
    
    // The Path to the file we have open
    wxString CurrentDocPath;
    
    wxDECLARE_EVENT_TABLE();
    
};

enum{
    
    ID_Open =  wxID_HIGHEST + 1,
    ID_Save =  wxID_HIGHEST + 2,
    ID_SaveAs =  wxID_HIGHEST + 3,
    ID_Close =  wxID_HIGHEST + 5,
    ID_button_ok =  wxID_HIGHEST + 6,
    ID_button_cancel =  wxID_HIGHEST + 7,
    ID_artificial_horizon =  wxID_HIGHEST + 8,
    ID_combo_body = wxID_HIGHEST + 9,
    ID_combo_limb = wxID_HIGHEST + 10,
    ID_combo_sign_index_error = wxID_HIGHEST + 11,
    ID_box_year = wxID_HIGHEST + 12,
    ID_combo_month = wxID_HIGHEST + 13,
    ID_combo_day = wxID_HIGHEST + 14,
    ID_stopwatch = wxID_HIGHEST + 15,
    ID_combo_hour_masterclock = wxID_HIGHEST + 16,
    ID_combo_minute_masterclock = wxID_HIGHEST + 17,
    ID_combo_hour_stopwatch = wxID_HIGHEST + 18,
    ID_combo_minute_stopwatch = wxID_HIGHEST + 19,
    ID_combo_sign_TAI_minus_UTC = wxID_HIGHEST + 20,
    ID_combo_hour_TAI_minus_UTC = wxID_HIGHEST + 21,
    ID_combo_minute_TAI_minus_UTC = wxID_HIGHEST + 22

};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Open,   MyFrame::OnOpen)
EVT_MENU(ID_Save,   MyFrame::OnSave)
EVT_MENU(ID_SaveAs,   MyFrame::OnSaveAs)
EVT_MENU(ID_Close,  MyFrame::OnClose)
EVT_BUTTON(ID_artificial_horizon,   MyFrame::OnCheckArtificialHorizon)
EVT_BUTTON(ID_button_cancel,   MyFrame::OnPressCancel)
EVT_BUTTON(ID_button_ok,   MyFrame::OnPressReduce)
//EVT_COMBOBOX(ID_combo_body, MyFrame::OnSelectBody)
EVT_CHECKBOX(ID_stopwatch, MyFrame::OnCheckStopwatch)
//EVT_COMBOBOX(ID_combo_month, MyFrame::TabulateDays)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    unsigned int i;
    
    //obtain width and height of the display, and create a wxRect with height and width half ot the height and width of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*0.75);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*0.75);
    
    for(degrees.Clear(), i=0; i<360; i++){
        degrees.Add(wxString::Format(wxT("%i"), i));
    }

    
    MyFrame *frame = new MyFrame( "Sight", wxDefaultPosition, rectangle.GetSize(), String(""));
    
    frame->Show( true );
    
    return true;
    
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(NULL, wxID_ANY, title, pos, size){
    
    //pointer to init.txt to read fixed sight data from in there
    File file_init;
    String new_prefix;
    unsigned int i, deg;
    double min;
    bool check = true;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    file_init.set_name(String(path_file_init));
    check &= (file_init.open(String("in"), prefix));
    
    wxMenu *menuFile = new wxMenu;
    catalog = new Catalog(String(path_file_catalog), String(""));
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    H_s = new AngleField(this);
    
    for(months.Clear(), months.Add(wxT("")), i=0; i<12; i++){
        months.Add(wxString::Format(wxT("%i"),i+1));
    }
    for(i=0; i<24; i++){
        hours.Add(wxString::Format(wxT("%i"),i));
    }
    for(i=0; i<60; i++){
        minutes.Add(wxString::Format(wxT("%i"),i));
    }
    
    
    menuFile->Append(ID_Open, "&Open...\tCtrl-O", "This is to open a file");
    //this adds a separator, a horizontal line in the menu
    menuFile->AppendSeparator();
    menuFile->Append(ID_SaveAs, "&Save as...\tCtrl-Shift-S", "This is to save as");
    menuFile->Append(ID_Save, "&Save...\tCtrl-S", "This is to save");
    menuFile->Append(ID_Close, "&Close...\tCtrl-W", "This is to close the document");
    
    menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    SetMenuBar( menuBar );
    
    
    grid_sizer = new wxGridSizer(9, 2, 0, 0);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_5 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_6 = new wxBoxSizer(wxHORIZONTAL);
    sizer = new wxBoxSizer(wxVERTICAL);
    
    for(i=0; i<(*catalog).list.size(); i++){
        bodies.Add((((*catalog).list)[i]).name.value.c_str());
    }
    wxStaticText* text_combo_body = new wxStaticText(panel, wxID_ANY, wxT("Celestial body"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    combo_body = new wxComboBox(panel, ID_combo_body, wxT(""), wxDefaultPosition, wxDefaultSize, bodies, wxCB_DROPDOWN);
    combo_body->Bind(wxEVT_KILL_FOCUS, wxFocusEventHandler(MyFrame::OnSelectBody), this);
    //combo_body->SetValue("");
    
    limbs.Add(wxT("upper"));
    limbs.Add(wxT("lower"));
    limbs.Add(wxT("center"));
    wxStaticText* text_combo_limb = new wxStaticText(panel, wxID_ANY, wxT("Limb"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    combo_limb = new wxComboBox(panel, ID_combo_limb, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    //combo_limb->SetValue("");
    combo_limb->Enable(false);
    
    wxStaticText* text_H_s = new wxStaticText(panel, wxID_ANY, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));

    
    signs.Add(wxT("+"));
    signs.Add(wxT("-"));
    combo_sign_index_error = new wxComboBox(panel, ID_combo_sign_index_error, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
    //combo_sign_index_error->SetValue("");
    
    //index error
    wxStaticText* text_index_error = new wxStaticText(panel, wxID_ANY, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //read index error from init file
    cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
    sight.index_error.read_from_file(String("index error"), file_init, true, new_prefix);
    sight.index_error.to_deg_min(&deg, &min);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    index_error = new AngleField(this);
    index_error->deg->SetValue(wxString::Format(wxT("%i"), deg));
    index_error->min->SetValue(wxString::Format(wxT("%f"), min));
    
    
    //artificial horizon
    wxStaticText* text_artificial_horizon = new wxStaticText(panel, wxID_ANY, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    artificial_horizon = new wxCheckBox(panel, ID_artificial_horizon, wxT(""), wxDefaultPosition, wxDefaultSize);
    
    
    //master-clock date
    sight.master_clock_date_and_hour.date.set_current(prefix);
    wxStaticText* text_date = new wxStaticText(panel, wxID_ANY, wxT("Master-clock UTC date and hour of sight"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    box_year = new wxTextCtrl(panel, ID_box_year, "", wxDefaultPosition, wxDefaultSize);
    box_year->SetInitialSize(box_year->GetSizeFromTextSize(box_year->GetTextExtent(wxS("0000"))));
    box_year->Bind(wxEVT_KILL_FOCUS, &MyFrame::TabulateDays, this);
    box_year->SetValue(wxString::Format(wxT("%i"),sight.master_clock_date_and_hour.date.Y));
    
    wxStaticText* text_hyphen_1 = new wxStaticText(panel, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    combo_month = new wxComboBox(panel, ID_combo_month, wxT(""), wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN);
    combo_month->Bind(wxEVT_KILL_FOCUS, wxFocusEventHandler(MyFrame::TabulateDays), this);
    combo_month->SetValue(wxString::Format(wxT("%i"),sight.master_clock_date_and_hour.date.M));
    
    wxStaticText* text_hyphen_2 = new wxStaticText(panel, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    for(days.Clear(), days.Add(wxT("")), i=0; i<days_per_month_common[1-1]; i++){
        days.Add(wxString::Format(wxT("%i"),i+1));
    }
    combo_day = new wxComboBox(panel, ID_combo_day, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN);
    combo_day->SetValue(wxString::Format(wxT("%i"),sight.master_clock_date_and_hour.date.D));
    //combo_day->Enable(false);
    
    
    //master-clock hour
    sight.master_clock_date_and_hour.chrono.set_current(prefix);
    wxStaticText* text_space_1 = new wxStaticText(panel, wxID_ANY, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    combo_hour_masterclock = new wxComboBox(panel, ID_combo_hour_masterclock, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN);
    combo_hour_masterclock->SetValue(wxString::Format(wxT("%i"),sight.master_clock_date_and_hour.chrono.h));
    
    wxStaticText* text_colon_1 = new wxStaticText(panel, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    combo_minute_masterclock = new wxComboBox(panel, ID_combo_minute_masterclock, wxT(""), wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN);
    combo_minute_masterclock->SetValue(wxString::Format(wxT("%i"),sight.master_clock_date_and_hour.chrono.m));
    
    wxStaticText* text_colon_2 = new wxStaticText(panel, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    box_second_masterclock = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    box_second_masterclock->SetInitialSize(box_second_masterclock->GetSizeFromTextSize(box_second_masterclock->GetTextExtent(wxS("0.0000"))));
    box_second_masterclock->SetValue(wxString::Format(wxT("%f"),sight.master_clock_date_and_hour.chrono.s));
    
    
    //use of stopwatch
    wxStaticText* text_stopwatch = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    stopwatch = new wxCheckBox(panel, ID_stopwatch, wxT(""), wxDefaultPosition, wxDefaultSize);
    
    //stopwatch reading
    wxStaticText* text_stopwatch_reading = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    combo_hour_stopwatch = new wxComboBox(panel, ID_combo_hour_stopwatch, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN);
    combo_hour_stopwatch->Enable(false);
//    combo_hour_stopwatch->SetValue("");
    
    wxStaticText* text_colon_3 = new wxStaticText(panel, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    combo_minute_stopwatch = new wxComboBox(panel, ID_combo_minute_stopwatch, wxT(""), wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN);
    combo_minute_stopwatch->Enable(false);
//    combo_minute_stopwatch->SetValue("");
    
    wxStaticText* text_colon_4 = new wxStaticText(panel, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    box_second_stopwatch = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    box_second_stopwatch->SetInitialSize(box_second_stopwatch->GetSizeFromTextSize(box_second_stopwatch->GetTextExtent(wxS("0.0000"))));
    box_second_stopwatch->Enable(false);
    
    //TAI-UTC
    //read TAI_minus_UTC from /Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/index.txt
    cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
    sight.TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    wxStaticText* text_TAI_minus_UTC = new wxStaticText(panel, wxID_ANY, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));

    combo_sign_TAI_minus_UTC = new wxComboBox(panel, ID_combo_sign_TAI_minus_UTC, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
    combo_sign_TAI_minus_UTC->SetValue(wxT("+"));
    
    combo_hour_TAI_minus_UTC = new wxComboBox(panel, ID_combo_hour_TAI_minus_UTC, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN);
    combo_hour_TAI_minus_UTC->SetValue(wxString::Format(wxT("%i"),sight.TAI_minus_UTC.h));

    wxStaticText* text_colon_5 = new wxStaticText(panel, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));

    combo_minute_TAI_minus_UTC = new wxComboBox(panel, ID_combo_minute_TAI_minus_UTC, wxT(""), wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN);
    combo_minute_TAI_minus_UTC->SetValue(wxString::Format(wxT("%i"),sight.TAI_minus_UTC.m));

    wxStaticText* text_colon_6 = new wxStaticText(panel, wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    box_second_TAI_minus_UTC = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    box_second_TAI_minus_UTC->SetValue(wxString::Format(wxT("%f"),sight.TAI_minus_UTC.s));


    
    //buttons
    button_cancel = new wxButton(panel, ID_button_cancel, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_ok = new wxButton(panel, ID_button_ok, "Reduce", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    
    
    grid_sizer->Add(text_combo_body);
    grid_sizer->Add(combo_body);
    
    grid_sizer->Add(text_combo_limb);
    grid_sizer->Add(combo_limb);
    
    grid_sizer->Add(text_H_s);
    H_s->InsertIn<wxGridSizer>(grid_sizer);
    
    grid_sizer->Add(text_index_error);
    box_sizer_3->Add(combo_sign_index_error);
    index_error->InsertIn<wxBoxSizer>(box_sizer_3);
    grid_sizer->Add(box_sizer_3);

    grid_sizer->Add(text_artificial_horizon);
    grid_sizer->Add(artificial_horizon);
    
    grid_sizer->Add(text_date);
    box_sizer_4->Add(box_year);
    box_sizer_4->Add(text_hyphen_1);
    box_sizer_4->Add(combo_month);
    box_sizer_4->Add(text_hyphen_2);
    box_sizer_4->Add(combo_day);
    box_sizer_4->Add(text_space_1);
    box_sizer_4->Add(combo_hour_masterclock);
    box_sizer_4->Add(text_colon_1);
    box_sizer_4->Add(combo_minute_masterclock);
    box_sizer_4->Add(text_colon_2);
    box_sizer_4->Add(box_second_masterclock);
    
    grid_sizer->Add(box_sizer_4);
    
    grid_sizer->Add(text_stopwatch);
    grid_sizer->Add(stopwatch);
    
    grid_sizer->Add(text_stopwatch_reading);
    box_sizer_5->Add(combo_hour_stopwatch);
    box_sizer_5->Add(text_colon_3);
    box_sizer_5->Add(combo_minute_stopwatch);
    box_sizer_5->Add(text_colon_4);
    box_sizer_5->Add(box_second_stopwatch);
    
    grid_sizer->Add(box_sizer_5);
    
    grid_sizer->Add(text_TAI_minus_UTC);
    box_sizer_6->Add(combo_sign_TAI_minus_UTC);
    box_sizer_6->Add(combo_hour_TAI_minus_UTC);
    box_sizer_6->Add(text_colon_5);
    box_sizer_6->Add(combo_minute_TAI_minus_UTC);
    box_sizer_6->Add(text_colon_6);
    box_sizer_6->Add(box_second_TAI_minus_UTC);

    grid_sizer->Add(box_sizer_6);

    
    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer_2->Add(button_ok, 0, wxALIGN_BOTTOM);
    
    //here '0' means that the size of grid_sizer cannot be changed in the vertical direction, and wxEXPAND implies that grid_sizer is expanded horizontally
    sizer->Add(grid_sizer, 0, wxEXPAND);
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT);
    
    
    
    
    
    
    panel->SetSizer(sizer);
    //Maximize(panel);
    
    
    
    CurrentDocPath = wxT("");
    
    CreateStatusBar();
    SetStatusText( "Welcome to Michele's text editor!" );
    
    //SetSizerAndFit(panelSizer);
    
    //Maximize();
    
    file_init.close(prefix);
    
    if(!check){
        cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
    }
    
    
}

void MyFrame::OnClose(wxCommandEvent& event){
    
    CurrentDocPath = wxT("");
    SetTitle(_("untitled"));
    
}

void MyFrame::OnOpen(wxCommandEvent& event){
    
    wxFileDialog* OpenDialog = new wxFileDialog(this, _("Choose a file to open"), wxEmptyString, wxEmptyString,  _("Text files (*.txt)|*.txt|C++ Source Files (*.cpp, *.cxx)|*.cpp;*.cxx|C Source files (*.c)|*.c|C header files (*.h)|*.h"), wxFD_OPEN, wxDefaultPosition);
    
    
    
    // Creates a "open file" dialog with 4 file types
    if(/*OpenDialog->ShowModal() is the command which prompts the appearance of the file-selection window*/OpenDialog->ShowModal() == wxID_OK) {
        // if the user click "Open" instead of "cancel"
        
        CurrentDocPath = OpenDialog->GetPath();
        
        // Sets our current document to the file the user selected
        //        combo_H_s_deg->LoadFile(CurrentDocPath); //Opens that file
        // Set the Title to reflect the  file open
        SetTitle(wxString("Edit - ") << OpenDialog->GetFilename());
    }
    
    
}

void MyFrame::OnSaveAs(wxCommandEvent& event){
    
    
    wxFileDialog *SaveDialog = new wxFileDialog(this, _("Save File As _?"), wxEmptyString, wxEmptyString, _("Text files (*.txt)|*.txt|C++ Source Files (*.cpp)|*.cpp| C Source files (*.c)|*.c|C header files (*.h)|*.h"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
    
    // Creates a "save file" dialog with 4 file types
    if(SaveDialog->ShowModal() == wxID_OK) {
        
        CurrentDocPath = SaveDialog->GetPath();
        //        combo_H_s_deg->SaveFile(CurrentDocPath);
        
        SetTitle(SaveDialog->GetFilename());
        
    }
    
    SaveDialog->Destroy();
    
    
}


void MyFrame::OnSave(wxCommandEvent& event){
    
    if(CurrentDocPath == ""){
        OnSaveAs(event);
    }else{
        //        combo_H_s_deg->SaveFile(CurrentDocPath);
    }
    
}


void MyFrame::OnPressCancel(wxCommandEvent& event){
    
    
    Close(TRUE);
}


void MyFrame::OnCheckArtificialHorizon(wxCommandEvent& event){
    
    
    Close(TRUE);
}


void MyFrame::OnSelectBody(wxFocusEvent& event){
    
    unsigned int i;
    
    sight.body.name = String((combo_body->GetValue()).ToStdString());
    
    if(sight.body.check(&i, *catalog, String(""))){
        
        sight.body = ((*catalog).list)[i];
        
        if((sight.body.name == String("sun")) || (sight.body.name == String("moon"))){
            combo_limb->Enable(true);
        }else{
            combo_limb->Enable(false);
        }
        
        combo_body->SetBackgroundColour(*wxWHITE);
        
    }else{
        
        CallAfter(&MyFrame::PrintErrorMessage, combo_body, String("Body not found in catalog!\nBody must be in catalog."));
        
    }
    
    
    
    
}


void MyFrame::CheckDegrees(wxFocusEvent& event){

    wxComboBox* control;
    
    control = (wxComboBox*)(event.GetEventUserData());
    
    if(!check_unsigned_int((control->GetValue()).ToStdString(), NULL, true, 0, 360)){
        CallAfter(&MyFrame::PrintErrorMessage, control, String("Entered value is not valid!\nArcdegrees must be floating-point numbers >= 0° and < 360°"));

    }else{
        control->SetBackgroundColour(*wxWHITE);
    }


  
    event.Skip(true);
    
}


void MyFrame::CheckMinutes(wxFocusEvent& event){

    wxTextCtrl* control;
    
    control = (wxTextCtrl*)(event.GetEventUserData());
    
    if(!check_double((control->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
        CallAfter(&MyFrame::PrintErrorMessage, control, String("Entered value is not valid!\nArcminutes must be floating-point numbers >= 0' and < 60'"));

    }else{
        control->SetBackgroundColour(*wxWHITE);
    }


  
    event.Skip(true);
    
}


void MyFrame::PrintErrorMessage(wxControl* parent, String message){
    
    wxMessageBox(message.value);
    parent->SetBackgroundColour(*wxRED);
    parent->SetFocus();
    
}



void MyFrame::TabulateDays(wxFocusEvent& event){
    
    unsigned int i;
    
    if(((box_year->GetValue()) != wxT("")) && ((combo_month->GetValue()) != wxT(""))){
        
        //read the year
        sight.master_clock_date_and_hour.date.Y = ((unsigned int)wxAtoi(box_year->GetValue()));
        sight.master_clock_date_and_hour.date.check_leap_year();
        
        //read the month
        sight.master_clock_date_and_hour.date.M = ((unsigned int)wxAtoi(combo_month->GetValue()));
        
        if(sight.master_clock_date_and_hour.date.Y_is_leap_year){
            //in this case the year is a leap year: I fill the list of days from days_per_month_leap
            
            for(days.Clear(), i=0; i<days_per_month_leap[(sight.master_clock_date_and_hour.date.M)-1]; i++){
                days.Add(wxString::Format(wxT("%i"),i+1));
            }
            
        }else{
            //in this case the year is a common year: I fill the list of days from days_per_month_common
            
            for(days.Clear(), i=0; i<days_per_month_common[(sight.master_clock_date_and_hour.date.M)-1]; i++){
                days.Add(wxString::Format(wxT("%i"),i+1));
            }
            //
        }
        
        combo_day->Set(days);
        
        combo_day->Enable(true);
        
    }else{
        
        combo_day->Enable(false);
        
    }
    
    event.Skip(true);
    
}






void MyFrame::OnCheckStopwatch(wxCommandEvent& event){
    
    if(stopwatch->GetValue()){
        
        combo_hour_stopwatch->Enable(true);
        combo_minute_stopwatch->Enable(true);
        box_second_stopwatch->Enable(true);
        
    }else{
        
        combo_hour_stopwatch->Enable(false);
        combo_minute_stopwatch->Enable(false);
        box_second_stopwatch->Enable(false);
        
    }
    
}



void MyFrame::OnPressReduce(wxCommandEvent& event){
    
    //here I write the content of combo_H_s_deg into the string str
    wxString str_deg, str_min;
    double min;
    stringstream s;
    
    s << "Body : " << combo_body->GetValue() << "\n";
    
    if(((combo_body->GetValue()) == wxT("Sun")) || ((combo_body->GetValue()) == wxT("Moon"))){
        s << "Limb : " << (combo_limb->GetValue()) << "\n";
    }
    
    str_deg = H_s->deg->GetValue();
    str_min = H_s->min->GetValue();
    str_min.ToDouble(&min);
    
    s << "Sextant altitude = " << wxAtoi(str_deg) << "° " << min << "'\n";
    
    str_deg = index_error->deg->GetValue();
    str_min = index_error->min->GetValue();
    str_min.ToDouble(&min);
    
    s << "Index error = " << combo_sign_index_error->GetValue() << " " << wxAtoi(str_deg) << "° " << min << "'\n";
    
    s << "Artificial horizon = ";
    
    if((artificial_horizon->GetValue()) == wxCHK_CHECKED){
        s << "y";
    }else{
        s << "n";
    }
    
    
    wxMessageBox(s.str().c_str(), wxT("Here is the data which you entered:"));
    
    Close(TRUE);
    
}


//constructor of an AngleField object, based on the parent frame frame
AngleField::AngleField(MyFrame* frame){
    
    parent_frame = frame;
    
    deg = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    //deg->SetValue("");
    deg->Bind(wxEVT_KILL_FOCUS, &MyFrame::CheckDegrees, parent_frame, wxID_ANY, wxID_ANY, deg);

    text_deg = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("° "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    min = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS("0.0000"))));
    //min->SetValue("");
    min->Bind(wxEVT_KILL_FOCUS, &MyFrame::CheckMinutes, parent_frame, wxID_ANY, wxID_ANY, min);

    text_min = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("' "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(deg, 0, wxALIGN_CENTER);
    sizer_h->Add(text_deg);
    sizer_h->Add(min, 0, wxALIGN_CENTER);
    sizer_h->Add(text_min);
    

    
}

template<class T> void AngleField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

