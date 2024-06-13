
 
 
 //- compile on APPLE:
 //clear; clear; g++ main.cpp src/*.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib -I/Users/michele/Documents/sight_reduction_program/include
 
 //To run on WIN32, the resource file is resource_file_windows.rc
 
 



//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "main.h"
#include "my_app.h"
#include "constants.h"
#include "multiple_item_field.h"
#include "catalog.h"
#include "generic.h"
#include "double.h"
#include "my_string.h"
#include "chrono.h"
#include "angle.h"
#include "length.h"
#include "length_unit.h"
#include "speed.h"
#include "speed_unit.h"
#include "file.h"
#include "file_r.h"
#include "file_rw.h"
#include "angle_field.h"
#include "static_text.h"
#include "body.h"
#include "body_field.h"
#include "limb_field.h"
#include "check_angle.h"





//compute the astronomical position and updated all the GUI fields in set() and re-draws everything
template<class T> void ListFrame::ComputePosition([[maybe_unused]] T& event) {
    
    ShowQuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>* print_question;
    AllRoutes* all_routes;
    SomeRoutes* some_routes;
    
    all_routes = new AllRoutes(this);
    some_routes = new SomeRoutes(this);
    print_question = new ShowQuestionFrame<ListFrame, AllRoutes, SomeRoutes, UnsetIdling<ListFrame>>(this, all_routes, some_routes, unset_idling);
    
    selecting_route_for_position = true;
    
    //ask the user whether he/she wants to transport the sight with a an existing route or with a new route.
    print_question->SetAndCall(NULL, String("You want to determine the astronomical position"), String("With what route do you want to do it?"), String("All routes"), String("Some routes"));
    
}


wxIMPLEMENT_APP(MyApp);


