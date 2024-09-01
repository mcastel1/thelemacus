//
//  chart_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "chart_frame.h"

#include "bitmap.h"
#include "chart_transport_handler.h"
#include "constants.h"
#include "generic.h"
#include "static_length_field.h"
#include "units.h"
#include "unset_idling.h"

template<class P> class ChartTransportHandler;


ChartFrame::ChartFrame(ListFrame* parent_in, Projection projection_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_in, wxID_ANY, title, pos, size) {

    unsigned long long int i, j;
    String new_prefix, color;
    //empty wxStaticTexts to fill the empty spaces of the wxGridSizer sizer_buttons
    StaticText *empty_text_1, *empty_text_2, *empty_text_3, *empty_text_4, *empty_text_5;
    wxCommandEvent dummy_event;
    //a wxSizerFlags object to insert stuff into sizers
    wxSizerFlags flags;


    parent = parent_in;
    projection.set(projection_in);

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    flags.Center();
    
    lambda_min = new Angle;
    lambda_max = new Angle;
    phi_min = new Angle;
    phi_max = new Angle;

    //when a ChartFrame is created, the chart is not being dragged
    dragging_chart = false;

    //read lambda_min, ...., phi_max from file_init
    lambda_min->read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    lambda_max->read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    phi_min->read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    phi_max->read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), new_prefix);


    this->Bind(wxEVT_CLOSE_WINDOW, &ChartFrame::OnPressCtrlW<wxCloseEvent>, this);

    mouse_scrolling = false;
    //set the zoom factor to 1 for the initial configuration of the projection
    zoom_factor.set(1.0);
    //read zoom_factor_max from file_init
    wxGetApp().zoom_factor_max.read_from_file_to(String("maximal zoom factor"), (wxGetApp().path_file_init), String("R"), String(""));
    idling = false;
    unset_idling = new UnsetIdling<ChartFrame>(this);
    
    //coastline_polygons_now/before and polygon_position_now/before are resized to their maximum possible value
    for(i=0, j=0; i<parent->coastline_polygons_Position.size(); i++) {
        j += (parent->coastline_polygons_Position[i].size());
    }
    
    curves.points.reserve(j);
    curves.positions.reserve(parent->n_all_coastline_points);
    //    coastline_positions.reserve(parent->n_all_coastline_points);
    //    coastline_positions.resize(parent->coastline_polygons_Position.size());
    
    
    //    coastline_polygons_before.resize(j);
    //    polygon_position_before.reserve(parent->n_all_coastline_points);
    //    polygon_position_before.resize(parent->coastline_polygons_Position.size());
    
    print_error_message = new PrintMessage<ChartFrame, UnsetIdling<ChartFrame> >(this, unset_idling);

    panel = new ChartPanel(this, wxDefaultPosition, wxDefaultSize);
    draw_panel = new DrawPanel(panel, wxDefaultPosition, wxDefaultSize);

    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_slider = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxGridSizer(3, 3, 0, 0);
    
    //initialize the variable neededed for slider
    //allocate the slider
    slider = new wxSlider(panel, wxID_ANY, 1, 1, (int)((wxGetApp().zoom_factor_max).value), wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);

    //sets the coefficients for the function which relates the zoom factor to the slider value: read from file (wxGetApp().e_zoom) and set (wxGetApp().a_zoom), (wxGetApp().b_zoom)
    (wxGetApp().e_zoom).read_from_file_to(String("exponent zoom"), (wxGetApp().path_file_init), String("R"), String(""));
    (wxGetApp().a_zoom).set((-1.0 + ((wxGetApp().zoom_factor_max).value)) / (-1.0 + pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value)));
    (wxGetApp().b_zoom).set((pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value) - ((wxGetApp().zoom_factor_max).value)) / (-1.0 + pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value)));
 

    //text field showing the current value of the zoom slider
    chart_scale = new StaticText(panel, wxS(""), wxDefaultPosition, wxDefaultSize, 0);
    observer_height = new StaticLengthField<ChartFrame>(panel, draw_panel->d, &(wxGetApp().list_frame->data->recent_length_units));

    //navigation buttons
    button_up = new wxButton(panel, wxID_ANY, wxT("N"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_down = new wxButton(panel, wxID_ANY, wxT("S"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_left = new wxButton(panel, wxID_ANY, wxT("W"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_right = new wxButton(panel, wxID_ANY, wxT("E"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
//    button_reset = new wxButton(panel, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize);
    
    
    button_reset = new wxBitmapButton(
                                      panel,
                                      wxID_ANY,
                                      Bitmap(wxGetApp().path_file_reset_icon, (wxGetApp().size_large_button) - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                      wxDefaultPosition,
                                      wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)),
                                      wxBU_EXACTFIT | wxSIMPLE_BORDER
                                      );
    
    //button to show list
    button_show_list = new wxBitmapButton(
                                          panel,
                                          wxID_ANY,
                                          Bitmap(wxGetApp().path_file_list_icon, (wxGetApp().size_large_button) - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                          wxDefaultPosition,
                                          wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)),
                                          wxBU_EXACTFIT | wxSIMPLE_BORDER
                                          );
    button_show_list->Bind(wxEVT_BUTTON, &MyApp::ShowList, &wxGetApp());
    button_show_list->SetToolTip(wxString("Show the list of sights, positions and routes"));

    projection_field = new ProjectionField<ChartFrame>(panel, &projection, &(wxGetApp().list_frame->data->recent_projections));
    projection_field->Bind(wxEVT_COMBOBOX, &DrawPanel::OnChooseProjection<wxCommandEvent>, draw_panel);
    projection_field->SetToolTip(String("Choose the projection used in the chart"));

    button_up->Bind(wxEVT_BUTTON, &ChartFrame::MoveNorth<wxCommandEvent>, this);
    button_down->Bind(wxEVT_BUTTON, &ChartFrame::MoveSouth<wxCommandEvent>, this);
    button_left->Bind(wxEVT_BUTTON, &ChartFrame::MoveWest<wxCommandEvent>, this);
    button_right->Bind(wxEVT_BUTTON, &ChartFrame::MoveEast<wxCommandEvent>, this);
    button_reset->Bind(wxEVT_BUTTON, &ChartFrame::ResetRenderAnimate<wxCommandEvent>, this);
    button_reset->SetToolTip("Reset the chart");

    //bind all the elemetns of *this to KeyDown method
    Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    panel->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    draw_panel->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    projection_field->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);

    draw_panel->Bind(wxEVT_KEY_DOWN, &DrawPanel::KeyDown, draw_panel);
    panel->Bind(wxEVT_KEY_DOWN, &DrawPanel::KeyDown, draw_panel);

    draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, draw_panel);
    draw_panel->Bind(wxEVT_RIGHT_DOWN, &DrawPanel::OnMouseRightDown, draw_panel);
    draw_panel->Bind(wxEVT_LEFT_DOWN, &DrawPanel::OnMouseLeftDown, draw_panel);
    draw_panel->Bind(wxEVT_LEFT_UP, &DrawPanel::OnMouseLeftUp, draw_panel);
    draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseDrag, draw_panel);
    draw_panel->Bind(wxEVT_MOUSEWHEEL, &DrawPanel::OnMouseWheel, draw_panel);

    slider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &ChartFrame::OnScroll<wxCommandEvent>, this);
    slider->Bind(wxEVT_LEFT_DOWN, &ChartFrame::OnMouseLeftDownOnSlider<wxMouseEvent>, this);
    slider->Bind(wxEVT_LEFT_UP, &ChartFrame::OnMouseLeftUpOnSlider<wxMouseEvent>, this);
    slider->SetToolTip(wxString("Zoom in or out"));

    empty_text_1 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_2 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_3 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_4 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_5 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);


//    projection_field->object->set(Projection_types[wxGetApp().list_frame->data->recent_projections.front()]);
    projection_field->set();

    (projection_field->value_before_editing) = (projection_field->name->GetValue());

    //create a dummy_event and then call SetProjection(dummy_event) to set all objects according to the choice of the projeciton above.
    //    draw_panel->OnChooseProjection(dummy_event);
    draw_panel->SetProjection(dummy_event);

    //stores the x_min .. y_max, (size_chart.GetWidth()), height chart the first time that the chart is shown into x_min_0 ... height_chart_0
    (draw_panel->*(draw_panel->Set_x_y_min_max))();
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);
    (draw_panel->x_span_0) = (draw_panel->x_span());
    
    ResetRender(dummy_event);

    (draw_panel->width_chart_0) = (draw_panel->size_chart.GetWidth());
    (draw_panel->height_chart_0) = (draw_panel->size_chart.GetHeight());


    sizer_buttons->Add(empty_text_1, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_up, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_2, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_left, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_3, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_right, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_4, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_down, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_5, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

    sizer_slider->Add(slider, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_slider->Add(chart_scale, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    observer_height->StaticLengthField<ChartFrame>::InsertIn(sizer_slider, flags);
    sizer_slider->Add(sizer_buttons, 0, wxALIGN_CENTER | wxALL, 0);
    sizer_slider->Add(button_reset, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    projection_field->InsertIn(sizer_slider, flags);
    sizer_slider->AddStretchSpacer(1);
    sizer_slider->Add(button_show_list, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));


    sizer_h->Add(draw_panel, 0, wxALIGN_TOP | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_h->Add(sizer_slider, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    //    sizer_v->Add(text_position_now, 0, wxALIGN_LEFT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
        //    sizer_v->Fit(panel);

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_v);
    panel->Fit();
    Fit();

    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //    CenterOnScreen();

}

//when a ChartFrame is closed, I destroy it, delete the respecive item in parent->chart_frames vector, and rename all the other ChartFrames in that vector to take into account the shifting of the CartFrame ids due to the Chartframe deletion
template<class T> void ChartFrame::OnPressCtrlW(T& event) {

    vector<ChartFrame*>::iterator i;
    unsigned int j;
    stringstream s;

    i = find((parent->chart_frames).begin(), (parent->chart_frames).end(), this);


    Destroy();  // you may also do:  event.Skip();

    parent->chart_frames.erase(i);
    for (j = 0; j < (parent->chart_frames).size(); j++) {

        s.str("");
        s << "Chart #" << j + 1 << " - " << (((parent->chart_frames)[j])->projection.value) << " projection";

        ((parent->chart_frames)[j])->SetLabel(wxString(s.str().c_str()));

    }

    //given that one ChartFrame is gone, I show all ChartFrames and positions them properly on the screen
    wxGetApp().ShowCharts(event);
    wxGetApp().AnimateCharts();

}

//explicit instantiations
template void ChartFrame::OnPressCtrlW<wxCommandEvent>(wxCommandEvent&);


//moves (makes slide) to the north the chart
template<class T> void ChartFrame::MoveNorth(T& event) {

    //I am dragging the chart and the size of *this will not change -> set
    dragging_chart = true;


    switch (position_in_vector(projection, Projection_types)) {
            
        case 0: {
            //I am using the mercator projection
            
            double delta;
            PositionProjection p_ceil_min, p_floor_max;
            
            //I set delta as a fraction of y_max - y_min
            delta = ((wxGetApp().relative_displacement).value) * ((draw_panel->y_max) - (draw_panel->y_min));
            
            (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(deg_to_rad * floor_max_lat)), &p_floor_max, true);
            (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(deg_to_rad * ceil_min_lat)), &p_ceil_min, true);
            
            
            if (((draw_panel->y_max) + delta < (p_floor_max.y)) && ((draw_panel->y_min) + delta > (p_ceil_min.y))) {
                //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart
                
                //update y_min, y_max according to the drag.
                (draw_panel->y_min) += delta;
                (draw_panel->y_max) += delta;
                
                (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();
                
                
            }
            
            break;
            
        }
            
            
        case  1: {
            //I am using the 3d projection
            
            Angle /*the angular displacement of the operation MoveNorth*/delta;
            
            //I set delta as a fraction of circle_obsrever.omega
            delta = (draw_panel->circle_observer->omega) * ((wxGetApp().relative_displacement).value);
            
            //since I am moving north, I increase the b Euler ancgle of rotation
            (draw_panel->rotation->b) += delta;
            //I update rotation->matrix
            draw_panel->rotation->set(draw_panel->rotation->a, draw_panel->rotation->b, draw_panel->rotation->c);
            
            
            
            break;
            
        }
            
    }

    //re-draw the charton
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
//    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//explicit instantiations
template void ChartFrame::MoveNorth<wxKeyEvent>(wxKeyEvent&);



//moves (makes slide) to the south the chart
//moves (makes slide) up the chart
template<class T> void ChartFrame::MoveSouth(T& event) {

    //I am dragging the chart and the size of *this will not change -> set
    dragging_chart = true;

    switch(position_in_vector(projection, Projection_types)) {
            
        case 0: {
            //I am using the mercator projection
            
            
            double delta;
            PositionProjection p_ceil_min, p_floor_max;
            
            //I set delta as a fraction of y_max - y_min
            delta = ((wxGetApp().relative_displacement).value) * ((draw_panel->y_max) - (draw_panel->y_min));
            
            (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(deg_to_rad * floor_max_lat)), &p_floor_max, true);
            (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(deg_to_rad * ceil_min_lat)), &p_ceil_min, true);
            
            
            if (((draw_panel->y_max) - delta < (p_floor_max.y)) && ((draw_panel->y_min) - delta > (p_ceil_min.y))) {
                //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart
                
                //update y_min, y_max according to the drag.
                (draw_panel->y_min) -= delta;
                (draw_panel->y_max) -= delta;
                
                (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();
                
                
            }
            
            break;
            
        }
            
            
        case  1: {
            //I am using the 3d projection
            
            Angle /*the angular displacement of the operation MoveNorth*/delta;
            
            //I set delta as a fraction of circle_obsrever.omega
            delta = (draw_panel->circle_observer->omega) * ((wxGetApp().relative_displacement).value);
            
            //since I am moving north, I increase the b Euler ancgle of rotation
            (draw_panel->rotation->b) -= delta;
            //I update rotation->matrix
            draw_panel->rotation->set(draw_panel->rotation->a, draw_panel->rotation->b, draw_panel->rotation->c);
            
            break;
            
        }
            
            
    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
//    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//explicit instantiations
template void ChartFrame::MoveSouth<wxKeyEvent>(wxKeyEvent&);



//moves (makes slide) to the west the chart
template<class T> void ChartFrame::MoveWest(T& event) {

    //I am dragging the chart and the size of *this will not change -> set
    dragging_chart = true;


    switch (position_in_vector(projection, Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;

        delta = ((wxGetApp().relative_displacement).value) * (draw_panel->x_span());

        //update lambda_min, lambda_max according to the drag.
        (lambda_min->value) += delta;
        (lambda_max->value) += delta;

        lambda_min->normalize();
        lambda_max->normalize();

        (draw_panel->*(draw_panel->Set_x_y_min_max))();

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = (draw_panel->circle_observer->omega) * (wxGetApp().relative_displacement.value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation->a) -= delta;
        //I update rotation->matrix
        draw_panel->rotation->set(draw_panel->rotation->a, draw_panel->rotation->b, draw_panel->rotation->c);



        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
//    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//explicit instantiations
template void ChartFrame::MoveWest<wxKeyEvent>(wxKeyEvent&);




//moves (makes slide) to the east the chart
template<class T> void ChartFrame::MoveEast(T& event) {

    //I am dragging the chart and the size of *this will not change -> set
    dragging_chart = true;

    switch (position_in_vector(projection, Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;

        delta = ((wxGetApp().relative_displacement).value) * (draw_panel->x_span());

        //update lambda_min, lambda_max according to the drag.
        (lambda_min->value) -= delta;
        (lambda_max->value) -= delta;

        lambda_min->normalize();
        lambda_max->normalize();

        (draw_panel->*(draw_panel->Set_x_y_min_max))();

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer)->omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation->a) += delta;
        //I update rotation->matrix
        draw_panel->rotation->set(draw_panel->rotation->a, draw_panel->rotation->b, draw_panel->rotation->c);



        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
//    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;


    event.Skip(true);

}

//explicit instantiations
template void ChartFrame::MoveEast<wxKeyEvent>(wxKeyEvent&);





//this method is called when a key is pressed
template<class T> void ChartFrame::KeyDown(T& event) {

    if (wxGetKeyState(WXK_CONTROL)) {
        //the command key (APPLE operating system) or the control key (WIN32 operating system) is down

        switch ((event.GetUnicodeKey())) {

        case 'Q': {
            //command (APPLE) or control (WIN32) + q key has been pressed

            wxGetApp().OnPressCtrlQ(event);

            break;

        }

        case 'W': {
            //command (APPLE) or control (WIN32) + w key has been pressed

            parent->OnPressCtrlW(event);

            break;

        }

        case 'S': {

            if (wxGetKeyState(WXK_SHIFT)) {

                //command (APPLE) or control (WIN32) + shift + s key has been pressed

                parent->OnPressCtrlShiftS(event);
            }
            else {
                //command (APPLE) or control (WIN32) + s key has been pressed

                parent->OnPressCtrlS(event);

            }

            break;

        }

        }

    }


    event.Skip(true);

}


//reset the chart to its starting configuration for x_min ... y_max
template<class T> void ChartFrame::Reset(T& event) {

    idling = false;
    (draw_panel->idling) = false;
    
    switch (position_in_vector(projection, Projection_types)) {
            
        case 0: {
            //read lambda_min, ...., phi_max from file_init
            lambda_min->read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
            lambda_max->read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
            phi_min->read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
            phi_max->read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
            draw_panel->Set_x_y_min_max_Mercator();
            ComputeZoomFactor_Mercator(draw_panel->x_span());
            
            break;
            
        }
            
        case 1: {
            //reset d and the earth orientation to the initial one and set the zoom factor accordingly

            parent->circle_observer_0->omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), String(""));
            zoom_factor.set(1.0);
            ComputeZoomFactor_3D();

            draw_panel->rotation_0->read_from_file_to(String("rotation 0"), (wxGetApp().path_file_init), String("R"), String(""));
            draw_panel->rotation->set((*(draw_panel->rotation_0)));
            draw_panel->Set_x_y_min_max_3D();
            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();
            
            break;
            
        }
            
    }
            

    //now that x_min ... y_max have been set, I set x_min_0 ... equal to x_min ...
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);

    event.Skip(true);

}


//call Reset and Render everything
template<class T> void ChartFrame::ResetRender(T& event) {
    
    Reset<T>(event);
    
    (draw_panel->*(draw_panel->PreRender))();
    //now that (size_chart.GetWidth()) and (size_chart.GetHeight()) have been set, I set width_chart_0 and height_chart_0 equal to width_chart and (size_chart.GetHeight())
    (draw_panel->width_chart_0) = (draw_panel->size_chart.GetWidth());
    (draw_panel->height_chart_0) = (draw_panel->size_chart.GetHeight());
    (draw_panel->x_span_0) = (draw_panel->x_span());;
    
    
    draw_panel->CleanAndRenderAll();
    
    draw_panel->FitAll();
    UpdateSlider();
    //     Animate();
    
    event.Skip(true);

}


template<class T> void ChartFrame::ResetRenderAnimate(T& event) {
    
    ResetRender(event);
    Animate();

}




//makes a nice animation to present *this, by dragging the center of the chart to the desired Position from a Position on the antipodes
void ChartFrame::Animate(void){
    
    //the transport handler used to transport the chart in *this
    ChartTransportHandler< UnsetIdling<ListFrame> >* chart_transport_handler;
        
    //allocate chart_transport_handler and set the starting Position and the Route for the transport
    switch (position_in_vector(projection, Projection_types)) {
            
        case 0: {
            //I am using Projection_types[0]
            
            PositionProjection q_NE, q_SW, q_center;
            Position p;
            
            q_NE.NormalizeAndSetMercator(Position((*lambda_max), (*phi_max)));
            q_SW.NormalizeAndSetMercator(Position((*lambda_min), (*phi_min)));
            q_center = (q_NE + q_SW)/2;
            
            (draw_panel->*(draw_panel->ProjectionToGeo))(q_center, &p);
            
            chart_transport_handler = new ChartTransportHandler< UnsetIdling<ListFrame> >(
                                                                this,
                                                                Route(
                                                                      Route_types[0],
                                                                      p.antipode_lambda(),
                                                                      p
                                                                      ),
                                                                Double(1.0),
                                                                      parent->unset_idling
                                                                );

            //trigger the animation
            chart_transport_handler->operator()();
            
            
            break;
            
        }
            
        case 1: {
            //I am using Projection_types[1]
            
            chart_transport_handler = new ChartTransportHandler< UnsetIdling<ListFrame> >(
                                                                this,
                                                                Route(
                                                                      Route_types[1],
                                                                      draw_panel->circle_observer->reference_position->half(),
                                                                      (*(draw_panel->circle_observer->reference_position))
                                                                      ),
                                                                Double(1.0),
                                                                      parent->unset_idling
                                                                );

            //trigger the animation
            chart_transport_handler->operator()();
            
            break;
            
        }
            
    }
        
}





//updates the text in text_slider by writing in it the current value of the zoom factor, rounded to an integer for clarity
void ChartFrame::UpdateSliderLabel_Mercator(void) {

    stringstream s;
    Int scale_factor;
    
    //compute the chart_scale factor
    scale_factor.set( ((unsigned int)(
                   /*length of the NS edge of the plot area as measured on the surface of the earth, in  [nm]*/(((phi_max->normalize_pm_pi_ret().value) - (phi_min->normalize_pm_pi_ret().value)) * rad_to_deg * 60.0) / ( /*length of the NS edge of the plot area as shown on the screen of the computer, in [nm]*/((double)(draw_panel->size_plot_area.y))/((double)(wxGetApp().display.GetPPI().x)) * inch_to_km/nm_to_km ) )) );
    
    
    scale_factor.my_round(display_precision);
    

    s.str("");
    s << "1:" << scale_factor.to_string_spaces().value;
    chart_scale->SetLabel(s.str().c_str());
    
    //fir *this in order to account for the sliderlabal which has changed
    Fit();
    

}

//update the text in text_slider in the 3D projection: for the 3D projection the altitude of the observer is written into observer_height, thus text_slider is set to empty
void ChartFrame::UpdateSliderLabel_3D(void) {

    chart_scale->SetLabel("");

}

//computes the zoom factor of the chart based on the currenct value of span_x. It returns true and writes the value in zoom_factor if the zooming factor is smaller than (wxGetApp().zoom_factor_max), and returns false otherwise
bool ChartFrame::ComputeZoomFactor_Mercator(double delta_x) {

    double temp;
    bool output;

    temp = ((double)((draw_panel->size_chart).GetWidth())) / ((double)(draw_panel->width_chart_0)) * ((draw_panel->x_max_0) - (draw_panel->x_min_0)) / delta_x;

    output = ((1.0 <= temp) && (temp <= (wxGetApp().zoom_factor_max.value)));

    if (output) {
        zoom_factor.set(temp);
    }

    return(output);

}

//returns true if zoom_factor is valid, false otherwise
bool ChartFrame::ComputeZoomFactor_3D(void) {

    bool output;

    output = ((1.0 <= (zoom_factor.value)) && ((zoom_factor.value) <= (wxGetApp().zoom_factor_max.value)));

    if (output) {

        draw_panel->circle_observer->omega.set((parent->circle_observer_0->omega.value) / (zoom_factor.value));

    }

    return(output);

}

//this function updates the slider according to the zoom factor of the chart: it sets the slider value to the integer value closest to zoom_factor
void ChartFrame::UpdateSlider(void) {

    int temp;

    //compute the zoom factor of the chart and write it into zoom_factor

    
    switch (position_in_vector(projection, Projection_types)) {
            
        case 0: {
            ComputeZoomFactor_Mercator((draw_panel->x_span()));
            
            break;
            
        }
            
        case 1: {
            
            //is this necessary here ?
            ComputeZoomFactor_3D();
            
            break;
            
        }
            
    }


    //a tentative value for the value of slizer
    temp = round(pow(((zoom_factor.value) - ((wxGetApp().b_zoom).value)) / ((wxGetApp().a_zoom).value), 1.0 / ((wxGetApp().e_zoom).value)));


    //if the tentative value exceeds the slider boundaries, I set it to the respective boundary
    if (temp > (slider->GetMax())) { temp = (slider->GetMax()); }
    if (temp < 1) { temp = 1; }

    //    cout << "\t\t\ttemp = " << temp << "\n";
    //    cout << "\t\t\tzoom_factor = " << (zoom_factor.value) << "\n";

    slider->SetValue(temp);

    (this->*UpdateSliderLabel)();

}



//#include "limb_field.cpp"





template<class T> void ChartFrame::OnMouseLeftDownOnSlider(T& event) {

    //mouse scrolling starts
    mouse_scrolling = true;

    if (projection == Projection_types[0]) {

        (draw_panel->x_center_scrolling) = ((draw_panel->x_min) + (draw_panel->x_max)) / 2.0;
        (draw_panel->y_center_scrolling) = ((draw_panel->y_min) + (draw_panel->y_max)) / 2.0;

    }

    event.Skip(true);

}



template<class T> void ChartFrame::OnMouseLeftUpOnSlider(T& event) {

    //mouse scrolling ends
    mouse_scrolling = false;

    event.Skip(true);

}


template<class T> void ChartFrame::OnScroll(/*wxScrollEvent*/ T& event) {

    /*
     n = value of slider,
     z = zoom factor,

     1 <= z <= (wxGetApp().zoom_factor_max)
     1 <= n <= n_max

     n = ((double)(slider->GetValue()));
     n_max = ((double)(slider->GetMax()))

     z = (wxGetApp().a_zoom)*n^(wxGetApp().e_zoom) + (wxGetApp().b_zoom)
     n = log((z-(wxGetApp().b_zoom))/(wxGetApp().a_zoom))/(wxGetApp().e_zoom)


     (wxGetApp().a_zoom) = (-1 + (wxGetApp().zoom_factor_max))/(-1 + n_max^(wxGetApp().e_zoom));
     (wxGetApp().b_zoom) = (n_max^(wxGetApp().e_zoom) - (wxGetApp().zoom_factor_max))/(-1 + n_max^(wxGetApp().e_zoom));

     z = w/delta_x / (w_0/delta_x_0)

     delta_x = w/z/ (w_0/delta_x_0) = w*delta_x_0/(z*w_0)

     (size_chart.GetHeight())/(size_chart.GetWidth()) * x_span = (y_max-y_min);


     */


     //    cout << "Slider getvalue = " << ((double)(slider->GetValue())) << "\n";
     //     cout << "Zoom factor = " << ((double)zoom_factor.value) << "\n";

     //set zoom_factor from the value of slider
    zoom_factor.set(
        String(""),
        ((wxGetApp().a_zoom).value) * pow((slider->GetValue()), (wxGetApp().e_zoom).value) + ((wxGetApp().b_zoom).value)
        ,
        String("")
    );

    //if the resulting value of zoom_factor is outside the boundaries I set it back to the rspective boundary
    if (((zoom_factor.value) < 1.0) || ((slider->GetValue()) == (slider->GetMin()))) {

        (zoom_factor.value) = 1.0;

    }

    if (((zoom_factor.value) > (wxGetApp().zoom_factor_max.value)) || ((slider->GetValue()) == (slider->GetMax()))) {

        (zoom_factor.value) = (wxGetApp().zoom_factor_max.value);

    }

    switch (position_in_vector(projection, Projection_types)) {
            
        case 0: {
            
            PositionProjection p_min, p_max;

            //update x_min, ..., y_max according to the zoom (scroll) and *lambda_min, ..., *phi_max
            (draw_panel->x_min) = ((double)((draw_panel->x_center_scrolling))) - (((double)(((draw_panel->size_chart).GetWidth()) * (draw_panel->x_span_0))) / ((double)(((zoom_factor.value) * (draw_panel->width_chart_0))))) / 2.0;
            (draw_panel->x_max) = ((double)((draw_panel->x_center_scrolling))) + (((double)(((draw_panel->size_chart).GetWidth()) * (draw_panel->x_span_0))) / ((double)(((zoom_factor.value) * (draw_panel->width_chart_0))))) / 2.0;
            (draw_panel->y_min) = ((double)((draw_panel->y_center_scrolling))) - (((double)(((draw_panel->size_chart).GetHeight()) * (draw_panel->x_span()))) / ((double)((draw_panel->size_chart).GetWidth()))) / 2.0;
            (draw_panel->y_max) = ((double)((draw_panel->y_center_scrolling))) + (((double)(((draw_panel->size_chart).GetHeight()) * (draw_panel->x_span()))) / ((double)((draw_panel->size_chart).GetWidth()))) / 2.0;

            (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(max_lat)), &p_max, true);
            (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(min_lat)), &p_min, true);

            if ((((draw_panel->y_max) <= (p_max.y)) && ((draw_panel->y_min) >= (p_min.y)) && ((draw_panel->x_span()) <= 2.0 * M_PI))) {

                (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();

                (draw_panel->*(draw_panel->PreRender))();
                draw_panel->MyRefresh();
                draw_panel->FitAll();
                UpdateSlider();

            }
            else {
                //if the drag operation brings the chart out of the min and max latitude contained in the data files, I reset x_min, ..., y_max and the value of the slider to the values at the beginning of the drag, and set *lambda_min, ..., *phi_max accordingly.

                //uncomment this if you want to print an error message
                //print_error_message->SetAndCall(NULL, String("Error"), String("You moved the slider: Chart outside  boundaries! The chart must lie within the boundaries."));

                //I reset the chart to its original configuration
                Reset<T>(event);

            }
            
            break;
            
        }
            
        case 1: {
            
            (draw_panel->circle_observer->omega) = ((parent->circle_observer_0->omega) / (zoom_factor.value));

            (draw_panel->*(draw_panel->PreRender))();
            draw_panel->MyRefresh();
            draw_panel->FitAll();

            UpdateSlider();
            
            break;
            
        }
            
    }
    
    event.Skip(true);

}

//If slider->GetMin() <= slider_value <= slider->GetMax() it returns true, and sets slider to slider_value and updates the chart zoom accordingly. It returns false otherwise
bool ChartFrame::SetSlider(unsigned int slider_value) {

    if ((slider_value >= ((unsigned int)(slider->GetMin()))) && (slider_value <= ((unsigned int)(slider->GetMax())))) {

        wxCommandEvent dummy;

        if (!mouse_scrolling) {
            OnMouseLeftDownOnSlider(dummy);
        }
        slider->SetValue(slider_value);

        //call OnScroll to update evrything adter the change of the value of slider
        OnScroll(dummy);
        OnMouseLeftUpOnSlider(dummy);

        return true;

    }
    else {

        return false;

    }


}



//get the datapoints of curves and store them into parent->coastline_points
void ChartFrame::GetCoastLineData3D(void) {

    unsigned long long int i, j;
    //integer values of min/max lat/lon to be extractd from p_coastline
    int/*  i_adjusted = 0, j_adjusted = 0,*/ i_min, i_max, j_min, j_max;
//    double /*the cosine of the angle between the vector with latitude and longitude i, j (see below) and the vector that connects the center ofr the Earth to circle_observer.reference_position*/cos;
    PositionProjection temp;
    wxPoint q;
    Cartesian r, s;
    Position u;


    //set i_min/max, j_min/max
    i_min = floor(rad_to_deg * (phi_min->normalize_pm_pi_ret().value));
    i_max = ceil(rad_to_deg * (phi_max->normalize_pm_pi_ret().value));

    if (((*lambda_min) == 0.0) && ((*lambda_max) == 0.0)) {
        //in this case,Set_lambda_phi_min_max found out that circle_observer spans all longitudes, thus I set

        j_min = 0;
        j_max = 360;

    }
    else {
        //in this case, Set_lambda_phi_min_max found out that there are two finite longitudes which encircle circle_observer, thus I set

        if (((*lambda_min) < M_PI) && ((*lambda_max) > M_PI)) {

            j_min = floor(rad_to_deg * (lambda_max->value));
            j_max = 360 + ceil(rad_to_deg * (lambda_min->value));

        }
        else {

            j_min = floor(rad_to_deg * (lambda_max->value));
            j_max = ceil(rad_to_deg * (lambda_min->value));

        }

    }
    
    if(i_min < floor_min_lat){i_min = floor_min_lat;}
    if(i_max > ceil_max_lat){i_max = ceil_max_lat;}

    
    if ((parent->show_coastlines) == Answer('y', String(""))) {
        
        unsigned long long int  l, p, m, every, n_added_polygons;
        bool new_polygon;
        
        
        //go through coastline_polygons_map and fetch the polygons that fall within *rectangle_observer and store their ids into coastline_polygons_area_observer
        for(parent->coastline_polygons_area_observer.clear(), i=i_min-floor_min_lat; i<i_max-floor_min_lat; i++) {
            for(j=j_min; j<j_max; j++) {
                
                for(l=0; l<(parent->coastline_polygons_map)[i][j % 360].size(); l++){
                    parent->coastline_polygons_area_observer.push_back((parent->coastline_polygons_map)[i][j % 360][l]);
                }
                
            }
        }
        //the procedure above may lead to duplicates into coastline_polygons_area_observer -> delete them
        delete_duplicates(&(parent->coastline_polygons_area_observer));
        
        //this is a computationally efficient way of estimating every: the number of coastline points falling within circle_observer is estimated as (parent->n_all_coastline_points)*(draw_panel->circle_observer.omega.value))/M_PI), and every is set accordingly in such a way that, for every circle_observer, the number of plotting points is n_points_plot_coastline_3D
        every = ((unsigned long long int)(((parent->n_all_coastline_points)*(draw_panel->circle_observer->omega.value))/M_PI) / ((double)(wxGetApp().n_points_plot_coastline_3D.value)));
        if(every==0){every = 1;}
        
        
        
        for(p=0, i=0, l=0, n_added_polygons=0, curves.clear(); i<parent->coastline_polygons_area_observer.size(); i++) {
            //run through polygons
            
            if((l != (curves.positions.back())) && new_polygon){
                curves.positions.push_back(l);
            }
            
            //the id of the polygon that is being added, i.e. , the # of the polygon as entry of coastline_polygons_Position
            m = (parent->coastline_polygons_area_observer)[i];
            
            for(j=p; j<(parent->coastline_polygons_Cartesian)[m].size(); j+=every){
                //run through points in a polygon
                
                if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[m][j], &q, false)){
                    
                    curves.points.push_back(q);
                    l++;
                    new_polygon = false;
                    
                }else{
                    //(parent->coastline_polygons_Position)[i][j] is not a valid point -> I start a new polygon
                    
                    new_polygon = true;
                    
                    //updated curves.positions with the position of the new polygon
                    if((l != (curves.positions.back()))){
                        curves.positions.push_back(l);
                    }
                }
                
            }
            
            p = j - ((parent->coastline_polygons_Cartesian[m]).size());
            new_polygon = true;

        }
        
        if(n_added_polygons+1 > curves.positions.size()){
            curves.positions.resize(n_added_polygons+1);
        }
        curves.positions.back() = l;
        
    }

}


//this function efficiently reads coastline data stored in data_x in the interval of latitudes (*lambda_min), (*lambda_max), (*phi_min), (*phi_max), and writes this data x and y, writing n_points points at the most. This data is stored into parent->coastline_points
void ChartFrame::GetCoastLineDataMercator(void) {

    int i_min = 0, i_max = 0, j_min = 0, j_max = 0;
    unsigned long long int i, j;
    wxPoint q;

    if (((*lambda_min) < M_PI) && ((*lambda_max) > M_PI)) {

        j_min = floor(rad_to_deg * (lambda_max->value));
        j_max = ceil(rad_to_deg * ((lambda_min->value) + 2.0*M_PI));

    }else {

        if ((*lambda_min) > (*lambda_max)) {

            j_min = floor(rad_to_deg * (lambda_max->value));
            j_max = ceil(rad_to_deg * (lambda_min->value));

        }
        else {

            j_min = floor(rad_to_deg * (lambda_max->value));
            j_max = ceil(rad_to_deg * ((lambda_min->value) + 2.0*M_PI));

        }

    }

    i_min = floor(rad_to_deg * (phi_min->normalize_pm_pi_ret().value));
    i_max = floor(rad_to_deg * (phi_max->normalize_pm_pi_ret().value));


    if ((parent->show_coastlines) == Answer('y', String(""))) {
        
        unsigned long long int n_added_polygons, l, p, m, every;
        bool new_polygon;
        
        //go through coastline_polygons_map and fetch the polygons that fall within *rectangle_observer and store their ids into coastline_polygons_area_observer
        for(parent->coastline_polygons_area_observer.clear(), i=i_min-floor_min_lat; i<i_max-floor_min_lat; i++) {
            for(j=j_min; j<j_max; j++) {
                
                for(l=0; l<(parent->coastline_polygons_map)[i][j % 360].size(); l++){
                    parent->coastline_polygons_area_observer.push_back((parent->coastline_polygons_map)[i][j % 360][l]);
                }
                
            }
        }
        //the procedure above may lead to duplicates into coastline_polygons_area_observer -> delete them
        delete_duplicates(&(parent->coastline_polygons_area_observer));
        
        //count the total number of points included in the polygons of coastline_polygons_area_observer and store them in m
        //set every in such a way that the total number of plotted points is n_points_plot_coastline_Mercator, no matter what the size of *rectangle_observer
        for(m=0, i=0; i<parent->coastline_polygons_area_observer.size(); i++) {
            for(j=0; j<(parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]].size(); j++){
                
                if((draw_panel->*(draw_panel->ProjectionToDrawPanel))((parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]][j], NULL, false)){
                
                    m++;
                }
                
            }
        }
        every = ((unsigned long long int)(((double)m) / ((double)(wxGetApp().n_points_plot_coastline_Mercator.value))));
        if(every==0){every = 1;}
        
        
        for(p=0, i=0, l=0, n_added_polygons=0; i<parent->coastline_polygons_area_observer.size(); i++) {
            //run through polygons
            
            if((l != (curves.positions.back())) && new_polygon){
                curves.positions.push_back(l);
            }
            
            //the id of the polygon that is being added, i.e. , the # of the polygon as entry of coastline_polygons_Position
            m = (parent->coastline_polygons_area_observer)[i];
            
            for(j=p; j<(parent->coastline_polygons_Mercator)[m].size(); j+=every){
                //run through points in a polygon
                
                if((draw_panel->*(draw_panel->ProjectionToDrawPanel))((parent->coastline_polygons_Mercator)[m][j], &q, false)){
                    //(parent->coastline_polygons_Position)[i][j] is  a valid point

                    curves.points.push_back(q);
                    l++;
                    new_polygon = false;
                    
                }else{
                    //(parent->coastline_polygons_Position)[i][j] is not a valid point -> I start a new polygon
                    
                    new_polygon = true;
                    
                    //updated curves.positions with the position of the new polygon
                    if((l != (curves.positions.back()))){
                        curves.positions.push_back(l);
                    }

                }
                
            }
            
            p = j - ((parent->coastline_polygons_Position[m]).size());
            new_polygon = true;

        }
        
        if(n_added_polygons+1 > curves.positions.size()){
            curves.positions.resize(n_added_polygons+1);
        }
        curves.positions.back() = l;
        
    }
    
}


void ChartFrame::SetIdling(bool b) {

    idling = b;

}

//I call this function when I know that all GUI fields are properly filled in ChartFrame, and thus set the non GUI Angle objects relative to the Euler angles for the rotation of the 3D earth,  and draw everything
void ChartFrame::AllOk(void){

    wxCommandEvent dummy;
    
    projection_field->MultipleItemField<ChartFrame, Projection, CheckProjection<ChartFrame> >::Get(dummy);

    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

}


//same as ChartFrame::AllOk(void), but with an event argument, so this method can be triggered from an event
template<class T> void ChartFrame::AllOk(T& event) {

    AllOk();

}


//enable all GUI fields (buttons, slider, etc) in *this if enable  = true, and disable them otherwise
void ChartFrame::EnableAll(bool enable){
    
    button_reset->Enable(enable);
    button_up->Enable(enable);
    button_down->Enable(enable);
    button_left->Enable(enable);
    button_right->Enable(enable);
    slider->Enable(enable);
    projection_field->Enable(enable);
    
}
