//
//  chart_panel.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "chart_panel.h"




ChartPanel::ChartPanel(ChartFrame* parent_in, const wxPoint& position, const wxSize& size) : wxPanel(parent_in, wxID_ANY, position, size, wxTAB_TRAVERSAL, wxT("")) {

    parent = parent_in;

}
