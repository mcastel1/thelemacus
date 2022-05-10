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
 - add instrumental error
 - revise Angle::to_string in such a way that all angles take the same space when printed out to string
 - all quantities in pixels must be relative to screen size, not absolute pixel size.
 - add lambert projection and 3D sphere with no projection
 - change the initialization of structs (such as     (check.p) = this;) by transforming the structs into classes, creating their constructor, transforming check into a pointer and replacing     (check.p) = this; with check = new Check(this); do this for all objects
 - fix issue that related sight is not properly deleted when a route is deleted
 - Now the slider in log scale works, but there is a problem when i de-zoom back to 1:1
 - set up a proper output to a log file
 - turn gamma_lambda into an unsigned int
 - add margins in chart in such a way that ylabels are not cut
 - the number of points with which routes are plotted should be adapted dynamically to the zooming factor
 - create your own color class and a function to elegantly read colors from file.
- add separator between recent items and non-recent items in BodyField->name
 - replaces multiple ifs with switch cases
 - in the 3d projection two lengths l and d are redundant: remove one
 - fill the cases in Route::draw_3D for l and o
 - check why body field gets empty after you entered body name and kill focus
 - replace 'lambda_rotation_axis.set(String("lambda rotation axis"), -atan(gsl_vector_get(rp, 0), gsl_vector_get(rp, 1)), String(""));
 phi_rotation_axis.set(String("phi rotation axis"), asin(gsl_vector_get(rp, 2)), String(""));' with a function
 */



wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    /*
    String s("si.");
    File file_recent;
    
    file_recent.set_name(String(path_file_recent));
    file_recent.open(String("in"), String(""));
    
    s.write_to_file(String("risposta"), file_recent, String(""));
    file_recent.close(String(""));
    */
    /*
    Angle a;
    a.set(String(""), 5.5, String(""));
    string test = a.to_string(String(""), display_precision, true);
    */
     
    /*
    Angle a, b, c;
    a.set(String(""), 0.2, String(""));
    b.set(String(""), 2, String(""));
    c.set(String(""), 4, String(""));
    Rotation R(a, b, c);
    
    a.set(String(""), .232, String(""));
    b.set(String(""), .34, String(""));
    c.set(String(""), 2.66, String(""));
    Rotation S(a, b, c);
    
    Rotation T;
    T = R*S;
    R.print(String("R"), String("\t\t"), cout);
    S.print(String("S"), String("\t\t"), cout);
    T.print(String("composition"), String("\t\t"), cout);
    */
 
    
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
