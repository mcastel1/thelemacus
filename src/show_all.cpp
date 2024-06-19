//
//  show_all.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "show_all.h"

#include "show_frame.h"


ShowAll::ShowAll(ListFrame* frame_in) {

    frame = frame_in;

}

//show ListFrame and all ChartFrames
template <class T> void ShowAll::operator()(T& event) {

    ShowFrame<ListFrame>* show_frame;
    show_frame = new ShowFrame<ListFrame>(frame);

    (*show_frame)(event);
    for(unsigned int i=0; i<wxGetApp().list_frame->chart_frames.size(); i++){
        wxGetApp().list_frame->chart_frames[i]->ResetRender(event);
    }
    wxGetApp().ShowCharts(event);
    wxGetApp().AnimateCharts();

    event.Skip(true);

}
