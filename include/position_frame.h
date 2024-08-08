//
//  position_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef position_frame_h
#define position_frame_h

#include <iostream>

#include "position.h"

using namespace std;


template<class P> class StringField;


class PositionFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    Position* position;
    //this long represents the position in the list (this->GetParent())->listcontrol_positions of position. If list_position = -1, then sight is not in that list
    long position_in_listcontrol_positions;
    wxPanel *panel;
    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling;
    
    SetIdling<PositionFrame>* set_idling;
    UnsetIdling<PositionFrame>* unset_idling;
    PrintMessage<PositionFrame, UnsetIdling<PositionFrame> >* print_error_message;
    
    AngleField<PositionFrame>* lat, *lon;
    StringField<PositionFrame> *label;
    
    wxFlexGridSizer *sizer_grid_measurement, *sizer_grid_label;
    wxBoxSizer *sizer, *box_sizer_2;
    wxStaticBoxSizer *sizer_box_measurement;
    
    wxButton* /*button_ok denotes a button which triggers either an 'Add' command, when one is adding a new position, or a 'Modify' command, when one is modifying a position whih already exists*/button_ok, *button_cancel;
    wxMenuBar *menuBar;
    
    PositionFrame(ListFrame*, Position*, long, const wxString&, const wxPoint&, const wxSize&, String);
    void set(void);
    template<class T> void get(T&);
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    bool is_ok(void);
    void AllOk(void);
    void KeyDown(wxKeyEvent&);
    
    //    wxDECLARE_EVENT_TABLE();
    
};


#endif
