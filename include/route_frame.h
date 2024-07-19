//
//  route_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef route_frame_h
#define route_frame_h

#include <iostream>

#include "angle_field.h"
#include "chrono_field.h"
#include "dynamic_length_field.h"
#include "length_format_field.h"
#include "list_frame.h"
#include "route.h"
#include "route_type_field.h"
#include "set_idling.h"
#include "speed_field.h"
#include "unset_idling.h"


using namespace std;

template<class P> class DynamicLengthField;
class ListFrame;
template<class P> class SpeedField;
template<class P> class StringField;



class RouteFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    Route* route;
    //this long represe5nts the position in the list (this->GetParent())->listcontrol_routes of route. If list_position = -1, then route is not in that list
    long position_in_listcontrol_routes;
    wxPanel *panel;
    bool /*idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false*/idling, /*for_transport = true (false) means that the Route is meant to transport objects*/for_transport;
    
    SetIdling<RouteFrame> * set_idling;
    UnsetIdling<RouteFrame> * unset_idling;
    PrintMessage<RouteFrame, UnsetIdling<RouteFrame> >* print_error_message;
    RouteTypeField<RouteFrame>* type;
    AngleField<RouteFrame> *Z, *omega, *start_phi, *start_lambda, *GP_phi, *GP_lambda;
    //l_format tells whether the length of Route is written simply as a Length, or as a Speed x a Chrono (a time)
    LengthFormatField<RouteFrame>* length_format;
    //if the length of the Route is written directly as a length, this field is used
    DynamicLengthField<RouteFrame> *length;
    //if the lenght of the Route is written in terms of a speed multiplied by a time, the following two fields are used
    ChronoField<RouteFrame> *time;
    SpeedField<RouteFrame> *speed;
    StringField<RouteFrame> *label;
    
    wxFlexGridSizer *sizer_grid_type, *sizer_grid_Z, *sizer_grid_t_v, *sizer_grid_l, *sizer_grid_omega, *sizer_grid_start, *sizer_grid_GP, *sizer_grid_label;
    wxBoxSizer *sizer, /*this sizer containts the Length format GUI field, the Length GUI field and the speed and time GUI fields*/*sizer_l_format_l_t_v, *box_sizer;
    wxStaticBoxSizer *sizer_box_data, *sizer_box_l_format_l_t_v, *sizer_box_l, *sizer_box_t_v, *sizer_box_start, *sizer_box_GP;
    //static texts for the GUI fields containing the Route length, speed and time
    wxStaticText *text_time, *text_speed, *text_length;
    wxButton* /*this button triggers either the addition of a new Route, or the modification of an existing one*/button_ok, *button_cancel;
    
    RouteFrame(ListFrame*, Route*, bool, long, const wxString&, const wxPoint&, const wxSize&, String);
    void set(void);
    template<class T> void get(T&);
    template<class E> void OnChooseLengthFormatField(E&);
    template<class E> void UpdateLength(E&);
    void OnChooseLengthFormatField();
    void OnPressCancel(wxCommandEvent& event);
    void OnPressOk(wxCommandEvent& event);
    bool is_ok(void);
    void AllOk(void);
    template<class E> void AllOk(E&);
    void KeyDown(wxKeyEvent&);
    template<class E> void Check(E&);
    void Check(void);
        
};



#endif
