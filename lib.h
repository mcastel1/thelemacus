//this is the high precision used for storing data and making calculations with it 
#define data_precision 32
//this is the low precision used for displaying data
#define display_precision 4
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
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
#define RED     "\033[1;31m"      /* Red */
#define YELLOW     "\033[1;33m"      /* Red */
#define RESET   "\033[0m"
//all possible chars that can enter in an unsigned integer
#define chars_unsigned_int "0123456789"
//all possible chars that can enter in a non-negative double
#define chars_double "0123456789."
#define days_per_month_leap {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
#define days_per_month_common {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}

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

//this function asks the user to enter an unsigned int from keyboard and checks whether the entered value is an unsigned int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_unsigned_int(unsigned int* i, bool check_interval, unsigned int min, unsigned int sup, string name, string prefix){

  string s;
  bool check;

  do{
    
    s.clear();

    cout << prefix << "Enter " << name << ":";
    getline(cin >> ws, s);

    if(/*here I check whether the quantity entered in s is an unsigned integer, i.e., it contains only the characters 0123456789*/ ((s.find_first_not_of(chars_unsigned_int)) == (std::string::npos))){
    
      (*i) = stoi(s, NULL, 10);
      
      if(check_interval){
	
	if(((*i) >= min) && ((*i) < sup)){
	  check = true;
	}else{
	  check = false;
	}
	
      }else{
	check = true;
      }
      
    }else{
      check = false;
    }

    if(!check){
     cout << prefix << RED << "\tEntered value is not valid!\n" << RESET;
    }
    
  }while(!check);

}


//this function asks the user to enter a double from keyboard and checks whether the entered value contains the allowed chars for double and, if check_interval = true, that the entered value lies in [min, sup)
void enter_double(double* x, bool check_interval, double min, double sup, string name, string prefix){

  string s;
  bool check;

  do{
    
    s.clear();

    cout << prefix << "Enter " << name << ":";
    getline(cin >> ws, s);

    if(/*here I check whether the quantity entered in s contains the allowed chars for double, i.e., it contains only the characters 0123456789.*/ ((s.find_first_not_of(chars_double)) == (std::string::npos))){

      (*x) = stod(s);
      
      if(check_interval){
	
	if(((*x) >= min) && ((*x) < sup)){
	  check = true;
	}else{
	  check = false;
	}
	
      }else{
	check = true;
      }
      
    }else{
      check = false;
    }

    if(!check){
     cout << prefix << RED << "Entered value is not valid!\n" << RESET;
    }
    
  }while(!check);

}


class Catalog;
class File;

class String{

 public:
  string value;
  
  void enter(string, string);
  void print(string, string, ostream&);
  void read_from_file(string, File&, string);
  void set(string, string);

};

class File{

 public:
  fstream value;
  String name;
  unsigned int number_of_lines;

  File();
  void set_name(string);
  void enter_name(string);
  int open(string, string);
  void close(string);
  void remove(void);
  void count_lines(string);
  
};


File::File(){

  value.precision(data_precision);
  
}

void String::read_from_file(string name, File& file, string prefix){

  string line;
  size_t pos;

  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");

  //read the string after ' = ' until the end of line string and store it into value
  value = line.substr(pos+3, line.size() - (pos+3)).c_str();

  print(name, prefix, cout);

  
}


class Answer{

 public:
  char value;
  void enter(string, string);
  //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
  void print(string, string, ostream&);
  void read_from_file(string, File&, string);

};



void Answer::read_from_file(string name, File& file, string prefix){

  string line;
  size_t pos;

  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");

  value = line[pos+3];

  print(name, prefix, cout);

  
}


void String::enter(string name, string prefix){

  
  cout << prefix << "Enter " << name << ":";
  getline(cin, value);

  if(value.empty()){
    cout << prefix << YELLOW << "Entered an empty " << name << ".\n" << RESET;
  }

  print(name, prefix, cout);

}

void String::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " = " << value << "\n";
  
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
  void set(string, double, string);
  void print(string, string, ostream&);
  void read_from_file(string, File&, string);

  Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
  
};



void Angle::read_from_file(string name, File& file, string prefix){

  string line;
  size_t pos1, pos2, pos3;

  line.clear();
  getline(file.value, line);

  pos1 = line.find(" = ");
  pos2 = line.find("° ");
  pos3 = line.find("'");
  
  value = k*(stod(line.substr(pos1+3, pos2 - (pos1+3)).c_str()) + stod(line.substr(pos2+2, pos3 - (pos2+2)))/60.0);

  print(name, prefix, cout);
  
}


class Point{

 public:
  //latitude and longitude of the point
  Angle phi, lambda;
  //label to add a note about the point
  String label;
  
  void enter(string, string);
  void print(string, string, ostream&);
  void read_from_file(File&, string);

};

void Point::read_from_file(File& file, string prefix){

  stringstream new_prefix;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  phi.read_from_file("latitude", file, new_prefix.str());
  lambda.read_from_file("longitude", file, new_prefix.str());
  label.read_from_file("label", file, new_prefix.str());

}

class Date{

 public:
  unsigned int Y, M, D;
  bool Y_is_leap_year;
  vector<unsigned int> days_per_month;

  void print(string, string, ostream&);
  void enter(string, string);
  bool read_from_file(string, File&, string);
  stringstream to_string(void);
  void check_leap_year(void);

};

class Chrono{

 public:
  unsigned int h, m;
  double s;

  void print(string, string, ostream&);
  void enter(string, string);
  bool read_from_file(string, File&, string);
  stringstream to_string(unsigned int);

};

















bool Chrono::read_from_file(string name, File& file, string prefix){

  string line;
  stringstream new_prefix;
  bool check = true;
  
  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  size_t pos = 0;

  //read type
  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");

  //read hours
  h = stoi(line.substr(pos+3, 2).c_str(), NULL, 10);
  if(!((0 <= h) && (h < 24))){
    
    check &= false;
    cout << prefix << RED << "\tValue of hh is not valid!\n" << RESET;
    
  }

  //read minutes
  m = stoi(line.substr(pos+3+3, 2).c_str(), NULL, 10);   
  if(!((0 <= m) && (m < 60))){
      
    check &= false;
    cout << prefix << RED << "\tValue of mm is not valid!\n" << RESET;
      
  }
  
  //read seconds
  s = stod(line.substr(pos+3+3+3, line.size() - (pos+3+3+3)).c_str());
  if(!((0.0 <= s) && (s < 60.0))){

    check &= false;
    cout << prefix << RED << "\tValue of mm is not valid!\n" << RESET;

  }

  if(check){
    print(name, prefix, cout);
  }
	  
  return check;

}


class Time{

 public:
  Date date;
  Chrono chrono;
  //is s used?
  double s, MJD;
  void enter(string, string);
  void print(string, string, ostream&);
  bool read_from_file(string, File&, string);
  
  void to_MJD(void);
  void to_TAI(void);
  void add(Chrono);
  
  stringstream to_string(unsigned int);
  
};





//this function returns true if the date read is consistent, false if it is not 
bool Date::read_from_file(string name, File& file, string prefix){

  string line;
  stringstream new_prefix;
  bool check = true;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  size_t pos = 0;

  //read type
  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");

  Y = stoi(line.substr(pos+3, 4).c_str(), NULL, 10);

  check_leap_year();
  if((Y_is_leap_year)){
    (days_per_month) = days_per_month_leap;
    cout << new_prefix.str() << "YYYY is a leap year\n";
  }else{
    (days_per_month) = days_per_month_common;
    cout << new_prefix.str() << "YYYY is a common year\n";
  }
  
  M = stoi(line.substr(pos+3+5, 2).c_str(), NULL, 10);

  if(!((1<=M) && (M < 12+1))){
    check &= false;
    cout << new_prefix.str() << RED << "\tValue of MM is not valid!\n" << RESET;
  }

  D = stoi(line.substr(pos+3+5+3, 2).c_str());

  if(!((1<=D) && (D < days_per_month[M-1]+1))){
    check &= false;
    cout << prefix << RED << "\tValue of DD is not valid!\n" << RESET;
  }
  
  if(check){
    print(name, prefix, cout);
  }
 
  return check;
  
}









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

bool Time::read_from_file(string name, File& file, string prefix){

  string line;
  stringstream new_prefix;
  bool check = true;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  //read dummy line
  getline(file.value, line);

  cout << prefix << name << ":\n";
  
  //read date
  if(!(date.read_from_file(name, file, new_prefix.str()))){ 
    check &= false;
  }

  //read chrono
  if(!(chrono.read_from_file(name, file, new_prefix.str()))){
    check &= false;
  }

  to_MJD();

  if(check){
    print(name, prefix, cout);
  }
   
  return check;
  
}

void Time::add(Chrono chrono_in){

  MJD += (((double)(chrono_in.h)) + ((double)(chrono_in.m))/60.0 + ((double)(chrono_in.s))/(60.0*60.0))/24.0;
  to_TAI();

}


void File::enter_name(string prefix){

  name.enter("name of file", prefix);
  
}

void File::count_lines(string prefix){

  stringstream command, line_ins;
  string line, dummy;
  File file_number_of_lines;
  stringstream new_prefix;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;


  file_number_of_lines.set_name("output.out");
  file_number_of_lines.remove();
  
  command.str("");
  command << "wc -l " << (name.value)  << " >> " << ((file_number_of_lines.name).value);
  system(command.str().c_str());

  file_number_of_lines.open("in", new_prefix.str());
  
  getline((file_number_of_lines.value), line); 
  line_ins << line;
  line_ins >> number_of_lines >> dummy;

  file_number_of_lines.close(new_prefix.str());  

  cout << prefix << "Number of lines in file " << (name.value) << " = " << number_of_lines << "\n";

  
}


void File::remove(void){

  stringstream command;

  command.str("");
  command << "rm -rf " << (name.value) << "> /dev/null 2>&1";
  system(command.str().c_str());

  
}

void File::set_name(string filename){

  (name.value) = filename;
  
}

int File::open(string mode, string prefix){


  if(mode =="in"){
    value.open(name.value, ios::in);
  }else{
    value.open(name.value, ios::out);
  }
  
  cout << prefix << "Opening " << (name.value) << " in mode '" <<  mode << "' ... ";
  
  if(!value){
    
    cout << RED << "\nError opening file " << (name.value) << "!\n" << RESET;
    return 0;
    
  }else{
    
    cout << "File " << (name.value) << " opened.\n";
    return 1;
     
  }

}

void File::close(string prefix){
  
  value.close();
  cout << prefix << "File " << (name.value) << " closed.\n";
     

}

class Length{

 public:
  double value;
  void set(string, double, string);
  void enter(string, string);
  void print(string, string, ostream&);
  void read_from_file(string, File&, string);
  bool check_valid(string, string);

};

bool Length::check_valid(string name, string prefix){

  bool check = true;
  
  if(value<0.0){
    check &= false;
    cout << prefix << RED << "Entered value of " << name << " is not valid!\n" << RESET;
  }

  return check;
  
}

void Length::read_from_file(string name, File& file, string prefix){

  string line;
  stringstream new_prefix;
  size_t pos1, pos2;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  line.clear();
  getline(file.value, line);
  pos1 = line.find(" = ");
  pos2 = line.find(" nm");

  value = stod(line.substr(pos1+3, pos2 - (pos1+3)).c_str());
  
  print("radius", prefix, cout);

}



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
  void read_from_file(string, File&, string);
  
};

void Limb::read_from_file(string name, File& file, string prefix){

  string line;
  size_t pos;

  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");

  value = line[pos+3];
  
  print(name, prefix, cout);

  
}

class Body{

 public:
  string name, type;
  Length radius;
  Angle RA, d; 
  void enter(Catalog, string);
  void print(string, string, ostream&);
  void read_from_file(string, File&, string);
  
};

void Body::read_from_file(string name, File& file, string prefix){

  string line;
  stringstream new_prefix;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  size_t pos;

  cout << prefix << name << ":\n";
  
  //read first line with no information
  getline(file.value, line);

  //read type
  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");
  type = line.substr(pos+3, line.size() - (pos+3));
  cout << new_prefix.str() << "Type = " << type << "\n";


  //read name
  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");
  ((*this).name) = line.substr(pos+3, line.size() - (pos+3));
  cout << new_prefix.str() << "Name = " << ((*this).name) << "\n";


  if(type == "star"){
    RA.read_from_file("right ascension", file, new_prefix.str());
    d.read_from_file("declination", file, new_prefix.str());
  }else{
    radius.read_from_file("radius", file, new_prefix.str());
  }
  
}


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
  if(file.open("in", "")==1){

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
  
  
    file.close("");

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
  string temp;
  
  do{

    temp.clear();
    
    cout << prefix << "Enter " << name << " [y/n]:";

    getline(cin >> ws, temp);
    
    if(((temp[0]=='y') || (temp[0]=='n')) && (temp.size() == 1)){
      value = temp[0];
      check = true;
    }
    else{
      cout << prefix << RED << "Entered value is not valid!\n" << RESET;
      check = false;
    }
  }while(!check);

  print(name, prefix, cout);

}

void Answer::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " = " << value << "\n";
  
}



class Sight{

 public:
  Time master_clock_date_and_hour, time;
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
  //label to add a note about the sight
  String label;

  Sight();
  static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
  bool get_coordinates(string);
  void compute_DH_dip(string);
  bool compute_DH_refraction(string);
  void compute_DH_parallax_and_limb(string);

  void compute_H_a(string);
  bool compute_H_o(string);

  void enter(Catalog, string, string);
  void print(string, string, ostream&);
  bool read_from_file(File&, string);
  bool reduce(string);
  bool check_data_time_interval(string);
  
};

//this function returns true if the reading operation has been performed without errors, false otherwise
bool Sight::read_from_file(File& file, string prefix){

  stringstream new_prefix;
  string line;
  bool check = true;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  body.read_from_file("body", file, new_prefix.str());
  if(body.type != "star"){
    limb.read_from_file("limb", file, new_prefix.str());
  }
  H_s.read_from_file("sextant altitude", file, new_prefix.str());
  index_error.read_from_file("index error", file, new_prefix.str());
  artificial_horizon.read_from_file("artificial horizon", file, new_prefix.str());
  if((artificial_horizon.value) == 'n'){
    height_of_eye.read_from_file("height of eye", file, new_prefix.str());
  }
  
  check &= master_clock_date_and_hour.read_from_file("master-clock date and hour of sight", file, new_prefix.str());
  if(!check){
    cout << prefix << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
  }
  time = master_clock_date_and_hour;
 
  use_stopwatch.read_from_file("use of stopwatch", file, new_prefix.str());

  if(use_stopwatch.value == 'y'){
      
    stopwatch.read_from_file("stopwatch", file, new_prefix.str());
    time.add(stopwatch);

  }
  
  TAI_minus_UTC.read_from_file("TAI - UTC at time of master-clock synchronization with UTC", file, new_prefix.str());
  time.add(TAI_minus_UTC);
  time.print("TAI date and hour of sight", new_prefix.str(), cout);

  //check whether the date and hour of sight falls within the time window covered by JPL data files
  check &= check_data_time_interval(prefix);

  label.read_from_file("label", file, new_prefix.str());

  if(!check){
    cout << prefix << RED << "Error reading sight!\n" << RESET;
  }
  
  return check;
  
}

bool Sight::check_data_time_interval(string prefix){
  
  int l_min, l_max;
  stringstream temp, new_prefix;
  File data_file;
  bool check;
  
  //prepend \t to prefix
  new_prefix << "\t" << prefix;

  
  //data_file is the file where that data relative to body are stored: I count the number of lines in this file and store them in data_file.number_of_lines
  temp.clear();
  if((body.type) != "star"){
    temp << "data/" << body.name << ".txt";
  }else{
    temp << "data/j2000_to_itrf93.txt";
  }
  data_file.set_name(temp.str()); 
  data_file.count_lines(new_prefix.str());

  //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
  l_min = (int)(L*((time.MJD)-MJD_min))-(int)(N/2.0);
  //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
  l_max = (int)(L*((time.MJD)-MJD_min))+(int)(N/2.0);

  //check whether the lines from l_min to l_max, which are used for the data interpolation, are present in the file where data relative to the body are stored 
  if((l_min >= 0) && (l_max < (int)(data_file.number_of_lines))){
    check = true;
  }else{
    check = false;
    cout << prefix << RED << "Time lies outside interval of NASA's JPL data files!\n" << RESET;
  }

  return check;

  
}

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
  master_clock_date_and_hour.print("master-clock date and hour of sight", new_prefix.str(), ostr);
  use_stopwatch.print("use of stopwatch", new_prefix.str(), ostr);
  if(use_stopwatch.value == 'y'){
    stopwatch.print("stopwatch", new_prefix.str(), ostr);
  }
  TAI_minus_UTC.print("TAI - UTC at time of master-clock synchronization with UTC", new_prefix.str(), ostr);

  label.print("label", new_prefix.str(), ostr);

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
  bool add_sight(string);
  void add_point(string);
  void remove_sight(unsigned int);
  void remove_point(unsigned int);
  bool read_from_file(String, string);
  void print(string, ostream&);
  void show(string);
  void menu(void);

};

bool Plot::read_from_file(String filename, string prefix){

  File file;
  stringstream line_ins, new_prefix;
  string line;
  size_t pos;
  bool check = true;

  //prepend \t to prefix
  new_prefix << "\t" << prefix;

 
  file.set_name(filename.value);
  
  if(!(file.open("in", new_prefix.str()))){
    
    check &= false;
    
  }else{

    //1. Here I read sights
    
    //read dummy text line '	Sights in the plot:"
    getline(file.value, line);

    line.clear();
    //read dummyt text line 
    getline(file.value, line);
    pos = line.find("Sight #");

    //if I have found 'Sight #' in the line above, then I proceed and read the relative sight
    while(pos != (string::npos)){
    
      cout << new_prefix.str() << "Found new sight!\n";
  
      //read the sight block
      Sight sight;

      //if I find a sight which returns an error message when read from file, to be conservative I do not add any of the following sights in the file to sight_list because they may contain other errors
      check &= (sight.read_from_file(file, new_prefix.str()));
      if(check){
	  
	check &= (sight.reduce(new_prefix.str()));

	if(check){
	  sight.print("New sight", new_prefix.str(), cout);
    
	  sight_list.push_back(sight);
	  cout << new_prefix.str() << "Sight added as sight #" << sight_list.size() << ".\n";
	}
	  
      }
      
      line.clear();
      //read dummyt text line 
      getline(file.value, line);
      pos = line.find("Sight #");
 
    }

    //2. Here I read points

    line.clear();
    //read dummy text line 
    getline(file.value, line);
    pos = line.find("Point #");

    //if I have found 'Point #' in the line above, then I proceed and read the relative point
    while(pos != (string::npos)){
    
      cout << new_prefix.str() << "Found new point!\n";
  
      //read the point block
      Point point;

      point.read_from_file(file, new_prefix.str());
	  
      point.print("New point", new_prefix.str(), cout);
    
      point_list.push_back(point);
      cout << new_prefix.str() << "Point added as point #" << point_list.size() << ".\n";
	  
      line.clear();
      //read dummyt text line 
      getline(file.value, line);
      pos = line.find("Point #");
 
    }

    
    file.close(new_prefix.str());

  }

  if(!check){
    cout << prefix << RED << "There was an error while reading file!\n" << RESET;
  }
  
  return check;
  
}

void Plot::menu(void){

  unsigned int i;

  cout << "You can:\n";
  for(i=0; i<choices.size(); i++){
    cout << "\t(" << i+1 << ") " << choices[i] << "\n";
  }
  enter_unsigned_int(&i, true, 1, choices.size()+1, "choice #", "");

  
  
  switch(i){

  case 1:{
    add_sight("\t");
    print("\t", cout);
    show("\t");
    menu();  

  }
    break;
    
  case 2:{

    if(sight_list.size() > 0){

 
      print("\t", cout);

      enter_unsigned_int(&i, true, 1, sight_list.size()+1, "# of sight that you want to delete", "");
	
      i--;
   
      remove_sight(i);
      print("\t", cout);
      show("\t");

    }else{
      cout << RED << "There are no sights to delete!\n" << RESET;
    }

    menu();  

   
  }
    break;

  case 3:{

    add_point("\t");
    print("\t", cout);
    show("\t");
    menu();  

  }
    break;

  case 4:{

    if(point_list.size() > 0){


      print("\t", cout);

      enter_unsigned_int(&i, true, 1, point_list.size()+1, "# of point that you want to delete", "");

      i--;
	
      remove_point(i);
      print("\t", cout);
      show("\t");

    }else{
      cout << RED << "There are no points to delete!\n" << RESET;
    }
    
    menu();  

  }
    break;

case 5:{

  if(sight_list.size() + point_list.size() > 0){
  
    File file;
    stringstream temp;
    
    file.name.enter("name of file (without extension)", "\t");
    //add the extension .sav to name of file
    temp.str("");
    temp << file.name.value << ".sav";
    file.set_name(temp.str());

    file.open("out","\t");    
    print("\t", file.value);
    file.close("\t");

    command.str("");
    command << "mv plot.plt " << "'plot " << temp.str() << "'";
    system(command.str().c_str());

  }else{    
      cout << RED << "There are no sights nor points to save!\n" << RESET;
  }
  menu();
    
  }
    break;

  case 6:{

    String filename;
    stringstream line_ins;
    
    filename.enter("name of file (without extension)", "\t");
    line_ins << filename.value << ".sav"; 
    filename.value = line_ins.str();
    
    read_from_file(filename, "\t");
        
    print("\t", cout);
    show("\t");
    
    menu();  

  }
    break;
    
    
  case 7:{

    File file;
    string line;

    command.str("");
    command << "rm -rf output.out; date \"+%Y-%m-%d %H:%M:%S\" >> output.out";
    system(command.str().c_str());

    ((file.name).value) = "output.out";
    file.open("in", "");
    line.clear();
    getline(file.value, line);
    line.append(".sav");
    file.close("");
    
    command.str("");
    command << "rm -rf output.out";
    system(command.str().c_str());
 
    ((file.name).value) = line;
    file.open("out", "");
    print("", file.value);
    file.close("");

    //if plot.plt has been filled, here I save it
    if(sight_list.size() + point_list.size() >0){
      command.str("");
      command << "mv plot.plt " << "'plot " << line.c_str() << "'";
      system(command.str().c_str());
    }
    
    cout << "Fair winds, following seas...\n";
  }
    break;


  }

    
}

Plot::Plot(Catalog* cata){

  catalog = cata;
  job_id = -1;

  plot_command.precision(data_precision);
  command.precision(data_precision);

  file_id.set_name("job_id.txt");
  file_gnuplot.set_name("plot.plt");

  choices = {"Add a sight", "Delete a sight", "Add a point", "Delete a point", "Save to file", "Read from file", "Exit"};
  
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
  
  ostr << prefix << "Sights in the plot:\n";
  for(i=0; i<sight_list.size(); i++){
    name.str("");
    name <<  "Sight #" << i+1;
    (sight_list[i]).print(name.str().c_str(), new_prefix.str(), ostr);
  }
  
  ostr << prefix << "Points in the plot:\n";
  for(i=0; i<point_list.size(); i++){
    name.str("");
    name << "Point #" << i+1;
    (point_list[i]).print(name.str().c_str(), new_prefix.str(), ostr);
  }


}

bool Plot::add_sight(string prefix){

  Sight sight;
  bool check = true;
  
  sight.enter((*catalog), "new sight", prefix);
  check &= (sight.reduce(prefix));

  if(check){
    sight.print("Sight", prefix, cout);
  
    sight_list.push_back(sight);
    cout << prefix << "Sight added as sight #" << sight_list.size() << ".\n";
  }

  return check;
  
}

void Plot::add_point(string prefix){

  Point point;
  
  point.enter("new point", prefix);
  
  point_list.push_back(point);
  cout << prefix << "Point added as point #" << point_list.size() << ".\n";

 
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


void Plot::show(string prefix){

  stringstream line_ins;
  string line;
  unsigned int i;
  Angle t_min, t_max;

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

    t_max.set("t_{max}", acos(-tan((sight_list[i]).GP.phi.value)*tan(M_PI/2.0 - ((sight_list[i]).H_o.value))), prefix);
    t_min.set("t_{min}", 2.0*M_PI - acos(-tan((sight_list[i]).GP.phi.value)*tan(M_PI/2.0 - ((sight_list[i]).H_o.value))), prefix);
    
    plot_command << "replot [0.:2.*pi] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp ti \"" << (sight_list[i]).body.name << " " << (sight_list[i]).time.to_string(display_precision).str().c_str() << " TAI\"\\\n";
  }  
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "sed 's/#sight_plots/" << plot_command.str().c_str() << "/g' plot_dummy.plt >> plot_temp.plt \n";


  
  //replace line with point plots
  
  plot_command.str("");
  for(i=0; i<point_list.size(); i++){
    plot_command << "replot \"+\" u (xe(K*(" << (point_list[i]).lambda.value << "))):(ye(K*(" << (point_list[i]).phi.value << "))) w p  lw 2 ti \"" << (point_list[i]).label.value << "\"\\\n";
  }
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "sed 's/#point_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> " << ((file_gnuplot.name).value) << "\n";


  command << "gnuplot '" << ((file_gnuplot.name).value) << "' & \n echo $! >> " << ((file_id.name).value) << "\n";
  command << "rm -rf plot_temp.plt";
  
  system(command.str().c_str());

  //read the job id from file_id
  if(file_id.open("in", "\t")==1){
    getline(file_id.value, line);
    line_ins << line;
    line_ins >> job_id;
  }

  file_id.close("\t");
  file_id.remove();
  
  cout << prefix << "Job id = "<< job_id << "\n";
  
}

void Sight::enter(Catalog catalog, string name, string prefix){

  stringstream new_prefix;
  File file;

  //append \t to prefix
  //strcpy(new_prefix, prefix);    
  //new_prefix[strlen(prefix)] = '\t';
  new_prefix << prefix << "\t";
  
  cout << prefix << "Enter " << name << ":\n";
  
  body.enter(catalog, new_prefix.str());
  //GP.label.set("geographic position", new_prefix.str());

  if(body.type != "star"){
    limb.enter("limb", new_prefix.str());
  }
  H_s.enter("sextant altitude", new_prefix.str());
  index_error.enter("index error", new_prefix.str());
  artificial_horizon.enter("artificial horizon", new_prefix.str());
  if(artificial_horizon.value == 'n'){
    height_of_eye.enter("height of eye", new_prefix.str());
  }
  
  do{
  
    master_clock_date_and_hour.enter("master-clock date and hour of sight", new_prefix.str());
    time = master_clock_date_and_hour;
    
    use_stopwatch.enter("use of stopwatch", new_prefix.str());

    if(use_stopwatch.value == 'y'){
        
      stopwatch.enter("stopwatch reading", new_prefix.str());
      time.add(stopwatch);
    
    }

    TAI_minus_UTC.enter("TAI - UTC at time of master-clock synchronization with UTC", new_prefix.str());
    time.add(TAI_minus_UTC);
    time.print("TAI date and hour of sight", new_prefix.str(), cout);

  }while(!check_data_time_interval(prefix));

  label.enter("label", new_prefix.str());

}

bool Sight::reduce(string prefix){

  stringstream new_prefix;
  bool check = true;
  
  new_prefix << prefix << "\t";
  
  compute_H_a(new_prefix.str());
  check &= get_coordinates(new_prefix.str());
  check &= compute_H_o(new_prefix.str());

  if(!check){
     cout << prefix << RED << "Sight cannot be reduced!\n" << RESET;
  }

  return check;
  
}


void Sight::compute_H_a(string prefix){
  
  if(artificial_horizon.value == 'y'){
    H_a = (H_s-index_error)/2.0;
    H_a.print("apparent altitude", prefix, cout);

  }else{
    compute_DH_dip(prefix);
    H_a = H_s-index_error+DH_dip;
    H_a.print("apparent altitude", prefix, cout);
  }
  
}


bool Sight::compute_H_o(string prefix){

  stringstream new_prefix;
  bool check = true;
  
  new_prefix << prefix << "\t";
 
  check &= compute_DH_refraction(new_prefix.str());

  if(check){
    compute_DH_parallax_and_limb(new_prefix.str());
    H_o = H_a + DH_refraction + DH_parallax_and_limb;
    H_o.print("observed altitude", new_prefix.str(), cout);
  }else{
     cout << prefix << RED << "H_o cannot be computed!\n" << RESET;
  }

  return check;
  
}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(string prefix){

  stringstream new_prefix;
  
  new_prefix << prefix << "\t";

  H_i = H_a + DH_refraction;
  H_i.print("intermediate altitude", prefix, cout);

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
 
	cout << new_prefix.str() << "Using " << gsl_root_fsolver_name(s) << " method\n";
	
	cout << new_prefix.str() << "iter" <<  "[lower" <<  ", upper] " <<  "root " <<  "err " <<  "err(est)\n";
	
	iter = 0;
	do{
      
	  iter++;
	  status = gsl_root_fsolver_iterate (s);
      
	  x = gsl_root_fsolver_root(s);
	  x_lo = gsl_root_fsolver_x_lower(s);
	  x_hi = gsl_root_fsolver_x_upper(s);
	  status = gsl_root_test_interval (x_lo, x_hi, 0.0, epsrel);
	  if(status == GSL_SUCCESS){
	    cout << new_prefix.str() << "Converged.\n";
	  }
	  cout << new_prefix.str() << iter << " [" << x_lo << ", " << x_hi << "] " << x << " " << x_hi-x_lo << "\n";
	  //printf("%5d [%.7f, %.7f] %.7f %+.7f\n", iter, x_lo, x_hi, x, x_hi - x_lo);
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

    DH_parallax_and_limb.print("parallax and limb correction", prefix, cout);

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
  
  ostr << new_prefix.str() << "Type = " << type << "\n";
  ostr << new_prefix.str() << "Name = " << name << "\n";
  
  if(type == "star"){
    RA.print("Right ascension", new_prefix.str(), ostr);
    d.print("Declination", new_prefix.str(), ostr);
  }else{
    radius.print("Radius", new_prefix.str(), ostr);
  }
 
}


void Body::enter(Catalog catalog, string prefix){

  unsigned int i;
  bool check;
  string s;
  stringstream new_prefix;

  new_prefix << prefix << "\t";

  
  do{

    s.clear();
    
    cout << prefix << "Enter name of body:";
    getline(cin >> ws, s);

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
void Sight::compute_DH_dip(string prefix){

  Length zero_Length;
  zero_Length.value = 0.0;

  DH_dip.set("Dip correction",
	     -acos( atmosphere.n(zero_Length)/atmosphere.n(height_of_eye)*((atmosphere.earth_radius.value)/((atmosphere.earth_radius.value)+(height_of_eye.value)) ) ), prefix);

}


bool Sight::compute_DH_refraction(string prefix){

  gsl_integration_workspace * w = gsl_integration_workspace_alloc (1000);
  gsl_function F;
  double result, error;
  int status;
  bool check = true;



  F.function = &dH_refraction;
  F.params = &(*this);

  
  /* cout << "Value = " << dH_refraction(1.0, &(*this)); */
  /* cin >> result; */

  

  status = gsl_integration_qags (&F, (atmosphere.h)[(atmosphere.h).size()-1], (atmosphere.h)[0], 0.0, epsrel, 1000, w, &result, &error);
  //status = GSL_FAILURE

  if(status == GSL_SUCCESS){
    DH_refraction.set("refraction correction", result, prefix);
  }else{
    check &= false;
    cout << prefix << RED << "GSL integration failed!\n" << RESET;
  }
  
  gsl_integration_workspace_free(w);
  
  return check;
  
}

void Length::set(string name, double x, string prefix){

  stringstream new_prefix;

  new_prefix << "\t" << prefix;    
  
  value = x;
  
  if(check_valid(name, new_prefix.str())){
    print(name, prefix, cout); 
  }
  
}

//enter a length in meters
void Length::enter(string name, string prefix){

  stringstream temp;

  temp.clear();
  temp << name << " [m]";

  do{
    
    enter_double(&value, false, 0.0, 0.0, temp.str(), prefix);
    
  }while(!check_valid(name, prefix));
    
  //convert to nautical miles
  value/=(1e3*nm);
    
  print(name, prefix, cout); 
  
}

void Length::print(string name, string prefix, ostream& ostr){

  ostr << prefix << name << " = " << value << " nm.\n";
 
}

bool Sight::get_coordinates(string prefix){

  File file;
  stringstream filename, line_ins, new_prefix;
  string line, dummy, temp;
  int l, l_min, l_max;
  double MJD_tab[(unsigned int)N], GHA_tab[(unsigned int)N], d_tab[(unsigned int)N], sum;
  gsl_interp_accel* acc = gsl_interp_accel_alloc ();
  gsl_spline *interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)), *interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));
  bool check = true;
 
  new_prefix << "\t" << prefix;    
  

  if((body.type) != "star"){
    filename << "data/" << body.name << ".txt";
  }else{
    filename << "data/j2000_to_itrf93.txt";
  }  
  temp = filename.str();

  
  file.set_name(temp.c_str()); 
  if(file.open("in", new_prefix.str())==1){

    /* cout << "\nMJD = " << t.MJD; */
    /* cout << "\nMJD0 = " << MJD_min; */
    /* cout << "\ndiff = " << (t.MJD)-MJD_min; */
    /* cin >>l ; */

    //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
    l_min = (int)(L*((time.MJD)-MJD_min))-(int)(N/2.0);
    //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
    l_max = (int)(L*((time.MJD)-MJD_min))+(int)(N/2.0);

    /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */

    //dummy read of file data
    for(l=0; l<l_min; l++){
      line.clear();
      getline((file.value), line);
    }


    if((body.type) != "star"){
      //in this case I am getting the coordinate of a body with a non-zero size

      //if the body is not a star

      double r_tab[(unsigned int)N];
      gsl_spline *interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));
     
      for(; l<l_max; l++){
	
	line.clear();
	line_ins.clear();
	
	getline((file.value), line);
	line_ins << line;
	cout << new_prefix.str() << line << "\n";
	line_ins >> dummy >> dummy >> dummy >> GHA_tab[l-l_min] >> d_tab[l-l_min] >> r_tab[l-l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;
	
	MJD_tab[l-l_min] = ((double)(l-l_min))/L;
	
      }

      file.close(new_prefix.str());

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

      if(gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;};
      if(gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;}
      if(gsl_spline_init(interpolation_r, MJD_tab, r_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;}

  
      cout << new_prefix.str() << "Read values:\n";
      for(l=0; l<N; l++){
	cout << new_prefix.str() << MJD_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
      }

      if(gsl_spline_eval_e(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.lambda).value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	(GP.lambda).normalize();
	(GP.lambda).print("GHA", new_prefix.str(), cout);
      }	
      //(GP.lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix.str());


      if(gsl_spline_eval_e(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.phi).value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	(GP.phi).normalize();
	(GP.phi).print("d", new_prefix.str(), cout);
      }	
      //(GP.phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix.str());

      if(gsl_spline_eval_e(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc, &(r.value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	if((r.check_valid("r", new_prefix.str()))){
	  r.print("r", new_prefix.str(), cout);
	}else{
	  check &= false; 
	}
      }
      //r.set("r", gsl_spline_eval(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix.str());

      gsl_spline_free(interpolation_r);


    }else{
          //in this case I am getting the coordinate of a body with a zero size (a star)

      //if the body is a star
      double phi3, phi2, phi1;

      for(; l<l_max; l++){
	
	line.clear();
	line_ins.clear();
	
	getline((file.value), line);
	line_ins << line;
	cout << new_prefix.str() << line << "\n";
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

      file.close(new_prefix.str());

 

      //remove discontinuous jumps in GHA to allow for interpolation
      for(sum=0.0, l=0; l<N-1; l++){
	//cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
	if(((GHA_tab[l]-sum) > M_PI) && (GHA_tab[l+1] < M_PI)){
	  sum += 2.0*M_PI;
	}
	GHA_tab[l+1] += sum;
      }

      cout << new_prefix.str() << "Read values:\n";
      for(l=0; l<N; l++){
	cout << new_prefix.str() << MJD_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
      }

      if(gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;}
      if(gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;}

      
      if(gsl_spline_eval_e(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.lambda).value)) != GSL_SUCCESS){
	check &= false;
      }else{
	(GP.lambda).normalize();
	(GP.lambda).print("GHA", new_prefix.str(), cout);
      }

      if(gsl_spline_eval_e(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.phi).value)) != GSL_SUCCESS){
	check &= false;
      }else{
	(GP.phi).normalize();
	(GP.phi).print("d", new_prefix.str(), cout);
      }

    }

  }else{
    check &= false;
  }

  if(!check){
     cout << prefix << RED << "Cannot obtain coordinates!\n" << RESET;
  }
  
  gsl_interp_accel_free(acc);
  gsl_spline_free(interpolation_GHA);
  gsl_spline_free(interpolation_d);

  return check;
  
}

void Angle::set(string name, double x, string prefix){

  value = x;
  normalize();
  print(name, prefix, cout);
  
}

void Angle::enter(string name, string prefix){

  string s;
  stringstream new_prefix;
  unsigned int ad;
  double am;
  bool check;

  new_prefix << "\t" << prefix;    

  cout << prefix << "Enter " << name << " [s ddd mm.m]:\n";

  do{
    
    s.clear();

    cout << prefix << "\tEnter s: ";
    cin >> s;
    
    if((s=="+") || (s=="-")){check = true;}
    else{
      cout << prefix << RED << "\tEntered value is not valid!\n" << RESET;
      check = false;
    }
  }while(!check);



  enter_unsigned_int(&ad, true, 0, 360, "ddd", new_prefix.str());
  enter_double(&am, true, 0.0, 60.0, "mm.m", new_prefix.str());
 
  value = k*(((double)ad) + am/60.0);
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
  ostr << prefix << name << " = " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'\n";

}

void Limb::enter(string name, string prefix){

  bool check;

  do{
    cout << prefix << "Enter " << name << " [u/l/c]:";
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

  ostr << prefix << name << " = " << value << "\n";
  
}




stringstream Chrono::to_string(unsigned int precision){

  stringstream output;
  
  output.precision(precision);
  
  if(h<10){output << 0;}
  output << h << ":";
  if(m<10){output << 0;}
  output << m << ":";
  if(s<10.0){output << 0;}
  output << s;

  return output;
  
}

void Date::print(string name, string prefix, ostream& ostr){

  ostr << prefix << "date of " << name << " = " << to_string().str().c_str() << "\n";

};

void Date::enter(string name, string prefix) {

  stringstream new_prefix;
  string s;

  //append \t to prefix
  new_prefix << prefix << "\t";
  

  cout << prefix << "Enter " << name << " [YYYY-MM-DD]\n";

  enter_unsigned_int(&Y, false, 0, 0, "YYYY", prefix);
 
  check_leap_year();
  if((Y_is_leap_year)){
    (days_per_month) = days_per_month_leap;
    cout << new_prefix.str() << "YYYY is a leap year\n";
  }else{
    (days_per_month) = days_per_month_common;
    cout << new_prefix.str() << "YYYY is a common year\n";
  }

  enter_unsigned_int(&M, true, 1, 12+1, "MM", prefix);
  
  enter_unsigned_int(&D, true, 1, days_per_month[M-1]+1, "DD", prefix);

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

  unsigned int precision;

  //if I am printing to terminal, I print with display_precision. Otherwise, I print with data_precision
  if(ostr.rdbuf() == cout.rdbuf()){
    precision = display_precision;
  }else{
    precision = data_precision;
  }
  
  ostr << prefix << "hour of " << name << " = " << to_string(precision).str().c_str() << "\n";

};
void Chrono::enter(string name, string prefix) {

  stringstream new_prefix;

  //append \t to prefix
  new_prefix << prefix << "\t";
  
  cout << prefix << "Enter " << name << " [hh-mm-ss]\n";

  enter_unsigned_int(&h, true, 0, 24, "hh", new_prefix.str());
  enter_unsigned_int(&m, true, 0, 60, "mm", new_prefix.str());
  enter_double(&s, true, 0.0, 60.0, "ss.s", new_prefix.str());
 
}



stringstream Time::to_string(unsigned int precision){

  stringstream output;

  output << date.to_string().str() << " " << chrono.to_string(precision).str();
 
  return output;
  
}


void Time::print(string name, string prefix, ostream& ostr){

  stringstream new_prefix;

  //append \t to prefix
  new_prefix << prefix << "\t";
  
  ostr << prefix << name << ":\n";

  date.print(name, new_prefix.str(), ostr);
  chrono.print(name, new_prefix.str(), ostr);
  
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


