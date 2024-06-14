//
//  chart_panel.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef chart_panel_h
#define chart_panel_h

#include <iostream>

#include "chart_frame.h"

using namespace std;


class ChartFrame;

class ChartPanel : public wxPanel{
    
public:
    
    ChartFrame* parent;
    
    ChartPanel(ChartFrame*, const wxPoint&, const wxSize&);
    
};


#endif
