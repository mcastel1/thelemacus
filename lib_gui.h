//this string defines the width of GUI fields hosting floating-point numbers
#define sample_width_floating_point_field "0.000000000"

class BodyField;
class LimbField;
class CheckField;
class AngleField;
class LengthField;
class DateField;
class ChronoField;
class MyApp;
class MyFrame;
struct CheckArtificialHorizon;
struct CheckStopWatch;
struct CheckArcDegree;
struct CheckArcMinute;
struct CheckLength;
struct CheckYear;
struct CheckMonth;
struct CheckDay;
struct CheckHour;
struct CheckMinute;
struct CheckSecond;
struct TabulateDays;
struct PrintErrorMessage;


//this function adjusts the width of a wxComboBox according to its largest entry
void AdjustWidth(wxComboBox *control){
    
    unsigned int i;
    int max_width, width, additional;

    //this is the additional width occupied by the sqare with the arrow
    control->GetTextExtent(wxString("-----"), &additional, NULL);

    for(max_width=0, i=0; i<(control->GetCount()); i++){
        
        control->GetTextExtent(control->GetString(i), &width, NULL);
        
        if(width > max_width){
            max_width = width;
        }
        
    }
    
    control->SetMinSize(wxSize(max_width + additional, -1));
    
}


class CheckField{
    
public:
    
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    Answer* answer;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxCheckBox with the name of the bodies
    wxCheckBox* check;

    CheckField(MyFrame*, Answer*);
    
    template<class T> void InsertIn(T*);
    void set(void);
    
    
};


class BodyField{
    
public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    wxArrayString bodies;
    //this points to a Body object, which contains the date written in the GUI field of this
    Body* body;
    Catalog* catalog;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;

    bool ok;

    BodyField(MyFrame*, Body*, Catalog*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};


class LimbField{
    
public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    wxArrayString limbs;
    //this points to a Limn object, which contains the data written in the GUI field of this
    Limb* limb;
    wxBoxSizer *sizer_h, *sizer_v;
    bool ok;

    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;

    LimbField(MyFrame*, Limb*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};


//class for graphical object: a field to enter an angle, composed of a box for degrees, a degree symbol, another box for minutes and a minute symbol
class AngleField{
        
    public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    wxArrayString degrees;
    //degrees and minutes boxes
    wxComboBox* deg;
    wxTextCtrl *min;
    //texts
    wxStaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    Angle* angle;
    //deg_ok = true if the degrees part of this angle is formatted properly and set to the same value as the degree part of angle, and simiarly for min
    bool deg_ok, min_ok;
   
    
    AngleField(MyFrame*, Angle*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a length, composed of a box
class LengthField{
        
    public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    //degrees and minutes boxes
    wxTextCtrl *value;
    //texts
    wxStaticText* text;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool ok;
    
    LengthField(MyFrame*, Length*);
    void set(void);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};



class DateField{
    
    public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    wxArrayString days, months;
    //year, month and day boxes
    wxTextCtrl *year;
    wxComboBox *month, *day;
    //texts
    wxStaticText* text_hyphen_1, *text_hyphen_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Date* date;
    //year_ok = true if the year is formatted properly and set to the same value as date->Y, and similarly for the other variables
    bool year_ok, month_ok, day_ok;
    
    DateField(MyFrame*, Date*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};


class ChronoField{
    
    public:
    //the parent frame to which this object is attached
    MyFrame* parent_frame;
    wxArrayString hours, minutes;
    //hour and minute  boxes
    wxComboBox *hour, *minute;
    //second text control
    wxTextCtrl *second;
    //texts
    wxStaticText* text_colon_1, *text_colon_2;
    wxBoxSizer *sizer_h, *sizer_v;
    //this points to a Date object, which contains the date written in the GUI fields of this
    Chrono* chrono;
    //hour_ok = true if the hour is formatted properly and set to the same value as chrono->h, and similarly for the other variables
    bool hour_ok, minute_ok, second_ok;
    
    ChronoField(MyFrame*, Chrono*);
    void set(void);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};



struct CheckBody{
    
    BodyField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckLimb{
    
    LimbField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckArtificialHorizon{
    
    CheckField* p;
    
    void operator()(wxCommandEvent&);
    
};


struct CheckStopWatch{
    
    CheckField* p;
    
    void operator()(wxCommandEvent&);
    
    
};

struct CheckArcDegree{
    
    AngleField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckArcMinute{
    
    AngleField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckLength{
    
    LengthField* p;
    
    void operator()(wxFocusEvent&);
    
    
};


struct CheckYear{
    
    DateField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckMonth{
    
    DateField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckDay{
    
    DateField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckHour{
    
    ChronoField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckMinute{
    
    ChronoField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

struct CheckSecond{
    
    ChronoField* p;
    
    void operator()(wxFocusEvent&);
    
    
};



struct TabulateDays{
    
    DateField* p;
    
    void operator()(wxFocusEvent&);
    
    
};

//this functor pops out an error-message window with title tile and error message message, resulting from the wxControl control
struct PrintErrorMessage{
    
    MyFrame* f;
    wxControl* control;
    String title, message;
    
    void operator()(void);
    
    
};


class MyFrame: public wxFrame{
    
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, String prefix);
    
    Catalog* catalog;
    Sight* sight;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    CheckBody checkbody;
    CheckLimb checklimb;
    CheckArcDegree checkarcdegree;
    CheckArcMinute checkarcminute;
    CheckLength checklength;
    CheckArtificialHorizon checkartificialhorizon;
    CheckYear checkyear;
    CheckMonth checkmonth;
    CheckDay checkday;
    CheckHour checkhour;
    CheckMinute checkminute;
    CheckSecond checksecond;
    CheckStopWatch checkstopwatch;
    TabulateDays tabulatedays;
    PrintErrorMessage printerrormessage;
    
    BodyField* body;
    LimbField* limb;
    CheckField* artificial_horizon_check, *stopwatch_check;
    AngleField* H_s, *index_error;
    LengthField* height_of_eye;
    DateField *master_clock_date;
    ChronoField *master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    
    wxGridSizer *grid_sizer;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    
    wxArrayString bodies, limbs, signs;
    wxComboBox *combo_sign_index_error, *combo_sign_TAI_minus_UTC;
    wxButton* button_reduce, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void OnCheckArtificialHorizon(wxCommandEvent& event);
//    void OnSelectBody(wxFocusEvent& event);
//    void OnCheckStopwatch(wxCommandEvent& event);
//    void PrintErrorMessage(wxControl*, String);
    
    // The Path to the file we have open
    wxString CurrentDocPath;
    
    wxDECLARE_EVENT_TABLE();
    
};


void CheckBody::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        
        //(p->body)->name = String((combo_body->GetValue()).ToStdString());
        
        //I check whether the name in the GUI field body matches one of the body names in catalog
        for(check = false, i=0; (i<((p->catalog)->list).size()) && (!check); i++){
            if(String(((p->name)->GetValue().ToStdString())) == ((((p->catalog)->list)[i]).name)){
                check = true;
            }
        }
        i--;
        
        if(check){
            
            (*(p->body)) = ((p->catalog)->list)[i];
            
            if(((*(p->body)).name == String("sun")) || ((*(p->body)).name == String("moon"))){
                ((f->limb)->name)->Enable(true);
            }else{
                ((f->limb)->name)->Enable(false);
            }
            
            (p->name)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage).control) = (p->name);
            ((f->printerrormessage).title) = String("Body not found in catalog!");
            ((f->printerrormessage).message) = String("Body must be in catalog.");
            f->CallAfter((f->printerrormessage));
            
            (p->ok) = false;
            
        }
        
        
        
        (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
        
        event.Skip(true);
        
    }
    
}


void CheckLimb::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
  
    
    bool check;
    String s;

    
    s = String(((p->name)->GetValue().ToStdString()));
    //I check whether the name in the GUI field body matches one of the valid limb names
    check = (s == String("upper")) || (s == String("lower")) || (s == String("center"));
    
    if(check){
        
        //I set the char in ((p->limb)->value) to the first letter in the string (s.value)
        ((p->limb)->value) = (s.value)[0];
        
        (p->name)->SetBackgroundColour(*wxWHITE);
        (p->ok) = true;
        
    }else{
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->name);
        ((f->printerrormessage).title) = String("Limb not valid!");
        ((f->printerrormessage).message) = String("Limb must be upper, lower or center.");
        f->CallAfter((f->printerrormessage));
        
        (p->ok) = false;
        
    }

    (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
    
    event.Skip(true);
        
    }
    
}


void CheckArcDegree::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_unsigned_int(((p->deg)->GetValue()).ToStdString(), NULL, true, 0, 360)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, (p->deg), String("Entered value is not valid!\nArcdegrees must be unsigned integer numbers >= 0° and < 360°"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->deg);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Arcdegrees must be unsigned integer numbers >= 0° and < 360°");
        f->CallAfter((f->printerrormessage));
    
        (p->deg_ok) = false;
        
    }else{
        
        (p->deg)->SetBackgroundColour(*wxWHITE);
        
        if((p->min_ok)){
            
            double min_temp;
            
            ((p->min)->GetValue()).ToDouble(&min_temp);
            
            (p->angle)->from_deg_min(wxAtoi((p->deg)->GetValue()), min_temp);
            
        }
        (p->deg_ok) = true;
        
    }
    
    (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
    
    event.Skip(true);

    }
    
}

void CheckArcMinute::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_double(((p->min)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, p->min, String("Entered value is not valid!\nArcminutes must be floating-point numbers >= 0' and < 60'"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->min);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Arcminutes must be floating-point numbers >= 0' and < 60'");
        f->CallAfter((f->printerrormessage));

        (p->min_ok) = false;
        
    }else{
        (p->min)->SetBackgroundColour(*wxWHITE);
        if((p->deg_ok)){
            
            double min_temp;
            
            ((p->min)->GetValue()).ToDouble(&min_temp);
            
            (p->angle)->from_deg_min(wxAtoi((p->deg)->GetValue()), min_temp);
            
            
        }
        (p->min_ok) = true;
    }
    
    (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
 
    
    event.Skip(true);
        
    }
    
}

void CheckLength::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_double(((p->value)->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, p->value, String("Entered value is not valid!\nLengths must be floating-point numbers >= 0 m"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->value);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Lengths must be floating-point numbers >= 0 m");
        f->CallAfter((f->printerrormessage));
        
        (p->ok) = false;
        
    }else{
        
        double length_temp;
        
        (p->value)->SetBackgroundColour(*wxWHITE);
        
        (p->value)->GetValue().ToDouble(&length_temp);
        (p->length)->set(String("height of eye"), length_temp, String(""));
        
        (p->ok) = true;
        
    }
    
    (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
 
    event.Skip(true);
        
    }
    
}

void PrintErrorMessage::operator()(void){
    
    wxMessageDialog* dialog;
    
    //I may be about to prompt a temporary dialog window, thus I set f->idling to true
    f->SetIdling(true);
    
    if((control->GetBackgroundColour()) != *wxRED){
        
//        wxMessageBox(message.value, title.value);

        
//        wxIconLocation dlgIconLoc = wxIconLocation("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample.ico");
//        const wxIcon dlgIcon = wxIcon(dlgIconLoc);

        dialog = new wxMessageDialog(f, wxString(message.value), wxString(title.value));
//        dialog->SetIcon(dlgIcon);
        dialog->ShowModal();

        control->SetFocus();
        control->SetBackgroundColour(*wxRED);
     
    }
    
    //The temporary dialog window may have been closed, thus I set f->idling to false
    f->SetIdling(false);

}

class MyApp: public wxApp{
public:
    virtual bool OnInit();
};


enum{
    
    ID_Open =  wxID_HIGHEST + 1,
    ID_Save =  wxID_HIGHEST + 2,
    ID_SaveAs =  wxID_HIGHEST + 3,
    ID_Close =  wxID_HIGHEST + 5,
    ID_button_reduce =  wxID_HIGHEST + 6,
    ID_button_cancel =  wxID_HIGHEST + 7,
//    ID_artificial_horizon =  wxID_HIGHEST + 8,
    ID_combo_body = wxID_HIGHEST + 9,
//    ID_combo_limb = wxID_HIGHEST + 10,
    ID_combo_sign_index_error = wxID_HIGHEST + 11,
    ID_box_year = wxID_HIGHEST + 12,
    ID_combo_day = wxID_HIGHEST + 14,
    ID_stopwatch_check = wxID_HIGHEST + 15,
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
//EVT_BUTTON(ID_artificial_horizon,   MyFrame::OnCheckArtificialHorizon)
EVT_BUTTON(ID_button_cancel,   MyFrame::OnPressCancel)
EVT_BUTTON(ID_button_reduce,   MyFrame::OnPressReduce)
//EVT_COMBOBOX(ID_combo_body, MyFrame::OnSelectBody)
//EVT_CHECKBOX(ID_stopwatch, MyFrame::OnCheckStopwatch)
//EVT_COMBOBOX(ID_combo_month, MyFrame::TabulateDays)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    
    //obtain width and height of the display, and create a wxRect with height and width half ot the height and width of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*0.75);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*0.75);
    
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
    
    idling = false;
    (printerrormessage.f) = this;
    
    file_init.set_name(String(path_file_init));
    check &= (file_init.open(String("in"), prefix));
    
    wxMenu *menuFile = new wxMenu;
    catalog = new Catalog(String(path_file_catalog), String(""));
    
    sight = new Sight();
    
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    
     
    signs.Clear();
    signs.Add(wxT("+"));
    signs.Add(wxT("-"));

    
    menuFile->Append(ID_Open, "&Open...\tCtrl-O", "This is to open a file");
    //this adds a separator, a horizontal line in the menu
    menuFile->AppendSeparator();
    menuFile->Append(ID_SaveAs, "&Save as...\tCtrl-Shift-S", "This is to save as");
    menuFile->Append(ID_Save, "&Save...\tCtrl-S", "This is to save");
    menuFile->Append(ID_Close, "&Close...\tCtrl-W", "This is to close the document");
    
    menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    SetMenuBar( menuBar );
    
    
    grid_sizer = new wxGridSizer(10, 2, 0, 0);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer = new wxBoxSizer(wxVERTICAL);
    
    for(i=0; i<((*catalog).list).size(); i++){
        bodies.Add((((*catalog).list)[i]).name.value.c_str());
    }
    wxStaticText* text_combo_body = new wxStaticText(panel, wxID_ANY, wxT("Celestial body"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    body = new BodyField(this, &(sight->body), catalog);

//    combo_body = new wxComboBox(panel, ID_combo_body, wxT(""), wxDefaultPosition, wxDefaultSize, bodies, wxCB_DROPDOWN);
    //combo_body->Bind(wxEVT_KILL_FOCUS, wxFocusEventHandler(MyFrame::OnSelectBody), this);
    //combo_body->SetValue("");
    
    wxStaticText* text_limb = new wxStaticText(panel, wxID_ANY, wxT("Limb"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
//    combo_limb = new wxComboBox(panel, ID_combo_limb, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    limb = new LimbField(this, &(sight->limb));
//    (limb->name)->SetValue("");
//    (limb->name)->Enable(false);

    //sextant altitude
    wxStaticText* text_H_s = new wxStaticText(panel, wxID_ANY, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    H_s = new AngleField(this, &(sight->H_s));

    combo_sign_index_error = new wxComboBox(panel, ID_combo_sign_index_error, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
    AdjustWidth(combo_sign_index_error);
    //combo_sign_index_error->SetValue("");
    
    //index error
    wxStaticText* text_index_error = new wxStaticText(panel, wxID_ANY, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //read index error from init file
    cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
    (sight->index_error).read_from_file(String("index error"), file_init, true, new_prefix);
    (sight->index_error).to_deg_min(&deg, &min);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    index_error = new AngleField(this, &(sight->index_error));
    
    //artificial horizon
    wxStaticText* text_artificial_horizon_check = new wxStaticText(panel, wxID_ANY, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
//    artificial_horizon = new wxCheckBox(panel, ID_artificial_horizon, wxT(""), wxDefaultPosition, wxDefaultSize);
    artificial_horizon_check = new CheckField(this, &(sight->artificial_horizon));
    (checkartificialhorizon.p) = artificial_horizon_check;
    (artificial_horizon_check->check)->Bind(wxEVT_CHECKBOX, checkartificialhorizon);
    
    //height of eye
    wxStaticText* text_height_of_eye = new wxStaticText(panel, wxID_ANY, wxT("Height of eye"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    height_of_eye = new LengthField(this, &(sight->height_of_eye));
    
    //master-clock date
    //sets  sight.master_clock_date_and_hour.date to the current UTC date
    (sight->master_clock_date_and_hour).date.set_current(prefix);
    wxStaticText* text_date = new wxStaticText(panel, wxID_ANY, wxT("Master-clock UTC date and hour of sight"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_date = new DateField(this, &(sight->master_clock_date_and_hour.date));
    master_clock_date->set();
    
    //master-clock hour
    (sight->master_clock_date_and_hour).chrono.set_current(prefix);
    wxStaticText* text_space_1 = new wxStaticText(panel, wxID_ANY, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_chrono = new ChronoField(this, &(sight->master_clock_date_and_hour.chrono));
    master_clock_chrono->set();

    //check/uncheck stopwatch
    wxStaticText* text_stopwatch_check = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    stopwatch_check = new CheckField(this, &(sight->use_stopwatch));
//    stopwatch_check = new wxCheckBox(panel, ID_stopwatch_check, wxT(""), wxDefaultPosition, wxDefaultSize);
    //EVT_CHECKBOX(ID_stopwatch, MyFrame::OnCheckStopwatch)
    (checkstopwatch.p) = stopwatch_check;
    (stopwatch_check->check)->Bind(wxEVT_CHECKBOX, checkstopwatch);
    
    //stopwatch reading
    wxStaticText* text_stopwatch_reading = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //    stopwatch_reading = new ChronoField(this, &(sight.stopwatch));
    stopwatch_reading = new ChronoField(this, &(sight->stopwatch));

    //initialize stopwatch_check and stopwatch_reading
    (stopwatch_check->check)->SetValue(false);
    stopwatch_reading->Enable(false);
//    (stopwatch_reading->hour)->SetValue(wxString("0"));
//    (stopwatch_reading->minute)->SetValue(wxString("0"));
//    (stopwatch_reading->second)->SetValue(wxString("0.0"));


    //TAI-UTC
    //read TAI_minus_UTC from /Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/index.txt
    cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
    (sight->TAI_minus_UTC).read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    wxStaticText* text_TAI_minus_UTC = new wxStaticText(panel, wxID_ANY, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    TAI_minus_UTC = new ChronoField(this, &(sight->TAI_minus_UTC));

    

    //buttons
    button_cancel = new wxButton(panel, ID_button_cancel, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_reduce = new wxButton(panel, ID_button_reduce, "Reduce", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_reduce->Enable(false);
    
    grid_sizer->Add(text_combo_body);
    body->InsertIn<wxGridSizer>(grid_sizer);
//    grid_sizer->Add(combo_body);
    
    grid_sizer->Add(text_limb);
    limb->InsertIn<wxGridSizer>(grid_sizer);
    
    grid_sizer->Add(text_H_s);
    H_s->InsertIn<wxGridSizer>(grid_sizer);
    
    grid_sizer->Add(text_index_error);
    box_sizer_3->Add(combo_sign_index_error);
    index_error->InsertIn<wxBoxSizer>(box_sizer_3);
    grid_sizer->Add(box_sizer_3);

    grid_sizer->Add(text_artificial_horizon_check);
    artificial_horizon_check->InsertIn<wxGridSizer>(grid_sizer);
    
    grid_sizer->Add(text_height_of_eye);
    height_of_eye->InsertIn<wxGridSizer>(grid_sizer);
    
    grid_sizer->Add(text_date);
    master_clock_date->InsertIn<wxBoxSizer>(box_sizer_4);
    box_sizer_4->Add(text_space_1);
    master_clock_chrono->InsertIn<wxBoxSizer>(box_sizer_4);
    grid_sizer->Add(box_sizer_4);

    grid_sizer->Add(text_stopwatch_check);
    stopwatch_check->InsertIn<wxGridSizer>(grid_sizer);
    
    grid_sizer->Add(text_stopwatch_reading);
    stopwatch_reading->InsertIn<wxGridSizer>(grid_sizer);

    
    grid_sizer->Add(text_TAI_minus_UTC);
    TAI_minus_UTC->InsertIn<wxGridSizer>(grid_sizer);

    
    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer_2->Add(button_reduce, 0, wxALIGN_BOTTOM);
    
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
    
    //
    //here I read a sample sight from file_sample_sight, store into sight and set all the fields in this to the data in sight with set()
    File file_sample_sight;
    file_sample_sight.set_name(String("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample_sight.txt"));
    file_sample_sight.open(String("in"), String(""));
    sight->read_from_file(file_sample_sight, String(""));
    file_sample_sight.close(String(""));
    set();
    //

    
    
}

//set all the GUI fields in this equal to those in the non-GUI object this->sight
void MyFrame::set(void){
    
    body->set();
    limb->set();
    artificial_horizon_check->set();
    H_s->set();
    index_error->set();
    
    if(!((artificial_horizon_check->check)->GetValue())){
        height_of_eye->Enable(true);
        height_of_eye->set();
    }else{
        height_of_eye->Enable(false);
    }
    
    master_clock_date->set();
    master_clock_chrono->set();
    stopwatch_check->set();
    
    if(((stopwatch_check->check)->GetValue())){
        stopwatch_reading->Enable(true);
        stopwatch_reading->set();
    }else{
        stopwatch_reading->Enable(false);
   }
    
    TAI_minus_UTC->set();
    
}

void MyFrame::SetIdling(bool b){

    idling = b;

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








void CheckHour::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, (p->hour), String("Entered value is not valid!\nHours must be unsigned integer numbers >= 0 and < 24"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->hour);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Hours must be unsigned integer numbers >= 0 and < 24");
        f->CallAfter((f->printerrormessage));
        
        
        (p->hour_ok) = false;
        
    }else{
        
        (p->hour)->SetBackgroundColour(*wxWHITE);
        ((p->chrono)->h) = ((unsigned int)wxAtoi((p->hour)->GetValue()));
        (p->hour_ok) = true;
        
    }
    
     (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
    
    event.Skip(true);
        
    }
    
}


void CheckMinute::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, (p->minute), String("Entered value is not valid!\nMinutes must be unsigned integer numbers >= 0 and < 60"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->minute);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Minutes must be unsigned integer numbers >= 0 and < 60");
        f->CallAfter((f->printerrormessage));
        
        (p->minute_ok) = false;
        
    }else{
        
        (p->minute)->SetBackgroundColour(*wxWHITE);
        ((p->chrono)->m) = ((unsigned int)wxAtoi((p->minute)->GetValue()));
        (p->minute_ok) = true;
        
    }
    
     (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
    
    event.Skip(true);
        
    }
    
}



void CheckSecond::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
    
    if(!check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
        
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, p->second, String("Entered value is not valid!\nSeconds must be floating-point numbers >= 0.0 and < 60.0"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->second);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Seconds must be floating-point numbers >= 0.0 and < 60.0");
        f->CallAfter((f->printerrormessage));
        
        (p->second_ok) = false;
        
    }else{
        
        double s_temp;

        (p->second)->SetBackgroundColour(*wxWHITE);
        ((p->second)->GetValue()).ToDouble(&s_temp);
        ((p->chrono)->s) = s_temp;
        (p->second_ok) = true;

    }
    
    
     (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
    
    event.Skip(true);
        
    }
    
}



void CheckYear::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_unsigned_int(((p->year)->GetValue()).ToStdString(), NULL, false, 0, 0)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, p->year, String("Entered value is not valid!\nYear must be an unsigned integer"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->year);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Year must be an unsigned integer");
        f->CallAfter((f->printerrormessage));
        
        (p->year_ok) = false;
        (p->day)->Enable(false);


    }else{
        
        (p->year)->SetBackgroundColour(*wxWHITE);
        (p->date->Y) = (unsigned int)wxAtoi((p->year)->GetValue());
        (p->year_ok) = true;
        
        if(p->month_ok){
            (f->tabulatedays)(event);
            (p->day)->Enable(true);
        }

    }
    
    (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));


    event.Skip(true);
        
    }
    
}

void CheckMonth::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
    if(!check_unsigned_int(((p->month)->GetValue()).ToStdString(), NULL, true, 1, 12+1)){
        
//        f->CallAfter(&MyFrame::PrintErrorMessage, p->month, String("Entered value is not valid!\nMonth must be an unsigned integer >= 1 and <= 12"));
        
        //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
        ((f->printerrormessage).control) = (p->month);
        ((f->printerrormessage).title) = String("Entered value is not valid!");
        ((f->printerrormessage).message) = String("Month must be an unsigned integer >= 1 and <= 12");
        f->CallAfter((f->printerrormessage));

        (p->month_ok) = false;
        (p->day)->Enable(false);

    }else{
        
        (p->month)->SetBackgroundColour(*wxWHITE);
        (p->date->M) = (unsigned int)wxAtoi((p->month)->GetValue());
        (p->month_ok) = true;

        if(p->year_ok){
            (f->tabulatedays)(event);
            (p->day)->Enable(true);

        }

    }

     (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
    
    event.Skip(true);
        
    }
    
}


void CheckDay::operator()(wxFocusEvent &event){
    
    MyFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){

    //this variable = true if the day field is formatted correctly
    bool ok;
    
    
    //to check whether the p->day is formatted correctly, I first check whether p->year and p->month are formatted correctly, so I can extract a valid value of p->month. Then, I check whether p-> day is an unsigned int formatted correctly with check_unsigned_int, and whether this unsigned int lies in the correct interval relative to p->month
    if((p->year_ok) && (p->month_ok)){
        
        (p->date)->check_leap_year();
        
        if((p->date)->Y_is_leap_year){
            
            ok = check_unsigned_int(((p->day)->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi((p->month)->GetValue()))-1]+1);
        
        }else{

            ok = check_unsigned_int(((p->day)->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi((p->month)->GetValue()))-1]+1);
            
        }
        
        if(ok){
            
            (p->day)->Enable(true);
            (p->day)->SetBackgroundColour(*wxWHITE);
            (p->date->D) = (unsigned int)wxAtoi((p->day)->GetValue());
            (p->day_ok) = true;
            
        }else{
            
//            f->CallAfter(&MyFrame::PrintErrorMessage, p->day, String("Entered value is not valid!\nDay must be an unsigned integer comprised between the days of the relative month"));
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage).control) = (p->day);
            ((f->printerrormessage).title) = String("Entered value is not valid!");
            ((f->printerrormessage).message) = String("Day must be an unsigned integer comprised between the days of the relative month");
            f->CallAfter((f->printerrormessage));
            
            (p->day)->Enable(true);
            (p->day_ok) = false;
            
        }
        
        
    }else{
        
        (p->day)->Enable(false);
        
    }
    
     (f->button_reduce)->Enable(((f->body->is_ok())) && ((f->limb->is_ok())) && ((f->H_s)->is_ok()) && ((f->index_error)->is_ok()) && ((f->master_clock_date)->is_ok()) && ((f->master_clock_chrono)->is_ok()) && ((!(((f->stopwatch_check)->check)->GetValue())) || ((f->stopwatch_reading)->is_ok())) && ((f->TAI_minus_UTC)->is_ok()));
   
    event.Skip(true);
        
    }
    
}






void TabulateDays::operator()(wxFocusEvent &event){
    
    unsigned int i;
    MyFrame* f = (p->parent_frame);

    if((p->year_ok) && (p->month_ok)){
        
        //read the year
        ((f->sight)->master_clock_date_and_hour).date.Y = ((unsigned int)wxAtoi((p->year)->GetValue()));
        ((f->sight)->master_clock_date_and_hour).date.check_leap_year();
        
        //read the month
       ((f->sight)->master_clock_date_and_hour).date.M = ((unsigned int)wxAtoi((p->month)->GetValue()));
        
        if(((f->sight)->master_clock_date_and_hour).date.Y_is_leap_year){
            //in this case the year is a leap year: I fill the list of days from days_per_month_leap
            
            for((p->days).Clear(), i=0; i<days_per_month_leap[(((f->sight)->master_clock_date_and_hour).date.M)-1]; i++){
                (p->days).Add(wxString::Format(wxT("%i"),i+1));
            }
            
        }else{
            //in this case the year is a common year: I fill the list of days from days_per_month_common
            
            for((p->days).Clear(), i=0; i<days_per_month_common[(((f->sight)->master_clock_date_and_hour).date.M)-1]; i++){
                (p->days).Add(wxString::Format(wxT("%i"),i+1));
            }
            //
        }
        
        (p->day)->Set((p->days));
        (p->day)->SetValue(wxString("1"));
        (p->day)->Enable(true);
        
    }else{
        
        (p->day)->Enable(false);
        
    }
    
    event.Skip(true);
    
}



//this function writes into sight.artificial_horizon the value entered in the GUI box
void CheckArtificialHorizon::operator()(wxCommandEvent& event){
    
    MyFrame* f = (p->parent_frame);
    
    //I set p->answer to the value entered in the GUI checkbox
    if((p->check)->GetValue()){
        ((p->answer)->value) = 'y';
        (f->height_of_eye)->Enable(false);
    }else{
        ((p->answer)->value) = 'n';
        (f->height_of_eye)->Enable(true);
    }
  

}

//this function writes into sight.use_stopwatch the value written into the respective GUI box and it enables/disables all fields in stopwatch reading if stopwatch_check is enabled/disabled, respectively
void CheckStopWatch::operator()(wxCommandEvent& event){
    
    MyFrame* f = (p->parent_frame);

    //I set p->answetr to the value entered in the GUI checkbox
    if((p->check)->GetValue()){
        ((p->answer)->value) = 'y';
    }else{
        ((p->answer)->value) = 'n';
    }
    
    //I enable f->stopwatch reading GUI field and set all its entries to zero
    (f->stopwatch_reading)->Enable((p->check)->GetValue());
    ((f->stopwatch_reading)->hour)->SetValue(wxString(""));
    ((f->stopwatch_reading)->minute)->SetValue(wxString(""));
    ((f->stopwatch_reading)->second)->SetValue(wxString(""));

}



void MyFrame::OnPressReduce(wxCommandEvent& event){
    
    stringstream s;
    sight->print(String("body entered via GUI"), String(""), s);
    
    
    
    wxMessageBox(s.str().c_str(), wxT("Here is the data which you entered:"));
    
    Close(TRUE);
    
}


//constructor of a BodyField object, based on the parent frame frame
BodyField::BodyField(MyFrame* frame, Body* p, Catalog* c){

    unsigned int i;
    parent_frame = frame;
    //I link the internal pointers p and c to the respective body and body catalog
    body = p;
    catalog = c;
    
    for(bodies.Clear(), i=0; i<(catalog->list).size(); i++){
        bodies.Add(((catalog->list)[i]).name.value.c_str());
    }

    ((parent_frame->checkbody).p) = this;

    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, bodies, wxCB_DROPDOWN);
    //name->SetInitialSize(name->GetSizeFromTextSize(name->GetTextExtent(wxS("000"))));
    //name->SetValue("");
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, parent_frame->checkbody);

    name->SetValue("");
    ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);
     
}


//sets the value in the GUI object name equal to the value in the non-GUI Body object body
void BodyField::set(void){
    
    name->SetValue((body->name).value);
    ok = true;
    
}

//sets the value in the GUI object name equal to the value in the non-GUI limb object limb
void LimbField::set(void){
    
    if((limb->value) == 'u'){
        name->SetValue("upper");
    }
    if((limb->value) == 'l'){
        name->SetValue("lower");
    }
    if((limb->value) == 'c'){
        name->SetValue("center");
    }

    ok = true;
    
}

//sets the value in the GUI object check equal to the value in the non-GUI limb object answer
void CheckField::set(void){
    
    if((answer->value) == 'y'){
        check->SetValue(true);
    }
    
    if((answer->value) == 'n'){
        check->SetValue(false);
    }
    
}

//sets the value in the GUI objects deg and min equal to the value in the non-GUI limb object angle
void AngleField::set(void){
    
    unsigned int deg_temp;
    double min_temp;
    
    angle->to_deg_min(&deg_temp, &min_temp);

    
    deg->SetValue(wxString::Format(wxT("%i"), deg_temp));
    min->SetValue(wxString::Format(wxT("%f"), min_temp));
    
    deg_ok = true;
    min_ok = true;
    
}


//sets the value in the GUI object value equal to the value in the non-GUI  object length
void LengthField::set(void){
    
    value->SetValue(wxString::Format(wxT("%f"), length->value));
    
    ok = true;
    
}

//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date
void DateField::set(void){
    
    year->SetValue(wxString::Format(wxT("%i"), date->Y));
    month->SetValue(wxString::Format(wxT("%i"), date->M));
    day->SetValue(wxString::Format(wxT("%i"), date->D));
    
    year_ok = true;
    month_ok = true;
    day_ok = true;
}

//sets the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object chrono
void ChronoField::set(void){
    
    hour->SetValue(wxString::Format(wxT("%i"), chrono->h));
    minute->SetValue(wxString::Format(wxT("%i"), chrono->m));
    second->SetValue(wxString::Format(wxT("%f"), chrono->s));
    
    hour_ok = true;
    minute_ok = true;
    second_ok = true;
    
}


//constructor of a LimbField object, based on the parent frame frame
LimbField::LimbField(MyFrame* frame, Limb* p){

    parent_frame = frame;
    //I link the internal pointers p the respective Limb object
    limb = p;
    
    limbs.Clear();
    limbs.Add(wxT("upper"));
    limbs.Add(wxT("lower"));
    limbs.Add(wxT("center"));


    ((parent_frame->checklimb).p) = this;

    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    //name->SetInitialSize(name->GetSizeFromTextSize(name->GetTextExtent(wxS("000"))));
    //name->SetValue("");
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, parent_frame->checklimb);
    
    name->SetValue(wxString(""));
    ok = false;
        
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);
     
}



//constructor of a CheckField object, based on the parent frame frame
CheckField::CheckField(MyFrame* frame, Answer* p){

    parent_frame = frame;
    //I link the internal pointers p and c to the respective Answer object
    answer = p;

    check = new wxCheckBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
//    name->Bind(wxEVT_KILL_FOCUS, parent_frame->checkbody);

    check->SetValue(false);
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(check, 0, wxALIGN_CENTER);
     
}



//constructor of an AngleField object, based on the parent frame frame
AngleField::AngleField(MyFrame* frame, Angle* p){

    unsigned int i;
    parent_frame = frame;
    angle = p;
    
    for(degrees.Clear(), i=0; i<360; i++){
        degrees.Add(wxString::Format(wxT("%i"), i));
    }

    ((parent_frame->checkarcdegree).p) = this;
    ((parent_frame->checkarcminute).p) = this;

    deg = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    AdjustWidth(deg);
    deg->Bind(wxEVT_KILL_FOCUS, parent_frame->checkarcdegree);

    text_deg = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("° "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    min = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS(sample_width_floating_point_field))));
    min->Bind(wxEVT_KILL_FOCUS, parent_frame->checkarcminute);

    text_min = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("' "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    deg->SetValue(wxString(""));
    min->SetValue(wxString(""));
    deg_ok = false;
    min_ok = false;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(deg, 0, wxALIGN_CENTER);
    sizer_h->Add(text_deg);
    sizer_h->Add(min, 0, wxALIGN_CENTER);
    sizer_h->Add(text_min);
    
}

//constructor of a LengthField object, based on the parent frame frame
LengthField::LengthField(MyFrame* frame, Length* p){

    parent_frame = frame;
    length = p;
    
    ((parent_frame->checklength).p) = this;

    
    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, parent_frame->checklength);

    text = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("m"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Length object
    value->SetValue(wxString(""));
    ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);
    sizer_h->Add(text);
    
}


bool AngleField::is_ok(void){
    
    return(deg_ok && min_ok);
    
}

bool LengthField::is_ok(void){
    
    return(ok);
    
}

bool ChronoField::is_ok(void){
    
    return(hour_ok && minute_ok && second_ok);
    
}

//constructor of a DateField object, based on the parent frame frame
DateField::DateField(MyFrame* frame, Date* p){
    
    unsigned int i;
    parent_frame = frame;
    date = p;
    
    ((parent_frame->checkyear).p) = this;
    ((parent_frame->checkmonth).p) = this;
    ((parent_frame->checkday).p) = this;
    ((parent_frame->tabulatedays).p) = this;

    for(months.Clear(), months.Add(wxT("")), i=0; i<12; i++){
        months.Add(wxString::Format(wxT("%i"), i+1));
    }
    
    year = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    year->SetInitialSize(year->GetSizeFromTextSize(year->GetTextExtent(wxS("0000"))));
    year->Bind(wxEVT_KILL_FOCUS, (parent_frame->checkyear));

    text_hyphen_1 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
    
    month = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN);
//    month->SetInitialSize(month->GetSizeFromTextSize(month->GetTextExtent(wxS("00"))));
    AdjustWidth(month);
    month->Bind(wxEVT_KILL_FOCUS, (parent_frame->checkmonth));

    text_hyphen_2 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);

    
    day = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN);
//I create a temporary days list to set the size of the wxComboBox day with AdjustWidth, and then destroy this temporary days list
    for(days.Clear(), i=0; i<31; i++){
        days.Add(wxString::Format(wxT("%i"), i+1));
    }
    day->Set(days);
    AdjustWidth(day);
    days.Clear();
    day->Bind(wxEVT_KILL_FOCUS, (parent_frame->checkday));

    
    year->SetValue(wxString(""));
    year_ok = false;
    
    month->SetValue(wxString(""));
    month_ok = false;
    
    day->SetValue(wxString(""));
    day_ok = false;
      
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(year, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_1, wxALIGN_CENTER);
    sizer_h->Add(month, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_2, wxALIGN_CENTER);
    sizer_h->Add(day, 0, wxALIGN_CENTER);

}

//constructor of a ChronoField object, based on the parent frame frame
ChronoField::ChronoField(MyFrame* frame, Chrono* p){
    
    unsigned int i;
    parent_frame = frame;
    chrono = p;
    
    ((parent_frame->checkhour).p) = this;
    ((parent_frame->checkminute).p) = this;
    ((parent_frame->checksecond).p) = this;

    for(hours.Clear(), hours.Add(wxT("")), i=0; i<24; i++){
        hours.Add(wxString::Format(wxT("%i"), i+1));
    }
    for(minutes.Clear(), minutes.Add(wxT("")), i=0; i<60; i++){
        minutes.Add(wxString::Format(wxT("%i"), i+1));
    }
    
    hour = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN);
//    hour->SetInitialSize(hour->GetSizeFromTextSize(hour ->GetTextExtent(wxS("00"))));
    AdjustWidth(hour);
    hour->Bind(wxEVT_KILL_FOCUS, (parent_frame->checkhour));

    text_colon_1 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize);
    
    minute = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN);
    AdjustWidth(minute);
//    minute->SetInitialSize(minute->GetSizeFromTextSize(minute->GetTextExtent(wxS("00"))));
    minute->Bind(wxEVT_KILL_FOCUS, (parent_frame->checkminute));

    text_colon_2 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize);

    second = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxCB_DROPDOWN);
    second->SetInitialSize(second->GetSizeFromTextSize(second->GetTextExtent(wxS(sample_width_floating_point_field))));
    second->Bind(wxEVT_KILL_FOCUS, (parent_frame->checksecond));
    
    
    hour->SetValue(wxString(""));
    hour_ok = false;
    
    minute->SetValue(wxString(""));
    minute_ok = false;
    
    second->SetValue(wxString(""));
    second_ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(hour, 0, wxALIGN_CENTER);
    sizer_h->Add(text_colon_1, wxALIGN_CENTER);
    sizer_h->Add(minute, 0, wxALIGN_CENTER);
    sizer_h->Add(text_colon_2, wxALIGN_CENTER);
    sizer_h->Add(second, 0, wxALIGN_CENTER);

}

//this function enables/disable the LengthField
void LengthField::Enable(bool is_enabled){
    
    value->Enable(is_enabled);
  
}

//this function enables/disable the whole ChronoField
void ChronoField::Enable(bool is_enabled){
    
    hour->Enable(is_enabled);
    minute->Enable(is_enabled);
    second->Enable(is_enabled);

}

bool BodyField::is_ok(void){
    
    return(ok);
    
}

bool LimbField::is_ok(void){
    
    return(ok);
    
}

bool DateField::is_ok(void){
    
    return(year_ok && month_ok && day_ok);
    
}

template<class T> void BodyField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void LimbField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void CheckField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}


template<class T> void AngleField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void LengthField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}


template<class T> void DateField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void ChronoField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}
