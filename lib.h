#define my_precision 32
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
//check that these values are covered by ephemerides data
#define Y_min 2016
#define Y_max 2017
//MJD_min corresponds to Jan 1 2016 00-00-26.00 TAI, i.e., Jan 1 2016 00-00-00.00 UTC  
#define MJD_min 57388.000300925923511385917663574
//NASA's webgeocalc datafiles show L lines per day
#define L 24.0
//the time window in which interpolation is made has a width of N lines in NASA's webgeocalc files
#define N 24.0
#define epsrel (1e-12)
#define max_iter (1e3)
//one nautical mile in kilometers
#define nm 1.852
#define RED     "\033[31m"      /* Red */
#define RESET   "\033[0m"


/*
 void youprint(string input, string prefix)
{
  cout << prefix << input << endl;
}


void myprint(string input, string prefix)
{
  cout << prefix << input << endl;
  stringstream new_prefix;
  new_prefix << "--" << prefix;
  youprint(input, new_prefix.str());
}
*/

//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal

class Catalog;

class Answer{

 public:
  char value;
  void enter(string, string);
  //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
  void print(string, string, ostream&);

};

class String{

 public:
  string value;
  
  void enter(string, string);
  void print(string, string, ostream&);
  void set(string, string);

};

void String::enter(string name, string prefix){

  
  cout << prefix << "Enter " << name << ":\n";

  getline(cin >> ws, value);

  print(name, prefix, cout);

}

void String::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << value << "\n";
  
}

void String::set(string name, string prefix){

  value = name;
  print(name, prefix, cout);

}



class Angle{

 public:
 
  double value;
  void normalize(void);
  void enter(string, string);
  void set(string, double);
  void print(string, string, ostream&);

  Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
  
};


class Point{

 public:
  //latitude and longitude of the point
  Angle phi, lambda;
  String label;
  
  void enter(string, string);
  void print(string, string, ostream&);

};

class Date{

 public:
  int Y, M, D;
  bool Y_is_leap_year;
  vector<unsigned int> days_per_month;

  void print(string, string, ostream&);
  void enter(string, string);
  stringstream to_string(void);
  bool check_Y(string), check_M(string), check_D(string);
  void check_leap_year(void);

};

class Chrono{

 public:
  unsigned int h, m;
  double s;

  bool check_h(string), check_m(string), check_s(string);
  void print(string, string, ostream&);
  void enter(string, string);
  stringstream to_string(void);

};



class Time{

 public:
  Date date;
  Chrono chrono;
  //is s used?
  double s, MJD;
  void enter(string, string);
  void print(string, string, ostream&);
  
  void to_MJD(void);
  void to_TAI(void);
  void add(Chrono);
  
  stringstream to_string(void);
  
};



void Date::check_leap_year(void){

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

  MJD += (((double)(chrono_in.h)) + ((double)(chrono_in.m))/60.0 + ((double)(chrono_in.s))/(60.0*60.0))/24.0;
  to_TAI();

}

class File{

 public:
  fstream value;
  string name;
  void set_name(string);
  int open(string);
  void close(void);
  void remove(void);
  
};

void File::remove(void){

  stringstream command;

  command.str("");
  command << "rm -rf " << name  << "> /dev/null 2>&1";
  system(command.str().c_str());

  
}

void File::set_name(string filename){

  name = filename;
  
}

int File::open(string mode){

  if(mode =="in"){
    value.open(name, ios::in);
  }else{
    value.open(name, ios::out);
  }
  
  cout << "Opening " << name << " in mode '" <<  mode << "' ... ";
  
  if(!value){
    
    cout << RED << "Frror opening file " << name << "!\n" << RESET;
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
  void set(string, double);
  void enter(string, string);
  void print(string, string, ostream&);

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
  void enter(string, string);
  void print(string, string, ostream&);

};


class Body{

 public:
  string name, type;
  Length radius;
  Angle RA, d; 
  void enter(Catalog, string);
  void print(string, string, ostream&);
  
};


class Catalog{

 public:
  vector<Body> list;
  Catalog(string);
  void add(string, string, double);
  void print(string, ostream&);

};

Catalog::Catalog(string filename){

  File file;
  string line;
  stringstream line_ins;
  Body temp;


  file.set_name(filename);
  if(file.open("in")==1){

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

void Catalog::print(string prefix, ostream& ostr){

  unsigned int i;
  stringstream new_prefix, name;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;
  
  for(i=0; i<list.size(); i++){

    name.str("");
    name << "Body #" << i;
    (list[i]).print(name.str(), new_prefix.str(), ostr);

  }

}

void Catalog::add(string type, string name, double radius){

  Body body;
  
  body.type = type;
  body.name = name;
  body.radius.value = radius;
  
  list.push_back(body);
  
  cout << "Added body to catalog:\n";
  body.print("body", "\t", cout);

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


void Answer::enter(string name, string prefix){

  bool check;
  
  do{

    cout << prefix << "Enter " << name << " [y/n]:\n";
    cin >> value;
    
    if((value=='y') || (value=='n')){
      check = true;
    }
    else{
      cout << prefix << RED << "Entered value is not valid!\n" << RESET;
      flush(cout);
      check = false;
    }
  }while(!check);

  print(name, prefix, cout);

}

void Answer::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << value << "\n";
  
}



class Sight{

 public:
  Time time;
  //stopwatch is the reading [hh:mm:ss.s] on the stopwatch
  Chrono TAI_minus_UTC, stopwatch;
  Point GP;
  Angle index_error, H_s, H_a, H_o, H_i, DH_refraction, DH_dip, DH_parallax_and_limb;
  Length r, height_of_eye;
  Atmosphere atmosphere;
  Body body;
  Limb limb;
  // use_stopwatch = 'n' -> time is in format "UTC" time. use_stopwatch  = 'y' -> master clock UTC time + stopwatch reading
  Answer artificial_horizon, use_stopwatch; 

  Sight();
  static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
  void get_coordinates(void);
  void compute_DH_dip(void);
  void compute_DH_refraction(void);
  void compute_DH_parallax_and_limb(void);

  void compute_H_a(void);
  void compute_H_o(void);

  void enter(Catalog, string, string);
  void print(string, string, ostream&);
  void reduce(void);
  
};

void Sight::print(string name, string prefix, ostream& ostr){

  stringstream new_prefix;
  new_prefix << "\t" << prefix;    

  ostr << prefix << name << ":\n";

  body.print("body", new_prefix.str(), ostr);
  if(body.type != "star"){
    limb.print("limb", new_prefix.str(), ostr);
  }
  H_s.print("sextant altitude", new_prefix.str(), ostr);
  index_error.print("index error", new_prefix.str(), ostr);
  artificial_horizon.print("artificial horizon", new_prefix.str(), ostr);
  if(artificial_horizon.value == 'n'){
    height_of_eye.print("height of eye", new_prefix.str(), ostr);
  }
  use_stopwatch.print("use of stopwatch", new_prefix.str(), ostr);
  if(use_stopwatch.value == 'y'){
    stopwatch.print("stopwatch", new_prefix.str(), ostr);
  }
  TAI_minus_UTC.print("TAI - UTC at time of master-clock synchronization with UTC", new_prefix.str(), ostr);
  time.print("TAI date and hour of sight", new_prefix.str(), ostr);

}

class Plot{
  
 public:
  Catalog* catalog;
  File file_id, file_gnuplot;
  int job_id;
  stringstream command, plot_command;
  vector<Sight> sight_list;
  vector<Point> point_list;
  vector<string> choices;

  Plot(Catalog*);
  //~Plot();
  void add_sight(void);
  void add_point(void);
  void remove_sight(unsigned int);
  void remove_point(unsigned int);
  void print(string, ostream&);
  void show(void);
  void menu(void);

};

void Plot::menu(void){

  unsigned int i;
  bool check;

  do{
    
    cout << "What do you want to do? [choice #]\n";
    for(i=0; i<choices.size(); i++){
      cout << "\t(" << i+1 << ") " << choices[i] << "\n";
    }
    cin >> i;
    
    if(!((0<i) && (i<choices.size()+1))){
      
      cout << RED << "Enterd value is not valid!\n" << RESET;
      check = true;
      
    }else{
      
      check = false;
      
    }
    
  }while(check);
  
  
  switch(i){

  case 1:{
    add_sight();
    print("\t", cout);
    show();
    menu();  

  }
    break;
    
  case 2:{

    do{

      print("\t", cout);
      cout << "Which sight do you want to delete? [sight #]\n";
      cin >> i;
      i--;

      if(!((0<=i) && (i<sight_list.size()))){
	cout << RED << "Enterd value is not valid!\n" << RESET;
	check = true;
      }
      else{
	check=false;
      }
    }while(check);
    
    remove_sight(i);
    print("\t", cout);
  show();
  menu();  

   
  }
    break;

  case 3:{

    add_point();
    print("\t", cout);
    show();
    menu();  

  }
    break;

  case 4:{

    do{

      print("\t", cout);
      cout << "Which point do you want to delete? [point #]\n";
      cin >> i;
      i--;

      if(!((0<=i) && (i<point_list.size()))){
	cout << RED << "Enterd value is not valid!\n" << RESET;
	check = true;
      }
      else{
	check=false;
      }
    }while(check);
    
    remove_point(i);
    print("\t", cout);
    show();
    menu();  

  }
    break;

    
  case 5:{

    File file;
    string line;

    command.str("");
    command << "rm -rf output.out; date \"+%Y-%m-%d %H:%M:%S\" >> output.out";
    system(command.str().c_str());

    file.name = "output.out";
    file.open("in");
    line.clear();
    getline(file.value, line);
    line.append(".saved");
    file.close();
    
    command.str("");
    command << "rm -rf output.out";
    system(command.str().c_str());
 
    file.name = line;
    file.open("out");
    print("", file.value);
    file.close();
    
    cout << "Fair winds, following seas...\n";
  }
    break;


  }

    
}

Plot::Plot(Catalog* cata){

  catalog = cata;
  job_id = -1;

  plot_command.precision(my_precision);
  command.precision(my_precision);

  file_id.set_name("job_id.txt");
  file_gnuplot.set_name("plot.plt");

  choices = {"Add a sight", "Delete a sight", "Add a point", "Delete a point", "Save and exit"};
  
}

/*
Plot::~Plot(){

  //file_gnuplot.remove();
  file_id.remove();
  
}
*/

void Plot::print(string prefix, ostream& ostr){

  stringstream new_prefix;
  stringstream name;
  unsigned int i;
  
  new_prefix << "\t" << prefix;
  
  ostr << "Sights in the plot:\n";
  for(i=0; i<sight_list.size(); i++){
    name.str("");
    name <<  "Sight #" << i+1;
    (sight_list[i]).print(name.str().c_str(), new_prefix.str(), ostr);
  }
  
  ostr << "Points in the plot:\n";
  for(i=0; i<point_list.size(); i++){
    name.str("");
    name << "Point #" << i+1;
    (point_list[i]).print(name.str().c_str(), new_prefix.str(), ostr);
  }


}

void Plot::add_sight(){

  Sight sight;
  
  sight.enter((*catalog), "new sight", "");
  sight.reduce();
  sight.print("Sight", "", cout);
  
  sight_list.push_back(sight);
  cout << "Sight added as sight #" << sight_list.size() << ".\n";

  //sight.~Sight();

}

void Plot::add_point(){

  Point point;
  
  point.enter("new point", "");
  
  point_list.push_back(point);
  cout << "Point added as point #" << point_list.size() << ".\n";

 
}


void Plot::remove_sight(unsigned int i){

  stringstream name;

  name.str("");
  name << "Sight to be removed: Sight #" << i+1;
  
  (sight_list[i]).print(name.str().c_str(), "\t", cout);
  
  sight_list.erase(sight_list.begin()+i);
  
  cout << "Sigh removed.\n";

}

void Plot::remove_point(unsigned int i){

  stringstream name;

  name.str("");
  name << "Point to be removed: Point #" << i+1;
  
  (point_list[i]).print(name.str().c_str(), "\t", cout);
  
  point_list.erase(point_list.begin()+i);
  
  cout << "Point removed.\n";

}


void Plot::show(void){

  stringstream line_ins;
  string line;
  unsigned int i;

  //if job_id = -1 this means that there is no gnuplot script running in the background, thus there is no need to stop it. Otherwise, the gnuplot script running in the background is stopped. 
  if(job_id != -1){
    
    command.str("");
    command << "kill -9 " << job_id;
    system(command.str().c_str());
    
  }

  file_id.remove();
  file_gnuplot.remove();
  
  command.str("");

  //replace line with sight plots
  
  plot_command.str("");
  for(i=0, plot_command.str(""); i<sight_list.size(); i++){
    plot_command << "replot [0.:2.*pi] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) w d ti \"" << (sight_list[i]).body.name << " " << (sight_list[i]).time.to_string().str().c_str() << " TAI\"\\\n";
  }  
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "sed 's/#sight_plots/" << plot_command.str().c_str() << "/g' plot_dummy.plt >> plot_temp.plt \n";


  
  //replace line with point plots
  
  plot_command.str("");
  for(i=0; i<point_list.size(); i++){
    plot_command << "replot \"+\" u (xe(K*" << ((point_list[i]).lambda.value - M_PI) << ")):(ye(K*" << (point_list[i]).phi.value << ")) w p  lw 2 ti \"" << (point_list[i]).label.value << "\"\\\n";
  }
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "sed 's/#point_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> " << file_gnuplot.name << "\n";


  command << "gnuplot '" << file_gnuplot.name << "' & \n echo $! >> " << file_id.name << "\n";
  command << "rm -rf plot_temp.plt";
  
  system(command.str().c_str());

  //read the job id from file_id
  if(file_id.open("in")==1){
    getline(file_id.value, line);
    line_ins << line;
    line_ins >> job_id;
  }

  file_id.close();
  file_id.remove();
  
  cout << "\nJob id = "<< job_id << "\n";
  
}

void Sight::enter(Catalog catalog, string name, string prefix){

  stringstream new_prefix;

  //append \t to prefix
  //strcpy(new_prefix, prefix);    
  //new_prefix[strlen(prefix)] = '\t';
  new_prefix << prefix << "\t";
  
  cout << prefix << "Enter " << name << ":\n";
  
  body.enter(catalog, new_prefix.str());
  GP.label.set("geographic position", new_prefix.str());

  if(body.type != "star"){
    limb.enter("limb", new_prefix.str());
  }
  H_s.enter("sextant altitude", new_prefix.str());
  index_error.enter("index error", new_prefix.str());
  artificial_horizon.enter("artificial horizon", new_prefix.str());
  if(artificial_horizon.value == 'n'){
    height_of_eye.enter("height of eye", new_prefix.str());
  }



  use_stopwatch.enter("use of stopwatch", new_prefix.str());
  if(use_stopwatch.value == 'n'){
    
    time.enter("master-clock date and hour", new_prefix.str());
    TAI_minus_UTC.enter("TAI - UTC at time of master-clock synchronization with UTC", new_prefix.str());
    time.add(TAI_minus_UTC);
    time.print("TAI time of sight", "", cout);
    
  }else{
        
    time.enter("master-clock date and hour", new_prefix.str());
    stopwatch.enter("stopwatch reading", new_prefix.str());
    time.add(stopwatch);
    TAI_minus_UTC.enter("TAI - UTC at time of master-clock synchronization with UTC", new_prefix.str());
    time.add(TAI_minus_UTC);
    time.print("TAI date and hour of sight", new_prefix.str(), cout);
    
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
    H_a.print("apparent altitude", "", cout);

  }else{
    compute_DH_dip();
    H_a = H_s-index_error+DH_dip;
    H_a.print("apparent altitude", "", cout);
  }
  
}


void Sight::compute_H_o(void){

  compute_DH_refraction();
  compute_DH_parallax_and_limb();
  H_o = H_a + DH_refraction + DH_parallax_and_limb;
  H_o.print("observed altitude", "", cout);
  
}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(void){

  H_i = H_a + DH_refraction;
  H_i.print("intermediate altitude", "", cout);

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
 
	printf("using %s method\n", gsl_root_fsolver_name (s));
	printf("%5s [%9s, %9s] %9s %10s %9s\n", "iter", "lower", "upper", "root", "err", "err(est)");

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
	  printf("%5d [%.7f, %.7f] %.7f %+.7f\n", iter, x_lo, x_hi, x, x_hi - x_lo);
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

    DH_parallax_and_limb.print("parallax and limb correction", "", cout);

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

    cout << RED << "Value of z is not valid!\n" << RESET;
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

    cout << RED << "Value of z is not valid!\n"<< RESET;
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

    cout << RED << "Value of z is not valid!\n" << RESET;
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
  
  //cout << "Atmosphere model: US 1976.\n";

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



void Body::print(string name_in, string prefix, ostream& ostr){

  stringstream new_prefix;
  new_prefix << prefix << "\t";
  
  ostr << prefix << name_in << ":\n";
  
  ostr << new_prefix.str() << "Type: " << type << "\n";
  ostr << new_prefix.str() << "Name: " << name << "\n";
  if((radius.value) != 0.0){
    radius.print("Radius", new_prefix.str(), ostr);
  }
  if(type == "star"){
    RA.print("Right ascension", new_prefix.str(), ostr);
    d.print("Declination", new_prefix.str(), ostr);
  }
 
}


void Body::enter(Catalog catalog, string prefix){

  unsigned int i;
  bool check;
  string s;
  stringstream new_prefix;

  new_prefix << prefix << "\t";

  
  do{
    cout << prefix << "Enter name of body:";
    cin >> s;

    for(i=0, check=true; (i<(catalog).list.size()) && check; i++){if((((catalog).list)[i]).name == s){check=false;}}
    if(check){cout << prefix << RED << "Body not found in catalog!\n" << RESET;}
      
  }while(check);
  
  i--;
  (*this) = (catalog.list)[i];
  print("body", prefix, cout);
  
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

void Length::set(string name, double x){

  if(x>=0.0){
    value = x;
    print(name, "", cout); 
  }
  else{
    cout << RED << "Entered value of " << name << " is not valid!\n" << RESET;
  }
  
}

void Length::enter(string name, string prefix){

  bool check;

  do{
    
    cout << prefix << "Enter " << name << " [m]:\n";
    cin >> value;
    if(value < 0.0){
      cout << prefix << RED << "Entered value of " << name << " is not valid!\n" << RESET;
      check = true;
    }else{
      check = false;
    }

  }while(check);
  //convert to nautical miles
  value/=(1e3*nm);
    
  print(name, prefix, cout); 
  
}

void Length::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << value << " nm.\n";
 
}

void Sight::get_coordinates(void){

  File file;
  stringstream filename, line_ins;
  string line, dummy, temp;
  unsigned int l, l_min, l_max;
  double MJD_tab[(unsigned int)N], GHA_tab[(unsigned int)N], d_tab[(unsigned int)N], sum;
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
  if(file.open("in")==1){

    /* cout << "\nMJD = " << t.MJD; */
    /* cout << "\nMJD0 = " << MJD_min; */
    /* cout << "\ndiff = " << (t.MJD)-MJD_min; */
    /* cin >>l ; */

    //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
    l_min = (unsigned int)(L*((time.MJD)-MJD_min))-(unsigned int)(N/2.0);
    //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
    l_max = (unsigned int)(L*((time.MJD)-MJD_min))+(unsigned int)(N/2.0);

    /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */

    //dummy read of file data
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
	
	MJD_tab[l-l_min] = ((double)(l-l_min))/L;
	
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

      gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N);
      gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N);
      gsl_spline_init(interpolation_r, MJD_tab, r_tab, (unsigned int)N);

  
      cout << "Read values:\n";
      for(l=0; l<N; l++){
	cout << MJD_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
      }

      
      (GP.lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc));
      (GP.phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc));
      r.set("r", gsl_spline_eval(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc));

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

	  
	MJD_tab[l-l_min] = ((double)(l-l_min))/L;
	
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
	cout << MJD_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
      }

      gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N);
      gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N);

      
      //add minus sign because in JPL convention longitude is positive when it is W
      (GP.lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc));
      (GP.phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc));

    }

  }

  gsl_interp_accel_free(acc);
  gsl_spline_free(interpolation_GHA);
  gsl_spline_free(interpolation_d);
  
}

void Angle::set(string name, double x){

  value = x;
  normalize();
  print(name, "", cout);
  
}

void Angle::enter(string name, string prefix){

  string s;
  int ad;
  double am;
  bool check;
  
  cout << prefix << "Enter " << name << " [s ddd mm.m]:\n";

  do{
    cout << prefix << "\tEnter s: ";
    cin >> s;
    
    if((s=="+") || (s=="-")){check = true;}
    else{
      cout << prefix << RED << "\tEntered value is not valid!\n" << RESET;
      flush(cout);
      check = false;
    }
  }while(!check);

  
  do{
    cout << prefix << "\tEnter ddd: ";
    cin >> ad;
    
    if((abs(ad) >= 0) && (abs(ad) < 360)){check = true;}
    else{
      cout << prefix << RED << "\tEntered value is not valid!\n" << RESET;
      flush(cout);
      check = false;
    }
  }while(!check);
  
  do{
    cout << prefix << "\tEnter mm: ";
    cin >> am;

    if((am >= 0.0) && (am < 60.0)){check = true;}
    else{
      cout << prefix << RED << "\tEntered value is not valid!.\n" << RESET;
      flush(cout);
      check = false;
    }
  }while(!check);

  value = k*(ad + am/60.0);
  if(s=="-"){value*=-1.0;}
  normalize();
  print(name, prefix, cout);


}

void Point::enter(string name, string prefix){

  bool check;
  stringstream new_prefix;

  new_prefix << "\t" << prefix;    

  cout << prefix << "Enter " << name << ":\n";

  do{
    phi.enter("latitude", new_prefix.str());
    if(!(((0.0 <= phi.value) && (M_PI/2.0 >= phi.value)) || ((3.0*M_PI/2.0 <= phi.value) && (2.0*M_PI >= phi.value)))){
      cout << new_prefix.str() << RED << "Entered value is not valid!\n" << RESET;
      check = true;
    }else{
      check = false;
    }
  }while(check);
  
  lambda.enter("longitude", new_prefix.str());
  label.enter("label", new_prefix.str());
  
}

void Point::print(string name, string prefix, ostream& ostr){

  stringstream new_prefix;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  ostr << prefix << name << ":\n";

  phi.print("latitude", new_prefix.str(), ostr);
  lambda.print("longitude", new_prefix.str(), ostr);

  label.print("label", new_prefix.str(), ostr);

}


void Angle::normalize(void){

  value = value - 2.0*M_PI*floor(value/(2.0*M_PI));

}


void Angle::print(string name, string prefix, ostream& ostr){

  normalize();
  ostr << prefix << name << " is " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'\n";

}

void Limb::enter(string name, string prefix){

  bool check;

  do{
    cout << prefix << "Enter " << name << " [u/l/c]:\n";
    cin >> value;
    
    if((value=='u') || (value=='l') || (value=='c')){check = true;}
    else{
      cout << prefix << RED << "Entered value is not valid!\n" << RESET;
      check = false;
    }
  }while(!check);
  
  print(name, prefix, cout);
  
}


void Limb::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << value << "\n";
  
}


bool Date::check_Y(string prefix){

  if((Y >= Y_min) && (Y <= Y_max)){return true;}
  else{
    cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    return false;
  }

};


bool Date::check_M(string prefix){

  if((M >= 1) && (M <= 12)){return true;}
  else{
    cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    flush(cout);
    return false;
  }

};

bool Date::check_D(string prefix){

  if((D >= 1) && (D <= ((int)days_per_month[M-1]))){return true;}
  else{
    cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    return false;
  }

};

bool Chrono::check_h(string prefix){

  if((h >= 0) && (h < 24)){return true;}
  else{
    cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    return false;
  }

};

bool Chrono::check_m(string prefix){

  if((m >= 0) && (m < 60)){return true;}
  else{
    cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    return false;
  }

};

bool Chrono::check_s(string prefix){

  if((s >= 0.0) && (s < 60.0)){return true;}
  else{
    cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    return false;
  }

}

stringstream Chrono::to_string(void){

  stringstream output;
  
  if(h<10){output << 0;}
  output << h << ":";
  if(m<10){output << 0;}
  output << m << ":";
  if(s<10.0){output << 0;}
  output << s;

  return output;
  
}

void Date::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << to_string().str().c_str() << "\n";

};

void Date::enter(string name, string prefix) {

  stringstream new_prefix;

  //append \t to prefix
  new_prefix << prefix << "\t";
  
  cout << prefix << "Enter " << name << " [YYYY-MM-DD]\n";

  do{
    cout << prefix << "\tEnter YYYY: ";
    cin >> Y;
  }while(!check_Y(new_prefix.str()));

  check_leap_year();
  if((Y_is_leap_year)){
    (days_per_month) = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    cout << new_prefix.str() << "Entered a leap year\n";
  }else{
    (days_per_month) = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    cout << new_prefix.str() << "Entered a common year\n";
  }

  do{
    cout << prefix << "\tEnter MM: ";
    cin >> M;
  }while(!check_M(new_prefix.str()));

  do{
    cout << prefix << "\tEnter DD: ";
    cin >> D;
  }while(!check_D(new_prefix.str()));
  
}


stringstream Date::to_string(void){

  stringstream output;
  
  output << Y << "-";
  if(M<10){output << 0;}
  output << M << "-";
  if(D<10){output << 0;}
  output << D;

  return output;
  
}


void Chrono::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << to_string().str().c_str() << "\n";

};

void Chrono::enter(string name, string prefix) {

  stringstream new_prefix;

  //append \t to prefix
  new_prefix << prefix << "\t";
  
  cout << prefix << "Enter " << name << " [hh-mm-ss]\n";

  do{
    cout << prefix << "\tEnter hh: ";
    cin >> h;
  }while(!check_h(new_prefix.str()));

  do{
    cout << prefix << "\tEnter mm: ";
    cin >> m;
  }while(!check_m(new_prefix.str()));

  do{
    cout << prefix << "\tEnter ss: ";
    cin >> s;
  }while(!check_s(new_prefix.str()));
  
}



stringstream Time::to_string(void){

  stringstream output;

  output << date.to_string().str() << " " << chrono.to_string().str();
 
  return output;
  
}


void Time::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " is " << to_string().str().c_str() << "\n";

};



void Time::enter(string name, string prefix) {

  stringstream new_prefix;

  //append \t to prefix
  new_prefix << prefix << "\t";
  
  cout << prefix << "Enter master-clock date and hour\n";
  
  date.enter("date", new_prefix.str());
  chrono.enter("hour", new_prefix.str());
  
  to_MJD();
  print(name, prefix, cout);

}

void Time:: to_TAI(void){
  //int &day, int &month, int &year, double &hour)
  /*
    Calculate the calendar date from the Modified Julian Date
   
    INPUT :
    MJD : Modified Julian Date (Julian Date - 2400000.5)
   
    OUTPUT :
    day, month, year : corresponding date
    hour : corresponding hours of the above date
  */
   
  int Yt, Mt, Dt;
  double ht;

  long int b, c, d, e, f, jd0;
   
  jd0 = long(MJD +  2400001.0);
  if (jd0 < 2299161) c = jd0 + 1524;    /* Julian calendar */
  else
    {                                /* Gregorian calendar */
      b = long (( jd0 - 1867216.25) / 36524.25);
      c = jd0 + b - (b/4) + 1525;
    };
   
  if (MJD < -2400001.0)  // special case for year < -4712
    {
      if (MJD == floor(MJD)) jd0 = jd0 + 1;
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
   
  ht = 24.0 * (MJD - floor(MJD));
    

  (date.Y) = ((unsigned int)Yt);
  (date.M) = ((unsigned int)Mt);
  (date.D) = ((unsigned int)Dt);

  (chrono.h) = (unsigned int)(floor(ht));
  (chrono.m) = floor((ht-floor(ht))*60.0);
  (chrono.s) = (((ht-floor(ht))*60.0) - floor((ht-floor(ht))*60.0))*60.0;

}

void Time:: to_MJD(void)
  
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
   
  MJD = 365.0 * Yt - 679004.0;
  //comment this out if you want to include hours, minutes and seconds in MJD
  MJD = MJD + b + int(30.6001 * (Mt + 1)) + Dt + (((double)(chrono.h)) + ((double)(chrono.m))/60.0 + ((double)(chrono.s))/(60.0*60.0)) / 24.0;
   
  
}


