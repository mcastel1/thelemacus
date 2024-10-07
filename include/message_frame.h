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

template<class P> class CloseFrame;


//this is a wxFrame designed to show a message to the GUI user. T is the type of the parent that called *this. FF_OK is the type of the functor class which will be called when the button ok is pressed. This type is variable, so it has been 'templated'. Note that here I do not name the type FF_OK -> F_OK because it would cause confusion with a type defined in wxWidgets
template<class T, class FF_OK> class MessageFrame: public wxFrame{
    
public:
    
    //the parent of *this
    T* parent;
    wxPanel *panel;
    wxBoxSizer *frame_sizer, *sizer_v;
    wxGridSizer* sizer_grid;
    wxButton* button_ok;
    StaticBitmap* image;
    //pointer to the class containing the functor which will be called when the button ok is pressed
    FF_OK* f_ok;
    //functor to close *this
    CloseFrame<MessageFrame>* close_frame;
    
    MessageFrame(T*, FF_OK*, const wxString&, const wxString&, String, const wxPoint&, const wxSize&, String);

    void KeyDown(wxKeyEvent&);
    void SetIdlingAndShow(void);
    
};



#endif
