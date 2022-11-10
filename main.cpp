/*
 
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/boost_1_66_0/ -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -O3
 */

//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "main.h"
#include "lib.cpp"

/*
 notes:
 - to watch a variable any time it changes value, 1. set a breakpoint 2. in the lldb console type watch set variable MyClass.variable_in_the_class 3. Press play again.
 - ChartDirector uses colors in the format 0xRRGGBB, while wxWidgets in format 0xBBGGRR
 
 ********** THINGS TO ADD ************

 - add instrumental error
 - add lambert projection and 3D sphere with no projection
 - add separator between recent items and non-recent items in BodyField->name
 -  in the 3d projection, plot the number of points in each lat/long square according to its inclination with respect to the observer
 - set up a proper output to a log file
 - set parents to constructors of all frames that you defined

 ********** THINGS TO FIX ************
 
 - transfrom all angular quantities in units of radians
 - make sure that Route::draw is used every time a Route is drawn (no useless copies of the same lines of code) -> use the fact that in the mercator projection loxodromes are straight lines to incoprporate them in the new Draw function
 - in Route::draw_3D, code the part for loxodrome curves
 - in DrawPanel::draw_3D : when drawing minor ticks on the x axis : because I am drawing a loxodrome, I am using the old function Route::draw -> replace this with Route::draw_3D in the future
 -   sort(t->begin(), t->end()); is wrong: you should take into account the case where the midpoint between t.begin and t.end lies outside circle_obsrever
 - add check on zoom factor in OnMouseRightDown for the 3D projections
 - add the stuff on setting d, zoom factor, x_y, lambda_phi in Draw_3D into Draw_Mercator too, and remove it elsewhere if unnecessary
 - remove zoom_factor, because it is related to omega
 - fix bug: when one clicks with the mouse on the second wxTextCtrl in ChronoField, nothing happens
 - if displayprecision is too small, contiguous labels on paralles / merdisians show the same value
- in Mercator projection, if you drag and go through the lambda = pi meridian there are problems
 */

//this function is executed reguarly over time, to check some things
void MyApp::OnTimer(wxTimerEvent& event){

    if(dark_mode != ((settings->GetAppearance()).IsDark())){
        //if the dark mode of the operating system has changed, I re-draw all the ChartFrames so their fore/background colors will be adapted to the new mode of the operating system.
        list_frame->DrawAll();
    }
     
    dark_mode = (settings->GetAppearance()).IsDark();
     
}

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
    
    /*
     wxBitmap *michele;
     wxMemoryDC dc;
     
     michele = new wxBitmap(100, 100);
     dc.SelectObject(*michele);
     dc.SetPen(wxPen(*wxRED, 1));
     dc.DrawLine(0,0,10,10);
     
     */
    
    unsigned int i;
    Int n_chart_frames;
    stringstream s;
    String default_projection;
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    //this contains the current time, the time of the transition from night to day (dawn), and the time of the transition from day to night (dusk)
//    Chrono current_time, dawn, dusk;
    
    settings = new wxSystemSettings();
    timer = new wxTimer();
    
    wxImage::AddHandler(new wxPNGHandler);
    
    data_precision.read_from_file(String("data precision"), String(path_file_init), String(""));
    display_precision.read_from_file(String("display precision"), String(path_file_init), String(""));
    time_check.read_from_file(String("time check"), String(path_file_init), String(""));

    max_lat.read_from_file(String("maximal latitude coastline data"), String(path_file_init), String(""));
    min_lat.read_from_file(String("minimal latitude coastline data"), String(path_file_init), String(""));
    
    length_plot_area_over_length_chart.read_from_file(String("length of plot area over length of chart"), String(path_file_init), String(""));
    length_chart_over_length_chart_frame.read_from_file(String("length of chart over length of chart frame"), String(path_file_init), String(""));
    length_border_over_length_screen.read_from_file(String("length of border over length of screen"), String(path_file_init), String(""));
    
    
    //read the time, and set the background color to either the day or night background color, which are read from file
//    time_zone.read_from_file(String("time zone"), String(path_file_init), String(""));
//    dawn.read_from_file(String("dawn"), String(path_file_init), String(""));
//    dusk.read_from_file(String("dusk"), String(path_file_init), String(""));
//    current_time.set_current(time_zone, String(""));
    
    
//    if((current_time < dawn) || (current_time > dusk)){
//        //we are at night -> set background color to night mode
//
//        foreground_color.read_from_file(String("night foreground color"), String(path_file_init), String(""));
//
//    }else{
//        //we are at day -> set background color ot day mode
//
//        foreground_color.read_from_file(String("day foreground color"), String(path_file_init), String(""));
//
//    }
    
    timer->Bind(wxEVT_TIMER, &MyApp::OnTimer, this);

    
    
    foreground_color = Color(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    background_color = Color(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
    error_color.read_from_file(String("error color"), String(path_file_init), String(""));
    dark_mode = (settings->GetAppearance()).IsDark();

    rectangle_display = (display.GetClientArea());
    rectangle_display.SetWidth((int)((double)rectangle_display.GetWidth()));
    rectangle_display.SetHeight((int)((double)rectangle_display.GetHeight()));
    
    
    list_frame = new ListFrame(this, "List of sights", "", wxDefaultPosition, wxDefaultSize, String(""));
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
        ((list_frame->chart_frames)[i])->Raise();

    }
    
    //bring either of these wxFrames to front
//    list_frame->Raise();
    
    //fore/background color is determined from the default background color of extract_colors
    foreground_color = Color(list_frame->extract_colors->GetForegroundColour());
    background_color = Color(list_frame->extract_colors->GetBackgroundColour());

    
    //extracts the default font and creates a error_font, obtained from default font by setting its weight to wxFONTWEIGHT_BOLD
    default_font = (list_frame->extract_colors->GetFont());
    error_font = (list_frame->extract_colors->GetFont());
    error_font.SetWeight(wxFONTWEIGHT_BOLD);
    
    timer->Start(/*time_check is converted in milliseconds, because Start() takes its first argument i milliseconds*/((time_check.h)*60.0*60.0 + (time_check.m)*60.0 + (time_check.s)) * 1000.0, wxTIMER_CONTINUOUS);
    
    
    return true;
    
}
