//
//  list_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "list_frame.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/wfstream.h>
#include <wx/progdlg.h>

#include "all_routes.h"
#include "animate_to_object.h"
#include "ask_remove_related_route.h"
#include "ask_remove_related_sight.h"
#include "bitmap.h"
#include "chart_transport_handler.h"
#include "close_frame.h"
#include "confirm_transport.h"
#include "delete_position.h"
#include "delete_route.h"
#include "delete_sight.h"
#include "existing_route.h"
#include "file_r.h"
#include "generic.h"
#include "new_route.h"
#include "on_change_selection_in_list_control.h"
#include "on_new_route_in_list_control_for_transport.h"
#include "on_select_route_in_list_control_for_transport.h"
#include "position_frame.h"
#include "question_frame.h"
#include "reset_list_frame.h"
#include "save_and_reset.h"
#include "select_route.h"
#include "show_question_frame.h"
#include "sight_frame.h"
#include "some_routes.h"
#include "unset_idling.h"


ListFrame::ListFrame(const wxString& title, [[maybe_unused]] const wxString& message, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(NULL, wxID_ANY, title, pos, size) {
    
    unsigned int i, red, green, blue;
    wxListItem column, item;
    String s;
    vector<wxString> headers;
    wxBoxSizer* sizer_listcontrol_routes_plus_buttons, * sizer_big_buttons;
    vector<wxButton*> disableable_buttons;
    //temporary angles used to read and store the default values of lambda_min ... phi_max from init file
    Angle lambda_min_temp, lambda_max_temp, phi_min_temp, phi_max_temp;
    //pos_open denotes the positions, in the string s composed of the color '(i,j,k)', of '(', pos_comma_1 of the first ',', pos_comma_2 of the second ',', and pos_close of ')'.
    size_t pos_end;
    
    geo_position_start = new Position;
    geo_position_now = new Position;
    geo_position_end = new Position;
    rectangle_observer_0 = new PositionRectangle;
    
    circle_observer_0 = new Route;
    transporting_route_saved = new Route;
    
#ifdef WIN32
    timer = new wxTimer();
    
    timer->Bind(wxEVT_TIMER, &ListFrame::OnTimer, this);
#endif
    
    
    //the file has not been modified yet -> I set
    file_has_been_modified = false;
    //for the time being, the file has no title
    file_is_untitled = true;
    enable_highlight = true;
    selecting_route_for_position = false;
    transporting = false;
    transporting_with_new_route = false;
    transporting_with_selected_route = false;
    changing_highlighted_object = false;
    abort = false;
    mouse_moving = false;
    //when a ListFrame is created, no Route nor Position is  being dragged
    dragging_object = false;
#ifdef WIN32
    refresh = true;
#endif
    i_object_to_disconnect = -1;
    
    
    set_idling = new SetIdling<ListFrame>(this);
    unset_idling = new UnsetIdling<ListFrame>(this);
    highlight_route = new HighlightObject<ListFrame>(this, &highlighted_route_before, &highlighted_route_now);
    highlight_position = new HighlightObject<ListFrame>(this, &highlighted_position_before, &highlighted_position_now);
    confirm_transport = new ConfirmTransport<ListFrame>(this);
    close = new CloseFrame<ListFrame>(this);
    (*unset_idling)();
    
    ask_remove_related_sight = new AskRemoveRelatedSight(this);
    ask_remove_related_route = new AskRemoveRelatedRoute(this);
    select_route = new SelectRoute(this);
    print_warning_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(this, unset_idling);
    print_error_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(this, unset_idling);
    print_info_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(this, unset_idling);
    //    print_info_message_disconnect_route = new PrintMessage<ListFrame, AnimateToObjectNew>(this, launch_animation);
    print_question_message = new ShowQuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(this, confirm_transport, unset_idling, unset_idling);
    //create extract_color with zero size, because I will need extract_color only to get colors
    
    //set icon paths to all print_*_message
    print_warning_message->image_path = wxGetApp().path_file_warning_icon;
    print_info_message->image_path = wxGetApp().path_file_info_icon;
    print_error_message->image_path = wxGetApp().path_file_error_icon;
    
    data = new Data(wxGetApp().catalog, String(""));
    
    
    //read show_coastlines from file_init
    show_coastlines.read_from_file_to(String("show coastlines"), (wxGetApp().path_file_init), String("R"), String(""));
    //read load_sample_sight from file_init
    load_sample_sight.read_from_file_to(String("load sample sight"), (wxGetApp().path_file_init), String("R"), String(""));
    
    //set circle_observer_0.omega
    circle_observer_0->omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), String(""));
    
    //set rectangle_obseerver
    //read lambda_min, ...., phi_max from file_init
    lambda_min_temp.read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
    lambda_max_temp.read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
    phi_min_temp.read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
    phi_max_temp.read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
    (*rectangle_observer_0) = PositionRectangle(Position(lambda_min_temp, phi_max_temp), Position(lambda_max_temp, phi_min_temp), String(""));
    
    LoadCoastLineData(String(""));
    
    if (!abort) {
        //the user has not pressed cancel while charts were loading -> I proceed
        
        //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
        (wxGetApp().rectangle_display) = ((wxGetApp().display).GetClientArea());
        
        (wxGetApp().file_init).set_name((wxGetApp().path_file_init));
        
        //read color list from file_init
        s.read_from_file_to(String("color list"), (wxGetApp().path_file_init), String("R"), String(""));
        
        //in file_init, each color is written as '(i,j,k) ', where i, j, k are the integers for the levels of red, green and blue. To cound the number of colors, I thus count the number of '(' in the string
        (wxGetApp().color_list).resize(count((s.value).begin(), (s.value).end(), '('));
        
        //when the ListFrame is created there is no open selection rectangle in any ChartFrame
        selection_rectangle = false;
        
        //SetColor(this);
        
        for (i = 0; i < (wxGetApp().color_list).size(); i++) {
            
            //get rid of everything that comes before and at '(' at the beginnign of s
            pos_end = (s.value).find("(");
            s.set(s.value.substr(pos_end + 1).c_str());
            //look for the first ','
            
            pos_end = (s.value).find(",");
            
            //read red
            red = stoi(((s.value).substr(0, pos_end)).c_str());
            
            //get rid of the first ','
            s.set(s.value.substr(pos_end + 1).c_str());
            
            pos_end = (s.value).find(",");
            
            green = stoi((s.value).substr(0, pos_end).c_str());
            
            //get rid of the second ','
            s.set(s.value.substr(pos_end + 1).c_str());
            
            pos_end = (s.value).find(")");
            //get rid of '('
            blue = stoi((s.value).substr(0, pos_end + 1).c_str());
            
            //write the color that I just read in color_list
            (wxGetApp().color_list)[i] = Color(red, green, blue);
            
        }
        
        //no positions nor routes are highlighted when ListFrame is constructed
        highlighted_route_now = -1;
        highlighted_position_now = -1;
        
        menu_bar = new wxMenuBar;
        menu_app = new wxMenu;
        menu_file = new wxMenu;
        menu_chart = new wxMenu;
        menu_new_chart = new wxMenu;
        menu_item_mercator = new wxMenu;
        menu_item_3d = new wxMenu;
        
        menu_new_chart->Append(wxID_HIGHEST + 1, "Mercator\tCtrl-m");
        menu_new_chart->Append(wxID_HIGHEST + 2, "3D\tCtrl-3");
        menu_chart->AppendSubMenu(menu_new_chart, wxT("New"), wxT(""));
        menu_chart->Append(wxID_HIGHEST + 3, "Close\tCtrl-c");
        menu_chart->Append(wxID_HIGHEST + 4, "Close all\tCtrl-a");
        menu_app->Append(wxID_HIGHEST + 5, "Quit\tCtrl-q");
        menu_file->Append(wxID_HIGHEST + 6, "Open\tCtrl-o");
        menu_file->Append(wxID_HIGHEST + 7, "Close\tCtrl-w");
        menu_file->Append(wxID_HIGHEST + 8, "Save\tCtrl-s");
        menu_file->Append(wxID_HIGHEST + 9, "Save as...\tCtrl-Shift-s");
        
        menu_bar->Append(menu_app, wxT("&App"));
        menu_bar->Append(menu_file, wxT("&File"));
        menu_bar->Append(menu_chart, wxT("&Chart"));
        SetMenuBar(menu_bar);
        
        menu_file->Enable(wxID_HIGHEST + 7, false);
        
        menu_new_chart->Bind(wxEVT_MENU, &ListFrame::OnAddChartFrame, this, wxID_HIGHEST + 1);
        menu_new_chart->Bind(wxEVT_MENU, &ListFrame::OnAddChartFrame, this, wxID_HIGHEST + 2);
        menu_chart->Bind(wxEVT_MENU, &ListFrame::OnCloseActiveChartFrame, this, wxID_HIGHEST + 3);
        menu_chart->Bind(wxEVT_MENU, &ListFrame::OnCloseAllChartFrames, this, wxID_HIGHEST + 4);
        menu_bar->Bind(wxEVT_MENU, &MyApp::OnPressCtrlQ<wxCommandEvent>, &(wxGetApp()), wxID_HIGHEST + 5);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlO<wxCommandEvent>, this, wxID_HIGHEST + 6);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlW<wxCommandEvent>, this, wxID_HIGHEST + 7);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlS<wxCommandEvent>, this, wxID_HIGHEST + 8);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlShiftS<wxCommandEvent>, this, wxID_HIGHEST + 9);
        
        
        on_select_route_in_listcontrol_routes_for_transport = new OnSelectRouteInListControlRoutesForTransport(this);
        on_new_route_in_listcontrol_routes_for_transport = new OnNewRouteInListControlRoutesForTransport(this);
        
        //initialize delete_sight, which defines the functor to delete the sight but not its related route (it is called when the user answers 'n' to QuestionFrame)
        delete_sight = new DeleteSight(this, Answer('n', String("")));
        //initialize delete_sight_and_related_route, which defines the functor to delete the sight and its related route (it is called when the user answers 'y' to QuestionFrame)
        delete_sight_and_related_route = new DeleteSight(this, Answer('y', String("")));
        
        //initialize delete_route, which defines the functor to delete the route but not its related sight (it is called when the user answers 'n' to QuestionFrame)
        delete_route = new DeleteRoute(this, Answer('n', String("")));
        //initialize delete_route_and_related_sight, which defines the functor to delete the route and its related sight (it is called when the user answers 'y' to QuestionFrame)
        delete_route_and_related_sight = new DeleteRoute(this, Answer('y', String("")));
        
        //initialize delete_position, which defines the functor to delete a Position
        delete_position = new DeletePosition(this);
        
        
        //initialized existing_route and create_route, which define the functors to modify / create a Route
        existing_route = new ExistingRoute(this);
        new_route = new NewRoute(this);
        
        
        panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
        
        
        sizer_all = new wxBoxSizer(wxVERTICAL);
        sizer_h = new wxBoxSizer(wxHORIZONTAL);
        sizer_v = new wxBoxSizer(wxVERTICAL);
        sizer_listcontrol_routes_plus_buttons = new wxBoxSizer(wxHORIZONTAL);
        sizer_big_buttons = new wxBoxSizer(wxVERTICAL);
        
        sizer_buttons_sight = new wxBoxSizer(wxHORIZONTAL);
        sizer_buttons_position = new wxBoxSizer(wxHORIZONTAL);
        sizer_buttons_route = new wxBoxSizer(wxHORIZONTAL);
        
        sizer_box_sight = new wxStaticBoxSizer(wxVERTICAL, panel, "Sights");
        sizer_box_position = new wxStaticBoxSizer(wxVERTICAL, panel, "Positions");
        sizer_box_route = new wxStaticBoxSizer(wxVERTICAL, panel, "Routes");
        
        
        //button to modify a sight
        button_modify_sight = new wxBitmapButton(
                                                 panel,
                                                 wxID_ANY,
                                                 Bitmap(wxGetApp().path_file_pencil_icon, wxGetApp().size_small_button),
                                                 wxDefaultPosition,
                                                 wxDefaultSize,
                                                 wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                 );
        button_modify_sight->Bind(wxEVT_BUTTON, &ListFrame::OnModifySight<wxCommandEvent>, this);
        button_modify_sight->Enable(false);
        button_modify_sight->SetToolTip(wxString("Modify the selected sight"));
        
        //button to transport a sight
        button_transport_sight = new wxBitmapButton(
                                                    panel,
                                                    wxID_ANY,
                                                    Bitmap(wxGetApp().path_file_transport_icon, wxGetApp().size_small_button),
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                    );
        button_transport_sight->Bind(wxEVT_BUTTON, &ListFrame::OnTransportSight, this);
        button_transport_sight->Enable(false);
        button_transport_sight->SetToolTip(wxString("Transport the selected sight"));
        
        
        //button to disconnect a sight
        button_disconnect_sight = new wxBitmapButton(
                                                     panel,
                                                     wxID_ANY,
                                                     Bitmap(wxGetApp().path_file_disconnect_icon, wxGetApp().size_small_button),
                                                     wxDefaultPosition,
                                                     wxDefaultSize,
                                                     wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                     );
        button_disconnect_sight->Bind(wxEVT_BUTTON, &ListFrame::OnDisconnectSight, this);
        button_disconnect_sight->Enable(false);
        button_disconnect_sight->SetToolTip(wxString("Disconnect the selected sight from its route"));
        
        
        
        //button to modify a position
        button_modify_position = new wxBitmapButton(
                                                    panel,
                                                    wxID_ANY,
                                                    Bitmap(wxGetApp().path_file_pencil_icon, wxGetApp().size_small_button),
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                    );
        button_modify_position->Bind(wxEVT_BUTTON, &ListFrame::OnModifyPosition<wxCommandEvent>, this);
        button_modify_position->Enable(false);
        button_modify_position->SetToolTip(wxString("Modify the selected position"));
        
        //button to transport a position
        button_transport_position = new wxBitmapButton(
                                                       panel,
                                                       wxID_ANY,
                                                       Bitmap(wxGetApp().path_file_transport_icon, wxGetApp().size_small_button),
                                                       wxDefaultPosition,
                                                       wxDefaultSize,
                                                       wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                       );
        button_transport_position->Bind(wxEVT_BUTTON, &ListFrame::OnTransportPosition, this);
        button_transport_position->Enable(false);
        button_transport_position->SetToolTip(wxString("Transport the selected position"));
        
        //button to transport a Route
        button_transport_route = new wxBitmapButton(
                                                    panel,
                                                    wxID_ANY,
                                                    Bitmap(wxGetApp().path_file_transport_icon, wxGetApp().size_small_button),
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                    );
        button_transport_route->Bind(wxEVT_BUTTON, &ListFrame::OnTransportRoute, this);
        button_transport_route->Enable(false);
        button_transport_route->SetToolTip(wxString("Transport the selected route"));
        
        //button to disconect a Route
        button_disconnect_route = new wxBitmapButton(
                                                     panel,
                                                     wxID_ANY,
                                                     Bitmap(wxGetApp().path_file_disconnect_icon, wxGetApp().size_small_button),
                                                     wxDefaultPosition,
                                                     wxDefaultSize,
                                                     wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                     );
        button_disconnect_route->Bind(wxEVT_BUTTON, &ListFrame::OnDisconnectRoute, this);
        button_disconnect_route->Enable(false);
        button_disconnect_route->SetToolTip(wxString("Disconnect the selected route from its sight"));
        
        
        //button to modify a route
        button_modify_route = new wxBitmapButton(
                                                 panel,
                                                 wxID_ANY,
                                                 Bitmap(wxGetApp().path_file_pencil_icon, wxGetApp().size_small_button),
                                                 wxDefaultPosition,
                                                 wxDefaultSize,
                                                 wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                 );
        button_modify_route->Bind(wxEVT_BUTTON, &ListFrame::OnModifyRoute<wxCommandEvent>, this);
        button_modify_route->Enable(false);
        button_modify_route->SetToolTip(wxString("Modify the selected route"));
        
        //button to delete a sight
        button_delete_sight = new wxBitmapButton(
                                                 panel,
                                                 wxID_ANY,
                                                 Bitmap(wxGetApp().path_file_trash_icon, wxGetApp().size_small_button),
                                                 wxDefaultPosition,
                                                 wxDefaultSize,
                                                 wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                 );
        button_delete_sight->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteSight<wxCommandEvent>, this);
        button_delete_sight->Enable(false);
        button_delete_sight->SetToolTip(wxString("Delete the selected sight"));
        
        //button to delete a position
        button_delete_position = new wxBitmapButton(
                                                    panel,
                                                    wxID_ANY,
                                                    Bitmap(wxGetApp().path_file_trash_icon, wxGetApp().size_small_button),
                                                    wxDefaultPosition,
                                                    wxDefaultSize,
                                                    wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                    );
        button_delete_position->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeletePosition<wxCommandEvent>, this);
        button_delete_position->Enable(false);
        button_delete_position->SetToolTip(wxString("Delete the selected position"));
        
        //button to delete a route
        button_delete_route = new wxBitmapButton(
                                                 panel,
                                                 wxID_ANY,
                                                 Bitmap(wxGetApp().path_file_trash_icon, wxGetApp().size_small_button),
                                                 wxDefaultPosition,
                                                 wxDefaultSize,
                                                 wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                 );
        button_delete_route->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteRoute<wxCommandEvent>, this);
        button_delete_route->Enable(false);
        button_delete_route->SetToolTip(wxString("Delete the selected route"));
        
        
        //listcontrol_sights with sights
        disableable_buttons.clear();
        disableable_buttons.push_back(button_modify_sight);
        disableable_buttons.push_back(button_transport_sight);
        //    disableable_buttons.push_back(button_disconnect_sight);
        disableable_buttons.push_back(button_delete_sight);
        
        
        
        
        listcontrol_sights = new ListControl<Sight>(panel, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
        on_change_selection_in_listcontrol_sights = new OnChangeSelectionInListControl<Sight>(listcontrol_sights, String("sight"));
        //SetColor(listcontrol_sights);
        //    listcontrol_sights->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_sights);
        listcontrol_sights->Bind(wxEVT_LIST_ITEM_SELECTED, *on_change_selection_in_listcontrol_sights);
        listcontrol_sights->Bind(wxEVT_LIST_ITEM_DESELECTED, *on_change_selection_in_listcontrol_sights);
        listcontrol_sights->SetToolTip(wxString("List of sights"));
        
        headers.clear();
        headers.push_back(wxString("Number"));
        headers.push_back(wxString("Body"));
        headers.push_back(wxString("Limb"));
        headers.push_back(wxString("Artificial horizon"));
        headers.push_back(wxString("Sextant altitude"));
        headers.push_back(wxString("Index error"));
        headers.push_back(wxString("Height of eye"));
        headers.push_back(wxString("Master-clock date and hour (UTC)"));
        headers.push_back(wxString("Stopwatch"));
        headers.push_back(wxString("Stopwatch reading"));
        headers.push_back(wxString("TAI - UTC"));
        headers.push_back(wxString("Label"));
        headers.push_back(wxString("Related route"));
        listcontrol_sights->SetColumns(headers);
        
        
        
        
        sizer_box_sight->Add(listcontrol_sights, 0, wxALL, (wxGetApp().border.value));
        
        
        //listcontrol_positions with positions
        disableable_buttons.clear();
        disableable_buttons.push_back(button_modify_position);
        disableable_buttons.push_back(button_transport_position);
        disableable_buttons.push_back(button_delete_position);
        
        
        listcontrol_positions = new ListControl<Position>(panel, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
        on_change_selection_in_listcontrol_positions = new OnChangeSelectionInListControl<Position>(listcontrol_positions, String("position"));
        listcontrol_positions->Bind(wxEVT_LIST_ITEM_SELECTED, *on_change_selection_in_listcontrol_positions);
        listcontrol_positions->Bind(wxEVT_LIST_ITEM_DESELECTED, *on_change_selection_in_listcontrol_positions);
        listcontrol_positions->SetToolTip(wxString("List of positions"));
        
        headers.clear();
        headers.push_back(wxString("Number"));
        headers.push_back(wxString("Latitude"));
        headers.push_back(wxString("Longitude"));
        headers.push_back(wxString("Label"));
        listcontrol_positions->SetColumns(headers);
        
        sizer_box_position->Add(listcontrol_positions, 1, wxALL, (wxGetApp().border.value));
        
        
        //listcontrol routes with routes
        disableable_buttons.clear();
        disableable_buttons.push_back(button_modify_route);
        disableable_buttons.push_back(button_transport_route);
        //    disableable_buttons.push_back(button_disconnect_route);
        disableable_buttons.push_back(button_delete_route);
        
        listcontrol_routes = new ListControl<Route>(panel, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
        on_change_selection_in_listcontrol_routes = new OnChangeSelectionInListControl<Route>(listcontrol_routes, String("route"));
        //SetColor(listcontrol_routes);
        //    listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_routes);
        listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *on_change_selection_in_listcontrol_routes);
        listcontrol_routes->Bind(wxEVT_LIST_ITEM_DESELECTED, *on_change_selection_in_listcontrol_routes);
        listcontrol_routes->SetToolTip(wxString("List of routes"));
        //I bind ListFrame::OnMouseMovement to listcontrol_sights, listcontrol_routes and to panel, because I want ListFrame::OnMouseMovement to be called when the mouse is either on listcontrol_sights, listcontrol_routes and on panel
        listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        
        
        //bind all listcontrols to mouse double-click event, so when the user double clicks on an item in the listcontrol, the SightFrame, PositionFrame or RouteFrame is opened to modify the sight, position or route
        listcontrol_sights->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifySight<wxListEvent>, this);
        listcontrol_positions->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyPosition<wxListEvent>, this);
        listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, this);
        
        
        headers.clear();
        headers.push_back(wxString("Number"));
        headers.push_back(wxString("Type"));
        headers.push_back(wxString("Start"));
        headers.push_back(wxString("Z"));
        headers.push_back(wxString("Length"));
        headers.push_back(wxString("Ground Position"));
        headers.push_back(wxString("Omega"));
        headers.push_back(wxString("Label"));
        headers.push_back(wxString("Related Sight"));
        listcontrol_routes->SetColumns(headers);
        
        
        sizer_box_route->Add(listcontrol_routes, 1, wxALL, (wxGetApp().border.value));
        
        //bing everything to KeyDown method, so when a key is pressed on *this, panel, listcontrol... then KeyDown is called
        Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        panel->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        listcontrol_sights->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        listcontrol_routes->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        listcontrol_positions->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        
        
        if (load_sample_sight == Answer('y', String(""))) {
            //I read a sample sight from file default_open_directory/sample_sight.nav, store into sight and set all the fields in this to the data in sight with set()
            
            //I am loading data from path_file_sample_sight -> set name of data_file accordingly
            data_file.set_name((wxGetApp().path_file_sample_sight));
            
            
            
#ifdef __APPLE__
            //I am on APPLE operating system -> read the file from hard drive locatd in the Data directory
            
            data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("RW"), String(""));
            
#endif
#ifdef _WIN32
            //I am on WIN32 operating system
            
            
            //Fork
            //Case 1: If I open a sample sight file at startup stored in Windows resources, use this
            data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("R"), String(""));
            //Case 2: If I open a file on disk, use this
            // data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("RW"), String(""));
            
            
#endif
            
            //test for Route between two points: I construct a loxodrome that connects (data->position_list)[0] to (data->position_list)[1]
            /*
             Route* r;
             r = new Route(RouteType(Route_types[0]), (data->position_list)[0], (data->position_list)[1]);
             r->compute_end(String(""));
             
             data->route_list.push_back(*r);
             
             */
            
            
            file_is_untitled = false;
            menu_file->Enable(wxID_HIGHEST + 7, true);
            set();
            SetLabel(data_file.name->value);
            PreRenderAndFitAll();
            
        }
        else {
            
            file_is_untitled = true;
            
            //there is no file-> I initialize recent_bodies ect in lexicographic order
            for (i = 0; i < (data->recent_bodies.size()); i++) {
                (data->recent_bodies)[i] = i;
            }
            for (i = 0; i < (data->recent_projections.size()); i++) {
                (data->recent_projections)[i] = i;
            }
            
        }
        
        
        set();
        
        //button to show map
        button_show_map = new wxBitmapButton(
                                             panel,
                                             wxID_ANY,
                                             Bitmap(wxGetApp().path_file_map_icon, wxGetApp().size_large_button - ToDIP(wxSize((wxGetApp().border.value), (wxGetApp().border.value)))),
                                             wxDefaultPosition,
                                             (wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value))),
                                             wxBU_EXACTFIT | wxSIMPLE_BORDER
                                             );
        button_show_map->Bind(wxEVT_BUTTON, &MyApp::ShowCharts<wxCommandEvent>, &wxGetApp());
        button_show_map->SetToolTip(wxString("Show the chart"));
        
        //button to compute astronomical position
        button_compute_position = new wxBitmapButton(
                                                     panel,
                                                     wxID_ANY,
                                                     Bitmap(wxGetApp().path_file_position_icon, wxGetApp().size_large_button - ToDIP(wxSize((wxGetApp().border.value), (wxGetApp().border.value)))),
                                                     wxDefaultPosition,
                                                     (wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value))),
                                                     wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                     );
        button_compute_position->Bind(wxEVT_BUTTON, &ListFrame::ComputePosition<wxCommandEvent>, this);
        button_compute_position->SetToolTip(wxString("Compute the astronomical position"));
        
        
        //button to add a sight
        button_add_sight = new wxBitmapButton(
                                              panel,
                                              wxID_ANY,
                                              Bitmap(wxGetApp().path_file_plus_icon, wxGetApp().size_small_button),
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxBU_EXACTFIT | wxSIMPLE_BORDER
                                              );
        button_add_sight->Bind(wxEVT_BUTTON, &ListFrame::OnAddSight, this);
        button_add_sight->SetToolTip(wxString("Add a sight"));
        
        
        //button to add a position
        button_add_position = new wxBitmapButton(
                                                 panel,
                                                 wxID_ANY,
                                                 Bitmap(wxGetApp().path_file_plus_icon, wxGetApp().size_small_button),
                                                 wxDefaultPosition,
                                                 wxDefaultSize,
                                                 wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                 );
        button_add_position->Bind(wxEVT_BUTTON, &ListFrame::OnAddPosition, this);
        button_add_position->SetToolTip(wxString("Add a position"));
        
        //button to add a route
        button_add_route = new wxBitmapButton(
                                              panel,
                                              wxID_ANY,
                                              Bitmap(wxGetApp().path_file_plus_icon, wxGetApp().size_small_button),
                                              wxDefaultPosition,
                                              wxDefaultSize,
                                              wxBU_EXACTFIT | wxSIMPLE_BORDER
                                              );
        button_add_route->Bind(wxEVT_BUTTON, &ListFrame::OnAddRoute, this);
        button_add_route->SetToolTip(wxString("Add a route"));
        
        
        extract_colors = new wxTextCtrl(panel, wxID_ANY, wxS(""), wxDefaultPosition, ToDIP(wxSize(0, 0)));
        
        
        sizer_buttons_sight->Add(button_add_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_modify_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_transport_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_disconnect_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_delete_sight, 0, wxALIGN_CENTER);
        sizer_box_sight->Add(sizer_buttons_sight, 0, wxALIGN_LEFT | wxALL, (wxGetApp().border.value));
        
        sizer_buttons_position->Add(button_add_position, 0, wxALIGN_CENTER);
        sizer_buttons_position->Add(button_modify_position, 0, wxALIGN_CENTER);
        sizer_buttons_position->Add(button_transport_position, 0, wxALIGN_CENTER);
        sizer_buttons_position->Add(button_delete_position, 0, wxALIGN_CENTER);
        sizer_box_position->Add(sizer_buttons_position, 0, wxALIGN_LEFT | wxALL, (wxGetApp().border.value));
        
        sizer_buttons_route->Add(button_add_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_modify_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_transport_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_disconnect_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_delete_route, 0, wxALIGN_CENTER);
        sizer_box_route->Add(sizer_buttons_route, 0, wxALIGN_LEFT | wxALL, (wxGetApp().border.value));
        
        
        //
        
        //resize uniformly all columns
        //    for(i=0; i<(listcontrol_sights->GetColumnCount()); ++i){
        //        listcontrol_sights->SetColumnWidth(i, ((listcontrol_sights->GetSize()).GetWidth())/(listcontrol_sights->GetColumnCount()));
        //    }
        
        sizer_v->Add(sizer_box_sight, 1, wxALL, (wxGetApp().border.value));
        sizer_v->Add(sizer_box_position, 1, wxALL, (wxGetApp().border.value));
        sizer_listcontrol_routes_plus_buttons->Add(sizer_box_route, 0, wxALL, (wxGetApp().border.value));
        //    sizer_listcontrol_routes_plus_buttons->AddStretchSpacer(1);
        //here I set the flag '0' to avoid button_show_map from being stretched
        sizer_big_buttons->Add(button_compute_position, 0, wxALL | wxALIGN_CENTER, (wxGetApp().border.value));
        sizer_big_buttons->Add(button_show_map, 0, wxALL | wxALIGN_CENTER, (wxGetApp().border.value));
        //    sizer_listcontrol_routes_plus_buttons->Add(sizer_big_buttons, 0);
        //by adding the flag wxEXPAND here, I let the StretchSpacer in sizer_listcontrol_routes_plus_buttons expand, and thus I flush to the right button_show_map
        sizer_v->Add(sizer_listcontrol_routes_plus_buttons, 1, wxALL, (wxGetApp().border.value));
        sizer_h->Add(sizer_v, 1, wxALIGN_BOTTOM, (wxGetApp().border.value));
        sizer_h->Add(sizer_big_buttons, 0, wxALIGN_CENTER, (wxGetApp().border.value));
        sizer_all->Add(sizer_h, 1, wxALL | wxEXPAND, (wxGetApp().border.value));
        
#ifdef _WIN32
        //if I am on WIN32, I set the icon from the icon set in the .rc file
        SetIcon(wxICON(app_icon));
#endif
        
        //panel->SetSizer(sizer);
        //    Maximize(panel);
        panel->SetSizerAndFit(sizer_all);
        panel->Fit();
        Fit();
        
        //given that I have incoroporated the listcontrols into the sizers, listrcontrols may have been resized -> I Fit() them so their content is properly shown
        Resize();
        Centre();
        
    }
    
}

//create a new ChartFrame and appends it to the end of chart_frames
void ListFrame::OnAddChartFrame(wxCommandEvent& event) {
    
    stringstream s;
    Projection projection;
    
    //recognizes whether the creation of a new chart frame has been triggered by pressing the ((Projection_types[0]).value) or the ((Projection_types[1]).value) button, and writes the respective proejction namee into projection.
    if (event.GetId() == wxID_HIGHEST + 1) {
        
        projection = Projection_types[0];
        
    }
    
    if (event.GetId() == wxID_HIGHEST + 2) {
        
        projection = Projection_types[1];
        
    }
    
    
    chart_frames.resize(chart_frames.size() + 1);
    
    s.str("");
    s << "Chart #" << (chart_frames.size()) << " - " << (projection.value) << " projection";
    
    (chart_frames.back()) = new ChartFrame(
                                           this,
                                           projection,
                                           s.str(),
                                           /*place each ChartFrame by shifting it with respect to the top-left corner of the screen*/
                                           wxPoint(0, 0),
                                           wxDefaultSize,
                                           String("")
                                           );
    //    (chart_frames.back())->Show(true);
    
    chart_frames.back()->Reset(event);
    wxGetApp().ShowCharts(event);
    //    wxGetApp().AnimateCharts();
    chart_frames.back()->Animate();
    
    
    
}

//closes the ChartFrame that  has focus
void ListFrame::OnCloseActiveChartFrame(wxCommandEvent& event) {
    
    unsigned int i;
    
    //find the ChartFrame in chart_frames taht is Active and closes it
    for (i = 0; (i < (chart_frames.size())) && (((chart_frames[i])->IsActive()) == false); i++) {}
    
    if (i < (chart_frames.size())) {
        (chart_frames[i])->OnPressCtrlW(event);
    }
    
}

//closes the ChartFrame that  has focus
void ListFrame::OnCloseAllChartFrames(wxCommandEvent& event) {
    
    //closes all ChartFrames in chart_frames
    for (; 0 < (chart_frames.size()); ) {
        (chart_frames[0])->OnPressCtrlW(event);
    }
    
    
}

//this is the GUI function called when the user wants to compute the position: it calls the non-GUI method data->compute_position and returns GUI error/warning messages according to the output of data->compute_position
void ListFrame::OnComputePosition(void) {
    
    int output_compute_position;
    
    output_compute_position = (data->compute_position(String("\t")));
    
    if (output_compute_position == -1) {
        //the position could not be computed
        
        print_error_message->SetAndCall(NULL, String("Error"), String("I could not compute the astronomical position! No routes yield valid crossings"), (wxGetApp().path_file_error_icon));
        
    }else {
        
        switch (output_compute_position) {
                
            case 0:{
                //the astronomical Position couldbe computed by using all crossings/Routes and [# used crossings] >= 2
                
                //this functor will be used to trigger the animation
                AnimateToObject<Route, UnsetIdling<ListFrame> > animate(this, &(data->route_list.back()), unset_idling);
                
                set();
                //bring all charts to the astronomical position with an animation and do nothing at the end of the animation
                animate.operator()();
                
                break;
                
            }
                
            case 1:{
                //the astronomical Position couldbe computed by using only some crossings/Routes
                
                //this functor will be used to trigger the animation
                AnimateToObject<Route, PrintMessage<ListFrame, UnsetIdling<ListFrame> >  > animate(this, &(data->route_list.back()), print_warning_message);
                
                //set all parameters to prepare the printing of an error message, which will be called by ChartTransportHandler in AnimateToObjectOld at the end of the animation. To do this, I enter print_error_message as an argument in the call to AnimateToObjectOld
                print_warning_message->control = NULL;
                print_warning_message->message.set(String("Not all routes could be used to compute the astronomical position! Rome routes yield invalid crossings."));
                print_warning_message->title.set(String("Warning"));
                print_warning_message->image_path.set(wxGetApp().path_file_warning_icon);
                
                set();
                //bring all charts to the astronomical Position with an animation
                animate.operator()();
                
                break;
                
            }
                
            case 2:{
                
                //this functor will be used to trigger the animation
                AnimateToObject<Position, PrintMessage<ListFrame, UnsetIdling<ListFrame> > > animate(this, &(data->position_list.back()), print_warning_message);
                
                //the astronomical Position could be computed but not its error -> a Position has been added to position_list, but no Route (repreenting its error circle) has been added to route_list
                
                //set all parameters to prepare the printing of an error message, which will be called by ChartTransportHandler in AnimateToObjectOld at the end of the animation. To do this, I enter print_error_message as an argument in the call to AnimateToObjectOld
                print_warning_message->control = NULL;
                print_warning_message->message.set(String("The error on the astronomical position could not be computed!"));
                print_warning_message->title.set(String("Warning"));
                print_warning_message->image_path.set(wxGetApp().path_file_warning_icon);
                
                set();
                //bring all charts to the astronomical Position with an animation
                animate.operator()();
                
                break;
                
            }
                
        }
        
    }
    
}


//calls PreRender and FitAll in all che ChartFrames which are children of *this
void ListFrame::PreRenderAndFitAll(void) {
    
    for (long i = 0; i < (chart_frames.size()); i++) {
        
        //I call FitAll() because the positions have changed, so I need to re-draw the chart
        (((chart_frames[i])->draw_panel)->*(((chart_frames[i])->draw_panel)->PreRender))();
        ((chart_frames[i])->draw_panel)->Refresh();
        ((chart_frames[i])->draw_panel)->FitAll();
        
    }
    
}


//call Refresh()es on all chart_frames
void ListFrame::RefreshAll(void) {
    
    for (long i = 0; i < (chart_frames.size()); i++) {
        ((chart_frames[i])->draw_panel)->Refresh();
    }
    
}

//call MyRefresh on all chart_frames
void ListFrame::MyRefreshAll(void) {
    
    for (long i = 0; i < (chart_frames.size()); i++) {
        (chart_frames[i])->draw_panel->MyRefresh();
    }
    
}

//tabulate Routes in all chart_frames
void ListFrame::TabulateRoutesAll(void) {
    
    for (long i = 0; i < (chart_frames.size()); i++) {
        (chart_frames[i])->draw_panel->TabulateRoutes();
    }
    
}

//tabulate Positions in all chart_frames
void ListFrame::TabulatePositionsAll(void) {
    
    for (long i = 0; i < (chart_frames.size()); i++) {
        (chart_frames[i])->draw_panel->TabulatePositions();
    }
    
}


//fit the size of all listcontrols inside *this to their respective content and resize the respective sizers and *this to fit the new size of the listcontrols
void ListFrame::Resize(void) {
    
    listcontrol_sights->Resize(data->sight_list);
    sizer_box_sight->Layout();
    
    listcontrol_positions->Resize(data->position_list);
    sizer_box_position->Layout();
    
    listcontrol_routes->Resize(data->route_list);
    sizer_box_route->Layout();
    
    sizer_v->Layout();
    sizer_h->Layout();
    sizer_all->Layout();
    
    //    Maximize(panel);
    panel->Fit();
    Fit();
    Layout();
    
}

//set all the GUI fileds in *this from the data in this->data and adapts the size of columns and panel accordingly
void ListFrame::set() {
    
    //write the sights contained into data->sight_list into listcontrol_sights
    listcontrol_sights->set(data->sight_list, false);
    
    //write the positions into data->position_list into listcontrol_sights
    listcontrol_positions->set(data->position_list, false);
    
    //write the routes into data->route_list into listcontrol_routes
    listcontrol_routes->set(data->route_list, false);
    
    Resize();
    //    Maximize(panel);
    
}


//set/unset the idling  membre of all DrawPanels equal to value
void ListFrame::SetIdlingAllDrawPanels(const bool& value){
    
    for(unsigned int i=0; i<chart_frames.size(); i++){
        (chart_frames[i])->draw_panel->SetIdling(value);
    }
    
}


void ListFrame::OnAddSight(wxCommandEvent& event) {
    
    SightFrame* sight_frame = new SightFrame(this, NULL, -1, "New sight", wxDefaultPosition, wxDefaultSize, String(""));
    sight_frame->Show(true);
    
    event.Skip(true);
    
}

void ListFrame::OnAddPosition(wxCommandEvent& event) {
    
    PositionFrame* position_frame = new PositionFrame(this, NULL, -1, "New position", wxDefaultPosition, wxDefaultSize, String(""));
    position_frame->Show(true);
    
    event.Skip(true);
    
}

//method to be called when a new Route is added to *this
void ListFrame::OnAddRoute(wxCommandEvent& event) {
    
    route_frame = new RouteFrame(this, NULL, false, -1, "New route", wxDefaultPosition, wxDefaultSize, String(""));
    route_frame->Show(true);
    
    event.Skip(true);
    
}

//method to be called when a new Route is added to *this to transport something
void ListFrame::OnAddRouteForTransport(wxCommandEvent& event) {
    
    route_frame = new RouteFrame(this, NULL, true, -1, "New route for transport", wxDefaultPosition, wxDefaultSize, String(""));
    route_frame->Show(true);
    
    event.Skip(true);
    
}

template<class E> void ListFrame::OnModifySight(E& event) {
    
    long item;
    item = listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    
    if (item != -1) {
        
        stringstream s;
        
        s.str("");
        s << "Sight #" << item + 1;
        
        SightFrame* sight_frame = new SightFrame(this, &((data->sight_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        //        (sight_frame->sight) = &((data->sight_list)[item]);
        sight_frame->Show(true);
        
        
    }
    
    
    event.Skip(true);
    
}

void ListFrame::OnTransportSight(wxCommandEvent& event) {
    
    //I am transporting a Route (related to a Sight)
    transported_object_type = String("sight");
    
    //here I call ShowQuestionFrame with third functor equal to unset_idling and bind_esc_to_button_b = false, because I want the esc key and button_b to do different things: by pressing esc, the operation is aborted, while by pressing button_b the operation keeps going by using a new Route as transporting Route
    ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>* print_question = new ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>(this, existing_route, new_route, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("You want to transport a sight. With what route do you want to transport? Press ESC to abort."), String("Existing route"), String("New route"), CheckRoutesForTransport(), true, false);
    
    OnModifyFile();
    
    event.Skip(true);
    
}

//this method is called when the useer wants to disconnect a Sight from its related Route
void ListFrame::OnDisconnectSight(wxCommandEvent& event) {
    
    //set i_object_to_disconnect to the currently selected Sight in listcontrol_sights and call Disconnect to disconnect that Sight from its related Route
    i_object_to_disconnect = ((int)(listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));
    
    Disconnect(event);
    //print an info message
    print_info_message->SetAndCall(NULL, String("Warning"), String("The sight which you want do disconnect is connected to a route: disconnecting the sight from the route."), wxGetApp().path_file_info_icon);
    
}

//this method is called when the useer wants to disconnect a Route from its related Sight
void ListFrame::OnDisconnectRoute(wxCommandEvent& event) {
    
    //set i_object_to_disconnect to the currently selected Route in listcontrol_routes and call Disconnect to disconnect that Route from its related Sight
    i_object_to_disconnect = (((data->route_list)[(listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED))]).related_sight.value);
    
    Disconnect(event);
    //print an info message
    print_info_message->SetAndCall(NULL, String("Warning"), String("The route which you want do disconnect is connected to a sight: disconnecting the route from the sight."), wxGetApp().path_file_info_icon);
    
}

void ListFrame::OnTransportPosition(wxCommandEvent& event) {
    
    // I am transporting a Position
    transported_object_type = String("position");
    
    //here I call ShowQuestionFrame with third functor equal to unset_idling and bind_esc_to_button_b = false, because I want the esc key and button_b to do different things: by pressing esc, the operation is aborted, while by pressing button_b the operation keeps going by using a new Route as transporting Route
    
    //ask the user whether he/she wants to transport the sight with a an existing Route or with a new Route.
    ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>* print_question = new ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>(this, existing_route, new_route, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("You want to transport a position. With what route do you want to transport? Press ESC to abort."), String("Existing route"), String("New route"), CheckRoutesForTransport(), true, false);
    
    OnModifyFile();
    
    event.Skip(true);
    
}



template<class E> void ListFrame::OnModifyPosition(E& event) {
    
    long item;
    item = listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    
    if (item != -1) {
        
        stringstream s;
        
        s.str("");
        s << "Position #" << item + 1;
        
        PositionFrame* position_frame = new PositionFrame(this, &((data->position_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        position_frame->Show(true);
        
    }
    
    event.Skip(true);
    
}

template<class E> void ListFrame::OnModifyRoute(E& event) {
    
    
    long item;
    item = listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    
    if (item != -1) {
        
        stringstream s;
        
        s.str("");
        s << "Route #" << item + 1;
        
        route_frame = new RouteFrame(this, &((data->route_list)[item]), false, item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        route_frame->Show(true);
        
    }
    
    
    event.Skip(true);
    
}

void ListFrame::OnTransportRoute(wxCommandEvent& event) {
    
    //I am transporting a Route
    transported_object_type = String("route");
    
    //I store the # of the selected Route into i_object_to_transport
    i_object_to_transport = ((int)(listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));
    
    //here set i_object_to_transport to the currently selected Route
    
    //here I call ShowQuestionFrame with third functor equal to unset_idling and bind_esc_to_button_b = false, because I want the esc key and button_b to do different things: by pressing esc, the operation is aborted, while by pressing button_b the operation keeps going by using a new Route as transporting Route
    //ask the user whether he/she wants to transport the sight with a an existing Route or with a new Route.
    ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>* print_question = new ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>(this, existing_route, new_route, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("You want to transport a route. With what route do you want to transport? Press ESC to abort."), String("Existing route"), String("New route"), CheckRoutesForTransport(), true, false);
    
    OnModifyFile();
    
    event.Skip(true);
    
}



template<class E> void ListFrame::OnPressDeleteSight(E& event) {
    
    //ask the user whether he/she really wants to remove the Sight: if the answer is yes, then QuestionFrame calls the functor ask_remove_related_route. If no, I call the functor unsed_idling, which does nothing and simply sets idling to false
    ShowQuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>* print_question;
    
    
    print_question = new ShowQuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(this, ask_remove_related_route, unset_idling, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("Do you really want to remove this sight?"), String("Yes"), String("No"));
    
    
    event.Skip(true);
    
}

template<class E> void ListFrame::OnPressDeletePosition(E& event) {
    
    (delete_position->i_position_to_remove) = ((int)(listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));
    
    //ask the user whether he/she really wants to remove the Position: if the answer is yes, then QuestionFrame calls the functor delete_position. If no, I call the functor unsed_idling, which does nothing and simply sets idling to false
    
    ShowQuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>* print_question;
    
    print_question = new ShowQuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(this, delete_position, unset_idling, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("Do you really want to remove this position?"), String("Yes"), String("No"));
    
    
    event.Skip(true);
    
}

//only some Routes are viable to be transporting Routes. These are the Routes that: 1. are not related to any sight, 2. that are not circles of equal altitude 3. do not coincide with the object to transport -> I count how many Routes are available for transport -> If there is at least one, return true, otherwise return false
bool ListFrame::CheckRoutesForTransport(void) {
    
    unsigned int n_routes_for_transport;
    int i;
    
    for(i = 0, n_routes_for_transport=0; i < data->route_list.size(); i++){
        
        if(
           /*condition that the Route is not relatied to a Sight*/
           ((((data->route_list)[i]).related_sight.value) == -1) &&
           /*condition that the Route is not a circle of equal altitude*/
           (((data->route_list)[i]).type != Route_types[2]) &&
           /*condition that the Route does not coincide with the object to transport*/
           ((transported_object_type != String("route")) || (i_object_to_transport != i))
           ){
               
               n_routes_for_transport++;
               
           }
        
    }
    
    return((n_routes_for_transport > 0 ? true : false));
    
}



template<class E> void ListFrame::OnPressDeleteRoute(E& event) {
    
    //ask the user whether he/she really wants to remove the Route: if the answer is yes, then QuestionFrame calls the functor ask_remove_related_sight. If no, I call the functor unsed_idling, which does nothing and simply sets idling to false
    QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>* question_frame = new QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(NULL,
                                                                                                                                                                                                    ask_remove_related_sight, String("Yes"), unset_idling, String("No"), unset_idling, true, true, true,
                                                                                                                                                                                                    "",
                                                                                                                                                                                                    "Do you really want to remove this route?",
                                                                                                                                                                                                    wxGetApp().path_file_question_icon,
                                                                                                                                                                                                    wxDefaultPosition,
                                                                                                                                                                                                    wxDefaultSize,
                                                                                                                                                                                                    String(""));
    
    question_frame->Show(true);
    
    event.Skip(true);
    
}

//disconnects sight i_sight from its related Route
template<class E> void ListFrame::Disconnect(E& event) {
    
    int i_route;
    
    i_route = (((data->sight_list)[i_object_to_disconnect]).related_route).value;
    
    //disconnect route and sight
    (((data->sight_list)[i_object_to_disconnect]).related_route).set(-1);
    (((data->route_list)[i_route]).related_sight).set(-1);
    
    //update the related wxListCtrls in ListFrame
    ((data->sight_list)[i_object_to_disconnect]).update_ListControl(i_object_to_disconnect, listcontrol_sights);
    ((data->route_list)[i_route]).update_ListControl(i_route, listcontrol_routes);
    
    //set the background color of the related sight to white
    (listcontrol_sights)->SetItemBackgroundColour(i_object_to_disconnect, wxGetApp().background_color);
    
    //if an item is selected in listcontrol_sights, enable /disable button_transport_sight and button_disconnect_sight if the selected sight is related / unrelated to a Route
    if ((listcontrol_sights->GetSelectedItemCount()) != 0) {
        
        bool enable;
        
        enable = ((((data->sight_list)[listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value != -1);
        
        button_transport_sight->Enable(enable);
        button_disconnect_sight->Enable(enable);
        
    }
    
    //the Route has been disconnected from the sight -> a new Route which is not connected to any Sight is created -> the data of the file has been modified
    OnModifyFile();
    
    event.Skip(true);
    
}

template void ListFrame::Disconnect<wxTimerEvent>(wxTimerEvent&);


//disconnects sight i_sight from its related Route and prompt a message frame to inform the user
template<class E> void ListFrame::DisconnectAndPromptMessage(E& event) {
    
    Disconnect<E>(event);
    
    //print an info message
    print_info_message->SetAndCall(NULL, String("Warning"), String("The route which you want do disconnect is connected to a sight: disconnecting the route from the sight."), (wxGetApp().path_file_info_icon));
    
    
    event.Skip(true);
    
}

template  void ListFrame::DisconnectAndPromptMessage<wxMouseEvent>(wxMouseEvent&);


//when the mouse hovers over a given element of listcontrol_routes, sets highlighted_route_now equal to the id of that route, and the same for the relaetd sight in listcontrol_sights.
void ListFrame::OnMouseMovement(wxMouseEvent& event) {
    
    if(!idling){
        
        int i, j;
        
        //                cout << "Position of mouse screen = {" << wxGetMousePosition().x << " , " << wxGetMousePosition().y << "}\n";
        
        //save the id of the  Sight Route and Position highlighted at the preceeding step into highlighted_route_before
        highlighted_route_before = highlighted_route_now;
        highlighted_position_before = highlighted_position_now;
        
        
        //check whether the mouse is hovering over an element of listcontrol_routes / listcontrol_sights
        MousePositionOnListControl(listcontrol_sights, &highlighted_sight_now);
        MousePositionOnListControl(listcontrol_positions, &highlighted_position_now);
        MousePositionOnListControl(listcontrol_routes, &highlighted_route_now);
        
        if ((highlighted_sight_now == wxNOT_FOUND) && (highlighted_position_now == wxNOT_FOUND) && (highlighted_route_now == wxNOT_FOUND)) {
            //the mouse is not hovering over an element in listcontrol_sights nor listcontrol_routes: set a white background in all elements in listonctrol_routes and listcontrol_sights
            
            //set the beckgorund color of the Routes in listcontrol_sights and listcontrol_routes  and the background color of the Positions in listcontrol_positions to white
            for (i = 0; i < (listcontrol_sights->GetItemCount()); i++) {
                listcontrol_sights->SetItemBackgroundColour(i, wxGetApp().background_color);
            }
            for (i = 0; i < (listcontrol_positions->GetItemCount()); i++) {
                listcontrol_positions->SetItemBackgroundColour(i, wxGetApp().background_color);
            }
            for (i = 0; i < (listcontrol_routes->GetItemCount()); i++) {
                listcontrol_routes->SetItemBackgroundColour(i, wxGetApp().background_color);
            }
            
        }
        else {
            //the mouse is hovering over either an element of listcontrol_sights, or an element of listcontrol_routes, or an element of listcontrol_positions
            
            if ((highlighted_sight_now != wxNOT_FOUND) && enable_highlight) {
                // the mouse is hovering over an element of listcontrool_sights -> highlight it and the related route in listcontrol_routes, and set  a white background in all other leements in listcontrol_sights and listcontorl_routes
                
                highlighted_route_now = (((data->sight_list)[highlighted_sight_now]).related_route.value);
                
                for (i = 0; i < (listcontrol_sights->GetItemCount()); i++) {
                    
                    if (i == highlighted_sight_now) {
                        
                        //set the beckgorund color of the sight in listcontrol_sights and of its related route to a highlight color
                        listcontrol_sights->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                        if ((highlighted_route_now != -1) && ((listcontrol_routes->GetItemCount()) > highlighted_route_now)) {
                            listcontrol_routes->SetItemBackgroundColour(highlighted_route_now, (wxGetApp().color_selected_item));
                        }
                        
                    }
                    else {
                        
                        //set the beckgorund color of the sight in listcontrol_sights and of its related route to white
                        listcontrol_sights->SetItemBackgroundColour(i, wxGetApp().background_color);
                        if ((((((data->sight_list)[i]).related_route).value) != -1) && ((listcontrol_routes->GetItemCount()) > ((((data->sight_list)[i]).related_route).value))) {
                            listcontrol_routes->SetItemBackgroundColour(((((data->sight_list)[i]).related_route).value), wxGetApp().background_color);
                        }
                        
                    }
                    
                }
                
            }
            
            
            if (highlighted_position_now != wxNOT_FOUND) {
                //the mouse is hovering over an element of listcontrool_positions -> highlight it and the related position in listcontrol_positions, and set  a white background in all other leements in listcontrol_positions
                
                for (i = 0; i < (listcontrol_positions->GetItemCount()); i++) {
                    
                    if (i == highlighted_position_now) {
                        
                        //set the beckgorund color of the Position in listcontrol_positions to a highlight color
                        listcontrol_positions->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                        
                    }
                    else {
                        
                        //set the beckgorund color of the Route in listcontrol_routes and of its related sight to white
                        listcontrol_positions->SetItemBackgroundColour(i, wxGetApp().background_color);
                        
                    }
                    
                }
                
            }
            
            if ((highlighted_route_now != wxNOT_FOUND) && enable_highlight) {
                //the mouse is hovering over an element of listcontrool_routes -> highlight it and the related sight in listcontrol_sights, and set  a white background in all other leements in listcontrol_routes and listcontorl_sights
                
                j = ((((data->route_list)[highlighted_route_now]).related_sight).value);
                
                for (i = 0; i < (listcontrol_routes->GetItemCount()); i++) {
                    
                    if (i == highlighted_route_now) {
                        
                        //set the beckgorund color of the Route in listcontrol_routes and of its related sight to a highlight color
                        listcontrol_routes->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                        if ((j != -1) && ((listcontrol_sights->GetItemCount()) > j)) {
                            listcontrol_sights->SetItemBackgroundColour(j, (wxGetApp().color_selected_item));
                        }
                        
                    }
                    else {
                        
                        //set the beckgorund color of the Route in listcontrol_routes and of its related sight to white
                        listcontrol_routes->SetItemBackgroundColour(i, wxGetApp().background_color);
                        if ((((((data->route_list)[i]).related_sight).value) != -1) && ((listcontrol_sights->GetItemCount()) > ((((data->route_list)[i]).related_sight).value))) {
                            listcontrol_sights->SetItemBackgroundColour(((((data->route_list)[i]).related_sight).value), wxGetApp().background_color);
                        }
                        
                    }
                    
                }
                
            }
            
        }
        
        if ((highlighted_route_before != highlighted_route_now) || (highlighted_position_before != highlighted_position_now)) {
            //the highlighted Sight, or Route or Position has changed -> re-render the charts
            
            changing_highlighted_object = true;
            
            MyRefreshAll();
            
            changing_highlighted_object = false;
            
        }
        
    }
    
    event.Skip(true);
    
}

//this function is called every time the file is changed to mark that it is a modified file
void ListFrame::OnModifyFile(void) {
    
    if (!file_has_been_modified) {
        //file is being modified for the first time -> add a mark to the label of *this
        SetLabel((String(GetLabel().ToStdString()).append(String(" [modified]"))).value);
    }
    
    //file has been modified
    file_has_been_modified = true;
    
}



//this function is called every time the file is saved to mark that it is no longer a modified file
void ListFrame::OnSaveFile(void) {
    
    //set back the label of *this to the filename
    SetLabel(wxString(data_file.name->value));
    
    file_is_untitled = false;
    file_has_been_modified = false;
    
}



template<class E> void ListFrame::OnPressCtrlO(E& event) {
    
    wxFileDialog openFileDialog(this, _("Open"), (wxGetApp().default_open_directory).value, "", "nav files (*.nav)|*.nav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if ((openFileDialog.ShowModal()) != wxID_CANCEL) {
        // proceed loading the file chosen by the user;
        
        wxFileInputStream input_stream(openFileDialog.GetPath());
        
        if (!input_stream.IsOk()) {
            //file could not be opened
            
            wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
            
        }
        else {
            //file could be opened
            
            data_file.set_name(String((openFileDialog.GetPath()).ToStdString()));
            data->read_from_file_to(String("Data"), String((openFileDialog.GetPath()).ToStdString()), String("RW"), String(""));
            
            file_is_untitled = false;
            
            //emable the menu item to close data_file
            menu_file->Enable(wxID_HIGHEST + 7, true);
            //load the data in data into the GUI fields of *this
            set();
            //change the title of *this to the filename
            SetLabel(wxString(data_file.name->value));
            //resize and draw all charts according to the newly loaded data
            Resize();
            PreRenderAndFitAll();
            
        }
        
        
    }
    
    //the file has not been touched yet, thus
    file_has_been_modified = false;
    
    event.Skip(true);
    
}

template<class E> void ListFrame::OnPressCtrlW([[maybe_unused]] E& event) {
    
    ResetListFrame* reset_list_frame;
    
    reset_list_frame = new ResetListFrame(this);
    
    if (file_has_been_modified) {
        //the user wants to close a file that has been modified -> ask the user whethere he/she wants to save it before closing it
        
        SaveAndReset<ListFrame>* save_and_reset;
        
        ShowQuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>* print_question;
        
        save_and_reset = new SaveAndReset<ListFrame>(this);
        print_question = new ShowQuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>(this, save_and_reset, reset_list_frame, reset_list_frame);
        
        print_question->SetAndCall(NULL, String("Warning"), String("You pressed Ctrl+W. You are about to close a file that has been modified. Do you want to save changes?"), String("Yes"), String("No"));
        
    }
    else {
        //the user wants to close a file that has not been modified -> close it
        
        wxCommandEvent dummy;
        
        (*reset_list_frame)(dummy);
        
    }
    
    
}

template void ListFrame::OnPressCtrlW<wxKeyEvent>(wxKeyEvent&);



//write content of data into file
template<class E> void ListFrame::OnPressCtrlS(E& event) {
    
    data_file.open(String("out"), String(""));
    data->print(false, String(""), *(data_file.value));
    data_file.close(String(""));
    
    OnSaveFile();
    
    event.Skip(true);
    
}

template void ListFrame::OnPressCtrlS<wxKeyEvent>(wxKeyEvent&);


//write content of data into a named file
template<class E> void ListFrame::OnPressCtrlShiftS(E& event) {
    
    wxFileDialog openFileDialog(this, _(""), (wxGetApp().default_open_directory).value, "", "nav files (*.nav)|*.nav", wxFD_SAVE);
    
    if ((openFileDialog.ShowModal()) != wxID_CANCEL) {
        // the user did not presse cancel -> proceed saving on the file chosen by the user;
        
        data_file.set_name(String((openFileDialog.GetPath()).ToStdString()));
        //open a new file to save content on it
        data_file.open(String("out"), String(""));
        //writ edata into data_file
        data->print(false, String(""), *(data_file.value));
        
        //        //Here I write only the recent projections of the zero-th ChartFrame
        //        if(chart_frames.size() != 0){
        //            String((chart_frames[0])->projection->name->GetValue().ToStdString()).print(String("Recent projections"), false, String("\t"), *(data_file.value));
        //        }
        //
        //close the file
        data_file.close(String(""));
        
        OnSaveFile();
        
    }
    
    
    event.Skip(true);
    
}

template void ListFrame::OnPressCtrlShiftS<wxKeyEvent>(wxKeyEvent&);



template<class E> void ListFrame::KeyDown(E& event) {
    
    if (((event.GetKeyCode()) == WXK_DELETE) || ((event.GetKeyCode()) == WXK_BACK)) {
        // the user pressed delete or backspace
        
        
        if ((listcontrol_sights->HasFocus()) && ((listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)) {
            //an item was selected in listcontrol_sights while listcontrol_sights has focus -> call OnPressDeleteSight
            
            OnPressDeleteSight(event);
            
        }
        
        if ((listcontrol_positions->HasFocus()) && ((listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)) {
            //an item was selected in listcontrol_positions while listcontrol_positions has focus -> call OnPressDeleteposition
            
            OnPressDeletePosition(event);
            
        }
        
        if ((listcontrol_routes->HasFocus()) && ((listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)) {
            //an item was selected in listcontrol_routes  while listcontrol_routes has focus-> call OnPressDeleteroute
            
            OnPressDeleteRoute(event);
            
        }
        
        
    }
    
    if ((((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER))) {
        //the user pressed enter
        
        if (selecting_route_for_position) {
            
            long previous_item;
            
            previous_item = -1;
            (data->crossing_route_list).clear();
            do {
                
                previous_item = (listcontrol_routes->GetNextItem(previous_item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
                if(previous_item != -1){
                    my_push_back(&(data->crossing_route_list), crossing_route_list_temp[previous_item]);
                }
                
            } while (previous_item != -1);
            
            OnComputePosition();
            
            
        }
        
        //        if(transporting_with_selected_route && ((listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)){
        //            //the user is transporting an object with an existing Route and he/she selected that Route in listcontrol_routes -> proceed with the transport
        //
        //           (*on_select_route_in_listcontrol_routes_for_transport)(event);
        //
        //        }
        
    }
    
    event.Skip(true);
    
}





//this function fetches the data in ((wxGetApp().path_file_coastline_data_blocked).value) and stores them in data_x, data_y, all_coastline_points so that they can be read fastly
void ListFrame::LoadCoastLineData(String prefix) {
    
    FileR coastline_file;
    Position p_Position;
    Cartesian p_Cartesian;
    PositionProjection p_Mercator;
    string line, temp;
    stringstream ins, message_dialog;
    unsigned long long int i, j;
    
    //    string::size_type sz;
    //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
    //    vector<unsigned int> n_line;
    //    unsigned int l/*, n = 0*/;
    //    char* buffer = NULL;
    size_t pos_beg, pos_end;
    double lambda_temp, phi_temp, percentage_dialog;
    
    if (show_coastlines == Answer('y', String(""))) {
        //in file_init, show coastlines = y
        
        coastline_file.set_name((wxGetApp().path_coastline_file));
        coastline_file.count_lines(prefix);
        
        if ((!abort)) {
            
            message_dialog.str("");
            message_dialog << "\nLoading charts ... ";
            
            //a progress dialog to show progress during time-consuming operations
            wxProgressDialog progress_dialog(wxT("Welcome to Thelemacus!"), wxString(message_dialog.str().c_str()), max_dialog, NULL, wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME | wxPD_APP_MODAL);
            
            coastline_file.open(String(""));
            cout << prefix.value << "Reading file ...\n";
            
            coastline_polygons_Position.clear();
            coastline_polygons_Cartesian.clear();
            coastline_polygons_Mercator.clear();
            
            coastline_polygons_map.resize(ceil_max_lat - floor_min_lat);
            for(i=0; i<coastline_polygons_map.size(); i++){coastline_polygons_map[i].resize(360);}
            
#ifdef _WIN32
            //if I am on WIN32, I set the icon from the icon set in the .rc file
            progress_dialog.SetIcon(wxICON(app_icon));
#endif
            
            i = 0;
            n_all_coastline_points = 0;
            abort = false;
            
            getline(*(coastline_file.value), line);
            while ((!(coastline_file.value->eof())) && (!abort)) {
                //run through polygons
                
                coastline_polygons_Position.resize(i + 1);
                coastline_polygons_Cartesian.resize(i + 1);
                coastline_polygons_Mercator.resize(i + 1);
                
                pos_beg = line.find(":  ", 0)+3;
                do{
                    //run through points of a polygon
                    
                    //read longitude
                    pos_end = line.find(" ", pos_beg);
                    temp = line.substr(pos_beg, pos_end - pos_beg);
                    lambda_temp = stod(temp);
                    
                    //read latitude
                    pos_beg = pos_end+1;
                    pos_end = line.find("  ", pos_beg);
                    temp = line.substr(pos_beg, pos_end - pos_beg);
                    phi_temp = stod(temp);
                    
                    
                    p_Position.lambda.set(lambda_temp);
                    p_Position.phi.set(phi_temp);
                    p_Cartesian = p_Position.getCartesian();
                    p_Mercator.NormalizeAndSetMercator(p_Position);
                    
                    
                    
                    //push back the position into coastline_polygons_Position
                    my_push_back(&(coastline_polygons_Position.back()), p_Position);
                    //                  BE CAREFUL ABOUT THIS  BUG THAT MAY ARISE AGAIN IN THE FUTURE
                    //push back the position into coastline_polygons_Cartesian: this is the correct way to push back an element into coastline_polygons_Cartesian: if you use coastline_polygons_Cartesian[i][j].push_back(r_temp), the *memory address of coastline_polygons_Cartesian[i][j].back().r will be set equal to the memory adress of r_temp -> by iterating through the loop, all the entries of coastline_polygons_Cartesian[i][j].r will point to the same adress and thus contain the same value!!
                    //                  BE CAREFUL ABOUT THIS  BUG THAT MAY ARISE AGAIN IN THE FUTURE
                    (coastline_polygons_Cartesian[i]).resize((coastline_polygons_Cartesian[i]).size() + 1);
                    //here I allocate a completely new space for  coastline_polygons_Cartesian[i].back(), so all memory adresses in coastline_polygons_Cartesian[i][0], coastline_polygons_Cartesian[i][1], ... wil be differernt and all the points in there will be different
                    coastline_polygons_Cartesian[i].back() = Cartesian();
                    coastline_polygons_Cartesian[i].back() = p_Cartesian;
                    
                    my_push_back(&(coastline_polygons_Mercator.back()), p_Mercator);
                    
                    
                    if ((floor(rad_to_deg * (p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat) >=0 ) && (floor(rad_to_deg * (p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat) < coastline_polygons_map.size())) {
                        // polygon #i contains the point p_Position that falls within lat/long K*(p_Position.phi.normalize_pm_pi_ret().value) , floor(K*(p_Position.lambda.value)) -> I add i to coastline_polygons_map[floor(K*(p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat)][floor(K*(p_Position.lambda.value))]
                        coastline_polygons_map[floor(rad_to_deg * (p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat)][floor(rad_to_deg * (p_Position.lambda.value))].push_back(i);
                        n_all_coastline_points++;
                        
                    }
                    
                    
                    
                    pos_beg = pos_end+2;
                    
                    
                }while(pos_end !=  string::npos);
                
                
                percentage_dialog = 100.0 * ((double)i) / ((double)(coastline_file.number_of_lines));
                message_dialog.str("");
                message_dialog << "\nLoading charts ... " << ((int)percentage_dialog) << "%";
                abort = (!(progress_dialog.Update(percentage_dialog, wxString(message_dialog.str().c_str()))));
                
                getline(*(coastline_file.value), line);
                i++;
                
            }
            
            if ((!abort)) {
                
                progress_dialog.Update(max_dialog);
                cout << prefix.value << "... done.\n";
                
            }
            
            coastline_polygons_area_observer.reserve(n_all_coastline_points);
            
            coastline_file.close(String(""));
            
            //            unsigned long long int l;
            //given the way in which I added polygons to coastline_polygons_map, there may be duplicates in oastline_polygons_map[i][j] -> I delete them
            for(i=0/*, l=0*/; i<coastline_polygons_map.size(); i++){
                for(j=0; j<coastline_polygons_map[i].size(); j++){
                    
                    delete_duplicates(&(coastline_polygons_map[i][j]));
                    
                }
                
            }
            
        }
        
    }
    
}




template<class T, class F> void ListFrame::AnimateToObjectOld(T* object_in, F* f){
    
    unsigned int i;
    //the Position where the chart will be centered by the animation triggered when the user presses ok
    Position target_position;
    //the aperture angle of circle_observer at the end of the animation
    Angle target_omega;
    //the transport handlers used to transport the chart: there is one ChartTransportHandler per ChartFrame
    vector< ChartTransportHandler<F>* > chart_transport_handlers;
    Length d;
    
    chart_transport_handlers.resize(chart_frames.size());
    
    //bring all charts to front to show the animation
    wxGetApp().ShowCharts();
    
    for(i=0; i<chart_frames.size(); i++){
        
        switch (position_in_vector(((chart_frames[i])->projection), Projection_types)) {
                
            case 0: {
                //I am using Projection_types[0]
                
                //the center of the Mercator projection
                Position center;
                //the size of the object to which the animation is directed, in units of the coordinates x, y of the Mercator projection
                PositionProjection target_size, size_rectangle_observer_0;
                Double zoom_factor;
                
                if(std::is_same<T, Route>::value){
                    //object is a Route
                    
                    //I introduce the Route* object and set object_in = object by casting object_in into a Route pointer. This is necessary to make this method work with multiple types T (T=Position, T=Route, ...)
                    Route* object;
                    
                    object = (Route*)object_in;
                    
                    if(object->type == Route_types[2]){
                        //*route is a circle of equal altiutde -> at the end of the animation, the chart must be centered at the center of the circle of equal altitude, i.e., at reference_position. target_omega is given by the aperture angle of the circle of equal altitude, i.e., route.omega
                        
                        target_position = (*(object->reference_position));
                        
                    }else{
                        //*route is a loxodrome or an orthodrome -> at the end of the animaiton, the chart must be centered at the middle point of *route for *route to be visible at the end of the animation. The aperture angle is estimated as half the length of *route divided by the radius of the Earth
                        
                        Length length_saved;
                        
                        if((object->length_format) == LengthFormat_types[0]){
                            //length_format = LengthFormat_types[0] -> compute length from time and speed and have it in units LengthUnit_types[0] because this is the standard unit used to draw Routes
                            
                            object->set_length_from_time_speed();
                            
                        }else{
                            //length_format = LengtFormat_types[1] -> save *length into length_saved and convert the unit of measure of *length to LengthUnit_types[0] because this is the standard unit used to draw Routes
                            
                            length_saved.set((*(object->length)));
                            object->length->convert_to(LengthUnit_types[0]);
                            
                        }
                        
                        
                        object->compute_end(((*(object->length))/2.0), String(""));
                        target_position = (*(object->end));
                        
                        //write back length_saved into *length
                        object->length->set(length_saved);
                        
                    }
                    
                    //compute the size of the object to which the animation is directed and store it into target_size
                    object->size_Mercator(&target_size);
                    
                }
                
                if(std::is_same<T, Position>::value){
                    //object is a Position
                    
                    Position* object;
                    
                    object = ((Position*)object_in);
                    
                    //the target Position of the animation is *object
                    target_position = (*object);
                    //Positions do not have a size such as Routes -> create a Route of type Route_types[2] which has target_position as a reference_position and which has an aprture angle equal to angle_zoom_to_position -> compute its size in the mercator projection and write it into target_size
                    Route(Route_types[2], target_position, wxGetApp().angle_zoom_to_position).size_Mercator(&target_size);
                    
                }
                
                center.lambda.set((((*((chart_frames[i])->lambda_min)) + (*((chart_frames[i])->lambda_max))).value)/2.0);
                center.phi.set(mean_pm_pi((*((chart_frames[i])->phi_min)), (*((chart_frames[i])->phi_max))));
                
                rectangle_observer_0->SizeMercator(&size_rectangle_observer_0);
                
                zoom_factor.value = (wxGetApp().chart_transport_zoom_factor_coefficient.value) * min((size_rectangle_observer_0.x)/(target_size.x), (size_rectangle_observer_0.y)/(target_size.y));
                
                
                chart_transport_handlers[i] = new ChartTransportHandler<F>(
                                                                           (chart_frames[i]),
                                                                           Route(
                                                                                 Route_types[0],
                                                                                 center,
                                                                                 target_position
                                                                                 ),
                                                                           zoom_factor,
                                                                           f
                                                                           );
                
                //trigger the animation
                (chart_transport_handlers[i])->operator()();
                
                break;
                
            }
                
            case 1: {
                //I am using Projection_types[1]
                
                //the aperture angle of circle_observer at the end of the animation
                Angle omega_end;
                
                
                if(std::is_same<T, Route>::value){
                    //object is a Route
                    
                    //I introduce the Route* object and set object_in = object by casting object_in into a Route pointer. This is necessary to make this method work with multiple types T (T=Position, T=Route, ...)
                    Route* object;
                    
                    object = (Route*)object_in;
                    
                    if(object->type == Route_types[2]){
                        //*route is a circle of equal altiutde -> at the end of the animation, the chart must be centered at the center of the circle of equal altitude, i.e., at reference_position. target_omega is given by the aperture angle of the circle of equal altitude, i.e., route.omega
                        
                        target_position = (*(object->reference_position));
                        omega_end = object->omega;
                        
                        
                    }else{
                        //*route is a loxodrome or an orthodrome -> at the end of the animaiton, the chart must be centered at the middle point of *route for *route to be visible at the end of the animation. The aperture angle is estimated as half the length of *route divided by the radius of the Earth
                        
                        object->set_length_from_time_speed();
                        
                        object->compute_end(((*(object->length))/2.0), String(""));
                        target_position = (*(object->end));
                        
                        omega_end = (object->length->value)/2.0/(wxGetApp().Re.value);
                        
                    }
                    
                }
                
                if(std::is_same<T, Position>::value){
                    //object is a Position
                    
                    Position* object;
                    
                    object = ((Position*)object_in);
                    
                    //the target Position of the animation is *object
                    target_position = (*object);
                    
                    //Positions do not have a size such as Routes -> set omega_end equal to  angle_zoom_to_position projection and write it into target_size
                    omega_end.set(wxGetApp().angle_zoom_to_position);
                    
                }
                
                //compute the distance between the start and end poisition of the proposed andimation and store it in d
                target_position.distance(*((chart_frames[i])->draw_panel->circle_observer->reference_position), &d, String(""), String(""));
                
                //I do the animaiton only if the start and end position of the animation are large enough, in order to avoid NaNs in the transporting_route
                if (d > (wxGetApp().minimal_animation_distance_over_size_of_observer_region.value) * ((wxGetApp().Re.value)*(chart_frames[i])->draw_panel->circle_observer->omega.value)) {
                    
                    chart_transport_handlers[i] = new ChartTransportHandler<F>(
                                                                               (chart_frames[i]),
                                                                               Route(
                                                                                     Route_types[1],
                                                                                     (*((chart_frames[i])->draw_panel->circle_observer->reference_position)),
                                                                                     target_position
                                                                                     ),
                                                                               Double( ((wxGetApp().chart_transport_zoom_factor_coefficient.value) *  (circle_observer_0->omega.value) / (omega_end.value) ) ),
                                                                               f
                                                                               );
                    
                    //trigger the animation
                    (chart_transport_handlers[i])->operator()();
                    
                    
                }
                
                break;
                
            }
                
        }
        
    }
    
    
}

template void ListFrame::AnimateToObjectOld<Position, UnsetIdling<ListFrame>>(Position*, UnsetIdling<ListFrame>*);
template void ListFrame::AnimateToObjectOld<Route, HighlightObject<ListFrame>>(Route*, HighlightObject<ListFrame>*);
template void ListFrame::AnimateToObjectOld<Position, HighlightObject<ListFrame>>(Position*, HighlightObject<ListFrame>*);


//compute the astronomical position and updated all the GUI fields in set() and re-draws everything
template<class T> void ListFrame::ComputePosition([[maybe_unused]] T& event) {
    
    ShowQuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>* print_question;
    AllRoutes* all_routes;
    SomeRoutes* some_routes;
    
    all_routes = new AllRoutes(this);
    some_routes = new SomeRoutes(this);
    print_question = new ShowQuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>(this, all_routes, some_routes, unset_idling);
    
    selecting_route_for_position = true;
    
    //ask the user whether he/she wants to transport the sight with a an existing route or with a new route.
    print_question->SetAndCall(NULL, String("You want to determine the astronomical position"), String("With what route do you want to do it?"), String("All routes"), String("Some routes"));
    
}


#ifdef WIN32
//this method is used on WIN32 only, and it is called everytime *timer triggers a wxTimerEvent: it sets refresh to true, because enough time has elapsed since the last Refresh() during the drag of a graphical object, thus allowing for a new Refresh().
void ListFrame::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    
    refresh = true;
    
}
#endif
