//
//  list_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef list_frame_h
#define list_frame_h

#include <iostream>

#include "answer.h"
#include "catalog.h"
#include "chart_frame.h"
#include "data.h"
#include "list_control.h"
#include "my_app.h"
#include "position.h"
#include "position_projection.h"
#include "position_rectangle.h"
#include "print_message.h"
#include "route.h"
#include "route_frame.h"
#include "sight.h"
#include "set_idling.h"
#include "unset_idling.h"

using namespace std;

class AskRemoveRelatedSight;
class AskRemoveRelatedRoute;
class Cartesian;
class Catalog;
class ChartFrame;
template <class P> class CloseFrame;
template <class P> class ConfirmTransport;
class Data;
class DeletePosition;
class DeleteRoute;
class DeleteSight;
class ExistingRoute;
template<class P> class ListControl;
class NewRoute;
template<class P> class OnChangeSelectionInListControl;
class OnSelectRouteInListControlRoutesForTransport;
class OnNewRouteInListControlRoutesForTransport;
class PositionRectangle;
class Route;
class RouteFrame;
class SelectRoute;
template<class T, typename F_YES, typename F_NO, typename F_ABORT> class ShowQuestionFrame;
class Sight;



//this is a wxFrame designed to contain the list of sights, routes, etc...
class ListFrame: public wxFrame{
    
public:
    
    Data /*pointer to the non-GUI object Data which is related to the GUI object this*/*data;
    //point to the child frame of this
    vector<ChartFrame*> chart_frames;
    vector<Route> /*when I transport by using an existing Route, I will store the list of Routes which can be used for transport here*/route_list_for_transport, /*when I transport by using an existing Route, I will save data->list_routes temporarily here and recover it from here after the transport */route_list_saved;
    Route /*the same as draw_panel->circle_observer, but at the initial configuration of the chart*/*circle_observer_0, *transporting_route_saved;
    wxMenuBar *menu_bar;
    wxMenu *menu_app, *menu_file, *menu_chart, *menu_new_chart, *menu_item_mercator, *menu_item_3d;
    ListControl<Sight> *listcontrol_sights;
    ListControl<Route>* listcontrol_routes;
    ListControl<Position>* listcontrol_positions;
//    //this is used to transport objects listed in *this
//    GraphicalFeatureTransportHandler* transport_handler;
    //extract_colors is created only to detect the background and foreground colors set by the operating systems, it has no other use. As a result, its size will be set to zero.
    wxTextCtrl* extract_colors;
    wxPanel *panel;
    //a dummy RouteFrame which will be used for various purposes
    RouteFrame* route_frame;
    wxButton  *button_delete_position, *button_delete_route, *button_show_map, *button_compute_position;
    wxBitmapButton *button_add_sight, *button_add_position, *button_add_route, *button_modify_sight, *button_delete_sight, *button_transport_sight, *button_transport_position, *button_modify_position, *button_modify_route, *button_transport_route, *button_disconnect_sight, *button_disconnect_route;
    wxBoxSizer *sizer_all, *sizer_h, *sizer_v;
    wxSizer *sizer_buttons_sight, *sizer_buttons_position, *sizer_buttons_route;
    wxStaticBoxSizer* sizer_box_sight, *sizer_box_position, *sizer_box_route;
    DeleteSight *delete_sight, *delete_sight_and_related_route;
    DeleteRoute *delete_route, *delete_route_and_related_sight;
    DeletePosition *delete_position;
    wxPoint /*the instantaneous positions of the mouse with respect to the screen: this position is kept to the right value by DrawPanel::OnMouseMovement method, that is called every time the mouse moves. This variable belongs to ListFrame rather than to single ChartFrames or DrawPanel, because there is a unique screen position for all of them*/ screen_position_now, screen_position_before;
    OnChangeSelectionInListControl<Sight>* on_change_selection_in_listcontrol_sights; OnChangeSelectionInListControl<Route>* on_change_selection_in_listcontrol_routes; OnChangeSelectionInListControl<Position>* on_change_selection_in_listcontrol_positions;
    ExistingRoute *existing_route;
    NewRoute *new_route;
    //a temporary value of data->crossing_route_list
    vector<Route> crossing_route_list_temp;
    bool /*this is true if the mouse is moving, and false otherwise*/mouse_moving, /*this is true if a selection rectangle is being drawn, and false otherwise */selection_rectangle, /*this is true/false if highlighting of routes and sights is enabled/disables*/enable_highlight, /*idling = true means that the user is interacting with a temporary dialog window, thus all the handlers of wxFOCUS_EVENT do not make sense when idling = true and they will be disabled until idling is set back to false*/ idling, /*this is equal to true if file has been modified, false otherwise*/file_has_been_modified, /*this is equal to true if the file has no name, false otherwise*/file_is_untitled, /*this is true if I am computing the astronomical position, false otherwise*/selecting_route_for_position, /*this is equal to true (false) if the user has (has not) pressed cancel while charts were loading */abort, /*if this is true, I am transporting an object with a new Route, otherwise it is false*/ transporting_with_new_route, /*if this is true, I am transporting an object with an existing Route, otherwise it is false*/ transporting_with_selected_route, /*this is true if a Route or Position is being dragged, and false otherwise*/ dragging_object,    /*this is true if the highlighted Route / Position is being changed and false otherwise */  changing_highlighted_object;

    Answer /*if this is y/n, the coastlines are shown/not shown*/show_coastlines, /*if this is y/n, sample_sight.nav is loaded/not loaded at startup*/ load_sample_sight;
    //the file where the data is read and written
    FileRW data_file;
    unsigned int margin;
    int /*the # of the sight/route/position which is highlighted at the current (_now) or preceeding (_before) step of mouse movement, because the mouse is hovering over it in listcontrol_sights/routes/positions*/highlighted_sight_now, highlighted_route_now, highlighted_route_before,  highlighted_position_now, highlighted_position_before, /*# of the object to transport or disconnect */i_object_to_transport, i_object_to_disconnect, i_transporting_route;
    /*map[i] is the position in data->route_list of the i-th Route in route_list_for_transport*/
    vector<int> map;
    //coastline_polygons_Position/Cartesian/Mercator[i] is a vector which contains the the coastline datapoints (in Position/Cartesian/Mercator projection format) of polygon #i
    vector< vector<Position> > coastline_polygons_Position;
    vector< vector<Cartesian> > coastline_polygons_Cartesian;
    vector< vector<PositionProjection> > coastline_polygons_Mercator;
    //coastline_polygons_map[i][j] contans a list of polygon ids (i.e., an unsigned int denoting an entry in coastline_polygons_Position) that denote all polygons that fall within latitude / longitude i + floor_min_lat, j
    vector< vector< vector<unsigned long long int> > > coastline_polygons_map;
    //the total number of coastline points present in the coastline data file
    unsigned long long int n_all_coastline_points;
    //this vector containts the ids (as written in coastline_polygons_Position/Cartesian) of the polygons which have at least one point falling into *rectangle_observer (Mercator projection) or circle_observer (3D projection), and it is updated every time *rectangle_observer changes
    vector<unsigned long long int> coastline_polygons_area_observer;
    Position /*these are the geographic Positions where the right mouse button is clicked at the beginning, the current and preceeding Position during mouse movement, and the mouse Position and at the end of the drawing process for the selection rectangle*/ *geo_position_start, *geo_position_before, *geo_position_now, *geo_position_end;
    PositionRectangle /*same DrawPanel::*rectangle_observer, but for the initial configutaiton of ChartFrame*/ *rectangle_observer_0;
    String transported_object_type, /*the labels that will be drawn on position_start_label_selection_rectangle and position_end_label_selection_rectangle_now, respectively. end_label_selection_rectangle_now is the label of the end point of selection rectangle now, while end_label_selection_rectangle_before is the label of the end point of selection rectangle at the preceeding mouse position*/start_label_selection_rectangle, end_label_selection_rectangle_now, end_label_selection_rectangle_before;
    
    //a functor to set/unset idling mode in *this
    SetIdling<ListFrame>* set_idling;
    UnsetIdling<ListFrame>* unset_idling;
    ConfirmTransport<ListFrame>* confirm_transport;
    CloseFrame<ListFrame>* close;
    //a functor to let the user select a Route in listcontrol_routes
    SelectRoute* select_route;
    PrintMessage<ListFrame, UnsetIdling<ListFrame> >* print_warning_message, *print_error_message, * print_info_message;
    ShowQuestionFrame< ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame> , UnsetIdling<ListFrame> >* print_question_message;
    
    OnSelectRouteInListControlRoutesForTransport* on_select_route_in_listcontrol_routes_for_transport;
    OnNewRouteInListControlRoutesForTransport* on_new_route_in_listcontrol_routes_for_transport;
    AskRemoveRelatedSight* ask_remove_related_sight;
    AskRemoveRelatedRoute* ask_remove_related_route;
    
    ListFrame(const wxString&, const wxString&, const wxPoint&, const wxSize&, String);
    
    void set(void);
    void PreRenderAll(void);
    void MyRefreshAll(void);
    void RefreshAll(void);
    void TabulatePositionsAll(void);
    void TabulateRoutesAll(void);
    void Resize(void);
    
    void LoadCoastLineData(String);
    
    void OnAddSight(wxCommandEvent&);
    template<class E> void OnModifySight(E&);
    void OnTransportSight(wxCommandEvent&);
    void OnDisconnectSight(wxCommandEvent&);
    template<class E> void OnPressDeleteSight(E&);
    
    void OnAddPosition(wxCommandEvent& event);
    template<class E> void OnModifyPosition(E&);
    void OnTransportPosition(wxCommandEvent&);
    template<class E> void OnPressDeletePosition(E&);
    
    void OnAddRoute(wxCommandEvent&);
    void OnAddRouteForTransport(wxCommandEvent&);
    template<class E> void OnModifyRoute(E&);
    void OnTransportRoute(wxCommandEvent&);
    void OnDisconnectRoute(wxCommandEvent&);
    template<class E> void OnPressDeleteRoute(E&);
    bool CheckRoutesForTransport(void);
    
    template<class E> void Disconnect(E&);
    template<class E> void DisconnectAndPromptMessage(E&);
    
    void OnAddChartFrame(wxCommandEvent&);
    void OnCloseActiveChartFrame(wxCommandEvent&);
    void OnCloseAllChartFrames(wxCommandEvent&);
    void OnComputePosition(void);
    
    //    void OnMouseOnListControlSights(wxMouseEvent&);
    void OnMouseMovement(wxMouseEvent&);
    //    void OnMouseOnListControlPositions(wxMouseEvent&);
    
    void OnModifyFile(void);
    void OnSaveFile(void);
    template<class E> void OnPressCtrlO(E&);
    template<class E> void OnPressCtrlW(E&);
    template<class E> void OnPressCtrlS(E&);
    template<class E> void OnPressCtrlShiftS(E&);
    template<class E> void KeyDown(E&);
    template<class T> void ComputePosition(T&);
    template<class T, class F> void AnimateToObject(T*, F*);

    
};



#endif
