//
//  show_question_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef show_question_frame_h
#define show_question_frame_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif


using namespace std;

template<class F_A, class F_B, class F_ABORT> class QuestionFrame;


//this functor pops out a question window with title tile, quesiton question, and answers answer_y, answer_n, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this class adaptable. If the user answers yes/no to the question, f_yes/f_no are called
template<class T, typename F_YES, typename F_NO, typename F_ABORT> class ShowQuestionFrame{
    
public:
    
    T* f;
    wxControl* control;
    String title, message, question, answer_y, answer_n;
    F_YES* f_yes;
    F_NO* f_no;
    F_ABORT* f_abort;
    QuestionFrame<F_YES, F_NO, F_ABORT>* question_frame;
    bool /*these are true/false if the yes/no button are enabled/disabled, respectively*/ enable_button_a, enable_button_b, /*if this is true, then pressing esc will be equivalent to pressing button b, if this is false, pressing esc will call *f_abort*/ bind_esc_to_button_b;
    
    ShowQuestionFrame(T*, F_YES*, F_NO*, F_ABORT*);
    
    void SetAndCall(wxControl*, String, String, String, String);
    void SetAndCall(wxControl*, String, String, String, String, bool, bool, bool);
    void EnableDisableButtons(void);
    void operator()(void);
    
    
};




#endif
