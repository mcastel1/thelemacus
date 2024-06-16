//
//  draw_panel.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "gsl_math.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/dcclient.h>
#include <wx/dcgraph.h>



#include "draw_panel.h"
#include "constants.h"
#include "length_unit_field.h"
#include "route_type.h"



DrawPanel::DrawPanel(ChartPanel* parent_in, const wxPoint& position_in, const wxSize& size_in) : wxPanel(parent_in, wxID_ANY, position_in, size_in, wxTAB_TRAVERSAL, wxT("")) {

    String prefix;

    prefix = String("");

    //client_dc = new wxClientDC(this);

    mouse_dragging = false;
    re_draw = true;
    mouse_in_plot_area = false;

    parent = (parent_in->parent);

    SetCursor(*wxCROSS_CURSOR);

    //text field showing the latitude and longitude of the intantaneous (now) mouse position on the chart
    label_position_now = String("");

    circle_observer.omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), prefix);
    thickness_route_selection_over_length_screen.read_from_file_to(String("thickness route selection over length screen"), (wxGetApp().path_file_init), String("R"), prefix);

    rotation.set(Rotation(
                          Angle(String("Euler angle alpha"), -M_PI_2, String("")),
                          Angle(String("Euler angle beta"), 0.0, String("")),
                          Angle(String("Euler angle gamma"), 0.0, String(""))
                          ));

    //specify that circle_observer is a circle of equal altitude
    circle_observer.type = RouteType(((Route_types[2]).value));

    //clears the vector label_phi because tehre are not y labels yet.
    parallels_and_meridians_labels_now.resize(0);
    positions_parallels_and_meridians_labels_now.resize(0);


    idling = false;
    unset_idling = new UnsetIdling<DrawPanel>(this);
    print_error_message = new PrintMessage<DrawPanel, UnsetIdling<DrawPanel> >(this, unset_idling);


    //text for the coordinates of the mouse cursor relative to the corners of the selection rectangle
    (parent->parent->start_label_selection_rectangle) = String("");
    (parent->parent->end_label_selection_rectangle_now) = String("");
#ifdef _WIN32
    (parent->parent->end_label_selection_rectangle_before) = String("");
#endif
    label_dragged_object_now = String("");

    //set the background color of *this to background_color, so there is no need to draw a rectangle filled with background_color every time a paint event is triggered -> the code is faster
    SetBackgroundColour(wxGetApp().background_color);
    //set the border of the chart area
    SetWindowStyle(wxSIMPLE_BORDER);
    
    Bind(wxEVT_PAINT, &DrawPanel::PaintEvent, this);

}


//BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
//
//// catch paint events
//EVT_PAINT(DrawPanel::PaintEvent)
//
//END_EVENT_TABLE()


inline void DrawPanel::PaintEvent([[maybe_unused]] wxPaintEvent& event) {

    wxPaintDC dc(this);

    RenderAll(dc);

}


//render the mouse position with colors foreground_color and background_color
inline void DrawPanel::RenderMousePositionLabel(
                                                wxDC& dc,
                                                const String& label_position,
                                                const wxPoint& position_label_position,
                                                wxColor foreground_color,
                                                wxColor background_color
                                                ) {

    //wipe out position_label_position by writing on top of it a rectangle filled with color backgound_color
    dc.SetPen(background_color);
    dc.SetBrush(wxBrush(background_color, wxBRUSHSTYLE_SOLID));
    dc.DrawRectangle(position_label_position, get_size(label_position, &dc));

    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawText(wxString(label_position.value), position_label_position);


}


//call either Refresh() or RefreshWIN32 according to the operating system
void DrawPanel::MyRefresh(){
    
#ifdef __APPLE__
    //I am on apple operating system-> I use the wxWidgets Refresh() method, which is fast

    
    Refresh();

#endif

#ifdef _WIN32
//I am on windows operating system-> I call RefreshWIN32() because the wxWidgets Refresh() is slow
    
    RefreshWIN32();

#endif

}

//render the coastline by using the set of points points_coastline, meridians, parallels and their labels
inline void DrawPanel::RenderBackground(
                                        wxDC& dc,
                                        const wxPoint& position_plot_area,
                                        const vector< vector< vector<wxPoint> > >& grid,
                                        const vector< vector< vector<wxPoint> > >& ticks,
                                        const vector<wxString>& parallels_and_meridians_labels,
                                        const vector<wxPoint>& positions_parallels_and_meridians_labels,
                                        const vector<unsigned long long int>& polygon_positions,
                                        const vector<wxPoint>& points_coastline,
                                        wxColour foreground_color,
                                        wxColour background_color,
                                        double thickness
) {

    if (re_draw) {

        m_bgbuffer.Create(size_chart, 32);
        m_bgbuffer.UseAlpha();

        wxMemoryDC mdc(m_bgbuffer);

        //        mdc.SetPen(wxPen(foreground_color));
        //        mdc.SetBrush(wxBrush(foreground_color));

        wxGCDC dc_m_bgbuffer(mdc);
        //this needs to be commented out in order to not show a 'trail' when dragging
        //dc_m_bgbuffer.SetBackground(*wxTRANSPARENT_BRUSH);
        //        dc_m_bgbuffer.Clear();

        dc_m_bgbuffer.SetPen(wxPen(foreground_color));
        dc_m_bgbuffer.SetBrush(wxBrush(foreground_color));
        dc_m_bgbuffer.SetTextForeground(foreground_color);
        dc_m_bgbuffer.SetTextBackground(background_color);
        
        (this->*Render)(
                        &dc_m_bgbuffer,
                        position_plot_area,
                        grid,
                        ticks,
                        parallels_and_meridians_labels,
                        positions_parallels_and_meridians_labels,
                        polygon_positions,
                        points_coastline,
                        foreground_color,
                        background_color,
                        thickness
                        );
        
        mdc.SelectObject(wxNullBitmap);

        re_draw = false;
    }

    dc.SetPen(wxPen(foreground_color));
    dc.SetBrush(wxBrush(background_color));
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawBitmap(m_bgbuffer, 0, 0, false);

}


//same as  DrawPanel::RenderSelectionRectangle(wxDC& dc, Position geo_position, wxColour foreground_color, wxColour background_color), but it takes a  position (reckoned with respect to the ordigin of *this) as input rather than a  geographic Position
inline void DrawPanel::RenderSelectionRectangle(wxDC& dc,
                                                const wxPoint& position,
                                                const wxPoint& position_end_label,
                                                const String& end_label,
                                                wxColour foreground_color,
                                                wxColour background_color) {

    Position p;

    if(DrawPanelToGeo(position, &p)){
        RenderSelectionRectangle(dc, p, position_end_label, end_label, foreground_color, background_color);
    }

}


//render a selection rectangle with end Position geo_position (geographic position), foreground color foreground_color and backgrund color background_color, and label at its endpoint end_label located at position_end_label
inline void DrawPanel::RenderSelectionRectangle(wxDC& dc,
                                                const Position& geo_position,
                                                const wxPoint& position_end_label,
                                                const String& end_label,
                                                wxColour foreground_color,
                                                wxColour background_color) {
    
    Angle lambda_a, lambda_b, lambda_ab_span, Z;


    dc.SetPen(foreground_color);
    dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);


    //I draw the four edges of the rectangle in a way that is independent of the projection used
    //right vertical edge of rectangle
    (Route(
        RouteType(((Route_types[1]).value)),
        (parent->parent->geo_position_start),
        Angle(M_PI * (1.0 - GSL_SIGN((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value))) / 2.0),
        Length(Re * fabs((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value)))
    )).Draw(((wxGetApp().n_points_routes).value), &dc, this, String(""));

    //left vertical edge of rectangle
    (Route(
           RouteType(((Route_types[1]).value)),
        geo_position,
        Angle(M_PI * (1.0 + GSL_SIGN((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value))) / 2.0),
        Length(Re * fabs((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value)))
    )).Draw(((wxGetApp().n_points_routes).value), &dc, this, String(""));

    
    //top and bottom horizontal edge of rectangle
    lambda_a = (parent->parent->geo_position_start.lambda);
    lambda_b = geo_position.lambda;
    lambda_a.normalize();
    lambda_b.normalize();
    
    switch (position_in_vector(Projection((parent->projection->name->GetValue()).ToStdString()), Projection_types)) {
            
        case 0: {
            //I am using Projection_types[0]
            

            //the following cases follow the derivation in the note 'Selection rectangle'
            if((*(parent->lambda_min)) < (*(parent->lambda_max))){
                
                if(
                   (((lambda_a > 0.0) && (lambda_a < (*(parent->lambda_min)))) && ((lambda_b > 0.0) && (lambda_b < (*(parent->lambda_min))))) ||
                   ((lambda_a > (*(parent->lambda_max))) && (lambda_b > (*(parent->lambda_max))))
                   ){
                       //{lambda_a in A & lambda_b in A} or {lambda_a in B and lambda_b in B}
                       
                       lambda_ab_span.set(fabs((lambda_b.value) - (lambda_a.value)));
                       Z = Angle(-GSL_SIGN((lambda_b.value) - (lambda_a.value)) * M_PI_2);
                       
                   }else{
                       //{lambda_a in A & lambda_b in B} or {lambda_a in B and lambda_b in A}
                       
                       lambda_ab_span.set(2.0*M_PI - fabs((lambda_b.value) - (lambda_a.value)));
                       Z = Angle(GSL_SIGN((lambda_b.value) - (lambda_a.value)) * M_PI_2);
                       
                   }
                
            }else{
                
                lambda_ab_span.set(fabs((lambda_b.value) - (lambda_a.value)));
                Z = Angle(-GSL_SIGN((lambda_b.value) - (lambda_a.value)) * M_PI_2);
                
            }
            
           
            //now that lambda_span and Z have been set, I draw the Routes corresponding to the top and bottom horizontal edges
            Route(
                  RouteType(((Route_types[0]).value)),
                  geo_position,
                  Z+M_PI,
                  Length(Re * cos(geo_position.phi) * (lambda_ab_span.value))
                  ).DrawOld((wxGetApp().n_points_routes.value), &dc, this, String(""));
            Route(
                  RouteType(((Route_types[0]).value)),
                  parent->parent->geo_position_start,
                  Z,
                  Length(Re * cos(parent->parent->geo_position_start.phi) * (lambda_ab_span.value))
                  ).DrawOld((wxGetApp().n_points_routes.value), &dc, this, String(""));
            
            break;
        }
            
        case 1: {
            //I am using Projection_types[1]
            
            Angle temp, lambda_span_temp, Z_temp;
            
            temp.value = (normalize_pm_pi_ret(geo_position.lambda).value) - (parent->parent->geo_position_start.lambda.normalize_pm_pi_ret().value);
            
            if(fabs(temp.value) < M_PI){
                lambda_span_temp.set(fabs(temp.value));
                Z_temp = Angle(M_PI_2 + M_PI * (1.0 + GSL_SIGN(temp.value)) / 2.0);
            }else{
                lambda_span_temp.set(2.0*M_PI - fabs(temp.value));
                Z_temp = Angle(-(M_PI_2 + M_PI * (1.0 + GSL_SIGN(temp.value)) / 2.0));
            }


            
            //bottom horizontal edge of rectangle
            (Route(
                   RouteType(((Route_types[0]).value)),
                   (parent->parent->geo_position_start),
                   Z_temp,
                   Length(Re * cos(parent->parent->geo_position_start.phi) * (lambda_span_temp.value))
                   )
             ).DrawOld(wxGetApp().n_points_routes.value, &dc, this, String(""));
            
            //            //top horizontal edge of rectangle
            (Route(
                   RouteType(((Route_types[0]).value)),
                geo_position,
                Z_temp+M_PI,
                Length(Re * cos(geo_position.phi) * (lambda_span_temp.value))
            )).DrawOld(wxGetApp().n_points_routes.value, &dc, this, String(""));

            
            break;
        }
            
            
    }
    
    //render the labels of the selection rectangle
    //wipe out the space occupied by the label
    dc.SetPen(wxPen(background_color));
    dc.SetBrush(wxBrush(background_color, wxBRUSHSTYLE_SOLID));
    dc.DrawRectangle(position_end_label, get_size(end_label, &dc));

    //render the rectangle
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawText(wxString(end_label.value), position_end_label);
    dc.DrawText(wxString(parent->parent->start_label_selection_rectangle.value), position_start_label_selection_rectangle);


}


////draw the label of the start and end point of selection_rectangle with foreground and background colrs foreground_color and background_color, respectively
//void DrawPanel::RenderSelectionRectangleLabels(wxDC& dc) {
//
//
//}


inline void DrawPanel::RenderAll(wxDC& dc) {
    
    
    
    (this->*Render)(
                    &dc,
                    position_plot_area_now,
                    grid_now,
                    ticks_now,
                    parallels_and_meridians_labels_now,
                    positions_parallels_and_meridians_labels_now,
                    parent->polygon_position_now,
                    parent->coastline_polygons_now,
                    wxGetApp().foreground_color,
                    wxGetApp().background_color,
                    wxGetApp().standard_thickness.value
                    );
    
    
    RenderRoutes(dc,
                 points_route_list_now,
                 reference_positions_route_list_now,
                 (parent->parent->highlighted_route_now),
                 wxNullColour
                 );
    RenderPositions(dc,
                    points_position_list_now,
                    (parent->parent->highlighted_position_now),
                    wxNullColour
                    );
    RenderMousePositionLabel(
                             dc,
                             label_position_now,
                             position_label_position_now,
                             wxGetApp().foreground_color,
                             wxGetApp().background_color
                             );
    
    //render selection_rectangle and its labels
    if ((parent->parent->selection_rectangle)) {
        RenderSelectionRectangle(dc, parent->parent->geo_position_now, position_end_label_selection_rectangle_now, parent->parent->end_label_selection_rectangle_now, wxGetApp().foreground_color, wxGetApp().background_color);
    }
    
    if ((parent->parent->dragging_object)) {
        //I am draggingn a Route or Position -> show the coordinates of the Position or of the Route's reference_position
        RenderDraggedObjectLabel(dc, position_label_dragged_object_now, label_dragged_object_now, wxGetApp().foreground_color, wxGetApp().background_color);
    }
    
}



//render the Routes whose point coordinates with respect to the origin of DrawPanel are stored in points_curves, and whose reference-position coordinates with respect to the origin of DrawPanel are stored in reference_positions. the Route #highlighted_route is rendered with larger thickness. If foreground_color != wxNUllColour, the Routes are rendered with the colors in color_list, otherwise they are rendered with foreground_color
inline void DrawPanel::RenderRoutes(
                                    wxDC& dc,
                                    const vector< vector< vector<wxPoint> > >& points_curves,
                                    const vector<wxPoint>& reference_positions,
                                    int highlighted_route,
                                    wxColor foreground_color
                                    ) {

    int i, j, color_id;
    double thickness, radius;
    wxPoint p;

    //render Routes
    for (i = 0, color_id = 0; i < (points_curves.size()); i++) {

        //set the route thickness and pen
        if (i == highlighted_route) {
            thickness = max((int)((((wxGetApp().large_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1);
            radius = thickness;
        }
        else {
            thickness = (wxGetApp().standard_thickness.value);
            radius = 4 * thickness;
        }

        if (foreground_color != wxNullColour) {
            dc.SetPen(wxPen(foreground_color, thickness));
            //dc.SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_SOLID));
        }
        else {
            dc.SetPen(wxPen((wxGetApp().color_list)[(color_id++) % ((wxGetApp().color_list).size())], thickness));
        }

        //draw  reference_position[i] only if it is included in the plot area
        if (DrawPanelToGeo(reference_positions[i], NULL)) {
            dc.DrawCircle(reference_positions[i], radius);
        }

        //draw the route points
        //run over all connected chunks of routes
        for (j = 0; j < (points_curves[i]).size(); j++) {

            if ((points_curves[i][j]).size() > 1) {
                //I need to add this consdition to make sure that I am not drawing an empty connected chunk

                
                dc.DrawSpline((int)((points_curves[i][j]).size()), (points_curves[i][j]).data());
            }
            
            //render points of Routes for debug
            /*
             for(unsigned int l=0; l<points_curves[i][j].size(); l++){
             dc.DrawCircle(points_curves[i][j][l], thickness);
             }
             */
            //render points of Routes for debug
            

        }

    }

}

//clear everything and re-render all GUI objects
inline void DrawPanel::CleanAndRenderAll(void) {
    
    wxClientDC dc(this);
    
    
    dc.Clear();
    
    RenderMousePositionLabel(
                             dc,
                             label_position_now,
                             position_label_position_now,
                             wxGetApp().foreground_color,
                             wxGetApp().background_color
                             );
    
    (this->*Render)(
                    &dc,
                    position_plot_area_now,
                    grid_now,
                    ticks_now,
                    parallels_and_meridians_labels_now,
                    positions_parallels_and_meridians_labels_now,
                    parent->polygon_position_now,
                    parent->coastline_polygons_now,
                    wxGetApp().foreground_color,
                    wxGetApp().background_color,
                    wxGetApp().standard_thickness.value
                    );
    
    RenderRoutes(dc,
                 points_route_list_now,
                 reference_positions_route_list_now,
                 (parent->parent->highlighted_route_now), wxNullColour
                 );
    
    RenderPositions(dc,
                    points_position_list_now,
                    (parent->parent->highlighted_position_now),
                    wxNullColour
                    );
    
    RenderDraggedObjectLabel(dc,
                             position_label_dragged_object_now,
                             label_dragged_object_now,
                             wxGetApp().foreground_color,
                             wxGetApp().background_color
                             );
    
    
}

//wipe out all Routes on *this and re-draw them: this method is used to replace on WIN32 the wxWidgets default function Refresh(), which is not efficient on WIN32
inline void DrawPanel::RefreshWIN32(void) {

    wxClientDC dc(this);

    //1. erase _before objects

    if ((parent->parent->mouse_moving)) {
        //the mouse is moving -> wipe out the  mouse position label at the preceeding step of mouse movement

        RenderMousePositionLabel(
            dc,
            label_position_before,
            position_label_position_now,
            wxGetApp().background_color,
            wxGetApp().background_color
        );

    }

    if ((parent->dragging_chart) || (parent->mouse_scrolling)) {
        //the whole chart is being dragged or scrolled -> wipe out all objects at the preceeding step of the drag

        //wipe out the Routes at the preceeding mouse position
        RenderRoutes(dc,
            points_route_list_before,
            reference_positions_route_list_before,
            (parent->parent->highlighted_route_now),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_before,
            (parent->parent->highlighted_position_now),
                        wxGetApp().background_color
                        );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_before,
                        grid_before,
                        ticks_before,
                        parallels_and_meridians_labels_before,
                        positions_parallels_and_meridians_labels_before,
                        parent->polygon_position_before,
                        parent->coastline_polygons_before,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
    }

    if ((parent->parent->dragging_object)) {

        //wipe out the Routes, Positions and label of dragged object at the preceeding dragging configuration
        RenderRoutes(dc,
            points_route_list_before,
            reference_positions_route_list_before,
            (parent->parent->highlighted_route_now),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_before,
            (parent->parent->highlighted_position_now),
                        wxGetApp().background_color
                        );
        RenderDraggedObjectLabel(dc,
                                 position_label_dragged_object_before,
                                 label_dragged_object_before,
                                 wxGetApp().background_color, wxGetApp().background_color
                                 );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
        
    }

    if ((parent->parent->changing_highlighted_object)) {

        //wipe out the Routes at the preceeding mouse position
        RenderRoutes(dc,
            points_route_list_now,
            reference_positions_route_list_now,
            (parent->parent->highlighted_route_before),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_now,
            (parent->parent->highlighted_position_before),
            wxGetApp().background_color
        );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
        
    }
    
    if ((parent->parent->selection_rectangle)) {

        //wipe out the preceeding selection rectangle
        RenderSelectionRectangle(dc,
            (parent->parent->geo_position_before),
            position_end_label_selection_rectangle_before,
            parent->parent->end_label_selection_rectangle_before,
            wxGetApp().background_color,
            wxGetApp().background_color
        );

        //wipe out the Routes at the preceeding mouse position
        RenderRoutes(dc,
            points_route_list_now,
            reference_positions_route_list_now,
            (parent->parent->highlighted_route_now),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_now,
            (parent->parent->highlighted_position_now),
            wxGetApp().background_color
                        );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
        
    }


    //re-render  _new objects

    RenderMousePositionLabel(
        dc,
        label_position_now,
        position_label_position_now,
        wxGetApp().foreground_color,
        wxGetApp().background_color
    );

    if ((parent->dragging_chart) || (parent->mouse_scrolling) || (parent->parent->selection_rectangle) || (parent->parent->dragging_object) || (parent->parent->changing_highlighted_object)) {
        //I am either drawing a selection rectangle, dragging an object or changing the highlighted object -> I need to re-render all GUI objects
        
        //re-render all  objects in *this which may have been partially cancelled by the clean operation above
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().foreground_color,
                        wxGetApp().background_color,
                        wxGetApp().standard_thickness.value
                        );
        RenderRoutes(dc,
                     points_route_list_now,
                     reference_positions_route_list_now,
                     (parent->parent->highlighted_route_now), wxNullColour
        );
        RenderPositions(dc,
            points_position_list_now,
            (parent->parent->highlighted_position_now),
            wxNullColour
        );
        RenderDraggedObjectLabel(dc,
            position_label_dragged_object_now,
            label_dragged_object_now,
            wxGetApp().foreground_color,
            wxGetApp().background_color
        );



    }

    if ((parent->parent->selection_rectangle)) {

        //re-draw the current selection rectangle
        RenderSelectionRectangle(dc,
            parent->parent->geo_position_now,
            position_end_label_selection_rectangle_now,
            parent->parent->end_label_selection_rectangle_now,
            wxGetApp().foreground_color,
            wxGetApp().background_color
        );


    }

}



//render the Positions:  if foreground_color == wxNullColour, this method uses as foreground color the colors in color_list, otherwise it uses foreground_color
inline void DrawPanel::RenderPositions(wxDC& dc,
                                       const vector<wxPoint>& points,
                                       int highlighted_position,
                                       wxColor foreground_color) {

    int i, color_id;
    double thickness, radius;
    wxPoint p;


    //draw Positions
    for (i = 0, color_id = 0; i < (points.size()); i++) {

        //set thickness and pen
        if (i == highlighted_position) {
            thickness = max((int)((((wxGetApp().large_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1);
            radius = thickness;
        }
        else {
            thickness = (wxGetApp().standard_thickness.value);
            radius = 4 * thickness;
        }

        if (foreground_color != wxNullColour) {
            dc.SetPen(wxPen(foreground_color, thickness));
        }
        else {
            dc.SetPen(wxPen((wxGetApp().color_list)[(color_id++) % ((wxGetApp().color_list).size())], thickness));
        }

        if (DrawPanelToGeo(points[i], NULL)) {
            //the point returned from GeoToDrawPanel falls within the plot area -> plot it
            dc.DrawCircle(points[i], radius);
        }

    }

}


//render the coordinates of an object (Route or Position) which is being dragged by rendering the label label_dragged_object at position position_label_dragged_object (reckoned with respect to the origin of *this)
inline void DrawPanel::RenderDraggedObjectLabel(wxDC& dc,
                                                const wxPoint& position_label_dragged_object,
                                                const String& label_dragged_object,
                                                wxColor foreground_color,
                                                wxColor background_color) {


    //wipe out the space occupied by the label
    dc.SetPen(wxPen(background_color));
    dc.SetBrush(wxBrush(background_color, wxBRUSHSTYLE_SOLID));
    dc.DrawRectangle(position_label_dragged_object, get_size(label_dragged_object, &dc));


    //render label_dragged_object
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawText(wxString(label_dragged_object.value), position_label_dragged_object);

}





//fit the size of the chart, of parent, of parent->panel to the content
void DrawPanel::FitAll() {

    //set the size of the DrawPanel and of the ChartFrame which is its parent and fit the size of ChartFrame parent in such a way that it just fits its content
    this->SetMinSize(size_chart);
    parent->SetMinSize(ToDIP(wxSize(
        (size_chart.GetWidth()) + ((parent->slider)->GetSize().GetWidth()) + 4 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
        (size_chart.GetHeight()) + ((label_position_now.get_size(this)).GetHeight()) + 6 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)
    )));

    //position position_label_position_now at the bottom left corner of *this
    position_label_position_now = wxPoint(
        (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
        (size_chart.GetHeight())
        - (size_label_vertical + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))
    );

    parent->panel->SetSizerAndFit(parent->sizer_v);
    parent->panel->Fit();
    parent->Fit();

}


//render the polygons stored in points_polygons and polygon_positions
inline void DrawPanel::RenderPolygons(wxDC* dc,
                                      const vector<unsigned long long int>& polygon_positions,
                                      const vector<wxPoint>& points_polygons,
                                      wxColor foreground_color,
                                      wxColor background_color,
                                      double thickness) {
    
    long long int i;
    
    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_SOLID));
    for(i = 0; i < ((long long int)(polygon_positions.size()))-1; i++) {
        //run through polygons
        
        if(polygon_positions[i+1] - polygon_positions[i] > 1){
            
            dc->DrawLines((int)(polygon_positions[i+1] - polygon_positions[i]), (points_polygons.data()) + polygon_positions[i]);
            
        }
        
    }
    
}


//remember that any Draw command in this function takes as coordinates the coordinates relative to the position of the DrawPanel object!
inline void DrawPanel::Render_Mercator(wxDC* dc,
                                       const wxPoint& position_plot_area,
                                       const vector< vector< vector<wxPoint> > >& grid,
                                       const vector< vector< vector<wxPoint> > >& ticks,
                                       const vector<wxString>& parallels_and_meridians_labels,
                                       const vector<wxPoint>& positions_parallels_and_meridians_labels,
                                       const vector<unsigned long long int>& polygon_positions,
                                       const vector<wxPoint>& points_polygons,
                                       wxColor foreground_color,
                                       wxColor background_color,
                                       double thickness) {

    Angle lambda, phi;
    Route route;
    wxPoint p;
    PositionProjection temp;
    Position q;
    //this = true if, while drawing the x or y axis labels, the label that I one is about to draw is the first one
    int i, j;

    //draw a rectangle (representing the border) whose border and fill are with color wxGetApp().background_color on bitmap_image, so it will have the right background color
    dc->SetBrush(wxBrush(background_color, wxBRUSHSTYLE_TRANSPARENT));
    dc->SetPen(wxPen(foreground_color, thickness));
    //dc->DrawRectangle(0, 0, (size_chart.GetWidth()), (size_chart.GetHeight()));
    dc->DrawRectangle(position_plot_area.x, position_plot_area.y, (size_plot_area.GetWidth()), (size_plot_area.GetHeight()));

    //render coastlines
    RenderPolygons(dc, polygon_positions, points_polygons, foreground_color, background_color, thickness);


    //set thickness to normal thicnkness
//    thickness = max((int)((((wxGetApp().standard_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1);

    //render parallels and meridians
    for (i = 0; i < grid.size(); i++) {
        for (j = 0; j < (grid[i]).size(); j++) {

            //            (grid[i]).DrawOld((parent->parent->data->n_points_routes.value), foreground_color, thickness, dc, this);

            if ((grid[i][j]).size() > 1) {
                dc->DrawLines((int)((grid[i][j]).size()), (grid[i][j]).data());
            }

        }
    }
    //render parallels and meridian ticks
    for (i = 0; i < ticks.size(); i++) {
        for (j = 0; j < (ticks[i]).size(); j++) {

            //            (ticks[i]).DrawOld((wxGetApp().n_points_minor_ticks.value), foreground_color, thickness, dc, this);
            if ((ticks[i][j]).size() > 1) {
                dc->DrawLines((int)((ticks[i][j]).size()), (ticks[i][j]).data());
            }
        }
    }



    //render labels on parallels and meridians
    //WIN32: THESE LINES YIELD PARALLEL AND MERIDIAN LABELS WITH THE CORRECT SIZE
    dc->SetTextForeground(foreground_color);
    dc->SetTextBackground(background_color);
    dc->SetBrush(wxBrush(wxNullBrush)); //Set the brush to the device context
    dc->SetBackgroundMode(wxSOLID);
    for (i = 0; i < parallels_and_meridians_labels.size(); i++) {

        dc->DrawText(parallels_and_meridians_labels[i], positions_parallels_and_meridians_labels[i] /*+ wxPoint(-width_label - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value), -height_label / 2)*/);

    }
    //WIN32: THESE LINES YIELD PARALLEL AND MERIDIAN LABELS WITH THE CORRECT SIZE


}


//This function writes into *output the text label for a parallel or a meridian. The latitude/longitude in the text label is q.phi/q.lambda, min and max are the minimal and maximal latitudes/longitudes that are covered in the drawing process of the label by DrawPanel::SetLabel, they must be sorted in such a way that (max.normalize_pm_pi_ret()).value > (min.normalize_pm_pi_ret()).value. mode = "NS" or "EW" specifices whether the label to be plotted is a latitude or a longitude label, respectively. The output is written int *output
void DrawPanel::WriteLabel(const Position& q, Angle min, Angle max, Int precision, String mode, wxString* output) {

    double delta;
    //a pointer to the angle which will be used to draw the label
    Angle angle_label;
    stringstream s;
    wxString wx_string;

    s.str("");

    if (mode == String("NS")) {
        //if I am drawing latitude labels, I set the angle relative to the label to q.phi, and delta to delta_phi, and I let labels point to label_phi

        angle_label = (q.phi);
        delta = delta_phi;

    }
    else {
        //if I am drawing longitude labels, I set the angle relative to the label to q.lambda, and delta to delta_lambda, and I let labels point to label_lambda

        angle_label = (q.lambda);
        delta = delta_lambda;

    }


    if (/*If this condition is true, then angle_label.value*K is an integer multiple of one degree*/fabs(K * (angle_label.value) - round(K * (angle_label.value))) < epsilon_double) {
        //in this case, (angle_label.value) (or, in other words, the latitude phi) = n degrees, with n integer: I write on the axis the value of phi  in degrees

        s << angle_label.deg_to_string(mode, (precision.value));

    }
    else {

        //in this case, delta  is not an integer multiple of a degree. However, (angle_label.value) may still be or not be a multiple integer of a degree
        if (k * fabs(K * (angle_label.value) - ((double)round(K * (angle_label.value)))) < delta / 2.0) {
            //in this case, (angle_label.value) coincides with an integer mulitple of a degree: I print out its arcdegree part only

            s << angle_label.deg_to_string(mode, (precision.value));

        }
        else {
            //in this case, (angle_label.value) deos not coincide with an integer mulitple of a degree: I print out its arcminute part only

            //                if(ceil((K*((*(*(parent->phi_max))).value)))  - floor((K*((*(*(parent->phi_min))).value))) != 1){
            if (ceil((K * ((max.normalize_pm_pi_ret()).value))) - floor((K * ((min.normalize_pm_pi_ret()).value))) != 1) {
                //in this case, the phi interval which is plotted spans more than a degree: there will already be at least one tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I print out its arcminute part only.

                s << angle_label.min_to_string(mode, (precision.value));

            }
            else {
                //in this case, the phi interval which is plotted spans less than a degree: there will be no tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I add this tic by printing, at the first tic, both the arcdegrees and arcminutes.

                if (first_label) {

                    s << angle_label.to_string(mode, (precision.value), false);

                }
                else {

                    s << angle_label.min_to_string(mode, (precision.value));

                }

            }


        }

    }

    (*output) = wxString(s.str().c_str());


}

//This function draws into *this the text label for a parallel or a meridian, by placing it near the Position q. The label is pushed back to this->labels and its position is pushed back to this->positions_labels (this position is adjusted with respect to q in such a way that the label look nice and centered). The latitude/longitude in the text label is q.phi/q.lambda. min and max are the minimal and maximal latitudes/longitudes that are covered in the drawing process, they must be sorted in such a way that (max.normalize_pm_pi_ret()).value > (min.normalize_pm_pi_ret()).value. mode = "NS" or "EW" specifices whether the label to be plotted is a parallel or a meridian label, respectively.
void DrawPanel::DrawLabel(const Position& q, Angle min, Angle max, Int precision, String mode) {

    wxPoint p;
    wxSize size;

    if (/* convert temp to draw_panel coordinates p*/GeoToDrawPanel(q, &p, false)) {
        //if Position q lies on the visible side of the Earth, I proceed and draw its label

        wxString wx_string;

        //write the label into wx_string
        WriteLabel(q, min, max, precision, mode, &wx_string);


        parallels_and_meridians_labels_now.push_back(wx_string);
        positions_parallels_and_meridians_labels_now.push_back(p);


        size = String(parallels_and_meridians_labels_now.back().ToStdString()).get_size(this);

        if (mode == String("NS")) {
            //            I am drawing parallels label

            (positions_parallels_and_meridians_labels_now.back()) += wxPoint(-(size.GetWidth()) - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value), -(size.GetHeight()) / 2);

        }
        else {
            //            I am drawing meridians labels

            (positions_parallels_and_meridians_labels_now.back()) += wxPoint(-(size.GetWidth()) / 2, (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value));

        }

        first_label = false;

    }

}

//This function renders the chart in the 3D case. remember that any Draw command in this function takes as coordinates the coordinates relative to the position of the DrawPanel object!
inline void DrawPanel::Render_3D(
                                 wxDC* dc,
                                 const wxPoint& position_plot_area,
                                 const vector< vector< vector<wxPoint> > >& grid,
                                 const vector< vector< vector<wxPoint> > >& ticks,
                                 const vector<wxString>& parallels_and_meridians_labels,
                                 const vector<wxPoint>& positions_parallels_and_meridians_labels,
                                 const vector<unsigned long long int> & polygon_positions,
                                 const vector<wxPoint>& points_polygons,
                                 wxColor foreground_color,
                                 wxColor background_color,
                                 double thickness
                                 ) {
    
    int i, j;
    Double d_temp;
    Angle lambda;
    stringstream s;
    wxString wx_string;
    //this is a list of tabulated points for dummy_route, such as a meridian, which will be created and destroyed just after
    vector<wxPoint> points_dummy_route;
    PositionProjection dummy_projection;
    wxPoint p;
    Position q, temp;


    //draws a rectangle filled with color wxGetApp().background_color and with border wich color wxGetApp().foregrond_color on bitmap_image, so bitmap_image will have the right background color
    //dc->SetBrush(wxBrush(wxGetApp().background_color));
    //dc->SetPen(wxPen(foreground_color, thickness));
    //dc->DrawRectangle(0, 0, (size_chart.GetWidth()), (size_chart.GetHeight()));

    //render coastlines
    if((parent->parent->show_coastlines) == Answer('y', String(""))){
        RenderPolygons(dc, polygon_positions, points_polygons, foreground_color, background_color, thickness);
    }

    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_TRANSPARENT)); //Set the brush to the device context
    //render parallels and meridians
    for (i = 0; i < grid.size(); i++) {
        for (j = 0; j < (grid[i]).size(); j++) {

            //        (grid[i]).Draw((parent->parent->data->n_points_routes.value), foreground_color, background_color, thickness, dc, this, String(""));
            if ((grid[i][j]).size() > 1) {
                dc->DrawSpline((int)((grid[i][j]).size()), (grid[i][j]).data());
            }
        }
    }
    //render parallel and meridian ticks
    for (i = 0; i < ticks.size(); i++) {
        for (j = 0; j < (ticks[i]).size(); j++) {

            //        (ticks[i]).Draw((wxGetApp().n_points_minor_ticks.value), foreground_color, background_color, thickness, dc, this, String(""));
            if ((ticks[i][j]).size() > 1) {
                dc->DrawSpline((int)((ticks[i][j]).size()), (ticks[i][j]).data());
            }
        }
    }


    //render labels on parallels and meridians
    dc->SetTextForeground(foreground_color);
    dc->SetTextBackground(background_color);
    dc->SetBrush(wxBrush(wxNullBrush)); //Set the brush to the device context
    dc->SetBackgroundMode(wxSOLID);
    for (i = 0; i < parallels_and_meridians_labels.size(); i++) {

        dc->DrawText(parallels_and_meridians_labels[i], positions_parallels_and_meridians_labels[i]/* + wxPoint(-width_label - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value), -height_label / 2)*/);

    }



    //draw horizon circle
    //draw the circle repreentig the edge of the earth by creating a circle of equal altitude centered at GP_observer and with aperture omega_observer
    //set q to a point on the prime meridian and latitude equal to the maximal latitude of circle_observer, and convert it to 3D projection temp: the resulting temp.y is the radius of the circular horizon of the earth in 3d projection cooordinates
    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_TRANSPARENT)); //Set the brush to the device context

    //set q
    q.lambda.set(0.0);
    (q.phi) = (circle_observer.omega);

    //obtain the coordinates of q in the reference frame x'y'z'
    gsl_vector_set((rp.r), 0, 0.0);
    gsl_vector_set((rp.r), 1, -cos(q.phi));
    gsl_vector_set((rp.r), 2, sin((q.phi)));

    //project rp into the 3D projection and obtain temp: temp.y is the radius of the horizon circle
    d_temp.set(-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))));
    dummy_projection = PositionProjection(0.0, ((d_temp.value) * gsl_vector_get((rp.r), 2)) / ((d_temp.value) + 1.0 + gsl_vector_get((rp.r), 1)));
    //set the wxPen color for the horizon
//    dc->SetPen(wxPen(wxGetApp().color_horizon, 1));
    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(background_color, wxBRUSHSTYLE_TRANSPARENT));
    dc->SetBackground(background_color);
    //convert r.y to DrawPanel coordinates and trace a circle with the resulting radius
    dc->DrawCircle(
        (position_plot_area.x) + (int)(((double)(size_plot_area.GetWidth())) / 2.0),
        (position_plot_area.y) + (int)(((double)(size_plot_area.GetHeight())) / 2.0),
        (dummy_projection.y) / y_max * ((double)(size_plot_area.GetWidth())) / 2.0
    );

}




//this function tabulates into points_route_list_now and reference_positions_route_list_now the points and reference Positions, respectively, of all Routes. points_route_list will then be used to render the Routes
inline void DrawPanel::TabulateRoutes(void) {

    unsigned int i;
    wxPoint p;

    //resize points_route_list_now and reference_position_route_list_now, which needs to have the same size as (data->route_list), and clear up points_route_list
    points_route_list_now.resize((parent->parent->data->route_list).size());
    for (i = 0; i < (points_route_list_now.size()); i++) {
        (points_route_list_now[i]).clear();
    }

    reference_positions_route_list_now.clear();
    reference_positions_route_list_now.resize((parent->parent->data->route_list.size()));

    //tabulate the points of routes
    for (i = 0; i < parent->parent->data->route_list.size(); i++) {

        //write the points of the curves corresponding to the Routes into points_route_list_now
        //change this at the end, when you will have a function Draw that handles loxodromes. Then, you will use only the first case of this if
        if (((parent->parent->data->route_list)[i]).type != (Route_types[0])) {

            ((parent->parent->data->route_list)[i]).Draw((unsigned int)(wxGetApp().n_points_routes.value), this, (points_route_list_now.data()) + i, String(""));

        }
        else {

            ((parent->parent->data->route_list)[i]).DrawOld((unsigned int)(wxGetApp().n_points_routes.value), this, (points_route_list_now.data()) + i, String(""));

        }

        //write the reference Positions into reference_positions_route_list_now
        if (GeoToDrawPanel((*(((parent->parent->data->route_list)[i]).reference_position)), &p, false)) {
            //the reference position falls in the plot area -> write it into reference_positions_route_list_now
            reference_positions_route_list_now[i] = p;
        }
        else {
            //the reference position does not fall in the plot area -> write a 'Null' value into reference_positions_route_list_now which will be ignored in other methods because it lies outside the plot area
            reference_positions_route_list_now[i] = wxPoint(0, 0);
        }

    }

}



//tabulate into points_position_list_now all the Positions. points_position_list will then be used to render the Positions
inline void DrawPanel::TabulatePositions(void) {

    unsigned int i;
    wxPoint p;

    //resize points_position_list_now and, which needs to have the same size as (data->position_list)
    points_position_list_now.clear();
    points_position_list_now.resize((parent->parent->data->position_list).size());

    //tabulate the points of Positions
    for (i = 0; i < (parent->parent->data->position_list.size()); i++) {

        //write the reference Positions into reference_positions_route_list_now
        if (GeoToDrawPanel((parent->parent->data->position_list)[i], &p, false)) {
            //the  position falls in the plot area -> write it into points_position_list_now
            points_position_list_now[i] = p;
        }
        else {
            //the  position does not fall in the plot area -> write a 'Null' value into points_position_list_now which will be ignored in other methods because it lies outside the plot area
            points_position_list_now[i] = wxPoint(0, 0);
        }

    }

}



//draws coastlines, Routes and Positions on the Mercator-projection case
inline void DrawPanel::PreRenderMercator(void) {

    PositionProjection delta_temp;
    unsigned int n_intervals_ticks, n_intervals_ticks_max;
    Position q;
    String prefix, new_prefix;
    wxPoint p;
    wxString dummy_label;
    Route route;
    Angle phi;

    //append \t to prefix
    prefix = String("");
    new_prefix = prefix.append(String("\t"));

    //client_dc->Clear();


    //here I compute multiple quantities relative to the y axis: this computation is done here, at the very beginning of PreRenderMercator, because these quantitites will be needed immediatly to compute size_label_horizontal
    //set phi_start, phi_end and delta_phi
    phi_span = (parent->phi_max->normalize_pm_pi_ret().value) - (parent->phi_min->normalize_pm_pi_ret().value);

    //gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (phi_span > k) {
        //in this case, phi_span is larger than one degree
        gamma_phi = 1;
        delta_phi_minor = -1.0;
    }
    else {
        if (phi_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > phi_span > 10 arcminutes
            gamma_phi = 60;
            delta_phi_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > phi_span
            gamma_phi = 60 * 10;
            delta_phi_minor = tenth_arcmin_radians;
        }
    }

    delta_phi = k / ((double)gamma_phi);
    while (((wxGetApp().n_intervals_ticks_preferred).value) * delta_phi < phi_span) {
        if (delta_phi == k / ((double)gamma_phi)) { delta_phi += k * 4.0 / ((double)gamma_phi); }
        else { delta_phi += k * 5.0 / ((double)gamma_phi); }
    }

    //here I set up things to plot meridians and parallels in Render_Mercator

    //set phi_start/end
    (phi_start.value) = floor((parent->phi_min->normalize_pm_pi_ret().value) / delta_phi) * delta_phi;
    (phi_end.value) = (parent->phi_max->normalize_pm_pi_ret().value);


    //compute size of largest label on parallel: run through all labels on parallels and set size_label_horizontal as the size of the largest label on parallel that has ben found
    for (size_label_horizontal = 0,
        first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminuted) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        ((q.phi).value) = (phi_start.value),
        (q.lambda) = (*(parent->lambda_min)) - epsilon_double;
        ((q.phi).value) < (phi_end.value);
        ((q.phi).value) += delta_phi
        ) {

        WriteLabel(q, (*(parent->phi_min)), (*(parent->phi_max)), label_precision, String("NS"), &dummy_label);

        if ((GetTextExtent(dummy_label).GetWidth()) > ((int)size_label_horizontal)) {
            size_label_horizontal = (GetTextExtent(dummy_label).GetWidth());
        }

    }

    //take the angle 0° 0.0' expresed with display_precision: the height of this angle label is the largest possible -> set it equal to size_label_vertical
    size_label_vertical = (GetTextExtent(wxString((Angle(0, 0.0).to_string(String("NS"), (display_precision.value), false)))).GetHeight());


    //set x_min, ..., y_max for the following
    (this->*Set_x_y_min_max)();

    //set rectangle_obseerver
    rectangle_observer = PositionRectangle(Position((*(parent->lambda_min)), (*(parent->phi_max))), Position((*(parent->lambda_max)), (*(parent->phi_min))), String(""));

    /*set the aspect ratio between height and width equal to the ratio between the y and x range: in this way, the aspect ratio of the plot is equal to 1*/

    if ((!(parent->dragging_chart)) && (!(parent->mouse_scrolling))) {
        //the ChartFrame is not being dragged and the mouse is not scrolling -> the chart's size will change -> re-compute its size

        if ((y_max - y_min) > x_span()) {
            //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
            parent->SetSize(
                (((wxGetApp().rectangle_display).GetSize()).GetHeight()) / ((y_max - y_min) / x_span()),
                (((wxGetApp().rectangle_display).GetSize()).GetHeight())
            );

        }
        else {
            //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
            parent->SetSize(
                (((wxGetApp().rectangle_display).GetSize()).GetHeight()),
                (((wxGetApp().rectangle_display).GetSize()).GetHeight()) * ((y_max - y_min) / x_span())
            );
        }
    }

    (this->*Set_size_chart)();
    //set the size of *this equal to the size of the chart, in such a way that draw_panel can properly contain the chart
    SetSize(size_chart);

    //I am about to modify position_plot_area_now -> save it in position_plot_area_before
    position_plot_area_before = position_plot_area_now;

    //sets size_plot_area and stores into position_plot_area the screen position of the top-left edge of the plot area.
    if (
        ((size_chart.GetWidth()) - (((int)size_label_horizontal) + 3 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))) * (size_chart.GetHeight()) / (size_chart.GetWidth())
        < (size_chart.GetHeight()) - (((int)size_label_vertical) + 3 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))
        ) {
        //if I set size_plot_area's width first to leave room for parallel labels and label_position_now, then there is enough space to set size_plot_area's height by keeping the aspect ratio

        size_plot_area.SetWidth(
            (size_chart.GetWidth())
            //space for  parallel labels
            - (((int)size_label_horizontal) + 3 * ((wxGetApp().border).value))
            //space for label_position_now
            - ((((int)size_label_vertical) + ((wxGetApp().border).value))) * (size_chart.GetWidth()) / (size_chart.GetHeight())
        );
        size_plot_area.SetHeight((size_plot_area.GetWidth()) * (size_chart.GetHeight()) / (size_chart.GetWidth()));

        position_plot_area_now = wxPoint(
            ((int)size_label_horizontal) + 2 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
            (((int)(size_chart.GetHeight())) - (((int)(size_plot_area.GetHeight())) + ((int)size_label_vertical) + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))) / 2
        );

    }
    else {
        //if I set size_plot_area's width first to leave room for  parallel labels and label_position_now and there is not enough space to set size_plot_area's height by keeping the aspect ratio -> I set size_plot_area's height first, by leaving space in the resulting height for meridian labels and label_position_now , and set the width later according to the aspect ratio

        size_plot_area.SetHeight(
            (size_chart.GetHeight())
            //space for meridian labels
            - (((int)size_label_vertical) + 3 * ((wxGetApp().border).value))
            //space for label_position_now
            - (((int)size_label_vertical) + ((wxGetApp().border).value))
        );
        size_plot_area.SetWidth((size_plot_area.GetHeight()) * (size_chart.GetWidth()) / (size_chart.GetHeight()));

        if (((size_plot_area.GetHeight()) * (size_chart.GetWidth()) / (size_chart.GetHeight())) < ((size_chart.GetWidth()) - (((int)size_label_horizontal) + 3 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)))) {
            //good: there is enough space

        }
        else {
            //bad: there is not enough space

        }

        position_plot_area_now = wxPoint(

            (((int)(size_chart.GetWidth())) - (((int)(size_plot_area.GetWidth())) - ((int)size_label_horizontal) - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))) / 2


            ,

            (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)

        );

    }
    
    tick_length = (((wxGetApp().tick_length_over_width_plot_area)).value) * (size_plot_area.GetWidth());

    //set p_NW and p_SE
    //updates the position of the draw pane this
//    DrawPanelToGeo(wxPoint(position_plot_area_now) /*I move the NW boundary of the plot area to the interior by one pixel*/ + wxPoint(1, 1), &p_NW);
//    DrawPanelToGeo(wxPoint(position_plot_area_now + size_plot_area) /*I move the SE boundary of the plot area to the interior by one pixel*/ - wxPoint(1, 1), &p_SE);

    //fetch the data on the region that I am about to plot from the data files and store it into parent->coastline_polygons_now
    parent->GetCoastLineData_Mercator();

    //the number of ticks is given by the minimum between the preferred value and the value allowed by fitting the (maximum) size of each axis label into the witdh of the axis
    n_intervals_ticks_max = ((unsigned int)floor(((double)(size_plot_area.GetWidth())) / ((double)size_label_horizontal)));
    n_intervals_ticks = min(
        (unsigned int)(wxGetApp().n_intervals_ticks_preferred.value),
        n_intervals_ticks_max
    );



    //set lambda_span
    if ((((*(parent->lambda_min)) < M_PI) && ((*(parent->lambda_max)) > M_PI)) || ((*(parent->lambda_min)) < (*(parent->lambda_max)))) {
        //the 'anomalous' situation where the chart encompasses the Greenwich antimeridian

        lambda_span = (parent->lambda_min->value) - (parent->lambda_max->value) + 2.0 * M_PI;

    }
    else {
        //the 'normal' situation where the chart does not encompass the Greenwich antimeridian

        lambda_span = (parent->lambda_min->value) - (parent->lambda_max->value);

    }

    //gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (lambda_span > k) {
        //in this case, lambda_span is larger than one degree
        gamma_lambda = 1;
        delta_lambda_minor = -1.0;
    }
    else {
        if (lambda_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > lambda_span > 10 arcminutes
            gamma_lambda = 60;
            delta_lambda_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > lambda_span
            gamma_lambda = 60 * 10;
            delta_lambda_minor = tenth_arcmin_radians;
        }
    }

    delta_lambda = k / ((double)gamma_lambda);
    while (n_intervals_ticks * delta_lambda < lambda_span) {
        if (delta_lambda == k / ((double)gamma_lambda)) { delta_lambda += k * 4.0 / ((double)gamma_lambda); }
        else { delta_lambda += k * 5.0 / ((double)gamma_lambda); }
    }


    if (((*(parent->lambda_min)) < M_PI) && ((*(parent->lambda_max)) > M_PI)) {

        (lambda_start.value) = ceil(((*(parent->lambda_max)).value) / delta_lambda) * delta_lambda;
        (lambda_end.value) = ((*(parent->lambda_min)).value) + (2.0 * M_PI);

    }
    else {

        if ((*(parent->lambda_min)) > (*(parent->lambda_max))) {

            (lambda_start.value) = ceil(((*(parent->lambda_max)).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((*(parent->lambda_min)).value);

        }
        else {

            (lambda_start.value) = ceil(((*(parent->lambda_max)).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((*(parent->lambda_min)).value) + 2.0 * M_PI;

        }

    }


    //compute labels on parallels and meridians
    //save parallels_and_meridians_labels_now and positions_parallels_and_meridians_labels_now into parallels_and_meridians_labels_before and  positions_parallels_and_meridians_labels_before, respectively.  clears all labels previously drawn
    parallels_and_meridians_labels_before = parallels_and_meridians_labels_now;
    positions_parallels_and_meridians_labels_before = positions_parallels_and_meridians_labels_now;
    parallels_and_meridians_labels_now.resize(0);
    positions_parallels_and_meridians_labels_now.resize(0);

    //compute labels on parallels
    for (first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminuted) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        ((q.phi).value) = (phi_start.value),
        (q.lambda) = (*(parent->lambda_min)) - epsilon_double;
        ((q.phi).value) < (phi_end.value);
        ((q.phi).value) += delta_phi
        ) {

        DrawLabel(q, (*(parent->phi_min)), (*(parent->phi_max)), label_precision, String("NS"));

    }

    //compute labels on meridians
    for (first_label = true,
        //set the label precision: if gamma_lambda = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_lambda*K*60 (the spacing between labels in arcminutes) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_lambda == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_lambda * K * 60)))),
        (q.lambda.value) = (lambda_start.value),
        (q.phi) = (*(parent->phi_min)) + epsilon_double;
        (q.lambda.value) < (lambda_end.value);
        (q.lambda.value) += delta_lambda
        ) {

        DrawLabel(q, (*(parent->lambda_max)), (*(parent->lambda_min)), label_precision, String("EW"));

    }

    grid_now.clear();
    ticks_now.clear();

    //prerender meridians
    //set route equal to a meridian going through lambda: I set everything except for the longitude of the ground posision, which will vary in the loop befor and will be fixed inside the loop
    route.type.set(String(((Route_types[1]).value)));
    route.Z.set(0.0);
    (route.reference_position->phi) = (*(parent->phi_min));

    //draw the first chunk of intermediate ticks on the longitude axis
    if (gamma_lambda != 1) {

        route.length.set(Re * (wxGetApp().tick_length_over_width_plot_area.value) * phi_span, LengthUnit_types[0]);

        //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
        for ((route.reference_position->lambda.value) = (lambda_start.value) - delta_lambda;
            (route.reference_position->lambda.value) - ((lambda_start.value) - delta_lambda) < delta_lambda;
            (route.reference_position->lambda.value) += delta_lambda_minor) {

            //            ticks_now.push_back(route);
            ticks_now.resize((ticks_now.size()) + 1);
            route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));

            //            route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));

        }

    }


    for (route.length.set(Re* ((parent->phi_max->normalize_pm_pi_ret().value) - (parent->phi_min->normalize_pm_pi_ret().value)), LengthUnit_types[0]),
        (route.reference_position->lambda.value) = (lambda_start.value);
        (route.reference_position->lambda.value) < (lambda_end.value);
        (route.reference_position->lambda.value) += delta_lambda) {

        //add the current meridian that is being drawn (route) to meridians_now
//        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.Draw((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //             route.Draw(((((parent->parent)->data)->n_points_routes).value), foreground_color, background_color, thickness, dc, this, String(""));

        if (gamma_lambda != 1) {
            //draw intermediate ticks on the longitude axis

            (lambda_saved.value) = (route.reference_position->lambda.value);
            route.length.set(Re * (((wxGetApp().tick_length_over_width_plot_area)).value) * phi_span, LengthUnit_types[0]);

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for ((route.reference_position->lambda.value) = (lambda_saved.value);
                (route.reference_position->lambda.value) - (lambda_saved.value) < delta_lambda;
                (route.reference_position->lambda.value) += delta_lambda_minor) {

                //                ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //                     route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));
            }

            route.length.set(Re * ((parent->phi_max->normalize_pm_pi_ret().value) - (parent->phi_min->normalize_pm_pi_ret().value)), LengthUnit_types[0]);
            (route.reference_position->lambda.value) = (lambda_saved.value);

        }

    }

    //prerender parallels
    //set route equal to a parallel of latitude phi, i.e., a circle of equal altitude
    route.type.set(String(((Route_types[0]).value)));
    route.Z.set(M_PI_2);
    (route.reference_position->lambda) = (*(parent->lambda_min));

    //this loop runs over the latitude of the parallel, which we call phi
    for ((phi.value) = (phi_start.value);
        (phi.value) < (phi_end.value);
        (phi.value) += delta_phi
        ) {

        //route.omega  and route.reference_position->phi of the circle of equal altitude are set for each value of phi as functions of phi, in such a way that route.omega is always smaller than pi/2
        (route.reference_position->phi) = phi;
        route.length.set(
            Re * cos(phi) * ((

                ((((*(parent->lambda_min))) < M_PI) && (((*(parent->lambda_max))) > M_PI)) ? (((*(parent->lambda_min))) - ((*(parent->lambda_max))) + 2.0 * M_PI) : (((*(parent->lambda_min))) - ((*(parent->lambda_max))))

                ).value), LengthUnit_types[0]);

        //add the current parallel that is being drawn to parallels
//        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.DrawOld((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //here I use DrawOld because Draw cannot handle loxodromes
//        route.DrawOld((parent->parent->data->n_points_routes.value), foreground_color, thickness, dc, this);

        if (gamma_phi != 1) {
            //draw smaller ticks -> set route to a loxodrome pointing towards the E and draw it

            route.length.set(Re * (wxGetApp().tick_length_over_width_plot_area.value) * lambda_span, LengthUnit_types[0]);

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for (
                (route.reference_position->phi.value) = (phi.value);
                (route.reference_position->phi.value) - (phi.value) < delta_phi;
                (route.reference_position->phi.value) += delta_phi_minor
                ) {

                //                    ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.DrawOld((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //here I use DrawOld because Draw cannot handle loxodromes
//                route.DrawOld(((wxGetApp().n_points_minor_ticks)).value, foreground_color, thickness, dc, this);

            }

        }

    }


    TabulateRoutes();
    TabulatePositions();

    //tell PaintEvent that everything but highligghteable objects (coastlines, meridians ... ) must be re-drawn
    re_draw = true;

    //    (parent->point_coastline_now).clear();


    //center the parent in the middle of the screen because the plot shape has changed and the plot may thus be misplaced on the screen
    //    parent->CenterOnScreen();


}

//this function draws coastlines, Routes and Positions in the 3D case
inline void DrawPanel::PreRender3D(void) {

    Angle lambda_in, lambda_out, /*phi is an auxiliary variable used in the loop which draws parallels*/phi;
    Position q;
    PositionProjection temp;
    wxPoint p;
    wxString dummy_label;
    Route route;
    unsigned int n_intervals_ticks;


    //set zoom_factor, the boundaries of x and y for the chart, and the latitudes and longitudes which comrpise circle_observer
    parent->zoom_factor.set((parent->parent->circle_observer_0.omega.value) / (circle_observer.omega.value));
    (this->*Set_x_y_min_max)();
    (this->*Set_lambda_phi_min_max)();

    parent->GetCoastLineData_3D();

    if ((!(parent->dragging_chart)) && (!(parent->mouse_scrolling))) {
        //I am not dragging the chart nor scrolling -> the size of the chart may change -> re-compute it
        parent->SetSize(
            (((wxGetApp().rectangle_display).GetSize()).GetHeight()),
            (((wxGetApp().rectangle_display).GetSize()).GetHeight())
        );
    }
    (this->*Set_size_chart)();
    SetSize(size_chart);

    size_plot_area.SetWidth((size_chart.GetWidth()) * (length_plot_area_over_length_chart.value));
    size_plot_area.SetHeight((size_chart.GetHeight()) * (length_plot_area_over_length_chart.value));

    position_plot_area_before = position_plot_area_now;
    position_plot_area_now = wxPoint((int)(((double)(size_chart.GetWidth())) * (1.0 - (length_plot_area_over_length_chart.value)) / 2.0),
        (int)(((double)(size_chart.GetHeight())) * (1.0 - (length_plot_area_over_length_chart.value)) / 2.0));

    //the number of ticks is given by the minimum between the preferred value and the value allowed by fitting the (maximum) size of each axis label into the witdh of the axis
    n_intervals_ticks = (unsigned int)(wxGetApp().n_intervals_ticks_preferred.value);


    //here I set up things to plot paralles and meridians in Render_3D

    //set lambda_span
    if (((*(parent->lambda_min)) == 0.0) && ((*(parent->lambda_max)) == 0.0)) {
        //in this case circle_observer spans all longitudes

        //because in this case lambda_min/max span the whole angle 2 pi and cannot define a range for lambda_span, I set
        lambda_span = 2.0 * ((circle_observer.omega).value);

    }
    else {
        //in this case, there are two finite longitudes which encircle circle_observer

        if (((*(parent->lambda_min)) < M_PI) && ((*(parent->lambda_max)) > M_PI)) {

            lambda_span = ((*(parent->lambda_min)).value) - ((*(parent->lambda_max)).value) + 2.0 * M_PI;

        }
        else {

            lambda_span = ((*(parent->lambda_min)).value) - ((*(parent->lambda_max)).value);

        }

    }


    //gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (lambda_span > k) {
        //in this case, lambda_span is larger than one degree
        gamma_lambda = 1;
        delta_lambda_minor = -1.0;
    }
    else {
        if (lambda_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > lambda_span > 10 arcminutes
            gamma_lambda = 60;
            delta_lambda_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > lambda_span
            gamma_lambda = 60 * 10;
            delta_lambda_minor = tenth_arcmin_radians;
        }
    }

    //compute delta_lambda
    delta_lambda = k / ((double)gamma_lambda);
    while (n_intervals_ticks * delta_lambda < lambda_span) {
        if (delta_lambda == k / ((double)gamma_lambda)) { delta_lambda += k * 4.0 / ((double)gamma_lambda); }
        else { delta_lambda += k * 5.0 / ((double)gamma_lambda); }
    }

    //compute lambda_middle
    lambda_middle.set(round((((circle_observer.reference_position).lambda).value) / delta_lambda) * delta_lambda);


    //set lambda_start, lambda_end
    if (((*(parent->lambda_min)) == 0.0) && ((*(parent->lambda_max)) == 0.0)) {
        //in this case circle_observer spans all longitudes

        (lambda_start.value) = 0.0;
        (lambda_end.value) = 2.0 * M_PI;

    }
    else {
        //in this case, there are two finite longitudes which encircle circle_observer

        if (((*(parent->lambda_min)) < M_PI) && ((*(parent->lambda_max)) > M_PI)) {

            (lambda_start.value) = floor(((*(parent->lambda_max)).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((*(parent->lambda_min)).value) + (2.0 * M_PI);

        }
        else {

            (lambda_start.value) = floor(((*(parent->lambda_max)).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((*(parent->lambda_min)).value);

        }

    }




    //set phi_start, phi_end and delta_phi
    phi_span = 2.0 * ((circle_observer.omega).value);

    //gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (phi_span > k) {
        //in this case, phi_span is larger than one degree
        gamma_phi = 1;
        delta_phi_minor = -1.0;
    }
    else {
        if (phi_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > phi_span > 10 arcminutes
            gamma_phi = 60;
            delta_phi_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > phi_span
            gamma_phi = 60 * 10;
            delta_phi_minor = tenth_arcmin_radians;
        }
    }

    delta_phi = k / ((double)gamma_phi);
    while (((wxGetApp().n_intervals_ticks_preferred).value) * delta_phi < phi_span) {
        if (delta_phi == k / ((double)gamma_phi)) { delta_phi += k * 4.0 / ((double)gamma_phi); }
        else { delta_phi += k * 5.0 / ((double)gamma_phi); }
    }

    //set phi_start/end and phi_middle
    (phi_start.value) = floor((((*(parent->phi_min)).normalize_pm_pi_ret()).value) / delta_phi) * delta_phi;
    (phi_end.value) = (((*(parent->phi_max)).normalize_pm_pi_ret()).value);

    phi_middle.set(round((((circle_observer.reference_position).phi).value) / delta_phi) * delta_phi);
    //if the line above sets phi_middle equal to +/- pi/2. the labels of meridians will all be put at the same location on the screen (the N/S pole), and they would look odd ->
    if ((fabs((phi_middle.value) - M_PI_2) < epsilon_double) || (fabs((phi_middle.value) - (3.0 * M_PI_2)) < epsilon_double)) {
        (phi_middle.value) -= GSL_SIGN((phi_middle.normalize_pm_pi_ret()).value) * delta_phi;
    }



    //compute the size of labels, i.e., size_label_horizontal and size_label_vertical
    //compute size of largest label on parallel: run through all labels on parallels and set size_label_horizontal as the size of the largest label on parallel that has ben found
    for (size_label_horizontal = 0,
        first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminutes) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        ((q.phi).value) = (phi_start.value),
        (q.lambda) = (*(parent->lambda_min)) - epsilon_double;
        ((q.phi).value) < (phi_end.value);
        ((q.phi).value) += delta_phi
        ) {

        WriteLabel(q, parent->phi_min, parent->phi_max, label_precision, String("NS"), &dummy_label);

        if ((GetTextExtent(dummy_label).GetWidth()) > ((int)size_label_horizontal)) {
            size_label_horizontal = (GetTextExtent(dummy_label).GetWidth());
        }

    }

    //take the angle 0° 0.0' expresed with display_precision: the height of this angle label is the largest possible -> set it equal to size_label_vertical
    size_label_vertical = (GetTextExtent(wxString((Angle(0, 0.0).to_string(String("NS"), (display_precision.value), false)))).GetHeight());

    TabulateRoutes();
    TabulatePositions();



    //compute labels on parallels and meridians
    //save parallels_and_meridians_labels_now and positions_parallels_and_meridians_labels_now into parallels_and_meridians_labels_before and  positions_parallels_and_meridians_labels_before, respectively.  clears all labels previously drawn
    parallels_and_meridians_labels_before = parallels_and_meridians_labels_now;
    positions_parallels_and_meridians_labels_before = positions_parallels_and_meridians_labels_now;
    parallels_and_meridians_labels_now.resize(0);
    positions_parallels_and_meridians_labels_now.resize(0);

    //compute labels on parallels
    for (first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminuted) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        (q.phi.value) = floor((circle_observer.reference_position.phi.normalize_pm_pi_ret().value - circle_observer.omega.value) / delta_phi) * delta_phi,
        (q.lambda) = lambda_middle;
        (q.phi.value) < (circle_observer.reference_position.phi.normalize_pm_pi_ret().value) + (circle_observer.omega.value);
        (q.phi.value) += delta_phi
        ) {

        DrawLabel(q, parent->phi_min, parent->phi_max, label_precision, String("NS"));

    }

    //compute labels on meridians
    for (first_label = true,
        //set the label precision: if gamma_lambda = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_lambda*K*60 (the spacing between labels in arcminutes) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_lambda == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_lambda * K * 60)))),
        ((q.lambda).value) = (lambda_start.value),
        (q.phi) = phi_middle;
        ((q.lambda).value) < (lambda_end.value);
        ((q.lambda).value) += delta_lambda
        ) {

        DrawLabel(q, parent->lambda_max, parent->lambda_min, label_precision, String("EW"));

    }


    grid_now.clear();
    ticks_now.clear();

    //draw meridians
    //set route equal to a meridian going through lambda: I set everything except for the longitude of the ground posision, which will vary in the loop befor and will be fixed inside the loop
    route.type.set(String(((Route_types[1]).value)));
    route.length.set(Re * M_PI, LengthUnit_types[0]);
    route.Z.set(0.0);
    (route.reference_position->phi) = -M_PI_2;

    for ((route.reference_position->lambda.value) = (lambda_start.value);
        (route.reference_position->lambda.value) < (lambda_end.value);
        (route.reference_position->lambda.value) += delta_lambda) {

        //add the current meridian that is being drawn (route) to meridians
        //        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.Draw((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //        route.Draw(((((parent->parent)->data)->n_points_routes).value), foreground_color, background_color, thickness, dc, this, String(""));

        if (gamma_lambda != 1) {
            //draw intermediate ticks on the longitude axis by setting route to an orthodrome pointing to the north

            (lambda_saved.value) = (route.reference_position->lambda.value);
            phi_saved = (route.reference_position->phi);
            Z_saved = (route.Z);

            route.Z.set(0.0);
            route.length.set(Re * 2.0 * ((wxGetApp().tick_length_over_aperture_circle_observer.value) * (circle_observer.omega.value)), LengthUnit_types[0]);
            (route.reference_position->phi) = phi_middle;

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for ((route.reference_position->lambda.value) = (lambda_saved.value);
                (route.reference_position->lambda.value) - (lambda_saved.value) < delta_lambda;
                (route.reference_position->lambda.value) += delta_lambda_minor) {

                //                ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //                route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));

            }

            route.length.set(Re * M_PI, LengthUnit_types[0]);
            (route.Z) = Z_saved;
            (route.reference_position->lambda.value) = (lambda_saved.value);
            (route.reference_position->phi) = phi_saved;

        }

    }

    //draw parallels
    //set route equal to a parallel of latitude phi, i.e., a circle of equal altitude
    route.type.set((Route_types[2]));
    (route.reference_position->lambda) = lambda_middle;

    //this loop runs over the latitude of the parallel, which we call phi
    for ((phi.value) = (phi_start.value);
        (phi.value) < (phi_end.value);
        (phi.value) += delta_phi
        ) {

        //route.omega  and route.reference_position->phi of the circle of equal altitude are set for each value of phi as functions of phi, in such a way that route.omega is always smaller than pi/2
        route.omega.set(M_PI_2 - fabs(phi.value));
        route.length.set(2.0 * M_PI * Re * sin(route.omega), LengthUnit_types[0]);
        route.reference_position->phi.set(GSL_SIGN(phi.value) * M_PI_2);

        //add the current parallel that is being drawn to parallels
        //        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.Draw((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //        route.Draw((parent->parent->data->n_points_routes.value), foreground_color, background_color, thickness, dc, this, String(""));

        if (gamma_phi != 1) {
            //to draw smaller ticks, I set route to a loxodrome pointing towards the E and draw it

            route.type.set(String(((Route_types[1]).value)));
            route.Z.set(M_PI_2);
            route.length.set(Re * 2.0 * ((wxGetApp().tick_length_over_aperture_circle_observer.value) * (circle_observer.omega.value)), LengthUnit_types[0]);

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for (
                (route.reference_position->phi.value) = (phi.value);
                (route.reference_position->phi.value) - (phi.value) < delta_phi;
                (route.reference_position->phi.value) += delta_phi_minor
                ) {

                //                    ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //                route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));

            }

            route.type.set((Route_types[2]));

        }

    }


    //updates the position of the DrawPanel *this
    draw_panel_origin = (this->GetScreenPosition());

    //tell PaintEvent that everything but highligghteable objects (coastlines, meridians ... ) must be re-drawn
    re_draw = true;

}




//if a key is pressed in the keyboard, I call this function
void DrawPanel::KeyDown(wxKeyEvent& event) {
    
    switch (event.GetKeyCode()) {
            
        case WXK_UP:
            
            parent->MoveNorth<wxKeyEvent>(event);
            
            break;
            
        case WXK_DOWN:
            
            parent->MoveSouth<wxKeyEvent>(event);
            
            break;
            
        case WXK_LEFT:
            
            parent->MoveWest<wxKeyEvent>(event);
            
            break;
            
        case WXK_RIGHT:
            
            parent->MoveEast<wxKeyEvent>(event);
            
            break;
            
        case WXK_ESCAPE:
            
            if(parent->parent->selection_rectangle){
                //If the user presses esc, I cancel the selection process with the rectangle in all ChartFrames, re-enable button_reset in all ChartFrames (because button_reset had been disabled as one started drawing selection_rectangle), and call RefreshAll and FitAll to re-draw the chart without the selection rectangle

                (parent->parent->selection_rectangle) = false;
                
                for(unsigned int i=0; i<parent->parent->chart_frames.size(); i++){
                    parent->parent->chart_frames[i]->button_reset->Enable(true);
                    
                }
            
            }
            
            
            (parent->parent->start_label_selection_rectangle) = String("");
            (parent->parent->end_label_selection_rectangle_now) = String("");
            (parent->parent->end_label_selection_rectangle_before) = String("");
            
            parent->parent->RefreshAll();
            FitAll();
            
            break;
            
        case WXK_PLUS:
            //the + key is pressed and control is pressed too -> I zoom in by multiplying the slider value by 2
            
            if (event.ControlDown()) {
                parent->SetSlider((parent->slider->GetValue()) * 2);
            }
            
            break;
            
            
        case WXK_MINUS:
            //the - key is pressed and control is pressed too -> I zoom out by dividing the slider value by 2
            
            if (event.ControlDown()) {
                parent->SetSlider(round(((parent->slider)->GetValue()) / 2.0));
            }
            
            break;
            
    }
    
    //    }
    
    event.Skip(true);
    
}



void DrawPanel::SetIdling(bool b) {

    idling = b;

}

//this function computes lambda_min, ... phi_max from x_min ... y_max for the mercator projection
void DrawPanel::Set_lambda_phi_min_max_Mercator(void) {

    parent->lambda_min.set(k * lambda_mercator(x_min));
    parent->lambda_max.set(k * lambda_mercator(x_max));

    parent->phi_min.set(k * phi_mercator(y_min));
    parent->phi_max.set(k * phi_mercator(y_max));

}

//this function computes lambda_min, ... phi_max (the  min/max latitudes and longitudes which encompass circle_observer) for the 3D projection
void DrawPanel::Set_lambda_phi_min_max_3D(void) {

    //consider the vector rp = {0,-1,0}, corresponding to the center of the circle of equal altitude above
    gsl_vector_set((rp.r), 0, 0.0);
    gsl_vector_set((rp.r), 1, -1.0);
    gsl_vector_set((rp.r), 2, 0.0);

    //convert rp -> r through rotation^{-1}
//        gsl_blas_dgemv(CblasTrans, 1.0, (rotation).matrix, (rp.r), 0.0, (r.r));
    cblas_dgemv(CblasRowMajor, CblasTrans, 3, 3, 1, rotation.matrix->data, 3, rp.r->data, 1, 0, r.r->data, 1);


    //obtain the  geographic position of the center of the circle of equal altitude above
    circle_observer.reference_position.setCartesian(String(""), r, String(""));


    //set lambda_min/max from circle_observer
    circle_observer.lambda_min_max((parent->lambda_min), (parent->lambda_max), String(""));

    //set
    d.set((-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))))*Re);
    //here I set the value of d into observer_height, not the unit of measure, because I want the user to decide the unit of measure by selecting in the wxComboBox in the unit field
    parent->observer_height->set_value_keep_unit();

    //set phi_min/max
    ((circle_observer.reference_position).phi).normalize_pm_pi();

    if (((((circle_observer.reference_position).phi).value) + ((circle_observer.omega).value) < M_PI_2) &&
        ((((circle_observer.reference_position).phi).value) - ((circle_observer.omega).value) > -M_PI_2)) {
        //in this case, circle_observer does not encircle the N/S pole

        (*(parent->phi_min)) = ((circle_observer.reference_position).phi) - (circle_observer.omega);
        (*(parent->phi_max)) = ((circle_observer.reference_position).phi) + (circle_observer.omega);

    }
    else {

        if ((((circle_observer.reference_position).phi).value) + ((circle_observer.omega).value) > M_PI_2) {
            //in this case, circle_observer encircles the N pole

            (*(parent->phi_min)) = ((circle_observer.reference_position).phi) - (circle_observer.omega);
            (*(parent->phi_max)).set(M_PI_2);

        }

        if ((((circle_observer.reference_position).phi).value) - ((circle_observer.omega).value) < -M_PI_2) {
            //in this case, circle_observer encircles the S pole

            (*(parent->phi_min)).set(3.0 * M_PI_2);
            (*(parent->phi_max)) = ((circle_observer.reference_position).phi) + (circle_observer.omega);

        }

    }

    ((circle_observer.reference_position).phi).normalize();

}


//this function computes x_min, ... y_max and from lambda_min ... phi_max in the Mercator projection
void DrawPanel::Set_x_y_min_max_Mercator(void) {
    
    PositionProjection p_min, p_max;
    Position temp;
    
    //    (this->*GeoToProjection)(Position(parent->lambda_min, parent->phi_min), &p_min, true);
    //    (this->*GeoToProjection)(Position(parent->lambda_max, parent->phi_max), &p_max, true);
    
    temp = Position(parent->lambda_min, parent->phi_min);
    //    (p_min.x) = -(((temp.lambda).normalize_pm_pi_ret()).value);
    //    (p_min.y) = log(1.0 / cos((temp.phi)) + tan((temp.phi)));
    p_min.NormalizeAndSetMercator(temp);
    
    temp = Position(parent->lambda_max, parent->phi_max);
    //    (p_max.x) = -(((temp.lambda).normalize_pm_pi_ret()).value);
    //    (p_max.y) = log(1.0 / cos((temp.phi)) + tan((temp.phi)));
    p_max.NormalizeAndSetMercator(temp);
    
    
    x_min = (p_min.x);
    y_min = (p_min.y);
    x_max = (p_max.x);
    y_max = (p_max.y);

}

//sets size_chart for the Mercator projection
void DrawPanel::Set_size_chart_Mercator(void) {

    if ((y_max - y_min) > x_span()) {

        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object

        size_chart.SetHeight((length_chart_over_length_chart_frame.value) * (((wxGetApp().rectangle_display).GetSize()).GetHeight()));
        size_chart.SetWidth((size_chart.GetHeight()) / ((y_max - y_min) / x_span()));
    }
    else {
        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object

        size_chart.SetWidth((length_chart_over_length_chart_frame.value) * (((wxGetApp().rectangle_display).GetSize()).GetHeight()));
        size_chart.SetHeight((size_chart.GetWidth()) * ((y_max - y_min) / x_span()));

    }

}

//set size_chart for the 3D projection
void DrawPanel::Set_size_chart_3D(void) {

    /*size_chart.SetHeight(((parent->GetSize()).GetHeight()) * 0.75);
    size_chart.SetWidth((size_chart.GetHeight()));*/


    size_chart.SetHeight((length_chart_over_length_chart_frame.value) * (((wxGetApp().rectangle_display).GetSize()).GetHeight()));
    size_chart.SetWidth((size_chart.GetHeight()));


}

/*returns a double: the width of the chart wich takes into account the fact that x_min and x_max may encompass the meridian lambda = pi*/
double DrawPanel::x_span(void) {

    if (x_max >= x_min) {
        //in this case, x_max, x_min do not encompass the meridian lambda = pi
        return(x_max - x_min);
    }
    else {
        //in this case, x_max, x_min encompass the meridian lambda = pi
        return(2.0 * M_PI - (x_min - x_max));
    }

}

//this function computes x_min, ... y_max from d in the 3D projection
void DrawPanel::Set_x_y_min_max_3D(void) {

    Double d_temp;

    //set d
    d_temp.set(-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))));

    x_min = -((d_temp.value) / sqrt(gsl_pow_2(((d_temp).value) + 1.0) - 1.0));
    x_max = -x_min;
    y_min = x_min;
    y_max = -y_min;

}

//puts point *p which lies outside the plot area, back into the plot area . It returns true if p is in the plot area, and false otherwise
bool DrawPanel::PutBackIn(wxPoint q, wxPoint* p) {

    bool output;

    output = true;

    (*p) = q;

    if ((p->x) < (position_plot_area_now.x)) {
        (p->x) = (position_plot_area_now.x);
        output = false;
    }

    if ((p->x) > (position_plot_area_now.x) + (size_plot_area.GetWidth())) {
        (p->x) = (position_plot_area_now.x) + (size_plot_area.GetWidth());
        output = false;
    }

    if ((p->y) < (position_plot_area_now.y)) {
        (p->y) = (position_plot_area_now.y);
        output = false;
    }

    if ((p->y) > (position_plot_area_now.y) + (size_plot_area.GetHeight())) {
        (p->y) = (position_plot_area_now.y) + (size_plot_area.GetHeight());
        output = false;
    }

    return output;

}


//generate a Rotation from the two points start and end (which are referred to the origin of the screen) in the 3D projection.
Rotation DrawPanel::rotation_start_end(const wxPoint& start, const wxPoint& end) {

    Position temp;
    Position p_start, p_end;

    //call ScreenToGeo_3D to generate rp, and then convert rp into spherical coordinates by writing it into p_start
    ScreenToGeo_3D(start, &temp);
    p_start.setCartesian(String(""), rp, String(""));

    //call ScreenToGeo_3D to generate rp, and then convert rp into spherical coordinates by writing it into p_end
    ScreenToGeo_3D(end, &temp);
    p_end.setCartesian(String(""), rp, String(""));

    //construct a Rotation between p_start and p_end by calling the overloaded constructor of the Rotation class
    return(Rotation(p_start, p_end));

}



//if i needs to be adjusted because it is not between -90 and +90, this method adjusts the pair of latitude, longitude (in arcdegrees) (i, j) and, if the pair is valud with respect to the latitude bounbdaries, it writes the adjusted vlaues in (*i_adjustged, *j_adjusted) and returns true, while it returns false otherwise
inline bool DrawPanel::AdjustLatitudeLongitude3D(const int& i, const int& j, int* i_adjusted, int* j_adjusted){
    
    bool check;
    
    //convert i,j into (*i_adjusted), (*j_adjusted)
    if (!((i >= -90) && (i <= 90))) {
        //in this case, i needs to be adjusted because it is not between -90 and +90
        
        if (i < -90) {
            
            if ((-(180 + i) - floor_min_lat >= 0) && (-(180 + i) - floor_min_lat < (parent->parent->coastline_polygons_Cartesian).size())) {
                
                (*i_adjusted) = -(180 + i);
                (*j_adjusted) = 180 + j;
                
                check = true;
                
            }
            else {
                
                check = false;
                
            }
            
        }
        
        if (i > 90) {
            
            if ((180 - i - floor_min_lat >= 0) && (180 - i - floor_min_lat < (parent->parent->coastline_polygons_Cartesian).size())) {
                
                (*i_adjusted) = 180 - i;
                (*j_adjusted) = 180 + j;
                
                check = true;
                
            }
            else {
                
                check = false;
                
            }
            
        }
        
        
    }
    else {
        
        if ((i - floor_min_lat >= 0) && (i - floor_min_lat < (parent->parent->coastline_polygons_Cartesian).size())) {
            
            (*i_adjusted) = i;
            (*j_adjusted) = j;
            
            check = true;
            
        }
        else {
            
            check = false;
            
        }
        
    }
    
    return check;
    
}


// the screen position p lies within the DrawPanel *this, it returns true and write it into the position q with respect to the DrawPanel *this. Otherwise, it returns alse, and does nothing with q
inline bool DrawPanel::ScreenToDrawPanel(const wxPoint& p, wxPoint* q) {

    bool check;

    check = ((p.x) >= (GetScreenPosition().x)) && ((p.x) <= (GetScreenPosition().x)) + (GetSize().x) && ((p.y) >= (GetScreenPosition().y)) && ((p.y) <= (GetScreenPosition().y)) + (GetSize().y);


    if (check) {
        (*q) = p - GetScreenPosition();
    }

    return check;

}

//if the point p (reckoned with respect to the origin of the screen) corresponds to a valid Position, i.e., it is in the plot area, convert p  into a geographic Position and  write the result into *q if q!=NULL. If p is in the plot area, it returns true and zero otherwise.
inline bool DrawPanel::ScreenToGeo_Mercator(const wxPoint& p, Position* q) {

    PositionProjection temp;
    bool check;

    //updates the position of the DrawPanel *this
    draw_panel_origin = (this->GetScreenPosition());


    check = ScreenToMercator(p, &temp);

    if (check && (q != NULL)) {

        ProjectionToGeo_Mercator(temp, q);
        
    }

    return check;

}

//convert the point p in the DrawPanel coordinates to the relative geographic position q, see specifics of ScreenToGeo_Mercator and ScreenToGeo_3D
inline bool DrawPanel::DrawPanelToGeo(const wxPoint& p, Position* q) {

    //computes the poisition of the DrawPanel *this which will be needed in the following
    draw_panel_origin = (this->GetScreenPosition());

    return(this->*ScreenToGeo)(p + draw_panel_origin, q);

}



//converts the point p on the screen with a 3D projection, to the relative geographic position q (if q!=NULL). It returns true if p lies within the circle denoting the boundaries of the earth, and false otherwise. If false is returned, q is the geographic position on the earth defined as follows: it lies on the intersection between the Earth and the x'z' plane and on the line between the center of the Earth and the vector rp corresponding to p (such vector rp lies on the x'z' plane)
inline bool DrawPanel::ScreenToGeo_3D(const wxPoint& p, Position* q) {

    PositionProjection temp;

    if (ScreenTo3D(p, &temp)) {
        //p lies within the circle of the earth

        if (q != NULL) {

            Double d_temp;

            //here I put the sign of (temp.x) in front of the square root, in order to pick the correct solutio among the two possible solutios for xp, yp. The correct solution is the one yielding the values of xp, yp on the visible side of the sphere. For example, for (temp.x)<0, a simple geometrical construction shows that the solution corresponding to the visible side of the sphere is the one with the larger (temp.x) -> I pick the solution with a positive sign in front of the square root through GSL_SIGN((temp.x))
            //set rp
            d_temp.set(-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))));

            gsl_vector_set((rp.r), 0, (-(temp.x) * sqrt(arg_sqrt) + (d_temp.value) * ((d_temp.value) + 1.0) * (temp.x)) / (gsl_sf_pow_int((d_temp.value), 2) + gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2)));
            gsl_vector_set((rp.r), 2, (-sqrt(arg_sqrt) * (temp.y) + (d_temp.value) * ((d_temp.value) + 1.0) * (temp.y)) / ((gsl_sf_pow_int((d_temp.value), 2) + gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2))));
            gsl_vector_set((rp.r), 1, -sqrt(1.0 - (gsl_pow_2(gsl_vector_get((rp.r), 0)) + gsl_pow_2(gsl_vector_get((rp.r), 2)))));

            //r = (rotation.matrix)^T . rp
            //            gsl_blas_dgemv(CblasTrans, 1.0, rotation.matrix, (rp.r), 0.0, (r.r));
            cblas_dgemv(CblasRowMajor, CblasTrans, 3, 3, 1, rotation.matrix->data, 3, rp.r->data, 1, 0, r.r->data, 1);


            q->setCartesian(String(""), r, String(""));

        }

        return true;

    }
    else {
        //p does not lie within the circle of the earth

        if (q != NULL) {

            Double d_temp;

            d_temp.set(-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))));

            //from projection, compute the relative point on the x'z' plane, which has y'=0
            gsl_vector_set((rp.r), 0, ((d_temp.value) + 1.0) / (d_temp.value) * (temp.x));
            gsl_vector_set((rp.r), 2, ((d_temp.value) + 1.0) / (d_temp.value) * (temp.y));
            gsl_vector_set((rp.r), 1, 0.0);

            //r = (rotation.matrix)^T . rp
            //            gsl_blas_dgemv(CblasTrans, 1.0, rotation.matrix, (rp.r), 0.0, (r.r));
            cblas_dgemv(CblasRowMajor, CblasTrans, 3, 3, 1, rotation.matrix->data, 3, rp.r->data, 1, 0, r.r->data, 1);

            q->setCartesian(String(""), r, String(""));

        }

        return false;

    }

}

//convert the point p on the screen to the  Mercator projection q of the relative geographic position, by writing into q only if q!=NULL. It returns true/false if q lies within the boundaris x_min .. y_max
inline bool DrawPanel::ScreenToMercator(const wxPoint& p, PositionProjection* q) {

    PositionProjection temp;
//    bool check_x;

    //updates the position of the draw pane this
    draw_panel_origin = (this->GetScreenPosition());

    (temp.x) = x_min + (((double)(p.x) - ((draw_panel_origin.x) + (position_plot_area_now.x))) / ((double)(size_plot_area.GetWidth()))) * x_span();
    (temp.y) = y_min - (((double)(p.y)) - ((draw_panel_origin.y) + (position_plot_area_now.y) + (size_plot_area.GetHeight()))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min);

    if (q) {
        (q->x) = (temp.x);
        (q->y) = (temp.y);
    }

    return(temp.CheckMercator(this));

}


//converts the point p on the screen (which is supposed to lie in the plot area), to the  3D projection (x,y), which is written in q if q!=NULL. If p lies within /outside the circle of the earth, it returns true/false.
inline bool DrawPanel::ScreenTo3D(const wxPoint& p, PositionProjection* q) {

    PositionProjection temp;
    Double d_temp;

    //set d for the following
    d_temp.set(-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))));

    //updates the position of the draw pane this
    draw_panel_origin = (this->GetScreenPosition());

    (temp.x) = x_min + ((((double)(p.x)) - ((draw_panel_origin.x) + (position_plot_area_now.x))) / ((double)(size_plot_area.GetWidth()))) * (x_max - x_min);
    (temp.y) = y_min - (((double)(p.y)) - ((draw_panel_origin.y) + (position_plot_area_now.y) + (size_plot_area.GetHeight()))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min);

    //I pulled out a factor (temp.x)^2 from arg_sqrt for clarity
    arg_sqrt = -((gsl_sf_pow_int((d_temp.value), 2) * (-1 + gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2)) + 2 * (d_temp.value) * (gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2))));

    //if q!=NULL, I write in it the result. I do this even if p lies outside the sphere of the earth
    if (q) {

        (*q) = temp;

    }

    if (arg_sqrt >= 0.0) {

        return true;

    }
    else {

        return false;

    }

}


//convert the geographic Position p  to the  3D PositionProjection (x,y). / If the PositionProjection of p falls in the visible side of the earth,  write its PositionProjection into *q (if q!=NULL) and returs true. If not, it returns false and, if write = true,  write its Projection in *q (if q!=NULL)
inline bool DrawPanel::GeoTo3D(const Position& p, PositionProjection* q, bool write) {


    //    clock_t t1, t2, t3;
    //    double Ta, Tb;
    //
    //    t1 = clock();

//    bool check, out;

    //compute rp before hand to rule out early the cases where check = 0
    //set r according equal to the 3d vector corresponding to the geographic position p
    /*
    gsl_vector_set((r.r), 0, cos((p.lambda)) * cos((p.phi)));
    gsl_vector_set((r.r), 1, -(cos((p.phi)) * sin((p.lambda))));
    gsl_vector_set((r.r), 2, sin((p.phi)));
    */
//    p.getCartesian(String(""), &r, String(""));
    r.setPosition(&p);

    return CartesianTo3D(r, q, write);
   
}



// If the PositionProjection of q falls within the plot area,  write its Projection into p (if p!=NULL) and return true. If not, it returns false and, if write = true, it writes its projection in q
inline bool DrawPanel::CartesianToMercator(const Cartesian& p, PositionProjection* q, bool write) {

    Position temp;

    temp.setCartesian(String(""), p, String(""));

    return ((this->*GeoToProjection)(temp, q, write));


}


//convert the Cartesian position p  to the  3D PositionProjection (x,y). / If the PositionProjection of p falls in the visible side of the earth,  write its PositionProjection into *q (if q!=NULL) and return true. If not,  return false and, if write = true,  write its Projection in *q (if q!=NULL)
inline bool DrawPanel::CartesianTo3D(const Cartesian& p, PositionProjection* q, bool write) {

    bool check, out;
    
    gsl_vector_set((rp.r), 1,
                   /*
                    gsl_matrix_get(rotation.matrix, 1, 0) * gsl_vector_get((p.r), 0) +
                    gsl_matrix_get(rotation.matrix, 1, 1) * gsl_vector_get((p.r), 1) +
                    gsl_matrix_get(rotation.matrix, 1, 2) * gsl_vector_get((p.r), 2)
                    */
                   cblas_ddot(3, (rotation.matrix->data)+3, 1, p.r->data, 1)
                   );
    
    
    check = (gsl_vector_get((rp.r), 1) < -1.0 / (1.0 + (d.value)/Re));



    //    t2 = clock();
    //    Ta = t2-t1;


    if (check || write) {

        if (q != NULL) {

            double temp;

            //rotate r by rotation, and write the result in rp!
            //            gsl_blas_dgemv(CblasNoTrans, 1.0, rotation.matrix, (p.r), 0.0, (rp.r));
            cblas_dgemv(CblasRowMajor, CblasNoTrans, 3, 3, 1, rotation.matrix->data, 3, p.r->data, 1, 0, rp.r->data, 1);
            
            temp = (d.value) / ((d.value) + Re*(1.0 + gsl_vector_get((rp.r), 1)));
            (q->x) = gsl_vector_get((rp.r), 0) * temp;
            (q->y) = gsl_vector_get((rp.r), 2) * temp;

        }

        out = check;


    }
    else {

        out = false;

    }

    //    t3 = clock();
    //    Tb = t3-t2;

    return out;


}





//this function converts the geographic position p into the screen position p
inline void DrawPanel::GeoToScreen(const Position& q, wxPoint* p) {

    //updates the position of the draw pane this
    draw_panel_origin = (this->GetScreenPosition());


    GeoToDrawPanel(q, p, false);

    (p->x) += (draw_panel_origin.x);
    (p->y) += (draw_panel_origin.y);

}


// If the projection of q falls within the plot area, it writes its projection into p (if p!=NULL) and returns true. If not, it returns false and, if write = true, it writes its projection in p
inline bool DrawPanel::GeoToMercator(const Position& q, PositionProjection* p, bool write) {

    //    clock_t t_start, t_end;
    //    t_start = clock();

    PositionProjection temp;
    bool /*check_x, */check, out;

    //    (temp.x) = -(normalize_pm_pi_ret(q.lambda).value);
    //    (temp.y) = log(1.0 / cos((q.phi)) + tan((q.phi)));
    temp.NormalizeAndSetMercator(q);
    check = temp.CheckMercator(this);
    

    if (check || write) {
        //if the point falls within the plot area, write it into x, y

        if (p != NULL) {

            (p->x) = (temp.x);
            //this is needed if lambda_min, lambda_max encompass the Greenwich antimeridian: if p->x is smaller than x_max, then it nees to be translated to the right by 2pi in order to be plotted
            if ((x_max < x_min) && ((p->x) <= x_max)) {
                (p->x) += 2.0*M_PI;
            }

            (p->y) = (temp.y);

        }

        out = check;


    }
    else {

        out = false;

    }

    return out;

}





// convert the geographic position q into the DrawPanel position p, reckoned with respect to the origin of the  DrawPanel. If q is a valid Position, it returns true and (if p!=NULL), it writes the resulting DrawPanel coordinates in p. If q is not a valid position, it returns false and, if write = true and p!=NULL, it writes the drawpanel position in p.
inline bool DrawPanel::GeoToDrawPanel(const Position& q, wxPoint* p, bool write) {

    PositionProjection temp;
    bool check;


    check = (this->*GeoToProjection)(q, &temp, write);

    if (check || write) {

        if (p) {
            (this->*ProjectionToDrawPanel)(temp, p, true);
        }

        return check;

    }
    else {

        return false;

    }

}


// convert the cartesian position q into the DrawPanel position p, reckoned with respect to the origin of the  DrawPanel. If q is a valid Cartesian position, return true and (if p!=NULL),  write the resulting DrawPanel coordinates in p. If q is not a valid Cartesian position,  return false and, if write = true and p!=NULL, it writes the drawpanel position in p.
inline bool DrawPanel::CartesianToDrawPanel(const Cartesian& q, wxPoint* p, bool write) {

    PositionProjection temp;
    bool check;

    check = (this->*CartesianToProjection)(q, &temp, write);

    if (check || write) {

        if (p) {
            (this->*ProjectionToDrawPanel)(temp, p, true);
        }

        return check;

    }
    else {

        return false;

    }

}

//converts the Mercator projection q into the DrawPanel position p, reckoned with respect to the origin of the mercator draw panel.   If q is a valid PositionProjection , return true and (if p!=NULL),  write the resulting DrawPanel coordinates in p. If q is not a valid  PositionProjection,  return false and, if write = true and p!=NULL, write the DrawPanel position in p.
inline bool DrawPanel::ProjectionToDrawPanel_Mercator(PositionProjection& q, wxPoint* p, bool write) {
    
    bool check;
    PositionProjection temp;
    
    check = q.CheckMercator(this);

    if(check || write){
        
        if(p){
            
            temp = q;
            
            //this is needed if lambda_min, lambda_max encompass the Greenwich antimeridian: if q.x is smaller than x_max, then it nees to be translated to the right by 2 * pi
            if ((x_max < x_min) && ((temp.x) <= x_max)) {
                (temp.x) += 2.0*M_PI;
            }
            
            (p->x) = (position_plot_area_now.x) + ((temp.x) - x_min) / x_span() * (size_plot_area.GetWidth());
            (p->y) = (position_plot_area_now.y) + (size_plot_area.GetHeight()) - (((temp.y) - y_min) / (y_max - y_min) * (size_plot_area.GetHeight()));
            
        }
        
        return check;
        
    }else{
        
        return false;
        
    }

}


//convert the Mercator Projection q into the Position p
inline void  DrawPanel::ProjectionToGeo_Mercator(const PositionProjection& q, Position* p) {

    p->lambda.set(k * lambda_mercator(q.x));
    p->phi.set(k * phi_mercator(q.y));
  
}


//convert the Mercator Projection q into the Position p
inline void  DrawPanel::ProjectionToGeo_3D(const PositionProjection& q, Position* p) {

  
}


//this function converts the 3D PositionProjection q into the DrawPanel position p, reckoned with respect to the origin of the mercator DrawPanel.  If q is a valid PositionProjection, return true and (if p!=NULL), write the resulting DrawPanel coordinates in p. If q is not a valid  PositionProjection,  return false and, if write = true and p!=NULL, write the DrawPanel position in p.
inline bool DrawPanel::ProjectionToDrawPanel_3D(PositionProjection& q, wxPoint* p, bool write) {
    
    bool check;
    
    
    check = ((fabs(q.x) <= x_max) && (fabs(q.y) <= y_max));
    
    if(check || write){
        
        if(p){
            
            (p->x) = ((double)(position_plot_area_now.x)) + (1.0 + (q.x) / x_max) * (((double)(size_plot_area.GetWidth())) / 2.0);
            (p->y) = ((double)(position_plot_area_now.y)) + (1.0 - (q.y) / y_max) * (((double)(size_plot_area.GetHeight())) / 2.0);
         
        }
        
        return check;
        
    }else{
        
        return false;
        
    }
    
}



//given a Position q if q lies witin *this, write in label a text with the geographic coordinates of q
void DrawPanel::ShowCoordinates(const Position& q, String* label) {

    wxPoint temp;

    if (GeoToDrawPanel(q, &temp, false)) {

        SetLabelAndAdjustPosition(q, &temp, label);

    }
    else {

        (*label) = String("");

    }

}


//given a geographic Positiojn q, if q lies within *this, write in label a text with the geographic coordinates corresponding to q, and write in *position the position of the label close to q (with some margin, for clarity). Otherwise, write "" in label and does nothing witg poisition
void DrawPanel::SetLabelAndPosition(const Position& q, wxPoint* position, String* label) {

    if (
        /*GeoToDrawPanel converts q into the wxPoint position, reckoned with respect to the origin *this*/(this->GeoToDrawPanel)(q, position, false)) {

        //SetCoordinateLabel uses position set from above, and ajusts it by including some margins
        SetLabelAndAdjustPosition(q, position, label);

    }
    else {

        (*label) = String("");


    }

}


//given a position q with respect to the origin of the screen, if q lies within *this, write in label a text with the geographic coordinates corresponding to q, and write in *position the position of the label close to q (with some margin, for clarity). Otherwise, write "" in label and does nothing witg poisition
void DrawPanel::SetLabelAndPosition(const wxPoint& q, wxPoint* position, String* label) {

    if ((this->ScreenToDrawPanel)(q, position)) {

        Position temp;

        (this->*ScreenToGeo)(q, &temp);
        SetLabelAndAdjustPosition(temp, position, label);

    }
    else {

        (*label) = String("");

    }

}


//given a geographic Position p and its corresponding wxPoint with respect to the origin of this *poisition, write a string containing the geographic coordinates of p into label, and adjust *poistiion in such a way that label is enclosed in *this
void DrawPanel::SetLabelAndAdjustPosition(const Position& p, wxPoint* position, String* label) {

    //the shift that will be applied to the position of *label
    wxPoint shift;

    //set the text of *label
    label->set(to_string(p, display_precision.value));

    //the default value of the shift
    shift = wxPoint(
        (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)
    );


    if ((position->x) + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value) + ((label->get_size(this)).x) > (size_plot_area.x) + (position_plot_area_now.x)) {
        //label does not fit into *this: it goes beyond the right edge of *this -> move it to the left

        shift -= wxPoint(
            (((label->get_size(this)).x) + 2 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)),
            0
        );

    }

    if ((position->y) + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value) + ((label->get_size(this)).y) > (size_plot_area.y) + (position_plot_area_now.y)) {
        //label does not fit into *this: it goes beyond the bottom edge of *this -> move up shift

        shift -= wxPoint(
            0,
            (((label->get_size(this)).y) + 2 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))
        );

    }

    //hift the label label with respect to p
//    label->SetPosition(position + shift);
    //adjust *position
    (*position) += shift;

}


//set ChartFrame title and all quantities according to a chosen projection
template<class E> void DrawPanel::SetProjection(E& event) {
    
    stringstream s;
    String temp;
    size_t pos;

    //set the ChartFrame title
    //extract the part of the title which comes before the name of the projection type
    temp = String((parent->GetLabel()).ToStdString());
    pos = (temp.value).find(" - ");
    temp.set(temp.value.substr(0, pos).c_str());

    //put together temp and the new name of the projection type and write the result into the title of parent
    s.str("");
    s << (temp.value) << " - " << (((parent->projection)->name)->GetValue().ToStdString()) << " projection";
    temp.set(s.str());
    parent->SetLabel(wxString(s.str().c_str()));


    if ((parent->projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {
        //if in projection "mercator" is selected, then I let the Draw function pointer point to PreRenderMercator, same for other functions, and I disable the fields of the angle for the Euler rotation of the 3d earth, which are not necessary

        PreRender = (&DrawPanel::PreRenderMercator);
        Render = (&DrawPanel::Render_Mercator);
        ProjectionToDrawPanel = (&DrawPanel::ProjectionToDrawPanel_Mercator);
        ProjectionToGeo = (&DrawPanel::ProjectionToGeo_Mercator);
        ScreenToProjection = (&DrawPanel::ScreenToMercator);
        CartesianToProjection = (&DrawPanel::CartesianToMercator);
        ScreenToGeo = (&DrawPanel::ScreenToGeo_Mercator);
        GeoToProjection = (&DrawPanel::GeoToMercator);
        Set_x_y_min_max = (&DrawPanel::Set_x_y_min_max_Mercator);
        Set_lambda_phi_min_max = (&DrawPanel::Set_lambda_phi_min_max_Mercator);
        Set_size_chart = (&DrawPanel::Set_size_chart_Mercator);
        (parent->UpdateSliderLabel) = (&ChartFrame::UpdateSliderLabel_Mercator);
        
        //in the 3D projection the scale of the chart, shown in text_slider, does not makes sense -> set it to empty
        parent->observer_height->value->SetLabel(wxS(""));
        parent->observer_height->unit->name->SetValue(wxS(""));
        parent->observer_height->unit->name->Enable(false);
   

    }

    if ((parent->projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {
        //if in projection ((Projection_types[1]).value) is selected, then I let the Draw function pointer point to PreRender3D, same for other functions, and I enable the angles for the 3d rotation of the 3d earth, which are now needed from the user.

        PreRender = (&DrawPanel::PreRender3D);
        Render = (&DrawPanel::Render_3D);
        ProjectionToDrawPanel = (&DrawPanel::ProjectionToDrawPanel_3D);
        ProjectionToGeo = (&DrawPanel::ProjectionToGeo_3D);
        ScreenToProjection = (&DrawPanel::ScreenTo3D);
        CartesianToProjection = (&DrawPanel::CartesianTo3D);
        ScreenToGeo = (&DrawPanel::ScreenToGeo_3D);
        GeoToProjection = (&DrawPanel::GeoTo3D);
        Set_x_y_min_max = (&DrawPanel::Set_x_y_min_max_3D);
        Set_lambda_phi_min_max = (&DrawPanel::Set_lambda_phi_min_max_3D);
        Set_size_chart = (&DrawPanel::Set_size_chart_3D);
        (parent->UpdateSliderLabel) = (&ChartFrame::UpdateSliderLabel_3D);

        //in the 3D projection the scale of the chart, shown in text_slider, does not makes sense -> set it to empty
        parent->chart_scale->SetLabel(wxS(""));
        parent->observer_height->unit->name->Enable(true);
        parent->observer_height->set();

        
    }
    
    event.Skip(true);

}


//this method is called when the used has chosen Projection -> set all quantities according to the chosen Projection and call Reset to re-render everything
template<class E> void DrawPanel::OnChooseProjection(E& event) {

    SetProjection<E>(event);
    parent->ResetRender<E>(event);
    parent->Animate();

    event.Skip(true);

}


//This function obtains the geographical Position p of the mouse hovering on the map of the world. It returns true if the mouse is in the plot area, false otherwise
bool DrawPanel::GetMouseGeoPosition(Position* p) {

    //    (parent->parent->screen_position_now) = wxGetMousePosition();

    return ((this->*ScreenToGeo)((parent->parent->screen_position_now), p));

}

void DrawPanel::OnMouseMovement(wxMouseEvent& event) {

    wxPoint q;
    stringstream s;
    int i, j, l;
    
    //lines for debug
    //    cout << "\nMouse moved";
    //    //    cout << "Position of text_position_now = {" << ((parent->text_position_now)->GetPosition()).x << " , " << ((parent->text_position_now)->GetPosition()).x << "}\n";
    //    cout << "Position of mouse screen = {" << (parent->parent->screen_position_now).x << " , " << (parent->parent->screen_position_now).y << "}\n";
    //    cout << "Position of mouse draw panel = {" << ((parent->parent->screen_position_now)-draw_panel_origin).x << " , " << ((parent->parent->screen_position_now)-draw_panel_origin).y << "}\n";
    //lines for debug


#ifdef _WIN32

    //store the former _now positions into the _before positions
    (parent->parent->screen_position_before) = (parent->parent->screen_position_now);
    (parent->parent->geo_position_before) = (parent->parent->geo_position_now);
    label_position_before = label_position_now;

#endif

    //update the instantaneous screen and geographic position of the mouse on the chart and compute mouse_in_plot_area, which will be used by other methods.
    (parent->parent->mouse_moving) = true;
    (parent->parent->screen_position_now) = wxGetMousePosition();
    mouse_in_plot_area = (this->*ScreenToGeo)((parent->parent->screen_position_now), &((parent->parent->geo_position_now)));
    if (mouse_in_plot_area && (!parent->parent->selection_rectangle)) {
        //the mouse has a screen position corresponding to a geographic position and no selection rectangle is being drawn -> I show the instantaneous mouse coordinates : I write them into label_position_now, otherwise label_position_now is left empty,

        label_position_now = String((parent->parent->geo_position_now.to_string(display_precision.value)));

    }
    else {

        label_position_now = String("");

    }


    if ((parent->parent->selection_rectangle)) {
        //a selection rectangle is being drawn -> update the instantaneous position of the final corner of the rectangle

#ifdef __APPLE__

        for (i = 0; i < (parent->parent->chart_frames.size()); i++) {

            //write the label and position of the selection rectangle for each DrawPanel into end_label_selection_rectangle_now and position_end_label_selection_rectangle_now, respectively
            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                (parent->parent->geo_position_now),
                &(((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_now),
                &(parent->parent->end_label_selection_rectangle_now)
            );


        }


#endif

#ifdef _WIN32

        //on WIN32, the Refresh() command slows down things -> I don't call it but use RefreshWIN32(), which cleans up the former selections rectangle in *this and draws a new one
        (parent->parent->end_label_selection_rectangle_before) = (parent->parent->end_label_selection_rectangle_now);

        for (i = 0; i < (parent->parent->chart_frames.size()); i++) {

            (((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_before) = (((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_now);

            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition((parent->parent->geo_position_now), &(((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_now), &(parent->parent->end_label_selection_rectangle_now));

//            ((parent->parent->chart_frames)[i])->draw_panel->RefreshWIN32();

        }

#endif

        parent->parent->MyRefreshAll();

    }
    else {
        //no selection rectangle is being drawn

        //run over all the routes, check if the mouse is hovering over one of them, and change the background color of the related position in listcontrol_routes

        //I compute the position of the mouse with respect to the origin of the DrawPanel, so I can compare it with points_route_list[i], which are also with respect to the origin of the draw panel
        position_draw_panel_now = (parent->parent->screen_position_now) - draw_panel_origin;

        //save the id of the Route highlighted at the preceeding step into highlighted_route_before
        (parent->parent->highlighted_route_before) = (parent->parent->highlighted_route_now);

        for ((parent->parent->highlighted_route_now) = -1, i = 0; i < (parent->parent->data->route_list).size(); i++) {

            //set the backgorund color of the Route in listcontrol_routes and of its related sight to white
            //when only a fraction of the Routes is Drawn, this will create a problem ---
            (parent->parent->listcontrol_routes)->SetItemBackgroundColour(i, wxGetApp().background_color);
            //when only a fraction of the Routes is Drawn, this will create a problem ---

            if ((((parent->parent->data->route_list)[i]).related_sight).value != -1) {
                (parent->parent->listcontrol_sights)->SetItemBackgroundColour((((parent->parent->data->route_list)[i]).related_sight).value, wxGetApp().background_color);
            }

            //run over Routes and check whether the mouse is hovering over one of them
            for (j = 0; j < (points_route_list_now[i]).size(); j++) {

                for (l = 0; l < ((int)((points_route_list_now[i][j]).size())) - 1; l++) {

                    //if the mouse is hovering over one of the points of route #i, I set the background color of route i in listcontrol_routes to a color different from white, to highlight it, and I highlight also the related sight in listcontrol_sights

                    if (/*to recognize that the mouse is hovering over a Route, I need the abscissas of two subsequent points of the Route to be different. Otherwise, there is not space on the screen where to recognize the presence of the mouse*/ (((points_route_list_now[i][j][l]).x) != ((points_route_list_now[i][j][l + 1]).x))

                        &&/*I check the the mouse's abscissa falls within the abscissas of two subsewquent points of the Route*/

                        (((((points_route_list_now[i][j][l]).x) <= (position_draw_panel_now.x)) && ((position_draw_panel_now.x) <= ((points_route_list_now[i][j][l + 1]).x))) ||

                            ((((points_route_list_now[i][j][l + 1]).x) <= (position_draw_panel_now.x)) && ((position_draw_panel_now.x) <= ((points_route_list_now[i][j][l]).x))))

                        &&/*I check the the mouse's ordinate falls within the ordinates of the two subsewquent points of the Route above*/

                        (
                            fabs(
                                (position_draw_panel_now.y) -
                                (((points_route_list_now[i][j][l]).y) + ((double)(((points_route_list_now[i][j][l + 1]).y) - ((points_route_list_now[i][j][l]).y))) / ((double)(((points_route_list_now[i][j][l + 1]).x) - ((points_route_list_now[i][j][l]).x))) * ((double)((position_draw_panel_now.x) - ((points_route_list_now[i][j][l]).x))))
                            )

                            <= (thickness_route_selection_over_length_screen.value) * ((double)(wxGetApp().rectangle_display.GetWidth())) / 2.0
                            )
                        ) {
                        //the mouse is overing over a Route


                        //set the highlighted route to i, so as to use highlighted_route in other functions
                        (parent->parent->highlighted_route_now) = i;

                        parent->parent->listcontrol_routes->EnsureVisible(i);
                        if ((((parent->parent->data->route_list)[i]).related_sight.value) != -1) {
                            parent->parent->listcontrol_sights->EnsureVisible(((parent->parent->data->route_list)[i]).related_sight.value);
                        }

                        //set highlighted_sight_now and the beckgorund color of the Route in listcontrol_routes and of its related sight to a highlight color
                        (parent->parent->listcontrol_routes)->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                        if ((((parent->parent->data->route_list)[i]).related_sight.value) != -1) {

                            (parent->parent->highlighted_sight_now) = (((parent->parent->data->route_list)[i]).related_sight.value);

                            parent->parent->listcontrol_sights->SetItemBackgroundColour(
                                (parent->parent->highlighted_sight_now),
                                (wxGetApp().color_selected_item)
                            );
                        }
                        else {

                            (parent->parent->highlighted_sight_now) = -1;

                        }


                        // quit the loops over l ad j
                        break;
                        break;

                    }

                }

            }

        }



        if ((parent->parent->highlighted_route_now) == -1) {
            //no Route is highlighted -> in listcontrol_sights and listcontrol_routes go back to showing the first respective items

            if ((parent->parent->listcontrol_routes->GetItemCount()) > 0) {

                parent->parent->listcontrol_routes->EnsureVisible(0);

            }

            if ((parent->parent->listcontrol_sights->GetItemCount()) > 0) {

                parent->parent->listcontrol_sights->EnsureVisible(0);

            }

        }


        //run over all the Positions, check if the mouse is hovering over one of them, and change the background color of the related Position in listcontrol_positions
        (parent->parent->highlighted_position_before) = (parent->parent->highlighted_position_now);

        for ((parent->parent->highlighted_position_now) = -1, i = 0; i < (parent->parent->data->position_list).size(); i++) {

            GeoToScreen((parent->parent->data->position_list)[i], &q);

            if (sqrt(gsl_pow_2(((parent->parent->screen_position_now).x) - (q.x)) + gsl_pow_2(((parent->parent->screen_position_now).y) - (q.y))) <
                4.0 * ((((wxGetApp().standard_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth())) {
                //the mouse is over a position

                //sets the highlighted position to i, so as to use highlighted_position_now in other functions
                (parent->parent->highlighted_position_now) = i;

                (parent->parent->listcontrol_positions)->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                parent->parent->listcontrol_positions->EnsureVisible(i);

            }
            else {
                //no Position is highlighted -> reset the background color in listcontrol positions, and in listcontrol_positions go back to showing the first  item

                (parent->parent->listcontrol_positions)->SetItemBackgroundColour(i, wxGetApp().background_color);
                parent->parent->listcontrol_positions->EnsureVisible(0);

            }

        }

        if (((parent->parent->highlighted_route_before) != (parent->parent->highlighted_route_now)) || ((parent->parent->highlighted_position_before) != (parent->parent->highlighted_position_now))) {
            //the highlighted Route or Position has changed -> update the charts

            (parent->parent->changing_highlighted_object) = true;

            parent->parent->MyRefreshAll();

            (parent->parent->changing_highlighted_object) = false;


        }
        else {
            //the highlighted Route has not changed ->  the chart does not need to be updated, but the coordinates of the instantaneous mouse position do -> call

            MyRefresh();

        }

    }


    (parent->parent->mouse_moving) = false;

    event.Skip(true);

}

//if the left button of the mouse is pressed, I record its position as the starting position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftDown(wxMouseEvent& event) {

    position_start_drag = wxGetMousePosition();
    (this->*ScreenToGeo)(position_start_drag, &geo_start_drag);

    if (((parent->projection->name)->GetValue()) == wxString(((Projection_types[0]).value))) {

        //I store the boundaries of the plot at the beginning of the drag, so if the drag is aborted I will restore these boundaries
        x_min_start_drag = x_min;
        x_max_start_drag = x_max;
        y_min_start_drag = y_min;
        y_max_start_drag = y_max;
        x_span_start_drag = x_span();

    }

    if (((parent->projection->name)->GetValue()) == wxString(((Projection_types[1]).value))) {

        //I store the orientation of the earth at the beginning of the drag in rotation_start_drag
        //        gsl_vector_memcpy((rp_start_drag.r), (rp.r));
        rotation_start_drag.set(rotation);
        //        geo_start_drag.print(String("position start drag"), String(""), cout);
        //        rotation_start_drag.print(String("rotation start drag"), String(""), cout);

    }

    event.Skip(true);

}

//if the left button of the mouse is released, I record its position as the ending position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftUp(wxMouseEvent& event) {

    SetCursor(*wxCROSS_CURSOR);

    //if the mouse left button was previously down because of a dragging event, then the dragging event is now over, and I set mouse_dragging = false;
    if (mouse_dragging) {
        //the left button of the mouse has been lifted at the end of a drag

        mouse_dragging = false;
        //given that the mosue drag has ended, I re-bind OnMoueMOvement to the mouse motion event
        this->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, this);


        position_end_drag = wxGetMousePosition();
        (this->*ScreenToGeo)(position_start_drag, &geo_end_drag);



        if (((parent->parent->highlighted_route_now) == -1) && (((parent->parent)->highlighted_position_now) == -1)) {
            //I am dragging the chart (not a Route nor  a Position)

            if ((parent->projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {
                //I am using the Mercator projection

                double delta_y;
                PositionProjection p_ceil_min, p_floor_max;

                delta_y = ((double)((position_end_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min);

                (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
                (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);


                if ((!((y_max + delta_y < (p_floor_max.y)) && (y_min + delta_y > (p_ceil_min.y))))) {
                    //in this case,  the drag operation ends out  the min and max latitude contained in the data files -> reset y_min, y_max to their original values

                    //                    x_min = x_min_start_drag;
                    //                    x_max = x_max_start_drag;
                    y_min = y_min_start_drag;
                    y_max = y_max_start_drag;

                    (this->*Set_lambda_phi_min_max)();

                    //re-draw the chart
                    (this->*PreRender)();
                    Refresh();
                    FitAll();

                }

            }

            if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {

//                gsl_vector_memcpy((rp_end_drag.r), (rp.r));
                rotation_end_drag.set(rotation);
//                geo_end_drag.print(String("position end drag"), String(""), cout);
//                rotation_end_drag.print(String("rotation end drag"), String(""), cout);

            }

            //the drag operation has ended -> I set
            (parent->dragging_chart) = false;
            (parent->parent->i_object_to_disconnect) = -1;

        }
        else {
            //I am dragging a Route or Position

            int i;

            //given that the drag is finished, I set to empty label_dragged_object for all ChartFrames
            for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                ((parent->parent->chart_frames[i])->draw_panel->label_dragged_object_now) = String("");
            }

#ifdef __APPLE__

            parent->parent->RefreshAll();

#endif

#ifdef WIN32


            for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                (parent->parent->chart_frames[i])->draw_panel->RefreshWIN32();
            }

#endif

            (parent->parent->dragging_object) = false;


            if (!(((((draw_panel_origin.x) + (position_plot_area_now.x) < (position_end_drag.x)) && ((position_end_drag.x) < (draw_panel_origin.x) + (position_plot_area_now.x) + (size_plot_area.GetWidth()))) &&
                (((draw_panel_origin.y) + (position_plot_area_now.y) < (position_end_drag.y)) && ((position_end_drag.y) < (draw_panel_origin.y) + (position_plot_area_now.y) + (size_plot_area.GetHeight())))))) {
                // drag_end_position lies out the plot area

                if ((parent->parent->highlighted_route_now) != -1) {
                    //I am dragging a Route: I restore the starting position of the route under consideration to its value at the beginning of the drag and re-tabulate the route points

                    (*(((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position)) = route_reference_position_drag_start;

                    TabulateRoutes();
                    Refresh();
                    FitAll();

                    print_error_message->SetAndCall(NULL, String("Error"), String("Route ground or start position outside plot area! Route start or start position must lie within the plot area."), (wxGetApp().path_file_error_icon));

                }

                if ((((parent->parent)->highlighted_position_now) != -1)) {
                    // I am dragging a Position: I restore the position under consideration to its value at the beginning of the drag

                    //convert the coordinates of position_start_drag into geographic coordinates, and assign these to the Position under consideration
                    (this->*ScreenToGeo)(position_start_drag, &((parent->parent->data->position_list)[((parent->parent)->highlighted_position_now)]));


                    //update the coordinates of the Position under consideration in listcontrol_positions
                    ((parent->parent->data->position_list)[((parent->parent)->highlighted_position_now)]).update_wxListCtrl(((parent->parent)->highlighted_position_now), (parent->parent)->listcontrol_positions);

                    //given that the position under consideration has changed, I re-pain the chart
                    Refresh();
                    FitAll();

                    print_error_message->SetAndCall(NULL, String("Error"), String("Position outside plot area! The position must lie within the plot area."), (wxGetApp().path_file_error_icon));

                }

            }

        }

    }
    else {
        //the left button of the mouse has not been lifted at the end of a drag

        //if, when the left button of the mouse was down, the mouse was hovering over a Position, then this position is selectd in listcontrol_positions and highlighted in color
        if (((parent->parent)->highlighted_position_now) != -1) {

            //deselect any previously selected item in listcontrol_positions, if any
            ((parent->parent)->listcontrol_positions)->DeselectAll();

            parent->parent->Raise();  // bring the ListFrame to front
            parent->parent->SetFocus();  // focus on the ListFrame

            //select the highlighted position in ListFrame
            ((parent->parent)->listcontrol_positions)->SetItemState((parent->parent)->highlighted_position_now, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

            //set the beckgorund color of the Position in listcontrol_positions in ListFrame to the color of selected items
            ((parent->parent)->listcontrol_positions)->SetItemBackgroundColour((parent->parent)->highlighted_position_now, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

        }

        //if, when the left button of the mouse was down, the mouse was hovering over a Route, then this Route and the related Sight (if any) is selectd in listcontrol_routes and listcontrol_sights, respectively, and highlighted in color
        if ((parent->parent->highlighted_route_now) != -1) {

            //deselect any previously selected item in listcontrol_routes, if any
            ((parent->parent)->listcontrol_routes)->DeselectAll();

            //deselect any previously selected item in listcontrol_sights, if any, to clear up things for the user and show only the selected Route in ListFrames
            ((parent->parent)->listcontrol_sights)->DeselectAll();


            parent->parent->Raise();  // bring the ListFrame to front
            parent->parent->SetFocus();  // focus on the ListFrame

            //select the highlighted Route in ListFrame
            ((parent->parent)->listcontrol_routes)->SetItemState((parent->parent)->highlighted_route_now, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

            //set the beckgorund color of the Route in listcontrol_routes in ListFrame to the color of selected items
            ((parent->parent)->listcontrol_routes)->SetItemBackgroundColour((parent->parent)->highlighted_route_now, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

            if ((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value != -1) {
                //the selected Route is related to a Sight



                //select the related Sight in ListFrame
                ((parent->parent)->listcontrol_sights)->SetItemState((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

                //set the beckgorund color of the related Sight in listcontrol_sights in ListFrame to the color of selected items
                ((parent->parent)->listcontrol_sights)->SetItemBackgroundColour((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

            }

        }


    }

    event.Skip(true);

}


void DrawPanel::OnMouseRightDown(wxMouseEvent& event) {
    
    stringstream s;
    
    //changes the 'sign' of selection rectangle
    (parent->parent->selection_rectangle) = (!(parent->parent->selection_rectangle));
    
    if ((parent->parent->selection_rectangle)) {
        //start drawing a selection rectangle
        
        int i;
        bool check;
        
        //disable all button_resets while a selection rectangle is being drawn
        for(i=0; i<parent->parent->chart_frames.size(); i++){
            parent->parent->chart_frames[i]->button_reset->Enable(false);
        }
        
        (parent->parent->geo_position_start) = (parent->parent->geo_position_now);
        //        GetMouseGeoPosition(&(parent->parent->geo_position_start));
        //        drawpanel_position_start = (parent->parent->screen_position_now);
        
        //store the position at the beginning of the selection process, to compute the zoom factor
        //run over all ChartFrames : if in the i-th ChartFrame geo_position_start falls within the plot area -> set label and position of the start position of selection_rectangle in that ChartFrame
        for (i = 0, check = false; i < (parent->parent->chart_frames).size(); i++) {
            
            if (
                (((parent->parent->chart_frames)[i])->draw_panel->*(((parent->parent->chart_frames)[i])->draw_panel->GeoToProjection))((parent->parent->geo_position_start), &(((parent->parent->chart_frames)[i])->draw_panel->projection_start), false)) {
                    //geo_position_start is valid in the i-th DrawPanel -> start the selection rectangle in the i-th DrawPanel
                    
                    ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                                                                                         (parent->parent->geo_position_now),
                                                                                         &(((parent->parent->chart_frames)[i])->draw_panel->position_start_label_selection_rectangle),
                                                                                         &(parent->parent->start_label_selection_rectangle));
                    
                    ((parent->parent->chart_frames)[i])->draw_panel->Refresh();
                    check = true;
                    
                }
            
            
        }
        
        
        if (!check) {
            //geo_position_start is invalid in all DrawPanels -> delete the selection rectangle by setting selection_rectangle to false
            
            (parent->parent->selection_rectangle) = false;
            
        }
        
        
    }
    else {
        //end drawing a selection rectangle
        
        GetMouseGeoPosition(&(parent->parent->position_end));
        drawpanel_position_end = (parent->parent->screen_position_now);
        
        unsigned int i;
        
        //store the position at the end of the selection process, to compute the zoom factor later
        if ((this->*ScreenToProjection)(drawpanel_position_end, &projection_end)) {
            //drawpanel_position_end is valid
            
            Angle lambda_a, lambda_b;
            
            
            //convert all the angles to the format between -pi and pi, so I can sort them numerically
            parent->parent->geo_position_start.phi.normalize_pm_pi();
            parent->parent->geo_position_start.lambda.normalize_pm_pi();
            parent->parent->position_end.phi.normalize_pm_pi();
            parent->parent->position_end.lambda.normalize_pm_pi();
            
            lambda_a = (parent->parent->geo_position_start.lambda);
            lambda_b = (parent->parent->position_end.lambda);
            
            switch (position_in_vector(Projection((parent->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using the Mercator projection
                    
                    if ((parent->ComputeZoomFactor_Mercator(fabs((projection_end.x) - (projection_start.x))))) {
                        //if the zoom factor of the map resulting from the selection is valid, I update x_min, ... , y_max
                        
                        
                        //in order to properly set lambda_min and lambda_max, I need to tell apart the following cases
                        if(GSL_SIGN((lambda_a.normalize_pm_pi_ret().value)) == GSL_SIGN(lambda_b.normalize_pm_pi_ret().value)){
                            //lambda_a and lambda_b lie in the same hemisphere
                            
                            if(lambda_b > lambda_a){
                                
                                (*(parent->lambda_min)) = lambda_b;
                                (*(parent->lambda_max)) = lambda_a;
                                
                            }else{
                                
                                (*(parent->lambda_min)) = lambda_a;
                                (*(parent->lambda_max)) = lambda_b;
                                
                            }
                            
                        }else{
                            //lambda_a and lambda_b lie in different hemispheres
                            
                            if(((lambda_a.normalize_pm_pi_ret().value) >= 0.0) && (lambda_b.normalize_pm_pi_ret().value) <= 0.0){
                                //lambda_a lies in the poisitive-logitude hemishere (0 < lambda < 180), lambda_b in the nevative-longitude hemisphere (180 < lambda < 360)
                                
                                if((parent->lambda_min->normalize_pm_pi_ret().value) > (parent->lambda_max->normalize_pm_pi_ret().value)){
                                    
                                    (*(parent->lambda_min)) = lambda_a;
                                    (*(parent->lambda_max)) = lambda_b;
                                    
                                }else{
                                    
                                    (*(parent->lambda_min)) = lambda_b;
                                    (*(parent->lambda_max)) = lambda_a;
                                    
                                }
                                
                            }else{
                                //lambda_a lies in the negative-logitude hemishere (180 < lambda < 360), lambda_b in the positive-longitude hemisphere (0 < lambda < 180)
                                
                                if((parent->lambda_min->normalize_pm_pi_ret().value) > (parent->lambda_max->normalize_pm_pi_ret().value)){
                                    
                                    (*(parent->lambda_min)) = lambda_b;
                                    (*(parent->lambda_max)) = lambda_a;
                                    
                                }else{
                                    
                                    (*(parent->lambda_min)) = lambda_a;
                                    (*(parent->lambda_max)) = lambda_b;
                                    
                                }
                                
                            }
                            
                        }
                        
                        
                        
                        if ((parent->parent->geo_position_start.phi) > ((parent->parent->position_end).phi)) {
                            (*(parent->phi_max)) = (((parent->parent)->geo_position_start).phi);
                            (*(parent->phi_min)) = (((parent->parent)->position_end).phi);
                        }else {
                            (*(parent->phi_min)) = (((parent->parent)->geo_position_start).phi);
                            (*(parent->phi_max)) = (((parent->parent)->position_end).phi);
                        }
                        //I normalize lambda_min, ..., phi_max for future use.
                        parent->lambda_min->normalize();
                        parent->lambda_max->normalize();
                        parent->phi_min->normalize();
                        parent->phi_max->normalize();
                        
                        parent->parent->geo_position_start.phi.normalize();
                        parent->parent->geo_position_start.lambda.normalize();
                        parent->parent->position_end.phi.normalize();
                        parent->parent->position_end.lambda.normalize();
                        
                        (this->*PreRender)();
                        parent->parent->RefreshAll();
                        FitAll();
                        
                        parent->UpdateSlider();
                        //the aspect ratio of ChartFrame may have changed -> call ShowChart to reposition everything properly on the screen
                        wxGetApp().ShowCharts(event);
                        
                    }
                    else {
                        //if the zoom factor is not valid, then I print an error message
                        
                        s.str("");
                        s << "Zoom level must be >= 1 and <= " << (wxGetApp().zoom_factor_max.value) << ".";
                        
                        //set the title and message for the functor print_error_message, and then call the functor
                        print_error_message->SetAndCall(NULL, String("Zoom level exceeded its maximal value!"), String(s.str().c_str()), (wxGetApp().path_file_error_icon));
                        
                    }
                    
                    
                    break;
                }
                    
                case 1: {
                    //I am using the 3d projection
                    
                    Length l1, l2;
                    Position reference_position_old;
                    
                    //store the current ground position of circle_observer into reference_position_old
                    reference_position_old = (circle_observer.reference_position);
                    
                    //normalize lambda_a/b and then compute the algebric mean -> this is the correct value of the longitude of circle_observer
                    circle_observer.reference_position.lambda.set((lambda_a.normalize_ret().value + lambda_b.normalize_ret().value)/2.0);
    
                    //normalize the two latitudes between -pi and pi and then compute the algebraic mean -> this is the correct value of the two latitudes
                    circle_observer.reference_position.phi.set(mean_pm_pi(parent->parent->geo_position_start.phi, parent->parent->position_end.phi));
                    
                    
                    //compute omega as half of  the largest angular distance between the middle of selection rectangle and its corners
                    circle_observer.reference_position.distance((parent->parent->geo_position_start), &l1, String(""), String(""));
                    circle_observer.reference_position.distance(Position(parent->parent->geo_position_start.lambda, parent->parent->position_end.phi), &l2, String(""), String(""));
                    circle_observer.omega.set(((max(l1, l2).value) / Re)/2.0);
                    
                    
                    //conpute the new rotation: the new rotation of the earth is the old one, composed with the rotation which brings the old reference_position onto the new one
                    //The coordinate transformation between a vector r in reference frame O and a vector r' in reference frame O' is r = (rotation^T).r', rotation . Rotation(circle_observer.reference_position, reference_position_old). (rotation^T) =   Rotation(circle_observer.reference_position, reference_position_old)' (i.e., Rotation(circle_observer.reference_position, reference_position_old) in reference frame O'), thus I set rotation = Rotation(circle_observer.reference_position, reference_position_old)' * rotation, and by simplifying I obtain
                    rotation.set((rotation * Rotation(circle_observer.reference_position, reference_position_old)));
                    
                    (this->*PreRender)();
                    parent->parent->RefreshAll();
                    FitAll();
                    
                    parent->UpdateSlider();
                    //the aspect ratio of ChartFrame may have changed -> call ShowChart to reposition everything properly on the screen
                    wxGetApp().ShowCharts(event);
                    
                    
                    break;
                }
                    
            }
            
            //set to empty the text fields of the geographical positions of the selek÷ction triangle, which is now useless
            
            (parent->parent->start_label_selection_rectangle) = String("");
            (parent->parent->end_label_selection_rectangle_now) = String("");
            
        }else {
            //the  end position for the selected rectangle is not valid -> cancel the rectangle by setting selection_rectangle to false and by setting to empty the text fields of the geographical positions of the selection triangle
            
            (parent->parent->selection_rectangle) = false;
            (parent->parent->start_label_selection_rectangle) = String("");
            (parent->parent->end_label_selection_rectangle_now) = String("");
            
        }
        
        //disable all button_resets while a selection rectangle is being drawn
        for(i=0; i<parent->parent->chart_frames.size(); i++){
            parent->parent->chart_frames[i]->button_reset->Enable(true);
        }
        
    }
    
    event.Skip(true);
    
}

//this function is called whenever mouse is dragged on *this
void DrawPanel::OnMouseDrag(wxMouseEvent& event) {
    
    if ((!idling) && (!(parent->idling)) && (!(parent->parent->idling))) {
        //I proceed only if this and its parent and the parent of its parent are not in idling mode
        
        if (wxGetMouseState().LeftIsDown()) {
            
            if (!mouse_dragging) {
                //the mouse has started dragging
                
                //If I am dragging a Route, I save the starting point of this Route into route_reference_position_drag_now
                
                //during the mouse drag, I disable DrawPanel::OnMouseMovement
                this->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, this);
                
                if ((parent->parent->highlighted_route_now) != -1) {
                    //set route_reference_position_drag_now to the start Position (if the route is a loxodrome / orthodrome) or to the ground Position (if the route is a circle of equal altitutde)
                    
                    if ((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).type) == (Route_types[2])) {
                        
                        route_reference_position_drag_start = (*(((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position));
                        
                        if (((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value) != -1) {
                            //here I am dragging a circle of equal altitude originally related to a sight. After dragging, this circle of equal altitude no longer results from that sight, thus I disconnect the sight and the circle of equal altitude, and update the wxListCtrs in parent->parent accordingly
                            
                            (parent->parent->i_object_to_disconnect) = ((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value);
                            
                            parent->parent->DisconnectAndPromptMessage(event);
                            
                        }
                        
                    }
                    else {
                        
                        route_reference_position_drag_start = (*(((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position));
                        
                    }
                    
                    
                }
                
            }
            
            mouse_dragging = true;
            
            SetCursor(wxCURSOR_HAND);
            
            position_now_drag = wxGetMousePosition();
            
            
            if ((this->*ScreenToGeo)(position_now_drag, NULL)) {
                //position_drag_now is a valid Position
                
                if ((((parent->parent->highlighted_route_now) == -1) && ((parent->parent->highlighted_position_now) == -1))) {
                    //the whole chart is being dragged (the mouse is not over a Route nor a Position while dragging)
                    
                    (parent->dragging_chart) = true;
                    
                    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                        //I am using the mercator projection
                        
                        PositionProjection p_ceil_min, p_floor_max;
                        
                        (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
                        (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);
                        
                        
                        if ((y_max_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_max_start_drag) < (p_floor_max.y)) && (y_min_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min_start_drag) > (p_ceil_min.y))) {
                            //in this case, the drag operation does not end out of the min and max latitude contained in the data files
                            
                            //update x_min, ..., y_max according to the drag.
                            x_min = x_min_start_drag - ((double)((position_now_drag.x) - (position_start_drag.x))) / ((double)(size_plot_area.GetWidth())) * x_span_start_drag;
                            x_max = x_max_start_drag - ((double)((position_now_drag.x) - (position_start_drag.x))) / ((double)(size_plot_area.GetWidth())) * x_span_start_drag;
                            y_min = y_min_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max_start_drag - y_min_start_drag);
                            y_max = y_max_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max_start_drag - y_min_start_drag);
                            
                            if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                                (this->*Set_lambda_phi_min_max)();
                            }
                            
                            
#ifdef __APPLE__
                            //re-draw the chart
                            (this->*PreRender)();
#endif
#ifdef WIN32
                            //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
                            (parent->polygon_position_before) = (parent->polygon_position_now);
//                            parent->coastline_polygons_before.resize(parent->coastline_polygons_now.size());
                            copy_n(parent->coastline_polygons_now.begin(), parent->coastline_polygons_now.size(), parent->coastline_polygons_before.begin() );

                            position_plot_area_before = position_plot_area_now;
                            grid_before.clear();
                            grid_before = grid_now;
                            ticks_before.clear();
                            ticks_before = ticks_now;
                            
                            //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
                            points_route_list_before.clear();
                            points_route_list_before = points_route_list_now;
                            
                            points_position_list_before.clear();
                            points_position_list_before = points_position_list_now;
                            
                            reference_positions_route_list_before.clear();
                            reference_positions_route_list_before = reference_positions_route_list_now;
                            
                            //re-draw the chart
                            (this->*PreRender)();
                            
#endif
                            MyRefresh();
                            //                            FitAll();
                            
                        }
                        
                    }
                    
                    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
                        //I am using the 3d projection
                        
                        //compose rotation_start_drag with the rotation resulting from the drag, so as to rotate the entire earth according to the mouse drag
                        rotation.set(rotation_start_end(position_start_drag, position_now_drag) * rotation_start_drag);
#ifdef __APPLE__
                        
                        //re-render the chart
                        (this->*PreRender)();
#endif
#ifdef WIN32
                        //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
                        (parent->polygon_position_before) = (parent->polygon_position_now);
//                        parent->coastline_polygons_before.resize(parent->coastline_polygons_now.size());
                        copy_n(parent->coastline_polygons_now.begin(), parent->coastline_polygons_now.size(), parent->coastline_polygons_before.begin() );

                        position_plot_area_before = position_plot_area_now;
                        grid_before.clear();
                        grid_before = grid_now;
                        ticks_before.clear();
                        ticks_before = ticks_now;
                        
                        //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
                        points_route_list_before.clear();
                        points_route_list_before = points_route_list_now;
                        
                        points_position_list_before.clear();
                        points_position_list_before = points_position_list_now;
                        
                        reference_positions_route_list_before.clear();
                        reference_positions_route_list_before = reference_positions_route_list_now;
                        
                        //re-draw the chart
                        (this->*PreRender)();
                        
#endif
                        MyRefresh();
                        
                    }
                    
                    
                }
                else {
                    //an object is being dragged (a Position or a Route)
                    
                    unsigned int i;
                    
                    (parent->parent->dragging_object) = true;
                    
                    //the data in the file are being modified -> I call
                    parent->parent->OnModifyFile();
                    
                    if ((parent->parent->highlighted_route_now) != -1) {
                        //a Route is being dragged
                        
                        wxPoint q;
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                            
                            wxPoint p;
                            
                            //convert the coordinates of route_reference_position_drag_now into DrawPanel coordinates, shift these coordinates according to the mouse drag, and  assign the resulting point to the starting (ground) Position of the Route under consideration if the Route is a loxodrome or orthodrome (circle of equal altitude): in this way, the whole Route under consideration is dragged along with the mouse
                            
                            GeoToDrawPanel(route_reference_position_drag_start, &p, false);
                            
                            //this command is the same for all types of Routes
                            DrawPanelToGeo(p + (position_now_drag - position_start_drag), &(((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position));
                            
                        }
                        
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
                            
                            //compose rotation with the rotation resulting from the drag and then apply it to route_reference_position_drag_now: route_reference_position_drag_now -> rotation^{-1}.(rotation due to drag).rotation.route_reference_position_drag_now. In this way, when Render() will plot the position route_reference_position_drag_now, it will apply to route_reference_position_drag_now the global rotation  'rotation' again, and the result will be rotation . rotation^{-1}.(rotation due to drag).rotation.route_reference_position_drag_now = (rotation due to drag).rotation.route_reference_position_drag_now, which is the desired result (i.e. route_reference_position_drag_now rotated by the global rotation 'rotation', and then rotated by the rotation due to the drag)
                            rotation_now_drag.set(
                            (rotation.inverse()) *
                            rotation_start_end(position_start_drag, position_now_drag) *
                            rotation);
                            
                            //                    (this->*GeoToDrawPanel)(route_reference_position_drag_now, &p);
                            
                            if ((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).type) == (Route_types[2])) {
                                
                                //                        DrawPanelToGeo(p + (position_now_drag - position_start_drag), &(((parent->parent->data->route_list)[(parent->parent->highlighted_route)]).reference_position));
                                route_reference_position_drag_start.rotate(String(""), rotation_now_drag, &(((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position), String(""));
                                
                            }
                            else {
                                
                                route_reference_position_drag_start.rotate(String(""), rotation_now_drag, &(((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position), String(""));
                                
                            }
                            
                        }
                        
                        
                        
                        
                        //update the data of the Route under consideration in listcontrol_routes
                        ((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).update_wxListCtrl((parent->parent->highlighted_route_now), parent->parent->listcontrol_routes);
                        
                        
                        for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                            //on APPLE, I compute the coordinates of the reference position of the Route that is being dragged and I call Refresh(), because Refresh() is fast. On WIN32 Refresh() is slow -> I use the RefreshWIN32() method, which wipes out graphical objects at the preceeding instant of time by drawing on them with color wxGetApp().background_color, and then renders the objects at the present instant of time with color wxGetApp().foreground_color
                            
#ifdef _WIN32
                            
                            //store the string with the coordinated of the object that is being dragged into label_dragged_position and its position into position_label_dragged_position, so PaintEvent will read it and draw the label of its coordinates on it
                            (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now);
                            (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now);
                            
#endif
                            
                            //obtain the coordinates of the reference position of the Route that is being dragged
                            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                                                                                                 ((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position,
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now),
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now)
                                                                                                 );
                            
#ifdef __APPLE__
                            
                            //given that the Route under consideration has changed, I re-tabulate the Routes and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulateRoutes();
                            
#endif
#ifdef _WIN32
                            
                            //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before, for all DrawPanels
                            ((parent->parent->chart_frames)[i])->draw_panel->points_route_list_before.clear();
                            (((parent->parent->chart_frames)[i])->draw_panel->points_route_list_before) = (((parent->parent->chart_frames)[i])->draw_panel->points_route_list_now);
                            
                            ((parent->parent->chart_frames)[i])->draw_panel->reference_positions_route_list_before.clear();
                            (((parent->parent->chart_frames)[i])->draw_panel->reference_positions_route_list_before) = (((parent->parent->chart_frames)[i])->draw_panel->reference_positions_route_list_now);
                            
                            
                            //given that the Route under consideration has changed, I re-tabulate the Routes and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulateRoutes();
                            
#endif
                            
                            ((parent->parent->chart_frames)[i])->draw_panel->MyRefresh();
                            
                            
                        }
                        
                    }
                    
                    if ((parent->parent->highlighted_position_now) != -1) {
                        //a Position is being dragged
                        
                        wxPoint p;
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                            
                            //convert the coordinates of position_now_drag into geographic coordinates, and assign these to the Position under consideration: in this way, the Position under consideration is dragged along with the mouse
                            (this->*ScreenToGeo)(position_now_drag, &(((parent->parent->data)->position_list)[(parent->parent->highlighted_position_now)]));
                            
                        }
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
                            
                            //compose rotation with the rotation resulting from the drag and then apply it to pp == &(((parent->parent->data)->position_list)[(parent->parent->highlighted_position_now)]): pp -> rotation^{-1}.(rotation due to drag).rotation.pp. In this way, when Render() will plot the position pp, it will apply to pp the global rotation  'rotation' again, and the result will be rotation . rotation^{-1}.(rotation due to drag).rotation.pp = (rotation due to drag).rotation.pp, which is the desired result (i.e. pp rotated by the global rotation 'rotation', and then rotated by the rotation due to the drag)
                            rotation_now_drag =
                            (rotation.inverse()) *
                            rotation_start_end(position_start_drag, position_now_drag) *
                            rotation;
                            geo_start_drag.rotate(String(""), rotation_now_drag, &((parent->parent->data->position_list)[(parent->parent->highlighted_position_now)]), String(""));
                            
                        }
                        
                        //update the data of the Position under consideration in listcontrol_positions
                        ((parent->parent->data->position_list)[(parent->parent->highlighted_position_now)]).update_wxListCtrl((parent->parent->highlighted_position_now), parent->parent->listcontrol_positions);
                        
                        //given that the Position under consideration has changed, I re-paint the charts
                        for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                            //on APPLE, I compute the coordinates of the Position that is being dragged and I call Refresh(), because Refresh() is fast. On WIN32 Refresh() is slow ->  I use the RefreshWIN32() method, which wipes out graphical objects at the preceeding instant of time by drawing on them with color wxGetApp().background_color, and then renders the objects at the present instant of time with color wxGetApp().foreground_color
                            
                            
#ifdef _WIN32
                            
                            //store the string with the coordinated of the object that is being dragged into label_dragged_position and its position into position_label_dragged_position, so PaintEvent will read it and draw the label of its coordinates on it
                            (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now);
                            (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now);
                            
                            
#endif
                            //obtain the coordinates of the reference position of the Route that is being dragged
                            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                                                                                                 (parent->parent->data->position_list)[(parent->parent->highlighted_position_now)],
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now),
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now)
                                                                                                 );
                            
#ifdef __APPLE__
                            
                            //given that the Positions under consideration has changed, I re-tabulate the Positions and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulatePositions();
                            
#endif
#ifdef _WIN32
                            
                            ((parent->parent->chart_frames)[i])->draw_panel->points_position_list_before.clear();
                            (((parent->parent->chart_frames)[i])->draw_panel->points_position_list_before) = (((parent->parent->chart_frames)[i])->draw_panel->points_position_list_now);
                            
                            //given that the Positions under consideration has changed, I re-tabulate the Positions and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulatePositions();
                            
#endif
                            (((parent->parent->chart_frames)[i])->draw_panel)->MyRefresh();
                            
                            
                        }
                        
                    }
                    
                }
                
            }
            else {
                //in this case, position_drag_now is not a valid position : in the Mercator projection, this does not make sense and I do nothing. In the 3D projection, I am dragging the chart from outside circle observer (I am rotating the earth) -> I proceed implementing this rotation
                
                switch (position_in_vector(Projection((parent->projection->name->GetValue().ToStdString())), Projection_types)) {
                        
                    case 0: {
                        //I am using the mercator projection: then the position is invalid and I may print an error message
                        
                        //uncomment this if you want an info message to be printed
                        //print_error_message->SetAndCall(NULL,  String("The drag goes through an invalid point!"), String("The drag must go through valid points."));
                        
                        break;
                        
                    }
                        
                    case 1: {
                        //I am using the 3d projection: even if the Position lies outside the circular boundary of the Earth,  thus this Position is a valid position for a drag which rotates the earth about the y' axis -> I do this rotation
                        
                        (parent->dragging_chart) = true;
                        
                        //compose rotation_start_drag with the rotation resulting from the drag, so as to rotate the entire earth according to the mouse drag
                        rotation.set(rotation_start_end(position_start_drag, position_now_drag) * rotation_start_drag);
                        
                        
                        
#ifdef __APPLE__
                        //re-draw the chart
                        (this->*PreRender)();
#endif
#ifdef _WIN32
                        //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
                        (parent->polygon_position_before) = (parent->polygon_position_now);
//                        parent->coastline_polygons_before.resize(parent->coastline_polygons_now.size());
                        copy_n(parent->coastline_polygons_now.begin(), parent->coastline_polygons_now.size(), parent->coastline_polygons_before.begin() );
                        
                        position_plot_area_before = position_plot_area_now;
                        grid_before.clear();
                        grid_before = grid_now;
                        ticks_before.clear();
                        ticks_before = ticks_now;
                        
                        //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
                        points_route_list_before.clear();
                        points_route_list_before = points_route_list_now;
                        
                        points_position_list_before.clear();
                        points_position_list_before = points_position_list_now;
                        
                        reference_positions_route_list_before.clear();
                        reference_positions_route_list_before = reference_positions_route_list_now;
                        
                        //re-draw the chart
                        (this->*PreRender)();
                        
#endif
                        MyRefresh();
                        
                        FitAll();
                        
                        break;
                        
                    }
                        
                        
                }
                
            }
            
        }
        
    }
    
    event.Skip(true);
    
}


//this function is called whenever mouse wheel is turned on *this
void DrawPanel::OnMouseWheel(wxMouseEvent& event) {

    int i, j;

    j = (event.GetWheelRotation());

    //    cout << "\n\n\nWheel rotation = " << event.GetWheelRotation() << "\n";
    //    cout << "Slider value old = " << ((parent->slider)->GetValue()) << "\n";
    //    //    cout << "Zoom factor max = " << ((wxGetApp().zoom_factor_max).value) << "\n";
    //    cout << "A = " << (-1 + ((parent->slider)->GetValue())) << "\n";
    //    cout << "B = " << ((double)j)/(event.GetWheelDelta()) << "\n";
    //    cout << "(int)(A*B) = " << ((int)((-1.0 + ((parent->slider)->GetValue())) * ((double)j)/(event.GetWheelDelta()))) << "\n";
    
    i = ((int)((parent->slider)->GetValue())) /*I put the minus sign here because a zoom-in scroll with the mouse wheel corresponds to a negative j*/ - j;

    //    cout << "Slider value new = " << i << "\n";

    //if i gets out of range, put it back in the correct range
    if (i < 1) { i = 1; }
    if (i > ((parent->slider)->GetMax())) { i = ((parent->slider)->GetMax()); }

    parent->SetSlider(i);

    event.Skip(true);

}

