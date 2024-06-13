
 
 
 //- compile on APPLE:
 //clear; clear; g++ main.cpp src/*.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib -I/Users/michele/Documents/sight_reduction_program/include -ferror-limit=0
 
 //To run on WIN32, the resource file is resource_file_windows.rc
 
 



//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "angle.h"
#include "angle_field.h"
#include "body.h"
#include "body_field.h"
#include "cartesian.h"
#include "catalog.h"
#include "check_angle.h"
#include "check_arc_degree.h"
#include "check_arc_minute.h"
#include "check_body.h"
#include "check_limb.h"
#include "check_sign.h"
#include "chrono.h"
#include "constants.h"
#include "double.h"
#include "file.h"
#include "file_r.h"
#include "file_rw.h"
#include "generic.h"
#include "int.h"
#include "length.h"
#include "length_unit.h"
#include "limb_field.h"
#include "main.h"
#include "multiple_item_field.h"
#include "my_app.h"
#include "my_string.h"
#include "position.h"
#include "position_frame.h"
#include "position_projection.h"
#include "position_rectangle.h"
#include "route.h"
#include "route_frame.h"
#include "route_type.h"
#include "route_type_field.h"
#include "speed.h"
#include "speed_unit.h"
#include "static_text.h"





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


