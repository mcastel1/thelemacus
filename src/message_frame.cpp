//
//  message_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "message_frame.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "close_frame.h"
#include "constants.h"
#include "list_frame.h"
#include "my_app.h"
#include "position_frame.h"
#include "sight_frame.h"
#include "static_text.h"

template<class T, class FF_OK> MessageFrame<T, FF_OK>::MessageFrame(T* parent_in, FF_OK* f_ok_in, const wxString& title, const wxString& message, String image_path, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(((wxWindow*)parent_in), wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN), parent(parent_in), f_ok(f_ok_in) {

    wxRect rectangle;

    
    //set to idling mode the parent if parent != NULL
    if(parent){
        (*(parent->set_idling))();
    }

    //SetColor(this);
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< MessageFrame<T, FF_OK> >(this);

    //image
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (wxGetApp().display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);

    //allocate sizers
    frame_sizer = new wxBoxSizer(wxVERTICAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    StaticText* text = new StaticText(panel, message, wxDefaultPosition, wxDefaultSize, 0);

    //bind the function MessageFrame<T, FF_OK>::KeyDown to the event where a keyboard dey is down
    panel->Bind(wxEVT_KEY_DOWN, &MessageFrame<T, FF_OK>::KeyDown, this);


    //buttons
    button_ok = new wxButton(panel, wxID_ANY, "Ok", wxDefaultPosition, wxDefaultSize);
    button_ok->Bind(wxEVT_BUTTON, *close_frame);
    button_ok->Bind(wxEVT_BUTTON, *f_ok);
    if(parent){
        button_ok->Bind(wxEVT_BUTTON, *(parent->unset_idling));
    }

    image = new StaticBitmap(
                             panel,
                             image_path,
                             //I use ToDIP to adjust the size independently of the screen resolution
                             ToDIP(wxSize(
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
                                          ))
                             );

    sizer_v->Add(text, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_v->Add(button_ok, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_v);
    panel->Fit();
    Fit();
    
    SetClientSize(panel->GetBestSize());
    CentreOnScreen();

}

template class MessageFrame<ChartFrame, UnsetIdling<ChartFrame>>;
template class MessageFrame<RouteFrame, UnsetIdling<RouteFrame>>;
template class MessageFrame<SightFrame, UnsetIdling<SightFrame>>;
template class MessageFrame<PositionFrame, UnsetIdling<PositionFrame>>;
template class MessageFrame<DrawPanel, UnsetIdling<DrawPanel>>;
template class MessageFrame<ListFrame, UnsetIdling<ListFrame>>;


//if a key is pressed in the keyboard, I call this function
template<class T, class FF_OK> void MessageFrame<T, FF_OK>::KeyDown(wxKeyEvent& event) {

    if (((event.GetKeyCode()) == WXK_ESCAPE) || ((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
        //the user presses esc or return -> I close *this and set the idling variable to false

        (*f_ok)();
        (*close_frame)(event);
        
        //if parent != NULL, call parent->unset_idling
        if(parent){
            (*(parent->unset_idling))();
        }
        
    }

}


//set the parent of *this to idling and show *this
template<class T, class FF_OK>  void MessageFrame<T, FF_OK>::SetIdlingAndShow(void){
 
    if(parent != NULL){
        (*(parent->set_idling))();
    }
    Show(true);
    
}
