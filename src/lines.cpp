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
