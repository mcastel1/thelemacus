//
//  lines.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include "lines.h"

Lines::Lines(void){
    
    points.clear();
    positions.clear();
    
    //add zero as a first entry of positions because the first chunk will start with points[0]
    positions.push_back(0);
    
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
