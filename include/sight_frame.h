//
//  sight_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef sight_frame_h
#define sight_frame_h



class SightFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    Catalog* catalog;
    Sight* sight;
    //this long represents the position in the list (this->GetParent())->listcontrol_sights of sight. If list_position = -1, then sight is not in that list
    long position_in_listcontrol_sights;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling, /*this is true if all the GUI fileds which specify sight->time are ok and the resulting sight->time falls within the time covered by ephemerids' data files, and false otherwise*/time_interval_ok;
    //this messages displays a text on SightFrame if the time interval entered by the user does not lie within the interval covered by the ephemerides' data
//    StaticText* text_time_interval_not_ok;
    //image shown on SightFrame if the time interval entered by the user does not lie within the interval covered by the ephemerides' data
//  ?  wxStaticBitmap* image_time_interval_status;
    wxImage* image_time_interval_not_ok;
    
    SetIdling<SightFrame>* set_idling;
    UnsetIdling<SightFrame>* unset_idling;
    PrintMessage<SightFrame, UnsetIdling<SightFrame> >* print_error_message;
    
    BodyField<SightFrame>* body;
    LimbField<SightFrame>* limb;
    CheckField<SightFrame, DynamicLengthField<SightFrame> >* artificial_horizon_check;
    CheckField<SightFrame, ChronoField<SightFrame> >* stopwatch_check;
    AngleField<SightFrame>* H_s, *index_error;
    DynamicLengthField<SightFrame>* height_of_eye;
    DateField<SightFrame>* master_clock_date;
    ChronoField<SightFrame>* master_clock_chrono, *stopwatch_reading, *TAI_minus_UTC;
    StringField<SightFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_time, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2, *box_sizer_3, *box_sizer_4;
    wxStaticBoxSizer *sizer_box_measurement, *sizer_box_time;
    
    wxButton* button_reduce, *button_cancel;
    wxMenuBar *menuBar;
    
    SightFrame(ListFrame*, Sight*, long, const wxString&, const wxPoint&, const wxSize&, String);
    void set(void);
    bool is_ok();
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressReduce(wxCommandEvent& event);
    void AllOk(void);
    template<class E> void AllOk(E&);
    //    template<class T> void OnEditTime(T&);
    void TimeIntervalOk(String);
    void KeyDown(wxKeyEvent&);
    template<class E> void Check(E&);
    void Check();
    template<class E> void update_recent_items(E&);

    //    wxDECLARE_EVENT_TABLE();
    
};

#endif
