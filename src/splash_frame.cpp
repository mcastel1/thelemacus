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



SplashFrame::SplashFrame(const String& image_path_in) : wxFrame(NULL, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFRAME_SHAPED|wxCLIP_CHILDREN), image_path(image_path_in) {
    
    CreateShapedFrame();
    Connect(wxEVT_PAINT, wxPaintEventHandler(SplashFrame::OnDraw));
    CentreOnScreen();

}

void SplashFrame::CreateShapedFrame(){
    
    wxBitmap bmpMask;
    bmpMask.LoadFile(image_path.value, wxBITMAP_TYPE_PNG);

    m_bmpBackground.LoadFile(image_path.value, wxBITMAP_TYPE_PNG);
//    m_bmpBackground.LoadFile(_T("skin.png"), wxBITMAP_TYPE_PNG);
    
    //    wxSize s;
    //    s = m_bmpBackground.GetSize();
    
    SetClientSize(m_bmpBackground.GetSize());

    wxRegion rgn(bmpMask, *wxBLACK);

    SetShape(rgn);
}


void SplashFrame::OnDraw(wxPaintEvent& event){
    
    wxPaintDC dc(this);

    dc.DrawBitmap(m_bmpBackground, wxPoint(-1, -1));
    
}
