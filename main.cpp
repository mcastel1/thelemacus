/*
 
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib -O3
 */

//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0

#include "main.h"
#include "lib.cpp"

/*
 notes: set parents to constructors of all frames that you defined
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
 - add separator between recent items and non-recent items in BodyField->name
 - fill the cases in Route::draw_3D for l and o
 - check why body field gets empty after you entered body name and kill focus
 - replace 'lambda_rotation_axis.set(String("lambda rotation axis"), -atan(gsl_vector_get(rp, 0), gsl_vector_get(rp, 1)), String(""));
 phi_rotation_axis.set(String("phi rotation axis"), asin(gsl_vector_get(rp, 2)), String(""));' with a function
 - when I translate an orthodrome with mouse drag, its reference_position assumes nan coordinates
 - change Route -> alpha to Z
 - fix issue that when you scroll and reach the chart boundaries, tons of error dialog windows are shown (maybe remove skip?)
 - when I replace sample_sight.txt with a file with only a Route, there is a debugging error being prompted
 - add margins in ChartFrame
 - add ft unit for lengths
 - conversion                                    (parent->color_horizon).GetRGBA() does not give the right color
 - check all times that GeoTo3D is called to see whether they are compatible with the new modification
 - transfrom all angular qantities in units of radians
 - in Render_Mercator, transform loop to draw labels into loop over coordinates of a geographic Position q, then transform q to draw panel coordinates and obtain p, and use p to set the location of the label
 - make sure that Route::draw is used every time a Route is drawn (no useless copies of the same lines of code)
 - transforms all remaining exprssions such as cos((route.reference_position.phi.value)) into cos((route.reference_position.phi))
 - update Route::draw
 - in Route::draw_3D, code the part for loxodrome and orthodrome curves
 - get rid of eventual superfluous if condition  (which checks whether angles are multiples of one degree) when drawing labels in the 3d projection
 - fix issue that a route is not drawn when it does not touch circle_observer and it is entirelty within it 
 */



wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit(){
    
    unsigned int i;
    Int n_chart_frames;
    stringstream s;
    String default_projection;
    
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    wxRect rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*0.75);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*0.75);
    
    
    ListFrame *list_frame = new ListFrame("List of sights", "", wxDefaultPosition, wxDefaultSize, String(""));
    list_frame->Show(true);
    
    
    //
    cout << "Intersection 1-0= " << ((list_frame->plot->route_list)[1]).intersection(((list_frame->plot->route_list)[0]), NULL, String("****")) << "\n";
    cout << "Intersection 2-0= " << ((list_frame->plot->route_list)[2]).intersection(((list_frame->plot->route_list)[0]), NULL, String("****")) << "\n";
    cout << "Intersection 3-0= " << ((list_frame->plot->route_list)[3]).intersection(((list_frame->plot->route_list)[0]), NULL, String("****")) << "\n";
    
    cout << "Intersection 2-1= " << ((list_frame->plot->route_list)[2]).intersection(((list_frame->plot->route_list)[1]), NULL, String("****")) << "\n";
    cout << "Intersection 3-1= " << ((list_frame->plot->route_list)[3]).intersection(((list_frame->plot->route_list)[1]), NULL, String("****")) << "\n";
    //
    
    //allocate and show the chart frames
    n_chart_frames.read_from_file(String("number chart frames"), String(path_file_init), String(""));
    (list_frame->chart_frames).resize(n_chart_frames.value);
    for(i=0; i<(list_frame->chart_frames).size(); i++){
        
        default_projection.read_from_file(String("default projection"), String(path_file_init), String(""));
        
        
        s.str("");
        s << "Chart #" << i+1 << " - " << (default_projection.value) << " projection";
        
        (list_frame->chart_frames)[i] = new ChartFrame(
                                                       list_frame,
                                                       String(""),
                                                       s.str(),
                                                       /*place each ChartFrame by shifting it with respect to the top-left corner of the screen*/
                                                       wxPoint(
                                                               0 + i*(int)(((double)(rectangle.GetWidth()))/20.0),
                                                               0 + i*(int)(((double)(rectangle.GetHeight()))/20.0)
                                                               ),
                                                       wxDefaultSize,
                                                       String("")
                                                       );
        (list_frame->chart_frames)[i]->Show(true);
        
    }
    
    return true;
    
}
