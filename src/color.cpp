//
//  color.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "color.h"
#include "constants.h"
#include "generic.h"


//constructs a color object by setting no arguments
Color::Color() : wxColour() {

}


//constructs a color object by setting no arguments
Color::Color(wxColour input) : wxColour(input) {

}


//constructs a color objct by setting its rgb values to red, green, blue
Color::Color(unsigned char red, unsigned char green, unsigned char blue) : wxColour(red, green, blue) {

}

//constructs a color objct by setting its rgb values to red, green, blue and transparency (alpha)
Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : wxColour(red, green, blue, alpha) {

}

//converts *this to RGB format
int Color::ToRGB(void) {

    int i, output;

    //convert this to BRG format which the wxWidgets function GetRGB() and then shift its bits so as to convert it to RGB format
    for (output = 0, i = 0; i < 3; i++) {

        output ^= ((((this->GetRGB()) >> 8 * i) & hexadecimal_mask) << 8 * (3 - (i + 1)));

    }

    return output;

}



//reads from file the color written after 'name = ' and writes it into this. If mode  = 'RW' ('R') it reads from a FileRW (FileR)
void Color::read_from_file(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    String new_prefix, s;
    size_t pos_end;
    int red, green, blue;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    s.read_from_file_to(name, filename, mode, new_prefix);

    //get rid of everything that comes before and at '(' at the beginnign of s
    pos_end = (s.value).find("(");
    s.set(s.value.substr(pos_end + 1).c_str());
    //look for the first ','

    pos_end = (s.value).find(",");

    //read red
    red = stoi(((s.value).substr(0, pos_end)).c_str());

    //get rid of the first ','
    s.set(s.value.substr(pos_end + 1).c_str());

    pos_end = (s.value).find(",");

    green = stoi((s.value).substr(0, pos_end).c_str());

    //get rid of the second ','
    s.set(s.value.substr(pos_end + 1).c_str());

    pos_end = (s.value).find(")");
    //get rid of '('
    blue = stoi((s.value).substr(0, pos_end + 1).c_str());

    (*this) = Color(red, green, blue);


}


