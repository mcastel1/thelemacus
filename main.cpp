/*
 
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -I/Applications/boost_1_66_0/ -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib -O3
 */

//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "main.h"
#include "lib.cpp"

/*
 notes:
 - to watch a variable any time it changes value, 1. set a breakpoint 2. in the lldb console type watch set variable MyClass.variable_in_the_class 3. Press play again.
 - set parents to constructors of all frames that you defined
 - add instrumental error
 - add lambert projection and 3D sphere with no projection
 - set up a proper output to a log file
 - add separator between recent items and non-recent items in BodyField->name
 - when I replace sample_sight.txt with a file with only a Route, there is a debugging error being prompted
 - conversion                                    (parent->color_horizon).GetRGBA() does not give the right color
 - check all times that GeoTo3D is called to see whether they are compatible with the new modification
 - transfrom all angular qantities in units of radians
 - in Render_Mercator, transform loop to draw labels into loop over coordinates of a geographic Position q, then transform q to draw panel coordinates and obtain p, and use p to set the location of the label
 - make sure that Route::draw is used every time a Route is drawn (no useless copies of the same lines of code)
 - transforms all  exprssions such as cos((route.reference_position.phi.value)) into cos((route.reference_position.phi))
 - in Route::draw_3D, code the part for loxodrome curves
 - get rid of eventual superfluous if condition  (which checks whether angles are multiples of one degree) when drawing labels in the 3d projection
 - in DrawPanel::draw_3D : when drawing minor ticks on the x axis : because I am drawing a loxodrome, I am using the old function Route::draw -> replace this with Route::draw_3D in the future
 -                                 sort(t->begin(), t->end()); is wrong: you should take into account the case where the midpoint between t.begin and t.end lies outside circle_obsrever
 - revise Route::intersection so as to make it work also in the singular case where this->reference_position->phi = pi/2 and route.reference_position.phi = pi/2 etc
 - in DrawPanel::Draw_3D delta_lambda is used without being initialized (?)
 - check fabs(K*((temp.lambda).value) - ((double)round(K*((temp.lambda).value)))) < delta_lambda/2.0 in  DrawPanel::Render_3D: it seems that this line has been written as if delta_lambda were expressed in degrees, while it is expressed in radians
 - it seems that DrawPanel::Draw_3D/Mercator  is called multiple times at the beginning of the code, and this is pointless -> check
 - if two entries in file_init have a common word, make sure that they are read correctly by the functions which read from file.
 - add check on zoom factor in OnMouseRightDown for the 3D projections
 - add the stuff on setting d, zoom factor, x_y, lambda_phi in Draw_3D into Draw_Mercator too, and remove it elsewhere if unnecessary
- remove zoom_factor, because it is related to omega
 - fix bug: when one clicks with the mouse on the second wxTextCtrl in ChronoField, nothing happens
 - uncomment at '//uncomment this at the end' to display coastlines at the end
 - when you are about to select and existing route to transport a sight or position, disable in list_routes the routes which come from a sight
 - transform all the instances where you compute the span between two angles with Angle::span
 - when you delete a Position, make sure that buttons that modify and delete a position are disabled 
 - in DrawPanel::Draw_Mercator, delete the setting and any use  of circle_observer, because you shouldn't need it for mercator projection. all you need for mercator projection if rectangle_observer
 - for night mode: add backdrount color to all frames, and change color of text to adapt to day/night mode
 - remove all wxSize(20,20) for absolute sizes
 */




bool MyApp::OnInit(){
    
    /*
    gsl_vector *a, *b, *c;
    a = gsl_vector_alloc(3);
    b = gsl_vector_alloc(3);
    
    gsl_vector_set(a, 0, 3.43);
    gsl_vector_set(a, 1, 2.43);
    gsl_vector_set(a, 2, 43);
    
    gsl_vector_set(b, 0, 2.3);
    gsl_vector_set(b, 1, .12);
    gsl_vector_set(b, 2, .3);
    
    cross(a, b, &c);
    
    cout << "\t\t c = " << gsl_vector_get(c, 0) << " " << gsl_vector_get(c, 1) << " " << gsl_vector_get(c, 2) << "\n";    
     */
    
    /*
     string a = "ciao";
     string b = "CiaOx";
     
     cout << "comparison = " << boost::iequals(a, b) << "\n";
     
     
     */
    
    /*
    Route cape_horn;
    Rectangle ice;
    vector<Angle> t;
    bool output;
    
    
    cape_horn = Route(String("c"), Position(Angle(-0.9), Angle(-1)), Angle(0.1));
    ice = Rectangle(Position(Angle(-1), Angle(-0.5)), Position(Angle(-1.2), Angle(-1.2)));
     
    output = cape_horn.is_included_in(ice, &t, String(""));
    
    
    */
    
    /*
    Color my_color(232,23,13);
    int j = my_color.ToRGB();
    */
    
    unsigned int i;
    Int n_chart_frames;
    stringstream s;
    String default_projection;
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    //this contains the current time, the time of the transition from night to day (dawn), and the time of the transition from day to night (dusk)
    Chrono current_time, dawn, dusk;

    
    wxImage::AddHandler(new wxPNGHandler);
    
    data_precision.read_from_file(String("data precision"), String(path_file_init), String(""));
    display_precision.read_from_file(String("display precision"), String(path_file_init), String(""));
    
    max_lat.read_from_file(String("maximal latitude coastline data"), String(path_file_init), String(""));
    min_lat.read_from_file(String("minimal latitude coastline data"), String(path_file_init), String(""));

    length_plot_area_over_length_chart.read_from_file(String("length of plot area over length of chart"), String(path_file_init), String(""));
    length_chart_over_length_chart_frame.read_from_file(String("length of chart over length of chart frame"), String(path_file_init), String(""));
    length_border_over_length_screen.read_from_file(String("length of border over length of screen"), String(path_file_init), String(""));
    
    
    //read the time, and set the background color to either the day or night background color, which are read from file
    time_zone.read_from_file(String("time zone"), String(path_file_init), String(""));
    dawn.read_from_file(String("dawn"), String(path_file_init), String(""));
    dusk.read_from_file(String("dusk"), String(path_file_init), String(""));
    current_time.set_current(time_zone, String(""));
    

    if((current_time < dawn) || (current_time > dusk)){
        //we are at night -> set background color to night mode
        
        foreground_color.read_from_file(String("night foreground color"), String(path_file_init), String(""));
        background_color.read_from_file(String("night background color"), String(path_file_init), String(""));

    }else{
        //we are at day -> set background color ot day mode
        
        foreground_color.read_from_file(String("day foreground color"), String(path_file_init), String(""));
        background_color.read_from_file(String("day background color"), String(path_file_init), String(""));
        
    }


    
    rectangle_display = (display.GetClientArea());
    rectangle_display.SetWidth((int)((double)rectangle_display.GetWidth()));
    rectangle_display.SetHeight((int)((double)rectangle_display.GetHeight()));
    
    
    ListFrame *list_frame = new ListFrame(this, "List of sights", "", wxDefaultPosition, wxDefaultSize, String(""));
    list_frame->Show(true);
    
    
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
                                                               0 + (i+1)*(int)(((double)(rectangle_display.GetWidth()))/20.0),
                                                               0 + (i+1)*(int)(((double)(rectangle_display.GetHeight()))/20.0)
                                                               ),
                                                       wxDefaultSize,
                                                       String("")
                                                       );
        ((list_frame->chart_frames)[i])->Show(true);
        
    }
    
    //bring either of these wxFrames to front
    //    list_frame->Raise();
    ((list_frame->chart_frames)[0])->Raise();
    
    
    return true;
    
}
