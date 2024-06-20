//
//  position_rectangle.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef position_rectangle_h
#define position_rectangle_h

#include <iostream>

#include "position.h"
#include "position_projection.h"

using namespace std;


class Position;
class PositionProjection;


//this class defines a 'rectangular' area on the Eearth's surface: it is a rectangle with vertices p_NW, p_SE in the Mercator projection
class PositionRectangle{
    
public:
    
    //thw two Positions which constitute the vertices of the rectangle: p is the NW vertex, p_SE is the SE vertex
    Position p_NW, p_SE;
    
    PositionRectangle();
    PositionRectangle(Position, Position, String);
    
    bool Contains(Position);
    bool SizeMercator(PositionProjection*);
    
};




#endif
