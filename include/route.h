//
//  route.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef route_h
#define route_h

#include <iostream>

#include "angle.h"
#include "color.h"
#include "constants.h"
#include "draw_panel.h"
#include "length.h"
#include "length_format.h"
#include "lines.h"
#include "my_string.h"
#include "position.h"
#include "position_projection.h"
#include "position_rectangle.h"
#include "route_type.h"
#include "speed.h"


using namespace std;

class DrawPanel;
class Length;
class Position;
class PositionRectangle;
class Speed;
class String;


//if type = l or o, the parameters specifying the route are reference_position (which in this case is the start position of the Route), alpha, l. if type = c, the parameters specifying the route are reference_position (which in this case is the GP of the circle of equal altitude) and omega.
class Route{
    
public:
    
    RouteType type;
    String label, temp_prefix;
    //the length of the Route is expressed as the length l (if length_format.value == "length"), or as the product of the Speed v and the time t (if length_format.value == "time and speed")
    LengthFormat length_format;
    //if type = l or o -> reference_position = start position, if type = c -> reference_position = ground position
    Position *reference_position, *end;
    //alpha: the angle that the vector tangent to the route describes with the local meridian at start; omega: the aperture angle of the cone for circles of equal altitude
    Angle /*this is equal to alpha_notes: it is the azimuth of the vector tangent to the Route at reference_position*/Z, /*the aprture angle, valid only if *this is a circle of equal altitude*/omega;
    //the length of the route
  
    Length *length;
    Speed *speed;
    Chrono time;
    //this is the position in sight_list of the sight linked to route. If there is no sight linked to route, then related_sight = -1.
    Int related_sight;
    //a list of points containing the geo coordinates of points on the Route this
    //    Position* points;
    
    Route(void);
    Route(RouteType, Position, Angle, Length);
    Route(RouteType, Position, Angle, Chrono, Speed);
    Route(const RouteType&, Position, Position);
    Route(RouteType, Position, Angle);
    
    void print(String, String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    void set(const Route&);
    void set_length_from_time_speed(void);
    void set_length_from_input(double);
    void compute_end(String);
    bool compute_end(Length, String);
    int crossing(Route, vector<Position>*, double*, String);
    static double lambda_minus_pi(double, void*);
    int inclusion(Route, bool, vector<Angle>*, String);
    int inclusion(PositionRectangle, bool, vector<Angle>*, String);
    int intersection(Route, bool, vector<Angle>*, String);
    bool closest_point_to(Position*, Angle*, Position, String);
    void size_Mercator(PositionProjection*);
    void add_to_ListControl(long, wxListCtrl*);
    void update_ListControl(long, wxListCtrl*);
    
    void DrawOld(unsigned int, Color, int, wxDC*, DrawPanel*);
    void DrawOld(unsigned int, wxDC*, DrawPanel*, String);
    void DrawOld(unsigned int, DrawPanel*, vector< vector<wxPoint> >*, String);
    void DrawOld(unsigned int, DrawPanel*, Lines*, String prefix);

    void Draw(unsigned int, Color, Color, int, wxDC*, DrawPanel*, String);
    void Draw(unsigned int, DrawPanel*, Lines*, String prefix);
    
    void lambda_min_max(Angle*, Angle*, String);
    bool phi_min_max(Angle*, Angle*, String);
    void compute_l_ends(vector<Length>*, bool*, DrawPanel*, String);
    
};


#endif
