//
//  bitmap.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef bitmap_h
#define bitmap_h

#include <iostream>

#include "my_string.h"

using namespace std;

//a class for Bitmap images
class Bitmap: public wxBitmap{
    
public:
    Bitmap();
    Bitmap(String, wxSize);
    
};

#endif
