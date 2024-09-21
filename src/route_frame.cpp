//
//  route_frame.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "animate_to_object.h"
#include "check_chrono.h"
#include "check_length.h"
#include "check_route_type.h"
#include "check_speed.h"
#include "check_string.h"
#include "on_change_selection_in_list_control.h"
#include "on_new_route_in_list_control_for_transport.h"
#include "route_frame.h"
#include "speed_unit_field.h"
#include "string_field.h"


//create a new RouteFrame. If for_transport = true/false, it enables the fields related to the start position of the Route and disables the circle of equal altitude type
RouteFrame::RouteFrame(ListFrame* parent_input, Route* route_in, bool for_transport_in, long position_in_listcontrol_routes_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    String new_prefix, label_button_ok;
    unsigned int common_width;
    bool check;


    parent = parent_input;
    check = true;
    for_transport = for_transport_in;
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //SetColor(this);

    set_idling = new SetIdling<RouteFrame>(this);
    unset_idling = new UnsetIdling<RouteFrame>(this);
    (*unset_idling)();
    print_error_message = new PrintMessage<RouteFrame, UnsetIdling<RouteFrame> >(this, unset_idling);

    //if this RouteFrame has been constructed with route_in = NULL, then I allocate a new Route object with the pointer this->route and set list_route to a 'NULL' value (list_route = -1). Otherwise, the pointer route_in points to a valid Route object -> I let this->route point to route_in, and set list_route to list_route_in.
    //if I am adding a brand new route, I name button_ok 'Add' if I am not adding it for transport, while I name button_ok 'Transport' if I am adding the Route for transport. Otherwise I name it "Modify"
    if (route_in != NULL) {
        route = route_in;
        position_in_listcontrol_routes = position_in_listcontrol_routes_in;
        label_button_ok.set(String("Modify"));
    }
    else {
        route = new Route();
        position_in_listcontrol_routes = -1;
//        label_button_ok.set(String("Add"));
        label_button_ok.set(String((for_transport ? "Transport" : "Add")));

    }


    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    //allocate buttons
    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value, wxDefaultPosition, wxDefaultSize);
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);


    sizer_grid_type = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_Z = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_l_format_l_t_v = new wxBoxSizer(wxVERTICAL);
    sizer_grid_l = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_t_v = new wxFlexGridSizer(2, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_start = new wxFlexGridSizer(2, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_GP = new wxFlexGridSizer(2, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_omega = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_label = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_box_data = new wxStaticBoxSizer(wxVERTICAL, panel, "Data");
    sizer_box_l_format_l_t_v = new wxStaticBoxSizer(wxVERTICAL, panel, "Length");
    sizer_box_l = new wxStaticBoxSizer(wxVERTICAL, panel, "Length");
    sizer_box_t_v = new wxStaticBoxSizer(wxVERTICAL, panel, "Time and speed");
    sizer_box_start = new wxStaticBoxSizer(wxVERTICAL, panel, "Start position");
    sizer_box_GP = new wxStaticBoxSizer(wxVERTICAL, panel, "Ground position");
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer = new wxBoxSizer(wxHORIZONTAL);


    //type:a wxComboBox which indicates the type of the route (loxodrome, orthordrome or circle of equal altitude)
    StaticText* text_type = new StaticText(panel, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0);
    //if the Route of *this is for transport, then only show 'loxodrome' and 'orthodrome' in type
    if (for_transport) {
        type = new RouteTypeField<RouteFrame>(panel, &(route->type), Route_types_for_transport, &(wxGetApp().list_frame->data->recent_Rout_types_for_transport));
    }else{
        type = new RouteTypeField<RouteFrame>(panel, &(route->type), Route_types,  &(wxGetApp().list_frame->data->recent_route_types));
    }

    //Z
    StaticText* text_Z = new StaticText(panel, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0);
    Z = new AngleField<RouteFrame>(panel, &(route->Z), NULL, String(""));

    //format in which lengths are expressed
    StaticText* text_l_format = new StaticText(panel, wxT("Length format"), wxDefaultPosition, wxDefaultSize, 0);
    length_format = new LengthFormatField<RouteFrame>(panel, &(route->length_format), &(wxGetApp().list_frame->data->recent_length_formats));

    //the field for time to set the Route length
    text_time = new StaticText(panel, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0);
    time = new ChronoField<RouteFrame>(panel, &(route->time));
    //the field for speed to set the Route length
    text_speed = new StaticText(panel, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0);
    speed = new SpeedField<RouteFrame>(
                                       panel,
                                       route->speed,
                                       &(wxGetApp().list_frame->data->recent_route_speed_value),
                                       &(wxGetApp().list_frame->data->recent_speed_units)
                                       );
    
    //the field for Length to set the Route length
    text_length = new StaticText(panel, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0);
    length = new DynamicLengthField<RouteFrame>(
                                                panel,
                                                route->length,
                                                &(wxGetApp().list_frame->data->recent_route_length_value)
                                                );


    //this is how to properly bind the DynamicLengthField length when it is inserted into a frame and I want a modification of the DynamicLengthField to trigger AllOk() in the frame. Given that I am including length in a frame, I want that every time value or unit is changed, SightFrame::AllOk() is triggered : 1. I first bind OnEditValue and OnEditUnit to length->value and length->unit 2. every time length is changed, OnEditValue and OnEditUnit will be called and set to true/false the value_ok and unit_ok variables 3. AllOk() will be called later, read the value_ok and unit_ok variables, and enable/disable button_reduce  accordingly
    length->Bind(wxEVT_COMBOBOX, &RouteFrame::AllOk<wxCommandEvent>, this);
    length->Bind(wxEVT_KEY_UP, &RouteFrame::AllOk<wxKeyEvent>, this);
    length->value->Bind(wxEVT_COMBOBOX, &DynamicLengthField<RouteFrame>::OnEditValue<wxCommandEvent>, length);
    length->value->Bind(wxEVT_KEY_UP, &DynamicLengthField<RouteFrame>::OnEditValue<wxKeyEvent>, length);
    length->unit->Bind(wxEVT_COMBOBOX, &DynamicLengthField<RouteFrame>::OnEditUnit<wxCommandEvent>, length);
    length->unit->Bind(wxEVT_KEY_UP, &DynamicLengthField<RouteFrame>::OnEditUnit<wxKeyEvent>, length);
    
    
    type->Bind(wxEVT_COMBOBOX, &LengthFormatField<RouteFrame>::OnEdit<wxCommandEvent>, length_format);
    type->Bind(wxEVT_KEY_UP, &LengthFormatField<RouteFrame>::OnEdit<wxKeyEvent>, length_format);

    //bind time and speed changes to UpdateLength, so every time time and speed GUI fields are edited, length is updated accordingly
    //I want ChronoField::OnEditHour, OnEditMinute and OnEditSecond to be called before RouteFrame::UpdateLength, because RouteFrame::UpdateLength checks the variables hour_ok, minute_ok and second_ok, which must have been set previously by ChronoField::OnEditHour, OnEditMinute and OnEditSecond, respectively -> To achieve this 1) I unbind ChronoField::OnEditHour, OnEditMinute and OnEditSecond from time->hour, minute and second, respectively, (they had been bound previously in the ChronoField constructor) 2) I bind RouteFrame::UpdateLength to time->hour, minute, second 3) I bind back ChronoField::OnEditHour, OnEditMinute, OnEditSecond to time->hour, minut and second, respectively. In this way, the Binding order has changed -> the order in which the event handlers will be called will be the right one.
    //1)
    time->hour->Unbind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditHour<wxCommandEvent>, time);
    time->hour->Unbind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditHour<wxKeyEvent>, time);
    time->minute->Unbind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditMinute<wxCommandEvent>, time);
    time->minute->Unbind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditMinute<wxKeyEvent>, time);
    time->second->Unbind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditSecond<wxKeyEvent>, time);
    //2)
    time->Bind(wxEVT_KEY_UP, &RouteFrame::UpdateLength<wxKeyEvent>, this);
    time->Bind(wxEVT_COMBOBOX, &RouteFrame::UpdateLength<wxCommandEvent>, this);
    //3)
    time->hour->Bind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditHour<wxCommandEvent>, time);
    time->hour->Bind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditHour<wxKeyEvent>, time);
    time->minute->Bind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditMinute<wxCommandEvent>, time);
    time->minute->Bind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditMinute<wxKeyEvent>, time);
    time->second->Bind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditSecond<wxKeyEvent>, time);

    //I want SpeedField::OnEditValue and OnEditUnit to be called before RouteFrame::UpdateLength, because RouteFrame::UpdateLength checks the variables value_ok and unit_ok, which must have been set previously by SpeedField::OnEditValue and OnEditUnit, respectively -> To achieve this 1) I unbind SpeedField::OnEditValue and OnEditUnit from speed->value and unit, respectively, (they had been bound previously in the SpeedField constructor) 2) I bind RouteFrame::UpdateLength to speed->value and unit 3) I bind back SpeedField::OnEditValue and OnEditUnit to speed->value and unit, respectively. In this way, the Binding order has changed -> the order in which the event handlers will be called will be the right one.
    speed->value->Unbind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditValue<wxKeyEvent>, speed);
    speed->unit->name->Unbind(wxEVT_COMBOBOX, &SpeedField<RouteFrame>::OnEditUnit<wxCommandEvent>, speed);
    speed->unit->name->Unbind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditUnit<wxKeyEvent>, speed);
    //2)
    speed->Bind(wxEVT_KEY_UP, &RouteFrame::UpdateLength<wxKeyEvent>, this);
    speed->unit->Bind(wxEVT_COMBOBOX, &RouteFrame::UpdateLength<wxCommandEvent>, this);
    //3)
    speed->value->Bind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditValue<wxKeyEvent>, speed);
    speed->unit->Bind(wxEVT_COMBOBOX, &SpeedField<RouteFrame>::OnEditUnit<wxCommandEvent>, speed);
    speed->unit->Bind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditUnit<wxKeyEvent>, speed);


    //start position
    //start_phi
    StaticText* text_start_phi = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    start_phi = new AngleField<RouteFrame>(panel, &(route->reference_position->phi), NULL, String("NS"));
    //start_lambda
    StaticText* text_start_lambda = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    start_lambda = new AngleField<RouteFrame>(panel, &(route->reference_position->lambda), NULL, String("EW"));

    //GP (ground position)
    //GP_phi
    StaticText* text_GP_phi = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    GP_phi = new AngleField<RouteFrame>(panel, &(route->reference_position->phi), NULL, String("NS"));
    //GP_lambda
    StaticText* text_GP_lambda = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    GP_lambda = new AngleField<RouteFrame>(panel, &(route->reference_position->lambda), NULL, String("EW"));

    //omega
    StaticText* text_omega = new StaticText(panel, wxT("Omega"), wxDefaultPosition, wxDefaultSize, 0);
    omega = new AngleField<RouteFrame>(panel, &(route->omega), NULL, String(""));

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<RouteFrame>(panel, &(route->label));

    //I enable the ok button only if route_in is a valid route with the entries propely filled, i.e., only if route_in != NULL
    button_ok->Bind(wxEVT_BUTTON, &RouteFrame::OnPressOk, this);
    button_ok->Enable((route_in != NULL));
    button_cancel->Bind(wxEVT_BUTTON, &RouteFrame::OnPressCancel, this);


    //bind the function SightFrame::KeyDown to the event where a keyboard dey is pressed down in panel, ... and all fields
    panel->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    type->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    Z->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    omega->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    start_phi->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    start_lambda->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    GP_phi->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    GP_lambda->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    length_format->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    length->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    time->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    speed->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);


    sizer_grid_type->Add(text_type, 0, wxALIGN_CENTER_VERTICAL);
    type->InsertIn<wxFlexGridSizer>(sizer_grid_type);

    sizer_grid_Z->Add(text_Z, 0, wxALIGN_CENTER_VERTICAL);
    Z->InsertIn<wxFlexGridSizer>(sizer_grid_Z);

    sizer_grid_t_v->Add(text_time, 0, wxALIGN_CENTER_VERTICAL);
    time->InsertIn<wxFlexGridSizer>(sizer_grid_t_v);
    sizer_grid_t_v->Add(text_speed, 0, wxALIGN_CENTER_VERTICAL);
    speed->InsertIn<wxFlexGridSizer>(sizer_grid_t_v);
    sizer_box_t_v->Add(sizer_grid_t_v);

    sizer_grid_l->Add(text_length, 0, wxALIGN_CENTER_VERTICAL);
    length->InsertIn<wxFlexGridSizer>(sizer_grid_l);
    sizer_box_l->Add(sizer_grid_l);

    sizer_l_format_l_t_v->Add(text_l_format, 0, wxALIGN_LEFT);
    length_format->InsertIn<wxBoxSizer>(sizer_l_format_l_t_v);
    sizer_l_format_l_t_v->Add(sizer_box_t_v);
    sizer_l_format_l_t_v->Add(sizer_box_l);

    sizer_box_l_format_l_t_v->Add(sizer_l_format_l_t_v);

    sizer_grid_omega->Add(text_omega, 0, wxALIGN_CENTER_VERTICAL);
    omega->InsertIn<wxFlexGridSizer>(sizer_grid_omega);

    sizer_grid_start->Add(text_start_phi);
    start_phi->InsertIn<wxFlexGridSizer>(sizer_grid_start);
    sizer_grid_start->Add(text_start_lambda);
    start_lambda->InsertIn<wxFlexGridSizer>(sizer_grid_start);

    sizer_box_start->Add(sizer_grid_start);

    sizer_grid_GP->Add(text_GP_phi);
    GP_phi->InsertIn<wxFlexGridSizer>(sizer_grid_GP);
    sizer_grid_GP->Add(text_GP_lambda);
    GP_lambda->InsertIn<wxFlexGridSizer>(sizer_grid_GP);

    sizer_box_GP->Add(sizer_grid_GP);

    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    sizer_box_data->Add(sizer_grid_type);
    sizer_box_data->Add(sizer_grid_Z);
    sizer_box_data->Add(sizer_box_l_format_l_t_v);
    sizer_box_data->Add(sizer_box_start);
    sizer_box_data->Add(sizer_box_GP);
    sizer_box_data->Add(sizer_grid_omega);

    box_sizer->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), NULL);
    box_sizer->Add(button_ok, 0, wxALIGN_BOTTOM | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), NULL);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_Z->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_omega->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    //add the various elements to sizer, by inserting a border of (wxGetApp().rectangle_display.GetSize().GetWidth())*(length_border_over_length_screen.value) in all directions
    sizer->Add(sizer_box_data, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    panel->Fit();
    Fit();


    if (!check) {

        cout << prefix.value << RED << "Cannot read route!\n" << RESET;

    }

    if (route_in == NULL) {
        //If the user is about to enter a brand new route, then the GUI fields are disabled/enabled according to the currently selected value in *type

        //set as Route type the most recent item in recetn _route_types (or recent route_for_transport_types if the new Route has been created for a transport): set first type->object and then write in type the value written in type->object

        if(for_transport){
            type->object->set(Route_types[wxGetApp().list_frame->data->recent_Rout_types_for_transport.front()]);
        }else{
            type->object->set(Route_types[wxGetApp().list_frame->data->recent_route_types.front()]);
        }
        type->MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame> >::set();
        
        length_format->object->set(LengthFormat_types[wxGetApp().list_frame->data->recent_length_formats.front()]);
        length_format->MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame> >::set();

        //call this to enable/disable the GUI fields in according to the choice of RouteType
        EnableDisableRouteTypeFields();
        //call this to enable/disable the GUI fields according ot the choice of LengthFormat
        EnableDisableLengthFormatFields();

    }
    else {
        //I am reading an existing Route -> I call set -> the LengthFormatField will be set -> I call OnChooseLengthFormat to enable/disable the related GUI fields accordingly

        set();

        start_phi->Enable(for_transport);
        start_lambda->Enable(for_transport);

    }

    //runs checl to write into the `ok` boolean variable of each field, and then AllOk to enable/disable button_reduce according to these `ok` variables
    Check();
    AllOk();
    
    Centre();

}



void RouteFrame::OnPressOk(wxCommandEvent& event) {

    unsigned int i;
    stringstream s;
    //this is true if a message to that the modified Route is being disconnected from its related sight has to be prompted, false otherwise
    bool prompt_disconnection_message;
    
    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }


    //write the values of the GUI fields in the non-GUI fields
    get(event);

    if (position_in_listcontrol_routes == -1) {
        //I am creating a new Route

        //if the constructor of RouteFrame has been called with route_in = NULL, then I push back the newly allocated route to the end of route_list and reduce it
        parent->data->add_route(route, String(""));
        
        prompt_disconnection_message = false;

    }else {
        //I am modifying an existing Route

        if ((route->related_sight) != -1) {
            //the Route that I am moidifying is related to a Sight

            //because I am modifying and thus altering the Route, I disconnect it from its related sight
            (parent->i_object_to_disconnect) = (route->related_sight.value);
            parent->Disconnect(event);
            //set i_obeject_to_disconnect to its original value
            (parent->i_object_to_disconnect) = -1;
            
            prompt_disconnection_message = true;

        }else{
            
            prompt_disconnection_message = false;
            
        }

    }


    //if I am adding a new Route, I resize points_route_list to add a new element to it
    if (position_in_listcontrol_routes == -1) {

        for (i = 0; i < (parent->chart_frames.size()); i++) {

            (((parent->chart_frames)[i])->draw_panel)->routes.resize(((((parent->chart_frames)[i])->draw_panel)->routes).size() + 1);
            (((parent->chart_frames)[i])->draw_panel)->reference_positions_route_list.resize(((parent->chart_frames)[i])->draw_panel->reference_positions_route_list.size() + 1);

        }
    }

    //call listcontrol_sights->set with true because I want to keep the selection in listcontrol_sights
    parent->listcontrol_sights->set((parent->data->sight_list), true);
    parent->listcontrol_positions->set((parent->data->position_list), true);
    //THIS CORRUPTS THE UNITS OF MEASURE OF route_list[i].length
    parent->listcontrol_routes->set((parent->data->route_list), false);
    //THIS CORRUPTS THE UNITS OF MEASURE OF route_list[i].length

    //given that I have reset the content of listcontrol_sights and listcontrol_routes, now no items will be selected in these ListControls -> I call:
    (*(parent->on_change_selection_in_listcontrol_sights))(event);
    (*(parent->on_change_selection_in_listcontrol_routes))(event);

    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    
    if ((parent->transporting_with_new_route)) {
        //if I am adding a new Route for transport, call on_new_route_in_listcontrol_routes_for_transport to execute the transport with this Route
        (*(parent->on_new_route_in_listcontrol_routes_for_transport))(event);

        //set the reference position of the transporting Route to the initial position of the object that has been transported: in thiw way, the transporting Route will look nice on the chart
        if ((parent->transported_object_type) == String("position")) {

            //store the starting position in *geo_position_start
            (*(((parent->data->route_list)[(parent->i_transporting_route)]).reference_position)) = (parent->data->position_list)[(parent->i_object_to_transport)];

        }
        else {

            if (((parent->transported_object_type) == String("sight")) || (parent->transported_object_type) == String("route")) {

                //store the starting reference position in *geo_position_start
                (*((parent->data->route_list)[(parent->i_transporting_route)]).reference_position) = (*(((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position));

            }

        }

        //I refresh everything because of the modification above
        //call listcontrol_routes->set with true because I want to keep the selection in listcontrol_routes
        parent->listcontrol_routes->set(parent->data->route_list, false);
        parent->Resize();
        parent->OnModifyFile();

    }

    
    if(prompt_disconnection_message){
        //I am modifying an existing Route and the Route that I am modifying is related to a Sight -> prepare the warning message to be prompted at the end of the animation and call AnimateToObjectOld with parent->print_info_message as an argument, in such a way that, at the end of the animation, this message is prompted

        AnimateToObject<Route, HighlightObject<ListFrame>> animate(parent, route, parent->highlight_route);
        
        parent->print_info_message->control = NULL;
        parent->print_info_message->title.set(String("Warning"));
        parent->print_info_message->message.set(String("The route which has been modified was related to a sight! Disconnecting the route from the sight."));
        
        //de-highlight all Positions
        parent->highlight_position->set_value(-1);
        parent->highlight_position->operator()(event);
        
        //1. set the highlighted_route equal to the id of the newly added/modified Route, so the user can see it easily
        parent->highlight_route->set_value(
                                           ((position_in_listcontrol_routes == -1) ? ((int)(parent->data->route_list.size()))-1 : ((int)position_in_listcontrol_routes))
                                           
                                           );
        parent->highlight_route->operator()(event);
        
        //2. in parent->highlight_route, set the value of the highlighted Route to be set equal to -1, and call AnimateToObjectOld with second argument parent->highlight_route : in this way, when the animation is over, the highlighted Route will be set to -1, i.e., no Route will be highlighted when the animation is over
        parent->highlight_route->set_value(-1);

        animate.operator()();
        
    }else{
        //I don't need to prompt a message warning the user that the Route under consideration is being disconnected from its related Sight -> trigger the animation that centers the chart on *route by callling UnsetIdling (intended as 'do nothing' here) at the end of the animation
        
        //If I am adding a new Route for transport, I do not call any animation, because there is already the transport animation that will be prompted. Otherwise, I call an animation that zooms on the newly added Route
        if (!(parent->transporting_with_new_route)) {
            //I am not adding a new Route for transrpot -> animate the charts to bring them to the Route related to the new Route. Set the highlighted_route equal to the newly added /modified Route, so the user can see it easily during the animation:
            
           AnimateToObject<Route, HighlightObject<ListFrame>> animate(parent, route, parent->highlight_route);

            //de-highlight all Positions
            parent->highlight_position->set_value(-1);
            parent->highlight_position->operator()(event);
            
            //1. set the highlighted_route equal to the id of the newly added/modified Route, so the user can see it easily
            parent->highlight_route->set_value(
                                               ((position_in_listcontrol_routes == -1) ? ((int)(parent->data->route_list.size()))-1 : ((int)position_in_listcontrol_routes))
                                               
                                               );
            parent->highlight_route->operator()(event);
            //2. in parent->highlight_route, set the value of the highlighted Route to be set equal to -1, and call AnimateToObjectOld with second argument parent->highlight_route : in this way, when the animation is over, the highlighted Route will be set to -1, i.e., no Route will be highlighted when the animation is over
            parent->highlight_route->set_value(-1);
            animate.operator()();
            
        }
        
    }
    
 
    event.Skip(true);

    Close(TRUE);

}

void RouteFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to false
    (*(parent->unset_idling))();

    Close(TRUE);

}


//write the content in the GUI fields into the non-GUI fields, and returns true if all is ok, false otherwise
bool RouteFrame::is_ok(void) {
    
    wxCommandEvent dummy;
    
    get(dummy);
    
    return((type->is_ok()) &&
           
           (
            (
             ((((type->name)->GetValue()) == wxString(((Route_types[0]).value))) || (((type->name)->GetValue()) == wxString(((Route_types[1]).value)))) &&
             (
              (Z->is_ok()) &&
              ((start_phi->is_ok()) || for_transport) &&
              ((start_lambda->is_ok()) || for_transport) &&
              (((((length_format->name)->GetValue()) == wxString(((LengthFormat_types[0]).value))) && ((time->is_ok()) && (speed->is_ok()))) || ((((length_format->name)->GetValue()) == wxString(((LengthFormat_types[1]).value))) && (length->is_ok())))
              )
             )
            
            ||
            
            (
             (((type->name)->GetValue()) == wxString(((Route_types[2]).value))) &&
             ((omega->is_ok()) &&
              (GP_phi->is_ok()) &&
              (GP_lambda->is_ok()))
             )
            )
           
           );
    
}

//tries to enable button_ok
void RouteFrame::AllOk(void) {

    button_ok->Enable(is_ok());

}


// same as RouteFrame::AllOk(void)  but with an event as an argument, so this method can be triggered from an event
template<class E> void RouteFrame::AllOk([[maybe_unused]] E& event) {

    AllOk();

}


//if a key is pressed in the keyboard, I call this function
void RouteFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        //the user pressed escape -> I do as if the user pressed button_cancel

        wxCommandEvent dummy;

        OnPressCancel(dummy);

    }
    else {

        if (((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
            //the user pressed return or numpad return

            if (is_ok()) {
                //if all fields are ok, I do as if the user presssed button_ok

                wxCommandEvent dummy;

                OnPressOk(dummy);

            }

        }

    }

    event.Skip(true);

}

//run check on all the GUI fields that are members of RouteFrame and write true/false in their ok variables
template<class E> void RouteFrame::Check(E& event) {

    (*(type->check))(event);
    
    
    if(((type->name->GetValue()) == wxString((Route_types[0]).value)) || ((type->name->GetValue()) == wxString((Route_types[1]).value))){
        //*route is either a loxodrome or an orthodrome -> I check the fields related to loxodromes and orthodromes
        
        (*(Z->check))(event);
        if(!for_transport){
            //if *route is not a Route for transport, then its starting position matters -> I check it
            (*(start_phi->check))(event);
            (*(start_lambda->check))(event);
        }
    
        
        switch ((String(length_format->name->GetValue().ToStdString()).position_in_list(length_format->catalog))) {
                
            case 0:
                //length format is time x speed
                
                (*(time->check))(event);
                (*(speed->check))(event);
                
                break;
                
            case 1:
                //length format is simply length
                
                (*(length->check))(event);
                
                break;
            
        }
        
        
    }else{
        //*route is a circle of equal altitude ->  I check the fields related to circles of equal altitude
        
        (*(omega->check))(event);
        (*(GP_phi->check))(event);
        (*(GP_lambda->check))(event);

    }
    
    (*(label->check))(event);

    event.Skip(true);

}


void RouteFrame::Check(void) {
    
    wxCommandEvent dummy;
    
    Check(dummy);
    
}


//set the values in all the GUI fields equal to the values in the respective non-GUI fields
void RouteFrame::set(void) {

    type->set();
    length_format->set();
    
    EnableDisableRouteTypeFields();
    
    if ((route->type.value) == wxString(((Route_types[2]).value))) {
        //I disable the GUI fields which do not define a circle of equal altitude and set the others

//        Z->Enable(false);

//        start_phi->Enable(false);
//        start_lambda->Enable(false);

        GP_phi->set();
        GP_lambda->set();
        omega->set();

    }
    else {
        //I disable the GUI fields which do not define a loxodrome or orthodrome and set the others

        wxCommandEvent dummy;

        Z->set();

        start_phi->set();
        start_lambda->set();
//        start_phi->Enable(!for_transport);
//        start_lambda->Enable(!for_transport);

//        GP_phi->Enable(false);
//        GP_lambda->Enable(false);
//        omega->Enable(false);

    }

    //enable the length or the time and speed fields
    EnableDisableLengthFormatFields();

    if ((route->length_format) == (LengthFormat_types[1])) {
        //the Route length is simply expressed as a length rather than as a time and speed -> set length field

        length->set();

    }
    else {
        //the Route length is expressed as a time and a speed -> set time and speed field, and set also the length field as the product of the time and speed

        time->set();
        speed->set();
        
        route->set_length_from_time_speed();
        //        (route->length) = Length(route->time, route->speed);
        length->set();

    }

    Check();
    label->set();


}


//set the values in all the non-GUI fields equal to the values in the respective GUI fields
template<class T> void RouteFrame::get(T& event) {

    type->MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame> >::Get(event);

    if (((type->name)->GetValue()) == wxString(((Route_types[2]).value))) {

        GP_phi->get(event);
        GP_lambda->get(event);
        omega->get(event);

    }else {

        Z->get(event);
        start_phi->get(event);
        start_lambda->get(event);

        if ((length_format->name->GetValue()) == length_format->catalog[0]) {
            //in the GUI field, lengths are expressed at Chrono x Speed -> get t and v and set in the non-GUI field to true. I also set route->l according to time and speed

            (route->length_format) = LengthFormat(((LengthFormat_types[0]).value));
            time->get(event);
            speed->get(event);
            (*(route->length)) = Length(route->time, (*(route->speed)));

        }else {
            //in the GUI field, lenght are expressed simply as a Length -> get l and set in the non-GUI field to false

            (route->length_format) = LengthFormat(((LengthFormat_types[1]).value));
            length->get(event);

        }

    }

    label->get(event);


}


//enable/disable the fields in *this according to the Route type
template<class E> void RouteFrame::EnableDisableRouteTypeFields([[maybe_unused]] E& event) {
    
    
    unsigned int i;
    bool check, enable;
  
    //I check whether the RouteTypeField *p of the GUI field matches one of the route types in catalog
    type->CheckInCatalog(&check, &i);

    if (check) {
        //the Route type is valid
        
        //enable/disable the related fields in RouteFrame f
        enable = ((((type->catalog)[i]) == wxString(((Route_types[0]).value))) || (((type->catalog)[i]) == wxString(((Route_types[1]).value))));

        Z->Enable(enable);

        //for start_phi/lambda to be enabled, not only the Route has to be of tyoe Route_types[0] or Route_types[1], but, in addition, it must not be a Route for transport (if it were, there would be no need to indicate its starting Position to do the transport )
        start_phi->Enable(enable && (!(for_transport)));
        start_lambda->Enable(enable && (!(for_transport)));

        GP_phi->Enable(!enable);
        GP_lambda->Enable(!enable);
        omega->Enable(!enable);

    }else{
        //the Route type is not valid

        Z->Enable(false);
        start_phi->Enable(false);
        start_lambda->Enable(false);

        GP_phi->Enable(false);
        GP_lambda->Enable(false);
        omega->Enable(false);

    }
    
}

template void RouteFrame::EnableDisableRouteTypeFields<wxFocusEvent>(wxFocusEvent&);
template void RouteFrame::EnableDisableRouteTypeFields<wxCommandEvent>(wxCommandEvent&);
template void RouteFrame::EnableDisableRouteTypeFields<wxKeyEvent>(wxKeyEvent&);


//same as RouteFrame::EnableDisableRouteTypeFields(E& event) but with no event argument
void RouteFrame::EnableDisableRouteTypeFields(void){
    
    wxCommandEvent dummy;
    
    EnableDisableRouteTypeFields(dummy);
    
}

//enable/disable the GUI fields in *this related to the Length format accoridng to the choice in type->name (the selected type of Route)
template<class E> void RouteFrame::EnableDisableLengthFormatFields(E& event) {
    
    if ((type->is_ok())) {
        //type is valid
        
        if ((type->name->GetValue()) != wxString(((Route_types[2]).value))) {
            //the Route is either a loxodrome or an orthodrome -> the Route allows for a Length -> enable length_format
            
            bool b = false;
            
            //given that loxodromes and orthodromes allow for a Length, I enable length_format
            length_format->Enable(true);
            
            if(length_format->is_ok()){
                //length_format has a valid content -> enable / disable the relative fields
                
                //run over all entries of length_format->catalog and store in i the id of the entry that is equal to l_format->name->GetValue()
                switch ((String((length_format->name->GetValue().ToStdString())).position_in_list(length_format->catalog))) {
                        
                    case 0: {
                        //l_format->name->GetValue() = "Time x speed" -> disable l, enable v and t
                        
                        b = true;
                        break;
                        
                    }
                        
                    case 1: {
                        //l_format->name->GetValue() = ((LengthFormat_types[1]).value) -> enable l, disable v and t
                        
                        b = false;
                        break;
                        
                    }
                        
                }
                
                
                time->Enable(b);
                speed->Enable(b);
                length->Enable(!b);
                text_time->Enable(b);
                text_speed->Enable(b);
                text_length->Enable(!b);
                
            }else{
                //length_format does not have a valid content -> disable all relative fields
                
                time->Enable(false);
                speed->Enable(false);
                length->Enable(false);
                text_time->Enable(false);
                text_speed->Enable(false);
                text_length->Enable(false);
                
            }
            
        }else {
            //the Route is a circle of equal altitude -> the length is not defined -> disable the length_format field as well as all fields related to the length
            
            length_format->Enable(false);
            
            time->Enable(false);
            speed->Enable(false);
            length->Enable(false);
            text_time->Enable(false);
            text_speed->Enable(false);
            text_length->Enable(false);
            
        }
        
    }else{
        //type is not vlaid
        
        length_format->Enable(false);
        
        time->Enable(false);
        speed->Enable(false);
        length->Enable(false);
        text_time->Enable(false);
        text_speed->Enable(false);
        text_length->Enable(false);
        
    }
    
    event.Skip(true);
    
}

template void RouteFrame::EnableDisableLengthFormatFields<wxKeyEvent>(wxKeyEvent&);
template void RouteFrame::EnableDisableLengthFormatFields<wxFocusEvent>(wxFocusEvent&);


//when time or speed are edited in RouteFrame, thie method updates the length GUI field by writing in it time x speed
template<class E> void RouteFrame::UpdateLength(E& event) {

    if ((time->is_ok()) && (speed->is_ok())) {

        length->set(Length(*(time->chrono), *(speed->object)));

    }else{

        length->value->SetValue(wxString(""));
        length->unit->name->SetValue(wxString(""));

    }

    event.Skip(true);

}

//same as RouteFrame::OnChooseLengthFormat(E& event), but it does not accept any argument
void RouteFrame::EnableDisableLengthFormatFields(void) {

    wxCommandEvent dummy;

    EnableDisableLengthFormatFields(dummy);

}
