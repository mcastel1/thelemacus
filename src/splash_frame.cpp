//
//  splash_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "splash_frame.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif



SplashFrame::SplashFrame(void) :  wxFrame(NULL, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFRAME_SHAPED|wxCLIP_CHILDREN) {

//    wxRect rectangle;
//
//    f_ok = f_ok_in;
//
//    //SetColor(this);
//    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
//    close_frame = new CloseFrame< SplashFrame<FF_OK> >(this);
//
//    //image
//    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
//    rectangle = (wxGetApp().display.GetClientArea());
//    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
//    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);
//
//    //allocate sizers
//    frame_sizer = new wxBoxSizer(wxVERTICAL);
//    sizer_v = new wxBoxSizer(wxVERTICAL);
//
//    //    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
//    //    sizer_grid = new wxGridSizer(3, 1, 0, 0);
//
//
//    StaticText* text = new StaticText(panel, message, wxDefaultPosition, wxDefaultSize, 0);
//
//    //bind the function SplashFrame<FF_OK>::KeyDown to the event where a keyboard dey is down
//    panel->Bind(wxEVT_KEY_DOWN, &SplashFrame<FF_OK>::KeyDown, this);
//
//
//    //buttons
//    button_ok = new wxButton(panel, wxID_ANY, "Ok", wxDefaultPosition, wxDefaultSize);
//    //    button_ok->Bind(wxEVT_BUTTON, &SplashFrame::OnPressOk, this);
//    button_ok->Bind(wxEVT_BUTTON, *close_frame);
//    button_ok->Bind(wxEVT_BUTTON, *f_ok);
//
//    image = new StaticBitmap(
//                             panel,
//                             image_path,
//                             //I use ToDIP to adjust the size independently of the screen resolution
//                             ToDIP(wxSize(
//                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
//                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
//                                          ))
//                             );
//
//    sizer_v->Add(text, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
//    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
//    sizer_v->Add(button_ok, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
//
//#ifdef _WIN32
//    //if I am on WIN32, I set the icon from the icon set in the .rc file
//    SetIcon(wxICON(app_icon));
//#endif
//
//    //panel->SetSizer(sizer);
//    //    Maximize(panel);
//    panel->SetSizerAndFit(sizer_v);
//    panel->Fit();
//    Fit();
//
//    
//    SetClientSize(panel->GetBestSize());
//    CentreOnScreen();

}

