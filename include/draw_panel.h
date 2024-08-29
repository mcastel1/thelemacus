//
//  draw_panel.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef draw_panel_h
#define draw_panel_h

#include <iostream>

#include <wx/gdicmn.h>
#include <wx/mstream.h>

#include "cartesian.h"
#include "chart_frame.h"
#include "chart_panel.h"
#include "int.h"
#include "length.h"
#include "lines.h"
#include "my_app.h"
#include "position.h"
#include "position_projection.h"
#include "position_rectangle.h"
#include "print_message.h"
#include "route.h"
#include "rotation.h"
#include "unset_idling.h"


using namespace std;


class Cartesian;
class ChartFrame;
class ChartPanel;
class Position;
class PositionRectangle;
class Rotation;

class DrawPanel : public wxPanel{
    
public:
    
    ChartFrame* parent;
    PrintMessage<DrawPanel, UnsetIdling<DrawPanel> >* print_error_message;
    wxPoint draw_panel_origin, position_plot_area_now/*, position_plot_area_before*/, drawpanel_position_end, position_start_drag, position_end_drag, /*the positions where the will be placed the labels start_label_selection_rectangle, end_label_selection_rectangle_now of the start and end point of selection_rectangle. position_end_label_selection_rectangle_now is the position of the end point of the selection rectangle now, while position_end_label_selection_rectangle_before is the position of the end point of selection rectangle at the preceeding mouse position*/ position_start_label_selection_rectangle, position_end_label_selection_rectangle_now/*, position_end_label_selection_rectangle_before*/, position_now_drag, position_draw_panel_now, /*the position of label_position_now*/ position_label_position_now, /*the position of label_dragged_object at the current step of the drag process and at the preceeding step of the drag process, respectively*/ position_label_dragged_object_now/*, position_label_dragged_object_before*/;

    //size, in pixels, of vertical and horizontal labels
    unsigned int size_label_vertical, size_label_horizontal;
    /*x_min, x_max, y_min, y_max do correspond to lambda_min, lambda_max, etc... They are ordered in such a way that x_min <= x_max and y_min <= y_max always. */
    double /*min and max values of the Mercator or 3D projections x, y*/x_min, x_max, y_min, y_max, x_min_start_drag, x_max_start_drag, y_min_start_drag, y_max_start_drag, x_span_start_drag,
    /*these are the values of x_min .. y_max and x_span when the plot is first drawn*/
    x_min_0, x_max_0, y_min_0, y_max_0, x_span_0,
    /*the coordinates of the center of the projection when scrolling starts*/ x_center_scrolling, y_center_scrolling,
    /*these are the values of x_min, ... y_max after each sliding event, corresponding to lambda_min, ... , phi_max read from file*/x_min_old, x_max_old, y_min_old, y_max_old, /*these are the angular separations in latitude and longitude between meridians and parallels, respectively */delta_lambda, delta_phi,     /*this is a quantity resulting from the 3D projection: if and only if it is >= 0, then the point under consideration in the 3d projeciton lies on the visible surface of the earth */ arg_sqrt, /*increments in longitude/latitude to draw minor ticks*/delta_lambda_minor, delta_phi_minor, lambda_span, phi_span;
    //the precision used to print out labels
    Int label_precision;
    
    //the euler angles which specify the orientation of the earth for the 3d representation
    //two auxiliary vectors which will be used later
    Cartesian *r, /*vector position in the x'y'z' reference frame used for multiple purposes*/*rp;
    Rotation /*the orientation of the Earth at the beginning / current time / end of a drag*/*rotation_start_drag, *rotation_now_drag, *rotation_end_drag, /*the rotation representing the current / initial orientation of the earth*/*rotation, *rotation_0;
    Double /*if the mouse hovers over a route and its y coordinate is equal to the y of the route +- (length sceen) * thickness_route_selection_over_length_screen /2, then the relative Route is highlighted in ListFrame*/ thickness_route_selection_over_length_screen;
    String /*this is used to display on the chart the coordinates of a Position that is being dragged or of the reference_position of a Route that is being dragged at the current step of the drag process (label_dragged_object_now) or at the preceeding step (label_dragged_object_before)*/ label_dragged_object_now/*, label_dragged_object_before*/, /*text showing the coordinates of the current mouse position on draw_panel*/ label_position_now/*, label_position_before*/;
    Length /*distance between the plane of the 2d projection and the eye of the observer for the 3d plot*/ *d;

    bool /*this is true if the mouse is dragging with the left button pressed*/mouse_dragging, idling, /*if re_draw = true (false), then one has to draw the non-highglighteable stuff in DrawPanel (coastlines, paralles, meridians ...  but not Routes nor Positions)*/re_draw, /*this is true if the current mouse position lies in the plot area, false otherwise*/mouse_in_plot_area;
    Position /*the starting Position (ground position) of a Route if the Route is a loxodrome or orthodrome (circle of equal altitude) that I want to drag, at the beginning of the drag process (route_reference_position_drag_start), at the preceeding step of the drag process (route_reference_position_drag_start) and at the current step of the drag process (route_reference_position_drag_now) */*route_reference_position_drag_start/*, *route_reference_position_drag_before*/, *route_reference_position_drag_now, /*starting and ending geographic position in a mouse drag process*/  *geo_start_drag, *geo_end_drag, /*the position on the sphere such that the vector between the center of the sphere and the position equals the direction of the rotation axis relative to a mouse drag*/*rotation_axis;
    Angle rotation_angle, /*an angle containing the middle longitude/latitude of the current 3D projection, rounded up to the closest value which is a multiple of delta_lambda/phi, used for drawing things in the middle of the projection*/lambda_middle, phi_middle, /*lambda/phi_start/end are the start/end values of longidue/latitude adapted in the right form ro the loopws which draw meridians/parallels*/ lambda_start, lambda_end, phi_start, phi_end, lambda_saved, phi_saved, Z_saved;
    PositionProjection /*the values of (x, y) at the beginning/end of the selection process with a rectangle*/*projection_start, *projection_end;
    Route /*this is a circle of equal altitude which is the intersection between the earth's surface and the visibility cone of the observer, whose vertex is at the observer. circle_oberserver.omega is the aperture angle at the center of the earth which specifies the part of the earth surface visible by the observer. circle_observer.rerefence_position is the ground position of the observer: the intersection between the line between the center of the earth and the observer, and the surface of the earth. These exist for the 3D projection only*/*circle_observer;
    
    PositionRectangle /* the rectangle x_min ... y_max in the Mercator projection*/*rectangle_observer;
    wxBitmap m_bgbuffer;
    
    wxSizer* sizer_h, *sizer_v;
    //the i-th element of point_route_list is a Lines object: this Lines object contains are as many elements as the number of connected curves in which the Route is cut (because of the meridian lambda = pi). In each of these elements there are the points of the Route chunk with respect to the origin of DrawPanel
    vector<Lines> routes_lines;
    //I store in reference_positions_route_list_now (reference_positions_route_list_before) the coordinates, with respect to the origin of DrawPanel, of the reference positions of the Routes at the current (preceeding) step of a drag process
    vector<wxPoint> reference_positions_route_list_now/*, reference_positions_route_list_before*/, points_position_list_now/*, points_position_list_before*/;
    //the i-th element of this vector contains a list of critical values of the parametric angle (t) of the i-th route. At these critical values, route #i crosses the meridian lambda = pi
    //the chart contains the plot area, and the following quantities are the width and height of chart and plot area
    wxSize size_chart, size_plot_area;
    unsigned int  /*these are the values of width/height_chart when the chart is first drawn*/width_chart_0, height_chart_0, tick_length, /* gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes when setting the ticks on the x axis, and similarly for gamma_phi*/gamma_lambda, gamma_phi;
    //this is true if the label which is being drawn is the first among the parallel/meridian labels, false otherwise
    bool first_label;
    wxMemoryInputStream * memory_input_stream;
    UnsetIdling<DrawPanel>* unset_idling;
    //labels of parallels and meridians at the current and preceeding chart configuration, respectively
    vector<wxString> parallels_and_meridians_labels_now/*, parallels_and_meridians_labels_before*/;
    //positions of labels of parallels and meridians at the current and preceeding chart configuration, respectively
    vector<wxPoint> positions_parallels_and_meridians_labels_now/*, positions_parallels_and_meridians_labels_before*/;
    //    vector<wxPoint> ticks_now/*, ticks_before*/;
    
    //this is a pointer to a class-member function which takes a void and returns a void. I will let it point to wither DrawPanel::PreRenderMercator or DrawPanel::PreRender3D, according to my needs, and similarly for the other pointers
    void (DrawPanel::*PreRender)(void);
    bool (DrawPanel::*ScreenToProjection)(const wxPoint&, PositionProjection*);
    bool (DrawPanel::*CartesianToProjection)(const Cartesian&, PositionProjection*, bool);
    bool (DrawPanel::*ScreenToGeo)(const wxPoint&, Position*);
    bool (DrawPanel::*GeoToProjection)(const Position&, PositionProjection*, bool);
    void (DrawPanel::*Render)(wxDC*,
                              const wxPoint&,
                              const Lines&,
                              const vector<wxString>&,
                              const vector<wxPoint>&,
                              const Lines&,
                              const wxColor&,
                              const wxColor&,
                              const double&);
    bool (DrawPanel::*ProjectionToDrawPanel)(PositionProjection&, wxPoint*, bool);
    void (DrawPanel::*ProjectionToGeo)(const PositionProjection&, Position*);
    void (DrawPanel::*Set_x_y_min_max)(void);
    void (DrawPanel::*Set_lambda_phi_min_max)(void);
    void (DrawPanel::*Set_size_chart)(void);
    
    DrawPanel(ChartPanel*, const wxPoint& position_in, const wxSize& size_in);

    void SetIdling(bool);
    void PreRenderMercator(void);
    void PreRender3D(void);
    void PaintEvent(wxPaintEvent&);
    void RenderAll(wxDC&);
    void MyRefresh(void);
    void RefreshWIN32(void);
    void RenderLines(wxDC*,
                        const Lines&,
                        const wxColor&,
                        const wxColor&,
                        const double&
                        );
    void RenderRoutes(wxDC&,
                      const vector< vector< vector<wxPoint> > >&,
                      const vector<wxPoint>&,
                      int,
                      const wxColor&
                      );
    void RenderPositions(
                         wxDC&,
                         const vector<wxPoint>&,
                         int,
                         const wxColor&);
    void RenderMousePositionLabel(wxDC&, const String&, const wxPoint&, const wxColor&, const wxColor&);
    void RenderDraggedObjectLabel(wxDC&, const wxPoint&, const String&, const wxColor&, const wxColor&);
    void RenderSelectionRectangle(wxDC&, const wxPoint&, const wxPoint&, const String&, const wxColor&, const wxColor&);
    void RenderSelectionRectangle(wxDC&, const Position&, const wxPoint&, const String&, const wxColor&, const wxColor&);
    void CleanAndRenderAll(void);
    void TabulateRoutes(void);
    void TabulatePositions(void);
    void FitAll();
    
    bool AdjustLatitudeLongitude3D(const int&, const int&, int*, int*);
    bool ScreenToDrawPanel(const wxPoint&, wxPoint*);
    void GeoToScreen(const Position&, wxPoint*);
    bool DrawPanelToGeo(const wxPoint&, Position*);
    bool ScreenToMercator(const wxPoint&, PositionProjection*);
    bool ScreenTo3D(const wxPoint&, PositionProjection*);
    bool GeoToDrawPanel(const Position&, wxPoint*, bool);
    bool CartesianToDrawPanel(const Cartesian&, wxPoint*, bool);
    bool GeoTo3D(const Position&, PositionProjection*, bool);
    bool CartesianTo3D(const Cartesian&, PositionProjection*, bool);
    bool CartesianToMercator(const Cartesian&, PositionProjection*, bool);
    bool GeoToMercator(const Position&, PositionProjection*, bool);
    bool ScreenToGeo_Mercator(const wxPoint&, Position*);
    bool ScreenToGeo_3D(const wxPoint&, Position*);
    bool ProjectionToDrawPanel_Mercator(PositionProjection&, wxPoint*, bool);
    bool ProjectionToDrawPanel_3D(PositionProjection&, wxPoint*, bool);
    void ProjectionToGeo_Mercator(const PositionProjection&, Position*);
    void ProjectionToGeo_3D(const PositionProjection&, Position*);
    void ShowCoordinates(const Position&, String*);
    void SetLabelAndPosition(const wxPoint&, wxPoint*, String*);
    void SetLabelAndPosition(const Position&, wxPoint*, String*);
    void SetLabelAndAdjustPosition(const Position&, wxPoint*, String*);
    void Set_lambda_phi_min_max_Mercator(void);
    void Set_lambda_phi_min_max_3D(void);
    void Set_x_y_min_max_Mercator(void);
    void Set_x_y_min_max_3D(void);
    void Set_size_chart_Mercator(void);
    void Set_size_chart_3D(void);
    bool PutBackIn(wxPoint, wxPoint*);
    double x_span(void);
    Rotation rotation_start_end(const wxPoint&, const wxPoint&);

    void RenderMercator(wxDC*,
                         const wxPoint&,
                         const Lines&,
                         const vector<wxString>&,
                         const vector<wxPoint>&,
                         const Lines&,
                         const wxColor&,
                         const wxColor&,
                         const double&);
    void Render3D(wxDC*,
                   const wxPoint&,
                   const Lines&,
                   const vector<wxString>&,
                   const vector<wxPoint>&,
                   const Lines&,
                   const wxColor&,
                   const wxColor&,
                   const double&);
    void WriteLabel(const Position&, Angle, Angle, Int, String, wxString*);
    void DrawLabel(const Position&, Angle, Angle, Int, String);
    
    bool GetMouseGeoPosition(Position*);
    void OnMouseMovement(wxMouseEvent&);
    void OnMouseLeftDown(wxMouseEvent&);
    void OnMouseLeftUp(wxMouseEvent&);
    void OnMouseRightDown(wxMouseEvent&);
    void OnMouseDrag(wxMouseEvent&);
    void OnMouseWheel(wxMouseEvent&);
    template<class E> void OnChooseProjection(E&);
    template<class E> void SetProjection(E&);
    void KeyDown(wxKeyEvent&);
    void ScaleFactor();
    
//    DECLARE_EVENT_TABLE()
};


#endif
