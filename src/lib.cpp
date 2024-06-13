//REMOVE THIS WHEN ALL CLASSES WILL BE MOVED TO FILES
//#include "body_field.cpp"
//REMOVE THIS WHEN ALL CLASSES WILL BE MOVED TO FILES


//example of variadic function: it takes a non-variadic argument n and then an arbitrary number of arguments `...`

using namespace std;








//reads from file the color written after 'name = ' and writes it into this. If mode  = 'RW' ('R') it reads from a FileRW (FileR)
void Color::read_from_file(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    String new_prefix, s;
    size_t pos_end;
    int red, green, blue;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    s.read_from_file_to(name, filename, mode, new_prefix);

    //get rid of everything that comes before and at '(' at the beginnign of s
    pos_end = (s.value).find("(");
    s.set(s.value.substr(pos_end + 1).c_str());
    //look for the first ','

    pos_end = (s.value).find(",");

    //read red
    red = stoi(((s.value).substr(0, pos_end)).c_str());

    //get rid of the first ','
    s.set(s.value.substr(pos_end + 1).c_str());

    pos_end = (s.value).find(",");

    green = stoi((s.value).substr(0, pos_end).c_str());

    //get rid of the second ','
    s.set(s.value.substr(pos_end + 1).c_str());

    pos_end = (s.value).find(")");
    //get rid of '('
    blue = stoi((s.value).substr(0, pos_end + 1).c_str());

    (*this) = Color(red, green, blue);


}









Answer::Answer(void) {
    //this is the default constructor, sets value to the default value, 'n' char

    value = 'n';

}

Answer::Answer(char c, [[maybe_unused]] String prefix) {

    if ((c == 'y') || (c == 'n')) {

        value = c;

    }
    else {
        //if the entered value is not valid, set value to 'n' and prints out this info

        value = 'n';
        cout << prefix.value << RED << "Value of answer is not valid, setting it to 'n'!\n" << RESET;

    }

}


bool Answer::operator==(const Answer& s) {

    return((value == (s.value)));

}

bool Answer::operator!=(const Answer& s) {

    return(!((*this) == s));

}


bool Answer::set(String name, char c, [[maybe_unused]] String prefix) {

    bool check;

    check = true;
    check &= ((c == 'y') || (c == 'n'));

    if (check) {

        value = c;

        if (name != String("")) {

            cout << prefix.value << name.value << " = " << c << "\n";

        }

    }
    else {

        cout << prefix.value << RED << "Value of answer is not valid!\n" << RESET;

    }

    return check;

}
//reads *this from file whose path is filename, by looking through the entire file
void Answer::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Answer>(this, name, filename, mode, prefix);

}



template<class S> void Answer::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << "... \n" << RESET;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline(*input_stream, line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));


    }
    else {

        line.clear();
        getline(*input_stream, line);

    }



    pos = line.find(" = ");

    value = line[pos + 3];

    cout << prefix.value << YELLOW << "... done.\n" << RESET;


    print(name, prefix, cout);


}




//constructor of a Rotation instance
Rotation::Rotation(void) {

    //allocate and set the rotation matrix
    matrix = gsl_matrix_alloc(3, 3);

}

//sets the Euler angles and matrix of *this from the Euler angles a_in, b_in, c_in
void Rotation::set(Angle a_in, Angle b_in, Angle c_in) {

    a = a_in;
    b = b_in;
    c = c_in;

    gsl_matrix_set(matrix, 0, 0, cos(a) * cos(c) - cos(b) * sin(a) * sin(c));
    gsl_matrix_set(matrix, 0, 1, -(cos(c) * sin(a)) - cos(a) * cos(b) * sin(c));
    gsl_matrix_set(matrix, 0, 2, -(sin(b) * sin(c)));

    gsl_matrix_set(matrix, 1, 0, cos(b) * cos(c) * sin(a) + cos(a) * sin(c));
    gsl_matrix_set(matrix, 1, 1, cos(a) * cos(b) * cos(c) - sin(a) * sin(c));
    gsl_matrix_set(matrix, 1, 2, cos(c) * sin(b));

    gsl_matrix_set(matrix, 2, 0, -(sin(a) * sin(b)));
    gsl_matrix_set(matrix, 2, 1, -(cos(a) * sin(b)));
    gsl_matrix_set(matrix, 2, 2, cos(b));

}

//sets matrix and Euler angles of *this from the matrix m
void Rotation::set(gsl_matrix* m) {

    gsl_matrix_memcpy(matrix, m);

    //extract the Euler angles from the matrix m
    b = acos(gsl_matrix_get(m, 2, 2));
    a = atan(gsl_matrix_get(m, 2, 1) / (-sin(b)), gsl_matrix_get(m, 2, 0) / (-sin(b)));
    c = atan(gsl_matrix_get(m, 1, 2) / sin(b), gsl_matrix_get(m, 0, 2) / (-sin(b)));


}


//set the content of *this eqaul to the content of r by copying the content of r into the memory of *this : the memory adresses of *this and r will be left unchanged
inline void Rotation::set(const Rotation& r){
    
    a = (r.a);
    b = (r.b);
    c = (r.c);
    
    gsl_matrix_memcpy(matrix, r.matrix);
    
}


//constructor of a Rotation instance which sets the rotation matrix according to three Euler angles
Rotation::Rotation(Angle a_in, Angle b_in, Angle c_in) {

    //allocate and set the rotation matrix
    matrix = gsl_matrix_alloc(3, 3);

    set(a_in, b_in, c_in);

}

//constructor of a Rotation instance which sets the rotation matrix equal to the rotation which brings Position p to q
Rotation::Rotation(Position p, Position q) {

    if (p != q) {
        //if p!= q, I compute the rotation which brings Position p to Position q

        double cos_rotation_angle;
        Angle rotation_angle;
        Position rotation_axis;
        Cartesian r_p, r_q, omega;

        //transform p and q into cartesian cordinates and write them into r_p and r_q, respectively
        p.getCartesian(&r_p);
        q.getCartesian(&r_q);

        gsl_blas_ddot((r_p.r), (r_q.r), &cos_rotation_angle);
        rotation_angle.set(acos(cos_rotation_angle));


        my_cross((r_p.r), (r_q.r), &(omega.r));
        gsl_vector_scale((omega.r), 1.0 / fabs(sin(rotation_angle)));

        rotation_axis.setCartesian(String(""), omega, String(""));


        (*this) = (Rotation(
            Angle(String(""), 0.0, String("")),
            Angle(String(""), M_PI_2 - (((rotation_axis).phi).value), String("")),
            Angle(String(""), -((((rotation_axis).lambda).value) + M_PI_2), String(""))
        )
            * Rotation(
                Angle(String(""), (((rotation_axis).lambda).value) + M_PI_2, String("")),
                Angle(String(""), -(M_PI_2 - (((rotation_axis).phi).value)), String("")),
                rotation_angle
            ));

        //writes the Euler angles from matrix
        set(matrix);

    }
    else {
        //if start = end, I return the identity as rotation

        (*this) = (Rotation(
            Angle(String(""), 0.0, String("")),
            Angle(String(""), 0.0, String("")),
            Angle(String(""), 0.0, String(""))
        ));

    }

}



//returns the inverse of the rotation (*this)
Rotation Rotation::inverse(void) {

    Rotation t;
    Angle temp;

    //set the euler angles corredponding to the inverse rotation
    b = b * (-1.0);
    temp = c;
    c = a * (-1.0);
    a = temp * (-1.0);

    //transposes (inverts) this->matrix and copies the result into t.matrix
    gsl_matrix_transpose_memcpy(t.matrix, matrix);

    return t;

}


//composition of two rotations: this yields the rotation given by this . s
Rotation Rotation::operator *(const Rotation& s) {

    Rotation t;

//    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, matrix, s.matrix, 0.0, t.matrix);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 3, 3, 3, 1, matrix->data, 3, s.matrix->data, 3, 0, t.matrix->data, 3);


    //extract the Euler angles from the matrix t and write them into t
    t.set(t.matrix);

    //    (t.b) = acos(gsl_matrix_get(t.matrix, 2, 2));
    //    (t.a) = atan(gsl_matrix_get(t.matrix, 2, 1)/(-sin(t.b)), gsl_matrix_get(t.matrix, 2, 0)/(-sin(t.b)));
    //    (t.c) = atan(gsl_matrix_get(t.matrix, 1, 2)/sin(t.b), gsl_matrix_get(t.matrix, 0, 2)/(-sin(t.b)));
    //
    return t;

}


void Rotation::print(String name, String prefix, ostream& ostr) {

    unsigned int i, j;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    ostr << prefix.value << name.value << "\n" << prefix.value << "Euler angles : \n";


    a.print(String("a"), new_prefix, ostr);
    b.print(String("b"), new_prefix, ostr);
    c.print(String("c"), new_prefix, ostr);


    ostr << prefix.value << "matrix : \n";

    for (i = 0; i < 3; i++) {

        for (ostr << new_prefix.value, j = 0; j < 3; j++) {

            ostr << gsl_matrix_get(matrix, i, j) << "\t";

        }

        ostr << "\n";

    }

}


//reads the Rotation from file whose path is filename, by looking through the entire file
template<class S> void Rotation::read_from_stream(String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    Angle alpha, beta, gamma;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

    if (search_entire_stream) {


        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }

    alpha.read_from_stream<S>(String("alpha"), input_stream, false, new_prefix);
    beta.read_from_stream<S>(String("beta"), input_stream, false, new_prefix);
    gamma.read_from_stream<S>(String("gamma"), input_stream, false, new_prefix);

    set(alpha, beta, gamma);

}

//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Rotation::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Rotation>(this, name, filename, mode, prefix);

}





bool operator<(const Angle& x, const Angle& y) {

    return(((x.value) < (y.value)));

}

bool operator<(const Angle& x, const double& y) {

    return(((x.value) < y));

}






PositionRectangle::PositionRectangle(void) {


}


//constructor which constructs p_NW and p_SE from p_NW_in and p_SE_in. For this to work, p_NW_in must lie at the NW of p_SE_in
PositionRectangle::PositionRectangle(Position p_NW_in, Position p_SE_in, [[maybe_unused]] String prefix) {

    p_NW = p_NW_in;
    p_SE = p_SE_in;

}


//returns true/false if p is containted in *this
bool PositionRectangle::Contains(Position p) {

    bool check_lambda;

    if ((p_NW.lambda) > (p_SE.lambda)) {
        //'normal' configuration where *this does not encompass the anti-greenwich meridian

        check_lambda = (((p.lambda) < (p_NW.lambda)) && ((p.lambda) > (p_SE.lambda)));

    }
    else {
        //'non-normal' configuration where *this  encompasses the anti-greenwich meridian

        check_lambda = (((p.lambda) < (p_NW.lambda)) || ((p.lambda) > (p_SE.lambda)));

    }

    return(check_lambda &&
        (((p.phi).normalize_pm_pi_ret() < ((p_NW.phi).normalize_pm_pi_ret())) &&
            ((p.phi).normalize_pm_pi_ret() > ((p_SE.phi).normalize_pm_pi_ret()))));

}


//if p !=NULL , compute rectangle of *this in units of x * y of the Mercator projection and writes it into *p and returns true, otherwise return false
bool PositionRectangle::SizeMercator(PositionProjection* p){
        
    if(p){
        
        Position p_NE, p_SW;
        PositionProjection q_A, q_B;

        p_NE = Position(p_SE.lambda, p_NW.phi);
        p_SW = Position(p_NW.lambda, p_SE.phi);
        q_A.NormalizeAndSetMercator(p_NE);
        q_B.NormalizeAndSetMercator(p_SW);
        
        (*p) = q_A-q_B;
    
        return true;
        
    }else{
        
        return false;
    }
    
}


//construct a brand new Route object and thus sets its related sight to -1, because this Route is not related to any sight yet. length_format_t_v is set to false: as the Route is created, lengths are written in l rather than in t and v
Route::Route(void) {

    related_sight.set(-1);
    length_format.set((LengthFormat_types[1]));

}

//constructs a brand new Route. where lengths are written in time * speed than in l. The route type is either ((Route_types[0]).value) or ((Route_types[1]).value) and thus set its related sight to -1, because this Route cannot be related to any sight.
Route::Route(RouteType type_in, Position reference_position_in, Angle Z_in, Chrono time_in, Speed speed_in) {
    
    length_format.set((LengthFormat_types[0]));
    type = type_in;
    
    time = time_in;
    speed = speed_in;
    set_length_from_time_speed();
    
    reference_position = reference_position_in;
    Z = Z_in;
    related_sight.set(-1);

}


//constructs a brand new Route. where lengths are written as a simple length rather than as time * speed. The route type is either ((Route_types[0]).value) or ((Route_types[1]).value) and thus set its related sight to -1, because this Route cannot be related to any sight.
Route::Route(RouteType type_in, Position reference_position_in, Angle Z_in, Length l_in) {

    length_format.set((LengthFormat_types[1]));
    type = type_in;
    
    length = l_in;
    length.convert_to(LengthUnit_types[0]);
    
    reference_position = reference_position_in;
    Z = Z_in;
    related_sight.set(-1);

}


//build a Route of type type that connects position_start and position_end
Route::Route(const RouteType& type_in,  Position p_start,  Position p_end){
    
    type = type_in;
    
    reference_position = p_start;
    (length_format.value) = ((LengthFormat_types[1]).value);
    
    switch (type.position_in_list(Route_types)) {
            
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
            
            //set length according to t* (see notes)
            set_length_from_input(fabs((p_end.lambda.value) - (p_start.lambda.value)));
            
            
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
            
            reference_position = p_start;
            
            
            //set the legnth as the length of the shortest great circle joining p_start and p_end
            phi.set(acos(r_start.dot(r_end)));
            
            length.set(Re*(phi.value), LengthUnit_types[0]);
            
            //set the tentative solution for the azimuth angle z: Z may be either z  (solkution 1) or -z (solution 2), I will pick the correct solution later
            z.set(String(""),
                  acos(-csc(phi) * sec(p_start.phi) * (cos(phi) * sin(p_start.phi) - sin(p_end.phi)) ),
                  String(""));

            //consider solution 1, compute end with this solution and store it in end_1
            Z.set(z.value);
            compute_end(String());
            end_1 = end;
            
            //consider solution 2, compute end with this solution and store it in end_2
            Z.set(-z.value);
            compute_end(String());
            end_2 = end;
            
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

    type = type_in;
    reference_position = reference_position_in;
    omega = omega_in;

    length_format.set((LengthFormat_types[1]));
    
    //the lenght of the circle of equal altitude is set by default
    length.set(2.0 * M_PI * Re * sin(omega), LengthUnit_types[0]);

    related_sight.set(-1);

}

//add the Route this to the wxListCtrl listcontrol
void Route::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

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

    update_wxListCtrl(i, listcontrol);

}


inline void Route::DrawOld(unsigned int n_points, DrawPanel* draw_panel, vector< vector<wxPoint> >* v, [[maybe_unused]] String prefix) {

    wxPoint p;
    bool end_connected;
    unsigned int i;

    
    set_length_from_time_speed();

    //tabulate the Route points
    for (/*this is true if at the preceeding step in the loop over i, I encountered a point which does not lie in the visible side of the chart, and thus terminated a connectd component of dummy_route*/v->clear(), end_connected = true, i = 0; i < n_points; i++) {

        compute_end(length*((double)i)/((double)(n_points - 1)), String(""));
        
        //treat the first and last point as a special one because it may be at the boundary of rectangle_observer-> check if they are and, if they are, put them back into rectangle_observer
        if((i==0) || (i==n_points-1)){
            end.put_back_in(draw_panel);
        }

        if ((draw_panel->GeoToDrawPanel)(end, &p, false)) {

            if (end_connected) {

                v->resize(v->size() + 1);
                end_connected = false;

            }

            ((*v)[v->size() - 1]).push_back(p);

        }
        else {

            end_connected = true;

        }

    }

}


//draws into draw_panel the Route this, by tabulating the Route with n points and connecting them with an spline. The route is drawn with color 'color' and width 'width'. If width = -1, then the Route is drawn with default width
inline void Route::DrawOld(unsigned int n_points, Color color, int width, wxDC* dc, DrawPanel* draw_panel) {

    vector< vector<wxPoint> > p;
    wxPoint temp;
    bool end_connected;
    unsigned int i;
    Length s;

    //sets color and width of memory_dc to the ones supported as arguments of PreRender
    dc->SetPen(wxPen(color, width));
    dc->SetBrush(wxBrush(wxGetApp().background_color, wxBRUSHSTYLE_TRANSPARENT));

    set_length_from_time_speed();

    //tabulate the Route points
    for (/*this is true if at the preceeding step in the loop over i, I encountered a point which does not lie in the visible side of the sphere, and thus terminated a connectd component of dummy_route*/end_connected = true, i = 0; i < n_points; i++) {

        //handle special cases i=0 and i = n_points-1 to avoind roundoff error
        if ((i > 0) && (i < n_points - 1)) {

            s = (length * ((double)i) / ((double)(n_points - 1)));

        }else{

            if (i == 0) {
                s.set(0.0, LengthUnit_types[0]);
            }else{
                s = length;
            }

        }

        compute_end(s, String(""));
        
        //treat the first and last point as a special one because it may be at the boundary of rectangle_observer-> check if they are and, if they are, put them back into rectangle_observer
        if((i==0) || (i==n_points-1)){
            end.put_back_in(draw_panel);
        }

        if ((draw_panel->GeoToDrawPanel)(end, &temp, false)) {

            if (end_connected) {

                p.resize(p.size() + 1);
                end_connected = false;

            }

            (p[p.size() - 1]).push_back(temp);

        }
        else {

            end_connected = true;

        }

    }

    //run all the connected components of the tabulated Route and draw each of them in draw_panel
    for (i = 0; i < p.size(); i++) {

        if (((p[i]).size()) > 1) {

            dc->DrawSpline((int)((p[i]).size()), (p[i]).data());

        }

    }

    //put back original parameters of memory_dc
    dc->SetPen(wxPen(wxGetApp().foreground_color, 1));

}

inline void Route::DrawOld(unsigned int n_points, wxDC* dc, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {

    int i;
    vector< vector<wxPoint> > v;

    DrawOld(n_points, draw_panel, &v, prefix);

    for (i = 0; i < (v.size()); i++) {
        //run over all chunks of the Route written in v
        if (((v[i]).size()) > 1) {
            //the i-th chunk v[i] has at least two points -> draw it
            dc->DrawSpline((int)((v[i]).size()), (v[i]).data());
        }
    }


}


//draws the Route *this into draw_panel, with any projection. n_points, color and width is the number of points, the line color and the width of the line used to draw *this, respectively
inline void Route::Draw(unsigned int n_points, Color foreground_color, Color background_color, int width, wxDC* dc, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {

    unsigned int i;
    vector<wxPoint> p;
    wxPoint temp, q;
    vector<Length> s;

    //sets color and width of memory_dc to the ones supported as arguments of PreRender
    dc->SetPen(wxPen(foreground_color, width));
    dc->SetBrush(wxBrush(background_color, wxBRUSHSTYLE_TRANSPARENT));

    //comoute the end values of l and writes them in s
    compute_l_ends(&s, NULL, draw_panel, prefix);

    if (type == (Route_types[0])) {
        //*this is a loxodrome

        cout << prefix.value << RED << "Cannot execute Draw: the Route is not an orthodrome nor a circle of equal altitude!\n" << RESET;

    }
    else {
        //*this is an orthodrome/circle of equal altitude

        //the following code holds for all projections

        bool compute_l_ends_ok;

        compute_l_ends(&s, &compute_l_ends_ok, draw_panel, prefix);

        if (compute_l_ends_ok) {
            //*this is included in the area of the chart visible by the observer

            unsigned int j;
            bool check;
            //a temporary length used to computed end
            Length r;

            for (j = 0; j < (s.size()) - 1; j++) {

                //clear up vectors where I am about to write
                p.clear();

                //tabulate the Route points
                for (i = 0; i < n_points; i++) {


                    //to avoid rounding problems, I set r exactly to s[j] (s[j+1]) for i=0 (i=n_points-2) and use the linear formula in between
                    if (i == 0) {
                        r = s[j];
                    }
                    else {
                        if (i < n_points - 1) {
//                            r = Length(((s[j]).value) + (((s[j + 1]) - (s[j])).value) * ((double)i) / ((double)(n_points - 1)));
                            r = s[j] + ((s[j + 1] - s[j])*((double)i)/((double)(n_points - 1)));
                        }
                        else {
                            r = s[j + 1];
                        }
                    }

                    compute_end(r, String(""));
                    
                    //treat the first and last point as a special one because it may be at the boundary of rectangle_observer-> check if they are and, if they are, put them back into rectangle_observer
                    if((i==0) || (i==n_points-1)){
                        end.put_back_in(draw_panel);
                    }

                    check = (draw_panel->GeoToDrawPanel)(end, &temp, true);

                    if(check){
                        //temp is a valid point

                        p.push_back(temp);

                    }else{
                        //temp is not a valid point

                        if (!(draw_panel->PutBackIn(temp, &q))) {
                            //temp does not fall in the plot area

                            p.push_back(temp);

                        }

                    }

                }

                if((p.size()) > 1){
                    dc->DrawSpline((int)(p.size()), p.data());
                }

                //free up memory
                p.clear();

            }

        }

    }

}


inline void Route::Draw(unsigned int n_points, wxDC* dc, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {

    int i;
    vector< vector<wxPoint> > v;

    Draw(n_points, draw_panel, &v, prefix);

    for (i = 0; i < (v.size()); i++) {
        //run over all chunks of the Route written in v
        if (((v[i]).size()) > 1) {
            //the i-th chunk v[i] has at least two points -> draw it
            dc->DrawSpline((int)((v[i]).size()), (v[i]).data());
        }
    }

}

//tabulate the points of Route *this in any projection of draw_panel and writes them into v
inline void Route::Draw(unsigned int n_points, DrawPanel* draw_panel, vector< vector<wxPoint> >* v, [[maybe_unused]] String prefix) {

    unsigned int i, j;
    wxPoint p;
    Angle lambda_a, lambda_b;
    vector<Length> s;
    bool compute_l_ends_ok;

    compute_l_ends(&s, &compute_l_ends_ok, draw_panel, prefix);
 
    //comoute the end values of l and writes them in s. If compute_l_ends returns true, than the endpoints have been computed correclty, and I can proceed
    if (compute_l_ends_ok) {
        
        bool check;
        //a vector where I will store the tentative points of each chunk of *this
        vector<wxPoint> v_tentative;
        //the number of points of each chunk for which GeoToDrawPanel returns true (without recurring to put_back_in)
        unsigned int n_points_check_ok;

         //run over all chunks of *this which are visible
         //given that s contains the number of intersection points of *this and that each pair of intersection point delimits a chunk, and that v contains the chunks, the size of v is equal to thte size of s minus one.
        for (j=0; j<(s.size()) - 1; j++) {
            //run over all chunks
 
            
            //tabulate the Route points of the jth chunk and store them in v_proposed
            for (v_tentative.clear(), n_points_check_ok=0, i = 0; i < n_points; i++) {

                //I slightly increase s[j] and slightly decrease s[j+1] (both by epsilon_double) in order to plot a chunk of the Route *this which is slightly smaller than the chunk [s[j], s[j+1]] and thus avoid  the odd lines that cross the whole plot area in the Mercator projection and that connect two points of the same chunk that are far from each other  on the plot area
                //                compute_end(Length(((s[j]).value) * (1.0 + epsilon_double) + (((s[j + 1]).value) * (1.0 - epsilon_double) - ((s[j]).value) * (1.0 + epsilon_double)) * ((double)i) / ((double)(n_points - 1))), String(""));
                compute_end(
                            (s[j] * (1.0 + epsilon_double)) + (((s[j + 1] * (1.0 - epsilon_double)) - ((s[j] * (1.0 + epsilon_double)))) * ((double)i)/((double)(n_points - 1))),
                            String(""));
                
                check = (draw_panel->GeoToDrawPanel)(end, &p, false);
                
                if (check) {
                    //end is a valid point -> convert it to a Position with GeoToDrawPanel

                    v_tentative.push_back(p);
                    n_points_check_ok++;

                }else{
                    //end is not a valid point
                    
                    if((i==0) || (i==n_points-1)){
                        //the non-valid point is the first or last point in the Route chunk -> the point may be non valid because it lies on the edge, i.e., because of a rounding error -> put it back in and, if the Position that has been put_back_in is valid, convert it to a Position with GeoToDrawPanel
                        
                        end.put_back_in(draw_panel);
                        
                        if((draw_panel->GeoToDrawPanel)(end, &p, false)){
                            
                            v_tentative.push_back(p);
                            
                        }

                    }else{
                        //the non-valid point lies in the middle of the Route chunk -> the reason why the point is non-valid cannot be a rounding error -> do not push the point to v_tentaive and break the for loop over i to terminate drawing the route chunk and switch to the next one
                        
                        break;
                        
                    }
     
                }
                
            }
            
            //now I decide if v_proposed is a valid chunk (a chunk to be plotted), and thus if I sholud push it back to v or not
            if(n_points_check_ok>0){
                //v_tentative containts at least one point for which GeoToDrawPanel evaluated to true (without recurring to put_back_in) -> it is a valid chunk -> I add it to v. On the other hand, if n_points_check_ok == 0, then the only points in v_tentative may be the first and the last, which have been pushed back to v_tentative by put_back_in, and the chunk will be an odd chunk with only two points put into rectangle_observer by put_back_in -> This may lead to odd diagonal lines in the Mercator projection 
                
                v->push_back(v_tentative);
                
            }

        }

    }else {

        //        cout << prefix.value << RED << "I could not compute ends of Route!\n" << RESET;

    }

}


//compute the values of the Length l for Route *this at which *this crosses draw_panel->circle/rectangle_observer, and writes them in *s. For (*s)[i] < l < (*s)[i+1], the Route *this lies within draw_panel -> circle/ draw_panel->rectangle_observer, and it is thus visible. If success != NULL, it writes true in *success if the values of the length above could be computed succesfully, and false otherwise.
inline void Route::compute_l_ends(vector<Length>* s, bool* success, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {
    
    vector<Angle> t;
    
    switch (type.position_in_list(Route_types)) {
            
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
            
            switch (position_in_vector(Projection((draw_panel->parent->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    check = inclusion(draw_panel->rectangle_observer, true, &t, String(""));
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    check = inclusion(draw_panel->circle_observer, true, &t, String(""));
                    
                    break;
                    
                }
                    
            }
            
            
            if(check == 1){
                //there is a part of *this which is included in circle/rectangle_observer -> some part of *this will lie on the visible part of the earth
                
                unsigned int i;
                                
                for (s->resize(t.size()), i = 0; i < (t.size()); i++) {
                    
                    ((*s)[i]).set(((t[i]).value) * Re);
                    
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
            
            switch (position_in_vector(Projection(draw_panel->parent->projection->name->GetValue().ToStdString()), Projection_types)) {
                    
                case 0: {
                    //I am using the Projection_types[0] projection
                    
                    if (inclusion(draw_panel->rectangle_observer, true, &t, String("")) == 1) {
                        //*this is included in rectangle_observer
                        
                        if ((t[0] == 0.0) && (t[1] == 0.0)) {
                            //*this is fully included into rectangle_observer and does not interscet with circle_observer: in this case, I draw the full circle of equal altitude *this
                            
                            s->resize(2);
                            ((*s)[0]).set(0.0, LengthUnit_types[0]);
                            ((*s)[1]).set(2.0 * M_PI * Re * sin(omega), LengthUnit_types[0]);
                            
                        }else{
                            
                            unsigned int i;
                            
                            //*this is partially included into rectangle_observer and it interscets rectangle_observer-> I write in s the values of the parametric length of *this at which these intersections occur
                            
                            for (s->resize(t.size()), i = 0; i < (t.size()); i++) {
                                
                                ((*s)[i]).set(((t[i]).value) * Re * sin(omega), LengthUnit_types[0]);
                                
                            }
                            
                        }
                        
                        if (success != NULL) {
                            (*success) = true;
                        }
                        
                    }
                    else {
                        //*this is not included in rectangle_observer
                        
                        if (success != NULL) {
                            (*success) = false;
                        }
                        
                    }
                    
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using the Projection_types[1] projection
                    
                    if (inclusion(draw_panel->circle_observer, true, &t, String("")) == 1) {
                        //there is a part of *this which is included in circle_observer -> some part of *this will lie on the visible part of the earth
                        
                        s->resize(2);
                        
                        if ((t[0] == 0.0) && (t[1] == 0.0)) {
                            //*this is fully included into circle_observer and does not interscet with circle_observer: in this case, I draw the full circle of equal altitude *this
                            
                            ((*s)[0]).set(0.0, LengthUnit_types[0]);
                            ((*s)[1]).set(2.0 * M_PI * Re * sin(omega), LengthUnit_types[0]);
                            
                        }
                        else {
                            //*this intersects with circle_observer: I draw only a chunk of the circle of equal altitutde *this
                            
                            Length l1, l2;
                            
                            //here I decide whether the chunk with average t t[0]+t[1]/2 or the chunk with average t t[0]+t[1]/2+pi is the one included in circle_observer
                            //note that here doing the average as ((((t[0]).value)+((t[1]).value)))/2.0 and doing it as ((t[0]+t[1]).value)/2.0
                            compute_end(
                                        Length(
                                               ((Angle(mean_value(t[0], t[1]))).value) * (Re * sin(omega))
                                               ),
                                        String(""));
                            draw_panel->circle_observer.reference_position.distance(end, &l1, String(""), String(""));
                            
                            compute_end(
                                        Length(
                                               ((Angle(mean_value(t[0], t[1]) + M_PI)).value) * (Re * sin(omega))
                                               ),
                                        String(""));
                            draw_panel->circle_observer.reference_position.distance(end, &l2, String(""), String(""));
                            
                            if (l2 > l1) {
                                
                                ((*s)[0]).set(((t[0]).value) * (Re * sin(omega)), LengthUnit_types[0]);
                                ((*s)[1]).set(((t[1]).value) * (Re * sin(omega)), LengthUnit_types[0]);
                                
                            }else{
                                
                                ((*s)[0]).set(((t[1]).value) * (Re * sin(omega)), LengthUnit_types[0]);
                                ((*s)[1]).set((2.0 * M_PI + ((t[0]).value)) * (Re * sin(omega)), LengthUnit_types[0]);
                                
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


void Route::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

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

        listcontrol->SetItem(i, j++, wxString(reference_position.to_string((display_precision.value))));
        listcontrol->SetItem(i, j++, wxString(omega.to_string(String(""), (display_precision.value), true)));

    }
    else {
        //in this case the type of this is 'loxodrome' or 'orthodrome': the last two fields are empty, and I fill in only the first three fields

        listcontrol->SetItem(i, j++, wxString(reference_position.to_string((display_precision.value))));
        listcontrol->SetItem(i, j++, wxString(Z.to_string(String(""), (display_precision.value), false)));
        
        set_length_from_time_speed();
        listcontrol->SetItem(i, j++, wxString(length.to_string(LengthUnit_types[0], (display_precision.value))));

        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));


    }

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol

    listcontrol->SetItem(i, j++, wxString(label.value));

    //    }else{
    //        //the label in *this is empty -> I write as label in listcontrol the current date and time
    //
    //        Time now;
    //
    //        now.set_current(String(""));
    //
    //        label.set(String("Label replacing empty label"), now.to_string(display_precision.value), String(""));
    //        listcontrol->SetItem(i, j++, wxString(now.to_string(display_precision.value)));
    //
    //    }


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


            atan((cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) - cos(reference_position.phi) * sin(q.phi)) /
                sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2)),
                (cos(q.phi) * sin((reference_position.lambda) - (q.lambda))) /
                sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2)))

            , new_prefix);


        t_2.set(String(""),

            atan((-(cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi)) + cos(reference_position.phi) * sin(q.phi)) /
                sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2)),
                -((cos(q.phi) * sin((reference_position.lambda) - (q.lambda))) /
                    sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                        gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                        2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                        gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2))))

            , new_prefix);

        //determine which one between the point on (*this) at t_1 and the one at t_2 is the one with minimum distance with respect to q, and store this point into (*p)
        compute_end(Length((t_1.value) * Re * sin(omega)), new_prefix);
        p_1 = end;
        q.distance(p_1, &s_1, String("Distance with respect to p_1"), new_prefix);

        compute_end(Length((t_2.value) * Re * sin(omega)), new_prefix);
        p_2 = end;
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

        (*p).print(String("Closest point"), prefix, cout);

    }
    else {

        cout << prefix.value << RED << "Closest point could not be computed!\n" << RESET;

    }

    return check;

}

//obtain the size of *this in the Mercator projection : consider the smallest rectangle that contains *this entirely, and say that this rectangle has, in the Mercator projection, bottom-left and top-right points (0,0) and *p, respectively -> compute the top-right point and write it in *p
void Route::size_Mercator(PositionProjection* p){
    
    PositionProjection q;
    

    //if the length of *this is expresed as time x speed, compute length from time and speed, otherwise the length of *this is already written in then and there is nothing to do
    set_length_from_time_speed();
    
    //in what follows, I store the two points representing the corners of the rectangle ennclosing *this in the Mercator projection in *p and q
    
    if(type == Route_types[0]){
        //*this is a loxodrome -> for loxodromes, latitude and longitude are either constantly increasing or decreasing along the Route -> I compute the points of maximal and minimal latitude / longitude from the extremal Positions on *this
        
        compute_end(String(""));
        p->NormalizeAndSetMercator(end);
        q.NormalizeAndSetMercator(reference_position);
 
        
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

                    if (reference_position.is_in(circle, prefix)) {
                        //reference_position is included into the circle of circle, thus *this is included into circle

                        if (write_t) {
                            
                            set_length_from_time_speed();

                            t->resize(2);
                            ((*t)[0]).set(String(""), 0.0, new_prefix);
                            ((*t)[1]).set(String(""), (length.value) / Re, new_prefix);

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

                            if (reference_position.is_in(circle, prefix)) {
                                //this->reference position is included into the circle of circle -> the part of *this comprised into circle is the one with 0 <= t <= (*t)[0]

                                t->insert(t->begin(), Angle(String(""), 0.0, new_prefix));

                            }
                            else {
                                //this->reference position is not included into the circle of circle -> this->end must be included into the circle of circle -> the part of *this comprised into circle is the one with  (*t)[0] <= t <= (l.value)/Re

                                set_length_from_time_speed();
                                t->push_back(Angle(String(""), (length.value) / Re, new_prefix));

                            }

                            break;

                        }

                        case 2: {
                            //there are two intersection points -> the part of *this comprised into circle is the one with (*t)[0] < t <(*t)[1] -> all I need to do is sort t

                            sort(t->begin(), t->end());

                            compute_end(Length(Re * (((((*t)[0]).value) + (((*t)[1]).value)) / 2.0)), String(""));

                            if (!(end.is_in(circle, String("")))) {
                                //the midpoints on *this between t[0] and t[1] is not comprised into circle

                                //I add 0 and 2*M_PI to the vector t, so I create two chunks of the curve *this which are comprised into circle
                                t->push_back(Angle(0.0));
                                t->push_back(Angle(2.0 * M_PI));
                                //                                    (t->back()).value = 2.0*M_PI;

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

                    reference_position.distance(circle.reference_position, &d, String(""), new_prefix);

                    if (d < (Re * ((omega + (circle.omega)).value))) {
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


        lambda_span = ((rectangle.p_NW).lambda).span((rectangle.p_SE).lambda);
        phi_span = ((rectangle.p_NW).phi).span((rectangle.p_SE).phi);

        //the parallel of latitude going through the North side of rectangle
        side_N = Route(
            RouteType(Route_types[2]),
            Position(Angle(0.0), Angle(GSL_SIGN((((rectangle.p_NW).phi).normalize_pm_pi_ret()).value) * M_PI_2)),
            Angle(M_PI_2 - fabs(((((rectangle.p_NW).phi).normalize_pm_pi_ret()).value)))
        );

        //the parallel of latitude going through the S side of rectangle
        side_S = Route(
            RouteType(Route_types[2]),
            Position(Angle(0.0), Angle(GSL_SIGN((((rectangle.p_SE).phi).normalize_pm_pi_ret()).value) * M_PI_2)),
            Angle(M_PI_2 - fabs(((((rectangle.p_SE).phi).normalize_pm_pi_ret()).value)))
        );

        //the meridian going through the W side of rectangle
        side_W = Route(
            RouteType(Route_types[2]),
            Position(((rectangle.p_NW).lambda) + M_PI_2, Angle(0.0)),
            Angle(M_PI_2)
        );

        //the meridian going through the E side of rectangle
        side_E = Route(
            RouteType(Route_types[2]),
            Position(((rectangle.p_SE).lambda) + M_PI_2, Angle(0.0)),
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

        u.push_back(Angle(0.0));


        //push back into u the angle which corresponds to the endpoint of Route *this
        if (type == (Route_types[1])) {
            set_length_from_time_speed();
            u.push_back(Angle((length.value) / Re));
        }

        //push back into u the angle which corresponds to the endpoint of Route *this
        if (type == (Route_types[2])) {
            u.push_back(Angle(2.0 * M_PI));
            (u.back()).value = 2.0 * M_PI;
        }

        sort(u.begin(), u.end());


        //run over all chunks of *this in between the intersections and find out whether some fall within rectangle
        if (write_t) { t->resize(0); }
        for (output = 0, is_fully_included = true, i = 0; i < (u.size()) - 1; i++) {

            //compute the midpoint between two subsequesnt intersections, and write it into this->end. I use u[(i+1) % (u.size())] in such a way that, when i = u.size() -1, this equals u[0], because the last chunk that I want to consider is the one between the last and the first intersection
            if (type == (Route_types[1])) {
                compute_end(Length(Re * (((u[i]).value) + ((u[i + 1]).value)) / 2.0), String(""));
            }
            if (type == (Route_types[2])) {
                compute_end(Length(Re * sin(omega) * (((u[i]).value) + ((u[i + 1]).value)) / 2.0), String(""));
            }

            if (rectangle.Contains(end)) {
                //if rectangle contains the midpoint, then the chunk of *this with u[i] < t < u[1+1] is included in rectangle -> I return 1

                output = 1;

                //If write_t == true, write into t the value of the intersections which delimit the chunk of *this which is included in rectangle
                if (write_t) {

                    t->push_back(u[i]);
                    t->push_back(u[i + 1]);

                }

            }
            else {

                is_fully_included = false;

            }

        }

        if (write_t) {

            //I push back into t the last value of u, wich corresponds to the endpoint of *this  and which has not been pushed back by the loop above
            t->push_back(u.back());
            
  
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
            reference_position.distance(route.reference_position, s.data(), String(""), prefix);

            //case 2: the end point of *this
            compute_end(prefix);
            end.distance(route.reference_position, (s.data()) + 1, String(""), prefix);

            cos_ts.set(String(""),
                (cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos((reference_position.phi)) * cos(((route.reference_position).phi)) + sin((reference_position.phi)) * sin(((route.reference_position).phi))) / sqrt(gsl_sf_pow_int(cos(((route.reference_position).phi)) * sin(Z) * sin((reference_position.lambda) - ((route.reference_position).lambda)) - cos(Z) * cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos(((route.reference_position).phi)) * sin((reference_position.phi)) + cos(Z) * cos((reference_position.phi)) * sin(((route.reference_position).phi)), 2) + gsl_sf_pow_int(cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos((reference_position.phi)) * cos(((route.reference_position).phi)) + sin((reference_position.phi)) * sin(((route.reference_position).phi)), 2)),
                prefix
            );

            //case 3:  extremum n. 1 in the middle of *this
            d.set(String(""), Re * acos(cos_ts.value), prefix);
            if (compute_end(d, prefix)) {

                s.resize(s.size() + 1);
                end.distance(route.reference_position, &(s.back()), String(""), prefix);

            }

            //case 4: extremum n. 2 in the middle of *this
            d.set(String(""), Re * (M_PI - acos(cos_ts.value)), prefix);
            if (compute_end(d, prefix)) {

                s.resize(s.size() + 1);
                end.distance(route.reference_position, &(s.back()), String(""), prefix);

            }

            //obtain the minimum distance across all cases, which may be 2, 3, or 4, and chekwhetehr it is smaller than Re * apertur angle of route
            if ((*min_element(s.begin(), s.end())) < Re * (route.omega.value)) {
                //in this case, *this and route intersect: I compute the values of the parametric angle t which parametrizes *this and at which the distance betweeen (point on *this at t) and (GP of route) is equal to Re*(angular aperture of route)

                Double a, b, square_root, cos_t_p, cos_t_m;
                int output;


                a.set(String(""),
                    -(cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos((reference_position.phi)) * cos(((route.reference_position).phi))) - sin((reference_position.phi)) * sin(((route.reference_position).phi)),
                    prefix);

                b.set(String(""),
                    -(cos(((route.reference_position).phi)) * sin(Z) * sin((reference_position.lambda) - ((route.reference_position).lambda))) + cos(Z) * cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos(((route.reference_position).phi)) * sin((reference_position.phi)) - cos(Z) * cos((reference_position.phi)) * sin(((route.reference_position).phi)),
                    prefix);


                square_root.set(sqrt(gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2) - gsl_sf_pow_int(cos(route.omega), 2)));

                //these are the values of cos(t) such that the distance between this->end at t  and route.reference_position equals Re*(route.omega), i.e., it is the value of cos(t) such that end(t) lies on route. There are two of them.
                cos_t_p.set(String(""), (-((a.value) * cos(route.omega)) + (square_root.value) * fabs((b.value))) / (gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2)), prefix);
                cos_t_m.set(String(""), (-((a.value) * cos(route.omega)) - (square_root.value) * fabs((b.value))) / (gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2)), prefix);

                //this will be the output of this function: it is set to false for starters
                output = 0;

                //clear up t because I will write in i in what follows
                if (write_t) { t->clear(); }

                if ((/*when I solve the equations a cos t + b * sqrt(1-(cos t)^2)  = - cos(route.omega), I manipulate the euqation and then square both sides, thus introducing spurious solutions. This condition allows me to check which one among the spurious solutions is valid. */-((a.value) * (cos_t_p.value) + cos(route.omega)) / (b.value) > 0.0) && compute_end(Length(Re * acos(cos_t_p)), prefix)) {

                    if (write_t) {
                        t->resize((t->size()) + 1);
                        (t->back()).set(String(""), acos(cos_t_p), prefix);
                    }

                    //if I find a viable instersection point, I set output to 1
                    output = 1;

                    if (compute_end(Length(Re * (2.0 * M_PI - acos(cos_t_p))), prefix)) {

                        if (write_t) {
                            t->resize((t->size()) + 1);
                            (t->back()).set(String(""), 2.0 * M_PI - acos(cos_t_p), prefix);
                        }

                        //if I find a viable instersection point, I set output to true
                        output = 1;

                    }

                }

                if ((/*when I solve the equations a cos t + b * sqrt(1-(cos t)^2)  = - cos(route.omega), I manipulate the euqation and then square both sides, thus introducing spurious solutions. This condition allows me to check which one among the spurious solutions is valid. */-((a.value) * (cos_t_m.value) + cos(route.omega)) / (b.value) > 0.0) && compute_end(Length(Re * acos(cos_t_m)), prefix)) {

                    if (write_t) {
                        t->resize((t->size()) + 1);
                        (t->back()).set(String(""), acos(cos_t_m), prefix);
                    }

                    //if I find a viable instersection point, I set output to 1
                    output = 1;

                    if (compute_end(Length(Re * (2.0 * M_PI - acos(cos_t_m))), prefix)) {

                        if (write_t) {
                            t->resize((t->size()) + 1);
                            (t->back()).set(String(""), 2.0 * M_PI - acos(cos_t_m), prefix);
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

                reference_position.distance(route.reference_position, &d, String(""), new_prefix);

                if (/*this is the condition that *this and route intersect*/(d > Re * fabs((omega.value) - (route.omega.value))) && (d < Re * ((omega + (route.omega)).value))) {
                    //in this case, *this and route intersect

                    if (write_t) {

                        t->resize(2);

                        if (((route.reference_position.phi) != M_PI_2) && ((route.reference_position.phi) != 3.0 * M_PI_2)) {
                            //theg general case where route.reference_position.phi != +-pi/2

                            t_a.value = atan((8 * cos((route.reference_position).phi) * ((cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi)) - cos(((reference_position).phi)) * sin((route.reference_position).phi)) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                abs(sin((reference_position.lambda) - (route.reference_position.lambda))) * cos((route.reference_position).phi) * sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) +
                                    2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                    gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) - 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                    2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2)))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))), (8 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) *
                                        sin((reference_position.lambda) - (route.reference_position.lambda)) - 8 * abs(sin((reference_position.lambda.value) - (route.reference_position.lambda.value))) * cos((route.reference_position).phi) * csc((reference_position.lambda.value) - (route.reference_position.lambda.value)) * (cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi)) - cos(((reference_position).phi)) * sin((route.reference_position).phi)) *
                                        sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) + 2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) -
                                            gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) + 2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                            gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) -
                                            2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                            gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))));


                            t_b.value = atan((-8 * cos((route.reference_position).phi) * ((-(cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi))) + cos(((reference_position).phi)) * sin((route.reference_position).phi)) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                abs(sin((reference_position.lambda) - (route.reference_position.lambda))) * cos((route.reference_position).phi) * sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) +
                                    2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                    gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) - 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                    2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2)))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))), (8 * cos((route.reference_position).phi) * sin((reference_position.lambda) - (route.reference_position.lambda)) *
                                        (cos((route.reference_position).phi) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                            abs(sin((reference_position.lambda) - (route.reference_position.lambda))) * gsl_sf_pow_int(csc((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * (cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi)) - cos(((reference_position).phi)) * sin((route.reference_position).phi)) *
                                            sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) +
                                                2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                                2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                                gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) +
                                                gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) - 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                                2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) +
                                                gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2)))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))));

                        }
                        else {
                            //the special case where  route.reference_position.phi = +- pi/2

                            t_a.set(acos(-GSL_SIGN((((route.reference_position).phi).normalize_pm_pi_ret()).value) * (cos(route.omega) * csc(omega) * sec(reference_position.phi)) + cot(omega) * tan(reference_position.phi)));
                            t_b.set(-acos(-GSL_SIGN((((route.reference_position).phi).normalize_pm_pi_ret()).value) * (cos(route.omega) * csc(omega) * sec(reference_position.phi)) + cot(omega) * tan(reference_position.phi)));


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

        reference_position.read_from_stream<S>(String("reference position"), input_stream, false, new_prefix);
        omega.read_from_stream<S>(String("omega"), input_stream, false, new_prefix);
        
        length.set(2.0 * M_PI * Re * sin(omega), LengthUnit_types[0]);

    }else{

        reference_position.read_from_stream<S>(String("reference position"), input_stream, false, new_prefix);

        Z.read_from_stream<S>(String("starting heading"), input_stream, false, new_prefix);

        length_format.read_from_stream(String("length format"), input_stream, false, new_prefix);

        if (length_format == (LengthFormat_types[0])) {
            //read time and speed, and set l accordingly

            time.read_from_stream(String("time"), input_stream, false, new_prefix);
            speed.read_from_stream(String("speed"), input_stream, false, new_prefix);
            set_length_from_time_speed();

        }
        else {

            length.read_from_stream<S>(String("length"), input_stream, false, new_prefix);

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

        theta.set(String("angle between the two GPs"), acos(cos((reference_position.phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) + sin((reference_position.phi)) * sin((route.reference_position).phi)), prefix);

        if ((abs((omega.value) - (route.omega.value)) < (theta.value)) && ((theta.value) < (omega.value) + (route.omega.value))) {
            //in this case routes intersect

            //t contains the parametric angle of Route (*this) where (*this) crosses route
            //u contains the parametric angle of Route route where route crosses (*this)
            vector<Angle> t, u;

            cout << prefix.value << "Routes intersect\n";

            intersection(route, true, &t, new_prefix);
            route.intersection((*this), true, &u, new_prefix);

            compute_end(Length(Re * sin(omega.value) * ((t[0]).value)), new_prefix);
            (*p)[0] = end;
            ((*p)[0]).label.set(String(""), String("crossing"), prefix);

            compute_end(Length(Re * sin(omega.value) * ((t[1]).value)), new_prefix);
            (*p)[1] = end;
            ((*p)[1]).label.set(String(""), String("crossing"), prefix);

            route.compute_end(Length(Re * sin(route.omega.value) * ((u[0]).value)), prefix);

            check &= ((*p)[0]).distance(route.end, &r, String(""), prefix);
            check &= ((*p)[1]).distance(route.end, &s, String(""), prefix);

            if (check) {

                if (r > s) {

                    cout << new_prefix.value << "Exchanging ts!\n";

                    t_temp = u[0];
                    u[0] = u[1];
                    u[1] = t_temp;

                }

                compute_end(Length(Re * sin(omega) * ((t[0]).value)), prefix);
                end.print(String("position of intersection 1 for Route 1"), prefix, cout);

                route.compute_end(Length(Re * sin(route.omega.value) * ((u[0]).value)), prefix);
                (route.end).print(String("position of intersection 1 for Route 2"), prefix, cout);

                (*cos_crossing_angle) = cos((reference_position.phi)) * cos((route.reference_position).phi) * sin(t[0]) * sin(u[0]) + (cos(t[0]) * sin(reference_position.lambda) - cos(reference_position.lambda) * sin((reference_position.phi)) * sin(t[0])) * (cos(u[0]) * sin((route.reference_position).lambda) - cos((route.reference_position).lambda) * sin((route.reference_position).phi) * sin(u[0])) +
                    (cos(reference_position.lambda) * cos(t[0]) + sin((reference_position.phi)) * sin(reference_position.lambda) * sin(t[0])) * (cos((route.reference_position).lambda) * cos(u[0]) + sin((route.reference_position).phi) * sin((route.reference_position).lambda) * sin(u[0]));

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


//evaluates whether Time (*this) is equal to t
bool Time::operator==(const Time& t) {

    Time temp;

    to_MJD();
    temp = t;
    temp.to_MJD();

    return((MJD == (temp.MJD)));

}

//evaluates whether Time (*this) is different from t
bool Time::operator!=(const Time& t) {

    return(!((*this) == t));

}




//evaluates whether Time (*this) is larger than t
bool Time::operator>(const Time& t) {

    Time temp;

    to_MJD();
    temp = t;
    temp.to_MJD();

    return((MJD > (temp.MJD)));

}

//convert *this to a string by adding the time zone if time_zone = true
string Time::to_string(unsigned int precision, bool time_zone) {

    stringstream output;

    //I deleted UTC at the end of string for the sake of shortness
    output << date.to_string() << " " << chrono.to_string(precision, time_zone);

    return (output.str().c_str());

}




void Time::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    date.print(String("date"), new_prefix, ostr);
    chrono.print(String("hour"), new_prefix, ostr);

};


void Time::to_TAI(void) {
    //int &day, int &month, int &year, double &hour)
    /*
     Calculate the calendar date from the Modified Julian Date

     INPUT :
     MJD : Modified Julian Date (Julian Date - 2400000.5)

     OUTPUT :
     day, month, year : corresponding date
     hour : corresponding hours of the above date
     */

    int Yt = 0, Mt = 0, Dt = 0;
    double ht;

    long int b, c, d, e, f, jd0;

    jd0 = long(MJD + 2400001.0);
    if (jd0 < 2299161) c = jd0 + 1524;    /* Julian calendar */
    else
    {                                /* Gregorian calendar */
        b = long((jd0 - 1867216.25) / 36524.25);
        c = jd0 + b - (b / 4) + 1525;
    };

    if (MJD < -2400001.0)  // special case for year < -4712
    {
        if (MJD == floor(MJD)) jd0 = jd0 + 1;
        c = long((-jd0 - 0.1) / 365.25);
        c = c + 1;
        Yt = -4712 - c;
        d = c / 4;
        d = c * 365 + d;  // number of days from JAN 1, -4712
        f = d + jd0;  // day of the year
        if ((c % 4) == 0) e = 61;
        else e = 60;
        if (f == 0)
        {
            Yt = Yt - 1;
            Mt = 12;
            Dt = 31;
            f = 500;  // set as a marker
        };
        if (f < e)
        {
            if (f < 32)
            {
                Mt = 1;
                Dt = f;
            }
            else
            {
                Mt = 2;
                Dt = f - 31;
            };
        }
        else
        {
            if (f < 500)
            {
                f = f - e;
                Mt = long((f + 123.0) / 30.6001);
                Dt = f - long(Mt * 30.6001) + 123;
                Mt = Mt - 1;
            };
        };
    }
    else   // normal case
    {
        d = long((c - 122.1) / 365.25);
        e = 365 * d + (d / 4);
        f = long((c - e) / 30.6001);
        Dt = c - e - long(30.6001 * f);
        Mt = f - 1 - 12 * (f / 14);
        Yt = d - 4715 - ((7 + Mt) / 10);
    };

    ht = 24.0 * (MJD - floor(MJD));


    (date.Y) = ((unsigned int)Yt);
    (date.M) = ((unsigned int)Mt);
    (date.D) = ((unsigned int)Dt);

    (chrono.h) = (unsigned int)(floor(ht));
    (chrono.m) = floor((ht - floor(ht)) * 60.0);
    (chrono.s) = (((ht - floor(ht)) * 60.0) - floor((ht - floor(ht)) * 60.0)) * 60.0;

}

void Time::to_MJD(void)

/*
 Modified Julian Date ( MJD = Julian Date - 2400000.5)
 valid for every date
 Julian Calendar up to 4-OCT-1582,
 Gregorian Calendar from 15-OCT-1582.
 */
{

    long int b, c;
    int Yt = (date.Y), Mt = (date.M), Dt = (date.D);

    MJD = 10000.0 * Yt + 100.0 * Mt + Dt;
    if (Mt <= 2)
    {
        Mt = Mt + 12;
        Yt = Yt - 1;
    };
    if (MJD <= 15821004.1)
    {
        b = ((Yt + 4716) / 4) - 1181;
        if (Yt < -4716)
        {
            c = Yt + 4717;
            c = -c;
            c = c / 4;
            c = -c;
            b = c - 1182;
        };
    }
    else b = (Yt / 400) - (Yt / 100) + (Yt / 4);
    //     { b = -2 + floor((Yt+4716)/4) - 1179;}
    // else {b = floor(Yt/400) - floor(Yt/100) + floor(Yt/4);};

    MJD = 365.0 * Yt - 679004.0;
    //comment this out if you want to include hours, minutes and seconds in MJD
    MJD = MJD + b + int(30.6001 * (Mt + 1)) + Dt + (chrono.get()) / 24.0;


}






//set length according to time and speed if the lenght format for *this is  time * speed
void Route::set_length_from_time_speed(void){
    
    if(length_format == LengthFormat_types[0]){
        
        length = Length(time, speed);
        
    }
    
}


//set length equal to l(t), where l(t) is the value of the curvilinear length corresponding to the parametric coordinate t
void Route::set_length_from_input(double t){
    
    switch ( type.position_in_list(Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            double C, eta;
        
            C = gsl_pow_2(cos(Z));
            eta = sqrt((1-sin((reference_position.phi)))/(1+sin((reference_position.phi))));

            //set the length format, the length unit and the value of the length from t
            length_format.set(LengthFormat_types[1]);
            
            if(fabs(C) > epsilon_double){
                //I am not in the special case where Z = pi/2 or 3 pi /2 (i.e., C = 0)
                
                double s;
                
                s = GSL_SIGN(cos(Z));
                length.set(s * 2.0*Re/sqrt(C) *( atan(eta) - atan( eta * exp(- s * sqrt(C/(1.0-C)) * t ) ) ),
                           LengthUnit_types[0]);
                
            }else{
                //I am in the special case where Z = pi/2 or 3 pi /2 (i.e., C = 0) -> set the length by using the analytical limit C->0 for  expression of the length
                
                length.set(2.0*Re*t*eta/(1.0+gsl_pow_2(eta)),
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
    switch (type.position_in_list(Route_types)) {
            
            
        case 0:{
            //loxodrome route
            
            //this is the +- sign appearing in \phi'(t)  = +- sqrt{C/(1-C)} cos(phi(t));
            int sigma, tau;
            double C, eta;
            Angle t;
            
            
            eta = sqrt((1.0 - sin(reference_position.phi.value)) / (1.0 + sin(reference_position.phi.value)));
            
            //tau = +-_{notes}
            if (((0.0 <= (Z.value)) && ((Z.value) < M_PI_2)) || ((3.0 * M_PI_2 <= (Z.value)) && ((Z.value) < 2.0 * M_PI))) { tau = +1; }
            else { tau = -1; }
            
            if ((0.0 <= (Z.value)) && ((Z.value) < M_PI)) { sigma = -1; }
            else { sigma = +1; }
            
            C = gsl_pow_2(cos(Z));
            
            /* cout << "sigma = " << sigma << "\n"; */
            /* cout << "tau = " << tau << "\n"; */
            /* cout << "C = " << C << "\n"; */
            
            if (((Z.value) != M_PI_2) && ((Z.value) != 3.0 * M_PI_2)) {
                //this is the general expression of t vs l for Z != pi/2
                
                (t.value) = -tau * sqrt((1.0 - C) / C)
                * log(1.0 / eta * tan(-tau * sqrt(C) * (length.value) / (2.0 * Re) + atan(sqrt((1.0 - sin(reference_position.phi.value)) / (1.0 + sin(reference_position.phi.value))))));
                
            }
            else {
                //this is the limit of the expression above in the case Z -> pi/2
                
                (t.value) = (length.value) * (1.0 + gsl_pow_2(eta)) / (2.0 * Re * eta);
                
            }
            
            /* t.print("t", prefix, cout); */
            
            (end.phi).set(String(""), asin(tanh(tau * sqrt(C / (1.0 - C)) * (t.value) + atanh(sin(reference_position.phi.value)))), prefix);
            
            (end.lambda).set(String(""), (reference_position.lambda.value) + sigma * (t.value), prefix);
            
            break;
            
        }
            
        case 1:{
            
            //orthodrome route
            Angle t;
            
     
            t.set(String(""), (length.value) / Re, prefix);
            
            (end.phi).set(String(""), asin(cos(Z) * cos(reference_position.phi) * sin(t) + cos(t) * sin(reference_position.phi)), prefix);
            (end.lambda).set(String(""),
                             -atan(cos(t) * cos(reference_position.lambda) * cos(reference_position.phi) + sin(t) * (sin(Z) * sin(reference_position.lambda) - cos(Z) * cos(reference_position.lambda) * sin(reference_position.phi))
                                   ,
                                   (cos(reference_position.lambda) * sin(t) * sin(Z) + sin(reference_position.lambda) * (-cos(t) * cos(reference_position.phi) + cos(Z) * sin(t) * sin(reference_position.phi)))),
                             prefix);
            
            break;
            
        }
            
        case 2:{
            
            Angle t;
            //compute the parametric angle for the circle of equal altitude starting from the length l of the curve, omega  and the Earth's radius
            //R sin omega = r, r t = l, t = l / (R sin omega)
            
            //compute the length of *this from time and speed, if the length is stored in *this as a time * speed
            set_length_from_time_speed();
            t.set(String(""), (length.value) / (Re * sin(omega)), prefix);
            
            
            (end.phi).set(String(""), M_PI_2 - acos(cos((omega.value)) * sin(reference_position.phi) - cos(reference_position.phi) * cos((t.value)) * sin((omega.value))), prefix);
            
            (end.lambda).set(String(""), -(atan((-sin(reference_position.lambda) * (cos(reference_position.phi) * cos((omega.value)) + cos((t.value)) * sin(reference_position.phi) * sin((omega.value))) + cos(reference_position.lambda) * sin((omega.value)) * sin((t.value))) / (cos(reference_position.phi) * cos(reference_position.lambda) * cos((omega.value)) + sin((omega.value)) * (cos(reference_position.lambda) * cos((t.value)) * sin(reference_position.phi) + sin(reference_position.lambda) * sin((t.value)))))), prefix);
            if (cos(reference_position.phi) * cos(reference_position.lambda) * cos((omega.value)) + sin((omega.value)) * (cos(reference_position.lambda) * cos((t.value)) * sin(reference_position.phi) + sin(reference_position.lambda) * sin((t.value))) <= 0.0) {
                (end.lambda) -= M_PI;
            }
            
            break;
            
        }
            
    }
    
    (end.label.value) = "";
    
}


//This is an overload of compute_end: if d <= (this->l), it writes into this->end the position on the Route at length d along the Route from start and it returns true. If d > (this->l), it returns false.  This method requires that Route::length and d are expressed in units LengthUnit_types[0]
bool Route::compute_end(Length d, [[maybe_unused]] String prefix) {
    
    set_length_from_time_speed();

    if ((type == (Route_types[2])) || (d <= length)) {

        Length l_saved;

        l_saved = length;
        length = d;
        compute_end(prefix);
        length = l_saved;

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

    ostr << prefix.value << name.value << ":\n";

    type.print(String("type"), true, new_prefix, ostr);

    if ((type == (Route_types[0])) || (type == (Route_types[1]))) {

        reference_position.print(String("start position"), new_prefix, ostr);
        Z.print(String("starting heading"), new_prefix, ostr);

        length_format.print(String("length format"), false, new_prefix, ostr);
        if (length_format == (LengthFormat_types[1])) {

            length.print(String("length"), new_new_prefix, ostr);

        }else {

            time.print(String("time"), new_new_prefix, ostr);
            speed.print(String("speed"), new_new_prefix, ostr);

        }

    }
    else {

        reference_position.print(String("ground position"), new_prefix, ostr);
        omega.print(String("aperture angle"), new_prefix, ostr);

    }

    label.print(String("label"), true, new_prefix, ostr);


    if ((related_sight.value) != -1) {

        cout << new_prefix.value << "Related sight # = " << (related_sight.value) + 1 << "\n";

    }

}


//this function returns true if the date read is consistent, false if it is not
template<class S> bool Date::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    bool check = true;
    size_t pos;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    pos = 0;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }

    pos = line.find(" = ");

    Y = stoi(line.substr(pos + 3, 4).c_str(), NULL, 10);

    check_leap_year();
    if ((Y_is_leap_year)) {
        (days_per_month) = days_per_month_leap;
        cout << new_prefix.str() << YELLOW << "Entered a leap year\n" << RESET;
    }
    else {
        (days_per_month) = days_per_month_common;
        cout << new_prefix.str() << "Entered a common year\n";
    }

    M = stoi(line.substr(pos + 3 + 5, 2).c_str(), NULL, 10);

    if (!((1 <= M) && (M < 12 + 1))) {
        check &= false;
        cout << new_prefix.str() << RED << "\tValue of MM is not valid!\n" << RESET;
    }

    D = stoi(line.substr(pos + 3 + 5 + 3, 2).c_str());

    if (!((1 <= D) && (D < days_per_month[M - 1] + 1))) {
        check &= false;
        cout << prefix.value << RED << "\tValue of DD is not valid!\n" << RESET;
    }

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}









void Date::check_leap_year(void) {

    if ((Y % 4) != 0) {

        Y_is_leap_year = false;

    }
    else {

        if ((Y % 100) != 0) {

            Y_is_leap_year = true;

        }
        else {
            if ((Y % 400) != 0) {

                Y_is_leap_year = false;

            }
            else {

                Y_is_leap_year = true;

            }

        }

    }

}

template<class S> bool Time::read_from_stream(String name, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    //read dummy line
    getline((*input_stream), line);

    cout << prefix.value << name.value << ":\n";

    //read date
    if (!(date.read_from_stream<S>(String("date"), input_stream, false, new_prefix))) {
        check &= false;
    }

    //read chrono
    if (!(chrono.read_from_stream<S>(String("hour"), input_stream, false, new_prefix))) {
        check &= false;
    }

    to_MJD();

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}

void Time::operator += (const Chrono& chrono_in) {

    Chrono temp;

    temp = chrono_in;

    to_MJD();
    MJD += temp.get() / 24.0;
    to_TAI();

}


void Time::operator -= (const Chrono& chrono_in) {

    Chrono temp;

    temp = chrono_in;


    to_MJD();
    MJD -= temp.get() / 24.0;
    to_TAI();

}


//return true of both the value and the unit of *this are valid, false otherwise
bool Length::check(String name, [[maybe_unused]] String prefix) {

    bool check = true;

    if (value < 0.0) {
        check &= false;
        cout << prefix.value << RED << "Entered value of " << name.value << " is not valid!\n" << RESET;
    }

    check &= (unit.check());
    
    return check;

}

//reads from file the content after 'name = ' and writes it into this. This function requires file to be correctly set and open
template<class S> void Length::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    size_t pos1, pos2;
    String unit_temp;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline(*input_stream, line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline(*input_stream, line);

    }

    pos1 = line.find(" = ");
    pos1 += 3;
    //from now on pos1 is the starting position of the  numerical value
    //pos2-1 contains the last character of th enumerical value
    pos2 = line.find(" ", pos1);
    
    //thus I store the numerical value in to value ...
    value = stod(line.substr(pos1, pos2 - pos1).c_str());
    
    // .. and the unit inot unit
    pos1 = pos2+1;
    unit.set(line.substr(pos1));

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);

}

//reads from file the content after 'name = ' and writes it into this.
void Length::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Length>(this, name, filename, mode, prefix);

}











void Answer::enter(String name, [[maybe_unused]] String prefix) {

    bool check;
    string temp;

    do {

        temp.clear();

        cout << prefix.value << "Enter " << name.value << " [y/n]:";

        getline(cin >> ws, temp);

        if (((temp[0] == 'y') || (temp[0] == 'n')) && (temp.size() == 1)) {
            value = temp[0];
            check = true;
        }
        else {
            cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
            check = false;
        }
    } while (!check);

    print(name, prefix, cout);

}

void Answer::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}


void Sight::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

    unsigned int j;
    Time time_UTC;


    j = 0;
    //set number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //set body column
    listcontrol->SetItem(i, j++, wxString(body.name.value));

    //set limb column
    if ((body.name == String("sun")) || (body.name == String("moon"))) {

        if (wxString(limb.value) == 'u') { listcontrol->SetItem(i, j++, wxString((Limb_types[0].value))); }
        if (wxString(limb.value) == 'l') { listcontrol->SetItem(i, j++, wxString((Limb_types[2].value))); }
        if (wxString(limb.value) == 'c') { listcontrol->SetItem(i, j++, wxString((Limb_types[1].value))); }

    }
    else {

        listcontrol->SetItem(i, j++, wxString(""));

    }

    //set artificial horizon column
    listcontrol->SetItem(i, j++, wxString(artificial_horizon.value));

    //set sextant altitude column
    listcontrol->SetItem(i, j++, wxString((H_s).to_string(String(""), (display_precision.value), true)));

    //set index error
    listcontrol->SetItem(i, j++, wxString((index_error).to_string(String(""), (display_precision.value), true)));

    //set height of eye column
    if (artificial_horizon.value == 'n') {

        //write the height_of_eye with its original unit of measure
        listcontrol->SetItem(i, j++, wxString(height_of_eye.to_string( (display_precision.value))));

    }
    else {

        listcontrol->SetItem(i, j++, wxString(""));

    }

    //set column of master-clock date and hour of sight
    //I add to master_clock_date_and_hour the value stopwatch (if any): I write the result in time_UTC and I write in the GUI object  time_UTC
    time_UTC = master_clock_date_and_hour;
    //    if((use_stopwatch.value)=='y'){time_UTC += stopwatch;}
    listcontrol->SetItem(i, j++, wxString(time_UTC.to_string(display_precision.value, false)));

    //set use of stopwatch
    listcontrol->SetItem(i, j++, wxString((use_stopwatch.value)));

    //set stopwatch reading
    if ((use_stopwatch.value) == 'y') {
        listcontrol->SetItem(i, j++, wxString((stopwatch).to_string(display_precision.value, false)));
    }
    else {
        listcontrol->SetItem(i, j++, wxString(""));
    }

    //set TAI-UTC
    listcontrol->SetItem(i, j++, wxString((TAI_minus_UTC).to_string((display_precision.value), false)));

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol

    listcontrol->SetItem(i, j++, wxString(label.value));

    //    }else{
    //        //the label in *this is empty -> I write as label in listcontrol the current date and time
    //
    //        Time now;
    //
    //        now.set_current(String(""));
    //
    //        label.set(String("Label replacing empty label"), now.to_string(display_precision.value), String(""));
    //        listcontrol->SetItem(i, j++, wxString(now.to_string(display_precision.value)));
    //
    //    }

    if ((related_route.value) == -1) {
        //if the sight is not connected to a route, I leave the column field empty

        listcontrol->SetItem(i, j++, wxString(""));

    }
    else {
        //if the sight is connected to a route, I write the # of the related route in the column field

        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_route.value) + 1));

    }


}


void Sight::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    long i;
    wxListItem item;

    if (position_in_listcontrol == -1) {

        i = (listcontrol->GetItemCount());

    }
    else {

        i = position_in_listcontrol;
        listcontrol->DeleteItem(i);

    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_wxListCtrl(i, listcontrol);

}


//this function returns true if the reading operation has been performed without errors, false otherwise
template<class S> bool Sight::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    bool check = true;
    String new_prefix;
    //this unsigned int counts how many additional entries have been inserted into the vector item
    unsigned int additional_items;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    additional_items = 0;

    body.read_from_stream<S>(String("body"), input_stream, false, new_prefix);
    if (body.type.value != "star") {
        items.insert(items.begin() + 1 + (additional_items++), all_items[1]);
        limb.read_from_stream<S>(String("limb"), input_stream, false, new_prefix);
    }
    H_s.read_from_stream<S>(String("sextant altitude"), input_stream, false, new_prefix);
    index_error.read_from_stream<S>(String("index error"), input_stream, false, new_prefix);
    artificial_horizon.read_from_stream<S>(String("artificial horizon"), input_stream, false, new_prefix);
    if (artificial_horizon == Answer('n', new_prefix)) {
        items.insert(items.begin() + 3 + (additional_items++), String("height of eye"));
        height_of_eye.read_from_stream<S>(String("height of eye"), input_stream, false, new_prefix);
    }

    check &= master_clock_date_and_hour.read_from_stream<S>(String("master-clock date and hour of sight"), input_stream, false, new_prefix);
    if (!check) {
        cout << prefix.value << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
    }
    time = master_clock_date_and_hour;

    use_stopwatch.read_from_stream<S>(String("use of stopwatch"), input_stream, false, new_prefix);

    if (use_stopwatch == Answer('y', new_prefix)) {

        items.insert(items.begin() + 5 + (additional_items++), String("stopwatch reading"));
        stopwatch.read_from_stream<S>(String("stopwatch"), input_stream, false, new_prefix);
        time += stopwatch;

    }

    TAI_minus_UTC.read_from_stream<S>(String("TAI - UTC at time of master-clock synchronization with UTC"), input_stream, false, new_prefix);
    time += TAI_minus_UTC;
    time.print(String("TAI date and hour of sight"), new_prefix, cout);

    //check whether the date and hour of sight falls within the time window covered by JPL data files
    check &= check_time_interval(prefix);

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }

    //given that the sight is not yet related to a route, I set
    (related_route.value) = -1;

    if (!check) {
        cout << prefix.value << RED << "Error reading sight!\n" << RESET;
    }

    return check;

}

//compute this->time and returns true if time lies within the data file of NASA JPL ephemerides files, and false otherwise. This function requires that body.name is specified: if body.name is "", it returns false.
bool Sight::check_time_interval(String prefix) {

    String new_prefix;
    stringstream temp;
    FileR data_file;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //data_file is the file where that data relative to body are stored: I count the number of lines in this file and store them in data_file.number_of_lines
    temp.clear();
    if ((body.type) != String("star")) {
        temp << (wxGetApp().data_directory).value << (body.name.value) << ".txt";
    }
    else {
        temp << (wxGetApp().data_directory).value << "j2000_to_itrf93.txt";
    }
    data_file.set_name(temp.str());


    if (data_file.check_if_exists(new_prefix)) {
        //the ephemerides file data_file exists -> check the time interval

        int l_min, l_max;
        bool check;

        //compute this->time

        //I first set time to master_clock_date_and_hour ...
        time = master_clock_date_and_hour;
        //.. then I add to it sight->stopwatch, if any ....
        if (use_stopwatch == Answer('y', String(""))) {
            time += stopwatch;
        }
        //... then I add to it TAI_minus_UTC, to convert it from the UTC to the TAI scale
        time += TAI_minus_UTC;

        data_file.count_lines(new_prefix);

        //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
        l_min = (int)(L * ((time.MJD) - MJD_min)) - (int)(N / 2.0);
        //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
        l_max = (int)(L * ((time.MJD) - MJD_min)) + (int)(N / 2.0);

        //check whether the lines from l_min to l_max, which are used for the data interpolation, are present in the file where data relative to the body are stored
        if ((l_min >= 0) && (l_max < (int)(data_file.number_of_lines))) {
            check = true;
        }
        else {
            check = false;
            cout << new_prefix.value << RED << "Time lies outside interval of NASA's JPL data files!\n" << RESET;
        }

        return check;

    }
    else {
        //the ephemerides file data_file does not exist

        cout << new_prefix.value << RED << "Cannot check time interval because ephemerides' file does not exist!\n" << RESET;

        return false;

    }

}

void Sight::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    body.print(String("body"), new_prefix, ostr);
    if (body.type.value != "star") {
        limb.print(String("limb"), new_prefix, ostr);
    }
    H_s.print(String("sextant altitude"), new_prefix, ostr);
    index_error.print(String("index error"), new_prefix, ostr);
    artificial_horizon.print(String("artificial horizon"), new_prefix, ostr);
    if (artificial_horizon == Answer('n', new_prefix)) {
        height_of_eye.print(String("height of eye"), new_prefix, ostr);
    }
    master_clock_date_and_hour.print(String("master-clock date and hour of sight"), new_prefix, ostr);
    use_stopwatch.print(String("use of stopwatch"), new_prefix, ostr);
    if (use_stopwatch == Answer('y', new_prefix)) {
        stopwatch.print(String("stopwatch"), new_prefix, ostr);
    }
    TAI_minus_UTC.print(String("TAI - UTC at time of master-clock synchronization with UTC"), new_prefix, ostr);

    label.print(String("label"), true, new_prefix, ostr);

    if (((related_route.value) != -1) && (&ostr == &cout)) {
        ostr << new_prefix.value << "# of related route = " << (related_route.value) + 1 << "\n";
    }

    /*
     cout << RED << "items:\n";
     for(unsigned int i=0; i<items.size(); i++){
     cout << new_prefix.value << items[i].value << "\n";

     }
     cout << RESET << "\n";
     */

}

//default constructor
PositionProjection::PositionProjection(void) {

}

//custom constructor, which initialized the point with coordinates x, y
PositionProjection::PositionProjection(const double x_in, const double y_in) {

    x = x_in;
    y = y_in;

}

inline PositionProjection PositionProjection::operator + (const PositionProjection& q) {

    PositionProjection p;

    (p.x) = x + (q.x);
    (p.y) = y + (q.y);

    return p;

}

inline PositionProjection PositionProjection::operator - (const PositionProjection& q) {

    PositionProjection p;

    (p.x) = x - (q.x);
    (p.y) = y - (q.y);

    return p;

}

//divide both members of *this by the number x and return the resulting  PositionProjection
inline PositionProjection PositionProjection::operator / (const double& q) {

    PositionProjection p;

    (p.x) = x/q;
    (p.y) = y/q;

    return p;

}


inline PositionProjection PositionProjection::operator / (const Double& q){
    
    return ((*this)/(q.value));
    
}

//multiply both members of *this by the number x and return the resulting  PositionProjection
inline PositionProjection PositionProjection::operator * (const double& q) {

    PositionProjection p;

    (p.x) = x*q;
    (p.y) = y*q;

    return p;

}


inline void PositionProjection::operator += (const PositionProjection& q) {

    (*this) = (*this) + q;
    
}


inline void PositionProjection::operator -= (const PositionProjection& q) {

    (*this) = (*this) - q;
    
}


inline void PositionProjection::operator *= (const double& a) {

    (*this) = (*this) * a;
    
}


inline void PositionProjection::operator /= (const double& a) {

    (*this) = (*this) / a;
    
}


//return true if *this is falls wihtin the plot area of *draw_panel in the Mercator projection, and false otherwise
inline bool PositionProjection::CheckMercator(DrawPanel* draw_panel){
    
    bool output;
    

    if ((draw_panel->x_min) <= (draw_panel->x_max)) {
        //this is the 'normal' configuration where the boundaries of the chart do not encompass the meridian lambda = pi

        output = (((draw_panel->x_min) <= x) && (x <= (draw_panel->x_max)));

    }else {
        //this is the 'non-normal' configuration where the boundaries of the chart encompass the meridian lambda = pi

        output = ((((draw_panel->x_min) <= x) && (x <= (draw_panel->x_max) + 2.0*M_PI)) ||  (((draw_panel->x_min) - 2.0*M_PI <= x) && (x <= (draw_panel->x_max))));

    }

    output &= (((draw_panel->y_min) <= y) && (y <= (draw_panel->y_max)));
    
    return output;

}


//normalize p.lambda, then set x and y equal to the Mercator projections of the Position p
inline void PositionProjection::SetMercator(const Position& p){
    
    x = -(p.lambda.value);
    y = log(1.0 / cos((p.phi)) + tan((p.phi)));
    
}


//normalize p.lambda, then set x and y equal to the Mercator projections of the Position p
inline void PositionProjection::NormalizeAndSetMercator(const Position& p){
    
    Position temp;
    
    temp = p;
    temp.lambda.normalize_pm_pi();
    
    SetMercator(temp);
     
}


// this function plots the Routes of type (Route_types[2]) in route_list in kml forma. WARNING: THIS FUNCTION USES THE SYSTEM() COMMAND AND THUS IT IS NOT PORTABLE ACROSS PLATFORMS. Also, this functions used file_kml, which has been removed from the code, and it should be revised.
void Data::print_to_kml(String prefix) {

    stringstream line_ins, /*plot_title contains the  title of the Route to be plotted*/ plot_title;
    string line;
    unsigned int i, j;
    double lambda_kml, phi_kml;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //	file_init.open(prefix);
    //	file_kml.remove(prefix);

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
            plot_command << "\\\t<Style id=\\\"" << i << "\\\">\\\n\\\t<LineStyle>\\\n\\\t\\\t<color>" << /*I use the remainder of i in this way, so if i > size of kml_colors, I start back reading from the beginning of kml_colors*/ kml_colors[i % (sizeof(kml_colors) / sizeof(*kml_colors))] << "<\\/color>\\\n\\\t\\\t<width>2<\\/width>\\\n\\\t<\\/LineStyle>\\\n\\\t<\\/Style>\\\n\\\t<Placemark>\\\n\\\t\\\t<name>"
                << (route_list[i]).label.value
                << "<\\/name>\\\n\\\t\\\t<styleUrl>#" << i << "<\\/styleUrl>\\\n\\\t\\\t<description>"
                << (route_list[i]).label.value
                << "<\\/description>\\\n\\\t\\\t<LineString>\\\n\\\t\\\t\\\t<extrude>1<\\/extrude>\\\n\\\t\\\t\\\t<tessellate>0<\\/tessellate>\\\n\\\t\\\t\\\t<altitudeMode>absolute<\\/altitudeMode>\\\n\\\t\\\t\\\t<coordinates>\\\n";



            for (j = 0; j < (unsigned int)(wxGetApp().n_points_routes.value); j++) {

                //I consider a Length equal to a temporary value of the length of the route, which spans between 0 and 2.0*M_PI*(Re*sin(((route_list[i]).omega.value))) across the for loop over j
                //I compute the coordinate of the endpoint of route_list[i] for the ((route_list[i]).l) above
                (route_list[i]).compute_end(Length(2.0 * M_PI * (Re * sin(((route_list[i]).omega.value))) * ((double)j) / ((double)(wxGetApp().n_points_routes.value - 1))), new_prefix);

                //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude, and in the third altitude (I am not interested in altitude, thus is set it to 0); The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.

                lambda_kml = -K * ((route_list[i]).end.lambda.value);
                if (lambda_kml < -180.0) {
                    lambda_kml += 360.0;
                }

                phi_kml = K * ((route_list[i]).end.phi.value);
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
        plot_command << "\\\n\\\t<Placemark>\\\n\\\t\\\t<Style>\\\n\\\t\\\t\\\t<IconStyle>\\\n\\\t\\\t\\\t\\\t<color>7733ff66<\\/color>\\\n\\\t\\\t\\\t\\\t<IconStyleSimpleExtensionGroup radius=\\\"3\\\" points=\\\"Infinity\\\" strokeColor=\\\"" << /*I use the remainder of i in this way, so if i > size of hex_colors, I start back reading from the beginning of hex_colors*/ hex_colors[i % (sizeof(hex_colors) / sizeof(*hex_colors))] << "\\\" strokeWidth=\\\"2\\\" lineDash=\\\"undefined\\\"\\/>\\\n\\\t\\\t\\\t<\\/IconStyle>\\\n\\\t\\\t\\\t<PolyStyle xmlns=\\\"\\\">\\\n\\\t\\\t\\\t\\\t<outline>0<\\/outline>\\\n\\\t\\\t\\\t<\\/PolyStyle>\\\n\\\t\\\t<\\/Style>\\\n\\\t\\\t<description>"
            << (position_list[i]).label.value << "<\\/description>\\\n\\\t\\\t<ExtendedData>\\\n\\\t\\\t\\\t<Data name=\\\"scStyle\\\"\\/>\\\n\\\t\\\t<\\/ExtendedData>\\\n\\\t\\\t<Point>\\\n\\\t\\\t\\\t<coordinates>";


        //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude. The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.

        lambda_kml = -K * ((position_list[i]).lambda.value);
        if (lambda_kml < -180.0) {
            lambda_kml += 360.0;
        }

        phi_kml = K * ((position_list[i]).phi.value);
        if (phi_kml > 270.0) {
            phi_kml -= 360.0;
        }

        plot_command << lambda_kml << "," << phi_kml << "<\\/coordinates>\\\n\\\t\\\t<\\/Point>\\\n\\\t<\\/Placemark>";

    }

    //add the line to plot_kml.kml which contains the parametric plot of the positions
    command << "LANG=C sed 's/\\/\\/position\\_plots/" << plot_command.str().c_str() << "/g' kml_temp.kml >> '" /*<< file_kml.name.value*/ << "'\nrm -rf kml_temp.kml\n";

    //execute the command string
    system(command.str().c_str());



    //	file_init.close(prefix);

}


Data::Data(Catalog* cata, [[maybe_unused]] String prefix) {

    String new_prefix;

    new_prefix = prefix.append(String("\t"));


    catalog = cata;
    job_id = -1;

    plot_command.precision((data_precision.value));
    command.precision((data_precision.value));

    //	file_init.set_name((wxGetApp().path_file_init));

    choices.push_back(String("Add a sight"));
    choices.push_back(String("Modify a sight"));
    choices.push_back(String("Delete a sight"));
    choices.push_back(String("Add a position"));
    choices.push_back(String("Modify a position"));
    choices.push_back(String("Transport a position"));
    choices.push_back(String("Delete a position"));
    choices.push_back(String("Add a route"));
    choices.push_back(String("Transport a route"));
    choices.push_back(String("Compute route crossings"));
    choices.push_back(String("Delete a route"));
    choices.push_back(String("Replot"));
    choices.push_back(String("Full zoom out"));
    choices.push_back(String("Clear"));
    choices.push_back(String("Line of position on paper chart"));
    choices.push_back(String("Save to file"));
    choices.push_back(String("Read from file"));
    choices.push_back(String("Exit"));


    //read paramters from init file

//	file_init.open(String("in"), prefix);

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

                        q.push_back(q_temp[0]);
                        q.push_back(q_temp[1]);

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
            r.set(M_PI * Re, LengthUnit_types[0]);

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
            q.push_back(center);

            for (i = 0; i < p.size(); i++) {

                if (!(((p[i][0]) == center) || ((p[i][1]) == center))) {

                    center.distance(p[i][0], &r, String(""), new_prefix);
                    center.distance(p[i][1], &s, String(""), new_prefix);

                    if(r > s){

                        q.push_back(p[i][1]);

                    }else{

                        q.push_back(p[i][0]);

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

            position_list.push_back(center);

            
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
                (error_circle.reference_position) = center;
                (error_circle.omega.value) = (r.value) / Re;
                (error_circle.label) = String("error on astronomical position");
                ((error_circle.related_sight).value) = -1;
                
                r.print(String("error on astronomical position"), prefix, cout);
                route_list.push_back(error_circle);
                
                
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
    (((*sight_in).related_route).value) = ((int)(route_list.size())) - 1;
    //push back sight_in into sight_list
    sight_list.push_back(*sight_in);
    (((route_list[route_list.size() - 1]).related_sight).value) = ((int)(sight_list.size())) - 1;

    //I commented this out because now the sight is enetered through the GUI
    //    (sight_list[sight_list.size()-1]).enter((*catalog), String("new sight"), prefix);
    check &= ((sight_list[sight_list.size() - 1]).reduce(&(route_list[route_list.size() - 1]), prefix));

    //I link the sight to the route, and the route to the sight
    ((sight_list[sight_list.size() - 1]).related_route.value) = ((int)route_list.size()) - 1;
    ((*sight_in).related_route.value) = ((int)route_list.size()) - 1;
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

    route_list.push_back(*route_in);
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

                sight_list.push_back(sight);
                cout << new_prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";

                route_list.push_back(route);
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

        route_list.push_back(route);
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

        position_list.push_back(position);
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

}


//print recent_items to ostr. Here name is the "[plural name of the item]"  : for example, if I am printing a list of Bodies, name.value = "bodies". This method is used for GUI fields of the format MultipleItemField
void Data::print_recent_items(const vector<int>& recent_items, String name, String prefix, ostream& ostr) {

    unsigned int i;
    stringstream temp;


    for (temp.str(""), i = 0; i < (recent_items.size()) - 1; i++) {
        temp << recent_items[i] << " ";
    }
    temp << recent_items.back();
    String(temp.str().c_str()).print(String("Recent ").append(name), false, prefix, ostr);

}


//insert the item 'item_id' into the vector of items *recent_items.  This method is used for GUI fields of the format MultipleItemField
void Data::insert_recent_item(unsigned int item_id, vector<int>* recent_items) {

    vector<int>::iterator position;

    position = find(recent_items->begin(), recent_items->end(), item_id);

    if (position == recent_items->end()) {
        //in this case, the selected item is not in the recent list: I write it in the recent list and in file_recent

        (*recent_items)[recent_items->size() - 1] = item_id;
        rotate(recent_items->begin(), recent_items->end() - 1, recent_items->end());

    }
    else {
        //the selected item is  in the recent list: I move the element in position to the first place in recent_items

        iter_swap(recent_items->begin(), position);

    }

}



bool Sight::reduce(Route* circle_of_equal_altitude, [[maybe_unused]] String prefix) {

    bool check = true;
    String new_prefix;
    stringstream temp;

    temp.clear();

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Reducing sight ...\n";

    (circle_of_equal_altitude->type.value) = ((Route_types[2]).value);

    compute_H_a(new_prefix);
    check &= get_coordinates(circle_of_equal_altitude, new_prefix);

    //link the circle of equal altitude (*circle_of_equal_altitude) to sight (*this)
    temp << body.name.value << " " << time.to_string(display_precision.value, false) << " TAI, " << label.value;
    (circle_of_equal_altitude->label).set(String(""), String(temp.str()), new_prefix);

    check &= compute_H_o(new_prefix);
    circle_of_equal_altitude->omega.set(M_PI_2 - (H_o.value));
    
    circle_of_equal_altitude->length.set(2.0 * M_PI * Re * sin(circle_of_equal_altitude->omega), LengthUnit_types[0]);

    if (!check) {

        cout << prefix.value << RED << "Sight cannot be reduced!\n" << RESET;

    }
    else {

        cout << prefix.value << "... done\n";

    }

    return check;

}


void Sight::compute_H_a(String prefix) {

    if (artificial_horizon == Answer('y', prefix)) {
        H_a = (H_s - index_error) / 2.0;
        H_a.print(String("apparent altitude"), prefix, cout);

    }
    else {
        compute_DH_dip(prefix);
        H_a = H_s - index_error + DH_dip;
        H_a.print(String("apparent altitude"), prefix, cout);
    }

}


bool Sight::compute_H_o(String prefix) {

    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Computing observed altitude ...\n";

    check &= compute_DH_refraction(new_prefix);

    if (check) {
        compute_DH_parallax_and_limb(new_prefix);
        H_o = H_a + DH_refraction + DH_parallax_and_limb;
        cout << prefix.value << "...done\n";
        H_o.print(String("observed altitude"), prefix, cout);
    }
    else {
        cout << prefix.value << RED << "H_o cannot be computed!\n" << RESET;
    }

    return check;

}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    H_i = H_a + DH_refraction;
    H_i.print(String("intermediate altitude"), prefix, cout);

    if (body.type.value != "star") {

        switch ((limb.value)) {

        case 'u':
        {
            int status = 0;
            int iter = 0;
            double x = 0.0, x_lo = 0.0, x_hi = 2.0 * M_PI;
            gsl_function F;
            const gsl_root_fsolver_type* T;
            gsl_root_fsolver* s;

            F.function = &rhs_DH_parallax_and_limb;
            F.params = &(*this);

            T = gsl_root_fsolver_brent;
            s = gsl_root_fsolver_alloc(T);
            gsl_root_fsolver_set(s, &F, x_lo, x_hi);

            cout << new_prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";

            cout << new_prefix.value << "iter" << "[lower" << ", upper] " << "root " << "err(est)\n";

            iter = 0;
            do {

                iter++;
                status = gsl_root_fsolver_iterate(s);

                x = gsl_root_fsolver_root(s);
                x_lo = gsl_root_fsolver_x_lower(s);
                x_hi = gsl_root_fsolver_x_upper(s);
                status = gsl_root_test_interval(x_lo, x_hi, 0.0, epsrel);
                if (status == GSL_SUCCESS) {
                    cout << new_prefix.value << "Converged.\n";
                }
                cout << new_prefix.value << iter << " [" << x_lo << ", " << x_hi << "] " << x << " " << x_hi - x_lo << "\n";

            } while ((status == GSL_CONTINUE) && (iter < max_iter));


            //H_o.value = (x_lo+x_hi)/2.0;
            DH_parallax_and_limb.value = (x_lo + x_hi) / 2.0 - (H_i.value);
            gsl_root_fsolver_free(s);

            break;
        }
        case 'l':
        {
            //    H_o.value = (H_i.value) + asin(((atmosphere.earth_radius.value)*cos(H_i)+(body.radius.value))/(r.value));
            DH_parallax_and_limb.value = asin(((atmosphere.earth_radius.value) * cos(H_i) + (body.radius.value)) / (r.value));
            break;
        }
        case 'c':
        {
            //H_o.value = (H_i.value) + asin((atmosphere.earth_radius.value)*cos(H_i)/(r.value));
            DH_parallax_and_limb.value = asin((atmosphere.earth_radius.value) * cos(H_i) / (r.value));
            break;
        }
        }

        DH_parallax_and_limb.print(String("parallax and limb correction"), prefix, cout);

    }
    else {

        DH_parallax_and_limb.value = 0.0;

    }


}

double Atmosphere::T(Length z) {

    double x = 0.0;
    //cout << "z = " << (z.value) << "\n";

    if (z <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, check = true; (i < n_layers) && check; i++) {
            if ((z >= h[i]) && (z < h[i + 1])) {
                x = t[i] + lambda[i] * ((z - h[i]).convert(LengthUnit_types[0]).value);
                check = false;
            }
        }
        //cout << "i = " << i << "\n";

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return x;


}

double Atmosphere::dTdz(Length z) {

    double x = 0.0;
    //cout << "z = " << (z.value) << "\n";

    if (z <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, check = true; (i < n_layers) && check; i++) {
            if ((z >= h[i]) && (z < h[i + 1])) {
                x = lambda[i];
                check = false;
            }
        }

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return x;


}


double Atmosphere::n(Length z) {

    double x = 0.0;

    if (z <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, x = 0.0, check = true; (i < n_layers) && check; i++) {
            if ((z >= h[i]) && (z < h[i + 1])) {
                if (lambda[i] != 0.0) {
                    x -= B / lambda[i] * log((t[i] + lambda[i] * ((z - h[i]).convert(LengthUnit_types[0]).value)) / t[i]);
                }
                else {
                    x -= B * ((z - h[i]).convert(LengthUnit_types[0]).value) / t[i];
                }
                check = false;
            }
            else {
                if (lambda[i] != 0.0) {
                    x -= B / lambda[i] * log((t[i] + lambda[i] * ((h[i + 1] - h[i]).convert(LengthUnit_types[0]).value)) / t[i]);
                }
                else {
                    x -= B * ((h[i + 1] - h[i]).convert(LengthUnit_types[0]).value) / t[i];
                }
            }
        }

        /*
         int_0^z dz/(t_n+lambda_n*(z-h_n)) = log()
         */

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return (A * P_dry_0 / T(z) * exp(x) / (1e6) + 1.0);


}

double Atmosphere::dndz(Length z) {

    return (-1.0 / T(z) * dTdz(z) * (n(z) - 1.0) - (n(z) - 1.0) * B / T(z));

}


double Sight::dH_refraction(double z, void* sight) {

    Sight* a = (Sight*)sight;
    Length z_Length, zero_Length;
    
//    z_Length.value = z;
    z_Length.set(z, LengthUnit_types[0]);
    zero_Length.set(0.0, LengthUnit_types[0]);

    return(-(a->atmosphere.earth_radius.value) * (a->atmosphere.n(zero_Length)) * cos((a->H_a)) * (a->atmosphere.dndz)(z_Length) / (a->atmosphere.n)(z_Length) / sqrt(gsl_pow_2(((a->atmosphere.earth_radius.value) + z) * (a->atmosphere.n)(z_Length)) - gsl_pow_2((a->atmosphere.earth_radius.value) * (a->atmosphere.n)(zero_Length) * cos((a->H_a)))));

}


//this function returns the longitude lambda of a circle of equal altitude (*this) - pi
double Route::lambda_minus_pi(double t, void* route) {

    Route* r = (Route*)route;
    String new_prefix;

    //append \t to prefix
    new_prefix = (r->temp_prefix.append(String("\t")));

    r->length.set(Re * sin((r->omega.value)) * t, LengthUnit_types[0]);
    r->compute_end(new_prefix);

    return(((*r).end.lambda.value) - M_PI);

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
        
        if (abs(-tan(reference_position.phi.value) * tan((omega.value))) < 1.0) {
            //im this case ( abs(-tan(reference_position.phi.value)*tan((omega.value))) < 1.0) there exists a value of t = t_{max} (t_{min}) such that reference_position.lambda vs. t has a maximum (minimum). In this case, I proceed and compute this maximum and minimum, and write reference_position.lambda_{t = t_{min}} and reference_position.lambda_{t = t_{max}}] in lambda_min, lambda_max
            
            //compute the values of the parametric Angle t, t_min and t_max, which yield the position with the largest and smallest longitude (p_max and p_min) on the circle of equal altitude
            t_max.set(String(""), acos(-tan(reference_position.phi.value) * tan((omega.value))), new_prefix);
            t_min.set(String(""), 2.0 * M_PI - acos(-tan(reference_position.phi.value) * tan((omega.value))), new_prefix);
            
            //p_max =  Position on the circle of equal altitude  at t = t_max
            length.set(Re * sin((omega.value)) * (t_max.value), LengthUnit_types[0]);
            compute_end(new_prefix);
            p_max = end;
            
            //p_min =  Position on circle of equal altitude  at t = t_min
            length.set(Re * sin((omega.value)) * (t_min.value), LengthUnit_types[0]);
            compute_end(new_prefix);
            p_min = end;
            
            //set lambda_min/max in this order, which is eventually rectified at the end of this function
            (*lambda_min) = (p_min.lambda);
            (*lambda_max) = (p_max.lambda);
            
            
            /* p_max.print(String("p_max"), new_prefix, cout); */
            /* p_min.print(String("p_min"), new_prefix, cout); */
            
        }else {
            //in this case, reference_position.lambda vs. t has no minimum nor maximum: lambda_min/max are simly given by
            
            //set lambda_min/max in this order, meaning that *this spans all longitudes, from 0 to 2 pi
            (*lambda_min).set(0.0);
            (*lambda_max).set(0.0);
            
        }
        
        //sort lambda_min and lambda_max
        if ((*lambda_min) > (*lambda_max)) {
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
        //eventually swap lambda_min/max in such a way that lambda_min lies on the left and lambda_max lies on the right as seen from the observer's position looking at the earth's center
        if ((((*lambda_min).value) < M_PI) && (((*lambda_max).value) > M_PI)) {
            
            if (!(((reference_position.lambda) < (*lambda_min)) || ((reference_position.lambda) > (*lambda_max)))) {
                
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
        
        (*lambda_min) = (reference_position.lambda);
        
        compute_end(String(""));
        (*lambda_max) = (end.lambda);
        
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
    
    
    switch (type.position_in_list(Route_types)) {
            
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
            
            ts = atan(sin(reference_position.phi), cos(Z)*cos(reference_position.phi));
            
            //inlude in phi the latitude of the starting point of *this
            compute_end(Length(0.0), String(""));
            phi.push_back(end.phi.normalize_pm_pi_ret());
            
            
            //there are two potential stationary points for the latitude vs t: include in phi the first one, if it lies on *this
            if((0.0 <= Re*ts) && (length >= Re*ts)){
                
                //                t.push_back(Angle(ts));
                
                compute_end(Length(Re*ts), String(""));
                phi.push_back(end.phi.normalize_pm_pi_ret());
                
            }
            
            //there are two potential stationary points for the latitude vs t: include in phi the second one, if it lies on *this
            if((0.0 <= Re*(ts+M_PI)) && (length >= Re*(ts+M_PI))){
                
                //                t.push_back(Angle(ts+M_PI));
                
                compute_end(Length(Re*(ts+M_PI)), String(""));
                phi.push_back(end.phi.normalize_pm_pi_ret());
                    
            }
            
            //*include in *phi the latitude of the endpoint of *this
            compute_end(String(""));
            phi.push_back(end.phi.normalize_pm_pi_ret());
            
            
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
                
            length.set(Re * sin((omega.value)) * 0.0, LengthUnit_types[0]);
            compute_end(new_prefix);
            p_max = end;
            
            length.set(Re * sin((omega.value)) * M_PI, LengthUnit_types[0]);
            compute_end(new_prefix);
            p_min = end;
            
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



double Sight::rhs_DH_parallax_and_limb(double h, void* sight) {

    Sight* a = (Sight*)sight;

    //  (((*a).atmosphere).earth_radius.value)      R
    //(((*a).body).radius.value)                   r
    //(((*a).r).value)          d

    return(-(((*a).H_i).value) + h + asin((((*a).body).radius.value) / sqrt(gsl_pow_2((((*a).r).value)) + gsl_pow_2((((*a).atmosphere).earth_radius.value)) - 2.0 * (((*a).r).value) * (((*a).atmosphere).earth_radius.value) * sin(h))) - atan(((((*a).atmosphere).earth_radius.value) * cos(h)) / ((((*a).r).value) - (((*a).atmosphere).earth_radius.value) * sin(h))));

}

void Atmosphere::set(void) {

    unsigned int i;
    double x;
    bool check;

    //cout << "Atmosphere model: US 1976.\n";

    n_layers = 7;
    A = 0.7933516713545163;
    B = 34.16 * nm_to_km;
    P_dry_0 = 101325.0;
    alpha = -6.5 * nm_to_km;
    beta = 2.8 * nm_to_km;
    gamma = -2.8 * nm_to_km;
    T0 = 288.15;
    earth_radius.value = Re;

    h.resize(n_layers + 1);
    lambda.resize(n_layers);
    t.resize(n_layers);

    
    h[0] = 0.0;
    h[1] = 11.0 * km_to_nm;
    h[2] = 20.0 * km_to_nm;
    h[3] = 32.0 * km_to_nm;
    h[4] = 47.0 * km_to_nm;
    h[5] = 51.0 * km_to_nm;
    h[6] = 71.0 * km_to_nm;
    h[7] = 84.8520 * km_to_nm;

    lambda[0] = -6.5 * nm_to_km;
    lambda[1] = 0.0 * nm_to_km;
    lambda[2] = 1.0 * nm_to_km;
    lambda[3] = 2.8 * nm_to_km;
    lambda[4] = 0.0 * nm_to_km;
    lambda[5] = -2.8 * nm_to_km;
    lambda[6] = -2.0 * nm_to_km;


    for (i = 0, x = T0, check = true; (i < n_layers) && check; i++) {
        t[i] = x;
        x += lambda[i] * ((h[i + 1] - h[i]).convert(LengthUnit_types[0]).value);
    }


    /* for(int i=0; i<4+1; i++){ */
    /*   cout << "\t\t" << i << " " << h[i] << "\n"; */
    /* } */


}





Sight::Sight(void) {
    
    //height_of_eye is expressed in meters -> set its unit accordingly
    height_of_eye.unit.set(LengthUnit_types[1]);

    //this is the list of all the possible items that a Sight object can have: some Sight objects may have an item list with fewer elements than all_items. For instance, a star Sight does not have the "limb" element.
    all_items.push_back(String("body"));
    all_items.push_back(String("limb"));
    all_items.push_back(String("sextant altitude"));
    all_items.push_back(String("artificial horizon"));
    all_items.push_back(String("height of eye"));
    all_items.push_back(String("master-clock date and hour of sight"));
    all_items.push_back(String("use of stopwatch"));
    all_items.push_back(String("stopwatch reading"));
    all_items.push_back(String("label"));

    items.push_back(all_items[0]);
    items.push_back(all_items[2]);
    items.push_back(all_items[3]);
    items.push_back(all_items[5]);
    items.push_back(all_items[6]);
    items.push_back(all_items[8]);

    //initiazlie the limb to a 'n/a' value
    limb.value = 'n';
    atmosphere.set();
    (related_route.value) = -1;

}

//this function simplifies the atmosphere between z=0 and z=eight of eye as a single layer, where within this layer the index of refracion is independent of z. Refine this in the future.
void Sight::compute_DH_dip(String prefix) {

    DH_dip.set(String("Dip correction"),
        -acos(atmosphere.n(Length(0.0)) / atmosphere.n(height_of_eye)
              * ((atmosphere.earth_radius.convert(LengthUnit_types[0]).value) / (((atmosphere.earth_radius) + (height_of_eye)).convert(LengthUnit_types[0]).value) )
              ), prefix);

}


bool Sight::compute_DH_refraction(String prefix) {

    gsl_integration_workspace* w = gsl_integration_workspace_alloc(1000);
    gsl_function F;
    double result, error;
    int status;
    bool check = true;


    F.function = &dH_refraction;
    F.params = this;

    /* cout << "Value = " << dH_refraction(1.0, &(*this)); */
    /* cin >> result; */

    status = gsl_integration_qags(&F, atmosphere.h.back().convert(LengthUnit_types[0]).value, atmosphere.h.front().convert(LengthUnit_types[0]).value, 0.0, epsrel, 1000, w, &result, &error);
    //status = GSL_FAILURE

    if (status == GSL_SUCCESS) {
        DH_refraction.set(String("refraction correction"), result, prefix);
    }else{
        check &= false;
        cout << prefix.value << RED << "GSL integration failed!\n" << RESET;
    }

    gsl_integration_workspace_free(w);

    return check;

}

//default constructor, which sets value to 0.0 by default and the unit to the first entry of LengthUnit_types
Length::Length() {

    value = 0.0;
    unit = LengthUnit(LengthUnit_types[0]);

}

//constructor which sets value to x  and the unit to the first entry of LengthUnit_types
Length::Length(double x) {

    value = x;
    unit = LengthUnit(LengthUnit_types[0]);

}


//constructor which sets value to x  and the unit to unit_in
Length::Length(double value_in, const LengthUnit& unit_in) {

    value = value_in;
    unit = unit_in;

}


//construct the Length *this frome time and speed, by setting it equal to time x speed and its unit equal to LengthUnit_types[0]. This method takes into account the units in which speed is expressed (which are stored into speed.unit)
Length::Length(Chrono time, Speed speed) {

    //conversion factor
    double c = 0.0;
    
    //consider all possible units in which speed is expressed
    switch (speed.unit.position_in_list(SpeedUnit_types)) {
            
        case 0: {
            //speed.unit = SpeedUnit_types[0]
            
            c = 1.0;
            
            break;
            
        }

        case 1: {
            //speed.unit = SpeedUnit_types[1]

            c = 1.0/nm_to_km;
            
            break;
            
        }
            
        case 2: {
            //speed.unit = SpeedUnit_types[2]
            
            //[m]/[s] = 1e-3 3600 [km]/[h] = 1e-3 3600 / nm_to_km [nm]/[h] = 1e-3 3600 / nm_to_km [kt]
            c = (1e-3)*60.0*60.0/nm_to_km;
            
            break;
            
        }


    }
    
    set(c * (time.get()) * (speed.value));
    unit.set(LengthUnit_types[0]);

}


//set the value of *this equal to x (expressed in units LengthUnit_types[0]). The unit is not modified
inline void Length::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(x);

    if(name != String("")){
        print(name, prefix, cout);
    }
    check(name, new_prefix);

}


//same as Length::set(String name, double x, [[maybe_unused]] String prefix)  but without printing out anything
inline void Length::set(double x) {
    
    value = x;
}


//set the value of *this equal to x and the units equal to unit_in
inline void Length::set(double value_in, const LengthUnit& unit_in) {
    
    value = value_in;
    unit = unit_in;
    
}


//convert *this to string with numerical precision precision
string Length::to_string(unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    output << fixed << value << " " << unit.value;
    
    return(output.str().c_str());
    
}


//convert *this to string by printing it in the unit of measure unit_in, with numerical precision precision
string Length::to_string(const LengthUnit& output_unit, unsigned int precision) {

    Length temp;
    
    temp = (*this);
    temp.convert_to(output_unit);
    
    return(temp.to_string(precision));

}


//print *this and its unit of measure
void Length::print(String name, String prefix, ostream& ostr) {
    
    unsigned int precision;

    //if I am printing to terminal, I print with display_precision. Otherwise, I print with (data_precision.value)
    if (ostr.rdbuf() == cout.rdbuf()) {
        precision = (display_precision.value);
    }
    else {
        precision = (data_precision.value);
    }

    if ((name.value) != "") {

        ostr << prefix.value << name.value << " = " << to_string(precision) << endl;
  
    }

}


//convert *this to unit of measure unit_in, set unit = unit_in and write the result in *this
inline void Length::convert_to(const LengthUnit& output_unit){
    
    //the value of this in units of measure LengthUnit_types[0]
    double value0 = 0.0;

    
    //1. convert *this to unit LengthUnit_types[0] and write the result in value_in_LengthUnit_types0
    switch (unit.position_in_list(LengthUnit_types)) {
            
        case 0:{
            //unit = LengthUnit_types[0]
            
            value0 = value;
            
            break;
            
        }

        case 1:{
            //unit = LengthUnit_types[1]
            
            value0 = value * m_to_nm;
            
            break;
            
        }
            
        case 2:{
            //unit = LengthUnit_types[2]
            
            value0 = value * ft_to_nm;
            
            break;
            
        }
            
    }
    
    
    //2. convert *this to unit output_unit and write the result in *this
    switch (String(output_unit).position_in_list(LengthUnit_types)) {
            
        case 0:{
            //output_unit = LengthUnit_types[0]
            
            value = value0;
            
            break;
            
        }

        case 1:{
            //output_unit = LengthUnit_types[1]
            
            value = value0 * nm_to_m;

            break;
            
        }
            
        case 2:{
            //output_unit = LengthUnit_types[2]
            
            value = value0 * nm_to_ft;

            break;
            
        }
            
    }
    
    unit = output_unit;
    
}


//same as convert_to, but it returns the result
inline Length Length::convert(const LengthUnit& output_unit){
    
    Length result;
    
    result = (*this);
    result.convert_to(output_unit);
    
    return result;
    
}


bool Sight::get_coordinates(Route* circle_of_equal_altitude, [[maybe_unused]] String prefix) {

    FileR file;
    stringstream filename, line_ins;
    string line, dummy, temp;
    int l, l_min, l_max;
    double MJD_tab[(unsigned int)N], GHA_tab[(unsigned int)N], d_tab[(unsigned int)N], sum;
    gsl_interp_accel* acc = gsl_interp_accel_alloc();
    gsl_spline* interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)), * interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));
    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Fetching ephemerides' data ...\n";

    if ((body.type.value) != "star") {
        filename << (wxGetApp().data_directory).value << body.name.value << ".txt";
    }
    else {
        filename << (wxGetApp().data_directory).value << "j2000_to_itrf93.txt";
    }
    temp = filename.str();


    file.set_name(String(temp.c_str()));
    if ((file.open(new_prefix)) && check_time_interval(new_prefix)) {
        //the file corresponding to this->body exists and the time of *this lies within the time interval of NASA JPL ephemerides data


        //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
        l_min = (int)(L * ((time.MJD) - MJD_min)) - (int)(N / 2.0);
        //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
        l_max = (int)(L * ((time.MJD) - MJD_min)) + (int)(N / 2.0);

        /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */

        //dummy read of file data
        for (l = 0; l < l_min; l++) {
            line.clear();
            getline(*(file.value), line);
        }


        if ((body.type.value) != "star") {
            //in this case I am getting the coordinate of a body with a non-zero size

            //if the body is not a star

            double r_tab[(unsigned int)N];
            gsl_spline* interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));

            for (; l < l_max; l++) {

                line.clear();
                line_ins.clear();

                getline(*(file.value), line);
                line_ins << line;
                cout << new_prefix.value << line << "\n";
                line_ins >> dummy >> dummy >> dummy >> GHA_tab[l - l_min] >> d_tab[l - l_min] >> r_tab[l - l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;

                MJD_tab[l - l_min] = ((double)(l - l_min)) / L;

            }

            file.close(new_prefix);

            //convert to radians and nm
            for (l = 0; l < N; l++) {

                //add minus sign because in JPL convention longitude is positive when it is E
                GHA_tab[l] *= (-1.0) * k;
                d_tab[l] *= k;
                r_tab[l] /= nm_to_km;

            }

            //remove discontinuous jumps in GHA to allow for interpolation
            for (sum = 0.0, l = 0; l < N - 1; l++) {
                //cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
                if (((GHA_tab[l] - sum) > 0.0) && (GHA_tab[l + 1] < 0.0)) {
                    sum += 2.0 * M_PI;
                }
                GHA_tab[l + 1] += sum;
            }

            if (gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; };

            if (gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }
            if (gsl_spline_init(interpolation_r, MJD_tab, r_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }


            cout << new_prefix.value << "Read values:\n";
            for (l = 0; l < N; l++) {
                cout << new_prefix.value << MJD_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
            }

            if (gsl_spline_eval_e(interpolation_GHA, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).lambda).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).lambda).normalize();
                (((*circle_of_equal_altitude).reference_position).lambda).print(String("GHA"), new_prefix, cout);
            }
            //(((*circle_of_equal_altitude).reference_position).lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);


            if (gsl_spline_eval_e(interpolation_d, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).phi).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).phi).normalize();
                (((*circle_of_equal_altitude).reference_position).phi).print(String("d"), new_prefix, cout);
            }
            //(((*circle_of_equal_altitude).reference_position).phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

            if (gsl_spline_eval_e(interpolation_r, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &(r.value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                if ((r.check(String("r"), new_prefix))) {
                    r.print(String("r"), new_prefix, cout);
                }
                else {
                    check &= false;
                }
            }
            //r.set("r", gsl_spline_eval(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

            gsl_spline_free(interpolation_r);


        }
        else {
            //in this case I am getting the coordinate of a body with a zero size (a star)

            //if the body is a star
            double phi3, phi2, phi1;

            for (; l < l_max; l++) {

                line.clear();
                line_ins.clear();

                getline(*(file.value), line);
                line_ins << line;
                cout << new_prefix.value << line << "\n";
                line_ins >> dummy >> dummy >> dummy >> phi3 >> phi2 >> phi1;

                phi1 *= k;
                phi2 *= k;
                phi3 *= k;

                d_tab[l - l_min] = asin(cos(phi2) * sin((body.d.value)) - cos((body.d.value)) * cos(phi1) * sin((body.RA.value)) * sin(phi2) + cos((body.RA.value)) * cos((body.d.value)) * sin(phi1) * sin(phi2));

                GHA_tab[l - l_min] = atan((-cos(phi3) * sin((body.d.value)) * sin(phi2) - cos((body.RA.value)) * cos((body.d.value)) * (-cos(phi2) * cos(phi3) * sin(phi1) - cos(phi1) * sin(phi3)) - cos((body.d.value)) * sin((body.RA.value)) * (cos(phi1) * cos(phi2) * cos(phi3) - sin(phi1) * sin(phi3))) / (sin((body.d.value)) * sin(phi2) * sin(phi3) + cos((body.d.value)) * sin((body.RA.value)) * (cos(phi3) * sin(phi1) + cos(phi1) * cos(phi2) * sin(phi3)) + cos((body.RA.value)) * cos((body.d.value)) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))));
                if ((sin((body.d.value)) * sin(phi2) * sin(phi3) + cos((body.d.value)) * sin((body.RA.value)) * (cos(phi3) * sin(phi1) + cos(phi1) * cos(phi2) * sin(phi3)) + cos((body.RA.value)) * cos((body.d.value)) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))) < 0.0) {
                    GHA_tab[l - l_min] += M_PI;
                }
                GHA_tab[l - l_min] = GHA_tab[l - l_min] - 2.0 * M_PI * floor(GHA_tab[l - l_min] / (2.0 * M_PI));


                MJD_tab[l - l_min] = ((double)(l - l_min)) / L;

            }

            file.close(new_prefix);



            //remove discontinuous jumps in GHA to allow for interpolation
            for (sum = 0.0, l = 0; l < N - 1; l++) {
                //cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
                if (((GHA_tab[l] - sum) > M_PI) && (GHA_tab[l + 1] < M_PI)) {
                    sum += 2.0 * M_PI;
                }
                GHA_tab[l + 1] += sum;
            }

            cout << new_prefix.value << "Read values:\n";
            for (l = 0; l < N; l++) {
                cout << new_prefix.value << MJD_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
            }

            if (gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }
            if (gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }


            if (gsl_spline_eval_e(interpolation_GHA, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).lambda).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).lambda).normalize();
                (((*circle_of_equal_altitude).reference_position).lambda).print(String("GHA"), new_prefix, cout);
            }

            if (gsl_spline_eval_e(interpolation_d, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).phi).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).phi).normalize();
                (((*circle_of_equal_altitude).reference_position).phi).print(String("d"), new_prefix, cout);
            }

        }

        //set the length of the circle of equal altitude
        //(*circle_of_equal_altitude).l.set(String("length of circle of equal altitude"), 2.0*M_PI*Re*sin((*circle_of_equal_altitude).omega.value), new_prefix);

    }
    else {

        check &= false;
    }

    if (!check) {

        cout << prefix.value << RED << "Cannot fetch ephemerides' data!\n" << RESET;

    }
    else {

        cout << prefix.value << "...done\n";

    }

    gsl_interp_accel_free(acc);
    gsl_spline_free(interpolation_GHA);
    gsl_spline_free(interpolation_d);

    return check;

}







void Date::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << to_string() << "\n";

};

//this function sets (*this) to the current UTC date and time
void Time::set_current(void) {

    date.set_current();
    chrono.set_current();

}

//this function sets (*this) to the current UTC date
void Date::set_current(void) {

    (wxGetApp().local_time) = (boost::posix_time::second_clock::local_time());

    Y = (wxGetApp().local_time).date().year();
    M = (wxGetApp().local_time).date().month().as_number();
    D = (wxGetApp().local_time).date().day();

}



void Date::enter(String name, String prefix) {

    string input;
    String new_prefix;
    bool check;
    size_t pos;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    do {

        check = true;

        cout << prefix.value << "Enter " << name.value << " [YYYY MM DD], or press enter for current UTC date:";
        getline(cin, input);

        if (input.empty()) {

            cout << prefix.value << YELLOW << "Entered an empty date, setting it to current UTC date!\n" << RESET;
            set_current();
            print(String("entered date"), prefix, cout);

        }
        else {

            //check whether the string contains two ' '
            check &= (count(input.begin(), input.end(), ' ') == 2);

            if (check) {

                pos = (input).find(" ");

                //check whether year part is formatted correctly
                check &= check_unsigned_int(input.substr(0, pos).c_str(), &Y, false, 0, 0);

                if (check) {
                    //in this case, the year's format is valid

                    //I check whether Y is a leap year and compute days_per_month
                    check_leap_year();
                    if (Y_is_leap_year) {

                        days_per_month = days_per_month_leap;
                        cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET;

                    }
                    else {

                        days_per_month = days_per_month_common;
                        cout << new_prefix.value << "Entered a common year\n";

                    }

                    //now I am no longer interested in the year, the string runs from the month to days
                    input = (input.substr(pos + 1).c_str());

                    //find the position of the second ' '
                    pos = input.find(" ");

                    //check whether month part is formatted correctly
                    check &= check_unsigned_int(input.substr(0, pos).c_str(), &M, true, 1, 12 + 1);

                    if (check) {
                        //in this case the month part is formatted correctly

                        //now I am no longer interested in the month, the string runs from the days to the end of the string
                        input = (input.substr(pos + 1).c_str());

                        //check whether day part is formatted correctly
                        check &= check_unsigned_int(input.c_str(), &D, true, 1, days_per_month[M - 1] + 1);

                    }

                }

            }

        }

        if (!check) {

            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;

        }

    } while (!check);


    /* check_leap_year(); */
    /* if((Y_is_leap_year)){ */
    /*   (days_per_month) = days_per_month_leap; */
    /*   cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET; */
    /* }else{ */
    /*   (days_per_month) = days_per_month_common; */
    /*   cout << new_prefix.value << "Entered a common year\n"; */
    /* } */

    //enter_unsigned_int(&M, true, 1, 12+1, String("MM"), prefix);

    //enter_unsigned_int(&D, true, 1, days_per_month[M-1]+1, String("DD"), prefix);

}


string Date::to_string(void) {

    stringstream output;

    output << Y << "-";
    if (M < 10) { output << 0; }
    output << M << "-";
    if (D < 10) { output << 0; }
    output << D;

    return (output.str().c_str());

}



//this function adjusts the width of a wxComboBox according to its largest entry
void AdjustWidth(wxComboBox* control) {

    unsigned int i;
    int max_width, width, additional;

    //this is the additional width occupied by the sqare with the arrow
    control->GetTextExtent(wxString("-----"), &additional, NULL);

    for (max_width = 0, i = 0; i < (control->GetCount()); i++) {

        control->GetTextExtent(control->GetString(i), &width, NULL);

        if (width > max_width) {
            max_width = width;
        }

    }

    control->SetMinSize(wxSize(max_width + additional, -1));

}

ChartPanel::ChartPanel(ChartFrame* parent_in, const wxPoint& position, const wxSize& size) : wxPanel(parent_in, wxID_ANY, position, size, wxTAB_TRAVERSAL, wxT("")) {

    parent = parent_in;

}


//get the datapoints of coastlines and store them into parent->coastline_polygons_now
void ChartFrame::GetCoastLineData_3D(void) {

    unsigned long long int i, j;
    //integer values of min/max lat/lon to be extractd from p_coastline
    int/*  i_adjusted = 0, j_adjusted = 0,*/ i_min, i_max, j_min, j_max;
//    double /*the cosine of the angle between the vector with latitude and longitude i, j (see below) and the vector that connects the center ofr the Earth to circle_observer.reference_position*/cos;
    PositionProjection temp;
    wxPoint q;
    Cartesian r, s;
    Position u;


    //set i_min/max, j_min/max
    i_min = floor(K * (phi_min.normalize_pm_pi_ret().value));
    i_max = ceil(K * (phi_max.normalize_pm_pi_ret().value));

    if ((lambda_min == 0.0) && (lambda_max == 0.0)) {
        //in this case,Set_lambda_phi_min_max found out that circle_observer spans all longitudes, thus I set

        j_min = 0;
        j_max = 360;

    }
    else {
        //in this case, Set_lambda_phi_min_max found out that there are two finite longitudes which encircle circle_observer, thus I set

        if ((lambda_min < M_PI) && (lambda_max > M_PI)) {

            j_min = floor(K * (lambda_max.value));
            j_max = 360 + ceil(K * (lambda_min.value));

        }
        else {

            j_min = floor(K * (lambda_max.value));
            j_max = ceil(K * (lambda_min.value));

        }

    }
    
    if(i_min < floor_min_lat){i_min = floor_min_lat;}
    if(i_max > ceil_max_lat){i_max = ceil_max_lat;}

    
    if ((parent->show_coastlines) == Answer('y', String(""))) {
        
        unsigned long long int  l, p, m, every, n_added_polygons;
        bool new_polygon;
        
        
        //go through coastline_polygons_map and fetch the polygons that fall within rectangle_observer and store their ids into coastline_polygons_area_observer
        for(parent->coastline_polygons_area_observer.clear(), i=i_min-floor_min_lat; i<i_max-floor_min_lat; i++) {
            for(j=j_min; j<j_max; j++) {
                
                for(l=0; l<(parent->coastline_polygons_map)[i][j % 360].size(); l++){
                    parent->coastline_polygons_area_observer.push_back((parent->coastline_polygons_map)[i][j % 360][l]);
                }
                
            }
        }
        //the procedure above may lead to duplicates into coastline_polygons_area_observer -> delete them
        delete_duplicates(&(parent->coastline_polygons_area_observer));
        
        
        //count the total number of points included in the polygons of coastline_polygons_area_observer and store them in m
        //set every in such a way that the total number of plotted points is n_points_plot_coastline_3D, no matter what the size of rectangle_observer
        //        for(m=0, i=0; i<parent->coastline_polygons_area_observer.size(); i++) {
        //            for(j=0; j<(parent->coastline_polygons_Cartesian)[(parent->coastline_polygons_area_observer)[i]].size(); j++){
        //                
        //                if(((draw_panel->*(draw_panel->CartesianToProjection))((parent->coastline_polygons_Cartesian)[(parent->coastline_polygons_area_observer)[i]][j], NULL, false))){
        //                    m++;
        //                }
        //                
        //            }
        //        }
        //        every = ((unsigned long long int)(((double)m) / ((double)(wxGetApp().n_points_plot_coastline_3D.value))));
        //        if(every==0){every = 1;}
        
        //this is a computationally efficient way of estimating every: the number of coastline points falling within circle_observer is estimated as (parent->n_all_coastline_points)*(draw_panel->circle_observer.omega.value))/M_PI), and every is set accordingly in such a way that, for every circle_observer, the number of plotting points is n_points_plot_coastline_3D
        every = ((unsigned long long int)(((parent->n_all_coastline_points)*(draw_panel->circle_observer.omega.value))/M_PI) / ((double)(wxGetApp().n_points_plot_coastline_3D.value)));
        if(every==0){every = 1;}
        
        
        
        for(p=0, i=0, l=0, n_added_polygons=0, polygon_position_now.clear(); i<parent->coastline_polygons_area_observer.size(); i++) {
            //run through polygons
            
            new_polygon=true;
            n_added_polygons++;
            if(n_added_polygons > polygon_position_now.size()){
                polygon_position_now.resize(n_added_polygons);
            }
            polygon_position_now[n_added_polygons-1] = l;
            
            //the id of the polygon that is being added, i.e. , the # of the polygon as entry of coastline_polygons_Position
            m = (parent->coastline_polygons_area_observer)[i];
            
            for(j=p; j<(parent->coastline_polygons_Cartesian)[m].size(); j+=every){
                //run through points in a polygon
                
                if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[m][j], &q, false)){
                    
                    coastline_polygons_now[l++] = q;
                    new_polygon = false;
                    
                }else{
                    //(parent->coastline_polygons_Position)[i][j] is not a valid point -> I start a new polygon
                    
                    if(!new_polygon){
                        
                        //updated polygon_position_now with the position of the new polygon
                        new_polygon = true;
                        n_added_polygons++;
                        if(n_added_polygons > polygon_position_now.size()){
                            polygon_position_now.resize(n_added_polygons);
                        }
                        polygon_position_now[n_added_polygons-1] = l;
                        
                    }

                }
                
            }
            
            p = j - ((parent->coastline_polygons_Cartesian[m]).size());

        }
        
        if(n_added_polygons+1 > polygon_position_now.size()){
            polygon_position_now.resize(n_added_polygons+1);
        }
        polygon_position_now.back() = l;

        
        
//        every = ((unsigned long long int)(((double)(parent->n_all_coastline_points)) * (  draw_panel->circle_observer.omega.value ) / ( draw_panel->circle_observer_0.omega.value ) ) / ((double)(wxGetApp().n_points_plot_coastline_3D.value)));
//        if(every==0){every = 1;}
        
        
        

//        for(p=0, i=0, l=0; i<parent->coastline_polygons_Cartesian.size(); i++) {
//            //run through polygons
//            
//            polygon_position_now[i] = 0;
//            for(j=p; j<(parent->coastline_polygons_Cartesian[i]).size(); j+=every){
//                //run through points in a polygon
//
//                
//                if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[i][j], &q, false)) {
//                    
//                    coastline_polygons_now[l++] = q;
//                    polygon_position_now[i]++;
//
//                }
//                
//                
//            }
//            
//            p = j - ((parent->coastline_polygons_Cartesian[i]).size());
//
//        }
        
        //set r
//        draw_panel->circle_observer.reference_position.getCartesian(String(""), &r, String(""));
        
//        //
//        for (n=0, i = i_min; i < i_max; i++) {
//            for (j = j_min; j < j_max; j++) {
//                
//                
//                if ((draw_panel->AdjustLatitudeLongitude3D(i, j, &i_adjusted, &j_adjusted))) {
//                    
//                    n += (parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360].size();
//                    
//                }
//                
//                
//            }
//            
//        }
        
//        every = (unsigned long long int)(((double)n) / ((double)(parent->data->n_points_plot_coastline_3D.value)));
//        n_points_per_cell = ((unsigned long long int)((double)n)/((double)n_cells));
//        
//        for (polygon_position_now=0, p=0, i = i_min; i < i_max; i++) {
//            for (j = j_min; j < j_max; j++) {
//                
//                if ((draw_panel->AdjustLatitudeLongitude3D(i, j, &i_adjusted, &j_adjusted))) {
//
//                    
////                    //n =  how many datapoints are in data_x[i][j] and in data_y[i][j]
////                    n = ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size();
//                    
//                
//                    //set s
//                    u.phi.set(k * ((double)i));
//                    u.lambda.set(k * ((double)j));
//                    u.getCartesian(String(""), &s, String(""));
//                    //compute cos
//                    gsl_blas_ddot((r.r), (s.r), &cos);
//                    if (cos == 0.0) { cos = 1.0; }
//                    
//                    
//                    //I plot every 'every_ij' data points. I include the factor 1/cos in such a way that the farther the point (i,j) from circle_observer.reference_position, the less data points I plot, because plotting more would be pointless. In this way, points (i,j) which are close to circle_observer.reference_position (which are nearly parallel to the plane of the screen and thus well visible) are plotted with a lot of points, and the other way around
////                    every = (unsigned long)(((double)n) / ((double)(((parent->data)->n_points_plot_coastline_3D).value)) * ((double)n_points_grid) / cos);
////                    if (every == 0) { every = 1; }
//                    every_ij =
//                    ceil(((double)every)/cos * ((double)((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size())/
//                    (((double)n)/((double)n_cells)));
//                    if(every_ij == 0){
//                        every_ij = 1;
//                    }
//                    
//                    
//                    
//                    //run over data_x)[i - floor_min_lat][j % 360] by picking one point every every points
//                    for (l = min(p, ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size() - n_points_per_cell); l < ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size(); l += every_ij) {
//                        
//                        if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360][l], &q, false)) {
//                            
//                            coastline_polygons_now[polygon_position_now++] = q;
//                            
//                        }
//             
//                    }
//                    
//                    p = l - ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size();
//
//                    
//                }
//                
//                
//            }
//            
//        }
        
    }

}


//this function efficiently reads coastline data stored in data_x in the interval of latitudes lambda_min, lambda_max, phi_min, phi_max, and writes this data x and y, writing n_points points at the most. This data is stored into parent->coastline_polygons_now 
void ChartFrame::GetCoastLineData_Mercator(void) {

    int i_min = 0, i_max = 0, j_min = 0, j_max = 0;
    unsigned long long int i, j;
    wxPoint q;

//    //transform the values i_min, i_max in a format appropriate for GetCoastLineData: normalize the minimal and maximal latitudes in such a way that they lie in the interval [-pi, pi], because this is the format which is taken by GetCoastLineData
//    phi_min.normalize_pm_pi();
//    phi_max.normalize_pm_pi();
//
//
    if ((lambda_min < M_PI) && (lambda_max > M_PI)) {

        j_min = floor(K * (lambda_max.value));
        j_max = ceil(K * ((lambda_min.value) + 2.0*M_PI));

    }else {

        if (lambda_min > lambda_max) {

            j_min = floor(K * (lambda_max.value));
            j_max = ceil(K * (lambda_min.value));

        }
        else {

            j_min = floor(K * (lambda_max.value));
            j_max = ceil(K * ((lambda_min.value) + 2.0*M_PI));

        }

    }

    i_min = floor(K * (phi_min.normalize_pm_pi_ret().value));
    i_max = floor(K * (phi_max.normalize_pm_pi_ret().value));


    if ((parent->show_coastlines) == Answer('y', String(""))) {
        
//        PositionProjection p_SW, p_NE, p_SW0, p_NE0;
        unsigned long long int n_added_polygons, l, p, m, every;
        bool new_polygon;
        
//        ( ((phi_max.normalize_pm_pi_ret() - phi_min.normalize_pm_pi_ret()).value)*((lambda_max.normalize_pm_pi_ret() - lambda_min.normalize_pm_pi_ret()).value) ) / ( (ceil_max_lat - floor_min_lat)*2*M_PI );
        
        /*
         the number of points plotted is [number of coastline data points in lambda_min ... phi_max] / every = n_points_plot_coastline_Mercator ->
         every = [number of coastline data points in x_min ... y_max] / n_points_plot_coastline_Mercator ~
         ( (x) * [total number of coastline data points] / n_points_plot_coastline_Mercator
         */
        
//        p_SW.SetMercator(Position(Angle(0.0), phi_min));
//        p_NE.SetMercator(Position(Angle(0.0), phi_max));
//        p_SW0.SetMercator(Position(Angle(0.0), Angle(k*floor_min_lat)));
//        p_NE0.SetMercator(Position(Angle(0.0), Angle(k*ceil_max_lat)));
        
        
        //        double t;
        //        unsigned long long int n_points_in_chart;
        
        //        if((lambda_min < M_PI) && (lambda_max > M_PI)){p_NE.x += 2.0 * M_PI;}
        
        //        t = ( ( (draw_panel->x_span()) * (p_NE.y - p_SW.y) ) / ( (draw_panel->x_span_0) *(p_NE0.y - p_SW0.y) ) );
        
        //        n_points_in_chart = ((unsigned long long int)(((double)(parent->n_all_coastline_points)) * ( ( (draw_panel->x_span()) * (p_NE.y - p_SW.y) ) / ( (draw_panel->x_span_0)  *(p_NE0.y - p_SW0.y) ) )));
        
  
        
        
        //go through coastline_polygons_map and fetch the polygons that fall within rectangle_observer and store their ids into coastline_polygons_area_observer
        for(parent->coastline_polygons_area_observer.clear(), i=i_min-floor_min_lat; i<i_max-floor_min_lat; i++) {
            for(j=j_min; j<j_max; j++) {
                
                for(l=0; l<(parent->coastline_polygons_map)[i][j % 360].size(); l++){
                    parent->coastline_polygons_area_observer.push_back((parent->coastline_polygons_map)[i][j % 360][l]);
                }
                
            }
        }
        //the procedure above may lead to duplicates into coastline_polygons_area_observer -> delete them
        delete_duplicates(&(parent->coastline_polygons_area_observer));
        
        //count the total number of points included in the polygons of coastline_polygons_area_observer and store them in m
        //set every in such a way that the total number of plotted points is n_points_plot_coastline_Mercator, no matter what the size of rectangle_observer
        for(m=0, i=0; i<parent->coastline_polygons_area_observer.size(); i++) {
            for(j=0; j<(parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]].size(); j++){
                
                if((draw_panel->*(draw_panel->ProjectionToDrawPanel))((parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]][j], NULL, false)){
                
//                if(draw_panel->ProjectionToDrawPanel_Mercator((parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]][j], &q, false)){
                    m++;
                }
                
            }
        }
        every = ((unsigned long long int)(((double)m) / ((double)(wxGetApp().n_points_plot_coastline_Mercator.value))));
        if(every==0){every = 1;}
        
//        every = ((unsigned long long int)(((double)(parent->n_all_coastline_points)) * ( ( (draw_panel->x_span()) * (p_NE.y - p_SW.y) ) / ( (draw_panel->x_span_0)  *(p_NE0.y - p_SW0.y) ) ) / ((double)(wxGetApp().n_points_plot_coastline_Mercator.value))));
//        if(every==0){every = 1;}

        
        for(p=0, i=0, l=0, n_added_polygons=0, polygon_position_now.clear(); i<parent->coastline_polygons_area_observer.size(); i++) {
            //run through polygons
            
            new_polygon=true;
            n_added_polygons++;
            if(n_added_polygons > polygon_position_now.size()){
                polygon_position_now.resize(n_added_polygons);
            }
            polygon_position_now[n_added_polygons-1] = l;
            
            //the id of the polygon that is being added, i.e. , the # of the polygon as entry of coastline_polygons_Position
            m = (parent->coastline_polygons_area_observer)[i];
            
            for(j=p; j<(parent->coastline_polygons_Mercator)[m].size(); j+=every){
                //run through points in a polygon
                
                if((draw_panel->*(draw_panel->ProjectionToDrawPanel))((parent->coastline_polygons_Mercator)[m][j], &q, false)){
                
//                if ((draw_panel->GeoToDrawPanel)((parent->coastline_polygons_Position)[m][j], &q, false)){
                    //(parent->coastline_polygons_Position)[i][j] is a valid point
                    
                    coastline_polygons_now[l++] = q;
                    new_polygon = false;
                    
                }else{
                    //(parent->coastline_polygons_Position)[i][j] is not a valid point -> I start a new polygon
                    
                    if(!new_polygon){
                        
                        //updated polygon_position_now with the position of the new polygon
                        new_polygon = true;
                        n_added_polygons++;
                        if(n_added_polygons > polygon_position_now.size()){
                            polygon_position_now.resize(n_added_polygons);
                        }
                        polygon_position_now[n_added_polygons-1] = l;
                        
                    }

                }
                
            }
            
            p = j - ((parent->coastline_polygons_Position[m]).size());

        }
        
        if(n_added_polygons+1 > polygon_position_now.size()){
            polygon_position_now.resize(n_added_polygons+1);
        }
        polygon_position_now.back() = l;

        
    }
    
}


void ChartFrame::SetIdling(bool b) {

    idling = b;

}

//I call this function when I know that all GUI fields are properly filled in ChartFrame, and thus set the non GUI Angle objects relative to the Euler angles for the rotation of the 3D earth,  and draw everything
void ChartFrame::AllOk(void) {

    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

}


//same as ChartFrame::AllOk(void), but with an event argument, so this method can be triggered from an event
template<class T> void ChartFrame::AllOk(T& event) {

    AllOk();

}


//enable all GUI fields (buttons, slider, etc) in *this if enable  = true, and disable them otherwise
void ChartFrame::EnableAll(bool enable){
    
    button_reset->Enable(enable);
    button_up->Enable(enable);
    button_down->Enable(enable);
    button_left->Enable(enable);
    button_right->Enable(enable);
    slider->Enable(enable);
    projection->Enable(enable);
    
}


//constructs a color object by setting no arguments
Color::Color() : wxColour() {

}


//constructs a color object by setting no arguments
Color::Color(wxColour input) : wxColour(input) {

}


//constructs a color objct by setting its rgb values to red, green, blue
Color::Color(unsigned char red, unsigned char green, unsigned char blue) : wxColour(red, green, blue) {

}

//constructs a color objct by setting its rgb values to red, green, blue and transparency (alpha)
Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : wxColour(red, green, blue, alpha) {

}

//converts *this to RGB format
int Color::ToRGB(void) {

    int i, output;

    //convert this to BRG format which the wxWidgets function GetRGB() and then shift its bits so as to convert it to RGB format
    for (output = 0, i = 0; i < 3; i++) {

        output ^= ((((this->GetRGB()) >> 8 * i) & hexadecimal_mask) << 8 * (3 - (i + 1)));

    }

    return output;

}


//constructs a Bitmap object by setting no arguments
Bitmap::Bitmap() : wxBitmap() {

}

//constructs a Bitmap object by loading it from path path and rescaling it to size, by keeping its proportions
Bitmap::Bitmap(String path, wxSize size) :

#ifdef __APPLE__
    //I am on apple operating system->I set the bitmap from the file path

    wxBitmap(RescaleProportionally(wxBitmap(path.value, wxBITMAP_TYPE_PNG).ConvertToImage(), size)) {}

#endif

#ifdef _WIN32
//I am on windows operating system-> I load the bitmap from the windows resources

wxBitmap(wxBitmap(wxString(path.filename_without_folder_nor_extension(String("")).value), wxBITMAP_TYPE_PNG_RESOURCE).ConvertToImage().Scale(size.GetWidth(), size.GetHeight())) {}

#endif


//construct a StaticBitmap object by assignign to it the parent parent, loading it from path path and rescaling it to size
StaticBitmap::StaticBitmap(wxWindow* parent, String path, [[maybe_unused]] wxSize size) : wxStaticBitmap(parent, wxID_ANY, wxNullBitmap) {

    FileR file;

    file.set_name(path);

#ifdef __APPLE__
    //I am on apple operating system

    SetBitmap(
        Bitmap(file.name.value,
            ToDIP(wxSize(
                (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
            ))
        )
    );

#endif

#ifdef _WIN32
    //I am on windows operating system

    wxBitmap temp;

    temp = wxBitmap(wxString(file.name_without_folder_nor_extension.value), wxBITMAP_TYPE_PNG_RESOURCE);
    temp = wxBitmap(temp.ConvertToImage().Scale(size.GetWidth(), size.GetHeight()));
    SetBitmap(temp);

#endif

}


ChartFrame::ChartFrame(ListFrame* parent_input, Projection projection_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    unsigned long long int i, j;
    String new_prefix, default_projection, color;
    //empty wxStaticTexts to fill the empty spaces of the wxGridSizer sizer_buttons
    StaticText* empty_text_1, * empty_text_2, * empty_text_3, * empty_text_4, * empty_text_5;
    wxCommandEvent dummy_event;
    //a wxSizerFlags object to insert stuff into sizers
    wxSizerFlags flags;

    parent = parent_input;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    flags.Center();

    //when a ChartFrame is created, the chart is not being dragged
    dragging_chart = false;

    //read lambda_min, ...., phi_max from file_init
    lambda_min.read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    lambda_max.read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    phi_min.read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    phi_max.read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), new_prefix);


    this->Bind(wxEVT_CLOSE_WINDOW, &ChartFrame::OnPressCtrlW<wxCloseEvent>, this);

    mouse_scrolling = false;
    //set the zoom factor to 1 for the initial configuration of the projection
    zoom_factor.set(1.0);
    //read zoom_factor_max from file_init
    wxGetApp().zoom_factor_max.read_from_file_to(String("maximal zoom factor"), (wxGetApp().path_file_init), String("R"), String(""));
    idling = false;
    unset_idling = new UnsetIdling<ChartFrame>(this);
    
    //coastline_polygons_now/before and polygon_position_now/before are resized to their maximum possible value
    for(i=0, j=0; i<parent->coastline_polygons_Position.size(); i++) {
        j += (parent->coastline_polygons_Position[i].size());
    }
    coastline_polygons_now.resize(j);
    polygon_position_now.resize(parent->coastline_polygons_Position.size());
    coastline_polygons_before.resize(j);
    polygon_position_before.resize(parent->coastline_polygons_Position.size());

    print_error_message = new PrintMessage<ChartFrame, UnsetIdling<ChartFrame> >(this, unset_idling);

    panel = new ChartPanel(this, wxDefaultPosition, wxDefaultSize);
    draw_panel = new DrawPanel(panel, wxDefaultPosition, wxDefaultSize);

    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_slider = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxGridSizer(3, 3, 0, 0);
    
    //initialize the variable neededed for slider
    //allocate the slider
    slider = new wxSlider(panel, wxID_ANY, 1, 1, (int)((wxGetApp().zoom_factor_max).value), wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);

    //sets the coefficients for the function which relates the zoom factor to the slider value: read from file (wxGetApp().e_zoom) and set (wxGetApp().a_zoom), (wxGetApp().b_zoom)
    (wxGetApp().e_zoom).read_from_file_to(String("exponent zoom"), (wxGetApp().path_file_init), String("R"), String(""));
    (wxGetApp().a_zoom).set((-1.0 + ((wxGetApp().zoom_factor_max).value)) / (-1.0 + pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value)));
    (wxGetApp().b_zoom).set((pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value) - ((wxGetApp().zoom_factor_max).value)) / (-1.0 + pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value)));
 

    //text field showing the current value of the zoom slider
    chart_scale = new StaticText(panel, wxS(""), wxDefaultPosition, wxDefaultSize, 0);
    observer_height = new StaticLengthField<ChartFrame>(panel, &(draw_panel->d));

    //navigation buttons
    button_up = new wxButton(panel, wxID_ANY, wxT("N"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_down = new wxButton(panel, wxID_ANY, wxT("S"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_left = new wxButton(panel, wxID_ANY, wxT("W"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_right = new wxButton(panel, wxID_ANY, wxT("E"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
//    button_reset = new wxButton(panel, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize);
    
    
    button_reset = new wxBitmapButton(
                                      panel,
                                      wxID_ANY,
                                      Bitmap(wxGetApp().path_file_reset_icon, (wxGetApp().size_large_button) - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                      wxDefaultPosition,
                                      wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)),
                                      wxBU_EXACTFIT | wxSIMPLE_BORDER
                                      );
    
    //button to show list
    button_show_list = new wxBitmapButton(
                                          panel,
                                          wxID_ANY,
                                          Bitmap(wxGetApp().path_file_list_icon, (wxGetApp().size_large_button) - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                          wxDefaultPosition,
                                          wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)),
                                          wxBU_EXACTFIT | wxSIMPLE_BORDER
                                          );
    button_show_list->Bind(wxEVT_BUTTON, &MyApp::ShowList, &wxGetApp());
    button_show_list->SetToolTip(wxString("Show the list of sights, positions and routes"));

    projection = new ProjectionField<ChartFrame>(panel, &(wxGetApp().list_frame->data->recent_projections));
    projection->name->Bind(wxEVT_COMBOBOX, &DrawPanel::OnChooseProjection<wxCommandEvent>, draw_panel);
    projection->SetToolTip(String("Choose the projection used in the chart"));

    button_up->Bind(wxEVT_BUTTON, &ChartFrame::MoveNorth<wxCommandEvent>, this);
    button_down->Bind(wxEVT_BUTTON, &ChartFrame::MoveSouth<wxCommandEvent>, this);
    button_left->Bind(wxEVT_BUTTON, &ChartFrame::MoveWest<wxCommandEvent>, this);
    button_right->Bind(wxEVT_BUTTON, &ChartFrame::MoveEast<wxCommandEvent>, this);
    button_reset->Bind(wxEVT_BUTTON, &ChartFrame::ResetRenderAnimate<wxCommandEvent>, this);
    button_reset->SetToolTip("Reset the chart");

    //bind all the elemetns of *this to KeyDown method
    Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    panel->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    draw_panel->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    projection->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);

    draw_panel->Bind(wxEVT_KEY_DOWN, &DrawPanel::KeyDown, draw_panel);
    panel->Bind(wxEVT_KEY_DOWN, &DrawPanel::KeyDown, draw_panel);

    draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, draw_panel);
    draw_panel->Bind(wxEVT_RIGHT_DOWN, &DrawPanel::OnMouseRightDown, draw_panel);
    draw_panel->Bind(wxEVT_LEFT_DOWN, &DrawPanel::OnMouseLeftDown, draw_panel);
    draw_panel->Bind(wxEVT_LEFT_UP, &DrawPanel::OnMouseLeftUp, draw_panel);
    draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseDrag, draw_panel);
    draw_panel->Bind(wxEVT_MOUSEWHEEL, &DrawPanel::OnMouseWheel, draw_panel);

    slider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &ChartFrame::OnScroll<wxCommandEvent>, this);
    slider->Bind(wxEVT_LEFT_DOWN, &ChartFrame::OnMouseLeftDownOnSlider<wxMouseEvent>, this);
    slider->Bind(wxEVT_LEFT_UP, &ChartFrame::OnMouseLeftUpOnSlider<wxMouseEvent>, this);
    slider->SetToolTip(wxString("Zoom in or out"));

    empty_text_1 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_2 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_3 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_4 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_5 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);

    //when the ChartFrame is initialized with projection_in = "", I choose to draw either the Mercator or the 3D chart, by reading the name of the projection from file_init. I set the value of projection->name to either of these,
    if (projection_in == Projection("")) {
        //if the constructor has been called with an empty projection_in, I use the default projection by reading it from the init file.

        default_projection.read_from_file_to(String("default projection"), (wxGetApp().path_file_init), String("R"), String(""));
        projection->name->SetValue(wxString(default_projection.value));

    }
    else {
        //if the construtor has been called with projection_in non-empty, I set projection_in equal to projection_in

        projection->name->SetValue(wxString(projection_in.value));

    }
    projection->value_before_editing = projection->name->GetValue();

    //create a dummy_event and then call SetProjection(dummy_event) to set all objects according to the choice of the projeciton above.
    //    draw_panel->OnChooseProjection(dummy_event);
    draw_panel->SetProjection(dummy_event);

    //stores the x_min .. y_max, (size_chart.GetWidth()), height chart the first time that the chart is shown into x_min_0 ... height_chart_0
    (draw_panel->*(draw_panel->Set_x_y_min_max))();
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);
    (draw_panel->x_span_0) = (draw_panel->x_span());
    
    ResetRender(dummy_event);

    (draw_panel->width_chart_0) = (draw_panel->size_chart.GetWidth());
    (draw_panel->height_chart_0) = (draw_panel->size_chart.GetHeight());


    sizer_buttons->Add(empty_text_1, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_up, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_2, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_left, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_3, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_right, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_4, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_down, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_5, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

    sizer_slider->Add(slider, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_slider->Add(chart_scale, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    observer_height->LengthField<ChartFrame>::InsertIn(sizer_slider, flags);
    sizer_slider->Add(sizer_buttons, 0, wxALIGN_CENTER | wxALL, 0);
    sizer_slider->Add(button_reset, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    projection->InsertIn(sizer_slider, flags);
    sizer_slider->AddStretchSpacer(1);
    sizer_slider->Add(button_show_list, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));


    sizer_h->Add(draw_panel, 0, wxALIGN_TOP | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_h->Add(sizer_slider, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    //	sizer_v->Add(text_position_now, 0, wxALIGN_LEFT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
        //    sizer_v->Fit(panel);

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_v);
    panel->Fit();
    Fit();

    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //    CenterOnScreen();

}

//when a ChartFrame is closed, I destroy it, delete the respecive item in parent->chart_frames vector, and rename all the other ChartFrames in that vector to take into account the shifting of the CartFrame ids due to the Chartframe deletion
template<class T> void ChartFrame::OnPressCtrlW(T& event) {

    vector<ChartFrame*>::iterator i;
    unsigned int j;
    stringstream s;

    i = find((parent->chart_frames).begin(), (parent->chart_frames).end(), this);


    Destroy();  // you may also do:  event.Skip();

    (parent->chart_frames).erase(i);
    for (j = 0; j < (parent->chart_frames).size(); j++) {

        s.str("");
        s << "Chart #" << j + 1 << " - " << ((((parent->chart_frames)[j])->projection)->name)->GetValue() << " projection";

        ((parent->chart_frames)[j])->SetLabel(wxString(s.str().c_str()));

    }

    //given that one ChartFrame is gone, I show all ChartFrames and positions them properly on the screen
    wxGetApp().ShowCharts(event);
    wxGetApp().AnimateCharts();

}

//moves (makes slide) to the north the chart
template<class T> void ChartFrame::MoveNorth(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;


//    switch (((projection->name->GetValue()).ToStdString())[0]) {
    switch (position_in_vector(Projection((projection->name->GetValue()).ToStdString()), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;
        PositionProjection p_ceil_min, p_floor_max;

        //I set delta as a fraction of y_max - y_min
        delta = ((wxGetApp().relative_displacement).value) * ((draw_panel->y_max) - (draw_panel->y_min));

        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);


        if (((draw_panel->y_max) + delta < (p_floor_max.y)) && ((draw_panel->y_min) + delta > (p_ceil_min.y))) {
            //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart

            //update y_min, y_max according to the drag.
            (draw_panel->y_min) += delta;
            (draw_panel->y_max) += delta;

            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();


        }

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.b) += delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);



        break;

    }


    }

    //re-draw the charton
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//moves (makes slide) to the south the chart
//moves (makes slide) up the chart
template<class T> void ChartFrame::MoveSouth(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;

    switch (position_in_vector(Projection((projection->name->GetValue()).ToStdString()), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;
        PositionProjection p_ceil_min, p_floor_max;

        //I set delta as a fraction of y_max - y_min
        delta = ((wxGetApp().relative_displacement).value) * ((draw_panel->y_max) - (draw_panel->y_min));

        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);


        if (((draw_panel->y_max) - delta < (p_floor_max.y)) && ((draw_panel->y_min) - delta > (p_ceil_min.y))) {
            //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart

            //update y_min, y_max according to the drag.
            (draw_panel->y_min) -= delta;
            (draw_panel->y_max) -= delta;

            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();


        }

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.b) -= delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);

        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//moves (makes slide) to the west the chart
template<class T> void ChartFrame::MoveWest(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;


    switch (position_in_vector(Projection((projection->name->GetValue().ToStdString())), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;

        delta = ((wxGetApp().relative_displacement).value) * (draw_panel->x_span());

        //update lambda_min, lambda_max according to the drag.
        (lambda_min.value) += delta;
        (lambda_max.value) += delta;

        lambda_min.normalize();
        lambda_max.normalize();

        (draw_panel->*(draw_panel->Set_x_y_min_max))();

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.a) -= delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);



        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}



//moves (makes slide) to the east the chart
template<class T> void ChartFrame::MoveEast(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;

    switch (position_in_vector(Projection((projection->name->GetValue().ToStdString())), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;

        delta = ((wxGetApp().relative_displacement).value) * (draw_panel->x_span());

        //update lambda_min, lambda_max according to the drag.
        (lambda_min.value) -= delta;
        (lambda_max.value) -= delta;

        lambda_min.normalize();
        lambda_max.normalize();

        (draw_panel->*(draw_panel->Set_x_y_min_max))();

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.a) += delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);



        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;


    event.Skip(true);

}

//this method is called when a key is pressed
template<class T> void ChartFrame::KeyDown(T& event) {

    if (wxGetKeyState(WXK_CONTROL)) {
        //the command key (APPLE operating system) or the control key (WIN32 operating system) is down

        switch ((event.GetUnicodeKey())) {

        case 'Q': {
            //command (APPLE) or control (WIN32) + q key has been pressed

            wxGetApp().OnPressCtrlQ(event);

            break;

        }

        case 'W': {
            //command (APPLE) or control (WIN32) + w key has been pressed

            parent->OnPressCtrlW(event);

            break;

        }

        case 'S': {

            if (wxGetKeyState(WXK_SHIFT)) {

                //command (APPLE) or control (WIN32) + shift + s key has been pressed

                parent->OnPressCtrlShiftS(event);
            }
            else {
                //command (APPLE) or control (WIN32) + s key has been pressed

                parent->OnPressCtrlS(event);

            }

            break;

        }

        }

    }


    event.Skip(true);

}


//reset the chart to its starting configuration for x_min ... y_max
template<class T> void ChartFrame::Reset(T& event) {

    idling = false;
    (draw_panel->idling) = false;

    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        //read lambda_min, ...., phi_max from file_init
        lambda_min.read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
        lambda_max.read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
        phi_min.read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
        phi_max.read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
        draw_panel->Set_x_y_min_max_Mercator();
        ComputeZoomFactor_Mercator(draw_panel->x_span());

        //reset the chart boundaries to the initial ones
        //        (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();

    }

    if ((projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {
        //reset d abd the earth orientation to the initial one and set the zoom factor accordingly

        parent->circle_observer_0.omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), String(""));
        zoom_factor.set(1.0);
        ComputeZoomFactor_3D();

        (draw_panel->rotation_0).read_from_file_to(String("rotation 0"), (wxGetApp().path_file_init), String("R"), String(""));
        draw_panel->rotation.set(draw_panel->rotation_0);
        draw_panel->Set_x_y_min_max_3D();
        (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();

    }

    //now that x_min ... y_max have been set, I set x_min_0 ... equal to x_min ...
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);

 

    event.Skip(true);

}


//call Reset and Render everything
template<class T> void ChartFrame::ResetRender(T& event) {
    
    Reset<T>(event);
    
    (draw_panel->*(draw_panel->PreRender))();
     //now that (size_chart.GetWidth()) and (size_chart.GetHeight()) have been set, I set width_chart_0 and height_chart_0 equal to width_chart and (size_chart.GetHeight())
     (draw_panel->width_chart_0) = (draw_panel->size_chart.GetWidth());
     (draw_panel->height_chart_0) = (draw_panel->size_chart.GetHeight());

     draw_panel->CleanAndRenderAll();
     
     draw_panel->FitAll();
     UpdateSlider();
//     Animate();
    
    event.Skip(true);

}


template<class T> void ChartFrame::ResetRenderAnimate(T& event) {
    
    ResetRender(event);
    Animate();

}




//makes a nice animation to present *this, by dragging the center of the chart to the desired Position from a Position on the antipodes
void ChartFrame::Animate(void){
    
    //the transport handler used to transport the chart in *this
    ChartTransportHandler< UnsetIdling<ListFrame> >* chart_transport_handler;
        
    //allocate chart_transport_handler and set the starting Position and the Route for the transport
    switch (position_in_vector(Projection((projection->name->GetValue().ToStdString())), Projection_types)) {
            
        case 0: {
            //I am using Projection_types[0]
            
            PositionProjection q_NE, q_SW, q_center;
            Position p;
            
            q_NE.NormalizeAndSetMercator(Position(lambda_max, phi_max));
            q_SW.NormalizeAndSetMercator(Position(lambda_min, phi_min));
            q_center = (q_NE + q_SW)/2;
            
            (draw_panel->*(draw_panel->ProjectionToGeo))(q_center, &p);
            
            chart_transport_handler = new ChartTransportHandler< UnsetIdling<ListFrame> >(
                                                                this,
                                                                Route(
                                                                      Route_types[0],
                                                                      p.antipode_lambda(),
                                                                      p
                                                                      ),
                                                                Double(1.0),
                                                                      parent->unset_idling
                                                                );

            //trigger the animation
            chart_transport_handler->operator()();
            
            
            break;
            
        }
            
        case 1: {
            //I am using Projection_types[1]
            
            chart_transport_handler = new ChartTransportHandler< UnsetIdling<ListFrame> >(
                                                                this,
                                                                Route(
                                                                      Route_types[1],
                                                                      draw_panel->circle_observer.reference_position.half(),
                                                                      draw_panel->circle_observer.reference_position
                                                                      ),
                                                                Double(1.0),
                                                                      parent->unset_idling
                                                                );

            //trigger the animation
            chart_transport_handler->operator()();
            
            break;
            
        }
            
    }
        
}





//updates the text in text_slider by writing in it the current value of the zoom factor, rounded to an integer for clarity
void ChartFrame::UpdateSliderLabel_Mercator(void) {

    stringstream s;
    Int scale_factor;
    
    //compute the chart_scale factor
    scale_factor.set( ((unsigned int)(
                   /*length of the NS edge of the plot area as measured on the surface of the earth, in  [nm]*/(((phi_max.normalize_pm_pi_ret().value) - (phi_min.normalize_pm_pi_ret().value)) * K * 60.0) / ( /*length of the NS edge of the plot area as shown on the screen of the computer, in [nm]*/((double)(draw_panel->size_plot_area.y))/((double)(wxGetApp().display.GetPPI().x)) * inch_to_km/nm_to_km ) )) );
    
    
    scale_factor.my_round(display_precision);
    

    s.str("");
    s << "1:" << scale_factor.to_string_spaces().value;
    chart_scale->SetLabel(s.str().c_str());
    
    //fir *this in order to account for the sliderlabal which has changed 
    Fit();
    

}

//update the text in text_slider in the 3D projection: for the 3D projection the altitude of the observer is written into observer_height, thus text_slider is set to empty
void ChartFrame::UpdateSliderLabel_3D(void) {

    chart_scale->SetLabel("");

}

//computes the zoom factor of the chart based on the currenct value of span_x. It returns true and writes the value in zoom_factor if the zooming factor is smaller than (wxGetApp().zoom_factor_max), and returns false otherwise
bool ChartFrame::ComputeZoomFactor_Mercator(double delta_x) {

    double temp;
    bool output;

    temp = ((double)((draw_panel->size_chart).GetWidth())) / ((double)(draw_panel->width_chart_0)) * ((draw_panel->x_max_0) - (draw_panel->x_min_0)) / delta_x;

    output = ((1.0 <= temp) && (temp <= (wxGetApp().zoom_factor_max.value)));

    if (output) {
        zoom_factor.set(temp);
    }

    return(output);

}

//returns true if zoom_factor is valid, false otherwise
bool ChartFrame::ComputeZoomFactor_3D(void) {

    bool output;

    output = ((1.0 <= (zoom_factor.value)) && ((zoom_factor.value) <= (wxGetApp().zoom_factor_max.value)));

    if (output) {

        draw_panel->circle_observer.omega.set((parent->circle_observer_0.omega.value) / (zoom_factor.value));

    }

    return(output);

}

//this function updates the slider according to the zoom factor of the chart: it sets the slider value to the integer value closest to zoom_factor
void ChartFrame::UpdateSlider(void) {

    int temp;

    //compute the zoom factor of the chart and write it into zoom_factor

    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        ComputeZoomFactor_Mercator((draw_panel->x_span()));

    }

    if ((projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {
        //is this necessary here ?
        ComputeZoomFactor_3D();

    }

    //a tentative value for the value of slizer
    temp = round(pow(((zoom_factor.value) - ((wxGetApp().b_zoom).value)) / ((wxGetApp().a_zoom).value), 1.0 / ((wxGetApp().e_zoom).value)));


    //if the tentative value exceeds the slider boundaries, I set it to the respective boundary
    if (temp > (slider->GetMax())) { temp = (slider->GetMax()); }
    if (temp < 1) { temp = 1; }

    //    cout << "\t\t\ttemp = " << temp << "\n";
    //    cout << "\t\t\tzoom_factor = " << (zoom_factor.value) << "\n";

    slider->SetValue(temp);

    (this->*UpdateSliderLabel)();

}



//#include "limb_field.cpp"





template<class T> void ChartFrame::OnMouseLeftDownOnSlider(T& event) {

    //mouse scrolling starts
    mouse_scrolling = true;

    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        (draw_panel->x_center_scrolling) = ((draw_panel->x_min) + (draw_panel->x_max)) / 2.0;
        (draw_panel->y_center_scrolling) = ((draw_panel->y_min) + (draw_panel->y_max)) / 2.0;

    }

    event.Skip(true);

}



template<class T> void ChartFrame::OnMouseLeftUpOnSlider(T& event) {

    //mouse scrolling ends
    mouse_scrolling = false;

    event.Skip(true);

}


template<class T> void ChartFrame::OnScroll(/*wxScrollEvent*/ T& event) {

    /*
     n = value of slider,
     z = zoom factor,

     1 <= z <= (wxGetApp().zoom_factor_max)
     1 <= n <= n_max

     n = ((double)(slider->GetValue()));
     n_max = ((double)(slider->GetMax()))

     z = (wxGetApp().a_zoom)*n^(wxGetApp().e_zoom) + (wxGetApp().b_zoom)
     n = log((z-(wxGetApp().b_zoom))/(wxGetApp().a_zoom))/(wxGetApp().e_zoom)


     (wxGetApp().a_zoom) = (-1 + (wxGetApp().zoom_factor_max))/(-1 + n_max^(wxGetApp().e_zoom));
     (wxGetApp().b_zoom) = (n_max^(wxGetApp().e_zoom) - (wxGetApp().zoom_factor_max))/(-1 + n_max^(wxGetApp().e_zoom));

     z = w/delta_x / (w_0/delta_x_0)

     delta_x = w/z/ (w_0/delta_x_0) = w*delta_x_0/(z*w_0)

     (size_chart.GetHeight())/(size_chart.GetWidth()) * x_span = (y_max-y_min);


     */


     //    cout << "Slider getvalue = " << ((double)(slider->GetValue())) << "\n";
     //     cout << "Zoom factor = " << ((double)zoom_factor.value) << "\n";

     //set zoom_factor from the value of slider
    zoom_factor.set(
        String(""),
        ((wxGetApp().a_zoom).value) * pow((slider->GetValue()), (wxGetApp().e_zoom).value) + ((wxGetApp().b_zoom).value)
        ,
        String("")
    );

    //if the resulting value of zoom_factor is outside the boundaries I set it back to the rspective boundary
    if (((zoom_factor.value) < 1.0) || ((slider->GetValue()) == (slider->GetMin()))) {

        (zoom_factor.value) = 1.0;

    }

    if (((zoom_factor.value) > (wxGetApp().zoom_factor_max.value)) || ((slider->GetValue()) == (slider->GetMax()))) {

        (zoom_factor.value) = (wxGetApp().zoom_factor_max.value);

    }

    
#ifdef WIN32
    //I am on WIN32 operating system: I will refresh the plot under the scroll operation, where I will wipe out the graphical objects in the former plot by drawing with background_color on top of them -> I need to keep track of the _before graphical objects and on the current _now graphical objects, and I do it here:
    
    //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
    polygon_position_before = polygon_position_now;
//    coastline_polygons_before.resize(coastline_polygons_now.size());
    copy_n(coastline_polygons_now.begin(), coastline_polygons_now.size(), coastline_polygons_before.begin());

    
    (draw_panel->position_plot_area_before) = (draw_panel->position_plot_area_now);
    draw_panel->grid_before.clear();
    (draw_panel->grid_before) = (draw_panel->grid_now);
    draw_panel->ticks_before.clear();
    (draw_panel->ticks_before) = (draw_panel->ticks_now);
    
    (draw_panel->parallels_and_meridians_labels_before) = (draw_panel->parallels_and_meridians_labels_now);
    (draw_panel->positions_parallels_and_meridians_labels_before) = (draw_panel->positions_parallels_and_meridians_labels_now);

    //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
    draw_panel->points_route_list_before.clear();
    (draw_panel->points_route_list_before) = (draw_panel->points_route_list_now);
    
    draw_panel->points_position_list_before.clear();
    (draw_panel->points_position_list_before) = (draw_panel->points_position_list_now);
    
    draw_panel->reference_positions_route_list_before.clear();
    (draw_panel->reference_positions_route_list_before) = (draw_panel->reference_positions_route_list_now);
    
#endif



    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        PositionProjection p_min, p_max;

        //update x_min, ..., y_max according to the zoom (scroll) and lambda_min, ..., phi_max
        (draw_panel->x_min) = ((double)((draw_panel->x_center_scrolling))) - (((double)(((draw_panel->size_chart).GetWidth()) * (draw_panel->x_span_0))) / ((double)(((zoom_factor.value) * (draw_panel->width_chart_0))))) / 2.0;
        (draw_panel->x_max) = ((double)((draw_panel->x_center_scrolling))) + (((double)(((draw_panel->size_chart).GetWidth()) * (draw_panel->x_span_0))) / ((double)(((zoom_factor.value) * (draw_panel->width_chart_0))))) / 2.0;
        (draw_panel->y_min) = ((double)((draw_panel->y_center_scrolling))) - (((double)(((draw_panel->size_chart).GetHeight()) * (draw_panel->x_span()))) / ((double)((draw_panel->size_chart).GetWidth()))) / 2.0;
        (draw_panel->y_max) = ((double)((draw_panel->y_center_scrolling))) + (((double)(((draw_panel->size_chart).GetHeight()) * (draw_panel->x_span()))) / ((double)((draw_panel->size_chart).GetWidth()))) / 2.0;

        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(max_lat)), &p_max, true);
        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(min_lat)), &p_min, true);

        if ((((draw_panel->y_max) <= (p_max.y)) && ((draw_panel->y_min) >= (p_min.y)) && ((draw_panel->x_span()) <= 2.0 * M_PI))) {

            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();
            //            ComputeZoomFactor_Mercator((draw_panel->x_span));

            (draw_panel->*(draw_panel->PreRender))();
            draw_panel->MyRefresh();
            draw_panel->FitAll();
            UpdateSlider();

        }
        else {
            //if the drag operation brings the chart out of the min and max latitude contained in the data files, I reset x_min, ..., y_max and the value of the slider to the values at the beginning of the drag, and set lambda_min, ..., phi_max accordingly.

            //uncomment this if you want to print an error message
            //print_error_message->SetAndCall(NULL, String("Error"), String("You moved the slider: Chart outside  boundaries! The chart must lie within the boundaries."));

            //I reset the chart to its original configuration
            Reset<T>(event);

        }

    }

    if ((projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {

        (draw_panel->circle_observer.omega) = ((parent->circle_observer_0.omega) / (zoom_factor.value));

        (draw_panel->*(draw_panel->PreRender))();
        draw_panel->MyRefresh();
        draw_panel->FitAll();

        UpdateSlider();

    }


    event.Skip(true);

}

//If slider->GetMin() <= slider_value <= slider->GetMax() it returns true, and sets slider to slider_value and updates the chart zoom accordingly. It returns false otherwise
bool ChartFrame::SetSlider(unsigned int slider_value) {

    if ((slider_value >= ((unsigned int)(slider->GetMin()))) && (slider_value <= ((unsigned int)(slider->GetMax())))) {

        wxCommandEvent dummy;

        if (!mouse_scrolling) {
            OnMouseLeftDownOnSlider(dummy);
        }
        slider->SetValue(slider_value);

        //call OnScroll to update evrything adter the change of the value of slider
        OnScroll(dummy);
        OnMouseLeftUpOnSlider(dummy);

        return true;

    }
    else {

        return false;

    }


}



//constructor of the struct, which initializes the Answer remove_related_route. If remove_related_route.value = 'y', then DeleteSight::operator() will delete both the sight and the related route. If remove_related_route.value = 'n', then it will remove the sight only.
DeleteSight::DeleteSight(ListFrame* f_in, Answer remove_related_route_in) {

    f = f_in;
    remove_related_route = remove_related_route_in;

}

ExistingRoute::ExistingRoute(ListFrame* f_in) {

    f = f_in;

}

NewRoute::NewRoute(ListFrame* f_in) {

    f = f_in;

}

AllRoutes::AllRoutes(ListFrame* f_in) {

    f = f_in;

}


SomeRoutes::SomeRoutes(ListFrame* f_in) {

    f = f_in;

}


template<class P> SetIdling<P>::SetIdling(P* parent_in) {

    parent = parent_in;

}

template<class P> UnsetIdling<P>::UnsetIdling(P* parent_in) {

    parent = parent_in;

}




AskRemoveRelatedSight::AskRemoveRelatedSight(ListFrame* parent_in) {

    parent = parent_in;

}

AskRemoveRelatedRoute::AskRemoveRelatedRoute(ListFrame* parent_in) {

    parent = parent_in;

}


SelectRoute::SelectRoute(ListFrame* parent_in) {

    parent = parent_in;

}


template<class P> FunctionOnPressOk<P>::FunctionOnPressOk(P* parent_in) {

    parent = parent_in;

}



void DeleteSight::operator()(wxCommandEvent& event) {

    //I remove the sight and the related route from  the non-GUI object data
    (f->data)->remove_sight(((unsigned int)i_sight_to_remove), remove_related_route, String(""));

    (f->listcontrol_sights)->set((f->data)->sight_list, false);
    (f->listcontrol_routes)->set((f->data)->route_list, false);

    //given that I called set in listcontrol_sights, no item is selected in listcontrol_sights, I call:
    (*(f->on_change_selection_in_listcontrol_sights))(event);

    if (remove_related_route == Answer('y', String(""))) {

        //given that I called set for listcontrol_routes, no item is selected in listcontrol_routes -> I call:
        (*(f->on_change_selection_in_listcontrol_routes))(event);
    }

    f->Resize();

    event.Skip(true);

}

void ExistingRoute::operator()(wxCommandEvent& event) {

    //by setting this to true, when the user presses enter, the method ListFrame::KeyDown will call on_select_route_in_listcontrol_routes_for_transport
    (f->transporting_with_selected_route) = true;

    //save data->route_list into route_list_saved
    f->route_list_saved.resize(((f->data)->route_list).size());
    copy((f->data->route_list).begin(), (f->data->route_list).end(), (f->route_list_saved).begin());

    //print an info message
    (f->print_question_message)->SetAndCall(NULL, String(""), String("You are about to transport with an existing route. Select the Route and press enter.\nDo you want to continue?"), String("Yes"), String("No, I want to cancel"));

    event.Skip(true);

}

void AllRoutes::operator()(wxCommandEvent& event) {

    unsigned int j;

    //there need to be at list two routes of type ((Route_types[2]).value) to compute crossings. Here I include all routes of type ((Route_types[2]).value) whhich are related to a Sight into crossing_route_list by writing their index into crossing_route_list
    for (((f->data)->crossing_route_list).clear(), j = 0; j < ((f->data)->route_list).size(); j++) {

        if ((((((f->data)->route_list))[j]).type.value) == ((Route_types[2]).value)) {
            ((f->data)->crossing_route_list).push_back(((((f->data)->route_list))[j]));
        }

    }

    f->OnComputePosition();

    event.Skip(true);

}

void SomeRoutes::operator()(wxCommandEvent& event) {


    int i;

    f->print_info_message->SetAndCall(NULL, String(""), String("Select the routes that you want to use to compute the astronomical position and press enter when done"), (wxGetApp().path_file_info_icon));

    //Given that a sight must be transported only with a Route that does not come from a Sight and a Route that is not a circle of equal altitude (it would not make sense), I store in route_list_for_transport the Routes in route_list which are not related to any sight and that are not circles of equal altitude, show route_list_for_transport in listcontrol_routes, and let the user select one item in route_list_for_transport to transport the Sight
    for ((f->crossing_route_list_temp.clear()), i = 0; i < (f->data->route_list).size(); i++) {

        if ((((f->data)->route_list)[i]).type == (Route_types[2])) {
            f->crossing_route_list_temp.push_back(((f->data->route_list)[i]));
        }

    }

    //setting this to true, now when the enter key is pressed the selected Routes are used to compute the position
    (f->selecting_route_for_position) = true;
    (f->listcontrol_routes)->set(f->crossing_route_list_temp, false);
    //I bind listcontrol_routes to on_select_route_in_listcontrol_routes_for_position in such a way that when the user will select an item in listcontrol, I perform the computation of the position
    //    (f->listcontrol_routes)->Bind(wxEVT_LIST_ITEM_SELECTED, *(f->on_select_route_in_listcontrol_routes_for_position));




    event.Skip(true);

}


void NewRoute::operator()(wxCommandEvent& event) {

    (f->transporting_with_new_route) = true;
    //call OnAddRoute to add a new Route
    (f->OnAddRouteForTransport)(event);

    //when button_ok in f->route_fram will be pressed, I call on_new_route_in_listcontrol_routes_for_transport to execute the transport with this Route
    //    f->route_frame->button_ok->Bind(wxEVT_BUTTON, *(f->on_new_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}


template<class P> void UnsetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = false;

    event.Skip(true);

}

//this is the same as template<class P> void UnsetIdling<P>::operator()(void){ but without the event argument
template<class P> void UnsetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}


template<class P> ConfirmTransport<P>::ConfirmTransport(P* parent_in) {

    parent = parent_in;

}


//I call this method when the user has decided to confirm that he/she wants to make the transport of an object
template<class P> void ConfirmTransport<P>::operator()(wxCommandEvent& event) {

    int i;

    //Given that an object must be transported only with a Route that does not come from a Sight and a Route that is not a circle of equal altitude (it would not make sense), I store in route_list_for_transport the Routes in route_list which are viable to be transporting Routes. These are the Routes that: 1. are not related to any sight, 2. that are not circles of equal altitude 3. That are different from  show route_list_for_transport in listcontrol_routes, and let the user select one item in route_list_for_transport to transport the Sight
    for ((parent->route_list_for_transport).clear(), (parent->map).clear(), i = 0; i < (parent->data->route_list).size(); i++) {

        if (
            /*condition that the Route is not relatied to a Sight*/
            (((((parent->data->route_list)[i]).related_sight).value) == -1) &&
            /*condition that the Route is not a circle of equal altitude*/
            (((parent->data->route_list)[i]).type != (Route_types[2])) &&
            /*condition that the Route does not coincide with the object to transport*/
            (((parent->transported_object_type) != String("route")) || ((parent->i_object_to_transport) != i))
            ) {

            (parent->route_list_for_transport).push_back((parent->data->route_list)[i]);
            (parent->map).push_back(i);

        }

    }

    parent->listcontrol_routes->set((parent->route_list_for_transport), false);
    parent->data->route_list.resize((parent->route_list_for_transport).size());
    copy((parent->route_list_for_transport).begin(), (parent->route_list_for_transport).end(), ((parent->data)->route_list).begin());
    parent->TabulateRoutesAll();
    parent->PreRenderAll();

    //I bind listcontrol_routes to on_select_route_in_listcontrol_routes_for_transport in such a way that when the user will double clock on an item in listcontrol (or single-click it and then press enter), I perform the transport
    parent->listcontrol_routes->Unbind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, parent);
    parent->listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, *(parent->on_select_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}

//this is the same as template<class P> void ConfirmTransport<P>::operator()(void){ but without the event argument)
template<class P> void ConfirmTransport<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}


template<class P> void SetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = true;

    event.Skip(true);

}

//this is the same as template<class P> void SetIdling<P>::operator()(wxCommandEvent& event){ but without the event argument
template<class P> void SetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}


void AskRemoveRelatedSight::operator()(wxCommandEvent& event) {

    int i_route_to_remove;

    //set i_route_to_remove equal to the currently relected Route in listcontrol_routes
    i_route_to_remove = ((int)(parent->listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    (parent->delete_route_and_related_sight->i_route_to_remove) = i_route_to_remove;
    (parent->delete_route->i_route_to_remove) = i_route_to_remove;


    if (((((parent->data->route_list)[i_route_to_remove]).related_sight).value) != -1) {
        //if the Route which I am about to remove is related to a Sight, I ask the user whether he wants to remove the related Sight too by showing a question_frame

        ShowQuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame> >* print_question;

        print_question = new ShowQuestionFrame< ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame> >(parent, parent->delete_route_and_related_sight, parent->delete_route, parent->unset_idling);

        print_question->SetAndCall(NULL, String(""), String("The route that you are about to remove is related to a sight. Do you want to remove the sight related to this route?"), String("Yes"), String("No"));

    }else{
        //if not, I simply delete teh route

        (*(parent->delete_route))(event);

    }

    parent->OnModifyFile();

    event.Skip(true);

}

void AskRemoveRelatedRoute::operator()(wxCommandEvent& event) {

    int i_sight_to_remove;

    //set i_sight_to_remove equal to the currently relected Sight in listcontrol_sights
    i_sight_to_remove = ((int)((parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    ((parent->delete_sight)->i_sight_to_remove) = i_sight_to_remove;


    if ((((((parent->data)->sight_list)[i_sight_to_remove]).related_route).value) != -1) {
        //if the sight which I am about to remove is related to a Route, I ask the user whether he wants to remove the related Route too by showing  question_frame

        ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>* print_question;

        ((parent->delete_route_and_related_sight)->i_route_to_remove) = (((((parent->data)->sight_list)[i_sight_to_remove]).related_route).value);

        //remove the route from the non-GUI object data
        //ask the user whether he/she wants to remove the related sight as well: if the answer is yes, then QuestionFrame calls the functor delete_sight_and_related_sight. If no, it calls the functor delete_sight.

        print_question = new ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>(parent, parent->delete_route_and_related_sight, parent->delete_sight, parent->unset_idling);

        print_question->SetAndCall(NULL, String(""), String("Do you want to remove the route related to this sight??"), String("Yes"), String("No"));


    }
    else {
        //if not, I simply delete teh sight

        (*(parent->delete_sight))(event);

    }

    parent->OnModifyFile();

    event.Skip(true);

}



void SelectRoute::operator()(wxCommandEvent& event) {

    (parent->idling) = true;

    //temporarily unbind listcontrol_routes from &ListFrame::OnChangeSelectionInListControl
    (parent->listcontrol_routes)->Unbind(wxEVT_LIST_ITEM_SELECTED, *(parent->on_change_selection_in_listcontrol_routes));
    (parent->listcontrol_routes)->Unbind(wxEVT_LIST_ITEM_DESELECTED, *(parent->on_change_selection_in_listcontrol_routes));



    //brings parent to front
    parent->Raise();

    //deselect all previously selected items in listcontrol_routes to allow the user to properly select an item
    (parent->listcontrol_routes)->DeselectAll();

    //	(parent->listcontrol_routes)->Bind(wxEVT_LIST_ITEM_SELECTED, *(parent->on_select_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}




template<class P> void FunctionOnPressOk<P>::operator()(wxCommandEvent& event) {

    //do something

    event.Skip(true);

}


//constructor of the struct, which initializes the Answer remove_related_sight. If remove_related_sight.value = 'y', then DeleteRoute::operator() will delete both the route and the related sight. If remove_related_sight.value = 'n', then it will remove the route only.
DeleteRoute::DeleteRoute(ListFrame* f_in, Answer remove_related_sight_in) {

    f = f_in;
    remove_related_sight = remove_related_sight_in;

}

void DeleteRoute::operator()(wxCommandEvent& event) {

    int i_related_sight;

    i_related_sight = ((((f->data)->route_list)[i_route_to_remove]).related_sight).value;

    //remove the sight related to the route which I am about to remove from the GUI object listcontrol_sights
    if ((i_related_sight != -1) && (remove_related_sight == Answer('y', String("")))) {

        //        (f->listcontrol_sights)->DeleteItem(i_related_sight);

        //given that after one item is deleted in listcontrol_sights, now no item is selected in listcontrol_sights, I call:
        (*(f->on_change_selection_in_listcontrol_sights))(event);


    }

    //I remove the route and the related sight from both the non-GUI object data
    (f->data)->remove_route(((unsigned int)i_route_to_remove), remove_related_sight, String(""));

    (f->listcontrol_sights)->set((f->data)->sight_list, false);
    (f->listcontrol_routes)->set((f->data)->route_list, false);

    //given that I called set in listcontrol_routes, now no item is selected in listcontrol_routes, I call:
    (*(f->on_change_selection_in_listcontrol_routes))(event);
    f->Resize();
    //given that a Route has been removed, I re-draw everything
    f->PreRenderAll();

    event.Skip(true);

}


DeletePosition::DeletePosition(ListFrame* f_in) {

    f = f_in;

}

//delete the  selected position in the GUI object f->listcontrol_position and in the non-GUI object f->data. If no position is selected, it does nothing.
void DeletePosition::operator()(wxCommandEvent& event) {

    long i;

    i = (f->listcontrol_positions)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (i != -1) {

        (f->data)->remove_position(((unsigned int)i), String(""));
        (f->listcontrol_positions)->set((f->data)->position_list, false);

        //given that I called set in listcontrol_positions, now no item is selected in listcontrol_positions-> I call:
        (*(f->on_change_selection_in_listcontrol_positions))(event);
        f->Resize();
        f->OnModifyFile();

    }

    event.Skip(true);

}


//set *this to the current time
void String::set_to_current_time(void) {

    Time now;

    now.set_current();
    //I write in the non-GUI object *this

    set(String("String set to current time"), String(now.to_string(data_precision.value, true)), String(""));

}



template<class P> template <class T> void SetStringFieldToCurrentTime<P>::operator()(T& event) {

    //if the label is empty, I replace it with the local time and date
    if ((p->value->GetValue()).IsEmpty()) {

        Time now;

        now.set_current();
        //I write in the non-GUI object (p->string)
        (*(p->string)) = String(now.to_string(data_precision.value, true));

        p->set();

    }

    event.Skip(true);

}



ResetListFrame::ResetListFrame(ListFrame* p_in) {

    p = p_in;

}


//reset *this by destroying this->data, and allocating a new one
template <class T> void ResetListFrame::operator()(T& event) {

    //clear p->data and allocate a new one
    p->data->~Data();
    //the file now has no title and has not been modified
    (p->file_is_untitled) = true;
    (p->file_has_been_modified) = false;

    p->data = new Data(p->catalog, String(""));

    //empty all listcontrols
    p->listcontrol_sights->DeleteAllItems();
    p->listcontrol_positions->DeleteAllItems();
    p->listcontrol_routes->DeleteAllItems();

    //resize, set an 'untitled' label for the new, empty ListFrame, and update the chartframes
    p->Resize();
    p->SetLabel(wxString("untitled"));
    p->PreRenderAll();

    event.Skip(true);

}



template<class F> SaveAndReset<F>::SaveAndReset(F* frame_in) {

    frame = frame_in;

}


CloseApp::CloseApp(MyApp* app_in){
    
    app = app_in;
    
}


template <class T> void CloseApp::operator()([[maybe_unused]] T& event) {
    
    app->list_frame->Close();
    app->disclaimer->Close();
    
}




template<class F> CloseFrame<F>::CloseFrame(F* frame_in) {

    frame = frame_in;

}

//closes a frame of type F
template<class F> template <class T> void CloseFrame<F>::operator()([[maybe_unused]] T& event) {

    DestroyFrame<F>* destroy_frame;

    destroy_frame = new DestroyFrame<F>(frame);

    //destroys frame
    frame->CallAfter(*destroy_frame);

    event.Skip(true);


}

template<class F> ShowFrame<F>::ShowFrame(F* frame_in) {

    frame = frame_in;

}

//show a frame of type F
template<class F> template <class T> void ShowFrame<F>::operator()(T& event) {

    frame->Show(true);

    event.Skip(true);


}

ShowAll::ShowAll(ListFrame* frame_in) {

    frame = frame_in;

}

//show ListFrame and all ChartFrames
template <class T> void ShowAll::operator()(T& event) {

    ShowFrame<ListFrame>* show_frame;
    show_frame = new ShowFrame<ListFrame>(frame);

    (*show_frame)(event);
    for(unsigned int i=0; i<wxGetApp().list_frame->chart_frames.size(); i++){
        wxGetApp().list_frame->chart_frames[i]->ResetRender(event);
    }
    wxGetApp().ShowCharts(event);
    wxGetApp().AnimateCharts();

    event.Skip(true);

}

template<class F> DestroyFrame<F>::DestroyFrame(F* frame_in) {

    frame = frame_in;

}

//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}



//saves the data in frame->data to file frame->file ,and closes frame
template<class F> template <class T> void SaveAndReset<F>::operator()(T& event) {

    ResetListFrame* reset_list_frame;

    reset_list_frame = new ResetListFrame(frame);


    if (frame->file_is_untitled) {
        //the file has no name -> save as

        frame->OnPressCtrlShiftS(event);

        //        wxFileDialog openFileDialog(frame, _(""), default_open_directory, "", "nav files (*.nav)|*.nav", wxFD_SAVE | wxFD_FILE_MUST_EXIST);
        //
        //
        //        if((openFileDialog.ShowModal()) != wxID_CANCEL){
        //            // the user did not presse cancel -> proceed saving on the file chosen by the user;
        //
        //            (frame->file).set_name(String((openFileDialog.GetPath()).ToStdString()));
        //            //open a new file to save content on it
        //            (frame->file).open(String("out"), String(""));
        //            //write frame->data into file
        //            (frame->data)->print(false, String(""), ((frame->file).value));
        //            //close the file
        //            (frame->file).close(String(""));
        //
        //        }

    }
    else {
        //the file has a name -> save

        //remove the file to avoid overwriting
        (frame->data_file).remove(String(""));
        //open a new data_file
        (frame->data_file).open(String("out"), String(""));
        //write frame->data into data_file
        (frame->data)->print(false, String(""), *((frame->data_file).value));
        //close the data_file
        (frame->data_file).close(String(""));


    }

    (*reset_list_frame)(event);

    event.Skip(true);

}

template<class P> CheckString<P>::CheckString(StringField<P>* p_in) {

    p = p_in;

}

//this functor does nothing, delete it in the future
template<class P> template<class T> void CheckString<P>::operator()(T& event) {

    P* f = (p->parent);

    f->AllOk();

    event.Skip(true);

}


//I write in the non-GUI object string the value entered in the GUI object value
template<class P> template<class T> void StringField<P>::get(T& event) {

    //here I don't check whether the StringField is ok, because any value in the string field is ok
    (*string) = String((value->GetValue().ToStdString()));

    event.Skip(true);

}




template<class NON_GUI, class P> ToDoAtEndOfTransport<NON_GUI, P>::ToDoAtEndOfTransport(NON_GUI* object_a_in, NON_GUI* object_b_in, P* parent_in){
    
    object_a = object_a_in;
    object_b = object_b_in;
    parent = parent_in;

}


//set *object_a equal to *object_b if object_a != NULL, and to the other tasks to be done at the end of a transport
template<class NON_GUI, class P> void ToDoAtEndOfTransport<NON_GUI, P>::operator()(void){
    
    if(object_a){
        (*object_a) = (*object_b);
    }
    
    if((parent->i_object_to_disconnect) != -1){
        
        //print an info message
        parent->print_info_message->SetAndCall(NULL, String("Warning"), String("The transported route  was related to a sight! The route has been disconnected from the sight."), (wxGetApp().path_file_info_icon));
        
       (parent->i_object_to_disconnect) = -1;
        
    }
    
    parent->listcontrol_sights->set((parent->data->sight_list), false);
    parent->listcontrol_routes->set((parent->data->route_list), false);
    parent->Resize();
    parent->PreRenderAll();

}


template<class P> CheckDate<P>::CheckDate(DateField<P>* p_in) {

    p = p_in;

    check_year = new CheckYear<P>(p);
    check_month = new CheckMonth<P>(p);
    check_day = new CheckDay<P>(p);

}

//this functor checks the whole date field by calling the check on its year, month and day parts
template<class P> template <class T> void CheckDate<P>::operator()(T& event) {

    (*check_year)(event);
    (*check_month)(event);
    (*check_day)(event);

    event.Skip(true);

}

//this functor writes the values written inthe whole GUI date field (year, month and day) in the respective non-GUI object date->D, date->M, date->D
template<class P> template <class T> void DateField<P>::get(T& event) {

    if (year_ok && (year->IsEnabled()) && month_ok && (month->IsEnabled()) && day_ok && (month->IsEnabled())) {

        (date->Y) = (unsigned int)wxAtoi(year->GetValue());
        (date->M) = (unsigned int)wxAtoi(month->GetValue());
        (date->D) = (unsigned int)wxAtoi(day->GetValue());

    }

    event.Skip(true);

}

template<class P> CheckLengthValue<P>::CheckLengthValue(DynamicLengthField<P>* p_in) {

    p = p_in;

}

//checks the value in the GUI field in DynamicLengthField
template<class P> template <class T> void CheckLengthValue<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

        if (check || (((p->value->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->value->GetValue()).ToStdString())) == String("")))) {
            //p->value either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set value_ok to true (false)
            (p->value_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in value
            p->value->SetForegroundColour(wxGetApp().foreground_color);
            p->value->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->value), String("Entered value is not valid!"), String("Lengths must be floating-point numbers >= 0 m"), (wxGetApp().path_file_error_icon));

            (p->value_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckLengthUnit<P>::CheckLengthUnit(DynamicLengthField<P>* p_in) {

    p = p_in;

}


//check the unit in the GUI field in LengthField
template<class P> template <class T> void CheckLengthUnit<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        
        p->unit->CheckInCatalog(&check, &i);

        if (check || (((p->unit->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->unit->name->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set unit->ok to true (false)
            (p->unit->ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            p->unit->name->SetForegroundColour(wxGetApp().foreground_color);
            p->unit->name->SetFont(wxGetApp().default_font);


        }else {

            stringstream temp;

            temp.str("");
            temp << "Available units are: ";
            for (i = 0; i < LengthUnit_types.size(); i++) {
                temp << (LengthUnit_types[i]).value << ((i < LengthUnit_types.size() - 1) ? ", " : ".");
            }

            f->print_error_message->SetAndCall((p->unit->name), String("Unit not found in list!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            (p->unit->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckLength<P>::CheckLength(DynamicLengthField<P>* p_in) {

    p = p_in;

    check_length_value = new CheckLengthValue<P>(p);
    check_length_unit = new CheckLengthUnit<P>(p);

}


//this functor checks the whole Length field by calling the check on its value and unit
template<class P> template <class T> void CheckLength<P>::operator()(T& event) {

    (*check_length_value)(event);
    (*check_length_unit)(event);

    event.Skip(true);

}


template<class P> CheckSpeedValue<P>::CheckSpeedValue(SpeedField<P>* p_in) {

    p = p_in;

}

//check the value in the GUI field in SpeedField
template<class P> template <class T> void CheckSpeedValue<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

        if (check || (((p->value->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->value->GetValue()).ToStdString())) == String("")))) {
            //p->value either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set value_ok to true (false)
            (p->value_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in value
            p->value->SetForegroundColour(wxGetApp().foreground_color);
            p->value->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->value), String("Entered value is not valid!"), String("Speeds must be floating-point numbers >= 0 m"), (wxGetApp().path_file_error_icon));

            (p->value_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckSpeedUnit<P>::CheckSpeedUnit(SpeedField<P>* p_in) {

    p = p_in;

}


//check the unit in the GUI field in SpeedField
template<class P> template <class T> void CheckSpeedUnit<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;
        
        p->unit->CheckInCatalog(&check, &i);

        if (check || (((p->unit->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->unit->name->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set unit->ok to true (false)
            (p->unit->ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in unit
            p->unit->name->SetForegroundColour(wxGetApp().foreground_color);
            p->unit->name->SetFont(wxGetApp().default_font);


        }else {

            stringstream temp;

            temp.str("");
            temp << "Available units are: ";
            for (i = 0; i < SpeedUnit_types.size(); i++) {
                temp << (SpeedUnit_types[i]).value << ((i < SpeedUnit_types.size() - 1) ? ", " : ".");
            }

            f->print_error_message->SetAndCall((p->unit->name), String("Unit not found in list!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            (p->unit->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckSpeed<P>::CheckSpeed(SpeedField<P>* p_in) {

    p = p_in;

    check_speed_value = new CheckSpeedValue<P>(p);
    check_speed_unit = new CheckSpeedUnit<P>(p);

}


//this functor checks the whole Speed field by calling the check on its value and unit
template<class P> template <class T> void CheckSpeed<P>::operator()(T& event) {

    (*check_speed_value)(event);
    (*check_speed_unit)(event);

    event.Skip(true);

}


//write the value and the unit of the GUI field in LengthField into the non-GUI field length
template<class P> template <class T> void DynamicLengthField<P>::get(T& event) {
    
    if(is_ok()){
        
        double x;
        
        value->GetValue().ToDouble(&x);
        LengthField<P>::length->set(x, LengthUnit((LengthField<P>::unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);
    
}



//if an item in listcontrol_sights/positions/routes is selected, I transport the Sight/Position/Route under consideration with such Route
template<class T> void OnSelectRouteInListControlRoutesForTransport::operator()(T& event) {
    
    //do the tasks tha need to be done at the end of the transport: set_back_transporting_route is used to set the transporting Route back to its original value after the animation transport is finished, in order to avoid the accumulation of numerical errors if one transported it back
    ToDoAtEndOfTransport<Route, ListFrame>* to_do_at_end_of_transport;
    
    //now I no longer need route_list to contain only the available Routes for transport -> I put back all the Routes before the transport into route_list by copying route_list_saved into route_list.
    // PaintEvent() will need points_route_list to be updated according to this change -> I call TabulateRoutesAll() to update points_route_list
    parent->data->route_list.resize((parent->route_list_saved.size()));
    copy((parent->route_list_saved.begin()), (parent->route_list_saved.end()), (parent->data->route_list.begin()));
    parent->TabulateRoutesAll();
    
    //this is the # of the transporting Route in the full Route list given by data->route_list
    (parent->i_transporting_route) = (parent->map)[(parent->listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED))];
    
    //during the animation, (parent->data->route_list)[(parent->i_transporting_route)] will be transported -> there may be accumulating numerical errors when I transport it back -> I store it in transporting_route_saved and then set (parent->data->route_list)[(parent->i_transporting_route)] equal to transporting_route_saved at the end of the whole animation
    (parent->transporting_route_saved) = (parent->data->route_list)[(parent->i_transporting_route)];
    
    to_do_at_end_of_transport = new ToDoAtEndOfTransport<Route, ListFrame>(
                                                                  &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                  &(parent->transporting_route_saved),
                                                                  parent
                                                                  );
    
    if (((parent->transported_object_type) == String("sight")) || (parent->transported_object_type) == String("route")) {
        //I am transporting a Sight or the Route related to it: allocate transport_handler with template NON_GUI = Route
        
        String new_label;
        
        
//        GraphicalFeatureTransportHandler<Route, UnsetIdling<ListFrame> >* transport_handler;
        //auxiliary_transport_handler_inbound will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the end of the transport (inbound), to set the transporting Route back where it was at the beginning
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* auxiliary_transport_handler_inbound;
        //transport_handler does the actual, main transport of the Route
        GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >* transport_handler;
        //auxiliary_transport_handler_outbount will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the beginning of the transport (outbound). Then the actual transport of transported_object will be done, and then the transporting Route is transported back to its original position
        GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >* auxiliary_transport_handler_outbound;
        
        
        
        if ((parent->transported_object_type) == String("sight")) {
            //the transported object is a Sight
            
            //the id of the Route that will be transported is the one of the Route related to the Sight that is being transported
            (parent->i_object_to_transport) = ((((parent->data->sight_list)[(parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value);
            
        }
        
//        transport_handler = new GraphicalFeatureTransportHandler<Route, UnsetIdling<ListFrame> >(
//                                                                                                 parent,
//                                                                                                 &((parent->data->route_list)[(parent->i_object_to_transport)]),
//                                                                                                 (parent->transported_object_type),
//                                                                                                 ((parent->data->route_list)[(parent->i_transporting_route)]),
//                                                                                                 parent->unset_idling
//                                                                                                 );
        
        
        auxiliary_transport_handler_inbound = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                             &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                             String("route"),
                                                                                                             Route(RouteType(Route_types[0]),
                                                                                                                   ((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position,
                                                                                                                   (parent->transporting_route_saved).reference_position),
                                                                                                             to_do_at_end_of_transport
                                                                                                             );
        transport_handler = new GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >(
                                                                                                                                        parent,
                                                                                                                                        &((parent->data->route_list)[(parent->i_object_to_transport)]),
                                                                                                                                        (parent->transported_object_type),
                                                                                                                                        ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                                        auxiliary_transport_handler_inbound
                                                                                                                                        );
        auxiliary_transport_handler_outbound = new GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >(
                                                                                                                                                                                                     parent,
                                                                                                                                                                                                     &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                                                                                                                     String("route"),
                                                                                                                                                                                                     Route(RouteType(Route_types[0]), (parent->transporting_route_saved).reference_position, ((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position),
                                                                                                                                                                                                                      transport_handler
                                                                                                                                                                                                                      );
        
        //start the transport
        //        (*transport_handler)();
        (*auxiliary_transport_handler_outbound)();

    }
    
    if ((parent->transported_object_type) == String("position")) {
        
        //auxiliary_transport_handler_inbound will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the end of the transport (inbound), to set the transporting Route back where it was at the beginning
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* auxiliary_transport_handler_inbound;
        //transport_handler does the actual, main transport of the Position
        GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >* transport_handler;
        //auxiliary_transport_handler_outbount will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the beginning of the transport (outbound). Then the actual transport of transported_object will be done, and then the transporting Route is transported back to its original position
        GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >* auxiliary_transport_handler_outbound;
        
        
        //        (parent->transporting_route_saved) = (parent->data->route_list)[(parent->i_transporting_route)];
        
        //the id of the Position that will be transported,
        (parent->i_object_to_transport) = ((int)((parent->listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED))));
        
        auxiliary_transport_handler_inbound = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                             &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                             String("route"),
                                                                                                             Route(RouteType(Route_types[0]),
                                                                                                                   ((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                   (parent->transporting_route_saved).reference_position),
                                                                                                             to_do_at_end_of_transport
                                                                                                             );
        transport_handler = new GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >(
                                                                                                                                        parent,
                                                                                                                                        &((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                                        (parent->transported_object_type),
                                                                                                                                        ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                                        auxiliary_transport_handler_inbound
                                                                                                                                        );
        auxiliary_transport_handler_outbound = new GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >(
                                                                                                                                                                                                     parent,
                                                                                                                                                                                                     &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                                                                                                                     String("route"),
                                                                                                                                                                                                     Route(RouteType(Route_types[0]), (parent->transporting_route_saved).reference_position, ((parent->data->position_list)[(parent->i_object_to_transport)])),
                                                                                                                                                                                                     transport_handler
                                                                                                                                                                                                     );
        

        
        //these timers of auxiliary_transport_handler and transport_handler run at the same time -> change this with CallAfter and a lambda call
        //start the auxiliary transport
        (*auxiliary_transport_handler_outbound)();
        //        (*transport_handler)();
        
    }
    
//    parent->CallAfter([this, (parent->transporting_route_saved)]()->void {
//        //set (parent->data->route_list)[(parent->i_transporting_route)] equal to its value before the transport, update parent and re-draw everthing
//        (parent->data->route_list)[(parent->i_transporting_route)]  = (parent->transporting_route_saved);
//        parent->listcontrol_sights->set((parent->data->sight_list), false);
//        parent->listcontrol_routes->set((parent->data->route_list), false);
//        parent->Resize();
//        parent->PreRenderAll();
//    });
    
    event.Skip(true);
    
}


//if a new item listcontrol_routes is created, I transport the sight/position under consideration with such Route
template<class T> void OnNewRouteInListControlRoutesForTransport::operator()(T& event) {
    
    //the id of the Route that will do the transport: it is the last item in listcontrol_routes, because it is the item of the newly added Route
    (parent->i_transporting_route) = ((parent->listcontrol_routes)->GetItemCount()) - 1;
    //do the tasks tha need to be done at the end of the transport
    ToDoAtEndOfTransport<Route, ListFrame>* to_do_at_end_of_transport;

    to_do_at_end_of_transport = new ToDoAtEndOfTransport<Route, ListFrame>(
                                                                  NULL,
                                                                  NULL,
                                                                  parent
                                                                  );


    if (((parent->transported_object_type) == String("sight")) || ((parent->transported_object_type) == String("route"))) {
        //I am transporting a Sight or the Route related to it: allocate transport_handler with template NON_GUI = Route
        
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* transport_handler;

        
        if ((parent->transported_object_type) == String("sight")) {
            
            
            //the id of the Route that will be transported
            (parent->i_object_to_transport) = (((((parent->data)->sight_list)[(parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value);
            
            
        }
        
        
        transport_handler = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(
                                                                                                                 parent,
                                                                                                                 &((parent->data->route_list)[(parent->i_object_to_transport)]),
                                                                                                                 (parent->transported_object_type),
                                                                                                                 ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                 to_do_at_end_of_transport
                                                                                                                 );
        
        //start the transport
        (*transport_handler)();
        
    }
    
    if ((parent->transported_object_type) == String("position")) {
        //I am transporting a Position: allocate transport_handler with template NON_GUI = Position
        
        GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame> >* transport_handler;
        
        
        //the id of the Route or Position that will be transported
        (parent->i_object_to_transport) = ((int)(parent->listcontrol_positions)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
        
        transport_handler = new GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                                    &((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                    (parent->transported_object_type),
                                                                                                                    ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                    to_do_at_end_of_transport
                                                                                                                    );
        
        //start the transport
        (*transport_handler)();
    }
    
    event.Skip(true);
    
}



template<class T, typename FF_OK> void PrintMessage<T, FF_OK>::operator()(void) {
    
    SetIdling<T>* set_idling;
    UnsetIdling<T>* unset_idling;
    
    set_idling = new SetIdling<T>(f);
    unset_idling = new UnsetIdling<T>(f);
    
    //I may be about to prompt a temporary dialog window, thus I set f->idling to true
    (*set_idling)();
    
    if (control != NULL) {
        
        if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {
            
            message_frame = new MessageFrame<FF_OK>(f, f_ok, title.value, message.value, image_path, wxDefaultPosition, wxDefaultSize, String(""));
            message_frame->Show(true);
            message_frame->Raise();
            
            // control->SetFocus();
            control->SetForegroundColour((wxGetApp().error_color));
            control->SetFont(wxGetApp().error_font);
            //                Reset(control);
            
        }

    }
    else {

        message_frame = new MessageFrame<FF_OK>(f, f_ok, title.value, message.value, image_path, wxDefaultPosition, wxDefaultSize, String(""));
        message_frame->Show(true);
        message_frame->Raise();

    }


    //AFTER the dialog window has been closed, I set f->idling to calse
    f->CallAfter(*unset_idling);


}



PositionFrame::PositionFrame(ListFrame* parent_input, Position* position_in, long position_in_listcontrol_positions_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    parent = parent_input;

    String new_prefix, /*the label which appears on button_ok*/label_button_ok;
    unsigned int common_width;


    bool check = true;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    //SetColor(this);

    set_idling = new SetIdling<PositionFrame>(this);
    unset_idling = new UnsetIdling<PositionFrame>(this);
    (*unset_idling)();

    print_error_message = new PrintMessage<PositionFrame, UnsetIdling<PositionFrame> >(this, unset_idling);

    //if this PositionFrame has been constructed with position_in = NULL, then I allocate a new Position object with the pointer this->position and set position_in_listcontrol_positions to a 'NULL' value (position_in_listcontrol_positions = -1). Otherwise, the pointer position_in points to a valid Position object -> I let this->position point to position_in, and set position_in_listcontrol_positions to position_in_listcontrol_positions_in.
    if (position_in != NULL) {
        position = position_in;
        position_in_listcontrol_positions = position_in_listcontrol_positions_in;
    }
    else {
        position = new Position();
        position_in_listcontrol_positions = -1;
    }

    //if I am adding a brand new position, I name button_ok 'Add'. Otherwise I name it "Modify"
    if (position_in == NULL) {
        label_button_ok.set(String("Add"));
    }
    else {
        label_button_ok.set(String("Modify"));
    }


    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value, wxDefaultPosition, wxDefaultSize);
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);

    sizer_grid_measurement = new wxFlexGridSizer(2, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_label = new wxFlexGridSizer(1, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);


    //latitude
    StaticText* text_lat = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    lat = new AngleField<PositionFrame>(panel, &(position->phi), String("NS"));

    //longitude
    StaticText* text_lon = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    lon = new AngleField<PositionFrame>(panel, &(position->lambda), String("EW"));

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<PositionFrame>(panel, &(position->label));


    //I bind  button_ok to label->set_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_ok->Bind(wxEVT_BUTTON, label->set_to_current_time);
    //If I press button_ok, I want all the fields in this PositionFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lat);
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lon);
    button_ok->Bind(wxEVT_BUTTON, &StringField<PositionFrame>::get<wxCommandEvent>, label);
    button_ok->Bind(wxEVT_BUTTON, &::PositionFrame::OnPressOk, this);
    button_cancel->Bind(wxEVT_BUTTON, &PositionFrame::OnPressCancel, this);


    panel->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    lat->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    lon->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);


    //I enable the reduce button only if position_in is a valid position with the entries propely filled, i.e., only if position_in != NULL
    button_ok->Enable((position_in != NULL));

    sizer_grid_measurement->Add(text_lat, 0, wxALIGN_CENTER_VERTICAL);
    lat->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_lon, 0, wxALIGN_CENTER_VERTICAL);
    lon->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL
    );
    box_sizer_2->Add(button_ok, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL
    );

    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Coordinates");

    sizer_box_measurement->Add(sizer_grid_measurement);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the StaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_lat->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_lon->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    //add the various elements to sizer, by inserting a border of (wxGetApp().rectangle_display.GetSize().GetWidth())*(length_border_over_length_screen.value) in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));


    //panel->SetSizer(sizer);

    //    CreateStatusBar();
    //    SetStatusText( "Welcome to UnsetIdling's text editor!" );

    if (!check) {
        cout << prefix.value << RED << "Cannot read position!\n" << RESET;
    }

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    //Maximize();
    panel->Fit();
    Fit();


    if (position_in != NULL) { set(); }

    Centre();

}


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
        type = new RouteTypeField<RouteFrame>(panel, &(route->type), Route_for_transport_types, &(wxGetApp().list_frame->data->recent_route_for_transport_types));
    }else{
        type = new RouteTypeField<RouteFrame>(panel, &(route->type), Route_types,  &(wxGetApp().list_frame->data->recent_route_types));
    }

    //Z
    StaticText* text_Z = new StaticText(panel, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0);
    Z = new AngleField<RouteFrame>(panel, &(route->Z), String(""));

    //format in which lengths are expressed
    StaticText* text_l_format = new StaticText(panel, wxT("Length format"), wxDefaultPosition, wxDefaultSize, 0);
    length_format = new LengthFormatField<RouteFrame>(panel, &(route->length_format), &(wxGetApp().list_frame->data->recent_length_formats));

    //the field for time to set the Route length
    text_time = new StaticText(panel, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0);
    time = new ChronoField<RouteFrame>(panel, &(route->time));
    //the field for speed to set the Route length
    text_speed = new StaticText(panel, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0);
    speed = new SpeedField<RouteFrame>(panel, &(route->speed));

    //the field for Length to set the Route length
    text_length = new StaticText(panel, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0);
    length = new DynamicLengthField<RouteFrame>(panel, &(route->length)/*, LengthUnit_types[0]*/);


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
    start_phi = new AngleField<RouteFrame>(panel, &((route->reference_position).phi), String("NS"));
    //start_lambda
    StaticText* text_start_lambda = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    start_lambda = new AngleField<RouteFrame>(panel, &((route->reference_position).lambda), String("EW"));

    //GP (ground position)
    //GP_phi
    StaticText* text_GP_phi = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    GP_phi = new AngleField<RouteFrame>(panel, &((route->reference_position).phi), String("NS"));
    //GP_lambda
    StaticText* text_GP_lambda = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    GP_lambda = new AngleField<RouteFrame>(panel, &((route->reference_position).lambda), String("EW"));

    //omega
    StaticText* text_omega = new StaticText(panel, wxT("Omega"), wxDefaultPosition, wxDefaultSize, 0);
    omega = new AngleField<RouteFrame>(panel, &(route->omega), String(""));

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

        //set as route type the most recent item in recetn _route_types (or recent route_for_transport_types if the new Route has been created for a transport): set first type->object and then write in type the value written in type->object

        if(for_transport){
            type->object->set(Route_types[wxGetApp().list_frame->data->recent_route_for_transport_types.front()]);
        }else{
            type->object->set(Route_types[wxGetApp().list_frame->data->recent_route_types.front()]);
        }
        type->MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame> >::set();
        
        length_format->object->set(LengthFormat_types[wxGetApp().list_frame->data->recent_length_formats.front()]);
        length_format->MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame> >::set();

        //call this to enable/disable the GUI fields in according to the choice of RouteType
        type->OnEdit();
        //call this to enable/disable the GUI fields according ot the choice of LengthFormat
        OnChooseLengthFormatField();

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



//set all the GUI fields in this equal to those in the non-GUI object this->position
void PositionFrame::set(void) {

    lat->set();
    lon->set();
    label->set();

}

//set all the non-GUI objects in this->position  equal to those in the GUI fields
template<class T> void PositionFrame::get(T& event) {

    lat->get(event);
    lon->get(event);
    label->get(event);

}


void PositionFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to false
    (*(parent->unset_idling))();

    Close(TRUE);

}


//this function is triggered when button_ok is pressed
void PositionFrame::OnPressOk(wxCommandEvent& event) {

    unsigned int i;
    stringstream s;

    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }

    //if I am adding a new Position, I resize points_position_list to add a new element to it
    if (position_in_listcontrol_positions == -1) {

        for (i = 0; i < (parent->chart_frames.size()); i++) {

            ((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).resize(((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).size() + 1);
            ((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).resize(((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).size() + 1);

        }
    }


    //writes the values of the GUI fields in the non-GUI fields
    get(event);

    position->print(String("position entered via GUI"), String(""), cout);

    //if the constructor of PositionFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of position_list
    if (position_in_listcontrol_positions == -1) {
        (((this->parent)->data)->position_list).push_back(*position);
    }

    (parent->listcontrol_positions)->set((parent->data)->position_list, false);

    //given that I have reset the content of listcontrol_positions, now no items are selected in this ListControl -> I call:
    (*(parent->on_change_selection_in_listcontrol_positions))(event);

    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    
//    parent->PreRenderAll();
    parent->AnimateToObject<Position, UnsetIdling<ListFrame> >(position, parent->unset_idling);
    

    event.Skip(true);

    Close(TRUE);

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

            (((parent->chart_frames)[i])->draw_panel)->points_route_list_now.resize(((((parent->chart_frames)[i])->draw_panel)->points_route_list_now).size() + 1);
            (((parent->chart_frames)[i])->draw_panel)->reference_positions_route_list_now.resize(((parent->chart_frames)[i])->draw_panel->reference_positions_route_list_now.size() + 1);

        }
    }

    //call listcontrol_sights->set with true because I want to keep the selection in listcontrol_sights
    parent->listcontrol_sights->set((parent->data)->sight_list, true);
    parent->listcontrol_positions->set((parent->data)->position_list, true);
    parent->listcontrol_routes->set((parent->data)->route_list, false);

    //given that I have reset the content of listcontrol_sights and listcontrol_routes, now no items will be selected in these ListControls -> I call:
    (*(parent->on_change_selection_in_listcontrol_sights))(event);
    (*(parent->on_change_selection_in_listcontrol_routes))(event);

    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    //insert the animation here
    

//    parent->PreRenderAll();

    if ((parent->transporting_with_new_route)) {
        //if I am adding a new Route for transport, call on_new_route_in_listcontrol_routes_for_transport to execute the transport with this Route
        (*(parent->on_new_route_in_listcontrol_routes_for_transport))(event);

        //set the reference position of the transporting Route to the initial position of the object that has been transported: in thiw way, the transporting Route will look nice on the chart
        if ((parent->transported_object_type) == String("position")) {

            //store the starting position in geo_position_start
            ((parent->data->route_list)[(parent->i_transporting_route)]).reference_position = (parent->data->position_list)[(parent->i_object_to_transport)];

        }
        else {

            if (((parent->transported_object_type) == String("sight")) || (parent->transported_object_type) == String("route")) {

                //store the starting reference position in geo_position_start
                ((parent->data->route_list)[(parent->i_transporting_route)]).reference_position = (((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position);

            }

        }

        //I refresh everything because of the modification above
        //call listcontrol_routes->set with true because I want to keep the selection in listcontrol_routes
        parent->listcontrol_routes->set((parent->data->route_list), false);
        parent->Resize();
        parent->OnModifyFile();
//        parent->PreRenderAll();

    }

    
    if(prompt_disconnection_message){
        //I am modifying an existing Route and the Route that I am modifying is related to a Sight -> prepare the warning message to be prompted at the end of the animation and call AnimateToObject with parent->print_info_message as an argument, in such a way that, at the end of the animation, this message is prompted

        parent->print_info_message->control = NULL;
        parent->print_info_message->title.set(String("Warning"));
        parent->print_info_message->message.set(String("The route which has been modified was related to a sight! Disconnecting the route from the sight."));
        
        parent->AnimateToObject<Route, PrintMessage<ListFrame, UnsetIdling<ListFrame> > >(route, parent->print_info_message);

        
    }else{
        //I don't need to prompt a message warning the user that the Route under consideration is being disconnected from its related Sight -> trigger the animation that centers the chart on *route by callling UnsetIdling (intended as 'do nothing' here) at the end of the animation
        
        //If I am adding a new Route for transport, I do not call any animation, because there is already the transport animation that will be prompted. Otherwise, I call an animation that zooms on the newly added Route
        if (!(parent->transporting_with_new_route)) {
            
            parent->AnimateToObject<Route, UnsetIdling<ListFrame>  >(route, parent->unset_idling);
            
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

//run check on all the GUI fields that are members of SightFrame and write true/false in their ok variables
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

    if ((route->type.value) == wxString(((Route_types[2]).value))) {
        //I disable the GUI fields which do not define a circle of equal altitude and set the others

        Z->Enable(false);

        start_phi->Enable(false);
        start_lambda->Enable(false);

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
        start_phi->Enable(!for_transport);
        start_lambda->Enable(!for_transport);

        GP_phi->Enable(false);
        GP_lambda->Enable(false);
        omega->Enable(false);

    }

    //enable the length or the time and speed fields
    OnChooseLengthFormatField();

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
            (route->length) = Length(route->time, route->speed);

        }else {
            //in the GUI field, lenght are expressed simply as a Length -> get l and set in the non-GUI field to false

            (route->length_format) = LengthFormat(((LengthFormat_types[1]).value));
            length->get(event);

        }

    }

    label->get(event);


}


//enable/disable the GUI fields in *this accoridng to the choice in type->name (the sleected type of Route)
template<class E> void RouteFrame::OnChooseLengthFormatField(E& event) {
    
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


//when time or speed are edited in RouteFrame, thie method updates the length GUI field by writing in it time x speed
template<class E> void RouteFrame::UpdateLength(E& event) {

    if ((time->is_ok()) && (speed->is_ok())) {

        length->set(Length(*(time->chrono), *(speed->speed)));

    }
    else {

        length->value->SetValue(wxString(""));
        length->unit->name->SetValue(wxString(""));

    }

    event.Skip(true);

}


//same as RouteFrame::OnChooseLengthFormat(E& event), but it does not accept any argument
void RouteFrame::OnChooseLengthFormatField(void) {

    wxCommandEvent dummy;

    OnChooseLengthFormatField(dummy);

}


//write all the content in the GUI fields into the non-GUI objects, checks whether all the fields in PositionFrame are ok and if they are it returns true and false otherwise
bool PositionFrame::is_ok(void) {

    wxCommandEvent dummy;

    get(dummy);

    return((lat->is_ok()) && (lon->is_ok()));


}

//if all_ok() returns turue it enables  button_add, and it disables it otherwise
void PositionFrame::AllOk(void) {

    button_ok->Enable(is_ok());

}



//if a key is pressed in the keyboard, I call this function
void PositionFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        // the use pressed escape -> I do as if the user pressed button_cancel

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

//this is an event table with template
//BEGIN_EVENT_TABLE_TEMPLATE1(MessageFrame, /*here I put the derived class*/wxFrame, /*here I put the template argument*/FF_OK)
//    EVT_PAINT(MessageFrame<FF_OK>::OnPaint)
//END_EVENT_TABLE()


template<typename FF_OK> MessageFrame<FF_OK>::MessageFrame(wxWindow* parent, FF_OK* f_ok_in, const wxString& title, const wxString& message, String image_path, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) {

    wxRect rectangle;

    f_ok = f_ok_in;

    //SetColor(this);
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< MessageFrame<FF_OK> >(this);

    //image
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (wxGetApp().display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);

    //allocate sizers
    frame_sizer = new wxBoxSizer(wxVERTICAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    //    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    //    sizer_grid = new wxGridSizer(3, 1, 0, 0);


    StaticText* text = new StaticText(panel, message, wxDefaultPosition, wxDefaultSize, 0);

    //bind the function MessageFrame<FF_OK>::KeyDown to the event where a keyboard dey is down
    panel->Bind(wxEVT_KEY_DOWN, &MessageFrame<FF_OK>::KeyDown, this);


    //buttons
    button_ok = new wxButton(panel, wxID_ANY, "Ok", wxDefaultPosition, wxDefaultSize);
    //    button_ok->Bind(wxEVT_BUTTON, &MessageFrame::OnPressOk, this);
    button_ok->Bind(wxEVT_BUTTON, *close_frame);
    button_ok->Bind(wxEVT_BUTTON, *f_ok);

    image = new StaticBitmap(
                             panel,
                             image_path,
                             //I use ToDIP to adjust the size independently of the screen resolution
                             ToDIP(wxSize(
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
                                          ))
                             );

    sizer_v->Add(text, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_v->Add(button_ok, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_v);
    panel->Fit();
    Fit();

    CentreOnScreen();

}

//if a key is pressed in the keyboard, I call this function
template<typename FF_OK> void MessageFrame<FF_OK>::KeyDown(wxKeyEvent& event) {

    if (((event.GetKeyCode()) == WXK_ESCAPE) || ((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
        //the user presses esc or return -> I close *this and set the idling variable to false

        (*close_frame)(event);
        ((f_ok->parent)->idling) = false;

    }

}

//template<typename FF_OK> void MessageFrame<FF_OK>::OnPaint(wxPaintEvent& WXUNUSED(event)){
//
//    wxPaintDC dc(this);
//    wxScopedPtr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
//
//    gc->SetFont(*wxNORMAL_FONT, *wxBLACK);
//    gc->DrawBitmap(*m_bitmap, 0, 0,
//                   (wxGetApp().rectangle_display.GetWidth())*((wxGetApp().size_message_image_over_width_screen).value),
//                   (wxGetApp().rectangle_display.GetWidth())*((wxGetApp().size_message_image_over_width_screen).value)
//                   );
//
//}


template<typename F_A, typename F_B, typename F_ABORT> QuestionFrame<F_A, F_B, F_ABORT>::QuestionFrame(wxWindow* parent, F_A* f_a_in, String string_a_in, F_B* f_b_in, String string_b_in, F_ABORT* f_abort_in, bool enable_button_a_in, bool enable_button_b_in, bool bind_esc_to_button_b_in, const wxString& title, const wxString& message, String path_icon_file, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) {

    wxRect rectangle;
    vector<StaticText*> text;
    //the wxString message will be split into multiple Strings which were separated by a '\n' in message, each string will be written in an entry of message_split
    vector<String> message_split;
    int i;

    f_a = f_a_in;
    string_a = string_a_in;
    f_b = f_b_in;
    string_b = string_b_in;
    f_abort = f_abort_in;
    
    enable_button_a = enable_button_a_in;
    enable_button_b = enable_button_b_in;
    
    bind_esc_to_button_b = bind_esc_to_button_b_in;

    //SetColor(this);
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< QuestionFrame<F_A, F_B, F_ABORT> >(this);

    //image
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (wxGetApp().display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);

    //allocate sizers
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(1, 2, 0, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

    //write each line into text
    for (i = 0, message_split = String(message.ToStdString()).split(), text.resize(message_split.size()); i < (message_split.size()); i++) {
        text[i] = new StaticText(panel, wxString((message_split[i]).value), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    }

    //buttons
    button_a = new wxButton(panel, wxID_ANY, string_a.value, wxDefaultPosition, wxDefaultSize);
    button_a->Bind(wxEVT_BUTTON, *f_a);
    button_a->Bind(wxEVT_BUTTON, *close_frame);
    button_b = new wxButton(panel, wxID_ANY, string_b.value, wxDefaultPosition, wxDefaultSize);
    button_b->Bind(wxEVT_BUTTON, *f_b);
    button_b->Bind(wxEVT_BUTTON, *close_frame);

    panel->Bind(wxEVT_KEY_DOWN, &QuestionFrame::KeyDown<wxKeyEvent>, this);

    image = new StaticBitmap(
                             panel,
                             path_icon_file,
                             //I use ToDIP to adjust the size independently of the screen resolution
                             ToDIP(wxSize(
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
                                          ))
                             );

    //add all entries of text to sizer_v
    for (i = 0; i < text.size(); i++) {
        sizer_v->Add(text[i], 0, wxALL | wxALIGN_CENTER);
    }
    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid->Add(button_a, 0, wxALIGN_CENTER);
    sizer_grid->Add(button_b, 0, wxALIGN_CENTER);
    sizer_v->Add(sizer_grid, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_h->Add(sizer_v, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_h);
    panel->Fit();
    Fit();

    CentreOnScreen();

}




//if the user presses return/escape, I call f_a / f_b
template<typename F_A, typename F_B, typename F_ABORT> template<class E> void QuestionFrame<F_A, F_B, F_ABORT>::KeyDown(E& event) {

    wxCommandEvent dummy;

    if (((event.GetKeyCode()) == WXK_ESCAPE) && enable_button_b) {
        //the user pressed esc
        
        if(bind_esc_to_button_b){
            //esc button is bound to button_b -> call *f_b
            
            (*f_b)(dummy);
            
        }else{
            //esc button is bound to *f_abort -> call *f_abort

            
            (*f_abort)(dummy);
            
        }


    }else {

        if ((((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) && enable_button_a) {
            //the user pressed return or numpad return

            (*f_a)(dummy);

        }

    }

    (*close_frame)(dummy);


    event.Skip(true);

}


template<class T, typename FF_OK> PrintMessage<T, FF_OK>::PrintMessage(T* f_in, FF_OK* f_ok_in) {

    f = f_in;
    f_ok = f_ok_in;

}

//set the wxControl, title,  message  and image_path for the functor *this, and I call the functor operator() with CallAfter
template<class T, typename FF_OK> void PrintMessage<T, FF_OK>::SetAndCall(wxControl* control_in, String title_in, String message_in, String image_path_in) {

    control = control_in;
    title = title_in;
    message = message_in;
    image_path = image_path_in;

    f->CallAfter(*this);

}

template<class T, typename F_YES, typename F_NO, typename F_ABORT> ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::ShowQuestionFrame(T* f_in, F_YES* f_yes_in, F_NO* f_no_in, F_ABORT* f_abort_in) {

    f = f_in;
    f_yes = f_yes_in;
    f_no = f_no_in;
    f_abort = f_abort_in;

}


//set the wxControl, title and question and answers for the functor *this,  set enable_button_y/n both to true,  and bind_esc_to_button_b to true. Then call the functor operator() with CallAfter
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::SetAndCall(wxControl* control_in, String title_in, String question_in, String answer_y_in, String answer_n_in) {

    control = control_in;
    title = title_in;
    question = question_in;
    answer_y = answer_y_in;
    answer_n = answer_n_in;
    
    enable_button_a = true;
    enable_button_b = true;
    
    bind_esc_to_button_b = true;

    f->CallAfter(*this);

}


//set the wxControl, title and question and answers for the functor *this,  set enable_butoon_y/n to enable_button_y/n_in, and set bind_esc_to_button_b = bind_esc_to_button_b_in.  I call the functor operator() with CallAfter
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::SetAndCall(wxControl* control_in, String title_in, String question_in, String answer_y_in, String answer_n_in, bool enable_button_a_in, bool enable_button_b_in, bool bind_esc_to_button_b_in) {

    control = control_in;
    title = title_in;
    question = question_in;
    answer_y = answer_y_in;
    answer_n = answer_n_in;
    
    enable_button_a = enable_button_a_in;
    enable_button_b = enable_button_b_in;
    
    bind_esc_to_button_b = bind_esc_to_button_b_in;

    f->CallAfter(*this);

}


//if question_frame != NULL, enable or disable question_frame->button_a/b according to the boolean variables enable_button_a/b
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::EnableDisableButtons(void) {

    if(question_frame != NULL){
        
        question_frame->button_a->Enable(enable_button_a);
        question_frame->button_b->Enable(enable_button_b);
        
    }
    
}

template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::operator()(void) {


    SetIdling<T>* set_idling;
    UnsetIdling<T>* unset_idling;

    set_idling = new SetIdling<T>(f);
    unset_idling = new UnsetIdling<T>(f);


    if (!(f->idling)) {

        //I may be about to prompt a temporary dialog window, thus I set f->idling to true
        (*set_idling)();

        if (control != NULL) {
            //this question has been prompted from a control

            if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {

                question_frame = new QuestionFrame<F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
                question_frame->Show(true);
                question_frame->Raise();

                EnableDisableButtons();
                control->SetForegroundColour((wxGetApp().highlight_color));
                control->SetFont(wxGetApp().highlight_font);

            }

        }
        else {
            //this question has not been prompted from a control

            question_frame = new QuestionFrame<F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
            question_frame->Show(true);
            question_frame->Raise();

            EnableDisableButtons();
            
        }

    }

    //AFTER the question has been aswered and the related frame closed, I unset idling in f
    f->CallAfter(*unset_idling);

}



template<class P> CheckYear<P>::CheckYear(DateField<P>* p_in) {

    p = p_in;
    tabulate_days = new TabulateDays<P>(p);

}

template<class P> CheckMonth<P>::CheckMonth(DateField<P>* p_in) {

    p = p_in;
    tabulate_days = new TabulateDays<P>(p);

}

template<class P> CheckDay<P>::CheckDay(DateField<P>* p_in) {

    p = p_in;

}


template<class P> template<class T> void CheckYear<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->year->GetValue()).ToStdString(), NULL, false, 0, 0);

        if (check || (((p->year->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->year->GetValue()).ToStdString())) == String("")))) {
            //p->year either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set year_ok to true (false)
            (p->year_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in year
            p->year->SetForegroundColour(wxGetApp().foreground_color);
            p->year->SetFont(wxGetApp().default_font);

            if (check && (p->month_ok)) {

                (*tabulate_days)(event);

            }

            p->day->Enable(check && (p->month_ok));


        }
        else {

            f->print_error_message->SetAndCall((p->year), String("Entered value is not valid!"), String("Year must be an unsigned integer"), (wxGetApp().path_file_error_icon));

            (p->year_ok) = false;
            p->day->Enable(false);

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> template<class T> void CheckMonth<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->month->GetValue()).ToStdString(), NULL, true, 1, 12 + 1);

        if (check || (((p->month->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->month->GetValue()).ToStdString())) == String("")))) {
            //p->month either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set month_ok to true (false)
            (p->month_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in month
            p->month->SetForegroundColour(wxGetApp().foreground_color);
            p->month->SetFont(wxGetApp().default_font);

            if (check && (p->year_ok)) {

                (*tabulate_days)(event);

            }

            p->day->Enable(check && (p->year_ok));

        }
        else {

            f->print_error_message->SetAndCall((p->month), String("Entered value is not valid!"), String("Month must be an unsigned integer >= 1 and <= 12"), (wxGetApp().path_file_error_icon));

            (p->month_ok) = false;
            p->day->Enable(false);

        }


        f->AllOk();

    }

    event.Skip(true);

}


template<class P> template<class T> void CheckDay<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        //this variable = true if the day field is formatted correctly
        bool check;

        p->date->check_leap_year();

        if (p->date->Y_is_leap_year) {

            check = check_unsigned_int((p->day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi(p->month->GetValue())) - 1] + 1);

        }
        else {

            check = check_unsigned_int((p->day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi(p->month->GetValue())) - 1] + 1);

        }


        if (check || (((p->day->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->day->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set day_ok to true (false)
            (p->day_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in day
            p->day->SetForegroundColour(wxGetApp().foreground_color);
            p->day->SetFont(wxGetApp().default_font);


        }
        else {

            f->print_error_message->SetAndCall((p->day), String("Entered value is not valid!"), String("Day must be an unsigned integer comprised between the days of the relative month"), (wxGetApp().path_file_error_icon));

            (p->day_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}



template<class P> TabulateDays<P>::TabulateDays(DateField<P>* p_in) {

    p = p_in;

}



template<class P> template<class T> void TabulateDays<P>::operator()(T& event) {

    unsigned int i;
    SightFrame* f = (p->parent_frame);

    if ((p->year_ok) && (p->month_ok)) {

        wxString temp;

        //save the old value of p->day into temp
        temp = (p->day->GetValue());

        //read the year
        ((((f->sight)->master_clock_date_and_hour).date).Y) = ((unsigned int)wxAtoi(p->year->GetValue()));
        (((f->sight)->master_clock_date_and_hour).date).check_leap_year();

        //read the month
        ((((f->sight)->master_clock_date_and_hour).date).M) = ((unsigned int)wxAtoi(p->month->GetValue()));

        if (((f->sight)->master_clock_date_and_hour).date.Y_is_leap_year) {
            //in this case the year is a leap year: I fill the list of days from days_per_month_leap

            for ((p->days).Clear(), i = 0; i < days_per_month_leap[(((f->sight)->master_clock_date_and_hour).date.M) - 1]; i++) {
                (p->days).Add(wxString::Format(wxT("%i"), i + 1));
            }

        }
        else {
            //in this case the year is a common year: I fill the list of days from days_per_month_common

            for ((p->days).Clear(), i = 0; i < days_per_month_common[(((f->sight)->master_clock_date_and_hour).date.M) - 1]; i++) {
                (p->days).Add(wxString::Format(wxT("%i"), i + 1));
            }
            //
        }

        p->day->Set(p->days);

        if (!(wxAtoi(temp) <= wxAtoi((p->days)[(p->days).GetCount() - 1]))) {
            //if the value in p->day is does not lie between the boundaries of the newly set days list (list of days of the month, then I reset it by setting it to 1

            p->day->SetValue(wxString("1"));

        }
        else {

            p->day->SetValue(temp);

        }

        p->day->Enable(true);

    }
    else {

        p->day->Enable(false);

    }

    event.Skip(true);

}

template<class P, class T>  CheckCheck<P, T>::CheckCheck(CheckField<P, T>* p_in) {

    p = p_in;

}

//this function reads the value in the GUI box checkbox, and enables/disables the related_field accordingly
template<class P, class T> template<class R> void CheckCheck<P, T>::operator()(R& event) {

    //I enable/disable related_field according to whether checkbox is checked or not, and according to the value of direct_reverse
    if (((p->checkbox->GetValue()) ^ (!(p->direct_reverse)))) {
        
        p->related_field->Enable(true);
        
    }else {
        
        p->related_field->Enable(false);
        
    }

    (*(p->related_field->check))(event);

    p->parent->AllOk();

    event.Skip(true);

}


//this function writes into the non-GUI field answer the value entered in the GUI box
template<class P, class T> template<class S> void CheckField<P, T>::get(S& event) {

    //I set p->answer to the value entered in the GUI checkbox
    if (checkbox->GetValue()) {
        (answer->value) = 'y';
    }
    else {
        (answer->value) = 'n';
    }

    event.Skip(true);

}

template<class P> CheckHour<P>::CheckHour(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckHour<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24);


        if (check || ((((p->hour)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->hour)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set hour_ok to true (false)
            (p->hour_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            (p->hour)->SetForegroundColour(wxGetApp().foreground_color);
            (p->hour)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->hour), String("Entered value is not valid!"), String("Hours must be unsigned integer numbers >= 0 and < 24"), (wxGetApp().path_file_error_icon));

            (p->hour_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckMinute<P>::CheckMinute(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P>  template<class T> void CheckMinute<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60);

        if (check || ((((p->minute)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->minute)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set minute_ok to true (false)
            (p->minute_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in minute
            (p->minute)->SetForegroundColour(wxGetApp().foreground_color);
            (p->minute)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->minute), String("Entered value is not valid!"), String("Minutes must be unsigned integer numbers >= 0 and < 60"), (wxGetApp().path_file_error_icon));

            (p->minute_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckSecond<P>::CheckSecond(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckSecond<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

        if (check || ((((p->second)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->second)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set second_ok to true (false)
            (p->second_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            (p->second)->SetForegroundColour(wxGetApp().foreground_color);
            (p->second)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->second), String("Entered value is not valid!"), String("Seconds must be floating-point numbers >= 0.0 and < 60.0"), (wxGetApp().path_file_error_icon));

            (p->second_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckChrono<P>::CheckChrono(ChronoField<P>* p_in) {

    p = p_in;

    check_hour = new CheckHour<P>(p);
    check_minute = new CheckMinute<P>(p);
    check_second = new CheckSecond<P>(p);

}

//this function writes into sight.stopwatch the value written into the respective GUI box
template<class P> template <class T> void CheckChrono<P>::operator()(T& event) {

    (*check_hour)(event);
    (*check_minute)(event);
    (*check_second)(event);

    event.Skip(true);

}

template<class P> CheckRouteType<P>::CheckRouteType(RouteTypeField<P>* p_in) {

    p = p_in;


}

//this functor checks the wxComboBox containing the Route type, and if it is equal to loxodrome or orthodrome, it enables length, Z and start_* fields in RouteFrame (the latter if for_transport = false, becasue I don't need a start position if I am using the Route for transport). If it is equal to circle of equal altitude, it enables only the GP and omege fields.
template<class P> template<class T> void CheckRouteType<P>::operator()(T& event) {

    P* f = (p->parent);
    bool enable;
    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field

    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode

        unsigned int i;
        bool check;
      
        //I check whether the RouteTypeField *p of the GUI field matches one of the route types in catalog
        p->CheckInCatalog(&check, &i);

        if (check) {
            //the Route type is valid

            //enable/disable the related fields in RouteFrame f
            enable = ((((p->catalog)[i]) == wxString(((Route_types[0]).value))) || (((p->catalog)[i]) == wxString(((Route_types[1]).value))));

            f->Z->Enable(enable);

            //for start_phi/lambda to be enabled, not only the Route has to be of tyoe Route_types[0] or Route_types[1], but, in addition, it must not be a Route for transport (if it were, there would be no need to indicate its starting Position to do the transport )
            f->start_phi->Enable(enable && (!(f->for_transport)));
            f->start_lambda->Enable(enable && (!(f->for_transport)));

            f->GP_phi->Enable(!enable);
            f->GP_lambda->Enable(!enable);
            f->omega->Enable(!enable);

        }else{
            //the Route type is not valid

            f->Z->Enable(false);
            f->start_phi->Enable(false);
            f->start_lambda->Enable(false);


            f->GP_phi->Enable(false);
            f->GP_lambda->Enable(false);
            f->omega->Enable(false);

        }

        f->OnChooseLengthFormatField();

    }
    
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}


template<class P> CheckLengthFormat<P>::CheckLengthFormat(LengthFormatField<P>* p_in) {

    p = p_in;

}



template<class P> template<class T> void CheckLengthFormat<P>::operator()(T& event) {
    
    
    P* f = (p->parent);
    bool enable;

    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field
    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode

        unsigned int i;
        bool check;
      
        //I check whether the name in the GUI field body matches one of the route types  in catalog
        p->CheckInCatalog(&check, &i);

        if (check) {
            //the length format is valid

            //enable/disable the related fields in RouteFrame f
            //if the Route type is a loxodrome or orthodrome, enable = true. Otherwise, enable = false
            enable = ((((p->catalog)[i]) == wxString(((Route_types[0]).value))) || (((p->catalog)[i]) == wxString(((Route_types[1]).value))));

            f->Z->Enable(enable);

            f->start_phi->Enable(!(f->for_transport));
            f->start_lambda->Enable(!(f->for_transport));

            f->GP_phi->Enable(!enable);
            f->GP_lambda->Enable(!enable);
            f->omega->Enable(!enable);

        }else {
            //the length format is not valid

            f->Z->Enable(false);
            f->start_phi->Enable(false);
            f->start_lambda->Enable(false);

            f->GP_phi->Enable(false);
            f->GP_lambda->Enable(false);
            f->omega->Enable(false);

        }

        f->OnChooseLengthFormatField();
        
    }
    
     
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}



template<class S> OnChangeSelectionInListControl<S>::OnChangeSelectionInListControl(ListControl<S>* caller_in, String type_in) {

    caller = caller_in;
    type = type_in;

}


//check whether there are some selected items in ListControl *caller, and enables/disables disableable_buttons accordingly
template<class S> template<class T>void OnChangeSelectionInListControl<S>::operator()(T& event) {
    
    long i;
    //a pointer to the ListFrame
    ListFrame* list_frame_pointer;
    
    list_frame_pointer = ((ListFrame*)(caller->GetParent()->GetParent()));
    
    for (i = 0; i < (caller->disableable_buttons).size(); i++) {
        (caller->disableable_buttons)[i]->Enable(caller->GetSelectedItemCount() != 0);
    }
    
    i = (caller->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
    
    if (i != -1) {
        //one item is selected
        
        switch (type.position_in_list(ListControl_types)) {
                
            case 0: {
                //type.value = "sight"->caller is listcontrol_sights
                
                //the selected Sight has/does not have a related Route -> enable/disable the buttons to disconnect and transport the selected Sight
                list_frame_pointer->button_disconnect_sight->Enable(((((list_frame_pointer->data->sight_list)[i]).related_route.value) != -1));
                list_frame_pointer->button_transport_sight->Enable(((((list_frame_pointer->data->sight_list)[i]).related_route.value) != -1));
                
                break;
                
            }
                
            case 2: {
                //type.value = "route" -> caller is listcontrol_routes
                
                //the selected Route has/does not have a related Sight -> enable/disable the button to disconnect the selected Route
                list_frame_pointer->button_disconnect_route->Enable(((((list_frame_pointer->data->route_list)[i]).related_sight.value) != -1));
                
                break;
                
            }
                
        }
        
    }
    else {
        //no item is selected -> disable the button to disconnect
        
        switch (type.position_in_list(ListControl_types)) {
                
            case 0: {
                //type.value = "sight"->caller is listcontrol_sights
                
                list_frame_pointer->button_disconnect_sight->Enable(false);
                
                break;
                
            }
                
            case 2: {
                //type.value = "route" -> caller is listcontrol_routes
                
                list_frame_pointer->button_disconnect_route->Enable(false);
                
                break;
                
            }
                
        }
        
        
    }
    
    event.Skip(true);
    
}


template<class P> OnChangeSelectionInLimbField<P>::OnChangeSelectionInLimbField(LimbField<P>* caller_in) {

    caller = caller_in;

}

//when an item is selected/deselcted in *caller, make sure that only one item stays selected in *calller
template<class P> template<class T>void OnChangeSelectionInLimbField<P>::operator()(T& event) {

    wxArrayInt temp;
    long i, j;

    temp.Clear();
    caller->name->GetCheckedItems(temp);


    if ((temp.GetCount()) <= 1) {
        //only one item is selected -> write temp into caller->selected_items

        caller->checked_items.Clear();
        (caller->checked_items) = temp;

    }
    else {
        //multiple items are selecte -> check for the newly selected item, and write it as the only element  into checked_items

        for (i = 0; i < temp.GetCount(); i++) {

            for (j = 0; (j < (caller->checked_items).GetCount()) && ((caller->checked_items.Item(j)) != (temp.Item(i))); j++) {}

            if (j == (caller->checked_items).GetCount()) {
                //the item temp.Item(i) is not present in (caller->checked_items) -> resize caller->checked_items to size one and write into its only element temp.Item(i)

                caller->checked_items.Clear();
                caller->checked_items.Add(temp.Item(i), 1);

                break;

            }
        }

    }

    //update the checked items in caller->name
    if ((caller->checked_items.GetCount()) == 0) {
        //checked_items is empty->uncheck everythig in name

        for (i = 0; i < (caller->limbs.size()); i++) {
            caller->name->Check(((unsigned int)i), false);
        }

    }
    else {
        //checked_items is not empty->check, in name, only the first element in checked_items

        for (i = 0; i < (caller->limbs.size()); i++) {
            caller->name->Check(((unsigned int)i), (i == (caller->checked_items.Item(0))));
        }

    }



    (caller->ok) = ((caller->checked_items.GetCount()) == 1);

    if (caller->ok) {

        caller->name->SetForegroundColour(wxGetApp().foreground_color);
        caller->name->SetFont(wxGetApp().default_font);

    }

    //tries to enable button_reduce
    caller->parent_frame->AllOk();

    event.Skip(true);

}


//this function writes into the non-GUI fields in chrono the value written into the respective GUI fields hour, minute and second
template<class P> template <class T> void ChronoField<P>::get(T& event) {

    if (hour_ok && (hour->IsEnabled()) && minute_ok && (minute->IsEnabled()) && second_ok && (second->IsEnabled())) {
        //I write only if hour, minute and second are ok and enabled

        double s_temp;

        (chrono->h) = ((unsigned int)wxAtoi(hour->GetValue()));
        ((chrono)->m) = ((unsigned int)wxAtoi(minute->GetValue()));
        ((second)->GetValue()).ToDouble(&s_temp);
        ((chrono)->s) = s_temp;

    }

    event.Skip(true);

}



//constructor of a ProjectionField object, based on the parent frame frame
template<class P> ProjectionField<P>::ProjectionField(
                                                      wxPanel* panel_of_parent,
                                                      vector<int>* recent_items_in) : MultipleItemField<P, Projection, void>(
                                                                                                                             panel_of_parent, 
                                                                                                                             NULL,
                                                                                                                             convert_vector<Projection, String>(Projection_types),
                                                                                                                             recent_items_in) {


    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, Projection, void>::name->Bind(wxEVT_COMBOBOX, &ProjectionField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, Projection, void>::name->Bind(wxEVT_KEY_UP, &ProjectionField::OnEdit<wxKeyEvent>, this);

}


//update the GUI dropdown menu of MultipleItemField according to MultipleItemField<P, NON_GUI>::recent_items in such a way that the recent items appear on top
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::Fill(void) {

    unsigned int i, j;
    wxArrayString items_temp;
    wxString name_temp;
    bool is_present;

    //save the current value of name in name_temp
    name_temp = (MultipleItemField<P, NON_GUI, CHECK>::name->GetValue());
    //create the temporary list of projections projections_temp from catalog
    for (items_temp.Clear(), i = 0; i < MultipleItemField<P, NON_GUI, CHECK>::items.GetCount(); i++) {
        items_temp.Add((MultipleItemField<P, NON_GUI, CHECK>::catalog)[i]);
    }

    //I first add to the items vector the recently selected items written in (MultipleItemField<P, NON_GUI>::recent_items)
    for (MultipleItemField<P, NON_GUI, CHECK>::items.Clear(), i = 0; i < (MultipleItemField<P, NON_GUI, CHECK>::recent_items->size()); i++) {

        MultipleItemField<P, NON_GUI, CHECK>::items.Add(items_temp[(*(MultipleItemField<P, NON_GUI, CHECK>::recent_items))[i]]);

    }

    //then, I fill the items vector with the remaining items
    for (i=0; i < items_temp.GetCount(); i++) {

        for (is_present = false, j = 0; (j < MultipleItemField<P, NON_GUI, CHECK>::items.GetCount()) && (!is_present); j++) {

            if (MultipleItemField<P, NON_GUI, CHECK>::items[j] == items_temp[i]) {
                is_present = true;
            }

        }

        if (!is_present) {
            MultipleItemField<P, NON_GUI, CHECK>::items.Add(items_temp[i]);
        }

    }

    MultipleItemField<P, NON_GUI, CHECK>::name->Set(MultipleItemField<P, NON_GUI, CHECK>::items);
    //because name->Set(projections clears the value of name, I set the value of name back to name_temp
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue(name_temp);
    //given that I just filled name with a valid item, I store this item in value_before_editing
    value_before_editing = name->GetValue();
    
    
    items_temp.Clear();

}

//return true(false) is *this is ok (not ok), i.e., if this->ok = true(false)
template<class P, class NON_GUI, class CHECK> bool MultipleItemField<P, NON_GUI, CHECK>::is_ok(void) {

    return(ok);

}


//this function enables/disable the LengthFormatField
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::Enable(bool is_enabled) {

    name->Enable(is_enabled);

}



//constructor of a LengthFormatField object, based on the parent frame frame
template<class P> LengthFormatField<P>::LengthFormatField(wxPanel* panel_of_parent, LengthFormat* object_in, vector<int>* recent_items_in)  : MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >(panel_of_parent, object_in, LengthFormat_types, recent_items_in){

    
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::check = new CheckLengthFormat<P>(this);
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::check));

    
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_COMBOBOX, &LengthFormatField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_KEY_UP, &LengthFormatField::OnEdit<wxKeyEvent>, this);

}


//this function is called every time the user modifies the text in this->name: it checks whether the text entered so far in name is valid, if name is valid, it calls OnChooseLengthFormat to select the projection written in name
template<class P> template<class E> void LengthFormatField<P>::OnEdit(E& event) {

    String s;
    bool success;

    //I check whether the name in the GUI field body matches one of the body names in catalog
    find_and_replace_case_insensitive(MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name, MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::items, &success, NULL);

    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::ok = success;

    if (success) {

        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->SetFont(wxGetApp().default_font);
        //choses the length format entered in name button_reduce
        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::parent->OnChooseLengthFormatField(event);

    }

    event.Skip(true);

}


LengthFormat::LengthFormat(void) : String() {


}


LengthFormat::LengthFormat(string input) : String(input) {


}

RouteType::RouteType(void) : String() {}

RouteType::RouteType(const String& input) : String(input.value) {}

RouteType::RouteType(const string& input) : String(input) {}



Projection::Projection(void) : String() {}


Projection::Projection(string input) : String(input) {}





//sets the value in the GUI object check equal to the value in the non-GUI limb object answer
template<class P, class T> void CheckField<P, T>::set(void) {

    if ((answer->value) == 'y') {
        checkbox->SetValue(true);
    }

    if ((answer->value) == 'n') {
        checkbox->SetValue(false);
    }

}

template<class P, class T> template <typename EventTag, typename Method, typename Object> void CheckField<P, T>::Bind(EventTag tag, Method method, Object object) {

    checkbox->Bind(tag, method, object);

}






//set the value and unit of measure in the GUI field *this equal to the value and the unit of measure in the non-GUI object *input
template<class P> void DynamicLengthField<P>::set(Length input) {
        
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, input.value));
    LengthField<P>::unit->set(input.unit);
    
}


//set the value in the GUI object value equal to the value in the non-GUI object length
template<class P> void DynamicLengthField<P>::set(void) {
    
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, LengthField<P>::length->value));
    value_ok = true;
    
    LengthField<P>::unit->set();

}

//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date_in
template<class P> void DateField<P>::set(Date date_in) {

    //    Time time_UTC;
    //
    //    //(parent_frame->sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    //    time_UTC = (parent_frame->sight->time);
    //    time_UTC -= (parent_frame->sight->TAI_minus_UTC);

    year->SetValue(wxString::Format(wxT("%i"), date_in.Y));
    month->SetValue(wxString::Format(wxT("%i"), date_in.M));
    day->SetValue(wxString::Format(wxT("%i"), date_in.D));

    year_ok = true;
    month_ok = true;
    day_ok = true;

}

//set color as the background color in all fields of *this
template<class P> void DateField<P>::SetBackgroundColor(Color color) {

    year->SetBackgroundColour(color);
    month->SetBackgroundColour(color);
    day->SetBackgroundColour(color);

}

//set the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object *chrono
template<class P> void ChronoField<P>::set(void) {

    set(*chrono);

}

//set the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object chrono_in
template<class P> void ChronoField<P>::set(Chrono chrono_in) {

    hour->SetValue(wxString::Format(wxT("%i"), chrono_in.h));
    minute->SetValue(wxString::Format(wxT("%i"), chrono_in.m));
    second->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, chrono_in.s));

    hour_ok = true;
    minute_ok = true;
    second_ok = true;

}

//set color as the background color in all fields of *this
template<class P> void ChronoField<P>::SetBackgroundColor(Color color) {

    hour->SetBackgroundColour(color);
    minute->SetBackgroundColour(color);
    second->SetBackgroundColour(color);

}

//sets the value in the GUI object equal to the value in the non-GUI  object string
template<class P> void RouteTypeField<P>::set(void) {
    
    switch (/*( (MultipleItemField<P, RouteType, CheckRouteType<P> >::object->value)[0])*/ (MultipleItemField<P, RouteType, CheckRouteType<P> >::object)->position_in_list(Route_types)) {
            
        case 0: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[0]).value));
            
            break;
            
        }
            
        case 1: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[1]).value));
            
            break;
            
        }
            
        case 2: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[2]).value));
            
            break;
            
        }
            
    }
    
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = true;
    
}

//sets the value in the GUI object value equal to the value in the non-GUI String object string
template<class P> void StringField<P>::set(void) {

    value->SetValue(wxString(string->value));

}


//constructor of a CheckField object, based on the parent frame frame
template<class P, class T> CheckField<P, T>::CheckField(wxPanel* panel_of_parent, Answer* p, T* related_field_in, bool direct_reverse_in) {

    parent = ((P*)(panel_of_parent->GetParent()));
    //I link the internal pointers p and c to the respective Answer object
    answer = p;
    related_field = related_field_in;
    direct_reverse = direct_reverse_in;

    check = new CheckCheck<P, T>(this);

    checkbox = new wxCheckBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    //SetColor(checkbox);
    checkbox->Bind(wxEVT_CHECKBOX, (*check));

    checkbox->SetValue(false);

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(checkbox, 0, wxALIGN_CENTER);

}





//constructor of a LengthField object, based on the parent frame frame
template<class P> LengthField<P>::LengthField(wxPanel* panel_of_parent, Length* p){

    parent = ((P*)(panel_of_parent->GetParent()));
    length = p;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);

}


//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template<class E> void LengthField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in units
    find_and_replace_case_insensitive(unit->name, unit->catalog, &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        unit->name->SetForegroundColour(wxGetApp().foreground_color);
        unit->name->SetFont(wxGetApp().default_font);

    }

    (unit->ok) = success;

    event.Skip(true);

}


//insert *this in *host
template<class P> template<class T> void LengthField<P>::InsertIn(T* host) {

    host->Add(LengthField<P>::sizer_v);

}


//same asLengthField<P>::InsertIn(T* host) but with flags to be provided
template<class P> template<class T> void LengthField<P>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}


//constructor of a EditableLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> DynamicLengthField<P>::DynamicLengthField(wxPanel* panel_of_parent, Length* p) : LengthField<P>( panel_of_parent, p) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    
    //initialize check
    check = new CheckLength<P>(this);
    
    value = new wxTextCtrl((LengthField<P>::parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Length object
    value->SetValue(wxString(""));
    value_ok = false;
    value->Bind(wxEVT_KILL_FOCUS, (*(check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
//    value->Bind(wxEVT_KEY_UP, &DynamicLengthField::OnEditValue<wxKeyEvent>, this);
    
    LengthField<P>::unit = new LengthUnitField<P>((LengthField<P>::parent->panel), &(LengthField<P>::length->unit), &(wxGetApp().list_frame->data->recent_length_units));
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
//    LengthField<P>::unit->name->Bind(wxEVT_COMBOBOX, &LengthField<P>::template OnEditUnit<wxCommandEvent>, this);
//    LengthField<P>::unit->name->Bind(wxEVT_KEY_UP, &LengthField<P>::template OnEditUnit<wxKeyEvent>, this);

    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    LengthField<P>::sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    LengthField<P>::unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::template InsertIn<wxBoxSizer>(LengthField<P>::sizer_h, flags);

}


//constructor of a StaticLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> StaticLengthField<P>::StaticLengthField(wxPanel* panel_of_parent, Length* p) : LengthField<P>( panel_of_parent, p) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    
    value = new StaticText((LengthField<P>::parent->panel),  "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetLabel(wxString(""));
    
    LengthField<P>::unit = new LengthUnitField<P>((LengthField<P>::parent->panel), &(LengthField<P>::length->unit), &(wxGetApp().list_frame->data->recent_length_units));
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    LengthField<P>::unit->Bind(wxEVT_COMBOBOX, &LengthField<P>::template OnEditUnit<wxCommandEvent>, this);
    LengthField<P>::unit->Bind(wxEVT_KEY_UP, &LengthField<P>::template OnEditUnit<wxKeyEvent>, this);

    LengthField<P>::unit->Bind(wxEVT_COMBOBOX, &StaticLengthField<P>:: template ConvertUnit<wxCommandEvent>, this);
    LengthField<P>::unit->Bind(wxEVT_KEY_UP, &StaticLengthField<P>::template ConvertUnit<wxKeyEvent>, this);
    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    LengthField<P>::sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    LengthField<P>::unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::template InsertIn<wxBoxSizer>(LengthField<P>::sizer_h, flags);

}


//set the value and the unit of the GUI field *this equal to the value and the unit in the non-GUI object input
template<class P> void StaticLengthField<P>::set(const Length& l) {
    
    value->SetLabel(wxString::Format(wxT("%.*f"), display_precision.value, l.value));
    LengthField<P>::unit->set(l.unit);
    
}


//set the value and the unit in the GUI object value equal to the value and the unit in the non-GUI object length
template<class P> void StaticLengthField<P>::set(void) {

    set(*(LengthField<P>::length));

    (LengthField<P>::unit->ok) = true;

}


//convert *length to the unis of measure in the GUI field *this and write the result in *this
template<class P> void StaticLengthField<P>::set_value_keep_unit(void) {
    
    Length temp;
    
    temp = (LengthField<P>::length->convert(LengthUnit(LengthField<P>::unit->name->GetValue().ToStdString())));
    
    set(temp);

}


//convert the numerical value stored into value according to the length unit unit 
template<class P> template<class E>  void StaticLengthField<P>::ConvertUnit(E& event) {
    
  
    event.Skip(true);

}


//constructor of a StringField object, based on the parent frame frame
template<class P> StringField<P>::StringField(wxPanel* panel_of_parent, String* p) {

    parent = ((P*)(panel_of_parent->GetParent()));
    string = p;

    //initialize check
    check = new CheckString<P>(this);

    (set_to_current_time.p) = this;

    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_string_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, (*check));

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);

}



template<class P> bool DynamicLengthField<P>::is_ok(void) {

    return(value_ok && (LengthField<P>::unit->ok));

}


template<class P> bool ChronoField<P>::is_ok(void) {

    return(hour_ok && minute_ok && second_ok);

}

//this function is called every time a keyboard button is lifted in this->hour: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditHour(E& event) {

    bool success;

    success = check_unsigned_int((hour->GetValue()).ToStdString(), NULL, true, 0, 24);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        hour->SetForegroundColour(wxGetApp().foreground_color);
        hour->SetFont(wxGetApp().default_font);

    }


    //hour_ok is true/false is the text entered is valid/invalid
    hour_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->minute: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditMinute(E& event) {

    bool success;

    success = check_unsigned_int((minute->GetValue()).ToStdString(), NULL, true, 0, 60);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        minute->SetForegroundColour(wxGetApp().foreground_color);
        minute->SetFont(wxGetApp().default_font);

    }


    //minute_ok is true/false is the text entered is valid/invalid
    minute_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->second: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditSecond(E& event) {

    bool success;

    success = check_double((second->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        second->SetForegroundColour(wxGetApp().foreground_color);
        second->SetFont(wxGetApp().default_font);

    }


    //second_ok is true/false is the text entered is valid/invalid
    second_ok = success;
    //tries to enable button_ok
    parent_frame->AllOk();

    event.Skip(true);

}



template<class P> template <typename EventTag, typename Method, typename Object> void ChronoField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind hour, minute and second to method
    hour->Bind(tag, method, object);
    minute->Bind(tag, method, object);
    second->Bind(tag, method, object);

}


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void DynamicLengthField<P>::OnEditValue(E& event) {

    bool success;

    success = check_double((value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        value->SetForegroundColour(wxGetApp().foreground_color);
        value->SetFont(wxGetApp().default_font);

    }


    //value_ok is true/false is the text entered is valid/invalid
    value_ok = success;
    
    //tries to enable button_reduce
//    LengthField<P>::parent->AllOk();

    event.Skip(true);

}


template<class P> template <typename EventTag, typename Method, typename Object> void DynamicLengthField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    LengthField<P>::unit->Bind(tag, method, object);

}


//constructor of a SpeedField object, based on the parent frame frame
template<class P> SpeedField<P>::SpeedField(wxPanel* panel_of_parent, Speed* p) {

    parent = ((P*)(panel_of_parent->GetParent()));
    speed = p;
    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    
    //initialize check
    check = new CheckSpeed<P>(this);

    flags.Center();

    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Speed object
    value->SetValue(wxString(""));
    value_ok = false;
    value->Bind(wxEVT_KILL_FOCUS, (*(check->check_speed_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    value->Bind(wxEVT_KEY_UP, &SpeedField::OnEditValue<wxKeyEvent>, this);


//    (unit->name) = new wxComboBox((parent->panel), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, units, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    unit = new SpeedUnitField<P>(parent->panel, &(speed->unit), &(wxGetApp().list_frame->data->recent_speed_units));
    
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    unit->Bind(wxEVT_COMBOBOX, &SpeedField::OnEditUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &SpeedField::OnEditUnit<wxKeyEvent>, this);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    unit->InsertIn(sizer_h, flags);
    
}


//set the value in the GUI object value equal to the value in the non-GUI object speed
template<class P> void SpeedField<P>::set(void) {
        
    value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, SpeedField<P>::speed->value));
    value_ok = true;
    
    SpeedField<P>::unit->set();
  
}


//write the value and the unit of the GUI field in SpeedField into the non-GUI field speed
template<class P> template <class T> void SpeedField<P>::get(T& event) {

    if(is_ok()){
        
        double x;
        
        value->GetValue().ToDouble(&x);
        speed->set(x, SpeedUnit((unit->name->GetValue()).ToStdString()));
        
    }

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditValue(E& event) {

    bool success;

    success = check_double((value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        value->SetForegroundColour(wxGetApp().foreground_color);
        value->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    value_ok = success;
    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in (unit->catalog)
    find_and_replace_case_insensitive(unit->name, (unit->catalog), &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        unit->name->SetForegroundColour(wxGetApp().foreground_color);
        unit->name->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    (unit->ok) = success;
    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//enable/disable the SpeedField
template<class P> void SpeedField<P>::Enable(bool is_enabled) {

    value->Enable(is_enabled);
    unit->Enable(is_enabled);

}


template<class P> template<class T> void SpeedField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P> template <typename EventTag, typename Method, typename Object> void SpeedField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}


template<class P> bool SpeedField<P>::is_ok(void) {

    return(value_ok && (unit->ok));

}


//constructor of a DateField object, based on the parent frame frame
template<class P> DateField<P>::DateField(wxPanel* panel_of_parent, Date* p) {

    unsigned int i;
    parent_frame = ((P*)(panel_of_parent->GetParent()));
    date = p;


    //initialize check and its objects
    check = new CheckDate<P>(this);
    //    (check.p) = this;
    //    (check->check_year->p) = this;
    //    ((check->check_year->tabulate_days).p) = this;
    //    (check->check_month->p) = this;
    //    ((check->check_month->tabulate_days).p) = this;
    //    (check->check_day->p) = this;

    for (months.Clear(), months.Add(wxT("")), i = 0; i < 12; i++) {
        months.Add(wxString::Format(wxT("%i"), i + 1));
    }

    year = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(year);
    year->SetInitialSize(year->GetSizeFromTextSize(year->GetTextExtent(wxS("0000"))));
    year->SetValue(wxString(""));
    year_ok = false;
    year->Bind(wxEVT_KILL_FOCUS, *(check->check_year));
    //as text is changed year by the user with the keyboard, call OnEditYear
    year->Bind(wxEVT_KEY_UP, &DateField::OnEditYear<wxKeyEvent>, this);


    text_hyphen_1 = new StaticText((parent_frame->panel), wxT("-"), wxDefaultPosition, wxDefaultSize, 0);
    month = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(month);
    AdjustWidth(month);
    month->SetValue(wxString(""));
    month_ok = false;
    month->Bind(wxEVT_KILL_FOCUS, *(check->check_month));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    month->Bind(wxEVT_COMBOBOX, &DateField::OnEditMonth<wxCommandEvent>, this);
    month->Bind(wxEVT_KEY_UP, &DateField::OnEditMonth<wxKeyEvent>, this);

    text_hyphen_2 = new StaticText((parent_frame->panel), wxT("-"), wxDefaultPosition, wxDefaultSize, 0);

    day = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(day);
    //I create a temporary days list to set the size of the wxComboBox day with AdjustWidth, and then destroy this temporary days list
    for (days.Clear(), i = 0; i < 31; i++) {
        days.Add(wxString::Format(wxT("%i"), i + 1));
    }
    day->Set(days);
    AdjustWidth(day);
    days.Clear();
    day->SetValue(wxString(""));
    day_ok = false;
    day->Bind(wxEVT_KILL_FOCUS, *(check->check_day));
    //as text is changed in day from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEditDay
    day->Bind(wxEVT_COMBOBOX, &DateField::OnEditDay<wxCommandEvent>, this);
    day->Bind(wxEVT_KEY_UP, &DateField::OnEditDay<wxKeyEvent>, this);





    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(year, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_1, wxALIGN_CENTER);
    sizer_h->Add(month, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_2, wxALIGN_CENTER);
    sizer_h->Add(day, 0, wxALIGN_CENTER);

}

//constructor of a ChronoField object, based on the parent frame frame
template<class P> ChronoField<P>::ChronoField(wxPanel* panel_of_parent, Chrono* p) {

    unsigned int i;
    parent_frame = ((P*)(panel_of_parent->GetParent()));
    chrono = p;

    check = new CheckChrono<P>(this);
    //    (check.p) = this;

    for (hours.Clear(), hours.Add(wxT("")), i = 0; i < 24; i++) {
        hours.Add(wxString::Format(wxT("%i"), i));
    }
    for (minutes.Clear(), minutes.Add(wxT("")), i = 0; i < 60; i++) {
        minutes.Add(wxString::Format(wxT("%i"), i));
    }

    hour = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(hour);
    //    hour->SetInitialSize(hour->GetSizeFromTextSize(hour ->GetTextExtent(wxS("00"))));
    AdjustWidth(hour);
    hour->SetValue(wxString(""));
    hour_ok = false;
    hour->Bind(wxEVT_KILL_FOCUS, *(check->check_hour));
    //as text is changed in hour from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    hour->Bind(wxEVT_COMBOBOX, &ChronoField::OnEditHour<wxCommandEvent>, this);
    hour->Bind(wxEVT_KEY_UP, &ChronoField::OnEditHour<wxKeyEvent>, this);

    static_text_1 = new StaticText((parent_frame->panel), wxT("h"), wxDefaultPosition, wxDefaultSize, 0);

    minute = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(minute);
    AdjustWidth(minute);
    //    minute->SetInitialSize(minute->GetSizeFromTextSize(minute->GetTextExtent(wxS("00"))));
    minute->SetValue(wxString(""));
    minute_ok = false;
    minute->Bind(wxEVT_KILL_FOCUS, *(check->check_minute));
    //as text is changed in minute from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    minute->Bind(wxEVT_COMBOBOX, &ChronoField::OnEditMinute<wxCommandEvent>, this);
    minute->Bind(wxEVT_KEY_UP, &ChronoField::OnEditMinute<wxKeyEvent>, this);

    static_text_2 = new StaticText((parent_frame->panel), wxT("m"), wxDefaultPosition, wxDefaultSize, 0);

    second = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    second->SetInitialSize(second->GetSizeFromTextSize(second->GetTextExtent(wxS(sample_width_floating_point_field))));
    //SetColor(second);
    second->SetValue(wxString(""));
    second_ok = false;
    second->Bind(wxEVT_KILL_FOCUS, *(check->check_second));
    //as text is changed in second from the user with either a keyboard button, call OnEditSecond
    second->Bind(wxEVT_KEY_UP, &ChronoField::OnEditSecond<wxKeyEvent>, this);
    static_text_3 = new StaticText((parent_frame->panel), wxT("s"), wxDefaultPosition, wxDefaultSize, 0);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(hour, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_1, 0, wxALIGN_CENTER);
    sizer_h->Add(minute, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_2, 0, wxALIGN_CENTER);
    sizer_h->Add(second, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_3, 0, wxALIGN_CENTER);

}

//constructor of a RouteTypeField object, based on the parent frame frame
template<class P> RouteTypeField<P>::RouteTypeField(wxPanel* panel_of_parent, RouteType* object_in, const vector<String>& route_types_in, vector<int>* recent_items_in) : MultipleItemField<P, RouteType, CheckRouteType<P> >(panel_of_parent, object_in, route_types_in, recent_items_in) {

    MultipleItemField<P, RouteType, CheckRouteType<P> >::check = new CheckRouteType<P>(this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, RouteType, CheckRouteType<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_COMBOBOX, &RouteTypeField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KEY_UP, &RouteTypeField::OnEdit<wxKeyEvent>, this);

}



//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> LengthUnitField<P>::LengthUnitField(wxPanel* panel_of_parent, LengthUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >(panel_of_parent, object_in, LengthUnit_types, recent_items_in) {

}
 

//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> SpeedUnitField<P>::SpeedUnitField(wxPanel* panel_of_parent, SpeedUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, SpeedUnit, CheckLengthUnit<P> >(panel_of_parent, object_in, SpeedUnit_types, recent_items_in) {

}




//this function enables/disable the DynamicLengthField
template<class P> void DynamicLengthField<P>::Enable(bool is_enabled) {

    value->Enable(is_enabled);
    LengthField<P>::unit->Enable(is_enabled);

}


//this function enables/disable the whole ChronoField
template<class P> void ChronoField<P>::Enable(bool is_enabled) {

    hour->Enable(is_enabled);
    minute->Enable(is_enabled);
    second->Enable(is_enabled);

}


template<class P> bool DateField<P>::is_ok(void) {

    return(year_ok && month_ok && day_ok);

}


//this function is called every time a keyboard button is lifted in this->year: it checks whether the text entered so far in year is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditYear(E& event) {

    bool success;

    success = check_unsigned_int((year->GetValue()).ToStdString(), NULL, false, 0, 0);

    if (success) {

        year->SetForegroundColour(wxGetApp().foreground_color);
        year->SetFont(wxGetApp().default_font);

    }

    //year_ok is true/false is the text enteres is valid/invalid
    year_ok = success;

    (*(((this->check)->check_month)->tabulate_days))(event);

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->month: it checks whether the text entered so far in month is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditMonth(E& event) {

    bool success;

    success = check_unsigned_int((month->GetValue()).ToStdString(), NULL, true, 1, 12 + 1);

    if (success) {

        month->SetForegroundColour(wxGetApp().foreground_color);
        month->SetFont(wxGetApp().default_font);

    }

    //month_ok is true/false is the text enteres is valid/invalid
    month_ok = success;

    (*(((this->check)->check_month)->tabulate_days))(event);

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->day: it checks whether the text entered so far in day is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditDay(E& event) {

    bool success;

    date->check_leap_year();

    if (date->Y_is_leap_year) {

        success = check_unsigned_int((day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi(month->GetValue())) - 1] + 1);

    }
    else {

        success = check_unsigned_int((day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi(month->GetValue())) - 1] + 1);

    }

    if (success) {

        day->SetForegroundColour(wxGetApp().foreground_color);
        day->SetFont(wxGetApp().default_font);

    }

    //day_ok is true/false is the text enteres is valid/invalid
    day_ok = success;

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}



template<class P> template <typename EventTag, typename Method, typename Object> void DateField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind year, month and day to method
    year->Bind(tag, method, object);
    month->Bind(tag, method, object);
    day->Bind(tag, method, object);

}


//template<class P> bool RouteTypeField<P>::is_ok(void) {
//
//    return(MultipleItemField<P, RouteType, CheckRouteType<P> >::ok);
//
//}


//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void RouteTypeField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success, enable;


    //I check whether the name in the GUI field  matches one of the entries  of catalog
    find_and_replace_case_insensitive(MultipleItemField<P, RouteType, CheckRouteType<P> >::name, MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog, &success, &i);

    if (success) {
        //the text entered in name is valid

        //enable/disable the related fields in RouteFrame f
        enable = ((((MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog)[i]) == wxString(((Route_types[0]).value))) || (((MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog)[i]) == wxString(((Route_types[1]).value))));

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->Z->Enable(enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_phi->Enable(enable && (!(MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->for_transport)));
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_lambda->Enable(enable && (!(MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->for_transport)));

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_phi->Enable(!enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_lambda->Enable(!enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->omega->Enable(!enable);

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetFont(wxGetApp().default_font);

    }
    else {

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->Z->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_phi->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_lambda->Enable(false);

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_phi->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_lambda->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->omega->Enable(false);

    }

    //ok is true/false is the text entered is valid/invalid
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = success;

    //try to enable time, speed and length
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->OnChooseLengthFormatField(event);

    //try to enable button_reduce
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->AllOk();

    event.Skip(true);

}

template<class P> void RouteTypeField<P>::OnEdit(void){
    
    wxCommandEvent dummy;
    
    OnEdit(dummy);
    
    
}


template<class P, class T> template<class R> void CheckField<P, T>::InsertIn(R* host) {

    host->Add(sizer_v);

}




template<class P> template<class T> void DateField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P> template<class T> void StringField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

//bind this -> value to method
template <class P> template <typename EventTag, typename Method, typename Object> void StringField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind value to method
    value->Bind(tag, method, object);

}


template<class P> template<class T> void ChronoField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

//template<class P> template<class T> void RouteTypeField<P>::InsertIn(T* host) {
//
//    host->Add(MultipleItemField<P, RouteType, CheckRouteType<P> >::sizer_v);
//
//}


//this function is called every time the user modifies the text in this->name: it checks whether the text entered so far in name is valid, if name is valid, it calls OnChooseProjection to select the projection written in name
template<class P> template<class E> void ProjectionField<P>::OnEdit(E& event) {

    String s;
    bool success;
    
    if(!(MultipleItemField<P, Projection, void>::editing)){
        //*the user has started editing *this 
        (MultipleItemField<P, Projection, void>::editing) = true;
    }

    //I check whether the name in the GUI field body matches one of the body names in catalog
    find_and_replace_case_insensitive(MultipleItemField<P, Projection, void>::name, MultipleItemField<P, Projection, void>::items, &success, NULL);

    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, Projection, void>::ok = success;

    if (success) {

        MultipleItemField<P, Projection, void>::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, Projection, void>::name->SetFont(wxGetApp().default_font);
        //choose the projection entered in name button_reduce
//        MultipleItemField<P, Projection, void>::parent->draw_panel->OnChooseProjection(event);

    }


    event.Skip(true);

}


OnSelectRouteInListControlRoutesForTransport::OnSelectRouteInListControlRoutesForTransport(ListFrame* f_in) {

    parent = f_in;

}

OnNewRouteInListControlRoutesForTransport::OnNewRouteInListControlRoutesForTransport(ListFrame* f_in) {

    parent = f_in;

}


template<class S> ListControl<S>::ListControl(wxWindow* parent_in, vector<wxButton*> disableable_buttons_in, const wxPoint& pos, const wxSize& size, long style) : wxListCtrl(parent_in, wxID_ANY, pos, size, style) {

    disableable_buttons = disableable_buttons_in;
    header_width.resize(0);

}

//set all columns of *this: add a first dummy column, which is not correctly sized on WIN32 (I don't know why) -> add the real columns -> remove the dummy column. The  size of column i that fits the header is stored in header_width[i]
template<class S> void ListControl<S>::SetColumns(vector<wxString> headers) {

    int i;

    for (i = 0, header_width.clear(), PushBackColumn(wxString("")); i < (headers.size()); i++) {
        PushBackColumn(headers[i]);
    }

    DeleteColumn(0);
    header_width.erase(header_width.begin());

}

//push back a column to ListControl and store the header size into header_size
template<class S> void ListControl<S>::PushBackColumn(wxString name) {

    //	wxListItem column;
    //
    //	column.SetId(GetColumnCount());
    //	column.SetText(name);
    //	column.SetAlign(wxLIST_FORMAT_LEFT);
    //	column.SetWidth(((this->GetSize()).GetWidth()) / ((this->GetColumnCount()) + 1));

    InsertColumn(GetColumnCount(), name, wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);
    header_width.push_back(GetColumnWidth(GetColumnCount() - 1));

}


//deselect all items in *this
template<class S> void ListControl<S>::DeselectAll(void) {

    int i;

    for (i = 0; i < (this->GetItemCount()); i++) {

        SetItemState(i, 0, wxLIST_STATE_SELECTED);

    }

}

//clears *this and sets all its items equal to the items in the non-GUI vector v. If keep_selected_items = true, I re-select the items in *this that were selected before ListControl::set was called (if they are compatible with the new size of *this)
template<class S> void ListControl<S>::set(vector<S> v, bool keep_selected_items) {

    unsigned int i;
    vector<long> selected_items;

    //store the selected items into selected_items
    GetSelectedItems(&selected_items);

    //set *this
    DeleteAllItems();
    for (i = 0; i < v.size(); i++) {
        (v[i]).add_to_wxListCtrl(-1, this);
    }

    if (keep_selected_items) {
        //restore selected items

        for (i = 0; i < selected_items.size(); i++) {
            if (selected_items[i] < GetItemCount()) {
                SetItemState(selected_items[i], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            }
        }

    }

}


//if check = true/false it enables/disables all disableable buttons in *this
template<class S> void ListControl<S>::EnableButtons(bool check) {

    unsigned int i;

    for (i = 0; i < disableable_buttons.size(); i++) {

        (disableable_buttons[i])->Enable(check);

    }

}

// resize the sizes of columns of *this to the maximum between the header size and the largest item size
template<class S> void ListControl<S>::Resize(vector<S> list) {

    int j, item_width;
    //a dummy listcontrol, never shown, used to set the column widths. To avoid resizing *this multiple times as the items are checked (ugly looking), I create a dummy_listcontrol whose column size is adapted to the largest item -> add all the items in list to it -> compute the item_width -> set the column width of *this as the maximum between header_width (already computed) and item_width. In this way, all columns of *this will be large enough to accomodate both its headers and items
    ListControl<S>* dummy_listcontrol;
    wxFrame* dummy_frame;

    dummy_frame = new wxFrame(NULL, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize);
    dummy_listcontrol = new ListControl<S>(dummy_frame, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

    for (j = 0; j < (this->GetColumnCount()); j++) {
        dummy_listcontrol->PushBackColumn(wxString(""));
    }

    //((ListFrame*)(GetParent()->GetParent()))->data->sight_list
    dummy_listcontrol->set(list, false);

    for (j = 0; j < (dummy_listcontrol->GetColumnCount()); j++) {
        dummy_listcontrol->SetColumnWidth(j, wxLIST_AUTOSIZE);
        item_width = (dummy_listcontrol->GetColumnWidth(j));
        SetColumnWidth(j, max(header_width[j], item_width));
    }

    dummy_listcontrol->Destroy();
    dummy_frame->Destroy();

}


//get the selected items from *this, clears and reallocate selected_items, and writes them in selected_items
template<class S> void ListControl<S>::GetSelectedItems(vector<long>* selected_items) {

    long item;

    item = -1;
    selected_items->clear();

    do {
        item = GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (item != -1) { selected_items->push_back(item); }

    } while (item != -1);


}

template<class F> MotionHandler<F>::MotionHandler(ListFrame* parent_in, const Route& transporting_route_in, F* f_in){
    

    timer = new wxTimer();

    parent = parent_in;
    transporting_route = transporting_route_in;
    f = f_in;

    t = 0;
//    timer->Bind(wxEVT_TIMER, &GraphicalObjectTransportHandler::OnTimer, this);

}


//constructor of GraphicalFeatureTransportHandler: f_in is the functor to be provided if something is supposed to be executed at the end of the transport (e.g., do another transport, show a MessageFrame, etc...). If nothing is supposed to be executed, set f_in = NULL
template<class NON_GUI, class F> GraphicalFeatureTransportHandler<NON_GUI, F>::GraphicalFeatureTransportHandler(ListFrame* parent_in, NON_GUI* object_in,  const String& type_of_transported_object_in, const Route& transporting_route_in, F* f_in) : MotionHandler<F>(parent_in, transporting_route_in, f_in){

    transported_object = object_in;
    type_of_transported_object = type_of_transported_object_in;
//    (MotionHandler<F>::transporting_route) = transporting_route_in;
//    (MotionHandler<F>::f) = f_in;
    

    (MotionHandler<F>::timer)->Bind(wxEVT_TIMER, &GraphicalFeatureTransportHandler::OnTimer, this);

}


//this method triggers the animation
template<class NON_GUI, class F> void GraphicalFeatureTransportHandler<NON_GUI, F>::operator()(void) {
    
    //the animation transport starts here
    (MotionHandler<F>::timer)->Start(
        /*animation_time is converted in milliseconds, because Start() takes its first argument in milliseconds*/
        (wxGetApp().animation_time.get()) * 60.0 * 60.0 / ((double)((wxGetApp().n_animation_steps.value) - 1)) * 1000.0,
        wxTIMER_CONTINUOUS);
    
}

//this method iterates the animation
template<class NON_GUI, class F> void GraphicalFeatureTransportHandler<NON_GUI, F>::OnTimer([[maybe_unused]] wxTimerEvent& event) {

    if(((MotionHandler<F>::t) < (wxGetApp().n_animation_steps.value))) {
        //the time parameter is undedr its maximum value

        if((MotionHandler<F>::t) == 0) {
            //I am at the beginning of the transport and *parent is not in idling mode -> proceed with the transport
            
            //set parameters back to their original value and reset listcontrol_routes to the original list of Routes
            (*((MotionHandler<F>::parent)->set_idling))();

            (MotionHandler<F>::transporting_route_temp) = (MotionHandler<F>::transporting_route);
            
            //during the transport, I disconnect DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement from mouse movements
            for (unsigned int i = 0; i < ((MotionHandler<F>::parent)->chart_frames.size()); i++) {
                (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel);
            }
            (MotionHandler<F>::parent)->listcontrol_sights->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->listcontrol_positions->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->listcontrol_routes->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->panel->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));

            

            if (type_of_transported_object == String("position")) {

                //store the starting position in geo_position_start
                (MotionHandler<F>::start) = (*((Position*)transported_object));
                //highlight the Position that is being transported
                (MotionHandler<F>::parent)->highlighted_position_now = address_position_in_vector<Position>(((Position*)transported_object), (MotionHandler<F>::parent)->data->position_list);
                
            }else {

                if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                    //store the starting reference position in geo_position_start
                    (MotionHandler<F>::start) = (((Route*)transported_object)->reference_position);
                    //highlight the Position that is being transported
                    (MotionHandler<F>::parent)->highlighted_route_now = address_position_in_vector<Route>(((Route*)transported_object), (MotionHandler<F>::parent)->data->route_list);
     

                }
                
                (MotionHandler<F>::start) = (((Route*)transported_object)->reference_position);


            }

            ((MotionHandler<F>::transporting_route_temp).reference_position) = (MotionHandler<F>::start);

            //I brind all ChartFrames to front to show the animation
            wxGetApp().ShowCharts(event);
            
            (MotionHandler<F>::t)++;

        }
        
        if((MotionHandler<F>::t) > 0){
            //the transport animation is in progress -> do the next chunk

            (MotionHandler<F>::transporting_route_temp).length.set(
                String(""),
                ((MotionHandler<F>::transporting_route).length.value) *
                (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))))
                ,
                String(""));


            if (type_of_transported_object == String("position")) {

                (*((Position*)transported_object)) = (MotionHandler<F>::start);
                ((Position*)transported_object)->transport_to((MotionHandler<F>::transporting_route_temp), String(""));

                (MotionHandler<F>::parent)->TabulatePositionsAll();
                
            }
            else {

                if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                    (((Route*)transported_object)->reference_position) = (MotionHandler<F>::start);
                    ((Route*)transported_object)->reference_position.transport_to((MotionHandler<F>::transporting_route_temp), String(""));

                }

                (MotionHandler<F>::parent)->TabulateRoutesAll();
                
            }

            (MotionHandler<F>::parent)->RefreshAll();
            //            cout << "\t\t t= " << t << "\n";
            
            (MotionHandler<F>::t)++;

        }

    }else {
        //the transport  is over

        if (type_of_transported_object == String("position")) {
            
            //do the whole transport rather than combining many little transports, to avoid rounding errors
            (*((Position*)transported_object)) = (MotionHandler<F>::start);
            //un-highlight the Position that is being transported
            (MotionHandler<F>::parent)->highlighted_position_now = -1;
            ((Position*)transported_object)->transport_to((MotionHandler<F>::transporting_route), String(""));


            //update labels
            (((Position*)transported_object)->label) = ((Position*)transported_object)->label.append(String(" transported with ")).append(((MotionHandler<F>::transporting_route).label));
            
            //update the Position information in f
            ((Position*)transported_object)->update_wxListCtrl(
                                                               address_position_in_vector<Position>(((Position*)transported_object), (MotionHandler<F>::parent)->data->position_list),
                                                               (MotionHandler<F>::parent)->listcontrol_positions
                                                               );


        }
        else {

            if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                String new_label;
                
                //un-highlight the Route that is being transported
                (MotionHandler<F>::parent)->highlighted_route_now = -1;

                //do the whole transport rather than combining many little transports, to avoid rounding errors
                (((Route*)transported_object)->reference_position) = (MotionHandler<F>::start);
                ((Route*)transported_object)->reference_position.transport_to((MotionHandler<F>::transporting_route), String(""));


                //update labels

                //the new label which will be given to the transported Route
                new_label = ((Route*)transported_object)->label.append(String(" transported with ")).append(((MotionHandler<F>::transporting_route).label));

                //set back listcontrol_routes to route_list, in order to include all Routes (not only those which are not related to a Sight)
                (MotionHandler<F>::parent)->listcontrol_routes->set(((MotionHandler<F>::parent)->data->route_list), false);

                if ((type_of_transported_object == String("sight")) || ( ((type_of_transported_object == String("route")) && ((((Route*)transported_object)->related_sight.value) != -1)) )) {
                    //I am transporting a Sight (i.e., Route related to a Sight) or I am transporting a Route that is connected to a Sight -> disconnect the Route from the sight

                    ((MotionHandler<F>::parent)->i_object_to_disconnect) = (((Route*)transported_object)->related_sight.value);
                    (MotionHandler<F>::parent)->Disconnect(event);

                }
                
            

                //change the label of *object by appending to it 'translated with [label of the translating Route]'
                (((Route*)transported_object)->label) = new_label;

            }

        }
        
        //set parameters back to their original value and reset listcontrol_routes to the original list of Routes

        (MotionHandler<F>::parent)->listcontrol_sights->set(((MotionHandler<F>::parent)->data->sight_list), false);
        (MotionHandler<F>::parent)->listcontrol_routes->set(((MotionHandler<F>::parent)->data->route_list), false);
        (MotionHandler<F>::parent)->Resize();
        //re-draw everything
        (MotionHandler<F>::parent)->PreRenderAll();
        
        //re-bind DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement once the transport is over
        for (unsigned int i = 0; i < ((MotionHandler<F>::parent)->chart_frames.size()); i++) {
            (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel);
        }
        (MotionHandler<F>::parent)->listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));

        
        //re-bind listcontrol_routes to &ListFrame::OnChangeSelectionInListControl
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *((MotionHandler<F>::parent)->on_change_selection_in_listcontrol_routes));
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_DESELECTED, *((MotionHandler<F>::parent)->on_change_selection_in_listcontrol_routes));


        if (((MotionHandler<F>::parent)->transporting_with_selected_route)) {
            //I am transporting with an existing, selected Route

            //the transport is over -> I reverse the Bind/Unbind(s) made before the transport started
            ((MotionHandler<F>::parent)->transporting_with_selected_route) = false;
            (MotionHandler<F>::parent)->listcontrol_routes->Unbind(wxEVT_LIST_ITEM_ACTIVATED, *((MotionHandler<F>::parent)->on_select_route_in_listcontrol_routes_for_transport));
            (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, (MotionHandler<F>::parent));

        }

        if (((MotionHandler<F>::parent)->transporting_with_new_route)) {
            //I am tranporting with a new Route

            ((MotionHandler<F>::parent)->transporting_with_new_route) = false;

        }

        (MotionHandler<F>::timer)->Stop();
        (*((MotionHandler<F>::parent)->unset_idling))();
        
        //call the functor to be called at the end of the animation, if any
        if((MotionHandler<F>::f) != NULL){
            (*(MotionHandler<F>::f))();
        }

    }

}


//constructor of ChartTransportHandler, which initializes *this with the Route transporting_route_in (used to to the transport) and with proposed zoom factor proposed _zoom_factor at end fo the transport.  This is a `proposed` zoom factor because, if such proposed zoom factor is < 1 or > zoom_factor_max, the actual zoom factor will be set to 1 and zoom_factor_max, respectively. Othersize, the actual zoom_factor will be equal to proposed_zoom_factor.
template<class F> ChartTransportHandler<F>::ChartTransportHandler(ChartFrame* chart_in, const Route& transporting_route_in, const Double& proposed_zoom_factor, F* f_in) : MotionHandler<F>(chart_in->parent, transporting_route_in, f_in){
    
    
    chart_frame = chart_in;
    
    if(proposed_zoom_factor.value < 1.0){
        zoom_factor = 1.0;
    }else{
        if(zoom_factor > wxGetApp().zoom_factor_max){
            zoom_factor = wxGetApp().zoom_factor_max;
        }else{
            zoom_factor = proposed_zoom_factor;
        }
    }
    
    
    
    switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
            
        case 0:{
            //I am using Projection_types[0]
            
            PositionProjection temp;
            
            //compute projection_size_end from zoom_factor and rectangle_observer_0
            chart_frame->parent->rectangle_observer_0.SizeMercator(&temp);
            projection_size_end = temp / zoom_factor;

            
            break;
            
        } 
        
        case 1:{
            //I am using Projection_types[1]

            //compute omega_end from zoom_factor and circle_observer_0.omega
            omega_end.set((chart_frame->parent->circle_observer_0.omega.value) / (zoom_factor.value));

            break;
            
        }
            
            
    }
    
    

    
    
    (MotionHandler<F>::timer)->Bind(wxEVT_TIMER, &ChartTransportHandler::OnTimer, this);

}


//prompt the movement of the center of the chart from position a to position b
template<class F> void ChartTransportHandler<F>::operator()(void) {
//void ChartTransportHandler::MoveChart(const Position& a, const Position& b){
    
    if(!((MotionHandler<F>::parent)->idling)){
        
        //the animation transport starts here (only if the parent ChartFrame is not in idling mode)
        (MotionHandler<F>::timer)->Start(
                     /*animation_time is converted in milliseconds, because Start() takes its first argument in milliseconds*/
                     (wxGetApp().animation_time.get()) * 60.0 * 60.0 / ((double)((wxGetApp().n_animation_steps.value) - 1)) * 1000.0,
                     wxTIMER_CONTINUOUS);
        
    }
    
    
}

//this method iterates the animation
template<class F> void ChartTransportHandler<F>::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    
    
    if(((MotionHandler<F>::t) < (wxGetApp().n_animation_steps.value))) {
        //the time parameter is undedr its maximum value

        if((MotionHandler<F>::t) == 0) {
            //I am at the beginning of the transport and *parent is not in idling mode -> proceed with the transport
            
            //set parameters back to their original value and reset listcontrol_routes to the original list of Routes
            (*((MotionHandler<F>::parent)->set_idling))();
            (chart_frame->dragging_chart) = true;
            chart_frame->EnableAll(false);

            (MotionHandler<F>::transporting_route_temp) = (MotionHandler<F>::transporting_route);
            
            (MotionHandler<F>::start) = (MotionHandler<F>::transporting_route).reference_position;
            
  
            //during the transport, I disconnect DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement from mouse movements
            chart_frame->draw_panel->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, chart_frame->draw_panel);
            chart_frame->parent->listcontrol_sights->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            chart_frame->parent->listcontrol_positions->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            chart_frame->parent->listcontrol_routes->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            chart_frame->parent->panel->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            
            //I don't want anything to be highlighted during the chart transport, so I set 
            (chart_frame->parent->highlighted_route_now) = -1;
            (chart_frame->parent->highlighted_sight_now) = -1;
            (chart_frame->parent->highlighted_position_now) = -1;

            
            (chart_frame->draw_panel->label_position_now) = String("");

       
            switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    PositionProjection q_A, q_B;
//                    Position p_A, p_B;
                    
    
                    //write in p_NW and p_SE the two corner points of the projection and write in projection_size the size (in x,y) of the relative rectangle
                    q_A.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->phi_max));
                    q_B.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->phi_min));
                    projection_size.x = chart_frame->draw_panel->x_span();
                    projection_size.y = (q_A.y) - (q_B.y);
                    projection_size_start = projection_size;
                    
                    
                    q_A.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->parent->rectangle_observer_0.p_NW.phi));
                    q_B.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->parent->rectangle_observer_0.p_SE.phi));
                    (projection_size_end.x) = chart_frame->draw_panel->x_span_0;
                    projection_size_end.y = (q_A.y) - (q_B.y);
                    projection_size_end /= (zoom_factor.value);

                    
                   
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
     
                    
                    //the Position where I start the animation (start) may not coincide with circle_observer.reference_position (for example, I may want to start the animaiton from the antipode of circle_observer.reference_position to show a nice turn of the earth during the animaiton): thus, to start the animation, I need to first set rotation to the rotation that brings circle_observer.reference_position to be centered on start -> to do this, I do 
                    chart_frame->draw_panel->rotation.set(((chart_frame->draw_panel->rotation) * Rotation((MotionHandler<F>::start), chart_frame->draw_panel->circle_observer.reference_position)));

                    
                    chart_frame->draw_panel->rotation_start_drag.set((chart_frame->draw_panel->rotation));
                    (chart_frame->draw_panel->circle_observer.reference_position) = (MotionHandler<F>::start);
                    omega_start = chart_frame->draw_panel->circle_observer.omega;
                    
                    break;
                    
                }
                    
            }
            
//            cout << "******************* Before the transport *******************" << endl;
//            (MotionHandler<F>::transporting_route).compute_end(String(""));
//            (MotionHandler<F>::transporting_route).reference_position.print(String("Start position of transporting route"), String("\t"), cout);
//            (MotionHandler<F>::transporting_route).end.print(String("Expected arrival position"), String("\t"), cout);
//            chart_frame->draw_panel->circle_observer.reference_position.print(String("Circle observer reference position"), String("\t"), cout);


         
            
            (MotionHandler<F>::t)++;

        }
        
        if((MotionHandler<F>::t) > 0){
            //the transport animation is in progress -> do the next chunk

            (MotionHandler<F>::transporting_route_temp).length.set(
                String(""),
                ((MotionHandler<F>::transporting_route).length.value) *
                                               (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value)))))
                                               ,
                                               String(""));
            
            
            switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    PositionProjection q_center;
                    
                    //transport the starting point of the animation, start, according to transporting_route_temp, and store the result in p_center -> this yields the updated center of the chart
                    (MotionHandler<F>::start).transport(&p_center, (MotionHandler<F>::transporting_route_temp), String(""));
                    //transform p_center into a PositionProjection
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->GeoToProjection))(p_center, &q_center, true);
                    
                    
                    //update projection_size
                    projection_size = projection_size_start + (projection_size_end - projection_size_start) * (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))));

                    
                    //shift q_center to the NE and to the SW by projection_size/2 -> these will be the updated values of p_NE and p_SE
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(q_center + projection_size/2.0, &p_NE);
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(q_center - projection_size/2.0, &p_SW);

                    //set lambda / phi min/max according to p_NE and p_SW
                    (chart_frame->lambda_max) = (p_NE.lambda);
                    (chart_frame->phi_max) = (p_NE.phi);

                    (chart_frame->lambda_min) = p_SW.lambda;
                    (chart_frame->phi_min) = p_SW.phi;
          
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    (MotionHandler<F>::transporting_route_temp).compute_end(String(""));
                    
                    //conpute the new rotation: the new rotation of the earth is the old one, composed with the rotation which brings the old reference_position onto the new one
                    chart_frame->draw_panel->rotation.set(((chart_frame->draw_panel->rotation_start_drag) * Rotation((MotionHandler<F>::transporting_route_temp).end, (MotionHandler<F>::start))));
                    
                    (chart_frame->draw_panel->circle_observer.reference_position) = ((MotionHandler<F>::transporting_route_temp).end);

                    chart_frame->draw_panel->circle_observer.omega = omega_start.value + (omega_end.value - omega_start.value) * (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))));
                    
                    
                    break;
                    
                }
                    
            }
            
//            cout << "********* t = " << t << " *************" << endl;
//            (MotionHandler<F>::transporting_route_temp).compute_end(String(""));
//            (MotionHandler<F>::transporting_route_temp).end.print(String("Expected arrival point with (MotionHandler<F>::transporting_route_temp)"), String("\t\t"), cout);
//            chart_frame->draw_panel->circle_observer.reference_position.print(String("Reference position"), String("\t\t"), cout);
            
            
#ifdef WIN32
            //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
            (chart_frame->polygon_position_before) = (chart_frame->polygon_position_now);
//            chart_frame->coastline_polygons_before.resize(chart_frame->coastline_polygons_now.size());
            copy_n(chart_frame->coastline_polygons_now.begin(), chart_frame->coastline_polygons_now.size(), chart_frame->coastline_polygons_before.begin() );

            (chart_frame->draw_panel->position_plot_area_before) = (chart_frame->draw_panel->position_plot_area_now);
            chart_frame->draw_panel->grid_before.clear();
            (chart_frame->draw_panel->grid_before) = (chart_frame->draw_panel->grid_now);
            chart_frame->draw_panel->ticks_before.clear();
            (chart_frame->draw_panel->ticks_before) = (chart_frame->draw_panel->ticks_now);
            
            //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
            chart_frame->draw_panel->points_route_list_before.clear();
            (chart_frame->draw_panel->points_route_list_before) = (chart_frame->draw_panel->points_route_list_now);
            
            chart_frame->draw_panel->points_position_list_before.clear();
            (chart_frame->draw_panel->points_position_list_before) = (chart_frame->draw_panel->points_position_list_now);
            
            chart_frame->draw_panel->reference_positions_route_list_before.clear();
            (chart_frame->draw_panel->reference_positions_route_list_before) = (chart_frame->draw_panel->reference_positions_route_list_now);
            
   
#endif
            //re-draw the chart
            (chart_frame->draw_panel->*(chart_frame->draw_panel->PreRender))();
            chart_frame->draw_panel->MyRefresh();
            chart_frame->UpdateSlider();
//            chart_frame->draw_panel->PaintNow();
            //

            //            cout << "\t\t t= " << t << "\n";
            
            (MotionHandler<F>::t)++;

        }

    }else {
        //the transport  is over

        
        switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
                
            case 0: {
                //I am using Projection_types[0]
                
//                PositionProjection temp;
//
//                
//                start.transport(&p_NE, transporting_route, String(""));
//                (chart_frame->lambda_max) = (p_NE.lambda);
//                (chart_frame->phi_max) =( p_NE.phi);
//                
//                temp.SetMercator(p_NE);
//                (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(temp - projection_size, &p_SW);
//                
//                (chart_frame->lambda_min) = p_SW.lambda;
//                (chart_frame->phi_min) = p_SW.phi;

                
                break;
                
            }
                
            case 1: {
                //I am using Projection_types[1]
                
                //do the whole transport rather than combining many little transports, to avoid rounding errors
//                chart_frame->draw_panel->circle_observer.reference_position = start;
//                chart_frame->draw_panel->circle_observer.reference_position.transport_to(transporting_route, String(""));

//                chart_frame->draw_panel->rotation.set(((chart_frame->draw_panel->rotation_start_drag) * Rotation(transporting_route.end, start)));

                chart_frame->draw_panel->rotation_end_drag.set((chart_frame->draw_panel->rotation));

                break;
                
            }
                
        }
            
//        cout << "******************* After the transport *******************" << endl;
//        transporting_route.compute_end(String(""));
//        transporting_route.end.print(String("Expected arrival position"), String("\t"), cout);
//        chart_frame->draw_panel->circle_observer.reference_position.print(String("Circle observer reference position"), String("\t"), cout);
        

        (chart_frame->dragging_chart) = false;
        chart_frame->EnableAll(true);
        chart_frame->Fit();

        //bind back all the methoud that have been unbound at the beginnign of the transport
        chart_frame->draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, chart_frame->draw_panel);
        chart_frame->parent->listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
    
        (MotionHandler<F>::timer)->Stop();
        (*((MotionHandler<F>::parent)->unset_idling))();
        
        //call the functor to be called at the end of the animation, if any
        if((MotionHandler<F>::f) != NULL){
            (*(MotionHandler<F>::f))();
        }
        
    }

}
