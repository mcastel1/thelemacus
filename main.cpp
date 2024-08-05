
 
 
 //- compile on APPLE:
 //clear; clear; g++ main.cpp src/*.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib -I/Users/michele/Documents/sight_reduction_program/include -ferror-limit=0

//compile on APPLE and write output to file:   clear; clear; rm ~/Desktop/errors.dat;  g++ main.cpp src/*.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17   -rpath /Applications/boost_1_83_0/stage/lib -I/Users/michele/Documents/sight_reduction_program/include  >& ~/Desktop/errors.dat
 
 //To run on WIN32, the resource file is resource_file_windows.rc
 
 



//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "angle.h"
#include "all_routes.h"
#include "angle_field.h"
#include "body.h"
#include "body_name_field.h"
#include "cartesian.h"
#include "catalog.h"
#include "check_angle.h"
#include "check_arc_degree.h"
#include "check_arc_minute.h"
#include "check_body_name.h"
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
#include "show_question_frame.h"
#include "some_routes.h"
#include "speed.h"
#include "speed_unit.h"
#include "static_text.h"





wxIMPLEMENT_APP(MyApp);


