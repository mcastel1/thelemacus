//
//  lines.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include "lines.h"

//construct and initializes a new Lines object
Lines::Lines(void){
      
    reset();
    
}


//clear points and positions
void Lines::clear(void){
    
    points.clear();
    positions.clear();
    
}


//reset *this by clearing all vectors and addind a first entry '0' to positions
void Lines::reset(void){
    
    clear();
    positions.push_back(0);
    
}


//truncate the tail of *this by resetting the size of points and positions to points_size and positions_size, respectively
void Lines::reset(const unsigned long& points_size, const unsigned long& positions_size){
    
    points.resize(points_size);
    positions.resize(positions_size);
    
}
