//
//  route.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "route.h"

#include "gsl_sf_pow_int.h"

#include "generic.h"
#include "constants.h"


//construct a brand new Route object and thus sets its related sight to -1, because this Route is not related to any sight yet. length_format_t_v is set to false: as the Route is created, lengths are written in l rather than in t and v
Route::Route(void) {

    related_sight.set(-1);
    length_format.set((LengthFormat_types[1]));
    
    reference_position = new Position;
    end = new Position;
    length = new Length;
    speed = new Speed;

}

//constructs a brand new Route. where lengths are written in time * speed than in l. The route type is either ((Route_types[0]).value) or ((Route_types[1]).value) and thus set its related sight to -1, because this Route cannot be related to any sight.
Route::Route(RouteType type_in, Position reference_position_in, Angle Z_in, Chrono time_in, Speed speed_in) {

    reference_position = new Position;
    end = new Position;
    length = new Length;
    speed = new Speed;


    length_format.set((LengthFormat_types[0]));
    type = type_in;
    
    time = time_in;
    (*speed) = speed_in;
    set_length_from_time_speed();
    
    (*reference_position) = reference_position_in;
    Z = Z_in;
    related_sight.set(-1);

}


//constructs a brand new Route. where lengths are written as a simple length rather than as time * speed. The route type is either ((Route_types[0]).value) or ((Route_types[1]).value) and thus set its related sight to -1, because this Route cannot be related to any sight.
Route::Route(RouteType type_in, Position reference_position_in, Angle Z_in, Length l_in) {
    
    reference_position = new Position;
    end = new Position;
    length = new Length;
    speed = new Speed;

    length_format.set((LengthFormat_types[1]));
    type = type_in;
    
    (*length) = l_in;
    length->convert_to(LengthUnit_types[0]);
    
    (*reference_position) = reference_position_in;
    Z = Z_in;
    related_sight.set(-1);

}


//build a Route of type type that connects position_start and position_end
Route::Route(const RouteType& type_in,  Position p_start,  Position p_end){
    
    reference_position = new Position;
    end = new Position;
    length = new Length;
    speed = new Speed;

    
    type = type_in;
    
    (*reference_position) = p_start;
    (length_format.value) = ((LengthFormat_types[1]).value);
    
    switch (position_in_vector(type, Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            PositionProjection projection_start, projection_end;
            
            projection_start.SetMercator(p_start);
            projection_end.SetMercator(p_end);
            
            
            if(p_start.phi != p_end.phi){
                //I am not in the special case where p_start and p_end have the same latitude
                
                
                //            reference_position.phi.normalize_pm_pi();
                
                //set Z
                Z.set(
                      String(""),
                      -GSL_SIGN((projection_end.x) - (projection_start.x))*acos(sqrt(1.0/(1.0 + gsl_pow_2(((projection_end.x) - (projection_start.x))/((projection_end.y) - (projection_start.y)))))),
                      String("")
                      );
                if((projection_end.y) > (projection_start.y)){
                    Z = Z*(-1.0);
                }else{
                    Z = Z + M_PI;
                }
                
            }else{
                //I am in the special case where p_start and p_end have the same latitude
                
                //set Z
                Z.set(
                      String(""),
                      -GSL_SIGN((projection_end.x) - (projection_start.x))*M_PI_2 + M_PI,
                      String("")
                      );
                
            }
            
            if(fabs((p_end.lambda.value) - (p_start.lambda.value)) > epsilon_double){
                //p_start and p_end have different longitudes
                
                //set *length from to t*  (see notes)
                set_length_from_input(fabs((p_end.lambda.value) - (p_start.lambda.value)));
                
            }else{
                //p_start and p_end have the same longitude

                //the loxodrome is directed to either N or S -> set *length from the latitudes of p_start and p_end
                //set the length format, the length unit and the value of the length from t
                length_format.set(LengthFormat_types[1]);
                length->set((wxGetApp().Re.value) * fabs((p_end.phi.value) - (p_start.phi.value)), LengthUnit_types[0]);
                
            }
            
            
            break;
            
        }
            
        case 1:{
            //*this is an orthodrome
            
            //p_start and p_end in Cartesian coordinates
            Cartesian r_start, r_end, s;
            Angle phi, z;
            Position p_a, p_b, end_1, end_2;
            
            p_start.getCartesian(&r_start);
            p_end.getCartesian(&r_end);
            
            (*reference_position) = p_start;
            
            
            //set the legnth as the length of the shortest great circle joining p_start and p_end
            phi.set(acos(r_start.dot(r_end)));
            
            length->set((wxGetApp().Re.value)*(phi.value), LengthUnit_types[0]);
            
            //set the tentative solution for the azimuth angle z: Z may be either z  (solkution 1) or -z (solution 2), I will pick the correct solution later
            z.set(String(""),
                  acos(-csc(phi) * sec(p_start.phi) * (cos(phi) * sin(p_start.phi) - sin(p_end.phi)) ),
                  String(""));

            //consider solution 1, compute end with this solution and store it in end_1
            Z.set(z.value);
            compute_end(String());
            end_1 = (*end);
            
            //consider solution 2, compute end with this solution and store it in end_2
            Z.set(-z.value);
            compute_end(String());
            end_2 = (*end);
            
            //check which one among end_1 and end_2 has a longitude closer to p_end and pick the correct solution accordingly
            if(fabs(end_1.lambda.value - p_end.lambda.value) < fabs(end_2.lambda.value - p_end.lambda.value)){
                Z.set(z.value);
            }else{
                Z.set(-z.value);
            }
            

            break;
            
        }
            
        case 2:{
            //*this is a circle of equal altitude
            
            cout << RED << "Cannot create a circle of equal altitute taht connects two positions!\n" << RESET;
            break;}
            
    }
    
}


//construct a brand new Route object of type ((Route_types[2]).value) and thus sets its related sight to -1, because this Route is not related to any sight yet.  length_format_t_v is set to false: as the Route is created, lengths are written in l rather than in t and v
Route::Route(RouteType type_in, Position reference_position_in, Angle omega_in) {

    reference_position = new Position;
    end = new Position;
    length = new Length;
    speed = new Speed;

    
    type = type_in;
    (*reference_position) = reference_position_in;
    omega = omega_in;

    length_format.set((LengthFormat_types[1]));
    
    //the lenght of the circle of equal altitude is set by default
    length->set(two_M_PI * (wxGetApp().Re.value) * sin(omega), LengthUnit_types[0]);

    related_sight.set(-1);

}

//add the Route this to the wxListCtrl listcontrol
void Route::add_to_ListControl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    unsigned int i;
    wxListItem item;

    if (position_in_listcontrol == -1) {
        //in this case, I add a new element at the end of listcontrol

        i = (listcontrol->GetItemCount());

    }
    else {
        //in this case, I delete the i-th elment in listcontrol and replace it

        i = ((unsigned int)position_in_listcontrol);
        listcontrol->DeleteItem(i);

    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_ListControl(i, listcontrol);

}


//reset *lines and tabulate the points of Route *this in any projection of draw_panel and write them into *lines.
void Route::DrawOld(
                    unsigned int n_points,
                    DrawPanel* draw_panel,
                    Lines* lines,
                    [[maybe_unused]] String prefix
                    ) {

    wxPoint p;
    bool starting_new_chunk;
    unsigned int i, n_points_chunk;
    Length length_saved;
    
    if(length_format == LengthFormat_types[0]){
        //length_format = LengthFormat_types[0] -> compute length from time and speed and have it in units LengthUnit_types[0] because this is the standard unit used to draw Routes
        
        set_length_from_time_speed();

    }else{
        //length_format = LengtFormat_types[1] -> save *length into length_saved and convert the unit of measure of *length to LengthUnit_types[0] because this is the standard unit used to draw Routes

        length_saved.set((*length));
        length->convert_to(LengthUnit_types[0]);

    }


    //tabulate the Route points
    //go through all the Route points
    for(n_points_chunk = 0, starting_new_chunk = true, i = 0; i < n_points; i++) {

        compute_end((*length)*((double)i)/((double)(n_points - 1)), String(""));
        
        //treat the first and last point as a special one because it may be at the boundary of *rectangle_observer-> check if they are and, if they are, put them back into *rectangle_observer
        if((i==0) || (i==n_points-1)){
            end->put_back_in(draw_panel);
        }

        if ((draw_panel->GeoToDrawPanel)((*end), &p, false)) {
            
            //the Route point considered is valid -> I increase n_points_chunk

            if (starting_new_chunk) {
                //I reached the end of a chunk
           
                starting_new_chunk = false;

            }

            lines->points.push_back(p);
            n_points_chunk++;
            
            if(i==n_points-1){
                
                lines->positions.push_back((lines->positions.back()) + n_points_chunk);
                
            }

        }else{
            //I have reached the end of a chunk

            //I set starting_new_chunk = true in such a way that the next iterations will recognize it
            starting_new_chunk = true;
            if(n_points_chunk > 0){
                lines->positions.push_back((lines->positions.back()) + n_points_chunk);
            }
            n_points_chunk = 0;

        }

    }
    

    
    //write back length_saved into *length
    length->set(length_saved);

}


//tabulate the points of Route *this and write them into *lines.
void Route::Draw(
                 unsigned int n_points,
                 DrawPanel* draw_panel,
                 Lines* lines,
                 [[maybe_unused]] String prefix
                 ) {

    unsigned int i, j;
    wxPoint p;
    Angle lambda_a, lambda_b;
    vector<Length> s;
    bool compute_l_ends_ok;
    Length length_saved;

    
    if(length_format == LengthFormat_types[0]){
        //length_format = LengthFormat_types[0] -> compute length from time and speed and have it in units LengthUnit_types[0] because this is the standard unit used to draw Routes
        
        set_length_from_time_speed();

    }else{
        //length_format = LengtFormat_types[1] -> save *length into length_saved and convert the unit of measure of *length to LengthUnit_types[0] because this is the standard unit used to draw Routes

        length_saved.set((*length));
        length->convert_to(LengthUnit_types[0]);

    }

    compute_l_ends(&s, &compute_l_ends_ok, draw_panel, prefix);
 
    //comoute the end values of l and writes them in s. If compute_l_ends returns true, than the endpoints have been computed correclty, and I can proceed
    if(compute_l_ends_ok){
        
        //a vector where I will store the tentative points of each chunk of *this
//        vector<wxPoint> temp;
        //the number of points of each chunk for which GeoToDrawPanel returns true (without recurring to put_back_in)
        unsigned int n_points_check_ok;
        
         //run over all chunks of *this which are visible
         //given that s contains the number of intersection points of *this and that each pair of intersection point delimits a chunk, and that v contains the chunks, the size of v is equal to thte size of s minus one.
        for(j=0; j<(s.size()) - 1; j++) {
            //run over all chunks
 
            //tabulate the Route points of the jth chunk and store them in w
            for(draw_panel->points_dummy.clear(), n_points_check_ok=0, i = 0; i < n_points; i++) {

                //I slightly increase s[j] and slightly decrease s[j+1] (both by epsilon_double) in order to plot a chunk of the Route *this which is slightly smaller than the chunk [s[j], s[j+1]] and thus avoid  the odd lines that cross the whole plot area in the Mercator projection and that connect two points of the same chunk that are far from each other  on the plot area
                //                compute_end(Length(((s[j]).value) * (1.0 + epsilon_double) + (((s[j + 1]).value) * (1.0 - epsilon_double) - ((s[j]).value) * (1.0 + epsilon_double)) * ((double)i) / ((double)(n_points - 1))), String(""));
                compute_end(
                            (s[j] * (1.0 + epsilon_double)) + (((s[j + 1] * (1.0 - epsilon_double)) - ((s[j] * (1.0 + epsilon_double)))) * ((double)i)/((double)(n_points - 1))),
                            String(""));
                
                if(((draw_panel->GeoToDrawPanel)((*end), &p, false))){
                    //end is a valid point -> convert it to a Position with GeoToDrawPanel

                    draw_panel->points_dummy.push_back(p);
                    n_points_check_ok++;

                }else{
                    //end is not a valid point
                    
                    if((i==0) || (i==n_points-1)){
                        //the non-valid point is the first or last point in the Route chunk -> the point may be non valid because it lies on the edge, i.e., because of a rounding error -> put it back in and, if the Position that has been put_back_in is valid, convert it to a Position with GeoToDrawPanel
                        
                        end->put_back_in(draw_panel);
                        
                        if((draw_panel->GeoToDrawPanel)((*end), &p, false)){
                            
                            draw_panel->points_dummy.push_back(p);
                            
                        }

                    }else{
                        //the non-valid point lies in the middle of the Route chunk -> the reason why the point is non-valid cannot be a rounding error -> do not push the point to v_tentaive and break the for loop over i to terminate drawing the route chunk and switch to the next one
                        
                        break;
                        
                    }
     
                }
                
            }
            
            //now I decide if v_proposed is a valid chunk (a chunk to be plotted), and thus if I sholud push it back to v or not
            if(n_points_check_ok > 0){
                //w containts at least one point for which GeoToDrawPanel evaluated to true (without recurring to put_back_in) -> it is a valid chunk -> I add it to points. On the other hand, if n_points_check_ok == 0, then the only points in w may be the first and the last, which have been pushed back to w by put_back_in, and the chunk will be an odd chunk with only two points put into *rectangle_observer by put_back_in -> This may lead to odd diagonal lines in the Mercator projection: thus, if n_points_check_ok == 0, I do not insert anytying in *points
                
                //I update *points
                lines->points.insert(lines->points.end(), draw_panel->points_dummy.begin(), draw_panel->points_dummy.end());
                //I update *poisitions
                lines->positions.push_back((lines->positions.back()) + (draw_panel->points_dummy.size()));
                
            }

        }

    }
    
    //write back length_saved into length
    length->set(length_saved);

}




//compute the values of the Length l for Route *this at which *this crosses draw_panel->*circle/*rectangle_observer, and writes them in *s. For (*s)[i] < l < (*s)[i+1], the Route *this lies within draw_panel -> circle/ draw_panel->*rectangle_observer, and it is thus visible. If success != NULL, it writes true in *success if the values of the length above could be computed succesfully, and false otherwise.
//inline 
void Route::compute_l_ends(vector<Length>* s, bool* success, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {
    
    vector<Angle> t;
    
    switch (position_in_vector(type, Route_types)) {
            
        case 0: {
            //the Route this is a loxodrome
            
            cout << prefix.value << RED << "Cannot execute compute_l_ends: the Route is not an orthodrome nor a circle of equal altitude!\n" << RESET;
            
            if (success != NULL) {
                (*success) = false;
            }
            
            break;
            
        }
            
        case 1: {
            //the Route this is an orthodrome
            
            int check = -1;
            
            switch (position_in_vector(draw_panel->parent->projection, Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    check = inclusion((*(draw_panel->rectangle_observer)), true, &t, String(""));
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    check = inclusion((*(draw_panel->circle_observer)), true, &t, String(""));
                    
                    break;
                    
                }
                    
            }
            
            
            if(check == 1){
                //there is a part of *this which is included in *circle/*rectangle_observer -> some part of *this will lie on the visible part of the earth
                
                unsigned int i;
                                
                for (s->resize(t.size()), i = 0; i < (t.size()); i++) {
                    
                    ((*s)[i]).set(((t[i]).value) * (wxGetApp().Re.value));
                    
                }
                
                t.clear();
                
                if (success != NULL) {
                    (*success) = true;
                }
                
            }else{
                //no part of this is included in circle/rectagle observer -> no part of this lies on the visible part of the earth
                
                if (success != NULL) {
                    (*success) = false;
                }
                
            }
            
            break;
            
        }
            
        case 2: {
            //the Route this is a circle of equal altitde.  its total length is the length of the circle itself, which reads:
            
            switch (position_in_vector(draw_panel->parent->projection, Projection_types)) {
                    
                case 0: {
                    //I am using the Projection_types[0] projection
                    
                    if (inclusion((*(draw_panel->rectangle_observer)), true, &t, String("")) == 1) {
                        //*this is included in *rectangle_observer
                        
                        if ((t[0] == 0.0) && (t[1] == 0.0)) {
                            //*this is fully included into *rectangle_observer and does not interscet with circle_observer: in this case, I draw the full circle of equal altitude *this
                            
                            s->resize(2);
                            ((*s)[0]).set(0.0, LengthUnit_types[0]);
                            ((*s)[1]).set(two_M_PI * (wxGetApp().Re.value) * sin(omega), LengthUnit_types[0]);
                            
                        }else{
                            
                            unsigned int i;
                            
                            //*this is partially included into *rectangle_observer and it interscets *rectangle_observer-> I write in s the values of the parametric length of *this at which these intersections occur
                            
                            for (s->resize(t.size()), i = 0; i < (t.size()); i++) {
                                
                                ((*s)[i]).set(((t[i]).value) * (wxGetApp().Re.value) * sin(omega), LengthUnit_types[0]);
                                
                            }
                            
                        }
                        
                        if (success != NULL) {
                            (*success) = true;
                        }
                        
                    }
                    else {
                        //*this is not included in *rectangle_observer
                        
                        if (success != NULL) {
                            (*success) = false;
                        }
                        
                    }
                    
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using the Projection_types[1] projection
                    
                    if (inclusion((*(draw_panel->circle_observer)), true, &t, String("")) == 1) {
                        //there is a part of *this which is included in circle_observer -> some part of *this will lie on the visible part of the earth
                        
                        s->resize(2);
                        
                        if ((t[0] == 0.0) && (t[1] == 0.0)) {
                            //*this is fully included into circle_observer and does not interscet with circle_observer: in this case, I draw the full circle of equal altitude *this
                            
                            ((*s)[0]).set(0.0, LengthUnit_types[0]);
                            ((*s)[1]).set(two_M_PI * (wxGetApp().Re.value) * sin(omega), LengthUnit_types[0]);
                            
                        }
                        else {
                            //*this intersects with circle_observer: I draw only a chunk of the circle of equal altitutde *this
                            
                            Length l1, l2;
                            
                            //here I decide whether the chunk with average t t[0]+t[1]/2 or the chunk with average t t[0]+t[1]/2+pi is the one included in circle_observer
                            //note that here doing the average as ((((t[0]).value)+((t[1]).value)))/2.0 and doing it as ((t[0]+t[1]).value)/2.0
                            compute_end(
                                        Length(
                                               ((Angle(mean_value(t[0], t[1]))).value) * ((wxGetApp().Re.value) * sin(omega))
                                               ),
                                        String(""));
                            draw_panel->circle_observer->reference_position->distance((*end), &l1, String(""), String(""));
                            
                            compute_end(
                                        Length(
                                               ((Angle(mean_value(t[0], t[1]) + M_PI)).value) * ((wxGetApp().Re.value) * sin(omega))
                                               ),
                                        String(""));
                            draw_panel->circle_observer->reference_position->distance((*end), &l2, String(""), String(""));
                            
                            if (l2 > l1) {
                                
                                ((*s)[0]).set(((t[0]).value) * ((wxGetApp().Re.value) * sin(omega)), LengthUnit_types[0]);
                                ((*s)[1]).set(((t[1]).value) * ((wxGetApp().Re.value) * sin(omega)), LengthUnit_types[0]);
                                
                            }else{
                                
                                ((*s)[0]).set(((t[1]).value) * ((wxGetApp().Re.value) * sin(omega)), LengthUnit_types[0]);
                                ((*s)[1]).set((two_M_PI + ((t[0]).value)) * ((wxGetApp().Re.value) * sin(omega)), LengthUnit_types[0]);
                                
                            }
                            
                        }
                        
                        t.clear();
                        
                        if (success != NULL) {
                            (*success) = true;
                        }
                        
                    }
                    else {
                        
                        if (success != NULL) {
                            (*success) = false;
                        }
                        
                    }
                    
                    break;
                    
                }
                    
            }
            
            break;
            
        }
            
    }
    
}


void Route::update_ListControl(long i, wxListCtrl* listcontrol) {

    unsigned int j;


    j = 0;
    //set the number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //set type column: I write the extended type names, not the short ones 'l', 'o' and 'c'
    if (type == (Route_types[0])) {
        listcontrol->SetItem(i, j++, wxString(((Route_types[0]).value)));
    }
    if (type == (Route_types[1])) {
        listcontrol->SetItem(i, j++, wxString(((Route_types[1]).value)));
    }
    if (type == (Route_types[2])) {
        listcontrol->SetItem(i, j++, wxString(((Route_types[2]).value)));
    }

    if (type == (Route_types[2])) {
        //in this case the type of this is 'circle of equal altitde': the first three fields are thus empty, and I fill in only the last two fields

        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));

        listcontrol->SetItem(i, j++, wxString(reference_position->to_string((display_precision.value))));
        listcontrol->SetItem(i, j++, wxString(omega.to_string(String(""), (display_precision.value), true)));

    }
    else {
        //in this case the type of this is 'loxodrome' or 'orthodrome': the last two fields are empty, and I fill in only the first three fields

        listcontrol->SetItem(i, j++, wxString(reference_position->to_string((display_precision.value))));
        listcontrol->SetItem(i, j++, wxString(Z.to_string(String(""), (display_precision.value), false)));
        
        set_length_from_time_speed();
        listcontrol->SetItem(i, j++, wxString(length->to_string((display_precision.value))));

        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));


    }

    listcontrol->SetItem(i, j++, wxString(label.value));


    if ((related_sight.value) == -1) {
        //if the route is not connected to a sight, I leave the column field empty

        listcontrol->SetItem(i, j++, wxString(""));

    }
    else {
        //if the route is connected to a sight, I write the # of the related sight in the column field

        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_sight.value) + 1));

    }


}



//Given the route (*this), this function returns the point on the Route which is closest to Position q, and writes this position and the corresponding value of t in p and tau, respectively.
bool Route::closest_point_to(Position* p, Angle* tau, Position q, [[maybe_unused]] String prefix) {

    String new_prefix;
    bool check;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    check = true;

    if (type == (Route_types[2])) {

        //these are the two values of the parametric angle t of the Route (*this), for which the distance between q and a point on (*this) vs. t has a maximum or a minimum
        Angle t_1, t_2;
        Position p_1, p_2;
        Length s_1, s_2;

        t_1.set(String(""),


            atan((cos((reference_position->lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position->phi) - cos(reference_position->phi) * sin(q.phi)) /
                sqrt(gsl_pow_int(cos((reference_position->lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position->phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position->lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position->phi) * cos((reference_position->lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position->phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position->phi), 2) * gsl_pow_int(sin(q.phi), 2)),
                (cos(q.phi) * sin((reference_position->lambda) - (q.lambda))) /
                sqrt(gsl_pow_int(cos((reference_position->lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position->phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position->lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position->phi) * cos((reference_position->lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position->phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position->phi), 2) * gsl_pow_int(sin(q.phi), 2)))

            , new_prefix);


        t_2.set(String(""),

            atan((-(cos((reference_position->lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position->phi)) + cos(reference_position->phi) * sin(q.phi)) /
                sqrt(gsl_pow_int(cos((reference_position->lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position->phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position->lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position->phi) * cos((reference_position->lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position->phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position->phi), 2) * gsl_pow_int(sin(q.phi), 2)),
                -((cos(q.phi) * sin((reference_position->lambda) - (q.lambda))) /
                    sqrt(gsl_pow_int(cos((reference_position->lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position->phi), 2) +
                        gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position->lambda) - (q.lambda)), 2) -
                        2 * cos(reference_position->phi) * cos((reference_position->lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position->phi) * sin(q.phi) +
                        gsl_pow_int(cos(reference_position->phi), 2) * gsl_pow_int(sin(q.phi), 2))))

            , new_prefix);

        //determine which one between the point on (*this) at t_1 and the one at t_2 is the one with minimum distance with respect to q, and store this point into (*p)
        compute_end(Length((t_1.value) * (wxGetApp().Re.value) * sin(omega)), new_prefix);
        p_1 = (*end);
        q.distance(p_1, &s_1, String("Distance with respect to p_1"), new_prefix);

        compute_end(Length((t_2.value) * (wxGetApp().Re.value) * sin(omega)), new_prefix);
        p_2 = (*end);
        q.distance(p_2, &s_2, String("Distance with respect to p_2"), new_prefix);

        if (s_2 > s_1) {
            (*p) = p_1;
            (*tau) = t_1;
        }
        else {
            (*p) = p_2;
            (*tau) = t_2;
        }


    }
    else {

        check &= false;

    }

    if (check) {

        p->print(String("Closest point"), prefix, cout);

    }
    else {

        cout << prefix.value << RED << "Closest point could not be computed!\n" << RESET;

    }

    return check;

}

//obtain the size of *this in the Mercator projection : consider the smallest rectangle that contains *this entirely, and say that this rectangle has, in the Mercator projection, bottom-left and top-right points (0,0) and *p, respectively -> compute the top-right point and write it in *p
void Route::size_Mercator(PositionProjection* p){
    
    PositionProjection q;
    Length length_saved;
    

    if(length_format == LengthFormat_types[0]){
        //length_format = LengthFormat_types[0] -> compute length from time and speed and have it in units LengthUnit_types[0] because this is the standard unit used to draw Routes
        
        set_length_from_time_speed();

    }else{
        //length_format = LengtFormat_types[1] -> save *length into length_saved and convert the unit of measure of *length to LengthUnit_types[0] because this is the standard unit used to draw Routes

        length_saved.set((*length));
        length->convert_to(LengthUnit_types[0]);

    }
    
    
    
    //in what follows, I store the two points representing the corners of the rectangle ennclosing *this in the Mercator projection in *p and q
    
    if(type == Route_types[0]){
        //*this is a loxodrome -> for loxodromes, latitude and longitude are either constantly increasing or decreasing along the Route -> I compute the points of maximal and minimal latitude / longitude from the extremal Positions on *this
        
        compute_end(String(""));
        p->NormalizeAndSetMercator((*end));
        q.NormalizeAndSetMercator((*reference_position));
 
        
    }else{
        //this is an orthodrome or a circle of equal altitude: latitude and longitude are not necessarily monotonic functions of the coordinate t along the curve-> compute them with lambda_min_max and phi_min_max
        
        Angle lambda_min, lambda_max, phi_min, phi_max;
        
        lambda_min_max(&lambda_min, &lambda_max, String(""));
        phi_min_max(&phi_min, &phi_max, String(""));
        
        q.NormalizeAndSetMercator(Position(lambda_min, phi_min));
        p->NormalizeAndSetMercator(Position(lambda_max, phi_max));
        
        
    }
    
    //I substract q to *p and store the absolute value of the result in p -> this is the size that *this occupies in the Mercator projection
    
    (*p) -= q;
    (p->x) = fabs(p->x);
    (p->y) = fabs(p->y);
    
    //write back length_saved into *length
    length->set(length_saved);
    
}


//If circle is not a circle of equal altitude, it returns -1 (error code). Otherwise, if the type of *this is not valid, it returns -1. Otherwise, the type of *this is valid-> if a part of *this is included into  circle, it returns 1, and 0 otherwise. If 1 is returned and write_t = true, it writes in t the value of the parametric angle of *this at which *this intersects circle and, if *this lies within circle and write_t = true, it sets t[0] = t[1] = 0.0. This method requires that Route::length is expressed in units LengthUnit_types[0]
int Route::inclusion(Route circle, bool write_t, vector<Angle>* t, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if (((circle.type) == (Route_types[2]))) {
        //circle is a circle of equal altittude

        if (type == Route_types[0]) {
            //*this is a loxodrome

            cout << prefix.value << RED << "Cannot determine whether *this is included in circle, because *this is a loxodrome!\n" << RESET;

            return -1;

        }
        else {
            //*this is either an orthodrome or a loxodrome

            if (type == Route_types[1]) {
                //*this is an orthodrome

                if (intersection(circle, true, t, new_prefix) == 0) {
                    //*this and circle do not intersect: check whether *this is fully included into circle

                    if (reference_position->is_in(circle, prefix)) {
                        //reference_position is included into the circle of circle, thus *this is included into circle

                        if (write_t) {
                            
                            set_length_from_time_speed();

                            t->resize(2);
                            ((*t)[0]).set(String(""), 0.0, new_prefix);
                            ((*t)[1]).set(String(""), (length->value) / (wxGetApp().Re.value), new_prefix);

                        }

                        return 1;

                    }
                    else {
                        //reference_position is not included into the circle of circle, thus *this is not included into circle

                        return 0;

                    }

                }
                else {
                    //*this and circle intersect ->  a part of *this is included into circle

                    if (write_t) {

                        switch (t->size()) {

                        case 1: {
                            //there is one intersection point

                            if (reference_position->is_in(circle, prefix)) {
                                //this->reference position is included into the circle of circle -> the part of *this comprised into circle is the one with 0 <= t <= (*t)[0]

                                t->insert(t->begin(), Angle(String(""), 0.0, new_prefix));

                            }
                            else {
                                //this->reference position is not included into the circle of circle -> this->end must be included into the circle of circle -> the part of *this comprised into circle is the one with  (*t)[0] <= t <= (l.value)/(wxGetApp().Re.value)

                                set_length_from_time_speed();
                                my_push_back(t, Angle(String(""), (length->value) / (wxGetApp().Re.value), new_prefix));

                            }

                            break;

                        }

                        case 2: {
                            //there are two intersection points -> the part of *this comprised into circle is the one with (*t)[0] < t <(*t)[1] -> all I need to do is sort t

                            sort(t->begin(), t->end());

                            compute_end(Length((wxGetApp().Re.value) * (((((*t)[0]).value) + (((*t)[1]).value)) / 2.0)), String(""));

                            if (!(end->is_in(circle, String("")))) {
                                //the midpoints on *this between t[0] and t[1] is not comprised into circle

                                //I add 0 and 2*M_PI to the vector t, so I create two chunks of the curve *this which are comprised into circle
                                my_push_back(t, Angle(0.0));
                                my_push_back(t, Angle(two_M_PI));
                                //                                    (t->back()).value = two_M_PI;

                                sort(t->begin(), t->end());

                            }


                            break;

                        }

                        }

                    }

                    return 1;

                }

            }
            else {

                if (type == Route_types[2]) {
                    //*this is a circle of equal altitude

                    Length d;

                    reference_position->distance((*(circle.reference_position)), &d, String(""), new_prefix);

                    if (d < ((wxGetApp().Re.value) * ((omega + (circle.omega)).value))) {
                        //the circles have a common area

                        if (write_t) {

                            if (intersection(circle, true, t, new_prefix) == 0) {
                                //the circles do no intersect: I set t[0] = t[1] = 0.0

                                t->resize(2);
                                ((*t)[0]).set(String(""), 0.0, new_prefix);
                                ((*t)[1]).set(String(""), 0.0, new_prefix);

                            }
                            else {
                                //the circles intersect: here you should compute t: method intersection called in the condition intersection(circle, true, t, new_prefix) == 0 has written in t the intersection angles



                            }

                        }

                        return 1;

                    }
                    else {
                        //the circles don't have a common area

                        return 0;

                    }

                }
                else {

                    cout << prefix.value << RED << "Cannot determine whether Route is included in circle, Route type is invalid!\n" << RESET;

                    return -1;

                }

            }

        }

    }
    else {

        cout << prefix.value << RED << "Cannot determine whether Route is included in circle, because circle is not a circle of equal altitude!\n" << RESET;

        return -1;

    }

}

//If *this is a loxodrome, return -1 because I don't know how to determine whetehr the loxodrome is included in a PositionRectangle. Otherwise, if *this is included into the PositionRectangle rectangle it returns 1, and 0 otherwise. If 1 is returned and write_t = true, it reallocates t and writes in t the value of the parametric angle of *this at which *this intersects rectangle and, if *this entirely lies within circle and write_t = true, it returns t[0] = t[1] = 0.0.  This method requires that Route::length is expressed in units LengthUnit_types[0]
int Route::inclusion(PositionRectangle rectangle, bool write_t, vector<Angle>* t, [[maybe_unused]] String prefix) {


    if (type == (Route_types[0])) {
        //*this is a loxodrome

        cout << prefix.value << RED << "Cannot determine whether *this is included in rectangle, because *this is a loxodrome!\n" << RESET;

        return -1;

    }else {
        //*this is an orthodrome or a circle of equal altitude

        Angle lambda_span, phi_span;
        Route side_N, side_S, side_E, side_W;
        vector<Angle> u, temp;
        bool  /*this is true if the entire Route *this is included in rectangle, and false otherwise*/is_fully_included;
        unsigned int i;
        int output;


        lambda_span = rectangle.p_NW->lambda.span(rectangle.p_SE->lambda);
        phi_span = rectangle.p_NW->phi.span(rectangle.p_SE->phi);

        //the parallel of latitude going through the North side of rectangle
        side_N = Route(
            RouteType(Route_types[2]),
            Position(Angle(0.0), Angle(GSL_SIGN((rectangle.p_NW->phi.normalize_pm_pi_ret()).value) * M_PI_2)),
            Angle(M_PI_2 - fabs(((rectangle.p_NW->phi.normalize_pm_pi_ret()).value)))
        );

        //the parallel of latitude going through the S side of rectangle
        side_S = Route(
            RouteType(Route_types[2]),
            Position(Angle(0.0), Angle(GSL_SIGN((rectangle.p_SE->phi.normalize_pm_pi_ret()).value) * M_PI_2)),
            Angle(M_PI_2 - fabs(((rectangle.p_SE->phi.normalize_pm_pi_ret()).value)))
        );

        //the meridian going through the W side of rectangle
        side_W = Route(
            RouteType(Route_types[2]),
            Position((rectangle.p_NW->lambda) + M_PI_2, Angle(0.0)),
            Angle(M_PI_2)
        );

        //the meridian going through the E side of rectangle
        side_E = Route(
            RouteType(Route_types[2]),
            Position((rectangle.p_SE->lambda) + M_PI_2, Angle(0.0)),
            Angle(M_PI_2)
        );


        //compute the intersections between *this and side_N/S/E/W, and writes in temp the values of the parametric angle t of *this at which *this crosses side_N/S/E/W: temps are then appended to u, which, at the end, will contain all intersections
        intersection(side_N, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());
        temp.clear();

        intersection(side_S, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());
        temp.clear();

        intersection(side_E, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());
        temp.clear();

        intersection(side_W, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());

        my_push_back(&u, Angle(0.0));


        //push back into u the angle which corresponds to the endpoint of Route *this
        if (type == (Route_types[1])) {
            set_length_from_time_speed();
            my_push_back(&u, Angle((length->value) / (wxGetApp().Re.value)));
        }

        //push back into u the angle which corresponds to the endpoint of Route *this
        if (type == (Route_types[2])) {
            my_push_back(&u, Angle(two_M_PI));
            (u.back()).value = two_M_PI;
        }

        sort(u.begin(), u.end());


        //run over all chunks of *this in between the intersections and find out whether some fall within rectangle
        if (write_t) { t->resize(0); }
        for (output = 0, is_fully_included = true, i = 0; i < (u.size()) - 1; i++) {

            //compute the midpoint between two subsequesnt intersections, and write it into this->end. I use u[(i+1) % (u.size())] in such a way that, when i = u.size() -1, this equals u[0], because the last chunk that I want to consider is the one between the last and the first intersection
            if (type == (Route_types[1])) {
                compute_end(Length((wxGetApp().Re.value) * (((u[i]).value) + ((u[i + 1]).value)) / 2.0), String(""));
            }
            if (type == (Route_types[2])) {
                compute_end(Length((wxGetApp().Re.value) * sin(omega) * (((u[i]).value) + ((u[i + 1]).value)) / 2.0), String(""));
            }

            if (rectangle.Contains((*end))) {
                //if rectangle contains the midpoint, then the chunk of *this with u[i] < t < u[1+1] is included in rectangle -> I return 1

                output = 1;

                //If write_t == true, write into t the value of the intersections which delimit the chunk of *this which is included in rectangle
                if (write_t) {

                    my_push_back(t, u[i]);
                    my_push_back(t, u[i + 1]);

                }

            }
            else {

                is_fully_included = false;

            }

        }

        if (write_t) {

            //I push back into t the last value of u, wich corresponds to the endpoint of *this  and which has not been pushed back by the loop above
            my_push_back(t, u.back());
            
  
            if ((type == (Route_types[2])) && is_fully_included && (t->size() == 2)) {
                //*this is  of type "circle fo equal altitude", its fully included in rectangle and it does not intersect rectangle

                //I set t[1].value = 0.0, so t[0].value = t[1].value = 0.0
                ((*t)[1]).normalize();

            }else{
                
                //delete duplicates from t
                delete_duplicates(t, &equal_rel_epsilon_double);
                
            }


            
            //            set<Angle> t_temp(t->begin(), t->end());
            //            t->assign(t_temp.begin(), t_temp.end());
            //
        }

        return output;

    }


}


//If route is not a circle of equal altitide,  returs -1 (error code). Othwewise, a) If *this and route intersect,  return 1 and, if write_t = true, it also allocates t and it writes in t the  values of the parametric angles t of (*this), at which (*this) crosses route. b) If *this and route do not intersect, it returns 0 and does nothing with t c) if the type of *this is such that the intersection cannot be computed, it returns -1
int Route::intersection(Route route, bool write_t, vector<Angle>* t, [[maybe_unused]] String prefix) {

    String new_prefix;
    Angle t_a, t_b;
    Length d;
    Double cos_ts;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if ((route.type) == (Route_types[2])) {
        //route is a circle of equal altitude

        if (type == (Route_types[1])) {
            //*this is an orthodrome -> I check whether route and *this intersect: I compute the minimal distance between a point on *this and the GP (reference position) of route. I do this by checking the distance at the two extrema (at the beginning and at the end of *this), and by looking for an extremum in the middle of *this

            vector<Length> s(2);

            //case 1: the starting point (or reference_position) of *this
            reference_position->distance((*(route.reference_position)), s.data(), String(""), prefix);

            //case 2: the end point of *this
            compute_end(prefix);
            end->distance((*(route.reference_position)), (s.data()) + 1, String(""), prefix);

            cos_ts.set(String(""),
                (cos((reference_position->lambda) - (route.reference_position->lambda)) * cos((reference_position->phi)) * cos((route.reference_position->phi)) + sin((reference_position->phi)) * sin((route.reference_position->phi))) / sqrt(gsl_sf_pow_int(cos((route.reference_position->phi)) * sin(Z) * sin((reference_position->lambda) - (route.reference_position->lambda)) - cos(Z) * cos((reference_position->lambda) - (route.reference_position->lambda)) * cos((route.reference_position->phi)) * sin((reference_position->phi)) + cos(Z) * cos((reference_position->phi)) * sin((route.reference_position->phi)), 2) + gsl_sf_pow_int(cos((reference_position->lambda) - (route.reference_position->lambda)) * cos((reference_position->phi)) * cos((route.reference_position->phi)) + sin((reference_position->phi)) * sin((route.reference_position->phi)), 2)),
                prefix
            );

            //case 3:  extremum n. 1 in the middle of *this
            d.set(String(""), (wxGetApp().Re.value) * acos(cos_ts.value), prefix);
            if (compute_end(d, prefix)) {

                s.resize(s.size() + 1);
                end->distance((*(route.reference_position)), &(s.back()), String(""), prefix);

            }

            //case 4: extremum n. 2 in the middle of *this
            d.set(String(""), (wxGetApp().Re.value) * (M_PI - acos(cos_ts.value)), prefix);
            if (compute_end(d, prefix)) {

                s.resize(s.size() + 1);
                end->distance((*(route.reference_position)), &(s.back()), String(""), prefix);

            }

            //obtain the minimum distance across all cases, which may be 2, 3, or 4, and chekwhetehr it is smaller than (wxGetApp().Re.value) * apertur angle of route
            if ((*min_element(s.begin(), s.end())) < (wxGetApp().Re.value) * (route.omega.value)) {
                //in this case, *this and route intersect: I compute the values of the parametric angle t which parametrizes *this and at which the distance betweeen (point on *this at t) and (GP of route) is equal to (wxGetApp().Re.value)*(angular aperture of route)

                Double a, b, square_root, cos_t_p, cos_t_m;
                int output;


                a.set(String(""),
                    -(cos((reference_position->lambda) - (route.reference_position->lambda)) * cos((reference_position->phi)) * cos((route.reference_position->phi))) - sin((reference_position->phi)) * sin((route.reference_position->phi)),
                    prefix);

                b.set(String(""),
                    -(cos((route.reference_position->phi)) * sin(Z) * sin((reference_position->lambda) - (route.reference_position->lambda))) + cos(Z) * cos((reference_position->lambda) - (route.reference_position->lambda)) * cos((route.reference_position->phi)) * sin((reference_position->phi)) - cos(Z) * cos((reference_position->phi)) * sin((route.reference_position->phi)),
                    prefix);


                square_root.set(sqrt(gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2) - gsl_sf_pow_int(cos(route.omega), 2)));

                //these are the values of cos(t) such that the distance between this->end at t  and route.reference_position equals (wxGetApp().Re.value)*(route.omega), i.e., it is the value of cos(t) such that end(t) lies on route. There are two of them.
                cos_t_p.set(String(""), (-((a.value) * cos(route.omega)) + (square_root.value) * fabs((b.value))) / (gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2)), prefix);
                cos_t_m.set(String(""), (-((a.value) * cos(route.omega)) - (square_root.value) * fabs((b.value))) / (gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2)), prefix);

                //this will be the output of this function: it is set to false for starters
                output = 0;

                //clear up t because I will write in i in what follows
                if (write_t) { t->clear(); }

                if ((/*when I solve the equations a cos t + b * sqrt(1-(cos t)^2)  = - cos(route.omega), I manipulate the euqation and then square both sides, thus introducing spurious solutions. This condition allows me to check which one among the spurious solutions is valid. */-((a.value) * (cos_t_p.value) + cos(route.omega)) / (b.value) > 0.0) && compute_end(Length((wxGetApp().Re.value) * acos(cos_t_p)), prefix)) {

                    if (write_t) {
                        t->resize((t->size()) + 1);
                        (t->back()).set(String(""), acos(cos_t_p), prefix);
                    }

                    //if I find a viable instersection point, I set output to 1
                    output = 1;

                    if (compute_end(Length((wxGetApp().Re.value) * (two_M_PI - acos(cos_t_p))), prefix)) {

                        if (write_t) {
                            t->resize((t->size()) + 1);
                            (t->back()).set(String(""), two_M_PI - acos(cos_t_p), prefix);
                        }

                        //if I find a viable instersection point, I set output to true
                        output = 1;

                    }

                }

                if ((/*when I solve the equations a cos t + b * sqrt(1-(cos t)^2)  = - cos(route.omega), I manipulate the euqation and then square both sides, thus introducing spurious solutions. This condition allows me to check which one among the spurious solutions is valid. */-((a.value) * (cos_t_m.value) + cos(route.omega)) / (b.value) > 0.0) && compute_end(Length((wxGetApp().Re.value) * acos(cos_t_m)), prefix)) {

                    if (write_t) {
                        t->resize((t->size()) + 1);
                        (t->back()).set(String(""), acos(cos_t_m), prefix);
                    }

                    //if I find a viable instersection point, I set output to 1
                    output = 1;

                    if (compute_end(Length((wxGetApp().Re.value) * (two_M_PI - acos(cos_t_m))), prefix)) {

                        if (write_t) {
                            t->resize((t->size()) + 1);
                            (t->back()).set(String(""), two_M_PI - acos(cos_t_m), prefix);
                        }

                        //if I find a viable instersection point, I set output to 1
                        output = 1;

                    }

                }

                return output;

            }
            else {
                //in this case, *this and route do not intersect

                return 0;

            }

        }
        else {

            if (type == (Route_types[2])) {
                //*this is a circle of equal altitude -> I check check whetehr *this and route intersect

                reference_position->distance((*(route.reference_position)), &d, String(""), new_prefix);

                if (/*this is the condition that *this and route intersect*/(d > (wxGetApp().Re.value) * fabs((omega.value) - (route.omega.value))) && (d < (wxGetApp().Re.value) * ((omega + (route.omega)).value))) {
                    //in this case, *this and route intersect

                    if (write_t) {

                        t->resize(2);

                        if (((route.reference_position->phi) != M_PI_2) && ((route.reference_position->phi) != 3.0 * M_PI_2)) {
                            //theg general case where route.reference_position->phi != +-pi/2

                            t_a.value = atan((8 * cos(route.reference_position->phi) * ((cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * sin((reference_position->phi)) - cos((reference_position->phi)) * sin(route.reference_position->phi)) * (cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                abs(sin((reference_position->lambda) - (route.reference_position->lambda))) * cos(route.reference_position->phi) * sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) +
                                    2 * cos(route.omega) * cot(omega) * csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                    2 * cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                    gsl_sf_pow_int(cos((reference_position->phi)), 2) * (-(gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos((reference_position->lambda.value) - (route.reference_position->lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin(route.reference_position->phi), 2)) +
                                    gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) - 2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * sin(reference_position->lambda) * sin(route.reference_position->lambda) +
                                    2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * sin(reference_position->lambda) * sin(route.reference_position->lambda) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2) +
                                    gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2)))) /
                                (gsl_sf_pow_int(cos(route.reference_position->phi), 2) * (-6 + 2 * cos(2 * (reference_position->phi.value)) + 2 * cos(2 * (reference_position->lambda.value) - 2 * (route.reference_position->lambda.value)) + cos(2 * ((reference_position->phi.value) + (reference_position->lambda.value) - (route.reference_position->lambda.value))) + cos(2 * ((reference_position->phi.value) - (reference_position->lambda.value) + (route.reference_position->lambda.value)))) - 8 * gsl_sf_pow_int(cos((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                    4 * cos((reference_position->lambda) - (route.reference_position->lambda)) * sin(2 * (reference_position->phi.value)) * sin(2 * (route.reference_position->phi.value))), (8 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * (cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) *
                                        sin((reference_position->lambda) - (route.reference_position->lambda)) - 8 * abs(sin((reference_position->lambda.value) - (route.reference_position->lambda.value))) * cos(route.reference_position->phi) * csc((reference_position->lambda.value) - (route.reference_position->lambda.value)) * (cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * sin((reference_position->phi)) - cos((reference_position->phi)) * sin(route.reference_position->phi)) *
                                        sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) + 2 * cos(route.omega) * cot(omega) * csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi) -
                                            gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) + 2 * cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                            gsl_sf_pow_int(cos((reference_position->phi)), 2) * (-(gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos((reference_position->lambda.value) - (route.reference_position->lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin(route.reference_position->phi), 2)) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) -
                                            2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * sin(reference_position->lambda) * sin(route.reference_position->lambda) + 2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * sin(reference_position->lambda) * sin(route.reference_position->lambda) +
                                            gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2))) /
                                (gsl_sf_pow_int(cos(route.reference_position->phi), 2) * (-6 + 2 * cos(2 * (reference_position->phi.value)) + 2 * cos(2 * (reference_position->lambda.value) - 2 * (route.reference_position->lambda.value)) + cos(2 * ((reference_position->phi.value) + (reference_position->lambda.value) - (route.reference_position->lambda.value))) + cos(2 * ((reference_position->phi.value) - (reference_position->lambda.value) + (route.reference_position->lambda.value)))) - 8 * gsl_sf_pow_int(cos((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                    4 * cos((reference_position->lambda) - (route.reference_position->lambda)) * sin(2 * (reference_position->phi.value)) * sin(2 * (route.reference_position->phi.value))));


                            t_b.value = atan((-8 * cos(route.reference_position->phi) * ((-(cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * sin((reference_position->phi))) + cos((reference_position->phi)) * sin(route.reference_position->phi)) * (cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                abs(sin((reference_position->lambda) - (route.reference_position->lambda))) * cos(route.reference_position->phi) * sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) +
                                    2 * cos(route.omega) * cot(omega) * csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                    2 * cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                    gsl_sf_pow_int(cos((reference_position->phi)), 2) * (-(gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos((reference_position->lambda.value) - (route.reference_position->lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin(route.reference_position->phi), 2)) +
                                    gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) - 2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * sin(reference_position->lambda) * sin(route.reference_position->lambda) +
                                    2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * sin(reference_position->lambda) * sin(route.reference_position->lambda) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2) +
                                    gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2)))) /
                                (gsl_sf_pow_int(cos(route.reference_position->phi), 2) * (-6 + 2 * cos(2 * (reference_position->phi.value)) + 2 * cos(2 * (reference_position->lambda.value) - 2 * (route.reference_position->lambda.value)) + cos(2 * ((reference_position->phi.value) + (reference_position->lambda.value) - (route.reference_position->lambda.value))) + cos(2 * ((reference_position->phi.value) - (reference_position->lambda.value) + (route.reference_position->lambda.value)))) - 8 * gsl_sf_pow_int(cos((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                    4 * cos((reference_position->lambda) - (route.reference_position->lambda)) * sin(2 * (reference_position->phi.value)) * sin(2 * (route.reference_position->phi.value))), (8 * cos(route.reference_position->phi) * sin((reference_position->lambda) - (route.reference_position->lambda)) *
                                        (cos(route.reference_position->phi) * (cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                            abs(sin((reference_position->lambda) - (route.reference_position->lambda))) * gsl_sf_pow_int(csc((reference_position->lambda.value) - (route.reference_position->lambda.value)), 2) * (cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * sin((reference_position->phi)) - cos((reference_position->phi)) * sin(route.reference_position->phi)) *
                                            sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) +
                                                2 * cos(route.omega) * cot(omega) * csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                                2 * cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin((reference_position->phi)) * sin(route.reference_position->phi)) +
                                                gsl_sf_pow_int(cos((reference_position->phi)), 2) * (-(gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos((reference_position->lambda.value) - (route.reference_position->lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin(route.reference_position->phi), 2)) +
                                                gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(route.reference_position->lambda), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) - 2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * sin(reference_position->lambda) * sin(route.reference_position->lambda) +
                                                2 * gsl_sf_pow_int(cos(route.reference_position->phi), 2) * cos(reference_position->lambda) * cos(route.reference_position->lambda) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * sin(reference_position->lambda) * sin(route.reference_position->lambda) + gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(cos(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2) +
                                                gsl_sf_pow_int(cos(route.reference_position->phi), 2) * gsl_sf_pow_int(sin((reference_position->phi)), 2) * gsl_sf_pow_int(sin(reference_position->lambda), 2) * gsl_sf_pow_int(sin(route.reference_position->lambda), 2)))) /
                                (gsl_sf_pow_int(cos(route.reference_position->phi), 2) * (-6 + 2 * cos(2 * (reference_position->phi.value)) + 2 * cos(2 * (reference_position->lambda.value) - 2 * (route.reference_position->lambda.value)) + cos(2 * ((reference_position->phi.value) + (reference_position->lambda.value) - (route.reference_position->lambda.value))) + cos(2 * ((reference_position->phi.value) - (reference_position->lambda.value) + (route.reference_position->lambda.value)))) - 8 * gsl_sf_pow_int(cos((reference_position->phi)), 2) * gsl_sf_pow_int(sin(route.reference_position->phi), 2) +
                                    4 * cos((reference_position->lambda) - (route.reference_position->lambda)) * sin(2 * (reference_position->phi.value)) * sin(2 * (route.reference_position->phi.value))));

                        }
                        else {
                            //the special case where  route.reference_position->phi = +- pi/2

                            t_a.set(acos(-GSL_SIGN(((route.reference_position->phi).normalize_pm_pi_ret()).value) * (cos(route.omega) * csc(omega) * sec(reference_position->phi)) + cot(omega) * tan(reference_position->phi)));
                            t_b.set(-acos(-GSL_SIGN(((route.reference_position->phi).normalize_pm_pi_ret()).value) * (cos(route.omega) * csc(omega) * sec(reference_position->phi)) + cot(omega) * tan(reference_position->phi)));


                        }

                        //normalize t_a and t_b to put them in a proper form and then properly compare their values
                        t_a.normalize();
                        t_b.normalize();

                        //write t_a, t_b in t by sorting them in ascending order.
                        if (t_a < t_b) {

                            ((*t)[0]).set(String(""), (t_a.value), new_prefix);
                            ((*t)[1]).set(String(""), (t_b.value), new_prefix);

                        }
                        else {

                            ((*t)[0]).set(String(""), (t_b.value), new_prefix);
                            ((*t)[1]).set(String(""), (t_a.value), new_prefix);

                        }

                    }

                    return 1;

                }
                else {
                    //in this case, *this and route do not intersect

                    return 0;

                }

            }
            else {

                if (type == (Route_types[0])) {

                    cout << new_prefix.value << RED << "Route is a loxodrome, I cannot compute intersection for loxodromes!\n" << RESET;

                    return -1;

                }
                else {

                    cout << new_prefix.value << RED << "Route type is invalid, I cannot compute intersection!\n" << RESET;

                    return -1;

                }

            }

        }

    }
    else {
        //in this case, *this and route are not circles of equal altitude

        cout << prefix.value << "Route is not a circle of equal altitude: I can only compute intersections if *this is a circle of equal altitude!\n";

        return -1;

    }

}

//reads from file the content after 'Route = ...' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Route::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Route>(this, name, filename, mode, prefix);

}


//read *this from the stream *input_stream. Here search_entire_stream is provided as argument only to match the argument pattern of read_from_stream in other classes, and it is not used (this function reads the Route at the current position of *input_stream)
template<class S> void Route::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    String new_prefix;
    string line;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    type.read_from_stream<S>(String("type"), input_stream, false, new_prefix);

    line.clear();
    getline(*input_stream, line);


    if (type == Route_types[2]) {

        reference_position->read_from_stream<S>(String("reference position"), input_stream, false, new_prefix);
        omega.read_from_stream<S>(String("omega"), input_stream, false, new_prefix);
        
        length->set(two_M_PI * (wxGetApp().Re.value) * sin(omega), LengthUnit_types[0]);

    }else{

        reference_position->read_from_stream<S>(String("reference position"), input_stream, false, new_prefix);

        Z.read_from_stream<S>(String("starting heading"), input_stream, false, new_prefix);

        length_format.read_from_stream(String("length format"), input_stream, false, new_prefix);

        if (length_format == (LengthFormat_types[0])) {
            //read time and speed, and set l accordingly

            time.read_from_stream(String("time"), input_stream, false, new_prefix);
            speed->read_from_stream(String("speed"), input_stream, false, new_prefix);
            set_length_from_time_speed();

        }
        else {

            length->read_from_stream<S>(String("length"), input_stream, false, new_prefix);

        }


    }

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }


    //when a sight is read from file, it is not yet linked to any route, thus I set
    (related_sight.value) = -1;

}

template void Route::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);
template void Route::read_from_stream<basic_istringstream<char, char_traits<char>, allocator<char>>>(String, basic_istringstream<char, char_traits<char>, allocator<char>>*, bool, String);


//set the content (not the memory adresses of *this) equal to the content of x
void Route::set(const Route& x){
    
    type.set(x.type);
    label.set(x.label);
    temp_prefix.set(x.temp_prefix);
    length_format.set(x.length_format);
    
    reference_position->set((*(x.reference_position)));
    end->set((*(x.end)));
    
    Z.set(x.Z);
    omega.set(x.omega);
    
    length->set((*(x.length)));
    speed->set((*(x.speed)));
    
    time.set(x.time);
    
    related_sight.set(x.related_sight);
    
}


//this function computes the crossings between Route (*this) and Route route: it writes the two crossing points in p, and the cosing of the crossing angle in cos_crossing_angle. If the intersection cannot be computed it returns -1 (error code), othwerwise it returns 1 (0) if the Routes intersect (do not interesect).
int Route::crossing(Route route, vector<Position>* p, double* cos_crossing_angle, [[maybe_unused]] String prefix) {

    //these are the two lengths along Route (*this) which correspond to the two crossings with route
    String new_prefix;
    bool check;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    if (!((type == (Route_types[2])) && (route.type == (Route_types[2])))) {

        cout << prefix.value << "Routes are not circles of equal altitude: this code only computes intersects between circles of equal altitudes\n";
        return (-1);

    }
    else {

        Angle theta, t_temp;
        Length r, s;

        check = true;

        theta.set(String("angle between the two GPs"), acos(cos((reference_position->phi)) * cos(route.reference_position->phi) * cos((reference_position->lambda.value) - (route.reference_position->lambda.value)) + sin((reference_position->phi)) * sin(route.reference_position->phi)), prefix);

        if ((abs((omega.value) - (route.omega.value)) < (theta.value)) && ((theta.value) < (omega.value) + (route.omega.value))) {
            //in this case routes intersect

            //t contains the parametric angle of Route (*this) where (*this) crosses route
            //u contains the parametric angle of Route route where route crosses (*this)
            vector<Angle> t, u;

            cout << prefix.value << "Routes intersect\n";

            intersection(route, true, &t, new_prefix);
            route.intersection((*this), true, &u, new_prefix);

            compute_end(Length((wxGetApp().Re.value) * sin(omega.value) * ((t[0]).value)), new_prefix);
            (*p)[0] = (*end);
            ((*p)[0]).label.set(String(""), String("crossing"), prefix);

            compute_end(Length((wxGetApp().Re.value) * sin(omega.value) * ((t[1]).value)), new_prefix);
            (*p)[1] = (*end);
            ((*p)[1]).label.set(String(""), String("crossing"), prefix);

            route.compute_end(Length((wxGetApp().Re.value) * sin(route.omega.value) * ((u[0]).value)), prefix);

            check &= ((*p)[0]).distance((*(route.end)), &r, String(""), prefix);
            check &= ((*p)[1]).distance((*(route.end)), &s, String(""), prefix);

            if (check) {

                if (r > s) {

                    cout << new_prefix.value << "Exchanging ts!\n";

                    t_temp = u[0];
                    u[0] = u[1];
                    u[1] = t_temp;

                }

                compute_end(Length((wxGetApp().Re.value) * sin(omega) * ((t[0]).value)), prefix);
                end->print(String("position of intersection 1 for Route 1"), prefix, cout);

                route.compute_end(Length((wxGetApp().Re.value) * sin(route.omega.value) * ((u[0]).value)), prefix);
                route.end->print(String("position of intersection 1 for Route 2"), prefix, cout);

                (*cos_crossing_angle) = cos((reference_position->phi)) * cos(route.reference_position->phi) * sin(t[0]) * sin(u[0]) + (cos(t[0]) * sin(reference_position->lambda) - cos(reference_position->lambda) * sin((reference_position->phi)) * sin(t[0])) * (cos(u[0]) * sin(route.reference_position->lambda) - cos(route.reference_position->lambda) * sin(route.reference_position->phi) * sin(u[0])) +
                    (cos(reference_position->lambda) * cos(t[0]) + sin((reference_position->phi)) * sin(reference_position->lambda) * sin(t[0])) * (cos(route.reference_position->lambda) * cos(u[0]) + sin(route.reference_position->phi) * sin(route.reference_position->lambda) * sin(u[0]));

                cout << prefix.value << "cos(angle 1 between tangents) = " << (*cos_crossing_angle) << "\n";

                t.clear();
                u.clear();

            }

        }
        else {
            //routes do not intersect

            cout << prefix.value << "Routes do no intersect\n";
            check &= false;

        }

        if (check) { return 1; }
        else { return 0; }

    }


}




//set length according to time and speed if the lenght format for *this is  time * speed
void Route::set_length_from_time_speed(void){
    
    if(length_format == LengthFormat_types[0]){
        
        (*length) = Length(time, (*speed));
        
    }
    
}


//set length equal to l(t), where l(t) is the value of the curvilinear length corresponding to the parametric coordinate t
void Route::set_length_from_input(double t){
    
    switch (position_in_vector(type, Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            double C, eta;
        
            C = gsl_pow_2(cos(Z));
            eta = sqrt((1-sin((reference_position->phi)))/(1+sin((reference_position->phi))));

            //set the length format, the length unit and the value of the length from t
            length_format.set(LengthFormat_types[1]);
            
            if(fabs(C) > epsilon_double){
                //I am not in the special case where Z = pi/2 or 3 pi /2 (i.e., C = 0)
                
                double s;
                
                s = GSL_SIGN(cos(Z));
                length->set(s * 2.0*(wxGetApp().Re.value)/sqrt(C) *( atan(eta) - atan( eta * exp(- s * sqrt(C/(1.0-C)) * t ) ) ),
                           LengthUnit_types[0]);
                
            }else{
                //I am in the special case where Z = pi/2 or 3 pi /2 (i.e., C = 0) -> set the length by using the analytical limit C->0 for  expression of the length
                
                length->set(2.0*(wxGetApp().Re.value)*t*eta/(1.0+gsl_pow_2(eta)),
                           LengthUnit_types[0]);
                
            }
            
            break;
            
        }
            
        case 1:{
            //*this is an orthodrome
            
            
            break;
            }
            
        case 2:{
            //*this is a circle of equal altitude
            
            break;
            
        }
            
    }
    
    
}


//write into this->end the Position on the Route at length this->length (which needs to be correclty set before this method is called) along the Route from start.  This method requires that Route::length is expressed in units LengthUnit_types[0]
void Route::compute_end(String prefix) {
    
    //picks the first (and only) character in string type.value
    switch (position_in_vector(type, Route_types)) {
            
            
        case 0:{
            //loxodrome route
            
            //this is the +- sign appearing in \phi'(t)  = +- sqrt{C/(1-C)} cos(phi(t));
            int sigma, tau;
            double C, eta;
            Angle t;
            
            
            eta = sqrt((1.0 - sin(reference_position->phi.value)) / (1.0 + sin(reference_position->phi.value)));
            
            //tau = +-_{notes}
            if (((0.0 <= (Z.value)) && ((Z.value) < M_PI_2)) || ((3.0 * M_PI_2 <= (Z.value)) && ((Z.value) < two_M_PI))) { tau = +1; }
            else { tau = -1; }
            
            if ((0.0 <= (Z.value)) && ((Z.value) < M_PI)) { sigma = -1; }
            else { sigma = +1; }
            
            C = gsl_pow_2(cos(Z));
            
            /* cout << "sigma = " << sigma << "\n"; */
            /* cout << "tau = " << tau << "\n"; */
            /* cout << "C = " << C << "\n"; */
            
            if ((Z != M_PI_2) && (Z != 3.0*M_PI_2) && (Z != 0.0) && (Z != M_PI)) {
                //this is the general expression of t vs l for Z != {pi/2, 3 pi/2, 0, pi}
                
                (t.value) = -tau * sqrt((1.0 - C) / C)
                * log(1.0 / eta * tan(-tau * sqrt(C) * (length->value) / (2.0 * (wxGetApp().Re.value)) + atan(sqrt((1.0 - sin(reference_position->phi.value)) / (1.0 + sin(reference_position->phi.value))))));
                
                end->phi.set(String(""), asin(tanh(tau * sqrt(C / (1.0 - C)) * (t.value) + atanh(sin(reference_position->phi.value)))), prefix);
                end->lambda.set(String(""), (reference_position->lambda.value) + sigma * (t.value), prefix);
     
                
            }else{
                
                if((Z == M_PI_2) || (Z == 3.0*M_PI_2)){
        
                    //this is the limit of the expression above in the case Z -> pi/2 or Z-> 3 pi /2
                    (t.value) = (length->value) * (1.0 + gsl_pow_2(eta)) / (2.0 * (wxGetApp().Re.value) * eta);
                    
                    end->phi.set(String(""), asin(tanh(tau * sqrt(C / (1.0 - C)) * (t.value) + atanh(sin(reference_position->phi.value)))), prefix);
                    end->lambda.set(String(""), (reference_position->lambda.value) + sigma * (t.value), prefix);

                    
                }
                
                if((Z == 0) || (Z == M_PI)){
                    
                    //this is the limit of the expression above in the case Z -> 0 or Z = pi
                    
                    
                    
                    end->phi.set(String(""),
                                 (reference_position->phi.value) + (length->value)/(wxGetApp().Re.value) *
                                 (Z == 0.0 ? 1.0 : -1.0),
                                 prefix);
                    end->lambda.set(String(""), (reference_position->lambda.value), prefix);
                    
                }
                
            }
            
            break;
            
        }
            
        case 1:{
            
            //orthodrome route
            Angle t;
            
     
            t.set(String(""), (length->value) / (wxGetApp().Re.value), prefix);
            
            end->phi.set(String(""), asin(cos(Z) * cos(reference_position->phi) * sin(t) + cos(t) * sin(reference_position->phi)), prefix);
            end->lambda.set(String(""),
                             -atan(cos(t) * cos(reference_position->lambda) * cos(reference_position->phi) + sin(t) * (sin(Z) * sin(reference_position->lambda) - cos(Z) * cos(reference_position->lambda) * sin(reference_position->phi))
                                   ,
                                   (cos(reference_position->lambda) * sin(t) * sin(Z) + sin(reference_position->lambda) * (-cos(t) * cos(reference_position->phi) + cos(Z) * sin(t) * sin(reference_position->phi)))),
                             prefix);
            
            break;
            
        }
            
        case 2:{
            
            Angle t;
            //compute the parametric angle for the circle of equal altitude starting from the length l of the curve, omega  and the Earth's radius
            //R sin omega = r, r t = l, t = l / (R sin omega)
            
            //compute the length of *this from time and speed, if the length is stored in *this as a time * speed
            set_length_from_time_speed();
            t.set(String(""), (length->value) / ((wxGetApp().Re.value) * sin(omega)), prefix);
            
            
            end->phi.set(String(""), M_PI_2 - acos(cos((omega.value)) * sin(reference_position->phi) - cos(reference_position->phi) * cos((t.value)) * sin((omega.value))), prefix);
            
            end->lambda.set(String(""), -(atan((-sin(reference_position->lambda) * (cos(reference_position->phi) * cos((omega.value)) + cos((t.value)) * sin(reference_position->phi) * sin((omega.value))) + cos(reference_position->lambda) * sin((omega.value)) * sin((t.value))) / (cos(reference_position->phi) * cos(reference_position->lambda) * cos((omega.value)) + sin((omega.value)) * (cos(reference_position->lambda) * cos((t.value)) * sin(reference_position->phi) + sin(reference_position->lambda) * sin((t.value)))))), prefix);
            if (cos(reference_position->phi) * cos(reference_position->lambda) * cos((omega.value)) + sin((omega.value)) * (cos(reference_position->lambda) * cos((t.value)) * sin(reference_position->phi) + sin(reference_position->lambda) * sin((t.value))) <= 0.0) {
                (end->lambda) -= M_PI;
            }
            
            break;
            
        }
            
    }
    
    (end->label.value) = "";
    
}


//This is an overload of compute_end: if d <= (this->l), it writes into this->end the position on the Route at length d along the Route from start and it returns true. If d > (this->l), it returns false.  This method requires that Route::length and d are expressed in units LengthUnit_types[0]
bool Route::compute_end(Length d, [[maybe_unused]] String prefix) {
    
    set_length_from_time_speed();

    if ((type == (Route_types[2])) || (d <= (*length))) {

        Length l_saved;

        l_saved = (*length);
        (*length) = d;
        compute_end(prefix);
        (*length) = l_saved;

        return true;

    }
    else {

        //        cout << prefix.value << RED << "Length is larger than Route length!\n" << RESET;

        return false;

    }

}





void Route::print(String name, String prefix, ostream& ostr) {

    String s, new_prefix, new_new_prefix;

    //append \t to prefix and \t\t to new_new_prefix
    new_prefix = prefix.append(String("\t"));
    new_new_prefix = new_prefix.append(String("\t"));
    
    if ((name.value) != "") {
        
        ostr << prefix.value << name.value << ":\n";
        
        type.print(String("type"), true, new_prefix, ostr);
        
        if ((type == (Route_types[0])) || (type == (Route_types[1]))) {
            
            reference_position->print(String("start position"), new_prefix, ostr);
            Z.print(String("starting heading"), new_prefix, ostr);
            
            length_format.print(String("length format"), false, new_prefix, ostr);
            if (length_format == (LengthFormat_types[1])) {
                
                length->print(String("length"), new_new_prefix, ostr);
                
            }else {
                
                time.print(String("time"), new_new_prefix, ostr);
                speed->print(String("speed"), new_new_prefix, ostr);
                
            }
            
        }
        else {
            
            reference_position->print(String("ground position"), new_prefix, ostr);
            omega.print(String("aperture angle"), new_prefix, ostr);
            
        }
        
        label.print(String("label"), true, new_prefix, ostr);
        
        
        if ((related_sight.value) != -1) {
            
            cout << new_prefix.value << "Related sight # = " << (related_sight.value) + 1 << "\n";
            
        }
        
    }

}





//this function returns the longitude lambda of a circle of equal altitude (*this) - pi
double Route::lambda_minus_pi(double t, void* route) {

    Route* r = (Route*)route;
    String new_prefix;

    //append \t to prefix
    new_prefix = (r->temp_prefix.append(String("\t")));

    r->length->set((wxGetApp().Re.value) * sin((r->omega.value)) * t, LengthUnit_types[0]);
    r->compute_end(new_prefix);

    return((r->end->lambda.value) - M_PI);

}


//comppute the extremal (min and max) longitudes taken by the points lying on *this, and write them in *lambda_min and *lambda_max. This method requires that length is expressed in units LengthUnit_types[0]
void Route::lambda_min_max(Angle* lambda_min, Angle* lambda_max, [[maybe_unused]] String prefix) {
    
    String new_prefix;
    Angle t_min, t_max, temp;
    Position p_min, p_max;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    if (type == (Route_types[2])) {
        //*this is a circle of equal altitude
        
        if (abs(-tan(reference_position->phi.value) * tan((omega.value))) < 1.0) {
            //im this case ( abs(-tan(reference_position->phi.value)*tan((omega.value))) < 1.0) there exists a value of t = t_{max} (t_{min}) such that reference_position.lambda vs. t has a maximum (minimum). In this case, I proceed and compute this maximum and minimum, and write reference_position.lambda_{t = t_{min}} and reference_position.lambda_{t = t_{max}}] in lambda_min, lambda_max
            
            //compute the values of the parametric Angle t, t_min and t_max, which yield the position with the largest and smallest longitude (p_max and p_min) on the circle of equal altitude
            t_max.set(String(""), acos(-tan(reference_position->phi.value) * tan((omega.value))), new_prefix);
            t_min.set(String(""), two_M_PI - acos(-tan(reference_position->phi.value) * tan((omega.value))), new_prefix);
            
            //p_max =  Position on the circle of equal altitude  at t = t_max
            length->set((wxGetApp().Re.value) * sin((omega.value)) * (t_max.value), LengthUnit_types[0]);
            compute_end(new_prefix);
            p_max = (*end);
            
            //p_min =  Position on circle of equal altitude  at t = t_min
            length->set((wxGetApp().Re.value) * sin((omega.value)) * (t_min.value), LengthUnit_types[0]);
            compute_end(new_prefix);
            p_min = (*end);
            
            //set lambda_min/max in this order, which is eventually rectified at the end of this function
            (*lambda_min) = (p_min.lambda);
            (*lambda_max) = (p_max.lambda);
            
            
            /* p_max.print(String("p_max"), new_prefix, cout); */
            /* p_min.print(String("p_min"), new_prefix, cout); */
            
        }else {
            //in this case, reference_position.lambda vs. t has no minimum nor maximum: lambda_min/max are simly given by
            
            //set lambda_min/max in this order, meaning that *this spans all longitudes, from 0 to 2 pi
            lambda_min->set(0.0);
            lambda_max->set(0.0);
            
        }
        
        //sort lambda_min and lambda_max
        if ((*lambda_min) > (*lambda_max)) {
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
        //eventually swap lambda_min/max in such a way that lambda_min lies on the left and lambda_max lies on the right as seen from the observer's position looking at the earth's center
        if (((lambda_min->value) < M_PI) && ((lambda_max->value) > M_PI)) {
            
            if (!(((reference_position->lambda) < (*lambda_min)) || ((reference_position->lambda) > (*lambda_max)))) {
                
                temp = (*lambda_min);
                (*lambda_min) = (*lambda_max);
                (*lambda_max) = temp;
                
            }
            
        }else {
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
        
    }else {
        //*this is a loxodrome or an orthodrome: in this case, the longitude along the Route *this is either a constantly increasing or a constantly decreasing function of the parameteric coordiante t -> set the minimal / maximal longitudes as the longitudes of the start and end point of this (or vice-versa)
        
        (*lambda_min) = (reference_position->lambda);
        
        compute_end(String(""));
        (*lambda_max) = (end->lambda);
        
        if((*lambda_min) > (*lambda_max)){
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
    }
    
}


//comppute the extremal latitudes taken by the points lying on *this, if the type of *this allows it, and write them in *phi_min/max. If the extremal latitudes could not be computed, an error is printed, false is returned, and phi_min /max are not touched; otherwise the maximal and minimal latitude are stored into *phi_min/max, and true is returned. This method requires length.unit to be equal to LengthUnit_types[0]
bool Route::phi_min_max(Angle* phi_min, Angle* phi_max, [[maybe_unused]] String prefix) {

    String new_prefix;
    Angle temp;
    Position p_min, p_max;
    bool check = false;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    
    switch (position_in_vector(type, Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            cout << prefix.value << RED << "Route is a loxodrome: phi min/max cannot be computed  for a loxodrome!\n" << RESET;
            check = false;

            break;
            
        }
        
        case 1:{
            //*this is an orthodrome
            
            double ts;
            //the candidate latitudes for the max and min latitude will be stored in phi
            vector<Angle> phi;
            
            set_length_from_time_speed();
            
            phi.clear();
            
            ts = atan(sin(reference_position->phi), cos(Z)*cos(reference_position->phi));
            
            //inlude in phi the latitude of the starting point of *this
            compute_end(Length(0.0), String(""));
            my_push_back(&phi, end->phi.normalize_pm_pi_ret());
            
            
            //there are two potential stationary points for the latitude vs t: include in phi the first one, if it lies on *this
            if((0.0 <= (wxGetApp().Re.value)*ts) && ((*length) >= (wxGetApp().Re.value)*ts)){
                
                //                t.push_back(Angle(ts));
                
                compute_end(Length((wxGetApp().Re.value)*ts), String(""));
                my_push_back(&phi, end->phi.normalize_pm_pi_ret());
                
            }
            
            //there are two potential stationary points for the latitude vs t: include in phi the second one, if it lies on *this
            if((0.0 <= (wxGetApp().Re.value)*(ts+M_PI)) && ((*length) >= (wxGetApp().Re.value)*(ts+M_PI))){
                                
                compute_end(Length((wxGetApp().Re.value)*(ts+M_PI)), String(""));
                my_push_back(&phi, end->phi.normalize_pm_pi_ret());
                    
            }
            
            //*include in *phi the latitude of the endpoint of *this
            compute_end(String(""));
            my_push_back(&phi, end->phi.normalize_pm_pi_ret());
            
            
            //write the min/max element into *phi_min/max, respectively
            (*phi_min) = *min_element(
                                      phi.begin(),
                                      phi.end(),
                                      Angle::strictly_smaller_normalize_pm_pi_ret
                                      );
            
            (*phi_max) = *max_element(
                                      phi.begin(),
                                      phi.end(),
                                      Angle::strictly_smaller_normalize_pm_pi_ret
                                      );
            
            check = true;
            
            break;
            
        }
        
        case 2:{
            //*this is a circle of equal altitude
                
            length->set((wxGetApp().Re.value) * sin((omega.value)) * 0.0, LengthUnit_types[0]);
            compute_end(new_prefix);
            p_max = (*end);
            
            length->set((wxGetApp().Re.value) * sin((omega.value)) * M_PI, LengthUnit_types[0]);
            compute_end(new_prefix);
            p_min = (*end);
            
            //set lambda_min/max in this order, which is eventually rectified at the end of this function
            (*phi_min) = (p_min.phi);
            (*phi_max) = (p_max.phi);
            
            //sort phi_min and phi_max
            if ((*phi_min) > (*phi_max)) {
                
                temp = (*phi_min);
                (*phi_min) = (*phi_max);
                (*phi_max) = temp;
                
            }
            
            check = true;
            
            break;
            
        }
            
    }
    
    return check;
    
}


