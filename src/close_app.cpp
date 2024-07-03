//
//  close_app.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "close_app.h"
#include "question_frame.h"


CloseApp::CloseApp(MyApp* app_in){
    
    app = app_in;
    
}


template <class T> void CloseApp::operator()([[maybe_unused]] T& event) {
    
    app->list_frame->Close();
    app->disclaimer->Close();
    
}

template void CloseApp::operator()<wxCommandEvent>(wxCommandEvent&);
