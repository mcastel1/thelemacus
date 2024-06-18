//
//  close_app.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef close_app_h
#define close_app_h

#include <iostream>

#include "my_app.h"

using namespace std;


class MyApp;




//this class defines a functor to close the app properly
class CloseApp{
    
public:
    
    MyApp* app;
    
    CloseApp(MyApp*);
    template<class T>void operator()(T&);
    
};


#endif
