//this string defines the width of GUI fields hosting floating-point numbers
#define sample_width_floating_point_field "0.000000000"
#define sample_width_string_field "Et repellat optio nam iste voluptatum in magnam?"

class BodyField;
class LimbField;
class CheckField;
class AngleField;
class LengthField;
class DateField;
class ChronoField;
class StringField;
class MyApp;
class MessageFrame;
class SightFrame;
class PlotFrame;

struct CheckBody;
struct CheckLimb;
struct CheckCheck;
struct CheckChrono;
struct CheckAngle;
struct CheckSign;
struct CheckArcDegree;
struct CheckArcMinute;
struct CheckLength;
struct CheckDate;
struct CheckYear;
struct CheckMonth;
struct CheckDay;
struct CheckHour;
struct CheckMinute;
struct CheckSecond;
struct CheckString;
struct SetLabelToCurrentTime;
struct TabulateDays;
struct PrintErrorMessage;
struct OnSelectInListBox;


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

struct CheckBody{
    
    BodyField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckLimb{
    
    LimbField* p;
    
    template<class T> void operator()(T&);

    
    
};


struct CheckCheck{
    
    CheckField* p;
    
    //this functor checks whether a GUI Check field is filled correctly and writes its value into the relative non-GUI field
    template<class T> void operator()(T&);
    
};



struct CheckSign{
    
    AngleField* p;
    
    template <class T> void operator()(T&);
    
    
};

struct CheckArcDegree{
    
    AngleField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckArcMinute{
    
    AngleField* p;
    
    template <class T> void operator()(T&);
    
    
};


struct CheckAngle{
    
    AngleField* p;
    CheckSign check_sign;
    CheckArcDegree check_arc_degree;
    CheckArcMinute check_arc_minute;
    
    template <class T> void operator()(T&);
    
};


struct CheckYear{
    
    DateField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckMonth{
    
    DateField* p;
    
    template<class T> void operator()(T&);

    
};

struct CheckDay{
    
    DateField* p;
    
    template<class T> void operator()(T&);

    
};


struct CheckDate{
    
    DateField* p;
    CheckYear check_year;
    CheckMonth check_month;
    CheckDay check_day;
    
    template <class T> void operator()(T&);
    
};


struct CheckLength{
    
    LengthField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckString{
    
    StringField* p;
    
    template<class T> void operator()(T&);
    
};


//this is a GUI field contaning a binary checkbox, which is either checked or unchecked
class CheckField{
    
public:
    
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    Answer* answer;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxCheckBox with the name of the bodies
    wxCheckBox* checkbox;
    CheckCheck check;
    
    CheckField(SightFrame*, Answer*);
    
    template<class T> void InsertIn(T*);
    void set(void);
    
    
};


class BodyField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString bodies;
    //this points to a Body object, which contains the date written in the GUI field of this
    Body* body;
    Catalog* catalog;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    CheckBody check;
    
    bool ok;
    
    BodyField(SightFrame*, Body*, Catalog*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};


class LimbField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString limbs;
    //this points to a Limn object, which contains the data written in the GUI field of this
    Limb* limb;
    wxBoxSizer *sizer_h, *sizer_v;
    bool ok;
    CheckLimb check;
    
    //this is the wxComboBox with the name of the bodies
    wxComboBox* name;
    
    LimbField(SightFrame*, Limb*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};



//class for graphical object: a field to enter an angle, composed of a box for the sign, a box for the degrees, a degree text symbol, another box for minutes and a minute text symbol
class AngleField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    wxArrayString signs, degrees;
    //degrees and minutes boxes
    wxComboBox*sign, * deg;
    wxTextCtrl *min;
    //texts
    wxStaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    Angle* angle;
    //deg_ok = true if the degrees part of this angle is formatted properly and set to the same value as the degree part of angle, and simiarly for min
    bool sign_ok, deg_ok, min_ok;
    CheckAngle check;
    
    
    AngleField(SightFrame*, Angle*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a length, composed of a box
class LengthField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    //degrees and minutes boxes
    wxTextCtrl *value;
    //texts
    wxStaticText* text;
    wxBoxSizer *sizer_h, *sizer_v;
    Length* length;
    //ok = true if this Length is formatted properly and set to the same value as the non-GUI object length
    bool ok;
    CheckLength check;
    
    LengthField(SightFrame*, Length*);
    void set(void);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a String, composed of a box
class StringField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    //label box
    wxTextCtrl *value;
    wxBoxSizer *sizer_h, *sizer_v;
    //non-GUI object related to this
    String* string;
    CheckString check;
    
    StringField(SightFrame*, String*);
    void set(void);
    template<class T> void InsertIn(T*);
    
};



class DateField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
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
    CheckDate check;
    
    DateField(SightFrame*, Date*);
    void set(void);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};


class ChronoField{
    
public:
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
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
    
    ChronoField(SightFrame*, Chrono*);
    void set(Chrono);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};







struct CheckChrono{
    
    CheckField* p;
    
    template<class T> void operator()(T&);
    
    
};




struct SetLabelToCurrentTime{
    
    StringField* p;
    
    void operator()(wxCommandEvent&);
    
    
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
    
    template<class T> void operator()(T&);
    
    
};

//this functor pops out an error-message window with title tile and error message message, resulting from the wxControl control
struct PrintErrorMessage{
    
    SightFrame* f;
    wxControl* control;
    String title, message;
    
    void operator()(void);
    
    
};


//this is a wxFrame designed to show a message to the GUI user
class MessageFrame: public wxFrame{
    
public:
    MessageFrame(wxWindow* parent, const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix);
    
    wxPanel *panel;
    wxBoxSizer *sizer_h, *sizer_v;
    wxGridSizer* sizer_grid;
    wxButton* button_ok;
    wxStaticBitmap* image;
    
    void OnPressOk(wxCommandEvent&);
};

//this is a wxFrame designed to contain the list of sights, routes, etc...
class PlotFrame: public wxFrame{
    
public:
    //this frame has no parent, because it is supposed to be the main frame of the appplication
    PlotFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix);
    
    //this is a pointer to the non-GUI object Plot which is related to the GUI object this
    Plot* plot;
    //this is a pointer to a Catalog object which will be used by plot
    Catalog *catalog;
    //    wxListBox* listbox;
    wxListCtrl* listcontrol;
    wxPanel *panel;
    wxButton* button_add, *button_delete;
    wxBitmapButton *button_modify;
    wxSizer* sizer_h, *sizer_v, *sizer_buttons;
    
    void OnAdd(wxCommandEvent& event);
    void OnModify(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    
};



class SightFrame: public wxFrame{
    
public:
    SightFrame(PlotFrame*, Sight*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    PlotFrame* parent;
    Catalog* catalog;
    Sight* sight;
    //this long represents the position in the list (this->GetParent())->listcontrol of sight. If position = -1, then sight is not in that list
    long position;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    CheckSign checksign;
    SetLabelToCurrentTime setlabeltocurrenttime;
    CheckHour checkhour;
    CheckMinute checkminute;
    CheckSecond checksecond;
    CheckChrono check_stopwatch;
    TabulateDays tabulatedays;
    PrintErrorMessage printerrormessage;
    
    BodyField* body;
    LimbField* limb;
    CheckField* artificial_horizon_check, *stopwatch_check;
    AngleField* H_s, *index_error;
    LengthField* height_of_eye;
    DateField *master_clock_date;
    ChronoField *master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    StringField *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_time, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    wxStaticBoxSizer *sizer_box_measurement, *sizer_box_time;
    
    wxArrayString bodies, limbs;
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
    void TryToEnableReduce(void);
    
    // The Path to the file we have open
    wxString CurrentDocPath;
    
    wxDECLARE_EVENT_TABLE();
    
};


template<class T>void CheckBody::operator()(T& event){
    
    cout << "You kjust called checkbody ----------- \n";
    
    SightFrame* f = (p->parent_frame);
    
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
            
            cout << "Start\n";
            (*(p->body)).print(String("XXXXX body"), String(""), cout);
            cout << "End\n";
            
        }else{
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage).control) = (p->name);
            ((f->printerrormessage).title) = String("Body not found in catalog!");
            ((f->printerrormessage).message) = String("Body must be in catalog.");
            f->CallAfter((f->printerrormessage));
            
            (p->ok) = false;
            
        }
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}


template<class T> void CheckLimb::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
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
        
        f->TryToEnableReduce();
        
    }

    event.Skip(true);
    
}


template <class T> void CheckSign::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is in idling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        
        //I check whether the name in the GUI field sign matches one of the sign values in the list signs
        for(check = false, i=0; (i<((p->signs).GetCount())) && (!check); i++){
            if(((p->sign)->GetValue()) == (p->signs)[i]){
                check = true;
            }
        }
        
        if(check){
            
            (p->sign)->SetBackgroundColour(*wxWHITE);
            
            if((p->deg_ok) && (p->min_ok)){
                
                double min_temp;
                
                ((p->min)->GetValue()).ToDouble(&min_temp);
                
                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
                
            }
            
            (p->sign_ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage).control) = (p->sign);
            ((f->printerrormessage).title) = String("Sign is not valid!");
            ((f->printerrormessage).message) = String("Sign must be + or -.");
            f->CallAfter((f->printerrormessage));
            
            (p->sign_ok) = false;
            
        }
        
        f->TryToEnableReduce();
        
        
    }

    event.Skip(true);

}

void SetLabelToCurrentTime::operator()(wxCommandEvent &event){
    
    //if the label is empty, I replace it with the local time and date
    if(((p->value)->GetValue()).IsEmpty()){
        
        Time time_temp;
        
        time_temp.set_current(String(""));
        //I write in the non-GUI object (p->string)
        (*(p->string)) = String(time_temp.to_string(display_precision));
        
        p->set();
        
    }
    
    event.Skip(true);
    
}


template<class T> void CheckString::operator()(T &event){
    
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is in idling mode
    if(!(f->idling)){
        
        //I write in the non-GUI object (p->string) the value entered in the GUI object (p->value)
        (*(p->string)) = String(((p->value)->GetValue().ToStdString()));
        
        f->TryToEnableReduce();
        
        
    }

    event.Skip(true);

}

//this functor checks the whole angle field by calling the check on its sign, arcdegree and arcminute parts‰
template <class T> void CheckAngle::operator()(T& event){
    
    check_sign(event);
    check_arc_degree(event);
    check_arc_minute(event);
    
    event.Skip(true);
    
}

//this functor checks the whole date field by calling the check on its year, month and day parts‰
template <class T> void CheckDate::operator()(T& event){
    
    check_year(event);
    check_month(event);
    check_day(event);
    
    event.Skip(true);
    
}



template<class T> void CheckArcDegree::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->deg)->GetValue()).ToStdString(), NULL, true, 0, 360)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->deg), String("Entered value is not valid!\nArcdegrees must be unsigned integer numbers >= 0° and < 360°"));
            
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
                
                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
                
            }
            (p->deg_ok) = true;
            
        }
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}

template <class T> void CheckArcMinute::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->min)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->min, String("Entered value is not valid!\nArcminutes must be floating-point numbers >= 0' and < 60'"));
            
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
                
                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
                
                
            }
            (p->min_ok) = true;
        }
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}

template <class T> void CheckLength::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->value)->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX)){
            
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
            (p->length)->set(String(""), /*the length is entered in the GUI field in meters, thus I convert it to nm here*/length_temp/(1e3*nm), String(""));
            
            (p->ok) = true;
            
        }
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}

struct OnSelectInListBox{
    
    PlotFrame* f;
    
    template<class T> void operator()(T&);
    
    
};

template<class T> void OnSelectInListBox::operator()(T& event){
    
    (f->button_modify)->Enable(true);

    event.Skip(true);

}

void PrintErrorMessage::operator()(void){
    
    MessageFrame* message_frame;
    
    //I may be about to prompt a temporary dialog window, thus I set f->idling to true
    f->SetIdling(true);
    
    if(control != NULL){
        
        if(((control->GetBackgroundColour()) != *wxRED)){
            
            message_frame = new MessageFrame(f, title.value, message.value, wxDefaultPosition, wxDefaultSize, String(""));
            message_frame ->Show(true);
            
            control->SetFocus();
            control->SetBackgroundColour(*wxRED);
            
        }
        
    }else{
        
        message_frame = new MessageFrame(f, title.value, message.value, wxDefaultPosition, wxDefaultSize, String(""));
        message_frame ->Show(true);
        
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
    
};

wxBEGIN_EVENT_TABLE(SightFrame, wxFrame)
EVT_MENU(ID_Open,   SightFrame::OnOpen)
EVT_MENU(ID_Save,   SightFrame::OnSave)
EVT_MENU(ID_SaveAs,   SightFrame::OnSaveAs)
EVT_MENU(ID_Close,  SightFrame::OnClose)
EVT_BUTTON(ID_button_cancel,   SightFrame::OnPressCancel)
EVT_BUTTON(ID_button_reduce,   SightFrame::OnPressReduce)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    
    
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*0.75);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*0.75);
    
    
    PlotFrame *list_frame = new PlotFrame("List of sights", "", wxDefaultPosition, rectangle.GetSize(), String(""));
    list_frame->Show(true);
    
    return true;
    
}

SightFrame::SightFrame(PlotFrame* parent_input, Sight* sight_in, long position_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    parent = parent_input;
    
    //pointer to init.txt to read fixed sight data from in there
    File file_init;
    String new_prefix;
    unsigned int i, deg, common_width;
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
    
    //if this SightFrame has been constructed with sight_in = NULL, then I allocate a new Sight object with the pointer this->sight and set position to a 'NULL' value (position = -1). Otherwise, the pointer sight_in points to a valid Sight object -> I let this->sight point to sight_in, and set position to position_in.
    if(sight_in != NULL){
        sight = sight_in;
        position = position_in;
    }else{
        sight = new Sight();
        position = -1;
    }
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    
    menuFile->Append(ID_Open, "&Open...\tCtrl-O", "This is to open a file");
    //this adds a separator, a horizontal line in the menu
    menuFile->AppendSeparator();
    menuFile->Append(ID_SaveAs, "&Save as...\tCtrl-Shift-S", "This is to save as");
    menuFile->Append(ID_Save, "&Save...\tCtrl-S", "This is to save");
    menuFile->Append(ID_Close, "&Close...\tCtrl-W", "This is to close the document");
    
    menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    SetMenuBar( menuBar );
    
    
    sizer_grid_measurement = new wxFlexGridSizer(6, 2, 0, 0);
    sizer_grid_time = new wxFlexGridSizer(4, 2, 0, 0);
    sizer_grid_label = new wxFlexGridSizer(1, 2, 0, 0);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer = new wxBoxSizer(wxVERTICAL);
    
    //First off, I need to set TAI_minus_UTC, which will be used in the following. If sight_in = NULL,  I read it from from file_init
    if(sight_in==NULL){
        
        cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
        (sight->TAI_minus_UTC).read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        
    }
    
    
    
    for(i=0; i<((*catalog).list).size(); i++){
        bodies.Add((((*catalog).list)[i]).name.value.c_str());
    }
    wxStaticText* text_combo_body = new wxStaticText(panel, wxID_ANY, wxT("Celestial body"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    body = new BodyField(this, &(sight->body), catalog);
        
    wxStaticText* text_limb = new wxStaticText(panel, wxID_ANY, wxT("Limb"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //    combo_limb = new wxComboBox(panel, ID_combo_limb, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    limb = new LimbField(this, &(sight->limb));
    //    (limb->name)->SetValue("");
    //    (limb->name)->Enable(false);
    
    //sextant altitude
    wxStaticText* text_H_s = new wxStaticText(panel, wxID_ANY, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    H_s = new AngleField(this, &(sight->H_s));
    
    //index error
    wxStaticText* text_index_error = new wxStaticText(panel, wxID_ANY, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //If sight_in = NULL, read index error from init file
    if(sight_in == NULL){
        cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
        (sight->index_error).read_from_file(String("index error"), file_init, true, new_prefix);
        (sight->index_error).to_deg_min(&deg, &min);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
    }
    index_error = new AngleField(this, &(sight->index_error));
    index_error->set();
    
    //artificial horizon
    wxStaticText* text_artificial_horizon_check = new wxStaticText(panel, wxID_ANY, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    artificial_horizon_check = new CheckField(this, &(sight->artificial_horizon));
    
    //height of eye
    wxStaticText* text_height_of_eye = new wxStaticText(panel, wxID_ANY, wxT("Height of eye"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    height_of_eye = new LengthField(this, &(sight->height_of_eye));
    
    //master-clock date
    //sets  sight.master_clock_date_and_hour.date and sight.time.date to the current UTC date if this constructor has been called with sight_in = NULL
    if(sight_in == NULL){
        (sight->master_clock_date_and_hour).date.set_current(prefix);
        (sight->master_clock_date_and_hour).chrono.set_current(prefix);
        (sight->time).date.set_current(prefix);
        (sight->time).chrono.set_current(prefix);
    }
    wxStaticText* text_date = new wxStaticText(panel, wxID_ANY, wxT("Master-clock UTC date and hour of sight"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_date = new DateField(this, &(sight->master_clock_date_and_hour.date));
    master_clock_date->set();
    
    //master-clock hour
    wxStaticText* text_space_1 = new wxStaticText(panel, wxID_ANY, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_chrono = new ChronoField(this, &(sight->master_clock_date_and_hour.chrono));
    
    //if sight_in != NULL, then I initialize the GUI filed master_clock_chrono with the one written in sight_in. Otherwise, master_clock_chrono is initialized as empty.
    if(sight_in!=NULL){
        
//        Time time_UTC;
//
//        //(sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
//        time_UTC = (sight->time);
//        time_UTC -= (sight->TAI_minus_UTC);
        master_clock_chrono->set(sight->master_clock_date_and_hour.chrono);
        
    }else{
        //if sight_in == NULL, I have previously set the non-GUI object (sight->master_clock_date_and_hour).chrono to the current hour, and I write this value into the GUI object master_clock_chrono
        
        master_clock_chrono->set((sight->master_clock_date_and_hour).chrono);
        
    }
    
    
    //check/uncheck stopwatch
    wxStaticText* text_stopwatch_check = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    stopwatch_check = new CheckField(this, &(sight->use_stopwatch));
    (check_stopwatch.p) = stopwatch_check;
    (stopwatch_check->checkbox)->Bind(wxEVT_CHECKBOX, check_stopwatch);
    
    //stopwatch reading
    wxStaticText* text_stopwatch_reading = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //    stopwatch_reading = new ChronoField(this, &(sight.stopwatch));
    stopwatch_reading = new ChronoField(this, &(sight->stopwatch));
    
    //initialize stopwatch_check and stopwatch_reading
    (stopwatch_check->checkbox)->SetValue(false);
    stopwatch_reading->Enable(false);
    //    (stopwatch_reading->hour)->SetValue(wxString("0"));
    //    (stopwatch_reading->minute)->SetValue(wxString("0"));
    //    (stopwatch_reading->second)->SetValue(wxString("0.0"));
    
    wxStaticText* text_TAI_minus_UTC = new wxStaticText(panel, wxID_ANY, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    TAI_minus_UTC = new ChronoField(this, &(sight->TAI_minus_UTC));
    TAI_minus_UTC->set(sight->TAI_minus_UTC);

    //label
    wxStaticText* text_label = new wxStaticText(panel, wxID_ANY, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    label = new StringField(this, &(sight->label));
    
    
    //buttons
    button_cancel = new wxButton(panel, ID_button_cancel, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_reduce = new wxButton(panel, ID_button_reduce, "Reduce", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_reduce->Bind(wxEVT_BUTTON, setlabeltocurrenttime);
    
    //If I press reduce, I want all the fields in this SightFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_reduce->Bind(wxEVT_BUTTON, (limb->check));
    button_reduce->Bind(wxEVT_BUTTON, (body->check));
    button_reduce->Bind(wxEVT_BUTTON, (index_error->check));
    button_reduce->Bind(wxEVT_BUTTON, (H_s->check));
    button_reduce->Bind(wxEVT_BUTTON, (height_of_eye->check));
    button_reduce->Bind(wxEVT_BUTTON, (master_clock_date->check));
    button_reduce->Bind(wxEVT_BUTTON, check_stopwatch);
    button_reduce->Bind(wxEVT_BUTTON, (label->check)        );


    //I enable the reduce button only if sight_in is a valid sight with the entries propely filled, i.e., only if sight_in != NULL
    button_reduce->Enable((sight_in != NULL));
    
    sizer_grid_measurement->Add(text_combo_body);
    body->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    //    sizer_grid_measurement->Add(combo_body);
    
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
    
    sizer_grid_label->Add(text_label);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);
    
    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer_2->Add(button_reduce, 0, wxALIGN_BOTTOM);
    
    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Measurement");
    sizer_box_time = new wxStaticBoxSizer(wxVERTICAL, panel, "Time");
    
    sizer_box_measurement->Add(sizer_grid_measurement);
    sizer_box_time->Add(sizer_grid_time);
    
    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Master-clock UTC date and hour of sight"
    common_width = GetTextExtent(wxS("Master-clock UTC date and hour of sight   ")).GetWidth();
    text_combo_body->SetMinSize(wxSize(common_width,-1));
    text_date->SetMinSize(wxSize(common_width,-1));
    text_label->SetMinSize(wxSize(common_width,-1));
    
    //add the various elements to sizer, by inserting a border of 5 in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_box_time, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, 5);
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, 5);
    
    
    //panel->SetSizer(sizer);
    Maximize(panel);
    
    CurrentDocPath = wxT("");
    
    CreateStatusBar();
    SetStatusText( "Welcome to Michele's text editor!" );
    
    SetSizerAndFit(sizer);
    //Maximize();
    
    file_init.close(prefix);
    
    if(!check){
        cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
    }
    
    /*
     //here I read a sample sight from file_sample_sight, store into sight and set all the fields in this to the data in sight with set()
     File file_sample_sight;
     file_sample_sight.set_name(String("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample_sight.txt"));
     file_sample_sight.open(String("in"), String(""));
     sight->read_from_file(file_sample_sight, String(""));
     file_sample_sight.close(String(""));
     set();
     */
    
    if(sight_in != NULL){set();}
    
}


MessageFrame::MessageFrame(wxWindow* parent, const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent, wxID_ANY, title, pos, size){
    
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_grid = new wxGridSizer(3, 1, 0, 0);
    
    
    wxStaticText* text = new wxStaticText(panel, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //buttons
    button_ok = new wxButton(panel, wxID_ANY, "Ok!", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_ok->Bind(wxEVT_BUTTON, &MessageFrame::OnPressOk, this);
    
    
    //image
    wxPNGHandler *handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*1./1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*1./1000.0);
    
    
    
    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/jolly_rogers_png.png", wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    
    
    sizer_grid->Add(text, 0, wxALIGN_CENTER);
    sizer_grid->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    sizer_grid->Add(button_ok, 0, wxALIGN_CENTER);
    
    
    sizer_h->Add(sizer_grid, 0, wxALIGN_CENTER_VERTICAL);
    sizer_v->Add(sizer_h, 0, wxALIGN_CENTER);
    //  Maximize(panel);
    
    sizer_v->Fit(panel);
    panel->SetSizer(sizer_v);
    
    //    SetSize(wxSize(100,100));
    
    //    CreateStatusBar();
    //    SetStatusText( "Welcome to Michele's text editor!" );
    
    //SetSizerAndFit(sizer_v);
    //Maximize();
    
    Centre();
    
    
}

PlotFrame::PlotFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(NULL, wxID_ANY, title, pos, size){
    
    unsigned int i;
    OnSelectInListBox onselectinlistbox;
    wxListItem column, item;
    
    
    (onselectinlistbox.f) = this;
    
    catalog = new Catalog(String(path_file_catalog), String(""));
    plot = new Plot(catalog, String(""));
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    
    
    //
    //here I read a sample sight from file_sample_sight, store into sight and set all the fields in this to the data in sight with set()
    File file_sample_sight;
    file_sample_sight.set_name(String("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample_sight.txt"));
    plot->read_from_file(file_sample_sight, String(""));
    plot->print(true, String(""), cout);
    //
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    //    listbox = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxSize(400,200));
    //    listbox->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, onselectinlistbox);
    //    //append the elements in plot->sight_list to listbox
    //    for(i=0; i<(plot->sight_list).size(); i++){
    //        listbox->Append(wxString(((plot->sight_list)[i]).label.value));
    //    }
    //
    //add columns to wxlistcontrol
    listcontrol = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize((this->GetSize()).GetWidth()*0.95 ,  (this->GetSize()).GetHeight()*0.8), wxLC_REPORT);
    listcontrol->Bind(wxEVT_LIST_ITEM_SELECTED, onselectinlistbox);
    
    
    int n_columns = 11;
    
    column.SetId(0);
    column.SetText(wxT("Body"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(0, column);
    
    column.SetId(1);
    column.SetText(wxT("Limb"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(1, column);
    
    column.SetId(2);
    column.SetText(wxT("Artificial horizon"));
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    column.SetAlign(wxLIST_FORMAT_LEFT);
    listcontrol->InsertColumn(2, column);
    
    column.SetId(3);
    column.SetText(wxT("Sextant altitude"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(3, column);
    
    column.SetId(4);
    column.SetText(wxT("Index error"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(4, column);
    
    column.SetId(5);
    column.SetText(wxT("Height of eye"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(5, column);
    
    column.SetId(6);
    column.SetText(wxT("Master-clock date and hour (UTC)"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(6, column);
    
    column.SetId(7);
    column.SetText(wxT("Stopwatch"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(7, column);
    
    column.SetId(8);
    column.SetText(wxT("Stopwatch reading"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(8, column);
    
    column.SetId(9);
    column.SetText(wxT("TAI - UTC"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(9, column);
    
    column.SetId(10);
    column.SetText(wxT("Label"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol->GetSize()).GetWidth()/n_columns);
    listcontrol->InsertColumn(10, column);
    
    
    //
    for(i=0; i<((plot->sight_list).size()); i++){
        
        ((plot->sight_list)[i]).add_to_wxListCtrl(-1, listcontrol);
                
    }
    
    
    //buttons
    //button to add a sight
    button_add = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_add->Bind(wxEVT_BUTTON, &PlotFrame::OnAdd, this);
    
    //button to modify a sight
    wxImage::AddHandler(new wxPNGHandler);
    wxBitmap my_bitmap = wxBitmap(wxT("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/pencil_icon.png"), wxBITMAP_TYPE_PNG);
    wxImage my_image = my_bitmap.ConvertToImage();
    my_image.Rescale(20,20);
    button_modify = new wxBitmapButton(panel, wxID_ANY, wxBitmap(my_image), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT   | wxBORDER_NONE);
    button_modify->Bind(wxEVT_BUTTON, &PlotFrame::OnModify, this);
    button_modify->Enable(false);
    //    button_modify->Enable(false);
    
    //button to delete a sight
    button_delete = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_delete->Bind(wxEVT_BUTTON, &PlotFrame::OnDelete, this);
    
    
    sizer_buttons->Add(button_add, 0, wxALIGN_CENTER);
    sizer_buttons->Add(button_modify, 0, wxALIGN_CENTER);
    sizer_buttons->Add(button_delete, 0, wxALIGN_CENTER);
    //
    
    //resize uniformly all column
    //    for(i=0; i<(listcontrol->GetColumnCount()); ++i){
    //        listcontrol->SetColumnWidth(i, ((listcontrol->GetSize()).GetWidth())/(listcontrol->GetColumnCount()));
    //    }
    sizer_v->Add(listcontrol, 1, wxEXPAND | wxALL, 10);
    sizer_v->Add(sizer_buttons, 0, wxALIGN_LEFT | wxALL, 5);
    //    sizer_v->Add(button_modify, 0,  wxALIGN_LEFT | wxALL, 5);
    //    sizer_v->Add(button_delete, 0, wxALIGN_LEFT | wxALL, 5);
    //    sizer_h->Add(listcontrol, 0, wxALIGN_TOP);
    
    panel->SetSizer(sizer_v);
    
}

void PlotFrame::OnAdd(wxCommandEvent& event){

    SightFrame *sight_frame = new SightFrame(this, NULL, -1, "New sight", wxDefaultPosition, wxDefaultSize, String(""));
    sight_frame->Show(true);

    event.Skip(true);

}

void PlotFrame::OnModify(wxCommandEvent& event){
    
    long item;
    item = listcontrol->GetNextItem(-1,
                                    wxLIST_NEXT_ALL,
                                    wxLIST_STATE_SELECTED);
    
    if(item != -1){
        
        stringstream s;
        
        s.str("");
        s << "Sight #" << item;
        
        SightFrame *sight_frame = new SightFrame(this, &((plot->sight_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
//        (sight_frame->sight) = &((plot->sight_list)[item]);
        sight_frame->Show(true);
        
        
    }


    event.Skip(true);

}




void PlotFrame::OnDelete(wxCommandEvent& event){

    long item;
    
    item = listcontrol->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    listcontrol->DeleteItem(item);
    
    event.Skip(true);
 
}


//set all the GUI fields in this equal to those in the non-GUI object this->sight
void SightFrame::set(void){
    
    Time time_UTC;

    body->set();
    limb->set();
    artificial_horizon_check->set();
    H_s->set();
    index_error->set();
    
    if(!((artificial_horizon_check->checkbox)->GetValue())){
        height_of_eye->Enable(true);
        height_of_eye->set();
    }else{
        height_of_eye->Enable(false);
    }
    
    master_clock_date->set();
    
    //(sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    time_UTC = (sight->time);
    time_UTC -= (sight->TAI_minus_UTC);
    master_clock_chrono->set(time_UTC.chrono);
    
    stopwatch_check->set();
    
    if(((stopwatch_check->checkbox)->GetValue())){
        stopwatch_reading->Enable(true);
        stopwatch_reading->set(sight->stopwatch);
    }else{
        stopwatch_reading->Enable(false);
    }
    
    TAI_minus_UTC->set(sight->TAI_minus_UTC);
    label->set();
    
}

void SightFrame::SetIdling(bool b){
    
    idling = b;
    
}

void SightFrame::OnClose(wxCommandEvent& event){
    
    CurrentDocPath = wxT("");
    SetTitle(_("untitled"));
    
}

void SightFrame::OnOpen(wxCommandEvent& event){
    
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

void SightFrame::OnSaveAs(wxCommandEvent& event){
    
    
    wxFileDialog *SaveDialog = new wxFileDialog(this, _("Save File As _?"), wxEmptyString, wxEmptyString, _("Text files (*.txt)|*.txt|C++ Source Files (*.cpp)|*.cpp| C Source files (*.c)|*.c|C header files (*.h)|*.h"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
    
    // Creates a "save file" dialog with 4 file types
    if(SaveDialog->ShowModal() == wxID_OK) {
        
        CurrentDocPath = SaveDialog->GetPath();
        //        combo_H_s_deg->SaveFile(CurrentDocPath);
        
        SetTitle(SaveDialog->GetFilename());
        
    }
    
    SaveDialog->Destroy();
    
    
}

//this function checks whether all the fields in SightFrame are ok, and if they are, it enables the button_reduce
void SightFrame::TryToEnableReduce(void){
    
    
    button_reduce->Enable((body->is_ok()) && ((((artificial_horizon_check->checkbox)->GetValue())) || (height_of_eye->is_ok())) && ((!(((limb->name)->IsEnabled()))) || (limb->is_ok())) && (H_s->is_ok()) && (index_error->is_ok()) && (master_clock_date->is_ok()) && (master_clock_chrono->is_ok()) && ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) && (TAI_minus_UTC->is_ok()));
    
}


void SightFrame::OnSave(wxCommandEvent& event){
    
    if(CurrentDocPath == ""){
        OnSaveAs(event);
    }else{
        //        combo_H_s_deg->SaveFile(CurrentDocPath);
    }
    
}


void SightFrame::OnPressCancel(wxCommandEvent& event){
    
    
    Close(TRUE);
}









void CheckHour::operator()(wxFocusEvent &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->hour), String("Entered value is not valid!\nHours must be unsigned integer numbers >= 0 and < 24"));
            
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
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}


void CheckMinute::operator()(wxFocusEvent &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->minute), String("Entered value is not valid!\nMinutes must be unsigned integer numbers >= 0 and < 60"));
            
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
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}



void CheckSecond::operator()(wxFocusEvent &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
            
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->second, String("Entered value is not valid!\nSeconds must be floating-point numbers >= 0.0 and < 60.0"));
            
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
        
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}



template<class T> void CheckYear::operator()(T&event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->year)->GetValue()).ToStdString(), NULL, false, 0, 0)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->year, String("Entered value is not valid!\nYear must be an unsigned integer"));
            
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
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);

}

template<class T> void CheckMonth::operator()(T&event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->month)->GetValue()).ToStdString(), NULL, true, 1, 12+1)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->month, String("Entered value is not valid!\nMonth must be an unsigned integer >= 1 and <= 12"));
            
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
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}


template<class T> void CheckDay::operator()(T& event){
    
    SightFrame* f = (p->parent_frame);
    
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
                
                //            f->CallAfter(&SightFrame::PrintErrorMessage, p->day, String("Entered value is not valid!\nDay must be an unsigned integer comprised between the days of the relative month"));
                
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
        
        f->TryToEnableReduce();
                
    }
    
    event.Skip(true);
    
}






template<class T> void TabulateDays::operator()(T& event){
    
    unsigned int i;
    SightFrame* f = (p->parent_frame);
    
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
template<class T> void CheckCheck::operator()(T& event){
    
    SightFrame* f = (p->parent_frame);
    
    //I set p->answer to the value entered in the GUI checkbox
    if((p->checkbox)->GetValue()){
        ((p->answer)->value) = 'y';
        (f->height_of_eye)->Enable(false);
    }else{
        ((p->answer)->value) = 'n';
        (f->height_of_eye)->Enable(true);
    }
    
    event.Skip(true);
    
}

//this function writes into sight.use_stopwatch the value written into the respective GUI box and it enables/disables all fields in stopwatch reading if stopwatch_check is enabled/disabled, respectively
template <class T> void CheckChrono::operator()(T& event){
    
    SightFrame* f = (p->parent_frame);
    
    //I set p->answetr to the value entered in the GUI checkbox
    if((p->checkbox)->GetValue()){
        ((p->answer)->value) = 'y';
    }else{
        ((p->answer)->value) = 'n';
    }
    
    //I enable f->stopwatch reading GUI field a
    (f->stopwatch_reading)->Enable((p->checkbox)->GetValue());
    
    event.Skip(true);

}



void SightFrame::OnPressReduce(wxCommandEvent& event){
    
    stringstream s;
    
    sight->print(String("body entered via GUI"), String(""), cout);
    
    //if the constructor of SightFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of sight_list
    if(position==-1){
        (((this->parent)->plot)->sight_list).push_back(*sight);
    }
    
    sight->add_to_wxListCtrl(position, ((this->parent)->listcontrol));
    
    event.Skip(true);
    
    Close(TRUE);

}


//constructor of a BodyField object, based on the parent frame frame
BodyField::BodyField(SightFrame* frame, Body* p, Catalog* c){
    
    unsigned int i;
    parent_frame = frame;
    //I link the internal pointers p and c to the respective body and body catalog
    body = p;
    catalog = c;
    
    for(bodies.Clear(), i=0; i<(catalog->list).size(); i++){
        bodies.Add(((catalog->list)[i]).name.value.c_str());
    }
    
    (check.p) = this;
    
    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, bodies, wxCB_DROPDOWN);
    //name->SetInitialSize(name->GetSizeFromTextSize(name->GetTextExtent(wxS("000"))));
    //name->SetValue("");
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, check);
    
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
        checkbox->SetValue(true);
    }
    
    if((answer->value) == 'n'){
        checkbox->SetValue(false);
    }
    
}

//sets the value in the GUI objects deg and min equal to the value in the non-GUI limb object angle
void AngleField::set(void){
    
    unsigned int deg_temp;
    double min_temp;
    
    angle->to_deg_min(&deg_temp, &min_temp);
    
    sign->SetValue(wxString("+"));
    deg->SetValue(wxString::Format(wxT("%i"), deg_temp));
    min->SetValue(wxString::Format(wxT("%f"), min_temp));
    
    sign_ok = true; 
    deg_ok = true;
    min_ok = true;
    
}


//sets the value in the GUI object value equal to the value in the non-GUI  object length
void LengthField::set(void){
    
    value->SetValue(wxString::Format(wxT("%f"), /*I convert the lenght from nm to meters*/(length->value)*1e3*nm));
    
    ok = true;
    
}

//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date
void DateField::set(void){
    
    Time time_UTC;

    //((parent_frame->sight)->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    time_UTC = ((parent_frame->sight)->time);
    time_UTC -= ((parent_frame->sight)->TAI_minus_UTC);
    
    year->SetValue(wxString::Format(wxT("%i"), (time_UTC.date).Y));
    month->SetValue(wxString::Format(wxT("%i"), (time_UTC.date).M));
    day->SetValue(wxString::Format(wxT("%i"), (time_UTC.date).D));
    
    year_ok = true;
    month_ok = true;
    day_ok = true;
    
}

//sets the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object chrono_in
void ChronoField::set(Chrono chrono_in){
    
    hour->SetValue(wxString::Format(wxT("%i"), chrono_in.h));
    minute->SetValue(wxString::Format(wxT("%i"), chrono_in.m));
    second->SetValue(wxString::Format(wxT("%f"), chrono_in.s));
    
    hour_ok = true;
    minute_ok = true;
    second_ok = true;
    
}

//sets the value in the GUI object value equal to the value in the non-GUI String object string
void StringField::set(void){
    
    value->SetValue(wxString(string->value));
    
}



//constructor of a LimbField object, based on the parent frame frame
LimbField::LimbField(SightFrame* frame, Limb* p){
    
    parent_frame = frame;
    //I link the internal pointers p the respective Limb object
    limb = p;
    
    //initialize check
    (check.p) = this;
      
    limbs.Clear();
    limbs.Add(wxT("upper"));
    limbs.Add(wxT("lower"));
    limbs.Add(wxT("center"));
    
    
    
    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    //name->SetInitialSize(name->GetSizeFromTextSize(name->GetTextExtent(wxS("000"))));
    //name->SetValue("");
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, check);
    
//    body->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    
    name->SetValue(wxString(""));
    ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);
    
}



//constructor of a CheckField object, based on the parent frame frame
CheckField::CheckField(SightFrame* frame, Answer* p){
    
    parent_frame = frame;
    //I link the internal pointers p and c to the respective Answer object
    answer = p;
    
    (check.p) = this;
    
    checkbox = new wxCheckBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    checkbox->Bind(wxEVT_CHECKBOX, check);

    checkbox->SetValue(false);
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(checkbox, 0, wxALIGN_CENTER);
    
}



//constructor of an AngleField object, based on the parent frame frame
AngleField::AngleField(SightFrame* frame, Angle* p){
    
    unsigned int i;
    parent_frame = frame;
    angle = p;
    
    //tabulate signs and degrees wxArrayStrings
    signs.Clear();
    signs.Add(wxString("+"));
    signs.Add(wxString("-"));
    
    for(degrees.Clear(), i=0; i<360; i++){
        degrees.Add(wxString::Format(wxT("%i"), i));
    }
    
    //initialize check and its objects
    (check.p) = this;
    ((check.check_sign).p) = this;
    ((check.check_arc_degree).p) = this;
    ((check.check_arc_minute).p) = this;
    
    sign = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
    AdjustWidth(sign);
    sign->Bind(wxEVT_KILL_FOCUS, (check.check_sign));
    
    deg = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    AdjustWidth(deg);
    deg->Bind(wxEVT_KILL_FOCUS, (check.check_arc_degree));
    
    text_deg = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("° "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    min = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS(sample_width_floating_point_field))));
    min->Bind(wxEVT_KILL_FOCUS, (check.check_arc_minute));
    
    text_min = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("' "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    sign->SetValue(wxString(""));
    deg->SetValue(wxString(""));
    min->SetValue(wxString(""));
    sign_ok = false;
    deg_ok = false;
    min_ok = false;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(sign, 0, wxALIGN_CENTER);
    sizer_h->Add(deg, 0, wxALIGN_CENTER);
    sizer_h->Add(text_deg);
    sizer_h->Add(min, 0, wxALIGN_CENTER);
    sizer_h->Add(text_min);
    
}

//constructor of a LengthField object, based on the parent frame frame
LengthField::LengthField(SightFrame* frame, Length* p){
    
    parent_frame = frame;
    length = p;
    
//    ((parent_frame->check_height_of_eye).p) = this;
    
    //initialize check
    (check.p) = this;
   
    
    
    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, check);
    
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


//constructor of a StringField object, based on the parent frame frame
StringField::StringField(SightFrame* frame, String* p){
    
    parent_frame = frame;
    string = p;
    
    //initialize check
    (check.p) = this;
    
    ((parent_frame->setlabeltocurrenttime).p) = this;
    
    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_string_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, check);
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);
    
}




bool AngleField::is_ok(void){
    
    return(sign_ok && deg_ok && min_ok);
    
}

bool LengthField::is_ok(void){
    
    return(ok);
    
}

bool ChronoField::is_ok(void){
    
    return(hour_ok && minute_ok && second_ok);
    
}

//constructor of a DateField object, based on the parent frame frame
DateField::DateField(SightFrame* frame, Date* p){
    
    unsigned int i;
    parent_frame = frame;
    date = p;
    
 
    //initialize check and its objects
    (check.p) = this;
    ((check.check_year).p) = this;
    ((check.check_month).p) = this;
    ((check.check_day).p) = this;
  
    ((parent_frame->tabulatedays).p) = this;
    
    for(months.Clear(), months.Add(wxT("")), i=0; i<12; i++){
        months.Add(wxString::Format(wxT("%i"), i+1));
    }
    
    year = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    year->SetInitialSize(year->GetSizeFromTextSize(year->GetTextExtent(wxS("0000"))));
    year->Bind(wxEVT_KILL_FOCUS, (check.check_year));
    
    text_hyphen_1 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
    
    month = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN);
    //    month->SetInitialSize(month->GetSizeFromTextSize(month->GetTextExtent(wxS("00"))));
    AdjustWidth(month);
    month->Bind(wxEVT_KILL_FOCUS, (check.check_month));
    
    text_hyphen_2 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
    
    
    day = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN);
    //I create a temporary days list to set the size of the wxComboBox day with AdjustWidth, and then destroy this temporary days list
    for(days.Clear(), i=0; i<31; i++){
        days.Add(wxString::Format(wxT("%i"), i+1));
    }
    day->Set(days);
    AdjustWidth(day);
    days.Clear();
    day->Bind(wxEVT_KILL_FOCUS, (check.check_day));
    
    
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
ChronoField::ChronoField(SightFrame* frame, Chrono* p){
    
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

//this functor quits the MessageFrame when Ok button is pressed
void MessageFrame::OnPressOk(wxCommandEvent& event){
    
    
    Close(TRUE);
    
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

template<class T> void StringField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void ChronoField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}
