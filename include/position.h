//
//  position.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef position_h
#define position_h

#include <wx/listctrl.h>

#include "length.h"
#include "angle.h"
#include "cartesian.h"
#include "draw_panel.h"
#include "rotation.h"

class DrawPanel;
class Cartesian;
class Rotation;

//a geographic Position
class Position{
    
public:
    //latitude and longitude of the position
    Angle phi, lambda;
    //label to add a note about the position
    String label;
    
    Position();
    Position(Angle, Angle);
    void setCartesian(String, const Cartesian, String);
    void getCartesian(Cartesian*);
    Cartesian getCartesian(void);
    void print(String, String, ostream&);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    bool transport(Position*, Route, String);
    bool transport_to(Route, String);
    void put_back_in(DrawPanel* draw_panel);
    Position half(void);
    Position antipode_lambda(void);
    string to_string(unsigned int);
    bool distance(Position, Length*, String, String);
    bool is_in(Route, String);
    void add_to_wxListCtrl(long, wxListCtrl*);
    void update_wxListCtrl(long, wxListCtrl*);
    void rotate(const String&, const Rotation&, Position*, const String&);
    
    bool operator==(const Position&);
    bool operator!=(const Position&);

};


#endif
