//
//  message_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef message_frame_h
#define message_frame_h


#include <iostream>

#include "static_bitmap.h"

using namespace std;


//this is a wxFrame designed to show a message to the GUI user. FF_OK is the type of the functor class which will be called when the button ok is pressed. This type is variable, so it has been 'templated'
template<class FF_OK> class MessageFrame: public wxFrame{
    
public:
    
    wxPanel *panel;
    wxBoxSizer *frame_sizer, *sizer_v;
    wxGridSizer* sizer_grid;
    wxButton* button_ok;
    StaticBitmap* image;
    //    wxBitmap* m_bitmap;
    //pointer to the class containing the functor which will be called when the button ok is pressed
    FF_OK* f_ok;
    
    MessageFrame(wxWindow*, FF_OK*, const wxString&, const wxString&, String, const wxPoint&, const wxSize&, String);
    //initialize the functor to close this MessageFrame when button_ok will be pressed
    CloseFrame<MessageFrame>* close_frame;
    void KeyDown(wxKeyEvent&);
    //    void OnPaint(wxPaintEvent&);
    //    void OnPressOk(wxCommandEvent&);
    
    //    DECLARE_EVENT_TABLE();
    
};



#endif
