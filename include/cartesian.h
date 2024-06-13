//
//  cartesian.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef cartesian_h
#define cartesian_h

#include "gsl_vector.h"

#include "position.h"


// a three-dimensional cartesian position in the reference frame centered at the earth's origin, with the x-y axes lying on the equator and the x axis aligned with Greenwich meridian
class Cartesian{
    
public:
    
    gsl_vector* r;
    
    Cartesian();
    Cartesian(const Position*);
    double dot(const Cartesian&);
    Cartesian cross (const Cartesian&);
    void setPosition(const Position*);
    void print(String, String, ostream&);
    void operator = (const Cartesian&);
    
};

#endif
