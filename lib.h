#define my_precision 16
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
#define Y_min 2021
#define Y_max 2021
#define mjd_min 59215.0
#define N 24.0
//one nautical mile in kilometers
#define nm 1.852


class Body{

 public:
  vector<string>::iterator name, type;
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
  void enter(void);
  void print(void);
  void to_mjd(void);
  
};

class Distance{

 public:
  double value;
  void set(double);
  void print(void);

};

class Angle{

public:
 
  double value;
  void normalize(void);
  void enter(void);
  void set(double);
  void print(void);
  
};




class Sight{

public:
  Time t;
  Angle index_error, GHA, d;
  Distance r;
  Body body;

  void get_coordinates(void);

};


void Distance::set(double x){

  if(x>=0.0){
    value = x;
    print(); 
  }
  else{
    cout << "Entered value is not valid!\n";
  }
  
}

void Distance::print(void){

    cout << "Value is " << value << " nm.\n";
 
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
  GHA.set(-gsl_spline_eval(interpolation_GHA, (t.mjd)-mjd_min-((double)l_min)/24.0, acc));
  d.set(gsl_spline_eval(interpolation_d, (t.mjd)-mjd_min-((double)l_min)/24.0, acc));
  r.set(gsl_spline_eval(interpolation_r, (t.mjd)-mjd_min-((double)l_min)/24.0, acc));
  
}

void Angle::set(double x){

  value = x;
  normalize();
  print();
  
}

void Angle::enter(void){

  string s;
  int ad;
  double am;
  bool check;
  
  cout << "Enter angle s adadad amam:\n";

  do{
    cout << "Enter s: ";
    cin >> s;
    
    if((s=="+") || (s=="-")){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);

  
  do{
    cout << "Enter adadad: ";
    cin >> ad;
    
    if((abs(ad) >= 0) && (abs(ad) < 360)){check = true;}
     else{
       cout << "Entered value is not valid! Try again.\n";
       flush(cout);
       check = false;
     }
  }while(!check);
  
  do{
    cout << "Enter amam: ";
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


}

void Angle::normalize(void){

  value = value - 2.0*M_PI*floor(value/(2.0*M_PI));

}


void Angle::print(void){

  normalize();
  cout << "Value is " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'\n";

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

void Time::print(void){

  cout << "Time is " << Y << " ";
  if(M<10){cout << 0;}
  cout << M << " ";
  if(D<10){cout << 0;}
  cout << D << " ";
  if(h<10){cout << 0;}
  cout << h << "-";
  if(m<10){cout << 0;}
  cout << m << "-";
  if(s<10.0){cout << 0;}
  cout << s << " UTC, " << mjd << " MJD\n";
  flush(cout);

};




void Time::enter(void) {

  cout << "\nEnter UTC time YYYY MM DD hh-mm-s:\n";

  do{
    cout << "Enter YYYY: ";
    cin >> Y;
  }while(!check_Y());
  
  do{
    cout << "Enter MM: ";
    cin >> M;
  }while(!check_M());

  do{
    cout << "Enter DD: ";
    cin >> D;
  }while(!(check_D()));

  do{
    cout << "Enter hh: ";
    cin >> h;
  }while(!(check_h()));

  do{
    cout << "Enter mm: ";
    cin >> m;
  }while(!(check_m()));

  do{
    cout << "Enter ss: ";
    cin >> s;
  }while(!(check_s()));

  to_mjd();

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


