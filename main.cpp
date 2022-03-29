/*
 
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib -O3
 */

//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0

#include "main.h"
#include "lib.cpp"

/*
 notes: set parents to constructors of all frames that you defined
 - when you click on modify and then press reduce, the code should not insert a new item in listcontrol, but replace the old one
- add recent items to dropdown menus
 - add instrumental error
 - add condition that an error message is prompted only if the GUI field is enabled not only to CheckLength, but also to all other Check* classes
 - add error message if phi_min, phi_max etc... exceed the min and max latitudes in the data file.
 - all quantities in pixels must be relative to screen size, not absolute pixel size.
 - re-arrange class definitions properly
 - add lambert projection and 3D sphere with no projection
 - make sure you can recover a correct chart after the print error message has been prompted because the zoom factor is too large
 - make sure that LengthField allows to enter lengths in both meters and nm
 - change the initialization of structs (such as     (check.p) = this;) by transforming the structs into classes, creating their constructor, transforming check into a pointer and replacing     (check.p) = this; with check = new Check(this); do this for all objects
 - set log scale on slider
 - fix issue that related sight is not properly deleted when a route is deleted
 - when mouse hovers over listcontrol of sights, highlight the related route, if any
 -     Now the slider in log scale works, but there is a problem when i de-zoom back to 1:1
 - compute authomatically n_interval_tics by calculating the width of the window and of each label on the axes
 
 */



wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*0.75);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*0.75);
    
    
    ListFrame *list_frame = new ListFrame("List of sights", "", wxDefaultPosition, wxDefaultSize, String(""));
    list_frame->Show(true);
    
    (list_frame->chart_frame) = new ChartFrame(list_frame, "A nautical chart",  wxDefaultPosition, wxDefaultSize, String(""));
    (list_frame->chart_frame)->Show(true);
    
    //    list_frame->plot->print(true, String("************* "), cout);
    
    
    //    Answer* answer;
    //    answer = new Answer();
    //    MessageFrame* message_frame = new MessageFrame(NULL, String("question"),  answer, "a", "b", wxDefaultPosition, wxDefaultSize, String(""));
    //    message_frame ->Show(true);
    
    
    return true;
    
}
