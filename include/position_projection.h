//
//  position_projection.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef position_projection_h
#define position_projection_h

#include "draw_panel.h"
#include "position.h"

//this class denotes a point in a generic 2D projection of the sphere
class PositionProjection{
    
public:
    //the x, y coordinates of the projected point.
    double x, y;
    
    PositionProjection();
    PositionProjection(const double, const double);
    void SetMercator(const Position&);
    bool CheckMercator(DrawPanel*);
    void NormalizeAndSetMercator(const Position&);
    PositionProjection operator + (const PositionProjection&), operator - (const PositionProjection&), operator / (const double &), operator / (const Double&), operator * (const double &);
    void operator += (const PositionProjection&), operator -= (const PositionProjection&), operator *= (const double&), operator /= (const double&);
        
};

#endif
