/*
 
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -lboost_filesystem -lboost_system -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions --std=c++17  -O3
 */

//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "main.h"
#include "lib.cpp"

/*
 
 notes:
 =======
 - sometimes the drag operation with mercator projection ends up to the original positon because you end up hitting the max min latitude when dragging 
 - to make the app executable: $chmod +x /Thelemacus.app/Contents/MacOS/Thelemacus
 - to watch a variable any time it changes value, 1. set a breakpoint 2. in the lldb console type watch set variable MyClass.variable_in_the_class 3. Press play again.
 - ChartDirector uses colors in the format 0xRRGGBB, while wxWidgets in format 0xBBGGRR
 - when a new chartframe is created, call ShowCharts to re-arrange all chartframes.
 - when a new chartframe is created, position it so it is not hidden by the menu bar on top of the screen
 - per creare un progetto xcode con wxwidgets 1. decomprimi il tar.gz di wxwidgets 2. in samples/minimal/ modifical minimal_cocoa mettendoci i files C++ e le referenze in system header search path, library search path, le linking flag, etc.
 - se xcode ti crea nuove copie dei file .cpp e .h quando li aggiungi a un progetto, fai attenzione a deselezioare 'copy items if needed' nel momento in cui li aggiungi al progetto. Non mettere i file in /src/, altrimenti xcode ti crea una copie di quei file nella directory in cui si trova il file .xcodeproj
 
 
 ********** THINGS TO ADD ************
 - when switching to dark mode, buttons with images must have white and black inverted -> adjust color of PNG images according to app color
 - fill up selection 'rectangle' in 3D projection by using a bundle of curves at fixed lat or lon
 - change     highlight_color = color_selected_item and     highlight_font = default_font;
 with a better  choice
 - add + / - button in chartframe to zoom in / out
 - add instrumental error
 - add lambert projection and 3D sphere with no projection
 - add separator between recent items and non-recent items in BodyField->name
 -  in the 3d projection, plot the number of points in each lat/long square according to its inclination with respect to the observer
 - set up a proper output to a log file
 - set parents to constructors of all frames that you defined
 - in Route::draw_3D, code the part for loxodrome curves
 - move all constants to the init file , such as  the 0.15 in  'position_plot_area = wxPoint((size_chart.GetWidth())*0.15, (size_chart.GetHeight())*0.1);'
 - when one tries to drag a route , ask the user whether he wants to disconnect it from the sight by allowing no for an answer
 - replace use of epsilon_double with more proper methods 
 - Make sure that Draw is not called uselessly some times
 - in DrawPanel::draw_3D : when drawing minor ticks on the x axis : because I am drawing a loxodrome, I am using the old function Route::draw -> replace this with Route::draw_3D in the future
 - move all stuff which is general enough in the code to MyApp class
 
 ********** THINGS TO FIX ************
 - remove useless stuff from init.txt
 - do not allocate a new wxDC every time you call Render*
- check whetehr '(draw_panel->memory_dc).SetBrush(wxBrush(wxGetApp().background_color, wxBRUSHSTYLE_TRANSPARENT));' is the reason why parallels and meridians are not shown on MSW
 - Do not write on file every time you update the recent items, write/read from to file only when you open /close the app
 - On MSW the old curves are shown when dragging the chart -> fix it
 - buttons on bottom of ListFrame are cut on Enrico's computer (fixed it on mine, check whether it has been fixed on another computer too)
 - check that WXK_PLUS does not depend on the computer
 
 ---
 

 for MSW:
 - images are not loaded correctly
 */

//this function is executed reguarly over time, to check some things
void MyApp::OnTimer([[maybe_unused]] wxTimerEvent& event){
    
    if(dark_mode != (settings->GetAppearance().IsDark())){
        //if the dark mode of the operating system has changed
        
        if((settings->GetAppearance().IsDark())){
            //the system is in dark mode -> set image path equal to the /Dark folder

            image_directory = run_directory.append(String("Contents/Resources/Images/Dark/"));
            
        }else{
            //the system is in light mode ->  set image path equal to the /Light folder
            
            image_directory = run_directory.append(String("Contents/Resources/Images/Light/"));

        }
        
        //I re-draw all the ChartFrames so their fore/background colors will be adapted to the new mode of the operating system.
        list_frame->DrawAll();
    }
    
    dark_mode = (settings->GetAppearance()).IsDark();
    
}

//if the user presses Ctrl + Q to exit the app, I call this function which prompts a message frame
template<class T> void MyApp::OnPressCtrlQ([[maybe_unused]] T& event){
    
    //    for(; 0<chart_frames.size(); ){
    //        (chart_frames[0])->OnClose(event);
    //    }
    //    
    //    Destroy();
    //    Close(true);
    
    //    return this->wxApp::OnPressCtrlQ();
    UnsetIdling<ListFrame>* unset_idling;
    CloseFrame<ListFrame>* close;
    PrintQuestion<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame> >* print_question;
    unset_idling = new UnsetIdling<ListFrame>(list_frame);
    close = new CloseFrame<ListFrame>(list_frame);
    
    //    PrintMessage<ListFrame, Close<ListFrame> >* print_info_message;
    
    print_question = new PrintQuestion<ListFrame, CloseFrame<ListFrame>, UnsetIdling<ListFrame> >(list_frame, close, unset_idling);
    
    print_question->SetAndCall(NULL, String("You pressed CTRL+Q"), String("Do you want to quit the app?"), String("Yes"), String("No"));
    
    //    QuestionFrame<Close<ListFrame>, UnsetIdling<ListFrame>>* question_frame = new QuestionFrame<Close<ListFrame>, UnsetIdling<ListFrame>>(NULL,
    //                                                                                                        close,
    //                                                                                                        String("Yes"),
    //                                                                                                        unset_idling, String("No"),
    //                                                                                                        "",
    //                                                                                                        "Do you want to close the app?",
    //                                                                                                        wxDefaultPosition,
    //                                                                                                        wxDefaultSize,
    //                                                                                                        String(""));
    //    question_frame->Show(true);
    
    
    //    print_info_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(list_frame, unset_idling);
    //    print_info_message = new PrintMessage<ListFrame, Close<ListFrame> >(list_frame, close);
    //    print_info_message->SetAndCall(NULL, String("You want to exit the app"), String("Exiting the app"));
    
    
    
    
    //    delete list_frame;
    //
    
    
}

//compute the astronomical position and updated all the GUI fields in set() and re-draws everything
template<class T> void ListFrame::ComputePosition([[maybe_unused]] T& event){
    
    PrintQuestion<ListFrame, AllRoutes, SomeRoutes>* print_question;
    AllRoutes* all_routes;
    SomeRoutes* some_routes;
    
    all_routes = new AllRoutes(this);
    some_routes = new SomeRoutes(this);
    print_question = new PrintQuestion<ListFrame, AllRoutes, SomeRoutes>(this, all_routes, some_routes);
    
    selecting_route_for_position = true;
    
    //ask the user whether he/she wants to transport the sight with a an existing route or with a new route.
    print_question->SetAndCall(NULL, String("You want to determine the astronomical position"), String("With what route do you want to do it?"), String("All routes"), String("Some routes"));
    
    
    
    
    
}

//shows all ChartFrames and positions them properly on the screen 
template<class T> void MyApp::ShowChart([[maybe_unused]] T& event){
    
    unsigned int i;
    //the spacing between one frame and another in the x and y direction, respectively
    double delta_x, delta_y;
    
    
    i = (((list_frame->menu_bar)->GetSize()).GetHeight());
    
    if(((list_frame->chart_frames).size()) > 1){
        //if ((list_frame->chart_frames).size() > 1 it makes sens to introduce delta_x, delta_y
        
        delta_x = ( ((double)(rectangle_display.GetWidth())) - ( (double)(((((list_frame->chart_frames)[0])->GetSize()).GetWidth()) + ((((list_frame->chart_frames)[((list_frame->chart_frames).size())-1])->GetSize()).GetWidth())) ) / 2.0  - 2.0 * ((wxGetApp().border).value) ) / ((double)(((list_frame->chart_frames).size())-1));
        delta_y = ( ((double)(rectangle_display.GetHeight())) - ( (double)(((((list_frame->chart_frames)[0])->GetSize()).GetHeight()) + ((((list_frame->chart_frames)[((list_frame->chart_frames).size())-1])->GetSize()).GetHeight())) ) / 2.0 - 2.0 * ((wxGetApp().border).value) ) / ((double)(((list_frame->chart_frames).size())-1));
        
    }else{
        //if ((list_frame->chart_frames).size() <= 1, it does not make sense to define delta_x, delta_y, and I set
        
        delta_x = 0.0;
        delta_y = 0.0;
        
    }
    
    
    for(i=0; i<((list_frame->chart_frames).size()); i++){
        
        
        
        ((list_frame->chart_frames)[i])->Show(true);
        ((list_frame->chart_frames)[i])->Raise();
        ((list_frame->chart_frames)[i])->SetPosition(wxPoint(
                                                             
                                                             
                                                             ( ((double)((((list_frame->chart_frames)[0])->GetSize()).GetWidth())) - ((double)((((list_frame->chart_frames)[i])->GetSize()).GetWidth())) ) / 2.0 + ((wxGetApp().border).value) + delta_x*((double)i)
                                                             
                                                             
                                                             ,
                                                             
                                                             /*here I shift everything down on the screen by the height of the menu_bar, because otherwise the ChartFrame on the top would be partially corvered by the menu bar and the one on the bottom would leave an empty space t the bottom of the screen */
                                                             (((list_frame->menu_bar)->GetSize()).GetHeight()) +
                                                             ( ((double)((((list_frame->chart_frames)[0])->GetSize()).GetHeight())) - ((double)((((list_frame->chart_frames)[i])->GetSize()).GetHeight())) ) / 2.0 + ((wxGetApp().border).value) + delta_y*((double)i)
                                                             
                                                             ));
        
    }
    
}

//shows ListFrame and positions it properly on the screen
void MyApp::ShowList([[maybe_unused]] wxCommandEvent& event){
    
    list_frame->SetPosition(wxPoint(0, /*move down ListFrame to take account of the menu_bar*/((list_frame->menu_bar)->GetSize()).GetHeight()));
    list_frame->Raise();
    
}

//writes into this->run_directory the path where the executable is currently running
void MyApp::where_am_I([[maybe_unused]] String prefix){
    
    stringstream ins;
    
    ins.str("");
    //note that here boost::dll::program_location() may return a path with './' at the end, but this has no effect because ./ simply says to stay in the same path
    ins << (boost::dll::program_location().parent_path());
    run_directory.value = ins.str().c_str();
    
    run_directory.print(String("Non-formatted run directory"), true, String("*******"), cout);
    
    
    //remove " from run_directory
    run_directory.value.erase(std::remove(run_directory.value.begin(), run_directory.value.end(), '"'), run_directory.value.end());
    
    run_directory.appendto(String("/../../"));
    run_directory.print(String("Formatted run directory"), true, String("*******"), cout);
    
    
}

bool MyApp::OnInit(){
        
    unsigned int i;
    Int n_chart_frames;
    stringstream s;
    String projection, temp;
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    wxDisplay display;
    wxCommandEvent dummy;
    //this contains the current time, the time of the transition from night to day (dawn), and the time of the transition from day to night (dusk)
    //    Chrono current_time, dawn, dusk;
    
    degree_symbol = String("\u00b0");
    
    //detect the operating system
#ifdef _WIN32
    
    cout << "You have Windows Operating System" << "\n";
    //to run the app with Visual Studio on Windows
    run_directory = String("C:/Users/miche/Documents/sight_reduction_program/");
    
#endif
    
#ifdef __APPLE__
    
    cout << "You have Apple Operating System" << "\n";
    
    //to build the app on mac
    //        where_am_I(String(""));
    //to develop the app with Xcode on OSX
    run_directory = String("/Users/macbookpro/Documents/sight_reduction_program/");
    
#endif
    
    

    
    
    settings = new wxSystemSettings();
    timer = new wxTimer();
    
    wxImage::AddHandler(new wxPNGHandler);
    //    wxImage::AddHandler(new wxBMPHandler);
    
    rectangle_display = (display.GetClientArea());
    rectangle_display.SetWidth((int)((double)rectangle_display.GetWidth()));
    rectangle_display.SetHeight((int)((double)rectangle_display.GetHeight()));
    
    
    //directories are set dynamically from run_directory
    path_file_init  = run_directory.append(String("Contents/Resources/Data/init.txt"));
    code_directory = run_directory;
    data_directory = run_directory.append(String("Contents/Resources/Data/"));
    if((settings->GetAppearance().IsDark())){
        //the system is in dark mode
        image_directory = run_directory.append(String("Contents/Resources/Images/Dark/"));
    }else{
        //the system is in light mode
        image_directory = run_directory.append(String("Contents/Resources/Images/Light/"));
    }
    default_open_directory = data_directory;
    
    //read the file names and prenend to the file name the respective directory where the file is located -> obtain the file path
    //files in code directory
    temp.read_from_file(String("name file utc date and time"), String(path_file_init), String(""));
    path_file_utc_date_and_time = code_directory.append(temp);
    
    temp.read_from_file(String("name file recent"), String(path_file_init), String(""));
    path_file_recent = data_directory.append(temp);
    
    temp.read_from_file(String("name file temp"), String(path_file_init), String(""));
    path_file_temp = code_directory.append(temp);
    
    //files in data directory
    temp.read_from_file(String("name file catalog"), String(path_file_init), String(""));
    path_file_catalog = data_directory.append(temp);
    
    temp.read_from_file(String("name file coastline data blocked"), String(path_file_init), String(""));
    path_file_coastline_data_blocked = data_directory.append(temp);
    
    temp.read_from_file(String("name file n line"), String(path_file_init), String(""));
    path_file_n_line = data_directory.append(temp);
    
    //files in image directory
    temp.read_from_file(String("name file app icon"), String(path_file_init), String(""));
    path_file_app_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file error icon"), String(path_file_init), String(""));
    path_file_error_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file warning icon"), String(path_file_init), String(""));
    path_file_warning_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file question icon"), String(path_file_init), String(""));
    path_file_question_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file plus icon"), String(path_file_init), String(""));
    path_file_plus_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file list icon"), String(path_file_init), String(""));
    path_file_list_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file position icon"), String(path_file_init), String(""));
    path_file_position_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file map icon"), String(path_file_init), String(""));
    path_file_map_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file pencil icon"), String(path_file_init), String(""));
    path_file_pencil_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file trash icon"), String(path_file_init), String(""));
    path_file_trash_icon = image_directory.append(temp);
    
    temp.read_from_file(String("name file arrow icon"), String(path_file_init), String(""));
    path_file_arrow_icon = image_directory.append(temp);

    temp.read_from_file(String("name file disconnect icon"), String(path_file_init), String(""));
    path_file_disconnect_icon = image_directory.append(temp);

    
    data_precision.read_from_file(String("data precision"), String(path_file_init), String(""));
    display_precision.read_from_file(String("display precision"), String(path_file_init), String(""));
    time_check.read_from_file(String("time check"), String(path_file_init), String(""));
    time_zone.read_from_file(String("time zone"), String(path_file_init), String(""));
    
    max_lat.read_from_file(String("maximal latitude coastline data"), String(path_file_init), String(""));
    min_lat.read_from_file(String("minimal latitude coastline data"), String(path_file_init), String(""));
    
    size_small_button_over_width_screen.read_from_file(String("size small button over width screen"), String(path_file_init), String(""));
    size_large_button_over_width_screen.read_from_file(String("size large button over width screen"), String(path_file_init), String(""));
    size_icon_over_width_screen.read_from_file(String("size icon over width screen"), String(path_file_init), String(""));
    size_message_image_over_width_screen.read_from_file(String("size message image over width screen"), String(path_file_init), String(""));
    point_size.read_from_file(String("point size"), String(path_file_init), String(""));
    
    
    //set size_small/large_button from size_small_button_over_width_screen and size_large_button_over_width_screen
    size_small_button = wxSize(
                               ((wxGetApp().rectangle_display).GetWidth())*((wxGetApp().size_small_button_over_width_screen).value),
                               ((wxGetApp().rectangle_display).GetWidth())*((wxGetApp().size_small_button_over_width_screen).value)
                               );
    size_large_button = wxSize(
                               ((wxGetApp().rectangle_display).GetWidth())*((wxGetApp().size_large_button_over_width_screen).value),
                               ((wxGetApp().rectangle_display).GetWidth())*((wxGetApp().size_large_button_over_width_screen).value)
                               );
    
    
    length_plot_area_over_length_chart.read_from_file(String("length of plot area over length of chart"), String(path_file_init), String(""));
    length_chart_over_length_chart_frame.read_from_file(String("length of chart over length of chart frame"), String(path_file_init), String(""));
    length_border_over_length_screen.read_from_file(String("length of border over length of screen"), String(path_file_init), String(""));
    
    border.set(String("border"), (rectangle_display.GetWidth())*(length_border_over_length_screen.value), String(""));
    
    //read n_points_minor_ticks from file_init
    n_points_minor_ticks.read_from_file(String("number of points for minor ticks"), String(path_file_init), String(""));
    //read relative_displacement from file_init
    relative_displacement.read_from_file(String("relative displacement"), String(path_file_init), String(""));
    //read standard_thickness_over_length_screen from file_init
    standard_thickness_over_length_screen.read_from_file(String("standard thickness over length screen"), String(path_file_init), String(""));
    //read large_thickness_over_length_screen from file_init
    large_thickness_over_length_screen.read_from_file(String("large thickness over length screen"), String(path_file_init), String(""));
    //read color horizon from file
    color_horizon.read_from_file(String("color horizon"), String(path_file_init), String(""));
    //read color selected item from file
    color_selected_item.read_from_file(String("color selected item"), String(path_file_init), String(""));
    //read tick length over width plot area from file_init
    tick_length_over_width_plot_area.read_from_file(String("tick length over width plot area"), String(path_file_init), String(""));
    //read tick length over width plot area from file_init
    tick_length_over_aperture_circle_observer.read_from_file(String("tick length over aperture circle observer"), String(path_file_init), String(""));
    //read min_crossing_angle from file_init
    min_crossing_angle.read_from_file(String("minimal crossing angle between circles of equal altitude"), (wxGetApp().path_file_init), String(""));
    
    
    
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
    highlight_color = color_selected_item;
    dark_mode = (settings->GetAppearance()).IsDark();
    
    //    default_projection.read_from_file(String("default projection"), String(path_file_init), String(""));
    
    
    list_frame = new ListFrame(this, "Unnamed", "", wxDefaultPosition, wxDefaultSize, String(""));
    list_frame->Show(true);
    
    
    //allocate and show the chart frames
    n_chart_frames.read_from_file(String("number chart frames"), String(path_file_init), String(""));
    (list_frame->chart_frames).resize(n_chart_frames.value);
    for(i=0; i<(list_frame->chart_frames).size(); i++){
        
        projection = String((((i % 2) == 0) ? "Mercator" : "3D"));
        
        //open a Mercator projection for even is and a 3D projection for odd is
        s.str("");
        s << "Chart #" << i+1 << " - " << projection.value << " projection";
        
        string dummy = s.str();
        
        (list_frame->chart_frames)[i] = new ChartFrame(
                                                       list_frame,
                                                       projection,
                                                       s.str(),
                                                       wxDefaultPosition,
                                                       wxDefaultSize,
                                                       String("")
                                                       );
        //        ((list_frame->chart_frames)[i])->Show(true);
        //        ((list_frame->chart_frames)[i])->Raise();
        
    }
    
    ShowChart(dummy);
    
    //bring either of these wxFrames to front
    //    list_frame->Raise();
    
    //fore/background color is determined from the default background color of extract_colors
    foreground_color = Color(list_frame->extract_colors->GetForegroundColour());
    background_color = Color(list_frame->extract_colors->GetBackgroundColour());
    
    
    //extracts the default font and creates a error_font, obtained from default font by setting its weight to wxFONTWEIGHT_BOLD
    default_font = (list_frame->extract_colors->GetFont());
    error_font = (list_frame->extract_colors->GetFont());
    highlight_font = default_font;
    error_font.SetWeight(wxFONTWEIGHT_BOLD);
    
    timer->Start(/*time_check is converted in milliseconds, because Start() takes its first argument i milliseconds*/((time_check.h)*60.0*60.0 + (time_check.m)*60.0 + (time_check.s)) * 1000.0, wxTIMER_CONTINUOUS);
    
    
    return true;
    
}
