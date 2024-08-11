//
//  data.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "data.h"

#include <sstream>

#include "generic.h"


// this function plots the Routes of type (Route_types[2]) in route_list in kml forma. WARNING: THIS FUNCTION USES THE SYSTEM() COMMAND AND THUS IT IS NOT PORTABLE ACROSS PLATFORMS. Also, this functions used file_kml, which has been removed from the code, and it should be revised.
void Data::print_to_kml(String prefix) {

    stringstream line_ins, /*plot_title contains the  title of the Route to be plotted*/ plot_title;
    string line;
    unsigned int i, j;
    double lambda_kml, phi_kml;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //    file_init.open(prefix);
    //    file_kml.remove(prefix);

        //replace line with number of points for routes in plot_dummy.plt
    plot_command.str("");
    command.str("");
    wxGetApp().n_points_routes.read_from_file_to(String("number of points for routes"), (wxGetApp().path_file_init), String("R"), new_prefix);



    //plot routes

    plot_command.str("");
    command.str("");
    for (i = 0; i < (route_list.size()); i++) {

        if (route_list[i].type == (Route_types[2])) {

            //this is the opening of a path code in kml format
            plot_command << "\\\t<Style id=\\\"" << i << "\\\">\\\n\\\t<LineStyle>\\\n\\\t\\\t<color>" << /*I use the remainder of i in this way, so if i > size of kml_colors, I start back reading from the beginning of kml_colors*/ kml_colors[i % (kml_colors.size())] << "<\\/color>\\\n\\\t\\\t<width>2<\\/width>\\\n\\\t<\\/LineStyle>\\\n\\\t<\\/Style>\\\n\\\t<Placemark>\\\n\\\t\\\t<name>"
                << (route_list[i]).label.value
                << "<\\/name>\\\n\\\t\\\t<styleUrl>#" << i << "<\\/styleUrl>\\\n\\\t\\\t<description>"
                << (route_list[i]).label.value
                << "<\\/description>\\\n\\\t\\\t<LineString>\\\n\\\t\\\t\\\t<extrude>1<\\/extrude>\\\n\\\t\\\t\\\t<tessellate>0<\\/tessellate>\\\n\\\t\\\t\\\t<altitudeMode>absolute<\\/altitudeMode>\\\n\\\t\\\t\\\t<coordinates>\\\n";



            for (j = 0; j < (unsigned int)(wxGetApp().n_points_routes.value); j++) {

                //I consider a Length equal to a temporary value of the length of the route, which spans between 0 and 2.0*M_PI*(Re*sin(((route_list[i]).omega.value))) across the for loop over j
                //I compute the coordinate of the endpoint of route_list[i] for the ((route_list[i]).l) above
                (route_list[i]).compute_end(Length(2.0 * M_PI * ((wxGetApp().Re.value) * sin(((route_list[i]).omega.value))) * ((double)j) / ((double)(wxGetApp().n_points_routes.value - 1))), new_prefix);

                //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude, and in the third altitude (I am not interested in altitude, thus is set it to 0); The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.

                lambda_kml = -rad_to_deg * ((route_list[i]).end->lambda.value);
                if (lambda_kml < -180.0) {
                    lambda_kml += 360.0;
                }

                phi_kml = rad_to_deg * ((route_list[i]).end->phi.value);
                if (phi_kml > 270.0) {
                    phi_kml -= 360.0;
                }

                plot_command << "\\\t\\\t\\\t\\\t" << lambda_kml << "," << phi_kml << ",0.0\\\n";

            }

            //this is the closing of a path code in kml format
            plot_command << "\\\t\\\t\\\t<\\/coordinates>\\\n\\\t\\\t<\\/LineString>\\\n\\\t<\\/Placemark>\\\n";

        }

    }

    //add the line to plot_kml.kml which contains the parametric plot of the routes
    command << "LANG=C sed 's/\\/\\/route\\_plots/" << plot_command.str().c_str() << "/g' kml_dummy.kml >> kml_temp.kml \n";

    //execute the command string
    system(command.str().c_str());




    //plot positions

    plot_command.str("");
    command.str("");
    for (i = 0; i < (position_list.size()); i++) {

        //this is the opening of a path code in kml format
        plot_command << "\\\n\\\t<Placemark>\\\n\\\t\\\t<Style>\\\n\\\t\\\t\\\t<IconStyle>\\\n\\\t\\\t\\\t\\\t<color>7733ff66<\\/color>\\\n\\\t\\\t\\\t\\\t<IconStyleSimpleExtensionGroup radius=\\\"3\\\" points=\\\"Infinity\\\" strokeColor=\\\"" << /*I use the remainder of i in this way, so if i > size of hex_colors, I start back reading from the beginning of hex_colors*/ hex_colors[i % (hex_colors.size())] << "\\\" strokeWidth=\\\"2\\\" lineDash=\\\"undefined\\\"\\/>\\\n\\\t\\\t\\\t<\\/IconStyle>\\\n\\\t\\\t\\\t<PolyStyle xmlns=\\\"\\\">\\\n\\\t\\\t\\\t\\\t<outline>0<\\/outline>\\\n\\\t\\\t\\\t<\\/PolyStyle>\\\n\\\t\\\t<\\/Style>\\\n\\\t\\\t<description>"
            << (position_list[i]).label.value << "<\\/description>\\\n\\\t\\\t<ExtendedData>\\\n\\\t\\\t\\\t<Data name=\\\"scStyle\\\"\\/>\\\n\\\t\\\t<\\/ExtendedData>\\\n\\\t\\\t<Point>\\\n\\\t\\\t\\\t<coordinates>";


        //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude. The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.

        lambda_kml = -rad_to_deg * ((position_list[i]).lambda.value);
        if (lambda_kml < -180.0) {
            lambda_kml += 360.0;
        }

        phi_kml = rad_to_deg * ((position_list[i]).phi.value);
        if (phi_kml > 270.0) {
            phi_kml -= 360.0;
        }

        plot_command << lambda_kml << "," << phi_kml << "<\\/coordinates>\\\n\\\t\\\t<\\/Point>\\\n\\\t<\\/Placemark>";

    }

    //add the line to plot_kml.kml which contains the parametric plot of the positions
    command << "LANG=C sed 's/\\/\\/position\\_plots/" << plot_command.str().c_str() << "/g' kml_temp.kml >> '" /*<< file_kml.name.value*/ << "'\nrm -rf kml_temp.kml\n";

    //execute the command string
    system(command.str().c_str());



    //    file_init.close(prefix);

}


Data::Data(Catalog* cata, [[maybe_unused]] String prefix) {

    String new_prefix;

    new_prefix = prefix.append(String("\t"));


    catalog = cata;
    job_id = -1;

    plot_command.precision((data_precision.value));
    command.precision((data_precision.value));

    //read number of intervals for ticks from file_init
    wxGetApp().n_intervals_ticks_preferred.read_from_file_to(String("preferred number of intervals for ticks"), wxGetApp().path_file_init, String("R"), new_prefix);

    //read number of points for routes from file_init
    wxGetApp().n_points_routes.read_from_file_to(String("number of points for routes"), wxGetApp().path_file_init, String("R"), new_prefix);

    //read n_points_plot_coastline_* from file_init
    wxGetApp().n_points_plot_coastline_Mercator.read_from_file_to(String("number of points coastline Mercator"), wxGetApp().path_file_init, String("R"), new_prefix);
    wxGetApp().n_points_plot_coastline_3D.read_from_file_to(String("number of points coastline 3D"), wxGetApp().path_file_init, String("R"), new_prefix);

}


/*compute the astronomical position by wriitng it into center, and the circle of uncertainty by writing it into error_cirocle. Then I push back center and error_circle to position_list and route_list, respectively.
 Return value:
    * If all crossings have been used to compute the astronomical position
        - return 0 if [ # crossings used ] >= 2
        - return 2 if [ # crossings used ] = 1
    * If only part of the crossings have been used to compute the astronomical position
        - return 1 if [ # crossings used ] >= 2
        - return 2 if [ # crossings used ] = 1
    * If no crossings could be used to compute the astronomical position , return -1
 */
 
int Data::compute_position(String prefix) {

    unsigned int i, j, l;
    String new_prefix;
    stringstream dummy;
    Length r, s;
    Position center;
    double x;
    Route error_circle;
    int output;


    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if (crossing_route_list.size() > 1) {
        //there are enough Routes in crossing_route_list -> I compute the crossing

        vector< vector<Position> > p;
        vector<Position> q, q_temp(2);

        cout << prefix.value << "Computing crossings between routes :\n";
        for (i = 0; i < crossing_route_list.size(); i++) {
            cout << new_prefix.value << "\t" << (crossing_route_list[i]).label.value << "\n";
        }

        //I run over all the pairs of circles of equal altitude and write their crossing points into p
        l = 0;
        for (i = 0; i < crossing_route_list.size(); i++) {
            for (j = i + 1; j < crossing_route_list.size(); j++) {

                //                cout << prefix.value << "Computing crossing between routes " << crossing_route_list[i]+1 << " and " << crossing_route_list[j]+1 << "\n";

                if (((crossing_route_list[i]).crossing(crossing_route_list[j], &q_temp, &x, new_prefix)) >= 0) {
                    //in this case, the two routes under consideration intercept with no error message

                    //if the two routes under consideration are not too parallel (i.e., |cos(their crossing angle)| < cos((wxGetApp().min_crossing_angle)), then I add this crossing to the list of sensible crossings
                    if (fabs(x) < cos((wxGetApp().min_crossing_angle))) {

                        p.resize(l + 1);
                        (p[l]).resize(2);

                        p[l] = q_temp;

                        my_push_back(&q, q_temp[0]);
                        my_push_back(&q, q_temp[1]);

                        l++;

                        cout << new_prefix.value << "Crossing is valid.\n";

                    }
                    else {

                        cout << new_prefix.value << "Crossing is not valid.\n";

                    }

                }

            }
        }


        if (l > 0) {
            //there is at least one valid crossing -> the astronomical Position can be computed


            //r is the minimal distance between crossing points. To find the minimum, here I set r to it largest possible value, obtained when the two points are at the antipodes. I find the pair of crossing points which is closest to each other, and set Position center to one of the Positions in this pair. center will thus represent the approximate astronomical position. I will then run over all the pairs of crossing points in p, p[i], and pick either p[i][0] or p[i][1]: I will pick the one which is closest to center

            cout << prefix.value << "Distances between pairs of crossing positions:\n";
            r.set(M_PI * (wxGetApp().Re.value), LengthUnit_types[0]);

            for (i = 0; i < q.size(); i++) {
                for (j = i + 1; j < q.size(); j++) {

                    dummy.str("");
                    dummy << "distance between points " << i << " and " << j;

                    (q[i]).distance((q[j]), &s, String(dummy.str()), new_prefix);

                    if(r > s){
                        r = s;
                        center = (q[i]);
                    }

                }
            }

            r.print(String("minimal distance between crossing points"), prefix, cout);
            center.print(String("center crossing"), prefix, cout);

            //I append center to the list of retained crossings, run through all the pairs of crossings except for center, and select the Position in the pair which is closer to center
            q.clear();
            my_push_back(&q, center);

            for (i = 0; i < p.size(); i++) {

                if (!(((p[i][0]) == center) || ((p[i][1]) == center))) {

                    center.distance(p[i][0], &r, String(""), new_prefix);
                    center.distance(p[i][1], &s, String(""), new_prefix);

                    if(r > s){

                        my_push_back(&q, p[i][1]);

                    }else{

                        my_push_back(&q, p[i][0]);

                    }

                }

            }
            

            //compute astronomical position by averaging on all viable crossing points
            center.lambda.value = 0.0;
            center.phi.value = 0.0;
            for (i = 0; i < q.size(); i++) {

                (center.lambda.value) += ((q[i]).lambda.value);
                (center.phi.value) += ((q[i]).phi.value);

            }
            (center.lambda.value) /= ((double)(q.size()));
            (center.phi.value) /= ((double)(q.size()));
            center.label.set(String(""), String("astronomical position"), prefix);
            center.print(String("astronomical position"), prefix, cout);

            my_push_back(&position_list, center);

            
            if(q.size() > 1){
                //there are >= 2 crossings -> the error on the astronomical position can be computed
                
                //compute error on astronomical position
//                (r.value) = 0.0;
                r.set(0.0, LengthUnit_types[0]);
                
                for (i = 0; i < q.size(); i++) {
                    for (j = i + 1; j < q.size(); j++) {
                        
                        (q[i]).distance(q[j], &s, String(""), new_prefix);
                        r += s;
                        
                    }
                }
                r /= ((double)((q.size()) * ((q.size()) - 1) / 2));
                
                //computes the circle of equal altitude which represents the error of the sight
                (error_circle.type) = RouteType(((Route_types[2]).value));
                (*(error_circle.reference_position)) = center;
                (error_circle.omega.value) = (r.value) / (wxGetApp().Re.value);
                (error_circle.label) = String("error on astronomical position");
                ((error_circle.related_sight).value) = -1;
                
                r.print(String("error on astronomical position"), prefix, cout);
                my_push_back(&route_list, error_circle);
                
                
                if (l == (crossing_route_list.size()) * ((crossing_route_list.size()) - 1) / 2) {
                    //all Routes in crossing_route_list have been used to get the position
                    
                    if(l > 1){
                        // all crossings have been used and there are >= 2 crossings used to compute the astronomical Position -> the error on the astronomical Position could be computed
                        
                        output = 0;
                        
                    }else{
                        // all crossings have been used and there is 1 crossing used to compute the astronomical Position  -> the error on the astronomical Position could not be computed

                        output = 2;
                        
                    }

                }else {
                    //only some Routes in crossing_route_list have been used to get the position

                    if(l > 1){
                        // only some crossings have been used  and there are >= 2 crossings used to compute the astronomical Position -> the error on the astronomical Position could be computed
                        
                        output = 1;
                        
                    }else{
                        // only some crossings have been used  there is 1 crossing used to compute the astronomical Position -> the error on the astronomical Position could not be computed

                        output = 2;
                        
                    }
                    
                }

            }else{
                
                cout << prefix.value << RED << "I could not compute the error on the astronomical position because there are not enough valid crossings!\n" << RESET;
                
                output = 2;
                
            }

        }else {

            cout << prefix.value << RED << "I could not compute the position because there are no valid crossings!\n" << RESET;
            output = -1;

        }

        p.clear();
        q.clear();
        q_temp.clear();


    }else {
        //there are not enough Routes in crossing_route_list -> I cannot compute the crossing

        cout << prefix.value << RED << "I could not compute the position because there are no valid Routes!\n" << RESET;
        
        output = -1;
        
    }
    
    return output;

}

//print all the data in data to ostr
void Data::print(bool print_all_routes, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Data:\n";

    print_sights(new_prefix, ostr);
    print_routes(print_all_routes, new_prefix, ostr);
    print_positions(new_prefix, ostr);

    print_recent_items(recent_bodies, String("bodies"), prefix, ostr);
    print_recent_items(recent_projections, String("projections"), prefix, ostr);
    print_recent_items(recent_length_formats, String("length formats"), prefix, ostr);
    print_recent_items(recent_route_types, String("route types"), prefix, ostr);
    print_recent_items(recent_route_for_transport_types, String("route for transport types"), prefix, ostr);
    print_recent_items(recent_length_units, String("length units"), prefix, ostr);
    print_recent_items(recent_speed_units, String("speed units"), prefix, ostr);
    
    recent_route_speed_value.print(String("Recent route speed value"), prefix, ostr);

}

void Data::print_sights(String prefix, ostream& ostr) {

    stringstream name;
    unsigned int i;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Sights:\n";
    for (i = 0; i < sight_list.size(); i++) {
        name.str("");
        name << "Sight #" << i + 1;
        (sight_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
    }


}

void Data::print_positions(String prefix, ostream& ostr) {

    stringstream name;
    unsigned int i;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Positions:\n";
    for (i = 0; i < position_list.size(); i++) {
        name.str("");
        name << "Position #" << i + 1;
        (position_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
    }


}

void Data::print_routes(bool print_all_routes, String prefix, ostream& ostr) {

    stringstream name;
    unsigned int i, j;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Routes:\n";

    for (i = 0, j = 0; i < (route_list.size()); i++) {

        //if print_all_routes = false, I only print routes which are not linked to a sight. This is to avoid doubles: If I print also Routes which are related to a Sight, then when the file to which I am saving will be read again, the code will reduce them and create double Routes identical to the ones already present in the file.
        if (((((route_list[i]).related_sight).value) == -1) || print_all_routes) {

            name.str("");
            name << "Route #" << j + 1;

            (route_list[i]).print(String(name.str().c_str()), new_prefix, ostr);

            j++;

        }

    }


}


bool Data::add_sight_and_reduce(Sight* sight_in, [[maybe_unused]] String prefix) {


    bool check = true;

    //I link the sight to the route, and the route to the sight
    //create a new route in the respective list
    route_list.resize(route_list.size() + 1);
    (sight_in->related_route.value) = ((int)(route_list.size())) - 1;
    //push back sight_in into sight_list
    my_push_back(&sight_list, *sight_in);
    (((route_list[route_list.size() - 1]).related_sight).value) = ((int)(sight_list.size())) - 1;

    //I commented this out because now the sight is enetered through the GUI
    //    (sight_list[sight_list.size()-1]).enter((*catalog), String("new sight"), prefix);
    check &= ((sight_list[sight_list.size() - 1]).reduce(&(route_list[route_list.size() - 1]), prefix));

    //I link the sight to the route, and the route to the sight
    ((sight_list[sight_list.size() - 1]).related_route.value) = ((int)route_list.size()) - 1;
    (sight_in->related_route.value) = ((int)route_list.size()) - 1;
    (((route_list[route_list.size() - 1]).related_sight).value) = ((int)sight_list.size()) - 1;


    if (check) {
        (sight_list[sight_list.size() - 1]).print(String("Sight"), prefix, cout);

        cout << prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";
        cout << prefix.value << "Route added as route #" << route_list.size() << ".\n";

    }

    return check;

}


//adds to Data-> this the Route written in *route_in
void Data::add_route(Route* route_in, [[maybe_unused]] String prefix) {


    //    route.enter(String("new route"), prefix);

    my_push_back(&route_list, *route_in);
    cout << prefix.value << "Route added as position #" << route_list.size() << ".\n";


}



//removes sight #i from sight_list by updating all the connections to between sights and routes. If remove_related_route = 'y', it removes also the route related to sight i
void Data::remove_sight(unsigned int i, Answer remove_related_route, [[maybe_unused]] String prefix) {

    stringstream name;
    unsigned int j;
    Int i_related_route;

    i_related_route = ((sight_list[i]).related_route);

    name.str("");
    name << "Sight to be removed: Sight #" << i + 1;

    (sight_list[i]).print(String(name.str().c_str()), prefix, cout);

    sight_list.erase(sight_list.begin() + i);

    //update the linking indexed of routes in accordance with the deletion of the sight
    for (j = 0; j < route_list.size(); j++) {

        if (((((route_list[j]).related_sight).value) != -1) && ((((route_list[j]).related_sight).value) >= ((int)i))) {

            if ((((route_list[j]).related_sight).value) == ((int)i)) {
                (((route_list[j]).related_sight).value) = -1;
            }
            else {
                (((route_list[j]).related_sight).value)--;
            }

        }

    }

    cout << prefix.value << "Sight removed.\n";


    if ((i_related_route.value) != -1) {

        if (remove_related_route == Answer('y', prefix)) {
            //the related route must be removed -> I remove it

            remove_route((i_related_route.value), Answer('n', prefix), prefix);

        }
        else {
            //the related route must not be removed: given that its related sight has been deleted, I set its related_sight.value to -1

            (route_list[i_related_route.value]).related_sight.set(-1);

        }

    }

}

void Data::remove_position(unsigned int i, [[maybe_unused]] String prefix) {

    stringstream name;

    name.str("");
    name << "Position to be removed: #" << i + 1;

    (position_list[i]).print(String(name.str().c_str()), prefix, cout);

    position_list.erase(position_list.begin() + i);

    cout << prefix.value << "Position removed.\n";

}

//removes route #i from route_list by updating all the connections to between sights and routes. If remove_related_sight = 'y', it removes also the sight related to route i
void Data::remove_route(unsigned int i, Answer remove_related_sight, [[maybe_unused]] String prefix) {

    unsigned int j;
    Int i_related_sight;
    stringstream name;

    (i_related_sight.value) = (((route_list[i]).related_sight).value);

    name.str("");
    name << "Route to be removed: #" << i + 1;

    (route_list[i]).print(String(name.str().c_str()), prefix, cout);

    route_list.erase(route_list.begin() + i);

    //update the linking indexed of sights in accordance with the deletion of the route
    for (j = 0; j < sight_list.size(); j++) {

        if ((((sight_list[j]).related_route.value) != -1) && (((sight_list[j]).related_route.value) >= ((int)i))) {

            if (((sight_list[j]).related_route.value) == ((int)i)) {
                ((sight_list[j]).related_route.value) = -1;
            }
            else {
                ((sight_list[j]).related_route.value)--;
            }

        }

    }

    cout << prefix.value << "Route removed.\n";


    if (((i_related_sight.value) != -1) && (remove_related_sight == Answer('y', prefix))) {

        remove_sight(i_related_sight.value, Answer('n', prefix), prefix);

    }

}


//read from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Data::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Data>(this, name, filename, mode, prefix);

}


template<class S> void Data::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;
    String new_prefix, temp;
    bool check;


    check = true;

    cout << prefix.value << "Reading " << name.value << " from stream " << input_stream << "... \n";


    if (search_entire_stream) {
        //rewind *input_stream, look for the beginning of the data block corresponding to name, and read its first line

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline(*input_stream, line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));


    }
    else {
        //read the first line of the data block corresponding to name

        line.clear();
        getline(*input_stream, line);

    }

    cout << prefix.value << "... done.\n";



    //1. Read Sights

    //read dummy text line '    Sights:"
    getline((*input_stream), line);

    line.clear();
    //read dummyt text line
    getline((*input_stream), line);
    pos = line.find("Sight #");

    //if I have found 'Sight #' in the line above, then I proceed and read the relative sight
    while (pos != (string::npos)) {

        cout << new_prefix.value << "Found new sight!\n";

        //read the sight block
        Sight sight;
        Route route;

        //if I find a sight which returns an error message when read from file, to be conservative I do not add any of the following sights in the file to sight_list because they may contain other errors
        check &= (sight.read_from_stream<S>(String("sight"), input_stream, false, new_prefix));
        if (check) {

            check &= (sight.reduce(&route, new_prefix));

            if (check) {
                sight.print(String("New sight"), new_prefix, cout);

                my_push_back(&sight_list, sight);
                cout << new_prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";

                my_push_back(&route_list, route);
                cout << new_prefix.value << "Route added as route #" << route_list.size() << ".\n";

                //I link the sight to the route, and the route to the sight
                ((route_list[route_list.size() - 1].related_sight).value) = ((int)(sight_list.size())) - 1;
                ((sight_list[sight_list.size() - 1].related_route).value) = ((int)(route_list.size())) - 1;

            }

        }

        line.clear();
        //read dummyt text line
        getline((*input_stream), line);
        pos = line.find("Sight #");

    }

    //2. read Routes

    line.clear();
    //read dummy text line
    getline((*input_stream), line);
    pos = line.find("Route #");

    //if I have found 'Route #' in the line above, then I proceed and read the relative position
    while (pos != (string::npos)) {

        cout << new_prefix.value << "Found new route!\n";

        //read the position block
        Route route;

        route.read_from_stream<S>(String("route"), input_stream, false, new_prefix);

        route.print(String("New route"), new_prefix, cout);

        my_push_back(&route_list, route);
        cout << new_prefix.value << "Route added as route #" << route_list.size() << ".\n";

        line.clear();
        //read dummyt text line
        getline((*input_stream), line);
        pos = line.find("Route #");

    }


    //3. read Positions

    line.clear();
    //read dummy text line
    getline((*input_stream), line);
    pos = line.find("Position #");

    //if I have found 'Position #' in the line above, then I proceed and read the relative position
    while (pos != (string::npos)) {

        cout << new_prefix.value << "Found new position!\n";

        //read the position block
        Position position;

        position.read_from_stream<S>(String("position"), input_stream, false, new_prefix);

        position.print(String("New position"), new_prefix, cout);

        my_push_back(&position_list, position);
        cout << new_prefix.value << "Position added as position #" << position_list.size() << ".\n";

        line.clear();
        //read dummyt text line
        getline((*input_stream), line);
        pos = line.find("Position #");

    }

    //4. Read recent items
    //read recent bodies and projections and ...
    read_list_from_stream<S>(String("Recent bodies"), input_stream, true, &recent_bodies);
    read_list_from_stream<S>(String("Recent projections"), input_stream, true, &recent_projections);
    read_list_from_stream<S>(String("Recent length formats"), input_stream, true, &recent_length_formats);
    read_list_from_stream<S>(String("Recent route types"), input_stream, true, &recent_route_types);
    read_list_from_stream<S>(String("Recent route for transport types"), input_stream, true, &recent_route_for_transport_types);
    read_list_from_stream<S>(String("Recent length units"), input_stream, true, &recent_length_units);
    read_list_from_stream<S>(String("Recent speed units"), input_stream, true, &recent_speed_units);
    
    recent_route_speed_value.read_from_stream(String("Recent route speed value"), input_stream, true, prefix);
    
}

template void Data::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);


//print recent_items to ostr. Here name is the "[plural name of the item]"  : for example, if I am printing a list of Bodies, name.value = "bodies". This method is used for GUI fields of the format MultipleItemField
void Data::print_recent_items(const vector<int>& recent_items, String name, String prefix, ostream& ostr) {

    unsigned int i;
    stringstream temp;


    if(recent_items.size() != 0){
        //recent_items is non-empty -> write into temp its content
        
        for (temp.str(""), i = 0; i < (recent_items.size()) - 1; i++) {
            temp << recent_items[i] << " ";
        }
        temp << recent_items.back();
        
    }else{
        //recent_items is empty -> write into temp the 0-th element only
        
        temp << 0;
        
    }
    
    String(temp.str().c_str()).print(String("Recent ").append(name), false, prefix, ostr);

}


//insert the item 'item_id' into the vector of items *recent_items.  This method is used for GUI fields of the format MultipleItemField. This method does not require recent_items->size() to be > 0
void Data::insert_recent_item(unsigned int item_id, vector<int>* recent_items) {
    
    if((recent_items->size()) > 0){
        //*recent_items is non-empty
        
        vector<int>::iterator position;
        
        position = find(recent_items->begin(), recent_items->end(), item_id);
        
        if (position == recent_items->end()) {
            //in this case, the selected item is not in *recent_items: I write it in *recent_items
            
            (*recent_items)[recent_items->size() - 1] = item_id;
            rotate(recent_items->begin(), recent_items->end() - 1, recent_items->end());
            
        }
        else {
            //the selected item is in *recent_itmes: I move the element in position to the first place in *recent_items
            
            iter_swap(recent_items->begin(), position);
            
        }
        
    }else{
        //*recent_items is empty -> I push back item into *recent_items
        
        recent_items->push_back(item_id);
        
    }

}


