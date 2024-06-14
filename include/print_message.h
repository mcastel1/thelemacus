//
//  print_message.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef print_message_h
#define print_message_h

#include <iostream>

#include "message_frame.h"

using namespace std;


//this functor pops out a message window with title tile and  message message, resulting from the wxControl control. The type of the frame from which the error message is printed is T, and it is variable so as to make this class adaptable
template<class T, typename FF_OK> class PrintMessage{
    
public:
    
    T* f;
    wxControl* control;
    String title, message, image_path;
    FF_OK* f_ok;
    MessageFrame<FF_OK>* message_frame;
    
    PrintMessage(T*, FF_OK*);
    
    void SetAndCall(wxControl*, String, String, String);
    void operator()(void);
    
    
};


#endif
