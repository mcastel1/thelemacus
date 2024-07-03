//
//  constants.cpp
//  thelemacus
//
//  Created by Michele on 03/07/2024.
//

#include <stdio.h>

#include "constants.h"


vector<string> kml_colors = {"ff0000ff", "ffff0000", "ff336699", "ff00ff00", "ff0080ff", "ffff00ff"};
vector<string> hex_colors = {"#000000", "#0000FF", "#00FF00", "#663300", "#3399FF", "#0000CC"};

vector<unsigned int> days_per_month_leap = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_common = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Int display_precision, data_precision;
Double length_plot_area_over_length_chart, length_chart_over_length_chart_frame, length_border_over_length_screen;
