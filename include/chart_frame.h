//
//  chart_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef chart_frame_h
#define chart_frame_h

#include <iostream>

#include "angle.h"
#include "draw_panel.h"
#include "unset_idling.h"
#include "chart_panel.h"
#include "print_message.h"
#include "projection.h"
#include "projection_field.h"
#include "list_frame.h"
#include "static_length_field.h"
#include "static_text.h"

using namespace std;


class Angle;
class DrawPanel;
class ListFrame;
class ChartPanel;
template<class P> class StaticLengthField;

class ChartFrame: public wxFrame{
    
public:
    
    ListFrame* parent;
    DrawPanel *draw_panel;
    ChartPanel* panel;
    StaticText *chart_scale;
    //a static text displaying the height of the observer
    StaticLengthField<ChartFrame>* observer_height;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_slider;
    wxGridSizer* sizer_buttons;
    wxStaticBitmap* image;
    wxSlider* slider;
    wxBitmapButton *button_show_list, *button_reset;
    wxButton* button_up, *button_down, *button_left, *button_right;
    ProjectionField<ChartFrame>* projection;
    PrintMessage<ChartFrame, UnsetIdling<ChartFrame> >* print_error_message;
    /*the latitude/longitude setting the boundaries of the plotted area on earth*/Angle *phi_min, *phi_max, /*it is not necessarily true that lambda_min < lambda_max: lambda_min(max) correspond to the left(right) edge of the plot area*/*lambda_min, *lambda_max;
    //a vector of the points of the coastlines in DrawPanel coordinates: these are points of polygons concatenated in the linear arrays coastline_polygons_now/ before. The i-th polygon is stored n coastline_polygons_now/before[polygon_position_now/before[i]], coastline_polygons_now[polygon_position_now/before[i]+1], ..., coastline_polygons_now[polygon_position_now/before[i+1]-1]
    vector<wxPoint> coastline_polygons_now, coastline_polygons_before;
    vector<unsigned long long int> polygon_position_now, polygon_position_before;

    //idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false
    bool idling, /*this is true if the user is currently scrolling*/mouse_scrolling, /*this is true if the chart is being dragged, and thus the size of *this must not change across multiple Draw(s), and false otherwise*/ dragging_chart;
    //This is the actual value of the maximal zoom factor allowed
    Double /*the zoom factor relative to the default configuration of either projection, the zoom factor  is not necessarily equal to the numerical value (slider->GetValue()) shown on the slider*/zoom_factor;
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither ChartFrame::UpdateSliderLabel_Mercator or ChartFrame::UpdateSliderLabel_3D, according to my needs, and similarly for the other pointers
    void (ChartFrame::*UpdateSliderLabel)(void);
    
    ChartFrame(ListFrame*, Projection, const wxString&, const wxPoint&, const wxSize&, String);

    void GetCoastLineData_Mercator(void);
    void GetCoastLineData_3D(void);
    void UpdateSlider(void);
    bool ComputeZoomFactor_Mercator(double);
    bool ComputeZoomFactor_3D(void);
    void UpdateSliderLabel_Mercator(void);
    void UpdateSliderLabel_3D(void);
    template<class T> void OnMouseLeftDownOnSlider(T&);
    template<class T> void OnMouseLeftUpOnSlider(T&);
    template<class T> void OnScroll(T&);
    bool SetSlider(unsigned int);
    template<class T> void OnPressCtrlW(T&);
    void SetIdling(bool);
    void AllOk(void);
    template<class T> void AllOk(T&);
    void EnableAll(bool);
    
    template<class T> void MoveNorth(T&);
    template<class T> void MoveSouth(T&);
    template<class T> void MoveWest(T&);
    template<class T> void MoveEast(T&);
    template<class T> void KeyDown(T&);
    template<class T> void Reset(T&);
    template<class T> void ResetRender(T&);
    template<class T> void ResetRenderAnimate(T&);
    void Animate(void);
    
    //this is created for testing
    UnsetIdling<ChartFrame> *unset_idling;
    
    
    
    
};


#endif