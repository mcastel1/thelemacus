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





//same asLengthField<P>::InsertIn(T* host) but with flags to be provided
template<class P> template<class T> void LengthField<P>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

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


//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> LengthUnitField<P>::LengthUnitField(wxPanel* panel_of_parent, LengthUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >(panel_of_parent, object_in, LengthUnit_types, recent_items_in) {

}
 

//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> SpeedUnitField<P>::SpeedUnitField(wxPanel* panel_of_parent, SpeedUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, SpeedUnit, CheckLengthUnit<P> >(panel_of_parent, object_in, SpeedUnit_types, recent_items_in) {

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
