#define my_precision 16
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
#define Y_min 2021
#define Y_max 2021
#define mjd_min 59215.0
#define N 24.0
//one nautical mile in kilometers
#define nm 1.852

//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal

class Atmosphere{

 public:
  double A, B, P_dry_0, alpha, beta, gamma, T0;
  vector<double> h; 
  void set(void);
  double T(double), n(double);
 
};

double Atmosphere::T(double z){

  double x = 0.0;
  unsigned int i;

  if(z < h[4]){

    i=0;
    do{   
      i++;
    }while(!(z>=h[i-1]) && (z<h[i]));
    i--;
  
    switch(i){

    case 0: x = T0+alpha*z;
    case 1: x = T0+alpha*h[1];
    case 2: x = T0+alpha*h[1] + beta*(z-h[2]);
    case 3: x = T0+alpha*h[1] + beta*(h[3]-h[2]) + gamma*(z-h[3]);
    
    }

  }else{

    cout << "Value of z is not valid!\n";
    x=0;

  }

  return x;
  
  
}

double Atmosphere::n(double z){

  unsigned int i;
  double x = 0.0;
  
  i=0;
  do{   
    i++;
  }while(!(z>=h[i-1]) && (z<h[i]));
  i--;
  
  switch(i){

  case 0: x = pow(1.0+alpha*z/T0, -B/alpha);
  case 1: x = pow((T0+alpha*h[1])/T0, -B/alpha) * exp(-B*(z-h[1])/(T0+alpha*h[1]));
  case 2: x = pow((T0+alpha*h[1])/T0, -B/alpha) * exp(-B*(h[2]-h[1])/(T0+alpha*h[1])) * pow(1.0+beta/(T0+alpha*h[1])*(z-h[2]), -B/beta);
  case 3: x = pow((T0+alpha*h[1])/T0, -B/alpha) * exp(-B*(h[2]-h[1])/(T0+alpha*h[1])) * pow((T0+alpha*h[1] + beta*(h[3]-h[2]))/(T0+alpha*h[1]), -B/beta)
      * pow(1.0+gamma/(T0+alpha*h[1] + beta*(h[3]-h[2]))*(z-h[3]), -B/gamma);
    
  }

  return (A*P_dry_0/T(z)*x/(1e6)+1.0);


}

void Atmosphere::set(void){



    cout << "Atmosphere model: US 1976.\n";

    A = 0.7933516713545163, B = 34.16*nm, P_dry_0 = 101325.0, alpha = -6.5*nm, beta = 2.8*nm, gamma = -2.8*nm, T0 = 288.15;

    h.resize(4+1);
    
    h[0] = 0.0;
    h[1] = 11.0/nm;
    h[2] = 20.0/nm;
    h[3] = 51.0/nm;
    h[4] = 71.0/nm;

  
}


class Distance{

 public:
  double value;
  void set(const char*, double);
  void enter(const char*);
  void print(const char*);

};

class Angle{

public:
 
  double value;
  void normalize(void);
  void enter(const char*);
  void set(const char*, double);
  void print(const char*);
  
};

class Limb{

 public:
  char value;
  void enter(const char*);
  void print(const char*);

};



class Body{

 public:
  vector<string>::iterator name, type;
  Distance radius;
  void enter(void), print(void);
  
};

void Body::print(void){

  cout << "Name: " << *name << "\n";
  cout << "Type: " << *type << "\n";

}

/*
void Body::enter(void){

  bool check;
  string s;



  do{
    cout << "Enter name of body:";
    cin >> s;
    name = find(body_name.begin(), body_name.end(), s);
    
    if(name!=body_name.end()){check = true;}
    else{
      cout << "Entered value is not valid! Try again.\n";
      flush(cout);
      check = false;
    }
  }while(!check);

  print();

  
}
*/



class Time{

public:  int Y, M, D, h, m;
  double s, mjd;
  bool check_Y(void), check_M(void), check_D(void), check_h(void), check_m(void), check_s(void);
  void enter(const char*);
  void print(const char*);
  void to_mjd(void);
  
};

class Sight{

public:
  Time t;
  Angle index_error, GHA, d;
  Distance r, height_of_eye;
  Body body;
  Limb limb;

  void get_coordinates(void);

};


void Distance::set(const char* name, double x){

  if(x>=0.0){
    value = x;
    print(name); 
  }
  else{
    cout << "Entered value of " << name << "is not valid!\n";
  }
  
}

void Distance::enter(const char* name){

    cout << "Enter " << name << " [m]:\n";

    do{
      cin >> value;    
    }while(value < 0.0);
    //convert to nautical miles
    value/=(1e3*nm);
    
    print(name); 
  
}

void Distance::print(const char* name){

    cout << name << " is " << value << " nm.\n";
 
}

void Sight::get_coordinates(void){

  ifstream infile;
  stringstream filename, line_ins;
  string line, dummy;
  unsigned int l, l_min, l_max;
  double mjd_int[(unsigned int)N], GHA_int[(unsigned int)N], d_int[(unsigned int)N], r_int[(unsigned int)N], sum;
  gsl_interp_accel* acc = gsl_interp_accel_alloc ();
  gsl_spline *interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)),
    *interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)),
    *interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));


  filename << "data/sun.txt";
  infile.open(filename.str().c_str());
  if(!infile){
    cout << "Error opening file " << filename.str().c_str() << endl;
    flush(cout);
  }

  /* cout << "\nMJD = " << t.mjd; */
  /* cout << "\nMJD0 = " << mjd_min; */
  /* cout << "\ndiff = " << (t.mjd)-mjd_min; */
  /* cin >>l ; */

  l_min = (unsigned int)(24.0*((t.mjd)-mjd_min))-(unsigned int)(N/2.0);
  l_max = (unsigned int)(24.0*((t.mjd)-mjd_min))+(unsigned int)(N/2.0);

  /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */
  
  for(l=0; l<l_min; l++){
    line.clear();
    getline(infile, line);
  }

  for(; l<l_max; l++){
    line.clear();
    line_ins.clear();
    getline(infile, line);
    line_ins << line;
    cout << line << "\n";
    line_ins >> dummy >> dummy >> dummy >> GHA_int[l-l_min] >> d_int[l-l_min] >> r_int[l-l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;
    mjd_int[l-l_min] = ((double)(l-l_min))/24.0;
  }

  infile.close();

  //convert to radians and nm
  for(l=0; l<N; l++){
    
    GHA_int[l]*=k; 
    d_int[l]*=k;
    r_int[l]/=nm;
  }

  //remove discontinuous jumps in GHA to allow for interpolation
  for(sum=0.0, l=0; l<N-1; l++){
    //cout << GHA_int[l] << " " << d_int[l] << " " << r_int[l] << "\n";
    if(((GHA_int[l]-sum) < 0.0) && (GHA_int[l+1] > 0.0)){
      sum -= 2.0*M_PI;
    }
    GHA_int[l+1] += sum;
  }

  gsl_spline_init(interpolation_GHA, mjd_int, GHA_int, (unsigned int)N);
  gsl_spline_init(interpolation_d, mjd_int, d_int, (unsigned int)N);
  gsl_spline_init(interpolation_r, mjd_int, r_int, (unsigned int)N);

  
  cout << "Read values:\n";
  for(l=0; l<N; l++){
    cout << mjd_int[l] << " " << GHA_int[l] << " " << d_int[l] << " " << r_int[l] << "\n";
  }

  //add minus sign because in JPL convention longitude is positive when it is W
  GHA.set("GHA", -gsl_spline_eval(interpolation_GHA, (t.mjd)-mjd_min-((double)l_min)/24.0, acc));
  d.set("d", gsl_spline_eval(interpolation_d, (t.mjd)-mjd_min-((double)l_min)/24.0, acc));
  r.set("r", gsl_spline_eval(interpolation_r, (t.mjd)-mjd_min-((double)l_min)/24.0, acc));
  
}

void Angle::set(const char* name, double x){

  value = x;
  normalize();
  print(name);
  
}

void Angle::enter(const char* name){

  string s;
  int ad;
  double am;
  bool check;
  
  cout << "Enter " << name << " [s adadad amam]:\n";

  do{
    cout << "\tEnter s: ";
    cin >> s;
    
    if((s=="+") || (s=="-")){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);

  
  do{
    cout << "\tEnter adadad: ";
    cin >> ad;
    
    if((abs(ad) >= 0) && (abs(ad) < 360)){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);
  
  do{
    cout << "\tEnter amam: ";
    cin >> am;

    if((am >= 0.0) && (am < 60.0)){check = true;}
    else{
      cout << "Entered value is not valid! Try again.\n";
      flush(cout);
      check = false;
    }
  }while(!check);

  value = k*(ad + am/60.0);
  if(s=="-"){value*=-1.0;}
  normalize();
  print(name);


}

void Angle::normalize(void){

  value = value - 2.0*M_PI*floor(value/(2.0*M_PI));

}


void Angle::print(const char* name){

  normalize();
  cout << name << " is " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'\n";

}

void Limb::enter(const char* name){

  bool check;
  cout << "Enter " << name << " [u/l]:\n";

  do{
    cin >> value;
    
    if((value=='u') || (value=='l')){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);
  
  print(name);
  
}


void Limb::print(const char* name){

  cout << name << " is " << value << "\n";
  
}


bool Time::check_Y(void){

  if((Y >= Y_min) && (Y <= Y_max)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};


bool Time::check_M(void){

  if((M >= 1) && (M <= 12)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_D(void){

  if((D >= 1) && (D <= 31)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_h(void){

  if((h >= 0) && (h < 24)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_m(void){

  if((m >= 0) && (m < 60)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Time::check_s(void){

  if((s >= 0.0) && (s < 60.0)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

void Time::print(const char* name){

  cout << name << " is " << Y << " ";
  if(M<10){cout << 0;}
  cout << M << " ";
  if(D<10){cout << 0;}
  cout << D << " ";
  if(h<10){cout << 0;}
  cout << h << "-";
  if(m<10){cout << 0;}
  cout << m << "-";
  if(s<10.0){cout << 0;}
  cout << s << " (" << mjd << " MJD)\n";
  flush(cout);

};




void Time::enter(const char* name) {

  cout << "Enter " << name << " [YYYY MM DD hh-mm-ss]\n";

  do{
    cout << "\tEnter YYYY: ";
    cin >> Y;
  }while(!check_Y());
  
  do{
    cout << "\tEnter MM: ";
    cin >> M;
  }while(!check_M());

  do{
    cout << "\tEnter DD: ";
    cin >> D;
  }while(!(check_D()));

  do{
    cout << "\tEnter hh: ";
    cin >> h;
  }while(!(check_h()));

  do{
    cout << "\tEnter mm: ";
    cin >> m;
  }while(!(check_m()));

  do{
    cout << "\tEnter ss: ";
    cin >> s;
  }while(!(check_s()));

  to_mjd();
  print(name);

}

void Time:: to_mjd(void)
  
  /*
    Modified Julian Date ( MJD = Julian Date - 2400000.5)
    valid for every date
    Julian Calendar up to 4-OCT-1582,
    Gregorian Calendar from 15-OCT-1582.
  */
{
  
  long int b, c;
  int Yt = Y, Mt = M, Dt = D;
   
  mjd = 10000.0 * Yt + 100.0 * Mt + Dt;
  if (Mt <= 2)
    {
      Mt = Mt + 12;
      Yt = Yt - 1;
    };
  if (mjd <= 15821004.1)
    {
      b = ((Yt+4716)/4) - 1181;
      if (Yt < -4716)
        {
	  c = Yt + 4717;
	  c = -c;
	  c = c / 4;
	  c = -c;
	  b = c - 1182;
        };
    }
  else b = (Yt/400) - (Yt/100) + (Yt/4);
  //     { b = -2 + floor((Yt+4716)/4) - 1179;}
  // else {b = floor(Yt/400) - floor(Yt/100) + floor(Yt/4);};
   
  mjd = 365.0 * Yt - 679004.0;
  //comment this out if you want to include hours, minutes and seconds in mjd
  mjd = mjd + b + int(30.6001 * (Mt + 1)) + Dt + (((double)h) + ((double)m)/60.0 + ((double)s)/(60.0*60.0)) / 24.0;
   
  
}


