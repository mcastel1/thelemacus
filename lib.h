#define my_precision 16
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
//check that these values are covered by ephemerides data
#define Y_min 0
#define Y_max 2021
#define mjd_min 59215.0
#define N 24.0
#define epsrel (1e-12)
#define max_iter (1e2)
//one nautical mile in kilometers
#define nm 1.852


//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal

class Catalog;

class Answer{

 public:
  char value;
  void enter(const char*);
  void print(const char*);

};


class Chrono{

 public:
  unsigned int h, m;
  double s;

  bool check_h(void), check_m(void), check_s(void);
  void print(const char*);
  void enter(const char*);
  stringstream to_string(void);

};

class Time{

 public:
  int Y, M, D;
  bool Y_is_leap_year;
  vector<unsigned int> days_per_month;
  Chrono chrono;
  double s, mjd;
  bool check_Y(void), check_M(void), check_D(void);
  void enter(const char*);
  void print(const char*);
  
  void to_mjd(void);
  void to_utc(void);
  void check_leap_year(void);
  void add(Chrono);
  
  stringstream to_string(void);
  
};

void Time::check_leap_year(void){

  if((Y % 4)!=0){
    
    Y_is_leap_year = false;
    
  }else{
    
    if((Y % 100)!=0){

      Y_is_leap_year = true;

    }else{
      if((Y % 400)!=0){
	
	Y_is_leap_year = false;

      }else{
	
	Y_is_leap_year = true;
	
      }
      
    }
    
  }
  
}

void Time::add(Chrono chrono_in){

  mjd += (((double)(chrono_in.h)) + ((double)(chrono_in.m))/60.0 + ((double)(chrono_in.s))/(60.0*60.0))/24.0;
  to_utc();

}

class File{

 public:
  ifstream value;
  const char *name;
  void set_name(const char*);
  int open(void);
  void close(void);
  void remove(void);
  
};

void File::remove(void){

  stringstream command;

  command << "rm -rf " << name  << "> /dev/null 2>&1";
  system(command.str().c_str());

  
}

void File::set_name(const char* filename){

  name = filename;
  
}

int File::open(void){

  value.open(name);
  
  if(!value){
    
    cout << "Error opening file " << name << "!\n";
    return 0;
    
  }else{
    
    cout << "File " << name << " opened.\n";
    return 1;
     
  }

}

void File::close(void){
  
  value.close();
  cout << "File " << name << " closed.\n";
     

}

class Length{

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
  Angle RA, d; 
  void enter(Catalog), print(void);
  
};


class Catalog{

 public:
  vector<Body> list;
  Catalog(const char*);
  void add(string, string, double);
  void print(void);

};

Catalog::Catalog(const char* filename){

  File file;
  string line;
  stringstream line_ins;
  Body temp;


  file.set_name(filename);
  if(file.open()==1){

    getline((file.value), line);

    line.clear();
    line_ins.clear();
    getline((file.value), line);

    while(!(file.value).eof()){

      line_ins << line;
      line_ins >>  temp.type >> temp.name >> temp.radius.value >> temp.RA.value >> temp.d.value;

      temp.RA.value *= k;
      temp.d.value *= k;
      
      temp.RA.normalize();
      temp.d.normalize();
      
      list.push_back(temp);
      /* cout << line << "-----" << temp.RA.value << "\t" << temp.d.value << "\n"; */

      line.clear();
      line_ins.clear();
      getline((file.value), line);

    }
  
  
    file.close();

  }

}

void Catalog::print(void){

  unsigned int i;
  for(i=0; i<list.size(); i++){
    
    cout << "Body #" << i << "\n";
    list[i].print();

  }


}

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


void Answer::enter(const char* name){

  bool check;
  
  cout << "Enter " << name << " [y/n]:\n";

  do{

    cin >> value;
    
    if((value=='y') || (value=='n')){
      check = true;
    }
    else{
      cout << "Entered value is not valid! Try again.\n";
      flush(cout);
      check = false;
    }
  }while(!check);

  print(name);

}

void Answer::print(const char* name){

  cout << name << " is " << value << "\n";
  
}



class Sight{

 public:
  Time time;
  Angle index_error, GHA, d, H_s, H_a, H_o, H_i, DH_refraction, DH_dip, DH_parallax_and_limb;
  Length r, height_of_eye;
  Atmosphere atmosphere;
  Body body;
  Limb limb;
  Answer artificial_horizon;

  Sight();
  static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
  void get_coordinates(void);
  void compute_DH_dip(void);
  void compute_DH_refraction(void);
  void compute_DH_parallax_and_limb(void);

  void compute_H_a(void);
  void compute_H_o(void);

  void enter(Catalog);
  void print(void);
  void reduce(void);
  
};

void Sight::print(void){


  body.print();
  if(body.type != "star"){
    limb.print("limb");
  }
  H_s.print("sextant altitude");
  index_error.print("index error");
  artificial_horizon.print("artificial horizon");
  if(artificial_horizon.value == 'n'){
    height_of_eye.print("height of eye");
  }
  time.print("UTC time of sight");

}

class Plot{
  
 public:
  File file_id, file_gnuplot;
  unsigned int job_id;
  stringstream command;
  vector<Sight> sight_list;

  Plot();
  ~Plot();
  void add(Catalog catalog);
  void remove(unsigned int);
  void print(void);
  void show(void);

};

Plot::Plot(){

  command.precision(my_precision);

  file_id.set_name("job_id.txt");
  file_gnuplot.set_name("plot.plt");
  
}

Plot::~Plot(){

  file_gnuplot.remove();
  file_id.remove();
  
}

void Plot::print(void){

  for(unsigned int i=0; i<sight_list.size(); i++){
    cout << "Sight #" << i << ":\n";
    (sight_list[i]).print();
  }

}

void Plot::add(Catalog catalog){

  Sight sight;
  
  sight.enter(catalog);
  sight.reduce();
  sight.print();
  
  sight_list.push_back(sight);
  cout << "Sight added as sight #" << sight_list.size()-1 << ".\n";

  //sight.~Sight();

}

void Plot::remove(unsigned int i){

  sight_list.erase(sight_list.begin()+i);
  cout << "Sight #" << i << " removed.\n";

}
  

void Plot::show(void){

  stringstream line_ins, plot_command;
  string line;
  Answer answer;
  unsigned int i;

  file_gnuplot.remove();
  file_id.remove();

  for(i=0, plot_command.str(""); i<sight_list.size(); i++){
    plot_command << "replot [0.:2.*pi] xe(K*Lambda(t, " << (sight_list[i]).d.value << ", " << (sight_list[i]).GHA.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).d.value << ", " << (sight_list[i]).GHA.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp ti \"" << (sight_list[i]).body.name << " " << (sight_list[i]).time.to_string().str().c_str() << "\"\\\n";
  }

  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "sed 's/dummy_line/" << plot_command.str().c_str() << "/g' plot_dummy.plt >> " << file_gnuplot.name << "\n";
  //delete job_id.txt file, run gnuplot and write the relative job ID to job_id.txt
  command << "gnuplot '" << file_gnuplot.name << "' & \n echo $! >> " << file_id.name;

  
  system(command.str().c_str());

  //read the job id from file_id
  if(file_id.open()==1){
    getline(file_id.value, line);
    line_ins << line;
    line_ins >> job_id;
  }

  file_id.close();

  cout << "\nJob id = "<< job_id << "\nDo you want to quit the plot? [y/n]";
  cin >> answer.value;
  if((answer.value)=='y'){

    command.str("");
    command << "kill -9 " << job_id;
    system(command.str().c_str());

  }

}

void Sight::enter(Catalog catalog){

  body.enter(catalog);
  if(body.type != "star"){
    limb.enter("limb");
  }
  H_s.enter("sextant altitude");
  index_error.enter("index error");
  artificial_horizon.enter("artificial horizon");
  if(artificial_horizon.value == 'n'){
    height_of_eye.enter("height of eye");
  }


  Answer stopwatch; // stopwatch = 'n' -> time is in format UTC time. stopwatch  = 'y' -> master clock UTC time + stopwatch reading

  stopwatch.enter("use of stopwatch reading");
  if(stopwatch.value == 'n'){
    
    time.enter("UTC time of sight");
    
  }else{
    
    Chrono temp;
    
    time.enter("master-clock UTC time");
    temp.enter("stopwatch reading");
    time.add(temp);
    time.print("UTC time of sight");
    
  }
  
}

void Sight::reduce(void){

  compute_H_a();
  get_coordinates();
  compute_H_o();
  
}


void Sight::compute_H_a(void){

  if(artificial_horizon.value == 'y'){
    H_a = (H_s-index_error)/2.0;
    H_a.print("apparent altitude");

  }else{
    compute_DH_dip();
    H_a = H_s-index_error+DH_dip;
    H_a.print("apparent altitude");
  }
  
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

  if(body.type != "star"){

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

  }else{

    DH_parallax_and_limb.value = 0.0;

  }
  
   
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

  cout << "Type: " << type << "\nName: " << name << "\n";
  if((radius.value) != 0.0){
    radius.print("Radius");
  }
  if(type == "star"){
    RA.print("Right ascension");
    d.print("Declination");
  }
 
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
  DH_refraction.set("refraction correction", result);
  
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

  File file;
  stringstream filename, line_ins;
  string line, dummy, temp;
  unsigned int l, l_min, l_max;
  double mjd_tab[(unsigned int)N], GHA_tab[(unsigned int)N], d_tab[(unsigned int)N], sum;
  gsl_interp_accel* acc = gsl_interp_accel_alloc ();
  gsl_spline *interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)),
    *interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));


  if((body.type) != "star"){
    filename << "data/" << body.name << ".txt";
  }else{
    filename << "data/j2000_to_itrf93.txt";
  }  
  temp = filename.str();

  
  file.set_name(temp.c_str()); 
  if(file.open()==1){

    /* cout << "\nMJD = " << t.mjd; */
    /* cout << "\nMJD0 = " << mjd_min; */
    /* cout << "\ndiff = " << (t.mjd)-mjd_min; */
    /* cin >>l ; */

    l_min = (unsigned int)(24.0*((time.mjd)-mjd_min))-(unsigned int)(N/2.0);
    l_max = (unsigned int)(24.0*((time.mjd)-mjd_min))+(unsigned int)(N/2.0);

    /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */
  
    for(l=0; l<l_min; l++){
      line.clear();
      getline((file.value), line);
    }

    if((body.type) != "star"){

      //if the body is not a star

      double r_tab[(unsigned int)N];
      gsl_spline *interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));

      
      for(; l<l_max; l++){
	line.clear();
	line_ins.clear();
	getline((file.value), line);
	line_ins << line;
	cout << line << "\n";
	line_ins >> dummy >> dummy >> dummy >> GHA_tab[l-l_min] >> d_tab[l-l_min] >> r_tab[l-l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;
	mjd_tab[l-l_min] = ((double)(l-l_min))/24.0;
      }

      file.close();

      //convert to radians and nm
      for(l=0; l<N; l++){
	//add minus sign because in JPL convention longitude is positive when it is E
	GHA_tab[l]*=(-1.0)*k; 
	d_tab[l]*=k;
	r_tab[l]/=nm;
      }

      //remove discontinuous jumps in GHA to allow for interpolation
      for(sum=0.0, l=0; l<N-1; l++){
	//cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
	if(((GHA_tab[l]-sum) > 0.0) && (GHA_tab[l+1] < 0.0)){
	  sum += 2.0*M_PI;
	}
	GHA_tab[l+1] += sum;
      }

      gsl_spline_init(interpolation_GHA, mjd_tab, GHA_tab, (unsigned int)N);
      gsl_spline_init(interpolation_d, mjd_tab, d_tab, (unsigned int)N);
      gsl_spline_init(interpolation_r, mjd_tab, r_tab, (unsigned int)N);

  
      cout << "Read values:\n";
      for(l=0; l<N; l++){
	cout << mjd_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
      }

      GHA.set("GHA", gsl_spline_eval(interpolation_GHA, (time.mjd)-mjd_min-((double)l_min)/24.0, acc));
      d.set("d", gsl_spline_eval(interpolation_d, (time.mjd)-mjd_min-((double)l_min)/24.0, acc));
      r.set("r", gsl_spline_eval(interpolation_r, (time.mjd)-mjd_min-((double)l_min)/24.0, acc));

      gsl_spline_free(interpolation_r);

    }else{

      //if the body is a star
      double phi3, phi2, phi1;

      for(; l<l_max; l++){
	
	line.clear();
	line_ins.clear();
	getline((file.value), line);
	line_ins << line;
	cout << line << "\n";
	line_ins >> dummy >> dummy >> dummy >> phi3 >> phi2 >> phi1;

	phi1*=k;
	phi2*=k;
	phi3*=k;
	
	d_tab[l-l_min] = asin(cos(phi2)*sin((body.d.value)) - cos((body.d.value))*cos(phi1)*sin((body.RA.value))*sin(phi2) + cos((body.RA.value))*cos((body.d.value))*sin(phi1)*sin(phi2));
	
	GHA_tab[l-l_min] = atan((-cos(phi3)* sin((body.d.value))* sin(phi2) -  cos((body.RA.value)) * cos((body.d.value)) * (-cos(phi2) *cos(phi3) * sin(phi1) - cos(phi1) * sin(phi3)) - cos((body.d.value)) * sin((body.RA.value)) * (cos(phi1) * cos(phi2) * cos(phi3) - sin(phi1) * sin(phi3)))/( sin((body.d.value)) * sin(phi2) * sin(phi3) + cos((body.d.value)) * sin((body.RA.value)) * (cos(phi3) * sin(phi1) +  cos(phi1) * cos(phi2) * sin(phi3)) + cos((body.RA.value)) * cos((body.d.value)) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))));
	if(( sin((body.d.value)) * sin(phi2) * sin(phi3) + cos((body.d.value)) * sin((body.RA.value)) * (cos(phi3) * sin(phi1) +  cos(phi1) * cos(phi2) * sin(phi3)) + cos((body.RA.value)) * cos((body.d.value)) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))) < 0.0){
	  GHA_tab[l-l_min] += M_PI;
	}
	GHA_tab[l-l_min] = GHA_tab[l-l_min] - 2.0*M_PI*floor(GHA_tab[l-l_min]/(2.0*M_PI));

	  
	mjd_tab[l-l_min] = ((double)(l-l_min))/24.0;
      }


      file.close();

 

      //remove discontinuous jumps in GHA to allow for interpolation
      for(sum=0.0, l=0; l<N-1; l++){
	//cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
	if(((GHA_tab[l]-sum) > M_PI) && (GHA_tab[l+1] < M_PI)){
	  sum += 2.0*M_PI;
	}
	GHA_tab[l+1] += sum;
      }

      cout << "Read values:\n";
      for(l=0; l<N; l++){
	cout << mjd_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
      }

      gsl_spline_init(interpolation_GHA, mjd_tab, GHA_tab, (unsigned int)N);
      gsl_spline_init(interpolation_d, mjd_tab, d_tab, (unsigned int)N);

      
      //add minus sign because in JPL convention longitude is positive when it is W
      GHA.set("GHA", gsl_spline_eval(interpolation_GHA, (time.mjd)-mjd_min-((double)l_min)/24.0, acc));
      d.set("d", gsl_spline_eval(interpolation_d, (time.mjd)-mjd_min-((double)l_min)/24.0, acc));

    }

  }

  gsl_interp_accel_free(acc);
  gsl_spline_free(interpolation_GHA);
  gsl_spline_free(interpolation_d);
  
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

  if((D >= 1) && (D <= ((int)days_per_month[M-1]))){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Chrono::check_h(void){

  if((h >= 0) && (h < 24)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Chrono::check_m(void){

  if((m >= 0) && (m < 60)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

};

bool Chrono::check_s(void){

  if((s >= 0.0) && (s < 60.0)){return true;}
  else{
    cout << "Entered value is not valid! Try again.\n";
    flush(cout);
    return false;
  }

}

stringstream Chrono::to_string(void){

  stringstream output;
  
  if(h<10){output << 0;}
  output << h << "-";
  if(m<10){output << 0;}
  output << m << "-";
  if(s<10.0){output << 0;}
  output << s;

  return output;
  
}


void Chrono::print(const char* name){

  cout << name << " is " << to_string().str().c_str() << "\n";

};

void Chrono::enter(const char* name) {

  cout << "Enter hour of " << name << " [hh-mm-ss]\n";

  do{
    cout << "\tEnter hh: ";
    cin >> h;
  }while(!check_h());

  do{
    cout << "\tEnter mm: ";
    cin >> m;
  }while(!check_m());

  do{
    cout << "\tEnter ss: ";
    cin >> s;
  }while(!check_s());

  //print(name);

}



stringstream Time::to_string(void){

  stringstream output;
  
  output << Y << " ";
  if(M<10){output << 0;}
  output << M << " ";
  if(D<10){output << 0;}
  output << D << " ";
  output << chrono.to_string().str().c_str();
  output << " UTC";
  //output << " (" << mjd << " MJD)\n";

  return output;
  
}


void Time::print(const char* name){

  cout << name << " is " << to_string().str().c_str() << "\n";

};



void Time::enter(const char* name) {

  cout << "Enter date of " << name << " [YYYY MM DD]\n";

  do{
    cout << "\tEnter YYYY: ";
    cin >> Y;
  }while(!check_Y());
  
  check_leap_year();
  if(Y_is_leap_year){
    days_per_month = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    cout << "Entered a leap year\n";
  }else{
    days_per_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    cout << "Entered a common year\n";
  }
  
  do{
    cout << "\tEnter MM: ";
    cin >> M;
  }while(!check_M());

  do{
    cout << "\tEnter DD: ";
    cin >> D;
  }while(!(check_D()));

  chrono.enter(name);
  
  to_mjd();
  print(name);

}

void Time:: to_utc(void){
  //int &day, int &month, int &year, double &hour)
  /*
    Calculate the calendar date from the Modified Julian Date
   
    INPUT :
    mjd : Modified Julian Date (Julian Date - 2400000.5)
   
    OUTPUT :
    day, month, year : corresponding date
    hour : corresponding hours of the above date
  */
   
  int Yt, Mt, Dt;
  double ht;

  long int b, c, d, e, f, jd0;
   
  jd0 = long(mjd +  2400001.0);
  if (jd0 < 2299161) c = jd0 + 1524;    /* Julian calendar */
  else
    {                                /* Gregorian calendar */
      b = long (( jd0 - 1867216.25) / 36524.25);
      c = jd0 + b - (b/4) + 1525;
    };
   
  if (mjd < -2400001.0)  // special case for year < -4712
    {
      if (mjd == floor(mjd)) jd0 = jd0 + 1;
      c = long((-jd0 - 0.1)/ 365.25);
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
      d = long ((c - 122.1) / 365.25);
      e = 365 * d + (d/4);
      f = long ((c - e) / 30.6001);
      Dt = c - e - long(30.6001 * f);
      Mt = f - 1 - 12 * (f / 14);
      Yt = d - 4715 - ((7 + Mt) / 10);
    };
   
  ht = 24.0 * (mjd - floor(mjd));
    

  Y = ((unsigned int)Yt);
  M = ((unsigned int)Mt);
  D = ((unsigned int)Dt);

  (chrono.h) = (unsigned int)(floor(ht));
  (chrono.m) = floor((ht-floor(ht))*60.0);
  (chrono.s) = (((ht-floor(ht))*60.0) - floor((ht-floor(ht))*60.0))*60.0;

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
  mjd = mjd + b + int(30.6001 * (Mt + 1)) + Dt + (((double)(chrono.h)) + ((double)(chrono.m))/60.0 + ((double)(chrono.s))/(60.0*60.0)) / 24.0;
   
  
}


