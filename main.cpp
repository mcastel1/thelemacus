/*
 
 
 - compile on APPLE:
 g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib
 
 To run on WIN32, the resource file is resource_file_windows.rc
 
 */



//uncomment this to test the code at higher speed
//#define wxDEBUG_LEVEL 0



#include "main.h"
#include "lib.cpp"



//this method  is executed reguarly over time, to check whether to switch between light and dark mode
void MyApp::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    
#ifdef __APPLE__
    
    
    if(dark_mode != (settings->GetAppearance().IsDark())) {
        //the dark mode of the operating system has changed
        
        if ((settings->GetAppearance().IsDark())) {
            //the system is in dark mode -> set image path equal to the /Dark folder
            
            image_directory = root_directory.append(String("Contents/Resources/Images/Dark/"));
            
        }
        else {
            //the system is in light mode ->  set image path equal to the /Light folder
            
            image_directory = root_directory.append(String("Contents/Resources/Images/Light/"));
            
        }
        
        //reset the images of all buttons

        
        //I re-draw all the ChartFrames so their fore/background colors will be adapted to the new mode of the operating system.
        list_frame->PreRenderAll();
    }
    
    dark_mode = (settings->GetAppearance()).IsDark();
    
#endif
#ifdef _WIN32
    
#endif
    
    
}

//if the user presses Ctrl + Q to exit the app, I call this function which prompts a message frame
template<class T> void MyApp::OnPressCtrlQ([[maybe_unused]] T& event) {
    
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
template<class T> void ListFrame::ComputePosition([[maybe_unused]] T& event) {
    
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
template<class T> void MyApp::ShowCharts([[maybe_unused]] T& event) {
    
    unsigned int i;
    //the spacing between one frame and another in the x and y direction, respectively
    double delta_x, delta_y;
    
    i = (list_frame->menu_bar->GetSize().GetHeight());
    
    if ((list_frame->chart_frames.size()) > 1) {
        //if ((list_frame->chart_frames).size() > 1 it makes sens to introduce delta_x, delta_y
        
        delta_x = (((double)(rectangle_display.GetWidth())) - ((double)(((((list_frame->chart_frames)[0])->GetSize()).GetWidth()) + ((((list_frame->chart_frames)[((list_frame->chart_frames).size()) - 1])->GetSize()).GetWidth()))) / 2.0 - 2.0 * ((wxGetApp().border).value)) / ((double)(((list_frame->chart_frames).size()) - 1));
        delta_y = (((double)(rectangle_display.GetHeight())) - ((double)(((((list_frame->chart_frames)[0])->GetSize()).GetHeight()) + ((((list_frame->chart_frames)[((list_frame->chart_frames).size()) - 1])->GetSize()).GetHeight()))) / 2.0 - 2.0 * ((wxGetApp().border).value)) / ((double)(((list_frame->chart_frames).size()) - 1));
        
    }
    else {
        //if ((list_frame->chart_frames).size() <= 1, it does not make sense to define delta_x, delta_y, and I set
        
        delta_x = 0.0;
        delta_y = 0.0;
        
    }
    
    
    for (i=0; i<(list_frame->chart_frames.size()); i++) {
        
        ((list_frame->chart_frames)[i])->Show(true);
        ((list_frame->chart_frames)[i])->Raise();
        ((list_frame->chart_frames)[i])->SetPosition(wxPoint(
                                                             
                                                             (((double)(((list_frame->chart_frames)[0])->GetSize().GetWidth())) - ((double)(((list_frame->chart_frames)[i])->GetSize().GetWidth()))) / 2.0 + ((wxGetApp().border).value) + delta_x * ((double)i)
                                                             ,
#ifdef __APPLE__
                                                             //I am on APPLE operating system -> there is a menu bar
                                                             /*here I shift everything down on the screen by the height of the menu_bar, because otherwise the ChartFrame on the top would be partially corvered by the menu bar and the one on the bottom would leave an empty space t the bottom of the screen */
                                                             
                                                             
                                                             (((list_frame->menu_bar)->GetSize()).GetHeight()) +
                                                             
#endif
                                                             
                                                             (((double)((((list_frame->chart_frames)[0])->GetSize()).GetHeight())) - ((double)(((list_frame->chart_frames)[i])->GetSize().GetHeight()))) / 2.0 + (wxGetApp().border.value) + delta_y * ((double)i)
                                                             
                                                             ));
        
    }
        
}


//same as template<class T> void MyApp::ShowCharts([[maybe_unused]] T& event) { but with no event argument
void MyApp::ShowCharts(void){
    
    wxCommandEvent dummy;
    
    ShowCharts(dummy);    
    
}


//make a nice animation to present all charts, by dragging them to the desired point from a point on the antipodes
inline void MyApp::AnimateCharts(void){

    for(unsigned int i=0; i<(list_frame->chart_frames.size()); i++){
        ((list_frame->chart_frames)[i])->Animate();
    }
    
}

//shows ListFrame and positions it properly on the screen
void MyApp::ShowList([[maybe_unused]] wxCommandEvent& event) {
    
    list_frame->SetPosition(wxPoint(0, /*move down ListFrame to take account of the menu_bar*/((list_frame->menu_bar)->GetSize()).GetHeight()));
    list_frame->Raise();
    
}

//writes into this->root_directory the path where the executable is currently running
void MyApp::where_am_I([[maybe_unused]] String prefix) {
    
    stringstream ins;
    
    ins.str("");
    //note that here boost::dll::program_location() may return a path with './' at the end, but this has no effect because ./ simply says to stay in the same path
    ins << (boost::dll::program_location().parent_path());
    root_directory.value = ins.str().c_str();
    
    root_directory.print(String("Non-formatted run directory"), true, String("*******"), cout);
    
    //remove " from root_directory
    
#ifdef __APPLE__
    //if I am on APPLE operating system, I go down by two directory levels to obtain the root_directory
    
    root_directory.value.erase(std::remove(root_directory.value.begin(), root_directory.value.end(), '"'), root_directory.value.end());
    root_directory.appendto(String("/../../"));
    
#endif
#ifdef _WIN32
    //I am on WIN32 operating system: I remove the '\"'that is present in root_directory at the beginning and at the end to obatin a root_directory that can be used as a path

   //regex vowel_re("//");
    //root_directory.value.erase(remove(root_directory.value.begin(), root_directory.value.end(), '\\'), root_directory.value.end());
    //boost::replace_all(root_directory.value, "\\", "\");
    //root_directory.value = my_replace(root_directory.value, '\\', '/');
    root_directory.replace_to('\\', '/');
    (root_directory.value) = (root_directory.value.substr(1, root_directory.value.size() - 2));
    root_directory.appendto(String("/"));
    
#endif
    
    root_directory.print(String("Formatted root directory"), true, String("*******"), cout);
    
    
}


//set the paths of all icons
void MyApp::set_icon_paths(void){
    
    //files in image directory
    path_file_app_icon = image_directory.append(read_from_file(String("name file app icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_error_icon = image_directory.append(read_from_file(String("name file error icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_warning_icon = image_directory.append(read_from_file(String("name file warning icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_info_icon = image_directory.append(read_from_file(String("name file info icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_question_icon = image_directory.append(read_from_file(String("name file question icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_plus_icon = image_directory.append(read_from_file(String("name file plus icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_list_icon = image_directory.append(read_from_file(String("name file list icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_reset_icon = image_directory.append(read_from_file(String("name file reset icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_position_icon = image_directory.append(read_from_file(String("name file position icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_map_icon = image_directory.append(read_from_file(String("name file map icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_pencil_icon = image_directory.append(read_from_file(String("name file pencil icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_trash_icon = image_directory.append(read_from_file(String("name file trash icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_arrow_icon = image_directory.append(read_from_file(String("name file arrow icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_disconnect_icon = image_directory.append(read_from_file(String("name file disconnect icon"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_michele_icon = image_directory.append(read_from_file(String("name file michele icon"), (wxGetApp().path_file_init), String("R"), String("")));
    
    
}

bool MyApp::OnInit() {
    

    /*
    vector<Angle> t(3);

    t[0].set(.5348609);
    t[1].set(.1);
    t[2].set(.1000000000000000001);
    delete_duplicates(&t, &equal_epsilon_double);
     
     */
    
    //
    Int I;
    I.value = 134435;
    
    I.my_round(display_precision);
    
    //
    
    unsigned int i;
    Int n_chart_frames;
    stringstream s;
    String temp;
    Projection projection;
    wxFrame* dummy_frame;
    ShowAll* show_all;
    //this contains the current time, the time of the transition from night to day (dawn), and the time of the transition from day to night (dusk)
    //    Chrono current_time, dawn, dusk;
    
    wxInitAllImageHandlers();
    
    
    degree_symbol = String("\u00b0");
    
    
    
    //detect the operating system
#ifdef __APPLE__
    
    cout << "You have Apple Operating System" << "\n";
    
    //to build the app on mac
    //            where_am_I(String(""));
    //to develop the app with Xcode on OSX
    root_directory = String("/Users/michele/Documents/sight_reduction_program/");

#endif
    
#ifdef _WIN32
    
    cout << "You have Windows Operating System" << "\n";
    //degree_symbol = String((char)176);
    
    
    //to run the app with Visual Studio on Windows
    //	root_directory = String("Z:/");
    where_am_I(String(""));
    
#endif
    
    settings = new wxSystemSettings();
    timer = new wxTimer();
    close_app = new CloseApp(this);
    
    dummy_frame = new wxFrame();
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    display.GetFromWindow(dummy_frame);
    
    rectangle_display = (display.GetClientArea());
    rectangle_display.SetWidth((int)((double)rectangle_display.GetWidth()));
    rectangle_display.SetHeight((int)((double)rectangle_display.GetHeight()));
    
    
    //directories are set dynamically from root_directory
    path_file_init = root_directory.append(String("Contents/Resources/Data/init.txt"));
    data_directory = root_directory.append(String("Contents/Resources/Data/"));
    if ((settings->GetAppearance().IsDark())) {
        //the system is in dark mode
        image_directory = root_directory.append(String("Contents/Resources/Images/Dark/"));
    }
    else {
        //the system is in light mode
        image_directory = root_directory.append(String("Contents/Resources/Images/Light/"));
    }
    default_open_directory = root_directory;
    
    //read the file names and prenend to the file name the respective directory where the file is located -> obtain the file path
    //files in code directory
    
    //files in data directory
    //	path_file_recent = data_directory.append(read_from_file(String("name file recent"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_catalog = data_directory.append(read_from_file(String("name file catalog"), (wxGetApp().path_file_init), String("R"), String("")));
    path_coastline_file = data_directory.append(read_from_file(String("name coastline file"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_n_line = data_directory.append(read_from_file(String("name file n line"), (wxGetApp().path_file_init), String("R"), String("")));
    path_file_sample_sight = data_directory.append(read_from_file(String("name file sample sight"), (wxGetApp().path_file_init), String("R"), String("")));
    
    
    set_icon_paths();
    
    
    data_precision.read_from_file_to(String("data precision"), (wxGetApp().path_file_init), String("R"), String(""));
    display_precision.read_from_file_to(String("display precision"), (wxGetApp().path_file_init), String("R"), String(""));
    time_check_light_dark.read_from_file_to(String("time check light dark"), (wxGetApp().path_file_init), String("R"), String(""));
    animation_time.read_from_file_to(String("animation time"), (wxGetApp().path_file_init), String("R"), String(""));
    time_zone.read_from_file_to(String("time zone"), (wxGetApp().path_file_init), String("R"), String(""));
    
    max_lat.read_from_file_to(String("maximal latitude coastline data"), (wxGetApp().path_file_init), String("R"), String(""));
    min_lat.read_from_file_to(String("minimal latitude coastline data"), (wxGetApp().path_file_init), String("R"), String(""));
    
    size_small_button_over_width_screen.read_from_file_to(String("size small button over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    size_large_button_over_width_screen.read_from_file_to(String("size large button over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    size_icon_over_width_screen.read_from_file_to(String("size icon over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    size_message_image_over_width_screen.read_from_file_to(String("size message image over width screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    point_size.read_from_file_to(String("point size"), (wxGetApp().path_file_init), String("R"), String(""));

    chart_transport_zoom_factor_coefficient.read_from_file_to(String("chart transport zoom factor coefficient"), (wxGetApp().path_file_init), String("R"),  String(""));
    minimal_animation_distance_over_size_of_observer_region.read_from_file_to(String("minimal animation distance over size of observer region"), (wxGetApp().path_file_init), String("R"),  String(""));

    
    //----- test for Data::read_from_file_to - start
    /*
     Data* my_data;
     Catalog* my_catalog;
     
     my_catalog = new Catalog((wxGetApp().path_file_catalog), String(""));
     my_data = new Data(my_catalog, String(""));
     
     my_data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("R"), String("**"));
     
     */
    //----- test for Data::read_from_file_to - end
    
    //set size_small/large_button from size_small_button_over_width_screen and size_large_button_over_width_screen
    size_small_button = dummy_frame->ToDIP(wxSize(
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_small_button_over_width_screen).value),
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_small_button_over_width_screen).value)
                               ));
    size_large_button = dummy_frame->ToDIP(wxSize(
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value),
                               ((wxGetApp().rectangle_display).GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)
                               ));
    
    
    length_plot_area_over_length_chart.read_from_file_to(String("length of plot area over length of chart"), (wxGetApp().path_file_init), String("R"),  String(""));
    length_chart_over_length_chart_frame.read_from_file_to(String("length of chart over length of chart frame"), (wxGetApp().path_file_init), String("R"),  String(""));
    length_border_over_length_screen.read_from_file_to(String("length of border over length of screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    
    border.set(String("border"), (rectangle_display.GetWidth()) * (length_border_over_length_screen.value), String(""));
    
    //read n_points_minor_ticks from file_init
    n_points_minor_ticks.read_from_file_to(String("number of points for minor ticks"), (wxGetApp().path_file_init), String("R"), String(""));
    //read relative_displacement from file_init
    relative_displacement.read_from_file_to(String("relative displacement"), (wxGetApp().path_file_init), String("R"), String(""));
    //read standard_thickness_over_length_screen from file_init
    standard_thickness_over_length_screen.read_from_file_to(String("standard thickness over length screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    //read large_thickness_over_length_screen from file_init
    large_thickness_over_length_screen.read_from_file_to(String("large thickness over length screen"), (wxGetApp().path_file_init), String("R"),  String(""));
    //read color horizon from file
    color_horizon.read_from_file(String("color horizon"), (wxGetApp().path_file_init), String("R"), String(""));
    //read color selected item from file
    color_selected_item.read_from_file(String("color selected item"), (wxGetApp().path_file_init), String("R"), String(""));
    //read tick length over width plot area from file_init
    tick_length_over_width_plot_area.read_from_file_to(String("tick length over width plot area"), (wxGetApp().path_file_init), String("R"), String(""));
    //read tick length over width plot area from file_init
    tick_length_over_aperture_circle_observer.read_from_file_to(String("tick length over aperture circle observer"), (wxGetApp().path_file_init), String("R"), String(""));
    //read min_crossing_angle from file_init
    min_crossing_angle.read_from_file_to(String("minimal crossing angle between circles of equal altitude"), (wxGetApp().path_file_init), String("R"), String(""));
    
    standard_thickness.set(String("standard thickness"), max((int)((((wxGetApp().standard_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1), String(""));
    large_thickness.set(String("large thickness"), max((int)((((wxGetApp().large_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1), String(""));
    
    
    //read the time, and set the background color to either the day or night background color, which are read from file
    //    time_zone.read_from_file(String("time zone"), (wxGetApp().path_file_init), String(""));
    //    dawn.read_from_file(String("dawn"), (wxGetApp().path_file_init), String(""));
    //    dusk.read_from_file(String("dusk"), (wxGetApp().path_file_init), String(""));
    //    current_time.set_current(time_zone, String(""));
    
    
    //    if((current_time < dawn) || (current_time > dusk)){
    //        //we are at night -> set background color to night mode
    //
    //        foreground_color.read_from_file(String("night foreground color"), (wxGetApp().path_file_init), String(""));
    //
    //    }else{
    //        //we are at day -> set background color ot day mode
    //
    //        foreground_color.read_from_file(String("day foreground color"), (wxGetApp().path_file_init), String(""));
    //
    //    }
    
    timer->Bind(wxEVT_TIMER, &MyApp::OnTimer, this);
    
    
    
    foreground_color = Color(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    background_color = Color(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));
    error_color.read_from_file(String("error color"), (wxGetApp().path_file_init), String("R"), String(""));
    highlight_color = color_selected_item;
    dark_mode = (settings->GetAppearance()).IsDark();
    
//    n_recent_bodies.read_from_file_to(String("number of recent bodies"), (wxGetApp().path_file_init), String("R"), String(""));
//    n_recent_projections.read_from_file_to(String("number of recent projections"), (wxGetApp().path_file_init), String("R"), String(""));
//    n_recent_length_formats.read_from_file_to(String("number of recent length formats"), (wxGetApp().path_file_init), String("R"), String(""));
//    n_recent_route_types.read_from_file_to(String("number of recent route types"), (wxGetApp().path_file_init), String("R"), String(""));
//    n_recent_route_for_transport_types.read_from_file_to(String("number of recent route for transport types"), (wxGetApp().path_file_init), String("R"), String(""));
//    n_recent_length_units.read_from_file_to(String("number of recent length units"), (wxGetApp().path_file_init), String("R"), String(""));
//    n_recent_speed_units.read_from_file_to(String("number of recent speed units"), (wxGetApp().path_file_init), String("R"), String(""));

    n_animation_steps.read_from_file_to(String("number of animation steps"), (wxGetApp().path_file_init), String("R"), String(""));
        
    list_frame = new ListFrame("Unnamed", "", wxDefaultPosition, wxDefaultSize, String(""));

    if(!(list_frame->abort)){
        //the user has not pressed cancel while charts were loading -> I proceed and start the app
        
        show_all = new ShowAll(list_frame);
        disclaimer = new QuestionFrame< ShowAll , CloseApp >(NULL, show_all, String("Yes"), close_app, String("No"), true, true, 
                                                                          "Welcome to Thelemacus!",
                                                                          //                                                                          "On December 16, 1719, Captain J. Cook perceived the first Australian aborigens from HMS Endeavour, off the coast of Perth.\n He was on a mission commissioned by King John III, designed to discover new commercial routes, and new worlds.\n His voyage had been made possible by the novel, state-of-the art astronomical positioning methods\n based on the marine chronometer built by J. Harrison, which was on board the Endeavour. \nThe reliability of the positioning method allowed the british realm to trace and map the coasts of new, unknonw lands, \nand paved the way to a new way to sail which lasted until the invention of GPS.\n With this application, you will bring back to life astronomical positioning methods, in a way that no other existing application allows for, and entering in a novel historical path. "
                                                                          "This is the state-of-the art application for celestial navigation, I hope you will enjoy it!\nRemember that this software comes with no warranty, use at your own risk!\nDo you want to proceed?\n\nFair winds, following seas ..."
                                                                          ,
                                                                          
                                                                          wxGetApp().path_file_warning_icon,
                                                                          wxDefaultPosition,
                                                                          wxDefaultSize,
                                                                          String(""));
        disclaimer->Show(true);
        disclaimer->Raise();
        //    list_frame->Show(true);
        
        
        //allocate and show the chart frames
        n_chart_frames.read_from_file_to(String("number chart frames"), (wxGetApp().path_file_init), String("R"), String(""));
        (list_frame->chart_frames).resize(n_chart_frames.value);
        for (i = 0; i < (list_frame->chart_frames).size(); i++) {
             
            projection = (((i % 2) == 0) ? (Projection_types[0]) : (Projection_types[1]));

            //open a Mercator projection for even is and a 3D projection for odd is
            s.str("");
            s << "Chart #" << i + 1 << " - " << projection.value << " projection";
            
            string dummy = s.str();
            
            (list_frame->chart_frames)[i] = new ChartFrame(
                                                           list_frame,
                                                           projection,
//                                                           String(""),
                                                           s.str(),
                                                           wxDefaultPosition,
                                                           wxDefaultSize,
                                                           String("")
                                                           );

            
        }
        
        
        //fore/background color is determined from the default background color of extract_colors
        foreground_color = Color(list_frame->extract_colors->GetForegroundColour());
        background_color = Color(list_frame->extract_colors->GetBackgroundColour());
        
        
        //extracts the default font and creates a error_font, obtained from default font by setting its weight to wxFONTWEIGHT_BOLD
        default_font = (list_frame->extract_colors->GetFont());
        error_font = (list_frame->extract_colors->GetFont());
        highlight_font = default_font;
        error_font.SetWeight(wxFONTWEIGHT_BOLD);
        
        timer->Start(/*time_check_light_dark is converted in milliseconds, because Start() takes its first argument in milliseconds*/((time_check_light_dark.h) * 60.0 * 60.0 + (time_check_light_dark.m) * 60.0 + (time_check_light_dark.s)) * 1000.0, wxTIMER_CONTINUOUS);
        
        
        return true;
        
    }else{
        //the user pressed cancel -> I quit the app
        
        wxCommandEvent dummy;
        
        (*(list_frame->close))(dummy);
        
        return false;
        
    }
        
}
