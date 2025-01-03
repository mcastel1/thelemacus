//
//  show_question_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "show_question_frame.h"

#include "all_routes.h"
#include "animate_to_object.h"
#include "my_app.h"
#include "question_frame.h"
#include "reset_list_frame.h"
#include "save_and_reset.h"
#include "set_idling.h"
#include "some_routes.h"
#include "unset_idling.h"


template<class T, typename F_YES, typename F_NO, typename F_ABORT> ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::ShowQuestionFrame(T* f_in, F_YES* f_yes_in, F_NO* f_no_in, F_ABORT* f_abort_in) {
    
    f = f_in;
    f_yes = f_yes_in;
    f_no = f_no_in;
    f_abort = f_abort_in;
    
}

template class ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>;
template class ShowQuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class ShowQuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;


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

template void ShowQuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);
template void ShowQuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String, String);


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
    
    
    //    if (!(f->idling)) {
    
    //        //I may be about to prompt a temporary dialog window, thus I set f->idling to true
    //        (*(f->set_idling))();
    
    if (control != NULL) {
        //this question has been prompted from a control
        
        if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {
            
            question_frame = new QuestionFrame<T, F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
            question_frame->SetIdlingAndShow();
            question_frame->Raise();
            
            EnableDisableButtons();
            control->SetForegroundColour((wxGetApp().highlight_color));
            control->SetFont(wxGetApp().highlight_font);
            
        }
        
    }
    else {
        //this question has not been prompted from a control
        
        question_frame = new QuestionFrame<T, F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
        question_frame->SetIdlingAndShow();
        question_frame->Raise();
        
        EnableDisableButtons();
        
    }
    
    //    }
    
    //AFTER the question has been aswered and the related frame closed, I unset idling in f
    //    f->CallAfter(*unset_idling);
    
}

