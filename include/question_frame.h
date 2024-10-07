//
//  question_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef question_frame_h
#define question_frame_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


#include "my_string.h"

using namespace std;


template<class P> class CloseFrame;
class StaticBitmap;
class String;



//a wxFrame designed to ask a  yes/no question to the GUI user. T is the type of the parent that called *this. F_A is the type of the functor class which will be called when the button yes is pressed. This type is variables, so it has been 'templated'. Same for F_B. If the user presses enter (esc), f_a  (f_b) are called
template<class T, class F_A, class F_B, class F_ABORT> class QuestionFrame: public wxFrame{
    
public:

    //the parent of *this
    T* parent;
    //the non-GUI object connected to the GUI object MessageFrame
    wxPanel *panel;
    wxBoxSizer *sizer_v, *sizer_h, *sizer_buttons;
    wxGridSizer* sizer_grid;
    wxButton* button_a, *button_b;
    bool enable_button_a, enable_button_b, /*if this is true, then pressing esc will be equivalent to pressing button b, if this is false, pressing esc will call *f_abort*/bind_esc_to_button_b;
    StaticBitmap* image;
    //functor to close *this 
    CloseFrame<QuestionFrame>* close_frame;
    //pointer to the class containing the functor which will be called when the button yes is pressed
    F_A* f_a;
    //pointer to the class containing the functor which will be called when the button no is pressed
    F_B* f_b;
    //pointer to the class containing the functor which will be called to abort
    F_ABORT* f_abort;
    String string_a, string_b;
    
    QuestionFrame(T*, F_A*, String, F_B*, String, F_ABORT*, bool, bool, bool, const wxString&, const wxString&,  String, const wxPoint&, const wxSize&, String);
    template<class E> void KeyDown(E&);
    void SetIdlingAndShow(void);
    
};

#endif
