//
//  static_bitmap.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef static_bitmap_h
#define static_bitmap_h

#include <iostream>

#include "my_string.h"

using namespace std;


//a class for image objects
class StaticBitmap: public wxStaticBitmap{
    
public:
    StaticBitmap(wxWindow*, String, wxSize);
    
};


#endif
