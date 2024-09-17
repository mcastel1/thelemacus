//
//  string.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef lines_h
#define lines_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

using namespace std;

//this class containes a set of segments
class Lines{
    
public:
    
    //position[j] = [the # of the entry in points at which the j-th chunk of connnected points starts]. Such j-th chunk is given by points[position[j]], points[position[j]+1], .... , points[osition[j+1]-1]
    vector<wxPoint> points;
    vector<unsigned long long int> positions;
    
    Lines(void);
    
    void clear(void);
    void reset(void);
    void reset(const unsigned long&, const unsigned long&);
    
};

#endif
