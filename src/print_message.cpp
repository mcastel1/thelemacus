//
//  print_message.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "print_message.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "message_frame.h"
#include "my_app.h"
#include "my_string.h"
#include "position_frame.h"
#include "set_idling.h"
#include "sight_frame.h"
#include "unset_idling.h"


template<class T, class FF_OK> PrintMessage<T, FF_OK>::PrintMessage(T* f_in, FF_OK* f_ok_in) {

    f = f_in;
    f_ok = f_ok_in;

}

template class PrintMessage<ChartFrame, UnsetIdling<ChartFrame>>;
template class PrintMessage<RouteFrame, UnsetIdling<RouteFrame>>;
template class PrintMessage<SightFrame, UnsetIdling<SightFrame>>;
template class PrintMessage<DrawPanel, UnsetIdling<DrawPanel>>;
template class PrintMessage<ListFrame, UnsetIdling<ListFrame>>;
template class PrintMessage<PositionFrame, UnsetIdling<PositionFrame>>;


//set the wxControl, title,  message  and image_path for the functor *this, and I call the functor operator() with CallAfter
template<class T, class FF_OK> void PrintMessage<T, FF_OK>::SetAndCall(wxControl* control_in, String title_in, String message_in, String image_path_in) {

    control = control_in;
    title = title_in;
    message = message_in;
    image_path = image_path_in;

    f->CallAfter(*this);

}

template void PrintMessage<ListFrame, UnsetIdling<ListFrame>>::SetAndCall(wxControl*, String, String, String);
template void PrintMessage<RouteFrame, UnsetIdling<RouteFrame>>::SetAndCall(wxControl*, String, String, String);
template void PrintMessage<SightFrame, UnsetIdling<SightFrame>>::SetAndCall(wxControl*, String, String, String);
template void PrintMessage<PositionFrame, UnsetIdling<PositionFrame>>::SetAndCall(wxControl*, String, String, String);
template void PrintMessage<DrawPanel, UnsetIdling<DrawPanel>>::SetAndCall(wxControl*, String, String, String);
template void PrintMessage<ChartFrame, UnsetIdling<ChartFrame>>::SetAndCall(wxControl*, String, String, String);


template<class T, class FF_OK> void PrintMessage<T, FF_OK>::operator()(void) {
    
    SetIdling<T>* set_idling;
    UnsetIdling<T>* unset_idling;
    
    set_idling = new SetIdling<T>(f);
    unset_idling = new UnsetIdling<T>(f);
    
    if (control != NULL) {
        
        if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {
            
            message_frame = new MessageFrame<T, FF_OK>(f, f_ok, title.value, message.value, image_path, wxDefaultPosition, wxDefaultSize, String(""));
            message_frame->SetIdlingAndShow();
            message_frame->Raise();
            
            control->SetForegroundColour((wxGetApp().error_color));
            control->SetFont(wxGetApp().error_font);
            
        }

    }else{

        message_frame = new MessageFrame<T, FF_OK>(f, f_ok, title.value, message.value, image_path, wxDefaultPosition, wxDefaultSize, String(""));
        message_frame->SetIdlingAndShow();
        message_frame->Raise();

    }

//    //AFTER the dialog window has been closed, I set f->idling to calse
//    f->CallAfter(*unset_idling);

}

//set the parent of *this to idling and show *this
template<class T, class FF_OK>  void MessageFrame<T, FF_OK>::SetIdlingAndShow(void){
 
    if(parent != NULL){
        (*(parent->set_idling))();
    }
    Show(true);
    
}
