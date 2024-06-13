//
//  data.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef data_h
#define data_h



class Data{
    
public:
    Catalog* catalog;
    //    FileR file_init;
    int job_id;
    stringstream command, plot_command;
    vector<Sight> sight_list;
    vector<Position> position_list;
    vector<Route> route_list;
    vector<String> choices;
    vector<Route> crossing_route_list;
    //the recent bodies, projections, ... selected by the user are stored here
    vector<int> recent_bodies, recent_projections, recent_length_formats, recent_route_types, recent_route_for_transport_types, recent_length_units, recent_speed_units;

    Data(Catalog*, String);
    //~Data();
    bool add_sight_and_reduce(Sight*, String);
    void add_route(Route*, String);
    void remove_sight(unsigned int, Answer, String);
    void remove_position(unsigned int, String);
    void remove_route(unsigned int, Answer, String);
    void read_from_file_to(String, String, String, String);
    template<class S> void read_from_stream(String, S*, bool, String);
    void print(bool, String, ostream&);
    void print_to_kml(String);
    void print_sights(String, ostream&);
    void print_positions(String, ostream&);
    void print_routes(bool, String, ostream&);
    
    void print_recent_items(const vector<int>&, String, String, ostream&);
    void insert_recent_item(unsigned int, vector<int>*);

//    void insert_recent_body(unsigned int);
//    void print_recent_bodies(String, ostream&);
//    void insert_recent_projection(unsigned int);
//    void print_recent_projections(String, ostream&);
//    void insert_recent_length_format(unsigned int);
//    void print_recent_length_formats(String, ostream&);
//    void insert_recent_route_type(unsigned int);
//    void print_recent_route_types(String, ostream&);

    void menu(String);
    int compute_position(String);
    
};


#endif
