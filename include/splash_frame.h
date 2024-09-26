//
//  splash_frame.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef splash_frame_h
#define splash_frame_h


#include <iostream>

#include "static_bitmap.h"

#include "my_string.h"

using namespace std;




class SplashFrame: public wxFrame{
    
public:
    
    SplashFrame(const String&);
    
private:
    
    String image_path;
    void CreateShapedFrame();
    wxBitmap m_bmpBackground;


    
};



#endif
