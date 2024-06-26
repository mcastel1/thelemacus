//
//  show_question_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "show_question_frame.h"

#include "my_app.h"
#include "question_frame.h"
#include "set_idling.h"
#include "unset_idling.h"





template<class T, typename F_YES, typename F_NO, typename F_ABORT> ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::ShowQuestionFrame(T* f_in, F_YES* f_yes_in, F_NO* f_no_in, F_ABORT* f_abort_in) {

    f = f_in;
    f_yes = f_yes_in;
    f_no = f_no_in;
    f_abort = f_abort_in;

}


//set the wxControl, title and question and answers for the functor *this,  set enable_button_y/n both to true,  and bind_esc_to_button_b to true. Then call the functor operator() with CallAfter
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::SetAndCall(wxControl* control_in, String title_in, String question_in, String answer_y_in, String answer_n_in) {

    control = control_in;
    title = title_in;
    question = question_in;
    answer_y = answer_y_in;
    answer_n = answer_n_in;
    
    enable_button_a = true;
    enable_button_b = true;
    
    bind_esc_to_button_b = true;

    f->CallAfter(*this);

}


//set the wxControl, title and question and answers for the functor *this,  set enable_butoon_y/n to enable_button_y/n_in, and set bind_esc_to_button_b = bind_esc_to_button_b_in.  I call the functor operator() with CallAfter
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::SetAndCall(wxControl* control_in, String title_in, String question_in, String answer_y_in, String answer_n_in, bool enable_button_a_in, bool enable_button_b_in, bool bind_esc_to_button_b_in) {

    control = control_in;
    title = title_in;
    question = question_in;
    answer_y = answer_y_in;
    answer_n = answer_n_in;
    
    enable_button_a = enable_button_a_in;
    enable_button_b = enable_button_b_in;
    
    bind_esc_to_button_b = bind_esc_to_button_b_in;

    f->CallAfter(*this);

}

template void ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String, bool, bool, bool);



//if question_frame != NULL, enable or disable question_frame->button_a/b according to the boolean variables enable_button_a/b
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::EnableDisableButtons(void) {

    if(question_frame != NULL){
        
        question_frame->button_a->Enable(enable_button_a);
        question_frame->button_b->Enable(enable_button_b);
        
    }
    
}

template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::operator()(void) {


    SetIdling<T>* set_idling;
    UnsetIdling<T>* unset_idling;

    set_idling = new SetIdling<T>(f);
    unset_idling = new UnsetIdling<T>(f);


    if (!(f->idling)) {

        //I may be about to prompt a temporary dialog window, thus I set f->idling to true
        (*set_idling)();

        if (control != NULL) {
            //this question has been prompted from a control

            if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {

                question_frame = new QuestionFrame<F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
                question_frame->Show(true);
                question_frame->Raise();

                EnableDisableButtons();
                control->SetForegroundColour((wxGetApp().highlight_color));
                control->SetFont(wxGetApp().highlight_font);

            }

        }
        else {
            //this question has not been prompted from a control

            question_frame = new QuestionFrame<F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
            question_frame->Show(true);
            question_frame->Raise();

            EnableDisableButtons();
            
        }

    }

    //AFTER the question has been aswered and the related frame closed, I unset idling in f
    f->CallAfter(*unset_idling);

}

