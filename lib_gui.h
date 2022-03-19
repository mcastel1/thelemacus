//maximal and minimal latitude of the points in file define path_file_coastlines
#define max_lat (83.6664731)
#define min_lat (-78.7290778)
#define floor_min_lat (floor(min_lat))
#define floor_max_lat (floor(max_lat))
//latitude span
#define span_lat ((floor_max_lat-floor_min_lat+1)
//the ratio between the width (height) of the plot area and the width (height) of the chart.
#define length_plot_area_over_length_chart 0.8
#define length_chart_over_length_chart_frame 0.8
//this is the ratio between (the length of the borders drawn around the widgets) and (the length of the frame in which the widgets are located)
#define length_border_over_length_frame 0.01
#define outfile_precision 16
//the maximum allowed value of the slider in ChartFrame
#define value_slider_max 1000
//#define wxDEBUG_LEVEL 0


//this string defines the width of GUI fields hosting floating-point numbers
#define sample_width_floating_point_field "0.000000000"
#define sample_width_string_field "Et repellat optio nam iste voluptatum in magnam?"
#define path_file_app_icon "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/jolly_rogers_png.png"
#define path_file_chart "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/chart.png"
#define path_file_pencil_icon "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/pencil_icon.png"
#define path_file_coastlines "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_toy.csv"
#define path_file_coastline_data_blocked "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
#define path_file_n_line "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/n_line_map_conv_blocked.txt"
#define path_file_selected_coastline_data "/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt"





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

//class which defines a functor which closes a frame with arbitrary type F
template<class F> class CloseFrame{
    
public:
    
    //the frame to be closed
    F* frame;
    CloseFrame(F*);
    
    template<class T> void operator()(T&);
    
};

struct CheckBody{
    
    BodyField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckLimb{
    
    LimbField* p;
    
    template<class T> void operator()(T&);
    
    
    
};


template<class T> struct CheckCheck{
    
    CheckField<T>* p;
    
    //this functor checks whether a GUI Check field is filled correctly and writes its value into the relative non-GUI field
    template<class R> void operator()(R&);
    
};



template<class P> struct CheckSign{
    
    AngleField<P>* p;
    
    template <class T> void operator()(T&);
    
    
};

template<class P> struct CheckArcDegree{
    
    AngleField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

template<class P> struct CheckArcMinute{
    
    AngleField<P>* p;
    
    template <class T> void operator()(T&);
    
    
};


template<class P> class CheckAngle{
    
public:
    
    //p is the AngleField which is parent of the CheckAngle object: the CheckAngle object checks the validity of the entries in AngleField
    AngleField<P>* p;
    CheckSign<P> check_sign;
    CheckArcDegree<P> check_arc_degree;
    CheckArcMinute<P> check_arc_minute;
    
    CheckAngle(AngleField<P>*);
    
    template <class T> void operator()(T&);
    
};

struct TabulateDays{
    
    DateField* p;
    
    template<class T> void operator()(T&);
    
    
};


struct CheckYear{
    
    DateField* p;
    TabulateDays tabulate_days;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckMonth{
    
    DateField* p;
    TabulateDays tabulate_days;
    
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

template<class P> struct CheckString{
    
    StringField<P>* p;
    
    template<class T> void operator()(T&);
    
};

struct CheckHour{
    
    ChronoField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckMinute{
    
    ChronoField* p;
    
    template<class T> void operator()(T&);
    
    
};

struct CheckSecond{
    
    ChronoField* p;
    
    template<class T> void operator()(T&);
    
};


struct CheckChrono{
    
    ChronoField* p;
    CheckHour check_hour;
    CheckMinute check_minute;
    CheckSecond check_second;
    
    template<class T> void operator()(T&);
    
};


template<class P> struct SetStringToCurrentTime{
    
    StringField<P>* p;
    
    template<class T> void operator()(T&);
    
    
};

//this class defines the functor () used to remove a sight from the non-GUI object plot
class DeleteSight{
    
public:
    
    DeleteSight(Answer);
    
    //the frame which called this struct
    ListFrame* f;
    //the id of the sight to be removed
    long i_sight_to_remove;
    //this is equal to 'y' if the route related to the removed sight has to be removed too, and 'n' otherwise
    Answer remove_related_route;
    
    void operator()(wxCommandEvent&);
    
};


//this is a GUI field contaning a binary checkbox, which is either checked or unchecked
template<class T> class CheckField{
    
public:
    
    //the parent frame to which this object is attached
    SightFrame* parent_frame;
    Answer* answer;
    //related_field is a GUI field (such as ChronoField, etc) related to this CheckField, such that: if direct_reverse = true->  when the checkbox in this CheckFIeld is checked (unchecked), related_field is active (inactive). If direct_reverse = false ->  when the checkbox in this CheckFIeld is unchecked (checked), related_field is active (inactive).
    T* related_field;
    bool direct_reverse;
    wxBoxSizer *sizer_h, *sizer_v;
    
    //this is the wxCheckBox with the name of the bodies
    wxCheckBox* checkbox;
    CheckCheck<T> check;
    
    CheckField(SightFrame*, Answer*, T*, bool);
    
    template<class R> void InsertIn(R*);
    template<class S> void get(S&);
    void set(void);
    
    
};



class DrawPanel : public wxPanel{
    
public:
    DrawPanel(ChartPanel*);
    ChartFrame* parent;
    XYChart* c;
    wxPoint position_draw_panel, position_plot_area, position_start_selection, position_end_selection, position_screen_now, position_start_drag, position_end_drag, position_now_drag;
    wxSize size_plot_area;
    wxSlider* slider;
    /*x_min, x_max, y_min, y_max do correspond to lambda_min, lambda_max, etc... They are ordered in such a way that x_min <= x_max and y_min <= y_max always. */
    double x_min, x_max, y_min, y_max,
    /*these are the values of x_min .. y_max when the plot is first drawn*/
    x_min_0, x_max_0, y_min_0, y_max_0,
    /*these are the values of x_min, ... y_max after each sliding event, corresponding to lambda_min, ... , phi_max read from file*/x_min_old, x_max_old, y_min_old, y_max_old, /*this is the ratio between the length of the tics on both axes, and the width of the plot area*/tic_length_over_width_plot_area, /* gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes when setting the tics on the x axis, and similarly for gamma_phi*/gamma_lambda, gamma_phi, /*these are the angular separations in latitude and longitude between meridians and parallels, respectively */delta_lambda, delta_phi;
    wxStaticText*text_position_start, *text_position_end;
    bool selection_rectangle, /*this is true if the mouse is dragging with the left button pressed*/mouse_dragging;
    //these are the positions where the right mouse button is clicked at the beginning and at the end of the drawing process for the selection rectangle on the world's chart
    Position p_start, p_end;
    wxSizer* sizer_h, *sizer_v;
    //the chart contains the plot area, and the following quantities are the width and height of chart and plot area
    unsigned int width_chart, height_chart, /*these are the values of width/height_chart when the chart is first drawn*/width_chart_0, height_chart_0, width_plot_area, height_plot_area, tic_length;
    
    void Draw(void);
    void PaintEvent(wxPaintEvent & evt);
    void PaintNow();
    void ScreenToGeo(wxPoint, Position*);
    void GeoToScreen(Position, wxPoint*);
    void Update_lambda_phi_min_max(void);
    void Update_x_y_min_max(void);
    
    void Render(wxDC& dc);
    
    // some useful events
    void GetMouseGeoPosition(Position*);
    void OnMouseMovement(wxMouseEvent&);
    void OnMouseLeftDown(wxMouseEvent&);
    void OnMouseLeftUp(wxMouseEvent&);
    void OnMouseRightDown(wxMouseEvent&);
    void OnMouseDrag(wxMouseEvent&);
    void OnScroll(wxScrollEvent&);
    
    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
// some useful events
/*
 EVT_MOTION(DrawPanel::mouseMoved)
 EVT_LEFT_DOWN(DrawPanel::mouseDown)
 EVT_LEFT_UP(DrawPanel::mouseReleased)
 EVT_RIGHT_DOWN(DrawPanel::rightClick)
 EVT_LEAVE_WINDOW(DrawPanel::mouseLeftWindow)
 EVT_KEY_DOWN(DrawPanel::keyPressed)
 EVT_KEY_UP(DrawPanel::keyReleased)
 EVT_MOUSEWHEEL(DrawPanel::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(DrawPanel::PaintEvent)

END_EVENT_TABLE()


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
    template<class T> void get(T&);
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
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
    
};



//class for graphical object: a field to enter an angle, composed of a box for the sign, a box for the degrees, a degree text symbol, another box for minutes and a minute text symbol
template<class P> class AngleField{
    
public:
    //the parent frame to which this object is attached
    P* parent_frame;
    wxArrayString signs, degrees;
    //the format of the AngleField: String("") for angles  between 0 and 2*pi, String("+-") for angles with a sign between 0 and pi, String("NS") for latitudes between -pi/2 and pi/2, and String("EW") for longitudes between -pi and pi
    String format;
    //degrees and minutes boxes
    wxComboBox*sign, * deg;
    wxTextCtrl *min;
    //texts
    wxStaticText* text_deg, *text_min;
    wxBoxSizer *sizer_h, *sizer_v;
    Angle* angle;
    //deg_ok = true if the degrees part of this angle is formatted properly and set to the same value as the degree part of angle, and simiarly for min
    bool sign_ok, deg_ok, min_ok;
    CheckAngle<P>* check_angle;
    
    
    AngleField(P*, Angle*, String);
    void set(void);
    template<class T> void get(T&);
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
    bool ok, /*this variable = true if this has been just enabled, and false otherwise*/ just_enabled;
    CheckLength check;
    
    LengthField(SightFrame*, Length*);
    void set(void);
    template<class T> void get(T&);
    void Enable(bool);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};

//class for graphical object: a field to enter a String, composed of a box. P is the type of the object in which this StringField will be inserted
template<class P> class StringField{
    
public:
    //the parent where this StringField object will be inserted
    P* parent_frame;
    //label box
    wxTextCtrl *value;
    wxBoxSizer *sizer_h, *sizer_v;
    //non-GUI object related to this
    String* string;
    CheckString<P> check;
    SetStringToCurrentTime<P> set_string_to_current_time;
    
    StringField(P*, String*);
    void set(void);
    template<class T> void get(T&);
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
    template<class T> void get(T&);
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
    bool hour_ok, minute_ok, second_ok, /*this variable = true if this has been just enabled, and false otherwise*/just_enabled;
    CheckChrono check;
    
    ChronoField(SightFrame*, Chrono*);
    void set(Chrono);
    void Enable(bool);
    template<class T> void get(T&);
    template<class T> void InsertIn(T*);
    bool is_ok(void);
    
};









//this functor pops out an error-message window with title tile and error message message, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this struct adaptable
template<class T> class PrintErrorMessage{
    
public:
    
    T* f;
    wxControl* control;
    String title, message;
    
    PrintErrorMessage(T*);
    
    void operator()(void);
    
    
};



//this is a wxFrame designed to contain the list of sights, routes, etc...
class ListFrame: public wxFrame{
    
public:
    //this frame has no parent, because it is supposed to be the main frame of the appplication
    ListFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix);
    
    //this is a pointer to the non-GUI object Plot which is related to the GUI object this
    Plot* plot;
    //this is a pointer to a Catalog object which will be used by plot
    Catalog *catalog;
    //    wxListBox* listbox;
    wxListCtrl* listcontrol_sights, *listcontrol_positions;
    wxPanel *panel;
    wxButton *button_add_sight, *button_delete_sight, *button_add_position, *button_delete_position;
    wxBitmapButton *button_modify_sight, *button_modify_position;
    wxSizer* sizer_h, *sizer_v, *sizer_buttons_sight, *sizer_buttons_position;
    wxStaticBoxSizer* sizer_box_sight, *sizer_box_position;
    DeleteSight *delete_sight, *delete_sight_and_related_route;
    
    void OnAddSight(wxCommandEvent& event);
    void OnModifySight(wxCommandEvent& event);
    void OnPressDeleteSight(wxCommandEvent& event);
    
    void OnAddPosition(wxCommandEvent& event);
    void OnModifyPosition(wxCommandEvent& event);
    void OnDeletePosition(wxCommandEvent& event);
    
};



class SightFrame: public wxFrame{
    
public:
    SightFrame(ListFrame*, Sight*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Catalog* catalog;
    Sight* sight;
    //this long represents the position in the list (this->GetParent())->listcontrol_sights of sight. If list_position = -1, then sight is not in that list
    long list_position;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    PrintErrorMessage<SightFrame>* printerrormessage;
    
    BodyField* body;
    LimbField* limb;
    CheckField<LengthField>* artificial_horizon_check;
    CheckField<ChronoField>* stopwatch_check;
    AngleField<SightFrame>* H_s, *index_error;
    LengthField* height_of_eye;
    DateField *master_clock_date;
    ChronoField *master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    StringField<SightFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_time, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    wxStaticBoxSizer *sizer_box_measurement, *sizer_box_time;
    
    wxArrayString bodies, limbs;
    wxButton* button_reduce, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void TryToEnableReduce(void);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

class PositionFrame: public wxFrame{
    
public:
    PositionFrame(ListFrame*, Position*, long, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    Position* position;
    //this long represents the position in the list (this->GetParent())->listcontrol_positions of position. If list_position = -1, then sight is not in that list
    long list_position;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    //these are the functors needed to check whether arcdegrees and arcminutes are entered in the right format
    PrintErrorMessage<PositionFrame>* printerrormessage;
    
    AngleField<PositionFrame>* lat, *lon;
    StringField<PositionFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2;
    wxStaticBoxSizer *sizer_box_measurement;
    
    wxButton* button_add, *button_cancel;
    wxMenuBar *menuBar;
    
    void SetIdling(bool);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressAdd(wxCommandEvent& event);
    void TryToEnableReduce(void);
    
    // The Path to the file we have open
    wxString CurrentDocPath;
    
    //    wxDECLARE_EVENT_TABLE();
    
};


class ChartFrame: public wxFrame{
    
public:
    ChartFrame(ListFrame*, const wxString&, const wxPoint&, const wxSize&, String);
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    wxStaticText *text_position_now, *text_slider;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxStaticBitmap* image;
    wxDisplay display;
    wxRect rectangle_display;
    TextBox* box;
    wxSlider* slider;
    PrintErrorMessage<ChartFrame>* print_error_message;
    //this variable is true if the user has started drawing a selection rectangle on image, by right-clicking on image and thus forming one of the corners of the rectangle, and zero otherwise.
    unsigned int /*this stores the value of slider*/value_slider_old;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    void GetCoastLineData(void);
    bool UpdateSlider(void);
    void UpdateSliderLabel(void);
    void SetIdling(bool);
    
};

class ChartPanel : public wxPanel{
    
public:
    
    ChartFrame* parent;
    
    ChartPanel(ChartFrame*, const wxPoint&, const wxSize&);
    
};

ChartPanel::ChartPanel(ChartFrame* parent_in, const wxPoint& position, const wxSize& size) : wxPanel(parent_in, wxID_ANY, position, size, wxTAB_TRAVERSAL, wxT("")){
    
    parent = parent_in;
    
}


//this function efficiently reads coastline data stored in path_file_coastline_data_blocked from latitudes p to P and longitudes l to L, and writes this data into path_file_selected_coastline_data, writing n_points points at the most
void ChartFrame::GetCoastLineData(void){
    
    File file_n_line, file_coastline_data_blocked, outfile_selected_coastline_data;
    string data, line;
    stringstream ins;
    int i, j, i_min = 0, i_max = 0, j_min = 0, j_max = 0, j_normalized = 0, lambda_min_int, lambda_max_int, phi_min_int, phi_max_int;
    //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
    vector<unsigned int> n_line(360*(floor_max_lat-floor_min_lat+1));
    unsigned int l, n = 0, every = 0, n_points_grid = 0;
    char* buffer = NULL;
    size_t pos_beg, pos_end;
    bool check;
    
    //transform the values phi_min_int, phi_max_int in a format appropriate for GetCoastLineData: normalize the minimal and maximal latitudes in such a way that they lie in the interval [-pi, pi], because this is the format which is taken by GetCoastLineData
    ((parent->plot)->phi_min).normalize_pm_pi();
    ((parent->plot)->phi_max).normalize_pm_pi();
    
    lambda_min_int = ceil(K*(((parent->plot)->lambda_min).value));
    lambda_max_int = floor(K*(((parent->plot)->lambda_max).value));
    phi_min_int = floor(K*(((parent->plot)->phi_min).value));
    phi_max_int = ceil(K*(((parent->plot)->phi_max).value));
    
    //transform the values lambda_min_int, lambda_max_int in a format appropriate for GetCoastLineData
    if((lambda_min_int < 180) && (lambda_max_int >= 180)){
        j_min = lambda_max_int - 360;
        j_max = lambda_min_int;
    }else{
        j_min = lambda_max_int;
        j_max = lambda_min_int;
    }
    
    i_min = phi_min_int - floor_min_lat;
    i_max = phi_max_int - floor_min_lat;
    
    cout << "\n\n\n\n\nCoordinates: " << phi_min_int << " " << phi_max_int << " " << lambda_min_int << " " << lambda_max_int << "\n";
    
    
    n_points_grid = (i_max - i_min + 1 ) * (j_max - j_min + 1);
    
    file_n_line.set_name(String(path_file_n_line));
    file_coastline_data_blocked.set_name(String(path_file_coastline_data_blocked));
    outfile_selected_coastline_data.set_name(String(path_file_selected_coastline_data));
    
    
    //read file n_line and store it into vector n_line
    file_n_line.open(String("in"), String(""));
    i=0;
    while(!(file_n_line.value.eof())){
        
        line.clear();
        ins.clear();
        
        getline(file_n_line.value, line);
        ins << line;
        ins >> (n_line[i++]);
        
        //cout << "\nn_line[" << i-1 << "] = " << n_line[i-1];
        
    }
    file_n_line.close(String(""));
    
    
    
    //read in map_conv_blocked.csv the points with i_min <= latitude <= i_max, and j_min <= longitude <= j_max
    file_coastline_data_blocked.open(String("in"), String(""));
    //open a new file selected coastline data and write into it the new data
    outfile_selected_coastline_data.remove(String(""));
    outfile_selected_coastline_data.open(String("out"), String(""));
    
    check = true;
    for(i=i_min; i<=i_max; i++){
        
        for(j=j_min; j<=j_max; j++){
            
            j_normalized = (j % 360);
            
            // read data as a block:
            file_coastline_data_blocked.value.seekg(n_line[360*i+j_normalized], file_coastline_data_blocked.value.beg);
            
            l = n_line[360*i+j_normalized + 1] - n_line[360*i+j_normalized] - 1;
            if(buffer != NULL){delete [] buffer;}
            buffer = new char [l];
            
            file_coastline_data_blocked.value.read(buffer, l);
            string data(buffer, l);
            
            if(!(file_coastline_data_blocked.value)){
                
                check = false;
                
            }
            
            //count how many datapoints are in data
            n = count(data.begin(), data.end(), ',');
            
            every = (unsigned int)(((double)n)/((double)(((parent->plot)->n_points_plot_coastline).value))*((double)n_points_grid));
            if(every == 0){every = 1;}
            
            l=0;
            pos_beg = 0;
            pos_end = data.find(" ", pos_beg);
            while(pos_end != (string::npos)){
                
                //I write points in data to outfile_selected_coastline_data in such a way to write (((parent->plot)->n_points_coastline).value) points to the most
                if((l % every) == 0){
                    
                    line.clear();
                    line = data.substr(pos_beg, pos_end - pos_beg + 1).c_str();
                    
                    replace(line.begin(), line.end(), ' ', '\n');
                    replace(line.begin(), line.end(), ',', ' ');
                    
                    (outfile_selected_coastline_data.value) << line;
                    
                }
                
                pos_beg = pos_end+1;
                pos_end = data.find(" ", pos_beg);
                
                l++;
                
            };
            
            
            data.clear();
            
        }
        
    }
    
    if(check){
        
        cout << "All characters read successfully\n";
        
    }else{
        
        cout << RED << "Error: only " << file_coastline_data_blocked.value.gcount() << " characters could be read\n" << RESET;
        
    }
    
    
    outfile_selected_coastline_data.close(String(""));
    file_coastline_data_blocked.close(String(""));
    
    
}


void ChartFrame::SetIdling(bool b){
    
    idling = b;
    
}

DrawPanel::DrawPanel(ChartPanel* parent_in) : wxPanel(parent_in){
    
    //when the DrawPan is created there is no open selection rectangle and the mouse is not being dragged.
    selection_rectangle = false;
    mouse_dragging = false;
    
    parent = (parent_in->parent);
    
    SetCursor(*wxCROSS_CURSOR);
    tic_length_over_width_plot_area = 0.01;
    
    
    //    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    
    //text for the coordinates of the mouse cursor relative to the corners of the selection rectangle
    text_position_start = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    text_position_end = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //    sizer_h->Add(text_phi);
    //    sizer_h->Add(text_lambda);
    //
    //    SetSizer(sizer_h);
    
    
    
}


void DrawPanel::PaintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    Render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void DrawPanel::PaintNow(){
    
    wxClientDC dc(this);
    Render(dc);
    
    //sets the size of the DrawPanel and of the ChartFrame which is its parent and fit the size of ChartFrame parent in such a way that it just fits its content
    this->SetMinSize(wxSize(c->getWidth(), c->getHeight()));
    //        (parent->panel)->SetMinSize(wxSize(
    //                                 (c->getWidth()) + (((parent->slider)->GetSize()).GetWidth()),
    //                                 (c->getHeight()) + (((parent->text_position_now)->GetSize()).GetHeight())
    //                                 ));
    parent->SetMinSize(wxSize(
                              (c->getWidth()) + ((parent->slider)->GetSize().GetWidth()),
                              (c->getHeight()) + (((parent->text_position_now)->GetSize()).GetHeight())
                              ));
    parent->SetSizerAndFit(parent->sizer_v);
    
    
}

//remember that any Draw command in this function takes as coordinates the coordinates relative to the position of the DrawPanel object!
void DrawPanel::Render(wxDC&  dc){
    
    Angle lambda, phi;
    double dummy;
    stringstream s;
    wxString wx_string;
    //this = true if, while drawing the x or y axis labels, the label that I one is about to draw is the first one
    bool first_label;
    
    wxBrush brush(wxColour(/*the first three entries are the rgb code for the color*/255, 0, 0, /*the last is the degree of transparency of the color*/25));
    //    brush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetBrush(brush);
    
    //    dc.SetBrush(*wxTRANSPARENT_BRUSH); //no filling
    dc.SetPen(wxPen(wxColor(255,175,175), 1 ) ); // 1-pixels-thick pink outline
    dc.DrawBitmap(wxBitmap(path_file_chart, wxBITMAP_TYPE_PNG), 0, 0);
    if(selection_rectangle){
        dc.DrawRectangle(
                         position_start_selection.x - (position_draw_panel.x),
                         position_start_selection.y - (position_draw_panel.y),
                         (position_screen_now.x)-(position_start_selection.x),
                         (position_screen_now.y)-(position_start_selection.y)
                         );
        
    }
    
    //draw labels on the x axis
    //set the initial value of dummy
    if(x_min > x_mercator((floor((K*(((((parent->parent)->plot)->lambda_min).value)))/delta_lambda))*delta_lambda)){
        dummy = x_mercator((ceil((K*(((((parent->parent)->plot)->lambda_min).value)))/delta_lambda))*delta_lambda);
    }else{
        dummy = x_mercator((floor((K*(((((parent->parent)->plot)->lambda_min).value)))/delta_lambda))*delta_lambda);
    }
    //starts the loop which draws the labels
    for(first_label = true; dummy <= x_max; dummy+=k*delta_lambda){
        
        s.str("");
        lambda.set(String(""), k*lambda_mercator(dummy), String(""));
        
        if(/*If this condition is true, then lambda.value*K is an integer multiple of one degree. I use delta_lambda to check this condition rather tahn lambda itself, because delta_lambda is not subject to rounding errors */delta_lambda == round(delta_lambda)){
            //in this case, lambda = n degrees, with n integer: I write on the axis only the degree part of lambda
            s << lambda.deg_to_string(String("EW"), display_precision);
        }else{
            //in this case, delta_lambda  is not an integer multiple of a degree. However, lambda_mercator(dummy) may still be or not be a multiple integer of a degree
            
            if(fabs(lambda_mercator(dummy) - ((double)round(lambda_mercator(dummy)))) < delta_lambda/2.0){
                //in this case, lamba_mercator(dummy) coincides with an integer mulitple of a degree: I print out its arcdegree part only
                
                s << lambda.deg_to_string(String("EW"), display_precision);
                
            }else{
                //in this case, lamba_mercator(dummy) deos not coincide with an integer mulitple of a degree.
                
                
                if(ceil((K*((((parent->parent)->plot)->lambda_max).value)))  - floor((K*((((parent->parent)->plot)->lambda_min).value))) != 1){
                    //in this case, the lambda interval which is plotted spans more than a degree: there will already be at least one tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I print out its arcminute part only.
                    
                    s << lambda.min_to_string(String("EW"), display_precision);
                }else{
                    //in this case, the lambda interval which is plotted spans les than a degree: there will be no tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I add this tic by printing, at the first tic, both the arcdegrees and arcminutes.
                    
                    if(first_label){
                        s << lambda.to_string(String("EW"), display_precision);
                    }else{
                        s << lambda.min_to_string(String("EW"), display_precision);
                    }
                }
                
            }
        }
        wx_string = wxString(s.str().c_str());
        
        dc.DrawRotatedText(
                           wx_string,
                           (position_plot_area.x) + (dummy-x_min)/(x_max-x_min)*width_plot_area - (GetTextExtent(wx_string).GetWidth())/2,
                           (position_plot_area.y) + height_plot_area /*this is the border, to allow some empty space between the text and the axis*/
                           + ((parent->GetSize()).GetWidth())*length_border_over_length_frame,
                           0);
        
        first_label = false;
        
    }
    
    //draw labels on the y axis
    //set first value of dummy
    if(y_min > floor((K*(((((parent->parent)->plot)->phi_min).value)))/delta_phi)*delta_phi){
        dummy = ceil((K*(((((parent->parent)->plot)->phi_min).value)))/delta_phi)*delta_phi;
    }else{
        dummy = floor((K*(((((parent->parent)->plot)->phi_min).value)))/delta_phi)*delta_phi;
    }
    //starts for loop which draws the ylabels
    for(first_label = true; dummy<(K*(((((parent->parent)->plot)->phi_max).value))); dummy+= delta_phi){
        
        s.str("");
        phi.set(String(""), k*dummy, String(""));
        phi.normalize_pm_pi();
        
        if(/*If this condition is true, then phi.value*K is an integer multiple of one degree. I use delta_phi to check this condition rather tahn lambda itself, because delta_phi is not subject to rounding errors */delta_phi== round(delta_phi)){
            //in this case, dummy (or, in other words, the latitude phi) = n degrees, with n integer: I write on the axis the value of phi  in degrees
            s << phi.deg_to_string(String("NS"), display_precision);
        }else{
            
            //in this case, delta_phi  is not an integer multiple of a degree. However, dummy may still be or not be a multiple integer of a degree
            if(fabs(dummy - ((double)round(dummy))) < delta_phi/2.0){
                //in this case, dummy coincides with an integer mulitple of a degree: I print out its arcdegree part only
                
                s << phi.deg_to_string(String("NS"), display_precision);
                
            }else{
                //in this case, dummy deos not coincide with an integer mulitple of a degree: I print out its arcminute part only
                
                //                s << phi.min_to_string(String("NS"), display_precision);
                
                if(ceil((K*((((parent->parent)->plot)->phi_max).value)))  - floor((K*((((parent->parent)->plot)->phi_min).value))) != 1){
                    //in this case, the phi interval which is plotted spans more than a degree: there will already be at least one tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I print out its arcminute part only.
                    
                    s << phi.min_to_string(String("NS"), display_precision);
                }else{
                    //in this case, the phi interval which is plotted spans less than a degree: there will be no tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I add this tic by printing, at the first tic, both the arcdegrees and arcminutes.
                    
                    if(first_label){
                        s << phi.to_string(String("NS"), display_precision);
                    }else{
                        s << phi.min_to_string(String("NS"), display_precision);
                    }
                }
                
                
            }
            
        }
        
        wx_string = wxString(s.str().c_str());
        
        dc.DrawRotatedText(
                           wx_string,
                           (position_plot_area.x) - (GetTextExtent(wx_string).GetWidth()) - /*this is the border, to allow some empty space between the text and the axis*/
                           ((parent->GetSize()).GetWidth())*length_border_over_length_frame,
                           (position_plot_area.y) + height_plot_area - ((y_mercator(dummy)-y_min)/(y_max-y_min)*height_plot_area) - (GetTextExtent(wx_string).GetHeight())/2,
                           0);
        
        first_label = false;
        
    }
    
    /*
     Position geo;
     wxPoint screen;
     cout << "A position_screen_now = " << (position_screen_now.x) << " " << (position_screen_now.y) << "\n";
     GetMouseGeoPosition(&geo);
     GeoToScreen(geo, &screen);
     dc.DrawCircle(screen.x - position_draw_panel.x, screen.y - position_draw_panel.y, 10);
     */
    
}


void DrawPanel::Draw(void){
    
    File world;
    stringstream line_ins;
    string line;
    double *x, *y, lambda, phi, x_dummy, y_dummy, phi_span, lambda_span;
    int i;
    unsigned int /*this is the number of geographical points on the map which will fall in the plot rectangle (x_min , x_max) x (y_min, y_max)*/number_of_points;
    
    //fetch the data on the region that I am about to plot from the data files.
    parent->GetCoastLineData();
    
    Update_x_y_min_max();
    
    /*I set the aspect ratio between height and width equal to the ration between the y and x range: in this way, the aspect ratio of the plot is equal to 1*/
    if((y_max-y_min) > (x_max-x_min)){
        //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
        parent->SetSize(
                        (((parent->rectangle_display).GetSize()).GetHeight())/((y_max-y_min)/(x_max-x_min)),
                        (((parent->rectangle_display).GetSize()).GetHeight())
                        );
        
        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object
        height_chart = length_chart_over_length_chart_frame * (((parent->rectangle_display).GetSize()).GetHeight());
        width_chart = height_chart/((y_max-y_min)/(x_max-x_min));
    }else{
        //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
        parent->SetSize(
                        (((parent->rectangle_display).GetSize()).GetHeight()),
                        (((parent->rectangle_display).GetSize()).GetHeight()) * ((y_max-y_min)/(x_max-x_min))
                        );
        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object
        width_chart = length_chart_over_length_chart_frame * (((parent->rectangle_display).GetSize()).GetHeight());
        height_chart = width_chart*((y_max-y_min)/(x_max-x_min));
    }
    width_plot_area = width_chart*length_plot_area_over_length_chart;
    height_plot_area = height_chart*length_plot_area_over_length_chart;
    tic_length = tic_length_over_width_plot_area*width_plot_area;
    
    
    // Create a XYChart object with the appropriate size
    c = new XYChart(width_chart, height_chart);
    //create the plot area of c with the appropriate size
    c->setPlotArea(width_chart*0.1, height_chart*0.1,
                   width_plot_area,
                   height_plot_area,
                   -1, -1, 0xc0c0c0, 0xc0c0c0, -1);
    
    //stores into position_plot_area the screen position of the top-left edge of the plot area.
    position_plot_area = wxPoint((c->getPlotArea())->getLeftX(), (c->getPlotArea())->getTopY());
    //stores in to size_plot_area the size of the plot area
    size_plot_area = wxSize((c->getPlotArea())->getWidth(), (c->getPlotArea())->getHeight());
    
    
    //
    world.set_name(String(path_file_selected_coastline_data));
    world.count_lines(String(""));
    
    x = new double [(world.number_of_lines)];
    y = new double [(world.number_of_lines)];
    
    world.open(String("in"), String(""));
    
    line.clear();
    getline(world.value, line);
    
    cout << "Number of lines = " << world.number_of_lines << "\n";
    
    for(number_of_points=0, i=0; i<(world.number_of_lines); i++){
        
        line.clear();
        line_ins.clear();
        getline(world.value, line);
        line_ins << line;
        line_ins >> phi >> lambda;
        
        x_dummy = x_mercator(lambda);
        y_dummy = y_mercator(phi);
        
        if((x_min <= x_dummy) && (x_dummy <= x_max) && (y_min <= y_dummy) && (y_dummy <= y_max)){
            
            x[number_of_points] = x_dummy;
            y[number_of_points] = y_dummy;
            number_of_points++;
            
        }
        
        //        cout << " ******* " << x[i] << " " << y[i] << "\n";
        
    }
    
    world.close(String(""));
    
    
    
    //set parallels
    lambda_span = K*(x_max-x_min);
    
    //set delta_lambda
    if(lambda_span > 1.0){gamma_lambda = 1.0;}
    else{gamma_lambda = 60.0;}
    
    delta_lambda=1.0/gamma_lambda;
    while(((((parent->parent)->plot)->n_intervals_tics).value)*delta_lambda<lambda_span){
        if(delta_lambda == 1.0/gamma_lambda){delta_lambda = delta_lambda + 4.0/gamma_lambda;}
        else{delta_lambda = delta_lambda + 5.0/gamma_lambda;}
    }
    if(delta_lambda > 1.0/gamma_lambda){
        if(delta_lambda == 5.0/gamma_lambda){delta_lambda = delta_lambda - 4.0/gamma_lambda;}
        else{delta_lambda = delta_lambda - 5.0/gamma_lambda;}
    }
    cout <<  "... delta_lambda = " << delta_lambda << "\n";
    
    lambda = ((int)((K*(((((parent->parent)->plot)->lambda_min).value)))/delta_lambda))*delta_lambda;
    for(x_dummy = x_mercator(lambda); x_dummy <= x_max; x_dummy+=k*delta_lambda){
        
        if((x_dummy >= x_min) && (x_dummy <= x_max)){
            
            c->addLine(
                       (position_plot_area.x) + (x_dummy-x_min)/(x_max-x_min)*width_plot_area,
                       (position_plot_area.y),
                       (position_plot_area.x) + (x_dummy-x_min)/(x_max-x_min)*width_plot_area,
                       (position_plot_area.y) + height_plot_area,
                       0x808080, 1);
            
        }
        
        if(gamma_lambda == 60.0){
            
            //plot the xtics from lambda to the next lambda (lambda + dlambda)
            for(i=0; (((double)i)/10.0)/60.0 < delta_lambda; i++){
                
                if(x_dummy + k*(((double)i)/10.0)/60.0 <= x_max){
                    //set custom-made minor xtics every tenths (i/10.0) of arcminute (60.0)
                    
                    c->addLine(
                               (position_plot_area.x) + ((x_dummy + k*(((double)i)/10.0)/60.0)-x_min)/(x_max-x_min)*width_plot_area,
                               (position_plot_area.y) + height_plot_area,
                               (position_plot_area.x) + ((x_dummy + k*(((double)i)/10.0)/60.0)-x_min)/(x_max-x_min)*width_plot_area,
                               (position_plot_area.y) + height_plot_area - height_plot_area*tic_length_over_width_plot_area,
                               0x0000ff, 1);
                    
                }
                
            }
            
        }
        
    }
    //
    
    //set meridians
    phi_span = K*(((((parent->parent)->plot)->phi_max).value) - ((((parent->parent)->plot)->phi_min).value));
    
    //gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if(phi_span > 1.0){gamma_phi = 1.0;}
    else{gamma_phi = 60.0;}
    
    delta_phi=1.0/gamma_phi;
    while(((((parent->parent)->plot)->n_intervals_tics).value)*delta_phi<phi_span){
        //print delta_phi;
        if(delta_phi == 1.0/gamma_phi){delta_phi = delta_phi + 4.0/gamma_phi;}
        else{delta_phi = delta_phi + 5.0/gamma_phi;}
    }
    if(delta_phi > 1.0/gamma_phi){
        if(delta_phi == 5.0/gamma_phi){delta_phi = delta_phi - 4.0/gamma_phi;}
        else{delta_phi = delta_phi - 5.0/gamma_phi;}
    }
    cout << "... delta_phi = "  << delta_phi << "\n";
    
    
    
    
    for(phi = ((int)((K*(((((parent->parent)->plot)->phi_min).value)))/delta_phi))*delta_phi; phi<(K*(((((parent->parent)->plot)->phi_max).value))); phi+= delta_phi){
        
        y_dummy = y_mercator(phi);
        
        if((y_dummy >= y_min) && (y_dummy <= y_max)){
            
            c->addLine(
                       (position_plot_area.x),
                       (position_plot_area.y) + height_plot_area - ((y_dummy-y_min)/(y_max-y_min)*height_plot_area),
                       (position_plot_area.x) + width_plot_area,
                       (position_plot_area.y) + height_plot_area - ((y_dummy-y_min)/(y_max-y_min)*height_plot_area),
                       0x808080, 1);
            
            if(gamma_phi == 60.0){
                
                //plot the ytics from phi to the next phi (phi + dphi)
                for(i=0; (((double)i)/10.0)*1.0/60.0 < delta_phi; i++){
                    
                    if(phi + (((double)i)/10.0)*1.0/60.0 <= (K*(((((parent->parent)->plot)->phi_max).value)))){
                        //set custom-made minor ytics every tenths (i/10.0) of arcminutes (60.0)
                        
                        c->addLine(
                                   (position_plot_area.x),
                                   (position_plot_area.y) + height_plot_area - (( y_mercator(phi + ((double)i)/10.0/60.0)  -y_min)/(y_max-y_min)*height_plot_area),
                                   (position_plot_area.x) + width_plot_area*tic_length_over_width_plot_area,
                                   (position_plot_area.y) + height_plot_area - ((y_mercator(phi + ((double)i)/10.0/60.0)-y_min)/(y_max-y_min)*height_plot_area),
                                   0x0000ff, 1);
                        
                    }
                    
                }
                
            }
            
        }
        
    }
    
    //
    
    
    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    //    c->addLegend(50, 30, false, "Times New Roman Bold Italic", 12)->setBackground(Chart::Transparent);
    
    // Add a title to the x axis using 12pt Arial Bold Italic font
    //    (c->xAxis())->setTitle("lambda", "Arial", 12);
    //set the interval of the x axis, and disables the xtics with the last NoValue argument
    (c->xAxis())->setLinearScale(x_min, x_max, 1.7E+308);
    
    //    delta_lambda = 15.0;
    //    (c->xAxis())->addLabel(0.0, "*");
    //    for(x_dummy=delta_lambda*k; x_dummy<x_max; x_dummy+=delta_lambda*k){
    //        (c->xAxis())->addLabel(x_dummy, "*");
    //    }
    //    for(x_dummy=-delta_lambda*k; x_dummy>x_min; x_dummy-=delta_lambda*k){
    //        (c->xAxis())->addLabel(x_dummy, "*");
    //    }
    //
    // Add a title to the y axis using 12pt Arial Bold Italic font
    //    (c->yAxis())->setTitle("phi", "Arial", 12);
    (c->yAxis())->setLinearScale(y_min, y_max, 1.7E+308);
    
    //    delta_phi = 30.0;
    //    (c->yAxis())->addLabel(0.0, "/");
    //    for(phi = delta_phi; y_mercator(phi)<y_max; phi+=delta_phi){
    //        (c->yAxis())->addLabel(y_mercator(phi), "/");
    //    }
    //    for(phi = -delta_phi; y_mercator(phi)>y_min; phi-=delta_phi){
    //        (c->yAxis())->addLabel(y_mercator(phi), "/");
    //    }
    
    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);
    
    // Add an orange (0xff9933) scatter chart layer, using 13 pixel diamonds as symbols
    c->addScatterLayer(DoubleArray(x, number_of_points), DoubleArray(y, number_of_points), "", Chart::CircleSymbol, 1, 000000);
    
    c->makeChart(path_file_chart);
    
    //free up resources
    delete [] x;
    delete [] y;
    
}


ChartFrame::ChartFrame(ListFrame* parent_input, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    stringstream s;
    String new_prefix;
    
    parent = parent_input;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    (parent->plot)->show(true, String(""));
    
    idling = false;
    print_error_message = new PrintErrorMessage<ChartFrame>(this);
    
    panel = new ChartPanel(this, wxDefaultPosition, wxDefaultSize);
    draw_panel = new DrawPanel(panel);
    
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_slider = new wxBoxSizer(wxVERTICAL);
    
    
    //text field showing the latitude and longitude of the intantaneous (now) mouse position on the chart
    text_position_now = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    
    //image
    wxPNGHandler *handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle_display = (display.GetClientArea());
    
    draw_panel->Draw();
    
    //stores the x_min .. y_max, width_chart, height chart the first time that the chart is shown into x_min_0 ... height_chart_0
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);
    (draw_panel->width_chart_0) = (draw_panel->width_chart);
    (draw_panel->height_chart_0) = (draw_panel->height_chart);
    
    
    //initialize the variable neededed for slider
    value_slider_old = 1;
    //allocate the slider
    slider = new wxSlider(panel, wxID_ANY, 1, value_slider_old, value_slider_max, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
    
    //text field showing the current value of the zoom slider
    s.str("");
    s << "1:" << value_slider_old;
    text_slider = new wxStaticText(panel, wxID_ANY, wxString(s.str().c_str()), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    
    
    //    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(path_file_chart, wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    draw_panel->Bind(wxEVT_MOTION, wxMouseEventHandler(DrawPanel::OnMouseMovement), draw_panel);
    draw_panel->Bind(wxEVT_RIGHT_DOWN, wxMouseEventHandler(DrawPanel::OnMouseRightDown), draw_panel);
    draw_panel->Bind(wxEVT_LEFT_DOWN, wxMouseEventHandler(DrawPanel::OnMouseLeftDown), draw_panel);
    draw_panel->Bind(wxEVT_LEFT_UP, wxMouseEventHandler(DrawPanel::OnMouseLeftUp), draw_panel);
    draw_panel->Bind(wxEVT_MOTION, wxMouseEventHandler(DrawPanel::OnMouseDrag), draw_panel);
    
    slider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(DrawPanel::OnScroll), draw_panel);
    
    draw_panel->SetMinSize(wxSize((draw_panel->c)->getWidth(),(draw_panel->c)->getHeight()));
    
    sizer_slider->Add(slider, 0, wxALIGN_CENTER | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_slider->Add(text_slider, 0, wxALIGN_CENTER | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    
    sizer_h->Add(draw_panel, 0, wxALIGN_TOP | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_h->Add(sizer_slider, 0, wxALIGN_TOP | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_v->Add(text_position_now, 0, wxALIGN_LEFT | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    //    sizer_v->Fit(panel);
    
    Maximize(panel);
    SetSizerAndFit(sizer_v);
    
    
    
    
    //    panel->SetSizer(sizer_v);
    //    sizer_v->Fit(this);
    
    //    SetSize((draw_panel->c)->getWidth() + ((draw_panel->c)->getWidth())*0.01, (draw_panel->c)->getHeight() + ((draw_panel->c)->getWidth())*0.01);
    
    //    SetAutoLayout(true);
    
    //    Fit();
    Centre();
    
}

//this function computes lambda_min, ... phi_max from x_min ... y_max
void DrawPanel::Update_lambda_phi_min_max(void){
    
    (((parent->parent)->plot)->lambda_min).set(String(""), k*lambda_mercator(x_min), String(""));
    (((parent->parent)->plot)->lambda_max).set(String(""), k*lambda_mercator(x_max), String(""));
    (((parent->parent)->plot)->phi_min).set(String(""), k*phi_mercator(y_min), String(""));
    (((parent->parent)->plot)->phi_max).set(String(""), k*phi_mercator(y_max), String(""));
    
}

//this function computes x_min, ... y_max from lambda_min ... phi_max
void DrawPanel::Update_x_y_min_max(void){
    
    x_min = x_mercator(K*((((parent->parent)->plot)->lambda_min).value));
    x_max = x_mercator(K*((((parent->parent)->plot)->lambda_max).value));
    y_min = y_mercator(K*((((parent->parent)->plot)->phi_min).value));
    y_max = y_mercator(K*((((parent->parent)->plot)->phi_max).value));
    
}

void ChartFrame::UpdateSliderLabel(void){
    
    stringstream s;
    
    s.str("");
    s << "1:" << value_slider_old;
    text_slider->SetLabel(s.str().c_str());
    
}

//this function updates the slider according to the zooming factor of the chart. If the zooming factor does not exceed the maximal allowed value, it returns true and it updates the slider, otherwise it returns false and it does not update the slider.
bool ChartFrame::UpdateSlider(void){
    
    bool output;
    
    //compute the zooming factor of the chart and write it into value_slider_old
    value_slider_old = ((unsigned int)((double)(draw_panel->width_chart))/((double)(draw_panel->width_chart_0))*((draw_panel->x_max_0)-(draw_panel->x_min_0))/((draw_panel->x_max)-(draw_panel->x_min)));
    
    cout << "***************** Slider value = " << value_slider_old << "\n";
    
    if(value_slider_old <= value_slider_max){
        
        slider->SetValue(value_slider_old);
        UpdateSliderLabel();
        
        output = true;
        
    }else{
        
        //        set the wxControl, title and message for the functor print_error_message, and then call the functor
        (print_error_message->control) = slider;
        (print_error_message->title) = String("Zoom level exceeded its maximal value!");
        (print_error_message->message) = String("Zoom level must be >= 1 and <= value_slider_max.");
        CallAfter(*print_error_message);
        
        output = false;
        
    }
    
    return output;
    
}

template<class T>void CheckBody::operator()(T& event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        //I check whether the name in the GUI field body matches one of the body names in catalog
        for(check = false, i=0; (i<((p->catalog)->list).size()) && (!check); i++){
            if(String(((p->name)->GetValue().ToStdString())) == ((((p->catalog)->list)[i]).name)){
                check = true;
            }
        }
        i--;
        
        if(check){
            
            //            (*(p->body)) = ((p->catalog)->list)[i];
            
            if((((p->catalog)->list)[i].name == String("sun")) || (((p->catalog)->list)[i].name == String("moon"))){
                ((f->limb)->name)->Enable(true);
            }else{
                ((f->limb)->name)->Enable(false);
            }
            
            (p->name)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->name);
            ((f->printerrormessage)->title) = String("Body not found in catalog!");
            ((f->printerrormessage)->message) = String("Body must be in catalog.");
            f->CallAfter(*(f->printerrormessage));
            
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
            
            
            
            (p->name)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->name);
            ((f->printerrormessage)->title) = String("Limb not valid!");
            ((f->printerrormessage)->message) = String("Limb must be upper, lower or center.");
            f->CallAfter(*(f->printerrormessage));
            
            (p->ok) = false;
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}


//writes the value contained in the GUI field into the non-GUI field
template<class T> void LimbField::get(T &event){
    
    if(ok){
        
        //I set the char in (limb->value) to the first letter in the string contained in the GUI field
        (limb->value) = ((String((name->GetValue().ToStdString()))).value)[0];
        
    }
    
    event.Skip(true);
    
}


//checks the value of the sign in the GUI field
template<class P> template <class T> void CheckSign<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is in idling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        //I check whether the name in the GUI field sign matches one of the sign values in the list signs
        if((p->format) == String("")){
            //if the AngleField p has no sign, the check is ok
            
            check=true;
            
        }else{
            //if the AngleField p has a sign, I check it
            
            for(check = false, i=0; (i<((p->signs).GetCount())) && (!check); i++){
                if(((p->sign)->GetValue()) == (p->signs)[i]){
                    check = true;
                }
            }
            
        }
        
        if(check){
            
            (p->sign)->SetBackgroundColour(*wxWHITE);
            (p->sign_ok) = true;
            
            //            p->get(event);
            
        }else{
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->sign);
            ((f->printerrormessage)->title) = String("Sign is not valid!");
            ((f->printerrormessage)->message) = String("Sign must be +-, NS or EW.");
            f->CallAfter(*(f->printerrormessage));
            
            (p->sign_ok) = false;
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}

//converts the point p on the screen (which is supposed to lie in the plot area), to the relative geographic position q
void DrawPanel::ScreenToGeo(wxPoint p, Position *q){
    
    //updates the position of the draw pane this
    position_draw_panel = (this->GetScreenPosition());
    
    (q->lambda).set(String(""), k*lambda_mercator(x_min+ (((double)(p.x)-((position_draw_panel.x)+(position_plot_area.x)))/((double)(size_plot_area.x)))*(x_max - x_min)), String(""));
    (q->phi).set(String(""), k*(phi_mercator(y_min - (((double)((p.y)-((position_draw_panel.y)+(position_plot_area.y)+(size_plot_area.y))))/((double)(size_plot_area.y)))*(y_max - y_min) )), String(""));
    
    
}

//this function converts the geographic position p into the screen position p 
void DrawPanel::GeoToScreen(Position q, wxPoint *p){
    
    //updates the position of the draw pane this
    position_draw_panel = (this->GetScreenPosition());
    
    (p->x) = (position_draw_panel.x) + (position_plot_area.x) + (x_mercator(K*((q.lambda).value))-x_min)/(x_max-x_min)*width_plot_area;
    (p->y) = (position_draw_panel.y) + (position_plot_area.y) + (height_plot_area) - ((y_mercator(K*((q.phi).value))-y_min)/(y_max-y_min)*height_plot_area);
    
    //    cout << "B = screen = " << (p->x) << " " << (p->y) << "\n";
    
    
}

//This function obtains the geographical Position p of the mouse hovering on the map of the world
void DrawPanel::GetMouseGeoPosition(Position* p){
    
    position_screen_now = wxGetMousePosition();
    ScreenToGeo(position_screen_now, p);
    
}

void DrawPanel::OnMouseMovement(wxMouseEvent &event){
    
    Position p;
    stringstream s;
    
    //    cout << "\nMouse moved";
    
    GetMouseGeoPosition(&p);
    
    //update the instantaneous position of the mouse on the chart
    s.str("");
    s << (p.phi).to_string(String("NS"), display_precision) << " " << (p.lambda).to_string(String("EW"), display_precision);
    (parent->text_position_now)->SetLabel(wxString(s.str().c_str()));
    
    //if a selection rectangle is being drawn, update the instantaneous position of the final corner of the rectangle
    if(selection_rectangle){
        s.str("");
        s << (p.phi).to_string(String("NS"), display_precision) << " " << (p.lambda).to_string(String("EW"), display_precision);
        text_position_end->SetLabel(wxString(s.str().c_str()));
        text_position_end->SetPosition(wxPoint((position_screen_now.x)-(position_draw_panel.x), (position_screen_now.y)-(position_draw_panel.y)));
        PaintNow();
    }
    
    
    
    //
    event.Skip(true);
    
}

//if the left button of the mouse is pressed, I record its position as the starting position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftDown(wxMouseEvent &event){
    
    position_start_drag = wxGetMousePosition();
    
    Position geo;
    ScreenToGeo(position_start_drag, &geo);
    geo.print(String("Position start drag"), String("************ "), cout);
    
    event.Skip(true);
    
}

//if the left button of the mouse is released, I record its position as the ending position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftUp(wxMouseEvent &event){
    
    SetCursor(*wxCROSS_CURSOR);
    
    position_end_drag = wxGetMousePosition();
    
    Position geo;
    ScreenToGeo(position_now_drag, &geo);
    
    geo.print(String("Position end drag"), String("************ "), cout);
    
    
    
    event.Skip(true);
    
}

void DrawPanel::OnMouseRightDown(wxMouseEvent &event){
    
    //changes the 'sign' of selection rectangle
    selection_rectangle = (!selection_rectangle);
    
    if(selection_rectangle){
        
        stringstream s;
        
        cout << "You started drawing\n";
        //        ((parent->plot)->lambda_min) = (p.lambda);
        //        ((parent->plot)->phi_min) = (p.phi);
        //
        
        GetMouseGeoPosition(&p_start);
        position_start_selection = position_screen_now;
        
        s.clear();
        s << (p_start.phi).to_string(String("NS"), display_precision) << " " << (p_start.lambda).to_string(String("EW"), display_precision);
        text_position_start->SetLabel(wxString(s.str().c_str()));
        text_position_start->SetPosition(wxPoint((position_start_selection.x)-(position_draw_panel.x), (position_start_selection.y)-(position_draw_panel.y)));
        
        
        cout << "p_start = {" << (p_start.lambda).to_string(String("EW"), display_precision) << " , " << (p_start.phi).to_string(String("NS"), display_precision) << " }\n";
        
        
    }else{
        
        cout << "You ended drawing\n";
        
        //        ((parent->plot)->lambda_max) = (p.lambda);
        //        ((parent->plot)->phi_max) = (p.phi);
        GetMouseGeoPosition(&p_end);
        position_end_selection = position_screen_now;
        
        cout << "p_end = {" << (p_end.lambda).to_string(String("EW"), display_precision) << " , " << (p_end.phi).to_string(String("NS"), display_precision) << " }\n";
        
        
        
        //reinitialize c and sets the new values of lambda_min, lambda_max, phi_min and phi_max
        delete c;
        //I convert all the angles to the format between -pi and pi, so I can sort them numerically
        (p_start.phi).normalize_pm_pi();
        (p_start.lambda).normalize_pm_pi();
        (p_end.phi).normalize_pm_pi();
        (p_end.lambda).normalize_pm_pi();
        //I assign the values of lambda_min and lamba_max, phi_min and phi_max from the vluaes of p_start.lambda, ... p_end.phi in such a way that lambda_min correspnds to the longitude of the leftmost edge x_min of the mercator projection, lambda_max to the rightmost one, etc.
        if((p_start.lambda)>(p_end.lambda)){
            (((parent->parent)->plot)->lambda_min) = (p_start.lambda);
            (((parent->parent)->plot)->lambda_max) = (p_end.lambda);
        }else{
            (((parent->parent)->plot)->lambda_min) = (p_end.lambda);
            (((parent->parent)->plot)->lambda_max) = (p_start.lambda);
        }
        if((p_start.phi)>(p_end.phi)){
            (((parent->parent)->plot)->phi_max) = (p_start.phi);
            (((parent->parent)->plot)->phi_min) = (p_end.phi);
        }else{
            (((parent->parent)->plot)->phi_min) = (p_start.phi);
            (((parent->parent)->plot)->phi_max) = (p_end.phi);
        }
        //I normalize lambda_min, ..., phi_max for future use.
        (((parent->parent)->plot)->lambda_min).normalize();
        (((parent->parent)->plot)->lambda_max).normalize();
        (((parent->parent)->plot)->phi_min).normalize();
        (((parent->parent)->plot)->phi_max).normalize();
        
        
        //once I draw a new, zoomed map, I set to empty the text fields of the geographical positions of the selection triangle, which is now useless
        text_position_start->SetLabel(wxString(""));
        text_position_end->SetLabel(wxString(""));
        
        Draw();
        
        PaintNow();
        
        parent->UpdateSlider();
        
    }
    
    event.Skip(true);
    
}

void DrawPanel::OnMouseDrag(wxMouseEvent &event){
    
    if(wxGetMouseState().LeftIsDown()){
        
        mouse_dragging = true;
        
        SetCursor(wxCURSOR_HAND);
        
        double delta_x, delta_y;
        
        position_now_drag = wxGetMousePosition();
        
        Position geo;
        //    ScreenToGeo(position_start_drag, &geo_start);
        ScreenToGeo(position_now_drag, &geo);
        
        
        //update x_min, ..., y_max according to the drag.
        delta_x = ((double)((position_now_drag.x)-(position_start_drag.x)))/((double)width_plot_area) * (x_max-x_min);
        delta_y = ((double)((position_now_drag.y)-(position_start_drag.y)))/((double)height_plot_area) * (y_max-y_min);
        x_min -= delta_x;
        x_max -= delta_x;
        y_min += delta_y;
        y_max += delta_y;
        
        Update_lambda_phi_min_max();
        
        geo.print(String("Position now drag"), String("************ "), cout);
        
        
        //re-draw the chart
        Draw();
        PaintNow();
        
        
    }
    
    event.Skip(true);
    
}

void DrawPanel::OnScroll(wxScrollEvent &event){
    
    cout << "Slider = " << (parent->slider)->GetValue() << "\n";
    
    //store the values of x_min ... y_max before the scrolling event into x_min_old .... y_max_old. The value of the slider before the sliding event is already stored in value_slider_old
    x_min_old = x_min;
    x_max_old = x_max;
    y_min_old = y_min;
    y_max_old = y_max;
    
    //update x_min, ..., y_max according to the zoom (scroll) and lambda_min, ..., phi_max
    x_min = (x_max_old + x_min_old)/2.0 - ( (x_max_old-x_min_old)/2.0 * ((double)(parent->value_slider_old))/((double)((parent->slider)->GetValue())) );
    x_max = (x_max_old + x_min_old)/2.0 + ( (x_max_old-x_min_old)/2.0 * ((double)(parent->value_slider_old))/((double)((parent->slider)->GetValue())) );
    y_min = (y_max_old + y_min_old)/2.0 - ( (y_max_old-y_min_old)/2.0 * ((double)(parent->value_slider_old))/((double)((parent->slider)->GetValue())) );
    y_max = (y_max_old + y_min_old)/2.0 + ( (y_max_old-y_min_old)/2.0 * ((double)(parent->value_slider_old))/((double)((parent->slider)->GetValue())) );
    Update_lambda_phi_min_max();
    
    cout << "x_min = " << x_min<< "\n";
    
    //update parent->value_slider_old
    (parent->value_slider_old) = ((parent->slider)->GetValue());
    
    Draw();
    PaintNow();
    parent->UpdateSliderLabel();
    
    event.Skip(true);
    
}

//writes to the non-GUI field angle the values written in the GUI fields sign, deg and min
template<class P> template <class T> void AngleField<P>::get(T &event){
    
    
    if(sign_ok && deg_ok && min_ok){
        
        double min_temp;
        char c;
        
        if(format == String("")){
            //in this case there is no sign in AngleField->this:
            
            c='+';
            
        }else{
            //in this case there is a sign in AngleField->this: I write the sign in c
            
            wxString s;
            s = (sign->GetValue());
            
            if(format == String("+-")){
                
                if(s==wxString("+")){c='+';}
                else{c='-';}
                
            }
            
            if(format == String("EW")){
                
                if(s==wxString("W")){c='+';}
                else{c='-';}
                
            }
            
            if(format == String("NS")){
                
                if(s==wxString("N")){c='+';}
                else{c='-';}
                
            }
            
        }
        
        
        (min->GetValue()).ToDouble(&min_temp);
        
        angle->from_sign_deg_min(c, wxAtoi(deg->GetValue()), min_temp);
        
    }
    
    event.Skip(true);
    
}


//constructor of the struct, which initializes the Answer remove_related_route. If remove_related_route.value = 'y', then DeleteSight::operator() will delete both the sight and the related route. If remove_related_route.value = 'n', then it will remove the sight only.
DeleteSight::DeleteSight(Answer remove_related_route_in){
    
    remove_related_route = remove_related_route_in;
    
}

void DeleteSight::operator()(wxCommandEvent& event){
    
    //    remove_related_route.print(String("Answer on remove rel rou"), String("xxxxxxxxx "), cout);
    
    (f->plot)->remove_sight(i_sight_to_remove, remove_related_route, String(""));
    
    //    f->plot->print(true, String("--------- "), cout);
    
    
    event.Skip(true);
    
}



template<class P> template <class T> void SetStringToCurrentTime<P>::operator()(T& event){
    
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

template<class F> CloseFrame<F>::CloseFrame(F* frame_in){
    
    frame = frame_in;
    
}

//closes a frame of type F
template<class F> template <class T> void CloseFrame<F>::operator()(T& event){
    
    event.Skip(true);
    
    frame->Close(true);
    
}


//this functor does nothing, delete it in the future
template<class P> template<class T> void CheckString<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //    (p->string)->set(String(""), String(((p->value)->GetValue()).ToStdString()), String(""));
    
    f->TryToEnableReduce();
    
    event.Skip(true);
    
}


//I write in the non-GUI object string the value entered in the GUI object value
template<class P> template<class T> void StringField<P>::get(T &event){
    
    //here I don't check whether the StringField is ok, because any value in the string field is ok
    (*string) = String((value->GetValue().ToStdString()));
    
    event.Skip(true);
    
}

template<class P> CheckAngle<P>::CheckAngle(AngleField<P>* p_in){
    
    p = p_in;
    
    (check_sign.p) = p;
    (check_arc_degree.p) = p;
    (check_arc_minute.p) = p;

}

//this functor checks the whole angle field by calling the check on its sign, arcdegree and arcminute parts‰
template<class P> template <class T> void CheckAngle<P>::operator()(T& event){
    
    check_sign(event);
    check_arc_degree(event);
    check_arc_minute(event);
    
    event.Skip(true);
    
}

//this functor checks the whole date field by calling the check on its year, month and day parts
template <class T> void CheckDate::operator()(T& event){
    
    check_year(event);
    check_month(event);
    check_day(event);
    
    event.Skip(true);
    
}

//this functor writes the values written inthe whole GUI date field (year, month and day) in the respective non-GUI object date->D, date->M, date->D
template <class T> void DateField::get(T& event){
    
    if(year_ok && (year->IsEnabled()) && month_ok && (month->IsEnabled()) && day_ok && (month->IsEnabled())){
        
        (date->Y) = (unsigned int)wxAtoi(year->GetValue());
        (date->M) = (unsigned int)wxAtoi(month->GetValue());
        (date->D) = (unsigned int)wxAtoi(day->GetValue());
        
    }
    
    event.Skip(true);
    
}


template<class P> template<class T> void CheckArcDegree<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->deg)->GetValue()).ToStdString(), NULL, true, 0, 360)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->deg), String("Entered value is not valid!\nArcdegrees must be unsigned integer numbers >= 0° and < 360°"));
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->deg);
            ((f->printerrormessage)->title) = String("Entered value is not valid!");
            ((f->printerrormessage)->message) = String("Arcdegrees must be unsigned integer numbers >= 0° and < 360°");
            f->CallAfter(*(f->printerrormessage));
            
            (p->deg_ok) = false;
            
        }else{
            
            (p->deg)->SetBackgroundColour(*wxWHITE);
            
            //            if((p->min_ok)){
            //
            //                double min_temp;
            //
            //                ((p->min)->GetValue()).ToDouble(&min_temp);
            //
            //                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
            //
            //            }
            
            (p->deg_ok) = true;
            
            //            p->get(event);
            
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}

template<class P> template <class T> void CheckArcMinute<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->min)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->min, String("Entered value is not valid!\nArcminutes must be floating-point numbers >= 0' and < 60'"));
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->min);
            ((f->printerrormessage)->title) = String("Entered value is not valid!");
            ((f->printerrormessage)->message) = String("Arcminutes must be floating-point numbers >= 0' and < 60'");
            f->CallAfter(*(f->printerrormessage));
            
            (p->min_ok) = false;
            
        }else{
            (p->min)->SetBackgroundColour(*wxWHITE);
            //            if((p->deg_ok)){
            //
            //                double min_temp;
            //
            //                ((p->min)->GetValue()).ToDouble(&min_temp);
            //
            //                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
            //
            //
            //            }
            (p->min_ok) = true;
            
            //            p->get(event);
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}

//checks the value in the GUI field in LengthField
template <class T> void CheckLength::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->value)->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX) && ((p->value)->IsEnabled())){
            
            if(!(p->just_enabled)){
                //if the content of the GUI field p is invalid and p has not been just enabled, then I am authorized to prompt an error message
                
                //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
                ((f->printerrormessage)->control) = (p->value);
                ((f->printerrormessage)->title) = String("Entered value is not valid!");
                ((f->printerrormessage)->message) = String("Lengths must be floating-point numbers >= 0 m");
                f->CallAfter(*(f->printerrormessage));
                
            }else{
                //if the LengthField p has just been enabled, I do not print any error message even if the content of p is invalid: this is because I want to give the opportunity to the user to enter the content of the GUI field before complaining that the content of the GUI field is invalid. However, I set just_enabled to false, because p is no longer just enabled.
                
                (p->just_enabled) = false;
                
            }
            
            (p->ok) = false;
            
        }else{
            
            (p->value)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}

//writes the value of the GUI field in LengthField into the non-GUI field length
template <class T> void LengthField::get(T &event){
    
    if(ok){
        
        double length_temp;
        
        value->GetValue().ToDouble(&length_temp);
        length->set(String(""), /*the length is entered in the GUI field in meters, thus I convert it to nm here*/length_temp/(1e3*nm), String(""));
        
    }
    
    event.Skip(true);
    
}



struct OnSelectInListControlSights{
    
    ListFrame* f;
    
    template<class T> void operator()(T&);
    
    
};

struct OnSelectInListControlPositions{
    
    ListFrame* f;
    
    template<class T> void operator()(T&);
    
    
};


//if an item in listcontrol_sights is selected, then the modify_sight and delete_sight buttons are enabled
template<class T> void OnSelectInListControlSights::operator()(T& event){
    
    (f->button_modify_sight)->Enable(true);
    (f->button_delete_sight)->Enable(true);
    
    event.Skip(true);
    
}

//if an item in listcontrol_positions is selected, then the modify_position and delete_position buttons are enabled
template<class T> void OnSelectInListControlPositions::operator()(T& event){
    
    (f->button_modify_position)->Enable(true);
    (f->button_delete_position)->Enable(true);
    
    event.Skip(true);
    
}


template<class T> void PrintErrorMessage<T>::operator()(void){
    
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


wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*0.75);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*0.75);
    
    
    ListFrame *list_frame = new ListFrame("List of sights", "", wxDefaultPosition, wxDefaultSize, String(""));
    list_frame->Show(true);
    
    ChartFrame* nautical_chart = new ChartFrame(list_frame, "A nautical chart",  wxDefaultPosition, wxDefaultSize, String(""));
    nautical_chart->Show(true);
    
    //    list_frame->plot->print(true, String("************* "), cout);
    
    
    //    Answer* answer;
    //    answer = new Answer();
    //    MessageFrame* message_frame = new MessageFrame(NULL, String("question"),  answer, "a", "b", wxDefaultPosition, wxDefaultSize, String(""));
    //    message_frame ->Show(true);
    
    
    return true;
    
}

SightFrame::SightFrame(ListFrame* parent_input, Sight* sight_in, long list_position_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
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
    printerrormessage = new PrintErrorMessage<SightFrame>(this);
    
    file_init.set_name(String(path_file_init));
    check &= (file_init.open(String("in"), prefix));
    
    catalog = new Catalog(String(path_file_catalog), String(""));
    
    //if this SightFrame has been constructed with sight_in = NULL, then I allocate a new Sight object with the pointer this->sight and set list_position to a 'NULL' value (list_position = -1). Otherwise, the pointer sight_in points to a valid Sight object -> I let this->sight point to sight_in, and set list_position to list_position_in.
    if(sight_in != NULL){
        sight = sight_in;
        list_position = list_position_in;
    }else{
        sight = new Sight();
        list_position = -1;
    }
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    
    
    
    
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
    
    //sextant altitude
    wxStaticText* text_H_s = new wxStaticText(panel, wxID_ANY, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    H_s = new AngleField<SightFrame>(this, &(sight->H_s), String("+-"));
    
    //index error
    wxStaticText* text_index_error = new wxStaticText(panel, wxID_ANY, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //If sight_in = NULL, read index error from init file
    if(sight_in == NULL){
        cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
        (sight->index_error).read_from_file(String("index error"), file_init, true, new_prefix);
        (sight->index_error).to_deg_min(&deg, &min);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
    }
    index_error = new AngleField<SightFrame>(this, &(sight->index_error), String("+-"));
    index_error->set();
    
    //artificial horizon
    wxStaticText* text_artificial_horizon_check = new wxStaticText(panel, wxID_ANY, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    artificial_horizon_check = new CheckField<LengthField>(this, &(sight->artificial_horizon), NULL, false);
    
    //height of eye
    wxStaticText* text_height_of_eye = new wxStaticText(panel, wxID_ANY, wxT("Height of eye"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    height_of_eye = new LengthField(this, &(sight->height_of_eye));
    //now that height_of_eye has been allocatd, I link artificial_horizon_check to height_of_eye
    (artificial_horizon_check->related_field) = height_of_eye;
    
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
    
    //master-clock chrono
    wxStaticText* text_space_1 = new wxStaticText(panel, wxID_ANY, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_chrono = new ChronoField(this, &(sight->master_clock_date_and_hour.chrono));
    
    //if sight_in != NULL, then I initialize the GUI filed master_clock_chrono with the one written in sight_in. Otherwise, master_clock_chrono is initialized as empty.
    if(sight_in!=NULL){
        
        master_clock_chrono->set(sight->master_clock_date_and_hour.chrono);
        
    }else{
        
        //if sight_in == NULL, I have previously set the non-GUI object (sight->master_clock_date_and_hour).chrono to the current hour, and I write this value into the GUI object master_clock_chrono
        master_clock_chrono->set((sight->master_clock_date_and_hour).chrono);
        
    }
    
    //check/uncheck stopwatch
    wxStaticText* text_stopwatch_check = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    stopwatch_check = new CheckField<ChronoField>(this, &(sight->use_stopwatch), NULL, true);
    
    //stopwatch reading
    wxStaticText* text_stopwatch_reading = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //    stopwatch_reading = new ChronoField(this, &(sight.stopwatch));
    stopwatch_reading = new ChronoField(this, &(sight->stopwatch));
    //now that stopwatch_reading has been allocatd, I link stopwatch_check to stopwatch_reading
    (stopwatch_check->related_field) = stopwatch_reading;
    
    
    //initialize stopwatch_check and stopwatch_reading
    (stopwatch_check->checkbox)->SetValue(false);
    stopwatch_reading->Enable(false);
    
    wxStaticText* text_TAI_minus_UTC = new wxStaticText(panel, wxID_ANY, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    TAI_minus_UTC = new ChronoField(this, &(sight->TAI_minus_UTC));
    TAI_minus_UTC->set(sight->TAI_minus_UTC);
    
    //label
    wxStaticText* text_label = new wxStaticText(panel, wxID_ANY, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    label = new StringField<SightFrame>(this, &(sight->label));
    
    
    //buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_cancel->Bind(wxEVT_BUTTON, &SightFrame::OnPressCancel, this);
    
    button_reduce = new wxButton(panel, wxID_ANY, "Reduce", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    //I bind reduce button to label->set_string_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_reduce->Bind(wxEVT_BUTTON, &SightFrame::OnPressReduce, this);
    button_reduce->Bind(wxEVT_BUTTON, label->set_string_to_current_time);
    
    //If I press reduce, I want all the fields in this SightFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_reduce->Bind(wxEVT_BUTTON, &BodyField::get<wxCommandEvent>, body);
    button_reduce->Bind(wxEVT_BUTTON, &LimbField::get<wxCommandEvent>, limb);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, H_s);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, index_error);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<LengthField>::get<wxCommandEvent>, artificial_horizon_check);
    button_reduce->Bind(wxEVT_BUTTON, &LengthField::get<wxCommandEvent>, height_of_eye);
    button_reduce->Bind(wxEVT_BUTTON, &DateField::get<wxCommandEvent>, master_clock_date);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField::get<wxCommandEvent>, master_clock_chrono);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<ChronoField>::get<wxCommandEvent>, stopwatch_check);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField::get<wxCommandEvent>, stopwatch_reading);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField::get<wxCommandEvent>, TAI_minus_UTC);
    button_reduce->Bind(wxEVT_BUTTON, &StringField<SightFrame>::get<wxCommandEvent>, label);
    
    
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
    
    Centre();
    
    
}






PositionFrame::PositionFrame(ListFrame* parent_input, Position* position_in, long list_position_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    parent = parent_input;
    
    String new_prefix;
    unsigned int common_width;
    
    bool check = true;
    
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    idling = false;
    printerrormessage = new PrintErrorMessage<PositionFrame>(this);
    
    
    //    wxMenu *menuFile = new wxMenu;
    
    //if this PositionFrame has been constructed with position_in = NULL, then I allocate a new Position object with the pointer this->position and set list_position to a 'NULL' value (list_position = -1). Otherwise, the pointer position_in points to a valid Position object -> I let this->position point to position_in, and set list_position to list_position_in.
    if(position_in != NULL){
        position = position_in;
        list_position = list_position_in;
    }else{
        position = new Position();
        list_position = -1;
    }
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    sizer_grid_measurement = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_grid_label = new wxFlexGridSizer(1, 2, 0, 0);
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    
    
    //latitude
    wxStaticText* text_lat = new wxStaticText(panel, wxID_ANY, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    lat = new AngleField<PositionFrame>(this, &(position->phi), String("NS"));
    
    //longitude
    wxStaticText* text_lon = new wxStaticText(panel, wxID_ANY, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    lon = new AngleField<PositionFrame>(this, &(position->lambda), String("EW"));
    
    //label
    wxStaticText* text_label = new wxStaticText(panel, wxID_ANY, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    label = new StringField<PositionFrame>(this, &(position->label));
    
    
    //buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_add = new wxButton(panel, wxID_ANY, "Add", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    //I bind reduce button to label->set_string_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_add->Bind(wxEVT_BUTTON, label->set_string_to_current_time);
    
    //If I press reduce, I want all the fields in this PositionFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_add->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lat);
    button_add->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lon);
    button_add->Bind(wxEVT_BUTTON, &StringField<PositionFrame>::get<wxCommandEvent>, label);
    button_add->Bind(wxEVT_BUTTON, &PositionFrame::OnPressAdd, this);
    
    
    //I enable the reduce button only if position_in is a valid position with the entries propely filled, i.e., only if position_in != NULL
    button_add->Enable((position_in != NULL));
    
    sizer_grid_measurement->Add(text_lat, 0, wxALIGN_CENTER_VERTICAL);
    lat->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_measurement->Add(text_lon, 0, wxALIGN_CENTER_VERTICAL);
    lon->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);
    
    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer_2->Add(button_add, 0, wxALIGN_BOTTOM);
    
    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Coordinates");
    
    sizer_box_measurement->Add(sizer_grid_measurement);
    
    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_lat->SetMinSize(wxSize(common_width,-1));
    text_lon->SetMinSize(wxSize(common_width,-1));
    text_label->SetMinSize(wxSize(common_width,-1));
    
    //add the various elements to sizer, by inserting a border of 5 in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, 5);
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, 5);
    
    
    //panel->SetSizer(sizer);
    Maximize(panel);
    
    CurrentDocPath = wxT("");
    
    CreateStatusBar();
    SetStatusText( "Welcome to Michele's text editor!" );
    
    SetSizerAndFit(sizer);
    //Maximize();
    
    
    if(!check){
        cout << prefix.value << RED << "Cannot read position!\n" << RESET;
    }
    
    
    
    if(position_in != NULL){set();}
    
    Centre();
    
    
}

//set all the GUI fields in this equal to those in the non-GUI object this->position
void PositionFrame::set(void){
    
    lat->set();
    lon->set();
    label->set();
    
}

//set all the non-GUI objects in this->position  equal to those in the GUI fields
template<class T> void PositionFrame::get(T &event){
    
    lat->get(event);
    lon->get(event);
    label->get(event);
    
}

void PositionFrame::SetIdling(bool b){
    
    idling = b;
    
}

void PositionFrame::OnPressCancel(wxCommandEvent& event){
    
    Close(TRUE);
    
}


void PositionFrame::OnPressAdd(wxCommandEvent& event){
    
    stringstream s;
    
    //writes the values of the GUI fields in the non-GUI fields
    get(event);
    
    position->print(String("position entered via GUI"), String(""), cout);
    
    //if the constructor of PositionFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of position_list
    if(list_position==-1){
        (((this->parent)->plot)->position_list).push_back(*position);
    }
    
    position->add_to_wxListCtrl(list_position, ((this->parent)->listcontrol_positions));
    
    event.Skip(true);
    
    Close(TRUE);
    
}




//this function checks whether all the fields in PositionFrame are ok, and if they are, it enables the button_add
void PositionFrame::TryToEnableReduce(void){
    
    button_add->Enable((lat->is_ok()) && (lon->is_ok()));
    
}



MessageFrame::MessageFrame(wxWindow* parent, const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent, wxID_ANY, title, pos, size){
    
    wxDisplay display;
    wxPNGHandler *handler;
    wxRect rectangle;
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    //image
    handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*1./1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*1./1000.0);
    
    //allocate sizers
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(3, 1, 0, 0);
    
    
    wxStaticText* text = new wxStaticText(panel, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //buttons
    button_ok = new wxButton(panel, wxID_ANY, "Ok!", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_ok->Bind(wxEVT_BUTTON, &MessageFrame::OnPressOk, this);
    
    
    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(path_file_app_icon, wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    
    
    sizer_grid->Add(text, 0, wxALIGN_CENTER);
    sizer_grid->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    sizer_buttons->Add(button_ok, 0, wxALIGN_CENTER);
    sizer_grid->Add(sizer_buttons, 0, wxALIGN_CENTER);
    
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
    
    CentreOnScreen();
    
    
}

template<typename F_YES, typename F_NO> QuestionFrame<F_YES, F_NO>::QuestionFrame(wxWindow* parent, F_YES* f_yes_in, F_NO* f_no_in, const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent, wxID_ANY, title, pos, size){
    
    wxDisplay display;
    wxPNGHandler *handler;
    wxRect rectangle;
    //initialize the functor to close thie QuestionFrame when button_yes or button_no will be pressed
    CloseFrame<QuestionFrame>* close_frame;
    
    f_yes = f_yes_in;
    f_no = f_no_in;
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame<QuestionFrame<F_YES, F_NO>>(this);
    
    //image
    handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*1./1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*1./1000.0);
    
    //allocate sizers
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(3, 1, 0, 0);
    
    wxStaticText* text = new wxStaticText(panel, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //buttons
    button_yes = new wxButton(panel, wxID_ANY, "Yes", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_yes->Bind(wxEVT_BUTTON, *f_yes);
    //    button_yes->Bind(wxEVT_BUTTON, *close_frame);
    button_no = new wxButton(panel, wxID_ANY, "No", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_no->Bind(wxEVT_BUTTON, *f_no);
    button_no->Bind(wxEVT_BUTTON, *close_frame);
    
    
    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(path_file_app_icon, wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    
    sizer_grid->Add(text, 0, wxALIGN_CENTER);
    sizer_grid->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    sizer_buttons->Add(button_yes, 0, wxALIGN_CENTER);
    sizer_buttons->Add(button_no, 0, wxALIGN_CENTER);
    sizer_grid->Add(sizer_buttons, 0, wxALIGN_CENTER);
    
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
    
    CentreOnScreen();
    
}

template<class T> PrintErrorMessage<T>::PrintErrorMessage(T* f_in){
    
    f = f_in;
    
}


ListFrame::ListFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(NULL, wxID_ANY, title, pos, size){
    
    unsigned int i, total_column_width, n_columns_listcontrol_sights, n_columns_listcontrol_positions/*, margin_h = 10*/, margin_v = 5;
    OnSelectInListControlSights on_select_in_listcontrol_sights;
    OnSelectInListControlPositions on_select_in_listcontrol_positions;
    wxListItem column, item;
    
    
    (on_select_in_listcontrol_sights.f) = this;
    (on_select_in_listcontrol_positions.f) = this;
    
    //initialize delete_sight, which defines the functor to delete the sight but not its related route (it is called when the user answers 'n' to QuestionFrame)
    delete_sight = new DeleteSight(Answer('n', String("")));
    (delete_sight->f) = this;
    //initialize delete_sight, which defines the functor to delete the sight and its related route (it is called when the user answers 'y' to QuestionFrame)
    delete_sight_and_related_route = new DeleteSight(Answer('y', String("")));
    (delete_sight_and_related_route->f) = this;
    
    catalog = new Catalog(String(path_file_catalog), String(""));
    plot = new Plot(catalog, String(""));
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_buttons_sight = new wxBoxSizer(wxHORIZONTAL);
    sizer_buttons_position = new wxBoxSizer(wxHORIZONTAL);
    sizer_box_sight = new wxStaticBoxSizer(wxVERTICAL, panel, "Sights");
    sizer_box_position = new wxStaticBoxSizer(wxVERTICAL, panel, "Positions");
    
    //
    //here I read a sample sight from file_sample_sight, store into sight and set all the fields in this to the data in sight with set()
    File file_sample_sight;
    file_sample_sight.set_name(String("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample_sight.txt"));
    plot->read_from_file(file_sample_sight, String(""));
    plot->print(true, String(""), cout);
    //
    
    
    
    //listcontrol_sights with sights
    listcontrol_sights = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize((this->GetSize()).GetWidth()*0.95 ,  -1), wxLC_REPORT);
    listcontrol_sights->Bind(wxEVT_LIST_ITEM_SELECTED, on_select_in_listcontrol_sights);
    
    
    n_columns_listcontrol_sights = 11;
    
    column.SetId(0);
    column.SetText(wxT("Body"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(0, column);
    
    column.SetId(1);
    column.SetText(wxT("Limb"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(1, column);
    
    column.SetId(2);
    column.SetText(wxT("Artificial horizon"));
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    column.SetAlign(wxLIST_FORMAT_LEFT);
    listcontrol_sights->InsertColumn(2, column);
    
    column.SetId(3);
    column.SetText(wxT("Sextant altitude"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(3, column);
    
    column.SetId(4);
    column.SetText(wxT("Index error"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(4, column);
    
    column.SetId(5);
    column.SetText(wxT("Height of eye"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(5, column);
    
    column.SetId(6);
    column.SetText(wxT("Master-clock date and hour (UTC)"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(6, column);
    
    column.SetId(7);
    column.SetText(wxT("Stopwatch"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(7, column);
    
    column.SetId(8);
    column.SetText(wxT("Stopwatch reading"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(8, column);
    
    column.SetId(9);
    column.SetText(wxT("TAI - UTC"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(9, column);
    
    column.SetId(10);
    column.SetText(wxT("Label"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_sights->GetSize()).GetWidth()/n_columns_listcontrol_sights);
    listcontrol_sights->InsertColumn(10, column);
    
    
    //write the sights into plot->sight_list into listcontrol_sights
    for(i=0; i<((plot->sight_list).size()); i++){
        
        ((plot->sight_list)[i]).add_to_wxListCtrl(-1, listcontrol_sights);
        
    }
    
    //    set the column width to the width of its longest item
    for(i=0; i<(listcontrol_sights->GetColumnCount()); i++){
        listcontrol_sights->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER );
    }
    for(total_column_width=0, i=0; i<(listcontrol_sights->GetColumnCount()); i++){
        total_column_width += (listcontrol_sights->GetColumnWidth(i));
    }
    //    cout << "----------------- total Column width = " << total_column_width << "\n";
    //    cout << "listcontrol_sights width = " << (listcontrol_sights->GetSize()).GetWidth() << "\n";
    //    //    listcontrol_sights->SetColumnWidth((listcontrol_sights->GetColumnCount())-1, ((listcontrol_sights->GetSize()).GetWidth()) - total_column_width);
    //
    
    listcontrol_sights->SetMinSize(wxSize(total_column_width,-1));
    
    sizer_box_sight->Add(listcontrol_sights, 0,  wxALL, margin_v);
    
    
    //listcontrol_positions with positions
    listcontrol_positions = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize((this->GetSize()).GetWidth()*0.95 ,  -1), wxLC_REPORT);
    listcontrol_positions->Bind(wxEVT_LIST_ITEM_SELECTED, on_select_in_listcontrol_positions);
    
    n_columns_listcontrol_positions = 3;
    
    column.SetId(0);
    column.SetText(wxT("Latitude"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_positions->GetSize()).GetWidth()/n_columns_listcontrol_positions);
    listcontrol_positions->InsertColumn(0, column);
    
    column.SetId(1);
    column.SetText(wxT("Longitude"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_positions->GetSize()).GetWidth()/n_columns_listcontrol_positions);
    listcontrol_positions->InsertColumn(1, column);
    
    column.SetId(2);
    column.SetText(wxT("Label"));
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth((listcontrol_positions->GetSize()).GetWidth()/n_columns_listcontrol_positions);
    listcontrol_positions->InsertColumn(2, column);
    
    
    //write the sights into plot->sight_list into listcontrol_sights
    for(i=0; i<((plot->position_list).size()); i++){
        ((plot->position_list)[i]).add_to_wxListCtrl(-1, listcontrol_positions);
    }
    
    
    //    set the column width to the width of its longest item
    for(i=0; i<(listcontrol_positions->GetColumnCount()); i++){
        listcontrol_positions->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }
    for(total_column_width=0, i=0; i<(listcontrol_positions->GetColumnCount()); i++){
        total_column_width += (listcontrol_positions->GetColumnWidth(i));
    }
    
    listcontrol_positions->SetMinSize(wxSize(total_column_width,-1));
    
    
    sizer_box_position->Add(listcontrol_positions, 0,  wxALL, margin_v);
    
    
    
    
    //buttons
    //image for buttons
    wxImage::AddHandler(new wxPNGHandler);
    wxBitmap my_bitmap = wxBitmap(wxT(path_file_pencil_icon), wxBITMAP_TYPE_PNG);
    wxImage my_image = my_bitmap.ConvertToImage();
    my_image.Rescale(20,20);
    
    //button to add a sight
    button_add_sight = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_add_sight->Bind(wxEVT_BUTTON, &ListFrame::OnAddSight, this);
    
    //button to add a position
    button_add_position = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_add_position->Bind(wxEVT_BUTTON, &ListFrame::OnAddPosition, this);
    
    //button to modify a sight
    button_modify_sight = new wxBitmapButton(panel, wxID_ANY, wxBitmap(my_image), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT   | wxBORDER_NONE);
    button_modify_sight->Bind(wxEVT_BUTTON, &ListFrame::OnModifySight, this);
    button_modify_sight->Enable(false);
    
    //button to modify a position
    button_modify_position = new wxBitmapButton(panel, wxID_ANY, wxBitmap(my_image), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT   | wxBORDER_NONE);
    button_modify_position->Bind(wxEVT_BUTTON, &ListFrame::OnModifyPosition, this);
    button_modify_position->Enable(false);
    
    //button to delete a sight
    button_delete_sight = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_delete_sight->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteSight, this);
    button_delete_sight->Enable(false);
    
    //button to delete a position
    button_delete_position = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_delete_position->Bind(wxEVT_BUTTON, &ListFrame::OnDeletePosition, this);
    button_delete_position->Enable(false);
    
    
    sizer_buttons_sight->Add(button_add_sight, 0, wxALIGN_CENTER);
    sizer_buttons_sight->Add(button_modify_sight, 0, wxALIGN_CENTER);
    sizer_buttons_sight->Add(button_delete_sight, 0, wxALIGN_CENTER);
    sizer_box_sight->Add(sizer_buttons_sight, 0, wxALIGN_LEFT | wxALL, margin_v);
    
    sizer_buttons_position->Add(button_add_position, 0, wxALIGN_CENTER);
    sizer_buttons_position->Add(button_modify_position, 0, wxALIGN_CENTER);
    sizer_buttons_position->Add(button_delete_position, 0, wxALIGN_CENTER);
    sizer_box_position->Add(sizer_buttons_position, 0, wxALIGN_LEFT | wxALL, margin_v);
    
    
    //
    
    //resize uniformly all column
    //    for(i=0; i<(listcontrol_sights->GetColumnCount()); ++i){
    //        listcontrol_sights->SetColumnWidth(i, ((listcontrol_sights->GetSize()).GetWidth())/(listcontrol_sights->GetColumnCount()));
    //    }
    sizer_v->Add(sizer_box_sight, 0,  wxALL, margin_v);
    sizer_v->Add(sizer_box_position, 0,  wxALL, margin_v);
    //    sizer_v->Add(button_modify_sight, 0,  wxALIGN_LEFT | wxALL, 5);
    //    sizer_v->Add(button_delete_sight, 0, wxALIGN_LEFT | wxALL, 5);
    //    sizer_h->Add(listcontrol_sights, 0, wxALIGN_TOP);
    
    Maximize(panel);
    SetSizerAndFit(sizer_v);
    //    panel->SetSizer(sizer_v);
    
    //    panel->SetSize(wxSize(total_column_width+4*margin_v,-1));
    //    this->SetSize(wxSize(total_column_width+6*margin_v,-1));
    //
}

void ListFrame::OnAddSight(wxCommandEvent& event){
    
    SightFrame *sight_frame = new SightFrame(this, NULL, -1, "New sight", wxDefaultPosition, wxDefaultSize, String(""));
    sight_frame->Show(true);
    
    event.Skip(true);
    
}

void ListFrame::OnAddPosition(wxCommandEvent& event){
    
    PositionFrame *position_frame = new PositionFrame(this, NULL, -1, "New position", wxDefaultPosition, wxDefaultSize, String(""));
    position_frame->Show(true);
    
    event.Skip(true);
    
}


void ListFrame::OnModifySight(wxCommandEvent& event){
    
    long item;
    item = listcontrol_sights->GetNextItem(-1,
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

void ListFrame::OnModifyPosition(wxCommandEvent& event){
    
    long item;
    item = listcontrol_positions->GetNextItem(-1,
                                              wxLIST_NEXT_ALL,
                                              wxLIST_STATE_SELECTED);
    
    if(item != -1){
        
        stringstream s;
        
        s.str("");
        s << "Position #" << item;
        
        PositionFrame *position_frame = new PositionFrame(this, &((plot->position_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        position_frame->Show(true);
        
    }
    
    event.Skip(true);
    
}



void ListFrame::OnPressDeleteSight(wxCommandEvent& event){
    
    //the id of the sight to removed is the one of the sight selected in listcontrol_sights: I write it in delete_sight_and_related_route and in delete_sight
    (delete_sight_and_related_route->i_sight_to_remove) = listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    (delete_sight->i_sight_to_remove) = listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    
    //remove the sight from the GUI object listcontrol_sights
    listcontrol_sights->DeleteItem((delete_sight->i_sight_to_remove));
    
    
    
    //remove the sight from the non-GUI object plot
    //ask the user whether he/she wants to remove the related route as well
    QuestionFrame<DeleteSight, DeleteSight>* question_frame = new QuestionFrame<DeleteSight, DeleteSight>(NULL,
                                                                                                          delete_sight_and_related_route,
                                                                                                          delete_sight,
                                                                                                          "",
                                                                                                          "Do you want to remove the route related to this sight?",
                                                                                                          wxDefaultPosition,
                                                                                                          wxDefaultSize,
                                                                                                          String(""));
    question_frame->Show(true);
    
    event.Skip(true);
    
}

void ListFrame::OnDeletePosition(wxCommandEvent& event){
    
    long item;
    
    item = listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    listcontrol_positions->DeleteItem(item);
    plot->remove_position(item, String(""));
    
    
    event.Skip(true);
    
}

//write into all the non-GUI objects the values of the GUI fields
template<class T> void SightFrame::get(T& event){
    
    body->get(event);
    limb->get(event);
    artificial_horizon_check->get(event);
    H_s->get(event);
    index_error->get(event);
    
    if(!((artificial_horizon_check->checkbox)->GetValue())){
        height_of_eye->get(event);
    }
    
    master_clock_date->get(event);
    master_clock_chrono->get(event);
    
    stopwatch_check->get(event);
    
    if(((stopwatch_check->checkbox)->GetValue())){
        stopwatch_reading->get(event);
    }
    
    TAI_minus_UTC->get(event);
    label->get(event);
    
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





//this function checks whether all the fields in SightFrame are ok, and if they are, it enables the button_reduce
void SightFrame::TryToEnableReduce(void){
    
    
    button_reduce->Enable(
                          (body->is_ok()) &&
                          (( ((body->name->GetValue()) == wxString("sun")) || ((body->name->GetValue()) == wxString("moon")) ) || (limb->is_ok())) &&
                          (H_s->is_ok()) &&
                          (index_error->is_ok()) &&
                          ((((artificial_horizon_check->checkbox)->GetValue())) || (height_of_eye->is_ok())) &&
                          (master_clock_date->is_ok()) &&
                          (master_clock_chrono->is_ok()) &&
                          ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) &&
                          (TAI_minus_UTC->is_ok()));
    
}





void SightFrame::OnPressCancel(wxCommandEvent& event){
    
    
    Close(TRUE);
}




template<class T> void CheckYear::operator()(T&event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->year)->GetValue()).ToStdString(), NULL, false, 0, 0)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->year, String("Entered value is not valid!\nYear must be an unsigned integer"));
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->year);
            ((f->printerrormessage)->title) = String("Entered value is not valid!");
            ((f->printerrormessage)->message) = String("Year must be an unsigned integer");
            f->CallAfter(*(f->printerrormessage));
            
            (p->year_ok) = false;
            (p->day)->Enable(false);
            
            
        }else{
            
            (p->year)->SetBackgroundColour(*wxWHITE);
            (p->year_ok) = true;
            
            if(p->month_ok){
                tabulate_days(event);
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
            ((f->printerrormessage)->control) = (p->month);
            ((f->printerrormessage)->title) = String("Entered value is not valid!");
            ((f->printerrormessage)->message) = String("Month must be an unsigned integer >= 1 and <= 12");
            f->CallAfter(*(f->printerrormessage));
            
            (p->month_ok) = false;
            (p->day)->Enable(false);
            
        }else{
            
            (p->month)->SetBackgroundColour(*wxWHITE);
            (p->month_ok) = true;
            
            if(p->year_ok){
                
                tabulate_days(event);
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
                (p->day_ok) = true;
                
            }else{
                
                //            f->CallAfter(&SightFrame::PrintErrorMessage, p->day, String("Entered value is not valid!\nDay must be an unsigned integer comprised between the days of the relative month"));
                
                //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
                ((f->printerrormessage)->control) = (p->day);
                ((f->printerrormessage)->title) = String("Entered value is not valid!");
                ((f->printerrormessage)->message) = String("Day must be an unsigned integer comprised between the days of the relative month");
                f->CallAfter(*(f->printerrormessage));
                
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



//this function reads the value in the GUI box checkbox, and enables/disables the related_field accordingly
template<class T> template<class R> void CheckCheck<T>::operator()(R& event){
    
    //I enable/disable related_field according to whether checkbox is checked or not, and according to the value of direct_reverse
    if((((p->checkbox)->GetValue()) ^ (!(p->direct_reverse)))){
        (p->related_field)->Enable(true);
        //I write into the related_field by setting its variable just_enabled to true: this means that no error message will be prompted when the user sets its focus to the related field GUIs
        ((p->related_field)->just_enabled) = true;
    }else{
        (p->related_field)->Enable(false);
    }
    
    (p->parent_frame)->TryToEnableReduce();
    
    event.Skip(true);
    
}


//this function writes into the non-GUI field answer the value entered in the GUI box
template<class T> template<class S> void CheckField<T>::get(S& event){
    
    //I set p->answer to the value entered in the GUI checkbox
    if(checkbox->GetValue()){
        (answer->value) = 'y';
    }else{
        (answer->value) = 'n';
    }
    
    event.Skip(true);
    
}


template<class T> void CheckHour::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24) && ((p->hour)->IsEnabled())){
            
            if(!(p->just_enabled)){
                //if the content of the GUI field p is invalid and p has not been just enabled, then I am authorized to prompt an error message
                
                //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
                ((f->printerrormessage)->control) = (p->hour);
                ((f->printerrormessage)->title) = String("Entered value is not valid!");
                ((f->printerrormessage)->message) = String("Hours must be unsigned integer numbers >= 0 and < 24");
                f->CallAfter(*(f->printerrormessage));
                
            }else{
                //if the ChronoField p has just been enabled, I do not print any error message even if the content of p is invalid: this is because I want to give the opportunity to the user to enter the content of the GUI field before complaining that the content of the GUI field is invalid. However, I set just_enabled to false, because p is no longer just enabled.
                
                
                (p->just_enabled) = false;
                
                
            }
            
            
            (p->hour_ok) = false;
            
        }else{
            
            (p->hour)->SetBackgroundColour(*wxWHITE);
            (p->hour_ok) = true;
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}


template<class T> void CheckMinute::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60) && ((p->minute)->IsEnabled())){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->minute), String("Entered value is not valid!\nMinutes must be unsigned integer numbers >= 0 and < 60"));
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->minute);
            ((f->printerrormessage)->title) = String("Entered value is not valid!");
            ((f->printerrormessage)->message) = String("Minutes must be unsigned integer numbers >= 0 and < 60");
            f->CallAfter(*(f->printerrormessage));
            
            (p->minute_ok) = false;
            
        }else{
            
            (p->minute)->SetBackgroundColour(*wxWHITE);
            (p->minute_ok) = true;
            
        }
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}



template<class T> void CheckSecond::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0) && ((p->second)->IsEnabled())){
            
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->second, String("Entered value is not valid!\nSeconds must be floating-point numbers >= 0.0 and < 60.0"));
            
            //set the wxControl, title and message for the functor printerrormessage, and then call the functor with CallAfter
            ((f->printerrormessage)->control) = (p->second);
            ((f->printerrormessage)->title) = String("Entered value is not valid!");
            ((f->printerrormessage)->message) = String("Seconds must be floating-point numbers >= 0.0 and < 60.0");
            f->CallAfter(*(f->printerrormessage));
            
            (p->second_ok) = false;
            
        }else{
            
            
            (p->second)->SetBackgroundColour(*wxWHITE);
            (p->second_ok) = true;
            
        }
        
        
        f->TryToEnableReduce();
        
    }
    
    event.Skip(true);
    
}


//this function writes into sight.stopwatch the value written into the respective GUI box
template <class T> void CheckChrono::operator()(T& event){
    
    check_hour(event);
    check_minute(event);
    check_second(event);
    
    event.Skip(true);
    
}

//this function writes into the non-GUI fields in chrono the value written into the respective GUI fields hour, minute and second
template <class T> void ChronoField::get(T& event){
    
    if(hour_ok && (hour->IsEnabled()) && minute_ok && (minute->IsEnabled()) && second_ok && (second->IsEnabled())){
        //I write only if hour, minute and second are ok and enabled
        
        double s_temp;
        
        (chrono->h) = ((unsigned int)wxAtoi(hour->GetValue()));
        ((chrono)->m) = ((unsigned int)wxAtoi(minute->GetValue()));
        ((second)->GetValue()).ToDouble(&s_temp);
        ((chrono)->s) = s_temp;
        
    }
    
    event.Skip(true);
    
}


void SightFrame::OnPressReduce(wxCommandEvent& event){
    
    stringstream s;
    
    //writes the values of the GUI fields in the non-GUI fields
    get(event);
    
    sight->print(String("sight entered via GUI"), String(""), cout);
    
    //if the constructor of SightFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of sight_list
    if(list_position==-1){
        (((this->parent)->plot)->sight_list).push_back(*sight);
    }
    
    sight->add_to_wxListCtrl(list_position, ((this->parent)->listcontrol_sights));
    
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

//sets the value in the non-GUI object body equal to the value in the GUI  object name
template<class T> void BodyField::get(T& event){
    
    unsigned int i;
    bool check;
    
    if(ok){
        //If the GUI field's content is ok...
        
        //I find the position of the content of the GUI field in the list of  the body names in catalog
        for(check=false, i=0; (i<(catalog->list).size()) && (!check); i++){
            if(String((name->GetValue().ToStdString())) == (((catalog->list)[i]).name)){
                check = true;
            }
        }
        i--;
        
        //I set the value of the non-GUI object body to the value obtained from the GUI object.
        (*body) = (catalog->list)[i];
        
    }
    
    event.Skip(true);
    
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
template<class T> void CheckField<T>::set(void){
    
    if((answer->value) == 'y'){
        checkbox->SetValue(true);
    }
    
    if((answer->value) == 'n'){
        checkbox->SetValue(false);
    }
    
}

//sets the value in the GUI objects deg and min equal to the value in the non-GUI limb object angle
template <class P> void AngleField<P>::set(void){
    
    unsigned int deg_temp;
    double min_temp;
    Angle angle_temp;
    
    
    if(format == String("")){
        
        angle->to_deg_min(&deg_temp, &min_temp);
        
    }else{
        //in this case format = +-, EW or NS
        
        if((angle->value) < M_PI){
            
            if(format==String("+-")){sign->SetValue(wxString("+"));}
            if(format==String("EW")){sign->SetValue(wxString("W"));}
            if(format==String("NS")){sign->SetValue(wxString("N"));}
            
            angle->to_deg_min(&deg_temp, &min_temp);
            
        }else{
            
            if(format==String("+-")){sign->SetValue(wxString("-"));}
            if(format==String("EW")){sign->SetValue(wxString("E"));}
            if(format==String("NS")){sign->SetValue(wxString("S"));}
            
            (angle_temp.value) = 2.0*M_PI - (angle->value);
            angle_temp.to_deg_min(&deg_temp, &min_temp);
            
        }
        
    }
    
    //all the cases above must share these lines, so I put them here
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
template<class P> void StringField<P>::set(void){
    
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
template<class T> CheckField<T>::CheckField(SightFrame* frame, Answer* p, T* related_field_in, bool direct_reverse_in){
    
    parent_frame = frame;
    //I link the internal pointers p and c to the respective Answer object
    answer = p;
    related_field = related_field_in;
    direct_reverse = direct_reverse_in;
    
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
template <class P> AngleField<P>::AngleField(P* parent_in, Angle* p, String format_in){
    
    unsigned int i;
    parent_frame = parent_in;
    angle = p;
    
    format = format_in;
    
    signs.Clear();
    degrees.Clear();
    if(format == String("+-")){
        //tabulate signs
        signs.Add(wxString("+"));
        signs.Add(wxString("-"));
    }
    if(format == String("NS")){
        signs.Add(wxString("N"));
        signs.Add(wxString("S"));
    }
    if(format == String("EW")){
        //here W <-> + sign, E <-> - sign
        signs.Add(wxString("E"));
        signs.Add(wxString("W"));
    }
    
    //tabuate degrees
    if((format == String(""))){
        for(i=0; i<360; i++){
            degrees.Add(wxString::Format(wxT("%i"), i));
        }
    }else{
        if(format == String("NS")){
            for(i=0; i<=90; i++){
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }else{
            for(i=0; i<=180; i++){
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }
    }
    
    
    
    //initialize check and its objects
    check_angle = new CheckAngle<P>(this);
    
    //here the allocation of sign is inserted in the code in such a way that if format = "+-" the sign is allocated before deg, text_deg, min, text_min: In this way, when the user tabs through the fields in PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if(format == String("+-")){
        sign = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
        AdjustWidth(sign);
        sign->Bind(wxEVT_KILL_FOCUS, (check_angle->check_sign));
    }
    
    deg = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    AdjustWidth(deg);
    deg->Bind(wxEVT_KILL_FOCUS, (check_angle->check_arc_degree));
    
    text_deg = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("° "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    min = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS(sample_width_floating_point_field))));
    min->Bind(wxEVT_KILL_FOCUS, (check_angle->check_arc_minute));
    
    text_min = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("' "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //here the allocation of sign is inserted in the code in such a way that if format = "NS" || "EW" the sign is allocated after deg, text_deg, min, text_min: In this way, when the user tabs through the fields in the PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if((format == String("NS")) || (format == String("EW"))){
        sign = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
        AdjustWidth(sign);
        sign->Bind(wxEVT_KILL_FOCUS, (check_angle->check_sign));
    }
    
    if(format != String("")){sign->SetValue(wxString(""));}
    deg->SetValue(wxString(""));
    min->SetValue(wxString(""));
    sign_ok = false;
    deg_ok = false;
    min_ok = false;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    if(format != String("")){
        
        if(format == String("+-")){
            
            //in this case I display the sign before the numerical value of the angle
            sizer_h->Add(sign, 0, wxALIGN_LEFT);
            
            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);
            
        }else{
            
            //in this case I display the sign after the numerical value of the angle
            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);
            
            sizer_h->Add(sign, 0, wxALIGN_LEFT);
            
        }
    }else{
        
        //in this case I display only the numerical value of the angle
        sizer_h->Add(deg, 0, wxALIGN_CENTER);
        sizer_h->Add(text_deg);
        sizer_h->Add(min, 0, wxALIGN_CENTER);
        sizer_h->Add(text_min);
        
    }
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
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
template<class P> StringField<P>::StringField(P* parent_in, String* p){
    
    parent_frame = parent_in;
    string = p;
    
    //initialize check
    (check.p) = this;
    
    (set_string_to_current_time.p) = this;
    
    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_string_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, check);
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);
    
}




template<class P> bool AngleField<P>::is_ok(void){
    
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
    (((check.check_year).tabulate_days).p) = this;
    ((check.check_month).p) = this;
    (((check.check_month).tabulate_days).p) = this;
    ((check.check_day).p) = this;
    
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
    
    (check.p) = this;
    ((check.check_hour).p) = this;
    ((check.check_minute).p) = this;
    ((check.check_second).p) = this;
    
    for(hours.Clear(), hours.Add(wxT("")), i=0; i<24; i++){
        hours.Add(wxString::Format(wxT("%i"), i+1));
    }
    for(minutes.Clear(), minutes.Add(wxT("")), i=0; i<60; i++){
        minutes.Add(wxString::Format(wxT("%i"), i+1));
    }
    
    hour = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN);
    //    hour->SetInitialSize(hour->GetSizeFromTextSize(hour ->GetTextExtent(wxS("00"))));
    AdjustWidth(hour);
    hour->Bind(wxEVT_KILL_FOCUS, (check.check_hour));
    
    text_colon_1 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize);
    
    minute = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN);
    AdjustWidth(minute);
    //    minute->SetInitialSize(minute->GetSizeFromTextSize(minute->GetTextExtent(wxS("00"))));
    minute->Bind(wxEVT_KILL_FOCUS, (check.check_minute));
    
    text_colon_2 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize);
    
    second = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxCB_DROPDOWN);
    second->SetInitialSize(second->GetSizeFromTextSize(second->GetTextExtent(wxS(sample_width_floating_point_field))));
    second->Bind(wxEVT_KILL_FOCUS, (check.check_second));
    
    
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

////this is called when the yes button is pressed in QuestionFrame
//template<typename F> void QuestionFrame<F>::OnPressYes(wxCommandEvent& event){
//
//    //set the answer variable to yes
//    answer->set(String("answer set to "), 'y', String("//////////////// "));
//
//    //calls the functor which is supposed to be called when button_yes is pressed
//    (*f_yes)(event);
//
//
//    event.Skip(true);
//
//    Close(TRUE);
//
//}
//
////this is called when the button No is pressed in QuestionFrame
//template<typename F> void QuestionFrame<F>::OnPressNo(wxCommandEvent& event){
//
////    answer->set(String(""), 'n', String(""));
//
//    Close(TRUE);
//
//}

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

template<class T> template<class R> void CheckField<T>::InsertIn(R* host){
    
    host->Add(sizer_v);
    
}


template<class P> template<class T> void AngleField<P>::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void LengthField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}


template<class T> void DateField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class P> template<class T> void StringField<P>::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void ChronoField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}
