//
//  position_projection.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "position_projection.h"



//default constructor
PositionProjection::PositionProjection(void) {

}

//custom constructor, which initialized the point with coordinates x, y
PositionProjection::PositionProjection(const double x_in, const double y_in) {

    x = x_in;
    y = y_in;

}

inline PositionProjection PositionProjection::operator + (const PositionProjection& q) {

    PositionProjection p;

    (p.x) = x + (q.x);
    (p.y) = y + (q.y);

    return p;

}

inline PositionProjection PositionProjection::operator - (const PositionProjection& q) {

    PositionProjection p;

    (p.x) = x - (q.x);
    (p.y) = y - (q.y);

    return p;

}

//divide both members of *this by the number x and return the resulting  PositionProjection
inline PositionProjection PositionProjection::operator / (const double& q) {

    PositionProjection p;

    (p.x) = x/q;
    (p.y) = y/q;

    return p;

}


inline PositionProjection PositionProjection::operator / (const Double& q){
    
    return ((*this)/(q.value));
    
}

//multiply both members of *this by the number x and return the resulting  PositionProjection
inline PositionProjection PositionProjection::operator * (const double& q) {

    PositionProjection p;

    (p.x) = x*q;
    (p.y) = y*q;

    return p;

}


inline void PositionProjection::operator += (const PositionProjection& q) {

    (*this) = (*this) + q;
    
}


inline void PositionProjection::operator -= (const PositionProjection& q) {

    (*this) = (*this) - q;
    
}


inline void PositionProjection::operator *= (const double& a) {

    (*this) = (*this) * a;
    
}


inline void PositionProjection::operator /= (const double& a) {

    (*this) = (*this) / a;
    
}


//return true if *this is falls wihtin the plot area of *draw_panel in the Mercator projection, and false otherwise
inline bool PositionProjection::CheckMercator(DrawPanel* draw_panel){
    
    bool output;
    

    if ((draw_panel->x_min) <= (draw_panel->x_max)) {
        //this is the 'normal' configuration where the boundaries of the chart do not encompass the meridian lambda = pi

        output = (((draw_panel->x_min) <= x) && (x <= (draw_panel->x_max)));

    }else {
        //this is the 'non-normal' configuration where the boundaries of the chart encompass the meridian lambda = pi

        output = ((((draw_panel->x_min) <= x) && (x <= (draw_panel->x_max) + 2.0*M_PI)) ||  (((draw_panel->x_min) - 2.0*M_PI <= x) && (x <= (draw_panel->x_max))));

    }

    output &= (((draw_panel->y_min) <= y) && (y <= (draw_panel->y_max)));
    
    return output;

}


//normalize p.lambda, then set x and y equal to the Mercator projections of the Position p
inline void PositionProjection::SetMercator(const Position& p){
    
    x = -(p.lambda.value);
    y = log(1.0 / cos((p.phi)) + tan((p.phi)));
    
}


//normalize p.lambda, then set x and y equal to the Mercator projections of the Position p
inline void PositionProjection::NormalizeAndSetMercator(const Position& p){
    
    Position temp;
    
    temp = p;
    temp.lambda.normalize_pm_pi();
    
    SetMercator(temp);
     
}
