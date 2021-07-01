#define my_precision 16
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
#define Y_min 2021
#define Y_max 2021
#define mjd_min 59215.0
#define N 24.0
#define epsrel (1e-12)
#define max_iter (1e2)
//one nautical mile in kilometers
#define nm 1.852

//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal

class Catalog;

class Length{

 public:
  double value;
  void set(const char*, double);
  void enter(const char*);
  void print(const char*);

};


class Time{

 public:  int Y, M, D, h, m;
  double s, mjd;
  bool check_Y(void), check_M(void), check_D(void), check_h(void), check_m(void), check_s(void);
  void enter(const char*);
  void print(const char*);
  void to_mjd(void);
  
};


class Angle{

 public:
 
  double value;
  void normalize(void);
  void enter(const char*);
  void set(const char*, double);
  void print(const char*);

  Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
  
};

Angle Angle::operator+ (const Angle& angle){
  Angle temp;

  temp.value = value +angle.value;
  temp.normalize();

  return temp;
}

Angle Angle::operator- (const Angle& angle){
  Angle temp;

  temp.value = value -angle.value;
  temp.normalize();

  return temp;
}

Angle Angle::operator/ (const double& x){
  Angle temp;

  temp.value = value/x;
  temp.normalize();

  return temp;
}


class Limb{

 public:
  char value;
  void enter(const char*);
  void print(const char*);

};


class Body{

 public:
  string name, type;
  Length radius;
  void enter(Catalog), print(void);
  
};


class Catalog{

 public:
  vector<Body> list;
  void add(string, string, double);

};

void Catalog::add(string type, string name, double radius){

  Body body;
  
  body.type = type;
  body.name = name;
  body.radius.value = radius;
  
  list.push_back(body);
  
  cout << "Added body to catalog:\n";
  body.print();

}


class Atmosphere{

 public:
  Length earth_radius;
  unsigned int n_layers;
  double A, B, P_dry_0, alpha, beta, gamma, T0;
  vector<double> h, lambda, t;
  void set(void);
  double T(Length), n(Length), dTdz(Length), dndz(Length);

};

class Sight{

 public:
  Time t;
  Angle index_error, GHA, d, H_s, H_a, H_o, H_i, DH_refraction, DH_dip, DH_parallax_and_limb;
  Length r, height_of_eye;
  Atmosphere atmosphere;
  Body body;
  Limb limb;

  Sight();
  static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
  void get_coordinates(void);
  void compute_DH_dip(void);
  void compute_DH_refraction(void);
  void compute_DH_parallax_and_limb(void);

  void compute_H_a(void);
  void compute_H_o(void);
  
};


void Sight::compute_H_a(void){

  compute_DH_dip();
  H_a = H_s-index_error+DH_dip;
  H_a.print("apparent altitude");
  
}


void Sight::compute_H_o(void){

  compute_DH_refraction();
  compute_DH_parallax_and_limb();
  H_o = H_a + DH_refraction + DH_parallax_and_limb;
  H_o.print("observed altitude");
  
}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(void){

  H_i = H_a + DH_refraction;
  H_i.print("intermediate altitude");

  switch((limb.value)){
    
  case 'u':
    {
    int status = 0;
    int iter = 0;
    double x = 0.0, x_lo = 0.0, x_hi = 2.0*M_PI;
    gsl_function F;
    const gsl_root_fsolver_type *T;
    gsl_root_fsolver *s;

    F.function = &rhs_DH_parallax_and_limb;
    F.params = &(*this);

    T = gsl_root_fsolver_brent;
    s = gsl_root_fsolver_alloc (T);
    gsl_root_fsolver_set(s, &F, x_lo, x_hi);
 
    printf ("using %s method\n", gsl_root_fsolver_name (s));
    printf ("%5s [%9s, %9s] %9s %10s %9s\n", "iter", "lower", "upper", "root", "err", "err(est)");

    iter = 0;
    do{
      
      iter++;
      status = gsl_root_fsolver_iterate (s);
      
      x = gsl_root_fsolver_root(s);
      x_lo = gsl_root_fsolver_x_lower(s);
      x_hi = gsl_root_fsolver_x_upper(s);
      status = gsl_root_test_interval (x_lo, x_hi, 0.0, epsrel);
      if(status == GSL_SUCCESS){
	printf ("Converged:\n");
      }
      printf ("%5d [%.7f, %.7f] %.7f %+.7f\n", iter, x_lo, x_hi, x, x_hi - x_lo);
    }
    while((status == GSL_CONTINUE) && (iter < max_iter));


    //H_o.value = (x_lo+x_hi)/2.0;
    DH_parallax_and_limb.value = (x_lo+x_hi)/2.0 - (H_i.value);
    gsl_root_fsolver_free (s);

    break;
    }
  case 'l':
    {
      //    H_o.value = (H_i.value) + asin(((atmosphere.earth_radius.value)*cos(H_i.value)+(body.radius.value))/(r.value));
      DH_parallax_and_limb.value = asin(((atmosphere.earth_radius.value)*cos(H_i.value)+(body.radius.value))/(r.value));
    break;
    }
  case 'c':
    {
      //H_o.value = (H_i.value) + asin((atmosphere.earth_radius.value)*cos(H_i.value)/(r.value));
      DH_parallax_and_limb.value = asin((atmosphere.earth_radius.value)*cos(H_i.value)/(r.value));
    break;
    }
  }

  DH_parallax_and_limb.print("parallax and limb correction");
   
}

double Atmosphere::T(Length z){

  double x = 0.0;
  //cout << "z = " << (z.value) << "\n";

  if((z.value) <= h[n_layers]){

    unsigned int i;
    bool check = true;
    
    for(i=0, check=true; (i<n_layers) && check; i++){
      if(((z.value)>=h[i]) && ((z.value)<h[i+1])){
	x=t[i] +lambda[i]*((z.value)-h[i]);
	check=false;
      }
    }
    //cout << "i = " << i << "\n";
  
  }else{

    cout << "Value of z is not valid!\n";
    x=-1.0;

  }

  return x;
  
  
}

double Atmosphere::dTdz(Length z){

  double x = 0.0;
  //cout << "z = " << (z.value) << "\n";

  if((z.value) <= h[n_layers]){

    unsigned int i;
    bool check = true;
    
    for(i=0, check=true; (i<n_layers) && check; i++){
      if(((z.value)>=h[i]) && ((z.value)<h[i+1])){
	x=lambda[i];
	check=false;
      }
    }
  
  }else{

    cout << "Value of z is not valid!\n";
    x=-1.0;

  }

  return x;
  
  
}


double Atmosphere::n(Length z){

  double x = 0.0;  

  if((z.value) <= h[n_layers]){

    unsigned int i;
    bool check = true;
    
    for(i=0, x=0.0, check=true; (i<n_layers) && check; i++){
      if(((z.value)>=h[i]) && ((z.value)<h[i+1])){
	if(lambda[i] != 0.0){
	  x-=B/lambda[i]*log((t[i]+lambda[i]*((z.value)-h[i]))/t[i]);
	}else{
	  x-=B*((z.value)-h[i])/t[i];
	}
	check=false;
      }else{
	if(lambda[i] != 0.0){
	  x-=B/lambda[i]*log((t[i]+lambda[i]*(h[i+1]-h[i]))/t[i]);
	}else{
	  x-=B*(h[i+1]-h[i])/t[i];
	}
      }
    }

    /*
      int_0^z dz/(t_n+lambda_n*(z-h_n)) = log()
    */
  
  }else{

    cout << "Value of z is not valid!\n";
    x=-1.0;

  }

  
  /* for(i=0, check=true; (i<4) && check; i++){ */
  /*   if((z>=h[i]) && (z<h[i+1])){check=false;} */
  /* } */
  /* i--; */
  
  /* switch(i){ */

  /* case 0: x = pow(1.0+alpha*z/T0, -B/alpha); */
  /*   break; */
    
  /* case 1: x = pow((T0+alpha*h[1])/T0, -B/alpha) * exp(-B*(z-h[1])/(T0+alpha*h[1])); */
  /*   break; */
    
  /* case 2: x = pow((T0+alpha*h[1])/T0, -B/alpha) * exp(-B*(h[2]-h[1])/(T0+alpha*h[1])) * pow(1.0+beta/(T0+alpha*h[1])*(z-h[2]), -B/beta); */
  /*   break; */
    
  /* case 3: x = pow((T0+alpha*h[1])/T0, -B/alpha) * exp(-B*(h[2]-h[1])/(T0+alpha*h[1])) * pow((T0+alpha*h[1] + beta*(h[3]-h[2]))/(T0+alpha*h[1]), -B/beta) */
  /*     * pow(1.0+gamma/(T0+alpha*h[1] + beta*(h[3]-h[2]))*(z-h[3]), -B/gamma); */
  /*   break; */
    
  /* } */

  return (A*P_dry_0/T(z)*exp(x)/(1e6)+1.0);


}

double Atmosphere::dndz(Length z){


  return (-1.0/T(z)*dTdz(z)*(n(z)-1.0) - (n(z)-1.0)*B/T(z));


}


double Sight::dH_refraction(double z, void* sight){

  Sight* a = (Sight*)sight;
  Length z_Length, zero_Length;
  z_Length.value = z;
  zero_Length.value = 0.0;
  
  return( -(((*a).atmosphere).earth_radius.value)*(((*a).atmosphere).n(zero_Length))*cos(((*a).H_a).value)*(((*a).atmosphere).dndz)(z_Length)/(((*a).atmosphere).n)(z_Length)/sqrt(gsl_pow_2(((((*a).atmosphere).earth_radius.value)+z)*(((*a).atmosphere).n)(z_Length))-gsl_pow_2((((*a).atmosphere).earth_radius.value)*(((*a).atmosphere).n)(zero_Length)*cos(((*a).H_a).value))));

  
}


double Sight::rhs_DH_parallax_and_limb(double h, void* sight){

  Sight* a = (Sight*)sight;

  //  (((*a).atmosphere).earth_radius.value)      R
  //(((*a).body).radius.value)                   r
  //(((*a).r).value)          d
  
  return( -(((*a).H_i).value) + h + asin( (((*a).body).radius.value)/sqrt(gsl_pow_2((((*a).r).value) )+gsl_pow_2((((*a).atmosphere).earth_radius.value))-2.0*(((*a).r).value) *(((*a).atmosphere).earth_radius.value)*sin(h)) ) - atan( ((((*a).atmosphere).earth_radius.value)*cos(h))/((((*a).r).value) -(((*a).atmosphere).earth_radius.value)*sin(h)) ) );
  
}

void Atmosphere::set(void){

  unsigned int i;
  double x;
  bool check;
  
  cout << "Atmosphere model: US 1976.\n";

  n_layers = 7;
  A = 0.7933516713545163, B = 34.16*nm, P_dry_0 = 101325.0, alpha = -6.5*nm, beta = 2.8*nm, gamma = -2.8*nm, T0 = 288.15;
  earth_radius.value = 6371.0/nm;

  h.resize(n_layers+1);
  lambda.resize(n_layers);
  t.resize(n_layers);
    
  h[0] = 0.0;
  h[1] = 11.0/nm;
  h[2] = 20.0/nm;
  h[3] = 32.0/nm;
  h[4] = 47.0/nm;
  h[5] = 51.0/nm;
  h[6] = 71.0/nm;
  h[7] = 84.8520/nm;

  lambda[0] = -6.5*nm;
  lambda[1] = 0.0*nm;
  lambda[2] = 1.0*nm;
  lambda[3] = 2.8*nm;
  lambda[4] = 0.0*nm;
  lambda[5] = -2.8*nm;
  lambda[6] = -2.0*nm;


  for(i=0, x=T0, check=true; (i<n_layers) && check; i++){
    t[i] = x;
    x+=lambda[i]*(h[i+1]-h[i]);
  }
  

  /* for(int i=0; i<4+1; i++){ */
  /*   cout << "\t\t" << i << " " << h[i] << "\n"; */
  /* } */

  
}



void Body::print(void){

  cout << "Name: " << name << "\n";
  cout << "Type: " << type << "\n";
  radius.print("radius");
  
}


void Body::enter(Catalog catalog){

  unsigned int i;
  bool check;
  string s;
  
  do{
    cout << "Enter name of body:";
    cin >> s;

    for(i=0, check=true; (i<(catalog).list.size()) && check; i++){if((((catalog).list)[i]).name == s){check=false;}}
    if(check){cout << "Body not found in catalog!\n";}
      
  }while(check);

  cout << "Body found in catalog.\n";
  
  i--;
  (*this) = (catalog.list)[i];
  print();
  
}






Sight::Sight(void){

  atmosphere.set();
  
}

//this function simplifies the atmosphere between z=0 and z=eight of eye as a single layer, where within this layer the index of refracion is independent of z. Refine this in the future. 
void Sight::compute_DH_dip(void){

  Length zero_Length;
  zero_Length.value = 0.0;

  DH_dip.set("Dip correction",
	     -acos( atmosphere.n(zero_Length)/atmosphere.n(height_of_eye)*((atmosphere.earth_radius.value)/((atmosphere.earth_radius.value)+(height_of_eye.value)) ) ));

}


void Sight::compute_DH_refraction(void){

  gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);
  gsl_function F;
  double result, error;

  F.function = &dH_refraction;
  F.params = &(*this);

  
  /* cout << "Value = " << dH_refraction(1.0, &(*this)); */
  /* cin >> result; */

  

  gsl_integration_qags (&F, (atmosphere.h)[(atmosphere.h).size()-1], (atmosphere.h)[0], 0.0, epsrel, 1000, w, &result, &error);
  DH_refraction.set("Refraction correction", result);
  
  gsl_integration_workspace_free(w);

}

void Length::set(const char* name, double x){

  if(x>=0.0){
    value = x;
    print(name); 
  }
  else{
    cout << "Entered value of " << name << "is not valid!\n";
  }
  
}

void Length::enter(const char* name){

  cout << "Enter " << name << " [m]:\n";

  do{
    cin >> value;    
  }while(value < 0.0);
  //convert to nautical miles
  value/=(1e3*nm);
    
  print(name); 
  
}

void Length::print(const char* name){

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
  cout << "Enter " << name << " [u/l/c]:\n";

  do{
    cin >> value;
    
    if((value=='u') || (value=='l') || (value=='c')){check = true;}
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


