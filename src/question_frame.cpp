//
//  question_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "question_frame.h"

#include "all_routes.h"
#include "animate_to_object.h"
#include "ask_remove_related_route.h"
#include "ask_remove_related_sight.h"
#include "close_app.h"
#include "close_frame.h"
#include "confirm_transport.h"
#include "delete_position.h"
#include "delete_route.h"
#include "delete_sight.h"
#include "existing_route.h"
#include "my_app.h"
#include "new_route.h"
#include "reset_list_frame.h"
#include "save_and_reset.h"
#include "show_all.h"
#include "some_routes.h"



template<class T, class F_A, class F_B, class F_ABORT> QuestionFrame<T, F_A, F_B, F_ABORT>::QuestionFrame(T* parent_in, F_A* f_a_in, String string_a_in, F_B* f_b_in, String string_b_in, F_ABORT* f_abort_in, bool enable_button_a_in, bool enable_button_b_in, bool bind_esc_to_button_b_in, const wxString& title, const wxString& message, String path_icon_file, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(((wxWindow*)parent_in), wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) {

    wxRect rectangle;
    vector<StaticText*> text;
    //the wxString message will be split into multiple Strings which were separated by a '\n' in message, each string will be written in an entry of message_split
    vector<String> message_split;
    int i;
    

    f_a = f_a_in;
    string_a = string_a_in;
    f_b = f_b_in;
    string_b = string_b_in;
    f_abort = f_abort_in;
    parent = parent_in;
    
    enable_button_a = enable_button_a_in;
    enable_button_b = enable_button_b_in;
    
    bind_esc_to_button_b = bind_esc_to_button_b_in;

    //SetColor(this);
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< QuestionFrame<T, F_A, F_B, F_ABORT> >(this);

    //image
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (wxGetApp().display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);

    //allocate sizers
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(1, 2, 0, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

    //write each line into text
    for (i = 0, message_split = String(message.ToStdString()).split(), text.resize(message_split.size()); i < (message_split.size()); i++) {
        text[i] = new StaticText(panel, wxString((message_split[i]).value), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    }

    //buttons
    button_a = new wxButton(panel, wxID_ANY, string_a.value, wxDefaultPosition, wxDefaultSize);
    button_a->Bind(wxEVT_BUTTON, *f_a);
    button_a->Bind(wxEVT_BUTTON, *close_frame);
    button_b = new wxButton(panel, wxID_ANY, string_b.value, wxDefaultPosition, wxDefaultSize);
    button_b->Bind(wxEVT_BUTTON, *f_b);
    button_b->Bind(wxEVT_BUTTON, *close_frame);
    
    if(parent){
        button_a->Bind(wxEVT_BUTTON, *(parent->unset_idling));
        button_b->Bind(wxEVT_BUTTON, *(parent->unset_idling));
    }


    panel->Bind(wxEVT_KEY_DOWN, &QuestionFrame::KeyDown<wxKeyEvent>, this);

    image = new StaticBitmap(
                             panel,
                             path_icon_file,
                             //I use ToDIP to adjust the size independently of the screen resolution
                             ToDIP(wxSize(
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
                                          ))
                             );

    //add all entries of text to sizer_v
    for (i = 0; i < text.size(); i++) {
        sizer_v->Add(text[i], 0, wxALL | wxALIGN_CENTER);
    }
    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid->Add(button_a, 0, wxALIGN_CENTER);
    sizer_grid->Add(button_b, 0, wxALIGN_CENTER);
    sizer_v->Add(sizer_grid, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_h->Add(sizer_v, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_h);
    panel->Fit();
    Fit();

    SetClientSize(panel->GetBestSize());
    CentreOnScreen();

}

template class QuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>;
template class QuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>;
template class QuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>;
template class QuestionFrame<MyApp, ShowAll, CloseApp, CloseApp>;


//if the user presses return/escape, I call f_a / f_b + close_frame and (if parent!=NULL) unset_idling. These calls reflect the calls made with Bind() to button_a, button_b in the constructor of QuestionFrame
template<class T, class F_A, class F_B, class F_ABORT> template<class E> void QuestionFrame<T, F_A, F_B, F_ABORT>::KeyDown(E& event) {

    wxCommandEvent dummy;

    if (((event.GetKeyCode()) == WXK_ESCAPE) && enable_button_b) {
        //the user pressed esc
        
        if(bind_esc_to_button_b){
            //esc button is bound to button_b -> call *f_b
            
            (*f_b)(dummy);
            
        }else{
            //esc button is bound to *f_abort -> call *f_abort

            
            (*f_abort)(dummy);
            
        }


    }else {

        if ((((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) && enable_button_a) {
            //the user pressed return or numpad return

            (*f_a)(dummy);

        }

    }

    (*close_frame)(dummy);
    
    //if parent != NULL, call parent->unset_idling
    if(parent){
        (*(parent->unset_idling))();
    }


    event.Skip(true);

}


//set the parent of *this to idling and show *this
template<class T, class F_A, class F_B, class F_ABORT>  void QuestionFrame<T, F_A, F_B, F_ABORT>::SetIdlingAndShow(void){
 
    if(parent != NULL){
        (*(parent->set_idling))();
    }
    Show(true);
    
}

template void QuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DisconnectSightAndUnsetIdling>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, AnimateToObject<Route, HighlightObject<ListFrame, DoNothing>>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>::SetIdlingAndShow();
template void QuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>::SetIdlingAndShow();
