//
//  color.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef color_h
#define color_h


//a class for color objects
class Color: public wxColour{
    
public:
    Color();
    Color(wxColour);
    Color(unsigned char, unsigned char, unsigned char);
    Color(unsigned char, unsigned char, unsigned char, unsigned char);
    
    void read_from_file(String, String, String, String);
    int ToRGB(void);
    
};

#endif
