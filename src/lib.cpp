//REMOVE THIS WHEN ALL CLASSES WILL BE MOVED TO FILES
//#include "body_field.cpp"
//REMOVE THIS WHEN ALL CLASSES WILL BE MOVED TO FILES


//example of variadic function: it takes a non-variadic argument n and then an arbitrary number of arguments `...`

using namespace std;



















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




//this function sets (*this) to the current UTC date and time
void Time::set_current(void) {

    date.set_current();
    chrono.set_current();

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
