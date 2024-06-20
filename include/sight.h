//
//  sight.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef sight_h
#define sight_h

#include <iostream>
#include <vector>

#include <wx/listctrl.h>

#include "angle.h"
#include "answer.h"
#include "atmosphere.h"
#include "body.h"
#include "int.h"
#include "length.h"
#include "limb.h"
#include "my_time.h"
#include "route.h"

using namespace std;

class Atmosphere;
class Body;
class Route;


class Sight{
    
public:
    Time master_clock_date_and_hour, /*this is the time of the observation, in the TAI time scale*/time;
    //stopwatch is the reading [hh:mm:ss.s] on the stopwatch
    Chrono TAI_minus_UTC, stopwatch;
    Angle index_error, H_s, H_a, H_o, H_i, DH_refraction, DH_dip, DH_parallax_and_limb;
    Length *r, *height_of_eye;
    Atmosphere *atmosphere;
    Body *body;
    Limb limb;
    // use_stopwatch = 'n' -> time is in format "UTC" time. use_stopwatch  = 'y' -> master clock UTC time + stopwatch reading
    Answer artificial_horizon, use_stopwatch;
    //label to add a note about the sight
    String label;
    //this is the position in route_list of the route linked to Sight. If there is no route linked to Sight, then related_route = -1.
    Int related_route;
    //all_items is a list of all the possible items which are part of a Sight object (master_clock_date_and_hour, body, ...). items is the list of items specific to a given Sight object: items may not include all the elements of all_items
    vector<String> items, all_items;
    
    
    Sight();
    static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
    bool get_coordinates(Route*, String);
    void compute_DH_dip(String);
    bool compute_DH_refraction(String);
    void compute_DH_parallax_and_limb(String);
    
    void compute_H_a(String);
    bool compute_H_o(String);
    
    void print(String, String, ostream&);
    template<class S> bool read_from_stream(String, S*, bool, String);
    bool reduce(Route*, String);
    bool check_time_interval(String);
    
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    
};



#endif
