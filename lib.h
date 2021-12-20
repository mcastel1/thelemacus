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
//earth radius in nautical miles
#define Re (60.0*360.0/(2.0*M_PI))
#define RED     "\033[1;31m"    
#define YELLOW     "\033[1;33m"   
#define CYAN "\033[1;36m"      
#define RESET   "\033[0m"
#define BOLD     "\033[1m"   
//all possible chars that can enter in an unsigned integer
#define chars_unsigned_int "0123456789"
//all possible chars that can enter in a non-negative double
#define chars_double "0123456789."
#define days_per_month_leap {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
#define days_per_month_common {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
#define path_file_init "data/init.txt"
#define path_file_catalog "data/catalog.txt"
//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal

inline double cot(double x){

  return 1.0/tan(x);

}

inline double csc(double x){

  return 1.0/sin(x);
  
}

inline double atan(double x, double y){

  if(x > 0.0){
    
    return atan(y/x);

  }else{

    return(atan(y/x)+M_PI);
  }

}

class Catalog;
class File;
class Time;
class Date;
class Chrono;
class Route;
class Sight;
class Atmosphere;

class String{

 public:
  string value;

  String();
  String(string);
  void enter(String, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, bool, String);
  void set(String, String, String);
  String append(String);
  String prepend(String);

  bool operator==(const String&);

};

bool String::operator==(const String& s){

  return((((*this).value) == (s.value)));
  
}





class Int{

 public:
  int value;

  void read_from_file(String, File&, bool, String);
  void print(String, String, ostream&);

};


class Double{

 public:
  double value;

  void read_from_file(String, File&, bool, String);
  void print(String, String, ostream&);

};

class File{

 public:
  fstream value;
  String name;
  unsigned int number_of_lines;

  File();
  void set_name(String);
  void enter_name(String);
  bool open(String, String);
  void close(String);
  void remove(String);
  void count_lines(String);
  bool check_if_exists(String);
  
};

void File::enter_name(String prefix){

  name.enter(String("name of file"), prefix);
  
}

void File::count_lines(String prefix){

  stringstream command, line_ins;
  string line, dummy;
  File file_number_of_lines;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));


  file_number_of_lines.set_name(String("output.out"));
  file_number_of_lines.remove(new_prefix);
  
  command.str("");
  command << "wc -l " << (name.value)  << " >> " << ((file_number_of_lines.name).value);
  system(command.str().c_str());

  file_number_of_lines.open(String("in"), new_prefix);
  
  getline((file_number_of_lines.value), line); 
  line_ins << line;
  line_ins >> number_of_lines >> dummy;

  file_number_of_lines.close(new_prefix);  

  cout << prefix.value << "Number of lines in file " << (name.value) << " = " << number_of_lines << "\n";

  
}


void File::remove(String prefix){

  stringstream command;

  command.str("");
  command << "rm -rf " << (name.value) << "> /dev/null 2>&1";
  system(command.str().c_str());

  cout << prefix.value << "File " << name.value << " removed\n";
  
}


File::File(){

  value.precision(data_precision);
  
}

void File::set_name(String filename){

  (name.value) = filename.value;
  
}

bool File::check_if_exists(String prefix){

  bool output;

  value.open(name.value, ios::in);
  
  if(value){
    cout << prefix.value << "File " << (name.value) << " exists\n";
    value.close();
    output = true;   
  }
  else{
    cout << prefix.value << "File " << (name.value) << " does not exist\n";
    output = false;
  }

  return output;

}

bool File::open(String mode, String prefix){


  if(mode.value == "in"){
    value.open(name.value, ios::in);
  }else{
    value.open(name.value, ios::out);
  }
  
  cout << prefix.value << "Opening " << (name.value) << " in mode '" << mode.value << "' ... \n";
  
  if(!value){
    
    cout << prefix.value << RED << "... error opening file " << (name.value) << "!\n" << RESET;
    return 0;
    
  }else{
    
    cout << prefix.value <<  "... done.\n";
    return 1;
     
  }

}

void File::close(String prefix){
  
  value.close();
  cout << prefix.value << "File " << (name.value) << " closed.\n";
     

}


void Double::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  size_t pos;

  if(search_entire_file){
    
    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!
  
    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));


  }else{

    line.clear();
    getline(file.value, line);
    
  }

  
  pos = line.find(" = ");

  //read the string after ' = ' until the end of line string and store it into value
  value = stod(line.substr(pos+3, line.size() - (pos+3)).c_str(), NULL);

  print(name, prefix, cout);
  
}

void Double::print(String name, String prefix, ostream& ostr){

  ostr << prefix.value << name.value << " = " << value << "\n";
 
}




void Int::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  size_t pos;

  if(search_entire_file){
    
    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!
  
    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));


  }else{

    line.clear();
    getline(file.value, line);
    
  }

  
  pos = line.find(" = ");

  //read the string after ' = ' until the end of line string and store it into value
  value = stoi(line.substr(pos+3, line.size() - (pos+3)).c_str(), NULL);

  print(name, prefix, cout);

  
}


void Int::print(String name, String prefix, ostream& ostr){

  ostr << prefix.value << name.value << " = " << value << "\n";
 
}


bool get_date_hour(String &line, String prefix){

  File file;
  stringstream command;
  bool check;

  check = true;

  command.str("");
  command << "rm -rf output.out; date \"+%Y-%m-%d %H:%M:%S\" >> output.out";
  system(command.str().c_str());

  ((file.name).value) = "output.out";
  check &= file.open(String("in"), prefix);
  line.value.clear();
  getline(file.value, line.value);
  file.close(prefix);

  command.str("");
  command << "rm -rf output.out";
  system(command.str().c_str());

  if(!check){
    cout << prefix.value << RED << "\tI could not get hour and date!\n" << RESET;
  }
    
  return check;

}

//this function checks whether the  unsigned int in string s is formatted correctly and, if check_interval = true, it also checks whether the this unsigned int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_unsigned_int(string s, unsigned int* i, bool check_interval, unsigned int min, unsigned int sup){

  bool check;
  unsigned int j;
  
  if(/*here I check whether the quantity entered in s is an unsigned integer, i.e., it contains only the characters 0123456789*/ ((s.find_first_not_of(chars_unsigned_int)) == (std::string::npos))){
    
    j = stoi(s, NULL, 10);
      
    if(check_interval){

      check = ((j >= min) && (j < sup));
		
    }else{
      
      check = true;
    }
      
  }else{
    
    check = false;
    
  }
  
  if((i != NULL) && check){

    (*i) = j;
    
  }

  return check;
  
}

  
//this function asks the user to enter an unsigned int from keyboard and checks whether the entered value is an unsigned int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_unsigned_int(unsigned int* i, bool check_interval, unsigned int min, unsigned int sup, String name, String prefix){

  string s;
  bool check;

  do{
    
    s.clear();

    cout << prefix.value << "Enter " << name.value << ":";
    getline(cin >> ws, s);

    check = check_unsigned_int(s, i, check_interval, min, sup);
    
    if(!check){
      
      cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
      
    }
    
  }while(!check);

}


//this function checks whether the double in string s is formatted correctly and, if check_interval = true, it also checks whether the this double lies in [min, sup). If x != NULL and the check is ok, it also writes the value of the double read from s into (*x)
bool check_double(string s, double* x, bool check_interval, double min, double sup){

  bool check;
  double y;

  if((/*here I check whether the quantity in s contains the allowed chars for double, i.e., it contains only the characters 0123456789.*/ ((s.find_first_not_of(chars_double)) == (std::string::npos))) && /*here I count whether the dot occurs zero or one time*/(count(s.begin(), s.end(), '.') <= 1)){

    y = stod(s);
      
    if(check_interval){
	
      check = ((y >= min) && (y < sup));
	
    }else{
      
      check = true;
      
    }
      
  }else{
    
    check = false;
    
  }

  if((x != NULL) && check){

    (*x) = y;
    
  }

  return check;
 
}


//this function asks the user to enter a double from keyboard and checks whether the entered value contains the allowed chars for double and, if check_interval = true, that the entered value lies in [min, sup)
void enter_double(double* x, bool check_interval, double min, double sup, String name, String prefix){

  string s;
  bool check;

  do{
    
    s.clear();

    cout << prefix.value << "Enter " << name.value << ":";
    getline(cin >> ws, s);

    check = check_double(s, x, check_interval, min, sup);
    
    if(!check){
      
      cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
      
    }
    
  }while(!check);

}




class Length{

 public:
  double value;
  
  void set(String, double, String);
  void enter(String, String, String);
  void print(String, String, String, ostream&);
  void read_from_file(String, File&, bool, String);
  bool check_valid(String, String);
  bool operator> (const Length&);
  Length operator + (const Length&);

};

//evaluates whether Length (*this) is larger than r
bool Length::operator>(const Length& r){

  return((((*this).value) > (r.value)));
  
}

Length Length::operator+ (const Length& l){
  
  Length s;

  (s.value) = value + (l.value);

  return s;
  
}


class Speed{

 public:
  double value;
  void enter(String, String);
  bool check_valid(String, String);
  void print(String, String, ostream&);


};

bool Speed::check_valid(String name, String prefix){

  bool check = true;
  
  if(value<0.0){
    check &= false;
    cout << prefix.value << RED << "Entered value of " << name.value << " is not valid!\n" << RESET;
  }

  return check;
  
}

void Speed::print(String name, String prefix, ostream& ostr){

  ostr << prefix.value << name.value << " = ";
  ostr << value << " kt\n";
 
}



//enter a speed in knots
void Speed::enter(String name, String prefix){

  do{
    
    enter_double(&value, false, 0.0, 0.0, name, prefix);
    
  }while(!check_valid(name, prefix));

  
  print(name, prefix, cout); 
  
}



String::String(){

  value = "";
  
}


String::String(string s){

  value = s;
  
}



void String::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  size_t pos;

  if(search_entire_file){
    
    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!
  
    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));


  }else{

    line.clear();
    getline(file.value, line);
    
  }

  
  pos = line.find(" = ");

  //read the string after ' = ' until the end of line string and store it into value
  value = line.substr(pos+3, line.size() - (pos+3)).c_str();

  print(name, prefix, cout);

  
}


class Answer{

 public:
  char value;
  void enter(String, String);
  bool set(String, char, String);
  //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
  void print(String, String, ostream&);
  void read_from_file(String, File&, bool, String);

};

bool Answer::set(String name, char c, String prefix){

  bool check;

  check = true;
  check &= ((c == 'y') || (c == 'n'));
  
  if(check){

    value = c;
    
    if(!(name == String(""))){
      
      cout << prefix.value << name.value << " = " << c << "\n";
      
    }

  }else{

    cout << prefix.value << RED << "Value of answer is not valid!\n" << RESET;
    
  }

  return check;
  
}


void Answer::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  size_t pos;

  if(search_entire_file){
    
    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!
  
    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));


  }else{

    line.clear();
    getline(file.value, line);
    
  }


  
  pos = line.find(" = ");

  value = line[pos+3];

  print(name, prefix, cout);

  
}


void String::enter(String name, String prefix){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter " << name.value << ":";
  getline(cin, value);

  if(value.empty()){
    get_date_hour((*this), new_prefix);
    cout << prefix.value << YELLOW << "Entered an empty " << name.value << ", setting it to " << value << "\n" << RESET;
  }

  print(name, prefix, cout);

}

void String::print(String name, String prefix, ostream& ostr){

  if(value != ""){ostr << prefix.value << name.value << " = " << value << "\n";}
  
}

void String::set(String name, String input_string, String prefix){

  value = (input_string.value);
  if(!(name == String(""))){print(name, prefix, cout);}

}



class Angle{

 public:
 
  double value;
  void normalize(void);
  void enter(String, bool, String);
  void set(String, double, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, bool, String);
  stringstream to_string(unsigned int);

  bool operator==(const Angle&);
  Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
  
};

bool Angle::operator==(const Angle& x){

  return((((*this).value) == (x.value)));
  
}



//I added the booleian variable search_entire_file. If true, then this function rewinds the file pointer to the beginning of file and goes through the file until it finds the quantity 'name'. If false, it reads the angle at the position where 'file' was when it was passed to this function 
void Angle::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  size_t pos1, pos2, pos3;

  if(search_entire_file){
    
    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!
  
    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));


  }else{

    line.clear();
    getline(file.value, line);
    
  }

  pos1 = line.find(" = ");
  pos2 = line.find("° ");
  pos3 = line.find("'");
  
  value = k*(stod(line.substr(pos1+3, pos2 - (pos1+3)).c_str()) + stod(line.substr(pos2+2, pos3 - (pos2+2)))/60.0);

  print(name, prefix, cout);
  
}




class Position{

 public:
  //latitude and longitude of the position
  Angle phi, lambda;
  //label to add a note about the position
  String label;
  //this is a list of the items which are part of a Position object (phi, lambda, ..)
  vector<String> items;

  Position();
  void enter(String, String);
  void modify(String);
  void print(String, String, ostream&);
  void read_from_file(File&, String);
  //this function transports the position and returns the Route with which it has been transported
  Route transport(String);
  stringstream to_string(unsigned int);
  bool distance(Position, Length*, String, String);
  bool operator==(const Position&);


};

//evaluates whether Length (*this) is larger than r
bool Position::operator==(const Position& p){

  bool check;
  
  check = true;
  
  check &= (((*this).phi) == (p.phi));
  check &= (((*this).lambda) == (p.lambda));
  check &= (((*this).label) == (p.label));

  return check;
  
}


Position::Position(void){

  items = {String("latitude"), String("longitude"), String("label")};

}

//here name is the name of the distance that I am computing; for example 'distance between positions A and B'
bool Position::distance(Position p, Length* l, String name, String prefix){

  bool check;
  check = true;

  ((*l).value) = Re * acos( cos((lambda.value) - (p.lambda.value))*cos((phi.value))*cos((p.phi.value)) + sin((phi.value))*sin((p.phi.value)) );

  check &= !isnan((*l).value);

  if(check){

    Angle a;
    a.set(String(""), ((*l).value)/Re, prefix);
    (*l).print(name, String("nm"), prefix, cout);

  }else{

    cout << prefix.value << RED << "\tI could not compute distance!\n" << RESET;
    
  }

  return check;
  
}

void Position::modify(String prefix){

  unsigned int i;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  cout << prefix.value << "Enter the item that you want to modify:\n";

  for(i=0; i<items.size(); i++){
    cout << prefix.value << "\t(" << i+1 << ") " << (items[i]).value << "\n";
  }

  enter_unsigned_int(&i, true, 1, items.size()+1, String("choice #"), prefix);

  switch(i){

  case 1:{
    
    phi.enter(String("latitude"), false, new_prefix);

  }
    break;


  case 2:{
    
    lambda.enter(String("longitude"), false, new_prefix);

  }
    break;

    
  case 3:{
    
    label.enter(String("label"), new_prefix);

  }
    break;

  }

  cout << prefix.value << "Position modified\n";
  
}

//if type = l or o, the parameters specifying the route are start, alpha, l. if type = c, the parameters specifying the route are GP and omega. 
class Route{

 public:
  String type, label, temp_prefix;
  //starting position of the route
  Position start, end, GP;
  //alpha: the angle that the vector tangent to the route describes with the local meridian at start; omega: the aperture angle of the cone for circles of equal altitude
  Angle alpha, omega;
  //the length of the route
  Length l;
  Speed sog;
  //this is the position in sight_list of the sight linked to route. If there is no sight linked to route, then related_sight = -1. 
  int related_sight;

  void enter(String, String);
  void print(String, String, ostream&);
  void read_from_file(File&, String);
  void compute_end(String);
  bool crossing(Route, vector<Position>*, double*, String);
  void transport(String);
  static double lambda_minus_pi(double, void*);
  bool t_crossing(Route, vector<Angle>*, String);

  
};

//returns in t the two values of the parametric angles t for circle of equal altitude (*this), at which (*this) crosses Route r
bool Route::t_crossing(Route route, vector<Angle> *t, String prefix){

  String new_prefix;
  bool check = true;
  Angle t_a, t_b;
  
  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  check = true;
  
  t_a.value = atan((8*cos((route.GP.phi.value))*((cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(((*this).GP.phi.value)) - cos(((*this).GP.phi.value))*sin((route.GP.phi.value)))*(cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*cot(((*this).omega.value)) - cos((route.omega.value))*csc(((*this).omega.value)) + cot(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
						 abs(sin(((*this).GP.lambda.value) - (route.GP.lambda.value)))*cos((route.GP.phi.value))*sqrt(-(gsl_sf_pow_int(cos((route.omega.value)),2)*gsl_sf_pow_int(csc(((*this).omega.value)),2)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2) + 
																	      2*cos((route.omega.value))*cot(((*this).omega.value))*csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value)) - gsl_sf_pow_int(cot(((*this).omega.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
																	      2*cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*csc(((*this).omega.value))*(cos((route.omega.value))*cot(((*this).omega.value)) - csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
																	      gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*(-(gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value) - (route.GP.lambda.value)),2)*gsl_sf_pow_int(cot(((*this).omega.value)),2)) + gsl_sf_pow_int(sin((route.GP.phi.value)),2)) + 
																	      gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2) - 2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + 
																	      2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2) + 
																	      gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2))))/
		   (gsl_sf_pow_int(cos((route.GP.phi.value)),2)*(-6 + 2*cos(2*((*this).GP.phi.value)) + 2*cos(2*((*this).GP.lambda.value) - 2*(route.GP.lambda.value)) + cos(2*(((*this).GP.phi.value) + ((*this).GP.lambda.value) - (route.GP.lambda.value))) + cos(2*(((*this).GP.phi.value) - ((*this).GP.lambda.value) + (route.GP.lambda.value)))) - 8*gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
		    4*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(2*((*this).GP.phi.value))*sin(2*(route.GP.phi.value))),(8*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*(cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*cot(((*this).omega.value)) - cos((route.omega.value))*csc(((*this).omega.value)) + cot(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value)))*
																	   sin(((*this).GP.lambda.value) - (route.GP.lambda.value)) - 8*abs(sin(((*this).GP.lambda.value) - (route.GP.lambda.value)))*cos((route.GP.phi.value))*csc(((*this).GP.lambda.value) - (route.GP.lambda.value))*(cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(((*this).GP.phi.value)) - cos(((*this).GP.phi.value))*sin((route.GP.phi.value)))*
																	   sqrt(-(gsl_sf_pow_int(cos((route.omega.value)),2)*gsl_sf_pow_int(csc(((*this).omega.value)),2)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2) + 2*cos((route.omega.value))*cot(((*this).omega.value))*csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value)) - 
																		gsl_sf_pow_int(cot(((*this).omega.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 2*cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*csc(((*this).omega.value))*(cos((route.omega.value))*cot(((*this).omega.value)) - csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
																		gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*(-(gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value) - (route.GP.lambda.value)),2)*gsl_sf_pow_int(cot(((*this).omega.value)),2)) + gsl_sf_pow_int(sin((route.GP.phi.value)),2)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2) - 
																		2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + 2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + 
																		gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2)))/
		   (gsl_sf_pow_int(cos((route.GP.phi.value)),2)*(-6 + 2*cos(2*((*this).GP.phi.value)) + 2*cos(2*((*this).GP.lambda.value) - 2*(route.GP.lambda.value)) + cos(2*(((*this).GP.phi.value) + ((*this).GP.lambda.value) - (route.GP.lambda.value))) + cos(2*(((*this).GP.phi.value) - ((*this).GP.lambda.value) + (route.GP.lambda.value)))) - 8*gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
		    4*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(2*((*this).GP.phi.value))*sin(2*(route.GP.phi.value))));

  check &= (!isnan(t_a.value));

      
  t_b.value = atan((-8*cos((route.GP.phi.value))*((-(cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(((*this).GP.phi.value))) + cos(((*this).GP.phi.value))*sin((route.GP.phi.value)))*(cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*cot(((*this).omega.value)) - cos((route.omega.value))*csc(((*this).omega.value)) + cot(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
						  abs(sin(((*this).GP.lambda.value) - (route.GP.lambda.value)))*cos((route.GP.phi.value))*sqrt(-(gsl_sf_pow_int(cos((route.omega.value)),2)*gsl_sf_pow_int(csc(((*this).omega.value)),2)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2) + 
																	       2*cos((route.omega.value))*cot(((*this).omega.value))*csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value)) - gsl_sf_pow_int(cot(((*this).omega.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
																	       2*cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*csc(((*this).omega.value))*(cos((route.omega.value))*cot(((*this).omega.value)) - csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
																	       gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*(-(gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value) - (route.GP.lambda.value)),2)*gsl_sf_pow_int(cot(((*this).omega.value)),2)) + gsl_sf_pow_int(sin((route.GP.phi.value)),2)) + 
																	       gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2) - 2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + 
																	       2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2) + 
																	       gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2))))/
		   (gsl_sf_pow_int(cos((route.GP.phi.value)),2)*(-6 + 2*cos(2*((*this).GP.phi.value)) + 2*cos(2*((*this).GP.lambda.value) - 2*(route.GP.lambda.value)) + cos(2*(((*this).GP.phi.value) + ((*this).GP.lambda.value) - (route.GP.lambda.value))) + cos(2*(((*this).GP.phi.value) - ((*this).GP.lambda.value) + (route.GP.lambda.value)))) - 8*gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
		    4*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(2*((*this).GP.phi.value))*sin(2*(route.GP.phi.value))),(8*cos((route.GP.phi.value))*sin(((*this).GP.lambda.value) - (route.GP.lambda.value))*
																	   (cos((route.GP.phi.value))*(cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*cot(((*this).omega.value)) - cos((route.omega.value))*csc(((*this).omega.value)) + cot(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
																	    abs(sin(((*this).GP.lambda.value) - (route.GP.lambda.value)))*gsl_sf_pow_int(csc(((*this).GP.lambda.value) - (route.GP.lambda.value)),2)*(cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(((*this).GP.phi.value)) - cos(((*this).GP.phi.value))*sin((route.GP.phi.value)))*
																	    sqrt(-(gsl_sf_pow_int(cos((route.omega.value)),2)*gsl_sf_pow_int(csc(((*this).omega.value)),2)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2) + 
																		 2*cos((route.omega.value))*cot(((*this).omega.value))*csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value)) - gsl_sf_pow_int(cot(((*this).omega.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
																		 2*cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*csc(((*this).omega.value))*(cos((route.omega.value))*cot(((*this).omega.value)) - csc(((*this).omega.value))*sin(((*this).GP.phi.value))*sin((route.GP.phi.value))) + 
																		 gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*(-(gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value) - (route.GP.lambda.value)),2)*gsl_sf_pow_int(cot(((*this).omega.value)),2)) + gsl_sf_pow_int(sin((route.GP.phi.value)),2)) + 
																		 gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos((route.GP.lambda.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2) - 2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + 
																		 2*gsl_sf_pow_int(cos((route.GP.phi.value)),2)*cos(((*this).GP.lambda.value))*cos((route.GP.lambda.value))*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*sin(((*this).GP.lambda.value))*sin((route.GP.lambda.value)) + gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(cos(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2) + 
																		 gsl_sf_pow_int(cos((route.GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin(((*this).GP.lambda.value)),2)*gsl_sf_pow_int(sin((route.GP.lambda.value)),2))))/
		   (gsl_sf_pow_int(cos((route.GP.phi.value)),2)*(-6 + 2*cos(2*((*this).GP.phi.value)) + 2*cos(2*((*this).GP.lambda.value) - 2*(route.GP.lambda.value)) + cos(2*(((*this).GP.phi.value) + ((*this).GP.lambda.value) - (route.GP.lambda.value))) + cos(2*(((*this).GP.phi.value) - ((*this).GP.lambda.value) + (route.GP.lambda.value)))) - 8*gsl_sf_pow_int(cos(((*this).GP.phi.value)),2)*gsl_sf_pow_int(sin((route.GP.phi.value)),2) + 
		    4*cos(((*this).GP.lambda.value) - (route.GP.lambda.value))*sin(2*((*this).GP.phi.value))*sin(2*(route.GP.phi.value))));

  check &= (!isnan(t_b.value));

  if(check){

    ((*t)[0]).normalize();
    ((*t)[0]).set(String(""), (t_a.value), new_prefix);
    ((*t)[1]).normalize();
    ((*t)[1]).set(String(""), (t_b.value), new_prefix);

  }else{

    cout << new_prefix.value << RED << "\tValues of intersections are not valid!\n" << RESET;
	
  }
      
  return check;
      
}

void Route::read_from_file(File& file, String prefix){

  String new_prefix;
  string line;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  type.read_from_file(String("type"), file, false, new_prefix);

  line.clear();
  getline(file.value, line);

  
  if((type.value)[0] == 'c'){

    GP.read_from_file(file, new_prefix);    
    omega.read_from_file(String("omega"), file, false, new_prefix);
    
  }else{
    
    start.read_from_file(file, new_prefix);

    alpha.read_from_file(String("starting heading"), file, false, new_prefix);
    l.read_from_file(String("length"), file, false, new_prefix);

  }
  
  label.read_from_file(String("label"), file, false, new_prefix);
  //when a sight is read from file, it is not yet linked to any route, thus I set
  related_sight = -1;

}
 

//this function computes the crossings between Route (*this) and Route route: it writes the two crossing points in p, and the cosing of the crossing angle in cos_crossing_angle
bool Route::crossing(Route route, vector<Position>* p, double* cos_crossing_angle, String prefix){

  //these are the two lengths along Route (*this) which correspond to the two crossings with route
  String new_prefix;
  bool output;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));




  if(!(((*this).type.value == "c") && ((*this).type.value == "c"))){
    
    cout << prefix.value << "Routes are not circles of equal altitude: this code only computes intersects between circles of equal altitudes\n";
    output = false; 
    
  }else{

    Angle theta, t_temp;
    Length r, s;
    
    theta.set(String("angle between the two GPs"), acos(cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value)) + sin(((*this).GP.phi.value))*sin((route.GP.phi.value))), prefix);

    if((abs(((*this).omega.value)-(route.omega.value)) < (theta.value)) && ((theta.value) < ((*this).omega.value)+(route.omega.value))){

      //t contains the parametric angle of Route (*this) where (*this) crosses route
      //u contains the parametric angle of Route route where route crosses (*this)
      vector<Angle> t(2), u(2);

      cout << prefix.value << "Routes intersect\n";

      (*this).t_crossing(route, &t, new_prefix);
      route.t_crossing((*this), &u, new_prefix);

      ((*this).l).set(String(""), Re * sin((*this).omega.value) * ((t[0]).value), prefix);
      (*this).compute_end(new_prefix);
      (*p)[0] = ((*this).end);
      ((*p)[0]).label.set(String(""), String("crossing"), prefix);

      ((*this).l).set(String(""), Re * sin((*this).omega.value) * ((t[1]).value), prefix);
      (*this).compute_end(new_prefix);
      (*p)[1] = ((*this).end);
      ((*p)[1]).label.set(String(""), String("crossing"), prefix);



      //

      (route.l).set(String("l of intersection"), Re * sin(route.omega.value) * ((u[0]).value), prefix);
      route.compute_end(prefix);
      
      ((*p)[0]).distance(route.end, &r, String(""), prefix);
      ((*p)[1]).distance(route.end, &s, String(""), prefix);

      if(r>s){

	cout << new_prefix.value << "Exchanging ts!\n";
	
	t_temp = u[0];
	u[0] = u[1];
	u[1] = t_temp;
	
      }

      ((*this).l).set(String("l of intersection 1 for Route 1"), Re * sin((*this).omega.value) * ((t[0]).value), prefix);
      (*this).compute_end(prefix);
      ((*this).end).print(String("position of intersection 1 for Route 1"), prefix, cout);
      
      (route.l).set(String("l of intersection 1 for Route 2"), Re * sin(route.omega.value) * ((u[0]).value), prefix);
      route.compute_end(prefix);
      (route.end).print(String("position of intersection 1 for Route 2"), prefix, cout);

      (*cos_crossing_angle) = cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*sin(((t[0]).value))*sin(((u[0]).value)) + (cos(((t[0]).value))*sin(((*this).GP.lambda.value)) - cos(((*this).GP.lambda.value))*sin(((*this).GP.phi.value))*sin(((t[0]).value)))*(cos(((u[0]).value))*sin((route.GP.lambda.value)) - cos((route.GP.lambda.value))*sin((route.GP.phi.value))*sin(((u[0]).value))) + 
	(cos(((*this).GP.lambda.value))*cos(((t[0]).value)) + sin(((*this).GP.phi.value))*sin(((*this).GP.lambda.value))*sin(((t[0]).value)))*(cos((route.GP.lambda.value))*cos(((u[0]).value)) + sin((route.GP.phi.value))*sin((route.GP.lambda.value))*sin(((u[0]).value)));
      
      cout << prefix.value << "cos(angle 1 between tangents) = " << (*cos_crossing_angle)  << "\n";
      
      //
	
 
      output = true;

      t.clear();
      u.clear();
      
    }else{

      cout << prefix.value << "Routes do no intersect\n";
      output = false;
      
    }
    
  }

  return output;

}

String String::append(String s){

  String output;
  stringstream temp;
  
  //append \t to prefix
  temp << value << s.value;

  output.value = temp.str();
  
  return output;
  
}

String String::prepend(String s){

  String output;
  stringstream temp;
  
  //append \t to prefix
  temp << s.value << value;

  output.value = temp.str();
  
  return output;
  
}


class Date{

 public:
  unsigned int Y, M, D;
  bool Y_is_leap_year;
  vector<unsigned int> days_per_month;

  void print(String, String, ostream&);
  void enter(String, String);
  bool read_from_file(String, File&, bool, String);
  stringstream to_string(void);
  void check_leap_year(void);

};

class Chrono{

 public:
  unsigned int h, m;
  double s;

  void print(String, String, ostream&);
  void enter(String, String);
  bool read_from_file(String, File&, bool, String);
  stringstream to_string(unsigned int);

};


class Time{

 public:
  Date date;
  Chrono chrono;
  //is s used?
  double s, MJD;
  void enter(String, String);
  void print(String, String, ostream&);
  bool read_from_file(String, File&, String);
  
  void to_MJD(void);
  void to_TAI(void);
  void add(Chrono);
  
  stringstream to_string(unsigned int);
  bool operator> (const Time&);
  
};

//evaluates whether Time (*this) is larger than t
bool Time::operator>(const Time& t){

  Time s;
  
  (*this).to_MJD();
  s = t;
  s.to_MJD();

  return((((*this).MJD) > (s.MJD)));
  
}

stringstream Time::to_string(unsigned int precision){

  stringstream output;

  output << date.to_string().str() << " " << chrono.to_string(precision).str();
 
  return output;
  
}

stringstream Position::to_string(unsigned int precision){

  stringstream output;

  output << phi.to_string(precision).str() << " " << lambda.to_string(precision).str();
 
  return output;
  
}


void Time::print(String name, String prefix, ostream& ostr){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  ostr << prefix.value << name.value << ":\n";

  date.print(name, new_prefix, ostr);
  chrono.print(name, new_prefix, ostr);
  
};



void Time::enter(String name, String prefix) {

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  cout << prefix.value << "Enter " << name.value << " date and hour\n";
  
  date.enter(String("date"), new_prefix);
  chrono.enter(String("hour"), new_prefix);
  
  to_MJD();
  print(name, prefix, cout);

}

void Time::to_TAI(void){
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



Route Position::transport(String prefix){

  Route route;
  stringstream temp_label;
  bool check;
  String new_prefix;
  Time t_start, t_end;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter route:\n";

  // route is not related to any sight, so I set
  (route.related_sight) = -1;
  
  do{
    route.type.enter(String("type [l(=loxodrome)/o(=orthodrome)]"), new_prefix);
    check = ((route.type.value == "l") || (route.type.value == "o"));
    if(!check){
      cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
    }
  }while(!check);
  route.start = (*this); 
  route.alpha.enter(String("Course Over Ground"), true, new_prefix);

  do{
    
    t_start.enter(String("start time of course"), new_prefix);
    t_end.enter(String("end time of course"), new_prefix);
    if(t_start > t_end){
      cout << new_prefix.value << RED << "start time of course is larger than end time of course!\n" << RESET; 
    }
    
  }while(t_start > t_end);
  
  (route.sog).enter(String("Speed Over Ground [kt]"), new_prefix);
  
  t_start.to_MJD();
  t_end.to_MJD();

  (route.l).set(String("Length"), (route.sog).value*((t_end.MJD)-(t_start.MJD))*24.0, new_prefix); 
 
  route.print(String("transport"), prefix, cout);
  
  route.compute_end(new_prefix);

  temp_label << label.value << "tr. w. " << route.type.value << ", COG = " << route.alpha.to_string(display_precision).str().c_str() << ", l = " << (route.l).value << " nm";
  (route.end.label).set(String(""), temp_label.str(), prefix);

  (*this) = route.end;

  print(String("transported position"), prefix, cout);

  return route;

}

void Position::read_from_file(File& file, String prefix){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  phi.read_from_file(String("latitude"), file, false, new_prefix);
  lambda.read_from_file(String("longitude"), file, false, new_prefix);
  label.read_from_file(String("label"), file, false, new_prefix);

}


//returns a position on the Route at length l along the Route from start
void Route::compute_end(String prefix){

  //picks the first (and only) character in string type.value
  switch((type.value)[0]){

  case 'o':
    {

      //orthodrome route
  
      (end.phi.value) = asin(cos((alpha.value)) * cos((start.phi.value)) * sin((l.value)/Re) + cos((l.value)/Re) * sin((start.phi.value)));
      (end.phi).normalize();

      (end.lambda.value) = -atan((cos((start.lambda.value)) * sin((l.value)/Re) * sin((alpha.value)) + sin((start.lambda.value)) * (-cos((l.value)/Re) * cos((start.phi.value)) +  cos((alpha.value)) * sin((l.value)/Re) * sin((start.phi.value))))/( cos((l.value)/Re) * cos((start.lambda.value)) * cos((start.phi.value)) +  sin((l.value)/Re) * (sin((alpha.value)) * sin((start.lambda.value)) -  cos((alpha.value)) * cos((start.lambda.value)) * sin((start.phi.value)))));

      if(cos((l.value)/Re) * cos((start.lambda.value)) * cos((start.phi.value)) + sin((l.value)/Re) * (sin((alpha.value)) * sin((start.lambda.value)) - cos((alpha.value)) * cos((start.lambda.value)) * sin((start.phi.value))) < 0.0){(end.lambda.value) += M_PI;}

      (end.lambda).normalize();

      break;
    }
    
  case 'l':
    {

      //loxodrome route

      //this is the +- sign appearing in \phi'(t)  = +- sqrt{C/(1-C)} cos(phi(t)); 
      int sigma, tau;
      double C;
      Angle t;

      //tau = +-_{notes}
      if(( (0.0 <= (alpha.value)) && ((alpha.value) < M_PI/2.0) ) || ( (3.0*M_PI/2.0 <= (alpha.value)) && ((alpha.value) < 2.0*M_PI) )){tau = +1;}
      else{tau = -1;}

      if((0.0 <= (alpha.value)) && ((alpha.value) < M_PI)){sigma = -1;}
      else{sigma = +1;}
    
      C = gsl_pow_2(cos(alpha.value));

      /* cout << "sigma = " << sigma << "\n"; */
      /* cout << "tau = " << tau << "\n"; */
      /* cout << "C = " << C << "\n"; */
    
      t.value = -tau*sqrt((1.0-C)/C)
	* log( sqrt((1.0+sin(start.phi.value))/(1.0-sin(start.phi.value))) * tan( -tau*sqrt(C)*(l.value)/(2.0*Re) + atan(sqrt((1.0-sin(start.phi.value))/(1.0+sin(start.phi.value)))) ) );
    
      /* t.print("t", prefix, cout); */
    
      (end.phi.value) = asin( tanh( tau*sqrt(C/(1.0-C))*(t.value) + atanh(sin(start.phi.value)) ) );
      (end.phi).normalize();

      (end.lambda.value) = (start.lambda.value) + sigma*(t.value);
      (end.lambda).normalize();

      break;
    }

  case 'c':
    {

      Angle t;
      //compute the parametric angle for the circle of equal altitude starting from the length l of the curve, omega  and the Earth's radius
      //R sin omega = r, r t = l, t = l / (R sin omega)
      t.set(String(""), (l.value)/(Re*sin(omega.value)), prefix);

      (end.phi.value) = M_PI/2.0-acos(cos((omega.value))* sin((GP.phi.value))-cos((GP.phi.value))* cos((t.value)) *sin((omega.value)));
      (end.phi).normalize();


      (end.lambda.value) = -(atan((-sin((GP.lambda.value)) *(cos((GP.phi.value)) *cos((omega.value)) + cos((t.value)) *sin((GP.phi.value))* sin((omega.value))) +  cos((GP.lambda.value))*sin((omega.value))*sin((t.value)))/( cos((GP.phi.value))*cos((GP.lambda.value))*cos((omega.value)) + sin((omega.value))*(cos((GP.lambda.value))*cos((t.value))*sin((GP.phi.value)) + sin((GP.lambda.value))*sin((t.value))))));
      if(cos((GP.phi.value))*cos((GP.lambda.value))*cos((omega.value)) + sin((omega.value))*(cos((GP.lambda.value))*cos((t.value))*sin((GP.phi.value)) + sin((GP.lambda.value))*sin((t.value))) <= 0.0){
	(end.lambda.value) -= M_PI;
      }     
      (end.lambda).normalize();

    }

  }

  (end.label.value) = "";

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

class Body{

 public:
  String name, type;
  Length radius;
  Angle RA, d; 
  void enter(Catalog, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, String);
  
};

class Limb{

 public:
  char value;
  void enter(String, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, bool, String);
  
};


class Sight{

 public:
  Time master_clock_date_and_hour, time;
  //stopwatch is the reading [hh:mm:ss.s] on the stopwatch
  Chrono TAI_minus_UTC, stopwatch;
  Angle index_error, H_s, H_a, H_o, H_i, DH_refraction, DH_dip, DH_parallax_and_limb;
  Length r, height_of_eye;
  Atmosphere atmosphere;
  Body body;
  Limb limb;
  // use_stopwatch = 'n' -> time is in format "UTC" time. use_stopwatch  = 'y' -> master clock UTC time + stopwatch reading
  Answer artificial_horizon, use_stopwatch;
  //label to add a note about the sight
  String label;
  //this is the position in route_list of the route linked to Sight. If there is no route linked to Sight, then related_route = -1. 
  int related_route;
  //this is a list of the items whic are part of a Sight object (master_clock_date_and_hour, time, body, ...)
  vector<String> items;


  Sight();
  static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
  bool get_coordinates(Route*, String);
  void compute_DH_dip(String);
  bool compute_DH_refraction(String);
  void compute_DH_parallax_and_limb(String);

  void compute_H_a(String);
  bool compute_H_o(String);

  bool enter(Catalog, String, String);
  void modify(String);
  void print(String, String, ostream&);
  bool read_from_file(File&, String);
  bool reduce(Route*, String);
  bool check_data_time_interval(String);

};

void Sight::modify(String prefix){
  
  unsigned int i;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  cout << prefix.value << "Enter the item that you want to modify:\n";

  for(i=0; i<items.size(); i++){
    cout << prefix.value << "\t(" << i+1 << ") " << (items[i]).value << "\n";
  }


}

void Route::print(String name, String prefix, ostream& ostr){

  String s, new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  ostr << prefix.value << name.value << ":\n";

  type.print(String("type"), new_prefix, ostr);

  if((type.value == "l") || (type.value == "o")){
    
    start.print(String("start position"), new_prefix, ostr);
    alpha.print(String("starting heading"), new_prefix, ostr);
    l.print(String("length"), String("nm"), new_prefix, ostr);
    
  }else{
    
    GP.print(String("ground position"), new_prefix, ostr);
    omega.print(String("aperture angle"), new_prefix, ostr);
    
  }

  label.print(String("label"), new_prefix, ostr);

  
  if(related_sight != -1){

    cout << new_prefix.value << "Related sight # = " << related_sight+1 << "\n";
    
  }
  
  
}


void Route::enter(String name, String prefix){

  string s;
  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter " << name.value << ":\n";

  do{
    type.enter(String("type [l(=loxodrome)/o(=orthodrome)/c(=circle of equal altitude)]"), new_prefix);
    check = ((type.value == "l") || (type.value == "o") || (type.value == "c"));
    if(!check){
      cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
    }
  }while(!check);
  
  if((type.value == "l") || (type.value == "o")){
    //if the route is a loxodrome or an orthodrome, I enter its starting point and  starting heading (the ground position GP and aperture angle remain unused)

    start.enter(String("starting position"), new_prefix);
    alpha.enter(String("starting heading"), true, new_prefix);
    l.enter(String("length"), String("nm"), new_prefix);

    
  }else{
    //if the route is a circle of equal altitude, I enter its ground position and its aperture angle (alpha remains unused) ... 
    GP.enter(String("ground position"), new_prefix);
    omega.enter(String("aperture angle"), true, new_prefix);
    
    /* //... and then compute the resulting starting position and starting heading */
    /* start.phi.set(String("latitude of ground position"), (GP.phi.value) - (omega.value), true, new_prefix); */
    /* start.lambda.set(String("longitude of ground position"), GP.lambda.value, true, new_prefix); */
  }

  label.enter(String("label"), new_prefix);

  //given that the route has just been entered, it is not yet related to any sight, thus I set
  related_sight = -1;

}




bool Chrono::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  stringstream new_prefix;
  bool check = true;
  size_t pos;
  
  //prepend \t to prefix
  new_prefix << "\t" << prefix.value;

  pos = 0;

  if(search_entire_file){

    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!

    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));

  }else{

    line.clear();
    getline(file.value, line);
    
  }


  
  pos = line.find(" = ");

  //read hours
  h = stoi(line.substr(pos+3, 2).c_str(), NULL, 10);
  if(!((0 <= h) && (h < 24))){
    
    check &= false;
    cout << prefix.value << RED << "\tValue of hh is not valid!\n" << RESET;
    
  }

  //read minutes
  m = stoi(line.substr(pos+3+3, 2).c_str(), NULL, 10);   
  if(!((0 <= m) && (m < 60))){
      
    check &= false;
    cout << prefix.value << RED << "\tValue of mm is not valid!\n" << RESET;
      
  }
  
  //read seconds
  s = stod(line.substr(pos+3+3+3, line.size() - (pos+3+3+3)).c_str());
  if(!((0.0 <= s) && (s < 60.0))){

    check &= false;
    cout << prefix.value << RED << "\tValue of mm is not valid!\n" << RESET;

  }

  if(check){
    print(name, prefix, cout);
  }
	  
  return check;

}







//this function returns true if the date read is consistent, false if it is not 
bool Date::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  stringstream new_prefix;
  bool check = true;
  size_t pos;

  //prepend \t to prefix
  new_prefix << "\t" << prefix.value;

  pos = 0;

  if(search_entire_file){

    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!

    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));

  }else{

    line.clear();
    getline(file.value, line);
    
  }
  
  pos = line.find(" = ");

  Y = stoi(line.substr(pos+3, 4).c_str(), NULL, 10);

  check_leap_year();
  if((Y_is_leap_year)){
    (days_per_month) = days_per_month_leap;
    cout << new_prefix.str() << YELLOW << "Entered a leap year\n" << RESET;
  }else{
    (days_per_month) = days_per_month_common;
    cout << new_prefix.str() << "Entered a common year\n";
  }
  
  M = stoi(line.substr(pos+3+5, 2).c_str(), NULL, 10);

  if(!((1<=M) && (M < 12+1))){
    check &= false;
    cout << new_prefix.str() << RED << "\tValue of MM is not valid!\n" << RESET;
  }

  D = stoi(line.substr(pos+3+5+3, 2).c_str());

  if(!((1<=D) && (D < days_per_month[M-1]+1))){
    check &= false;
    cout << prefix.value << RED << "\tValue of DD is not valid!\n" << RESET;
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

bool Time::read_from_file(String name, File& file, String prefix){

  string line;
  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

 
  //read dummy line
  getline(file.value, line);

  cout << prefix.value << name.value << ":\n";
  
  //read date
  if(!(date.read_from_file(name, file, false, new_prefix))){ 
    check &= false;
  }

  //read chrono
  if(!(chrono.read_from_file(name, file, false, new_prefix))){
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





bool Length::check_valid(String name, String prefix){

  bool check = true;
  
  if(value<0.0){
    check &= false;
    cout << prefix.value << RED << "Entered value of " << name.value << " is not valid!\n" << RESET;
  }

  return check;
  
}

void Length::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  stringstream new_prefix;
  size_t pos1, pos2;
  String unit;

  //prepend \t to prefix
  new_prefix << "\t" << prefix.value;

  if(search_entire_file){

    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!

    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));

  }else{

    line.clear();
    getline(file.value, line);
    
  }
   
  pos1 = line.find(" = ");
  pos2 = line.find(" nm");

  if(line.find(" nm") != (string::npos)){
    //in this case the units of the length read is nm
    cout << prefix.value << "Unit is in nm\n";
    pos2 = line.find(" nm");
    unit = String("nm");
  }else{
    //in this case the units of the length read is m
    cout << prefix.value << "Unit is in m\n";
    pos2 = line.find(" m");
    unit = String("m");
  }
  
  value = stod(line.substr(pos1+3, pos2 - (pos1+3)).c_str());
  if(unit.value == "m"){
    value/=(1e3*nm);
  }
  
  print(name, String("nm"), prefix, cout);

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



void Limb::read_from_file(String name, File& file, bool search_entire_file, String prefix){

  string line;
  size_t pos;

  if(search_entire_file){

    //rewind the file pointer
    file.value.clear();                 // clear fail and eof bits
    file.value.seekg(0, std::ios::beg); // back to the start!

    do{
    
      line.clear();
      getline(file.value, line);

    }while((line.find(name.value)) == (string::npos));

  }else{

    line.clear();
    getline(file.value, line);
    
  }
  pos = line.find(" = ");

  value = line[pos+3];
  
  print(name, prefix, cout);

  
}


void Body::read_from_file(String name, File& file, String prefix){

  string line;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  size_t pos;

  cout << prefix.value << name.value << ":\n";
  
  //read first line with no information
  getline(file.value, line);

  //read type
  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");
  type = line.substr(pos+3, line.size() - (pos+3));
  cout << new_prefix.value << "Type = " << type.value << "\n";


  //read name
  line.clear();
  getline(file.value, line);
  pos = line.find(" = ");
  ((*this).name) = line.substr(pos+3, line.size() - (pos+3));
  cout << new_prefix.value << "Name = " << ((*this).name).value << "\n";


  if(type.value == "star"){
    RA.read_from_file(String("right ascension"), file, false, new_prefix);
    d.read_from_file(String("declination"), file, false, new_prefix);
  }else{
    radius.read_from_file(String("radius"), file, false, new_prefix);
  }
  
}


class Catalog{

 public:
  vector<Body> list;
  Catalog(String, String);
  void add(String, String, double);
  void print(String, ostream&);

};

Catalog::Catalog(String filename, String prefix){

  File file;
  string line;
  Body body;
  streampos old_position;


  file.set_name(filename);
  if(file.open(String("in"), String(""))){

    // stores the position of file.value
    old_position = file.value.tellg();
    //read the next line in the file
    getline(file.value, line); 

    //check whether the next line in the file has reached the end of file
    while(!(file.value).eof()){
      
      //if the next line in the file has not reached the end of file, I set file.value to its old position and keep reading the file
      (file.value).seekg(old_position);

      body.read_from_file(String("read body"), file, prefix);
      list.push_back(body);

      // stores the position of file.value
      old_position = file.value.tellg();
      //read the next line in the file
      getline(file.value, line); 

    }
  
    file.close(String(""));

  }

}

void Catalog::print(String prefix, ostream& ostr){

  unsigned int i;
  stringstream name;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Bodies in the catalog:\n";
  for(i=0; i<list.size(); i++){

    name.str("");
    name << "Body #" << i;
    (list[i]).print(name.str(), new_prefix, ostr);

  }

}

void Catalog::add(String type, String name, double radius){

  Body body;
  
  body.type = type;
  body.name = name;
  body.radius.value = radius;
  
  list.push_back(body);
  
  cout << "Added body to catalog:\n";
  body.print(String("body"), String("\t"), cout);

}




void Answer::enter(String name, String prefix){

  bool check;
  string temp;
  
  do{

    temp.clear();
    
    cout << prefix.value << "Enter " << name.value << " [y/n]:";

    getline(cin >> ws, temp);
    
    if(((temp[0]=='y') || (temp[0]=='n')) && (temp.size() == 1)){
      value = temp[0];
      check = true;
    }
    else{
      cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
      check = false;
    }
  }while(!check);

  print(name, prefix, cout);

}

void Answer::print(String name, String prefix, ostream& ostr){

  ostr << prefix.value << name.value << " = " << value << "\n";
  
}




//this function transports the Route (*this) with the Route transporting route
void Route::transport(String prefix){

  String new_prefix;

  if((type.value)[0] == 'c'){
  
    Route transporting_route;
    stringstream temp_label;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    transporting_route = (GP.transport(new_prefix));
  

    //append 'translated to ...' to the label of sight, and make this the new label of sight
    temp_label << label.value << ", tr. w. " << transporting_route.type.value << ", COG = " << transporting_route.alpha.to_string(display_precision).str().c_str() << ", l = " << transporting_route.l.value << " nm";
    label.set(String(""), temp_label.str(), prefix);
    //given that I transported the Route object, this object is no longer directly connected to its Sight object, thus I set 
    related_sight = -1;

    print(String("transported route"), prefix, cout);

  }else{

    cout << prefix.value << RED << "I cannot transport routes different from circles of equal altitude\n" << RESET;

  }

}



//this function returns true if the reading operation has been performed without errors, false otherwise
bool Sight::read_from_file(File& file, String prefix){

  string line;
  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  body.read_from_file(String("body"), file, new_prefix);
  if(body.type.value != "star"){
    limb.read_from_file(String("limb"), file, false, new_prefix);
  }
  H_s.read_from_file(String("sextant altitude"), file, false, new_prefix);
  index_error.read_from_file(String("index error"), file, false, new_prefix);
  artificial_horizon.read_from_file(String("artificial horizon"), file, false, new_prefix);
  if((artificial_horizon.value) == 'n'){
    height_of_eye.read_from_file(String("height of eye"), file, false, new_prefix);
  }
  
  check &= master_clock_date_and_hour.read_from_file(String("master-clock date and hour of sight"), file, new_prefix);
  if(!check){
    cout << prefix.value << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
  }
  time = master_clock_date_and_hour;
 
  use_stopwatch.read_from_file(String("use of stopwatch"), file, false, new_prefix);

  if(use_stopwatch.value == 'y'){
      
    stopwatch.read_from_file(String("stopwatch"), file, false, new_prefix);
    time.add(stopwatch);

  }
  
  TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file, false, new_prefix);
  time.add(TAI_minus_UTC);
  time.print(String("TAI date and hour of sight"), new_prefix, cout);

  //check whether the date and hour of sight falls within the time window covered by JPL data files
  check &= check_data_time_interval(prefix);

  label.read_from_file(String("label"), file, false, new_prefix);
  
  //given that the sight is not yet related to a route, I set 
  related_route = -1;

  if(!check){
    cout << prefix.value << RED << "Error reading sight!\n" << RESET;
  }
  
  return check;
  
}

bool Sight::check_data_time_interval(String prefix){
  
  int l_min, l_max;
  stringstream temp;
  File data_file;
  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  
  //data_file is the file where that data relative to body are stored: I count the number of lines in this file and store them in data_file.number_of_lines
  temp.clear();
  if((body.type.value) != "star"){
    temp << "data/" << body.name.value << ".txt";
  }else{
    temp << "data/j2000_to_itrf93.txt";
  }
  data_file.set_name(temp.str()); 
  data_file.count_lines(new_prefix);

  //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
  l_min = (int)(L*((time.MJD)-MJD_min))-(int)(N/2.0);
  //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
  l_max = (int)(L*((time.MJD)-MJD_min))+(int)(N/2.0);

  //check whether the lines from l_min to l_max, which are used for the data interpolation, are present in the file where data relative to the body are stored 
  if((l_min >= 0) && (l_max < (int)(data_file.number_of_lines))){
    check = true;
  }else{
    check = false;
    cout << new_prefix.value << RED << "Time lies outside interval of NASA's JPL data files!\n" << RESET;
  }

  return check;

  
}

void Sight::print(String name, String prefix, ostream& ostr){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  ostr << prefix.value << name.value << ":\n";

  body.print(String("body"), new_prefix, ostr);
  if(body.type.value != "star"){
    limb.print(String("limb"), new_prefix, ostr);
  }
  H_s.print(String("sextant altitude"), new_prefix, ostr);
  index_error.print(String("index error"), new_prefix, ostr);
  artificial_horizon.print(String("artificial horizon"), new_prefix, ostr);
  if(artificial_horizon.value == 'n'){
    height_of_eye.print(String("height of eye"), String("m"), new_prefix, ostr);
  }
  master_clock_date_and_hour.print(String("master-clock date and hour of sight"), new_prefix, ostr);
  use_stopwatch.print(String("use of stopwatch"), new_prefix, ostr);
  if(use_stopwatch.value == 'y'){
    stopwatch.print(String("stopwatch"), new_prefix, ostr);
  }
  TAI_minus_UTC.print(String("TAI - UTC at time of master-clock synchronization with UTC"), new_prefix, ostr);

  label.print(String("label"), new_prefix, ostr);
  
  if((related_route != -1) && (&ostr == &cout)){
    ostr << new_prefix.value << "Related route # = " << related_route+1 << "\n";
  }
  
}

class Plot{
  
 public:
  Catalog* catalog;
  File file_id, file_gnuplot, file_boundary;
  int job_id;
  stringstream command, plot_command;
  vector<Sight> sight_list;
  vector<Position> position_list;
  vector<Route> route_list;
  vector<String> choices;
  vector<unsigned int> crossing_route_list;

  Plot(Catalog*, String);
  //~Plot();
  bool add_sight(String);
  void transport_route(unsigned int, String);
  void add_position(String);
  void add_route(String);
  void remove_sight(unsigned int, String);
  void transport_position(unsigned int, String);
  void remove_position(unsigned int, String);
  void remove_route(unsigned int, String);
  bool read_from_file(File&, String);
  void print(bool, String, ostream&);
  void print_sights(String, ostream&);
  void print_positions(String, ostream&);
  void print_routes(bool, String, ostream&);
  void show(bool, String);
  void menu(String);
  void compute_crossings(String);

};

bool Plot::read_from_file(File& file, String prefix){

  stringstream line_ins;
  string line;
  size_t pos;
  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  
  if(!(file.open(String("in"), new_prefix))){
    
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
    
      cout << new_prefix.value << "Found new sight!\n";
  
      //read the sight block
      Sight sight;
      Route route;
      
      //if I find a sight which returns an error message when read from file, to be conservative I do not add any of the following sights in the file to sight_list because they may contain other errors
      check &= (sight.read_from_file(file, new_prefix));
      if(check){
	  
	check &= (sight.reduce(&route, new_prefix));

	if(check){
	  sight.print(String("New sight"), new_prefix, cout);
    
	  sight_list.push_back(sight);
	  cout << new_prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";

	  route_list.push_back(route);
	  cout << new_prefix.value << "Route added as route #" << route_list.size() << ".\n";

	  //I link the sight to the route, and the route to the sight
	  route_list[route_list.size()-1].related_sight = sight_list.size()-1;
	  sight_list[sight_list.size()-1].related_route = route_list.size()-1;
	  
	}
	  
      }
      
      line.clear();
      //read dummyt text line 
      getline(file.value, line);
      pos = line.find("Sight #");
 
    }

    //2. Here I read routes

    line.clear();
    //read dummy text line 
    getline(file.value, line);
    pos = line.find("Route #");

    //if I have found 'Route #' in the line above, then I proceed and read the relative position
    while(pos != (string::npos)){
    
      cout << new_prefix.value << "Found new route!\n";
  
      //read the position block
      Route route;

      route.read_from_file(file, new_prefix);
      
      route.print(String("New route"), new_prefix, cout);

      route_list.push_back(route);
      cout << new_prefix.value << "Route added as position #" << route_list.size() << ".\n";
	  
      line.clear();
      //read dummyt text line 
      getline(file.value, line);
      pos = line.find("Route #");
 
    }


    //3. Here I read positions

    line.clear();
    //read dummy text line 
    getline(file.value, line);
    pos = line.find("Position #");

    //if I have found 'Position #' in the line above, then I proceed and read the relative position
    while(pos != (string::npos)){
    
      cout << new_prefix.value << "Found new position!\n";
  
      //read the position block
      Position position;

      position.read_from_file(file, new_prefix);
	  
      position.print(String("New position"), new_prefix, cout);
    
      position_list.push_back(position);
      cout << new_prefix.value << "Position added as position #" << position_list.size() << ".\n";
	  
      line.clear();
      //read dummyt text line 
      getline(file.value, line);
      pos = line.find("Position #");
 
    }

    
    file.close(new_prefix);

  }

  if(!check){
    cout << prefix.value << RED << "There was an error while reading file!\n" << RESET;
  }
  
  return check;
  
}




void Plot::menu(String prefix){

  String new_prefix;
  unsigned int i, j;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  cout << prefix.value << "You can:\n";
  
  cout << prefix.value << BOLD << "Sights:" << RESET << "\n";
  for(i=0; i<3; i++){
    cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
  }
  
  cout << prefix.value << BOLD << "Positions:" << RESET << "\n";
  for(i=3; i<7; i++){
    cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
  }
  
  cout << prefix.value << BOLD << "Routes:" << RESET << "\n";
  for(i=7; i<11; i++){
    cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
  }

  cout << prefix.value << BOLD << "Graph:" << RESET << "\n";
  for(i=11; i<14; i++){
    cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
  }


  cout << prefix.value << BOLD << "Files:" << RESET << "\n";
  for(i=14; i<16; i++){
    cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
  }
  
  i=16;
  cout << prefix.value << "\n";
  cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";



  enter_unsigned_int(&i, true, 1, choices.size()+1, String("choice #"), prefix);

  
  
  switch(i){

  case 1:{
    
    add_sight(new_prefix);
    print(true, new_prefix, cout);
    show(false, new_prefix);
    menu(prefix);  

  }
    break;


  case 2:{
    
    if(sight_list.size() > 0){

      print_sights(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, sight_list.size()+1, String("# of sight that you want to modify"), new_prefix);
      i--;

      (sight_list[i]).modify(new_prefix);
      print(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      cout << YELLOW << "There are no sights to modify!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;
   
  case 3:{

    if(sight_list.size() > 0){
 
      print_sights(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, sight_list.size()+1, String("# of sight that you want to delete"), new_prefix);	
      i--;
   
      remove_sight(i, new_prefix);
      print(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      cout << YELLOW << "There are no sights to delete!\n" << RESET;
    }

    menu(prefix);  
   
  }
    break;

  case 4:{

    add_position(new_prefix);
    print(true, new_prefix, cout);
    show(false, new_prefix);
    menu(prefix);  

  }
    break;


    
  case 5:{
    
    if(position_list.size() > 0){

      print_positions(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, position_list.size()+1, String("# of position that you want to modify"), new_prefix);
      i--;

      (position_list[i]).modify(new_prefix);
      print(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      cout << YELLOW << "There are no positions to modify!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;
    
  case 6:{
    
    if(position_list.size() > 0){

      print_positions(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, position_list.size()+1, String("# of position that you want to transport"), new_prefix);
      i--;

      transport_position(i, new_prefix);
      print(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      cout << YELLOW << "There are no positions to transport!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;

  case 7:{

    if(position_list.size() > 0){


      print_positions(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, position_list.size()+1, String("# of position that you want to delete"), new_prefix);
      i--;
	
      remove_position(i, new_prefix);
      print(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      cout << YELLOW << "There are no positions to delete!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;

  case 8:{

    add_route(new_prefix);
    print(true, new_prefix, cout);
    show(false, new_prefix);
    menu(prefix);  

  }
    break;


    
  case 9:{

    if(route_list.size() > 0){

      print_routes(true, new_prefix, cout);

      enter_unsigned_int(&i, true, 1, route_list.size()+1, String("# of route that you want to transport"), new_prefix);	
      i--;
   
      transport_route(i, new_prefix);
      print_routes(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      
      cout << YELLOW << "There are no routes to transport!\n" << RESET;
      
    }

    menu(prefix);
   
  }
    break;

  case 10:{
    
    //there need to be at list two routes of type "c" to compute crossings. Here I write the indexes of routes of type "c" into crossing_route_list 
    for(crossing_route_list.clear(), j=0; j<route_list.size(); j++){

      if((((route_list)[j]).type.value) == "c"){
	
	crossing_route_list.push_back(j);
	
      }
      
    }
	
    if(crossing_route_list.size() > 1){

      compute_crossings(new_prefix);

    }else{
      
      cout << YELLOW << "There are not enough routes to compute crossings!\n" << RESET;

    }

    crossing_route_list.clear();
    
    print(true, new_prefix, cout);
    show(false, new_prefix);
    menu(prefix);
   
  }
    break;

    
  case 11:{

    if(route_list.size() > 0){


      print_routes(true, new_prefix, cout);

      enter_unsigned_int(&i, true, 1, route_list.size()+1, String("# of route that you want to delete"), new_prefix);
      i--;
	
      remove_route(i, new_prefix);
      print(true, new_prefix, cout);
      show(false, new_prefix);

    }else{
      cout << YELLOW << "There are no routes to delete!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;


  case 12:{

    show(false, new_prefix);
    menu(prefix);  

  }
    break;

  case 13:{

    show(true, new_prefix);
    menu(prefix);  

  }
    break;


  case 14:{

    if(route_list.size() + position_list.size() > 0){
  
      //in this loop I don't increment i because position_list.size() decreases at each iteration
      for(i=0; i<position_list.size(); ){
	remove_position(i, new_prefix);
      }
      
      //in this loop I don't increment i because route_list.size() decreases at each iteration
      for(i=0; i<route_list.size(); ){
	remove_route(i, new_prefix);
      }
 
      print(true, new_prefix, cout);
      show(false, new_prefix);
   
    }else{
      
      cout << YELLOW << "There are no routes nor positions to clear!\n" << RESET;
      
    }
    
    menu(prefix);
    
  }
    break;

    
  case 15:{

    if(sight_list.size() + route_list.size() + position_list.size() > 0){
  
      File file;
      stringstream temp;
      Answer answer;

      //answer says whether one should create a new file on which the data will be saved
      answer.set(String(""), 'y', new_prefix);
    
      file.name.enter(String("name of file (without extension)"), new_prefix);
      //add the extension .sav to name of file
      temp.str("");
      temp << file.name.value << ".sav";
      file.set_name(temp.str());

      //If the file already exists, I ask whether the user want to overwrite it
      if((file.check_if_exists(new_prefix))){

	cout << new_prefix.value << YELLOW << "You may be overwriting data!\n" << RESET;
	answer.enter(String("whether you want to overwrite the file"), new_prefix);
	
      }

      if((answer.value) == 'y'){

	file.remove(new_prefix);	
	file.open(String("out"), new_prefix);    
	print(false, new_prefix, file.value);
	file.close(new_prefix);

	command.str("");
	command << "mv plot.plt " << "'plot " << temp.str() << "'";
	system(command.str().c_str());

      }

    }else{
      
      cout << YELLOW << "There are no routes nor positions to save!\n" << RESET;
      
    }
    
    menu(prefix);
    
  }
    break;

  case 16:{

    File file;
    stringstream line_ins;
    String line;

    line_ins.str("");
    line.enter(String("name of file (without extension)"), new_prefix);
    line_ins << line.value << ".sav";

    file.set_name(String(line_ins.str()));
    
    if(read_from_file(file, new_prefix)){
      print(true, new_prefix, cout);
      show(false, new_prefix);
    }
        
    menu(prefix);  

  }
    break;
    
    
  case 17:{

    File file;
    String line;

    //get date and time, which will be used for filename 
    get_date_hour(line, new_prefix);
    line = line.append(String(".sav"));

    //print all plots to file with the filename above
    ((file.name).value) = line.value;
    file.open(String("out"), new_prefix);
    print(false, new_prefix, file.value);
    file.close(new_prefix);

    //if plot.plt has been filled, here I save it with the name 'plot' + filename above
    if(sight_list.size() + position_list.size() >0){
      command.str("");
      command << "mv plot.plt " << "'plot " << line.value.c_str() << "'";
      system(command.str().c_str());
    }

    //if job_id = -1 this means that there is no gnuplot script running in the background, thus there is no need to stop it. Otherwise, the gnuplot script running in the background is stopped. 
    if(job_id != -1){
    
      command.str("");
      command << "kill -9 " << job_id;
      system(command.str().c_str());
    
    }
    file_id.remove(new_prefix);
    file_gnuplot.remove(new_prefix);
    file_boundary.remove(new_prefix);
    
    cout << prefix.value << CYAN << "Fair winds, following seas...\n" << RESET;
  }
    break;


  }

    
}

Plot::Plot(Catalog* cata, String prefix){

  catalog = cata;
  job_id = -1;

  plot_command.precision(data_precision);
  command.precision(data_precision);

  file_id.set_name(String("job_id.txt"));
  file_gnuplot.set_name(String("plot.plt"));
  file_boundary.set_name(String("boundary.txt"));

  file_boundary.remove(prefix);

  choices = {String("Add a sight"), String("Modify a sight"), String("Delete a sight"), String("Add a position"), String("Modify a position"), String("Transport a position"), String("Delete a position"), String("Add a route"), String("Transport a route"), String("Compute route crossings"), String("Delete a route"), String("Replot"), String("Full zoom out"), String("Clear"), String("Save to file"), String("Read from file"), String("Exit")};
  
}

/*
  Plot::~Plot(){

  //file_gnuplot.remove();
  file_id.remove();
  
  }
*/

void Plot::compute_crossings(String prefix){

  unsigned int i, j, l;
  String new_prefix;
  stringstream dummy;
  Length r, s;
  vector< vector<Position> > p;
  vector<Position> q, q_temp(2);
  Position center;
  File file_init;
  Angle min_crossing_angle;
  double x;
  Route error_circle;


  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  cout << prefix.value << YELLOW << "Reading minimal crossing angle between circles of equal altitude from file " << file_init.name.value << " ...\n" << RESET;
  file_init.set_name(String(path_file_init));
  file_init.open(String("in"), new_prefix);
  min_crossing_angle.read_from_file(String("minimal crossing angle between circles of equal altitude"), file_init, true, new_prefix);
  file_init.close(new_prefix);
  cout << prefix.value << YELLOW << "... done.\n" << RESET;


  
  cout << prefix.value << "Computing crossings between routes #:";
  for(i=0; i<crossing_route_list.size(); i++){
    cout << crossing_route_list[i]+1 << " ";
  }
  cout << "\n";

  //I run over all the pairs of circles of equal altitude and write their crossing points into p
  l=0;
  for(i=0; i<crossing_route_list.size(); i++){
    for(j=i+1; j<crossing_route_list.size(); j++){

 
      cout << prefix.value << "Computing crossing between routes " << crossing_route_list[i]+1 << " and " << crossing_route_list[j]+1 << "\n";
      
      (route_list[crossing_route_list[i]]).crossing((route_list[crossing_route_list[j]]), &q_temp, &x, new_prefix);

      //if the two routes under consideration are not too parallel (i.e., |cos(their crossing angle)| < cos(min_crossing_angle.value), then I add this crossing to the list of sensible crossings
      if(fabs(x) < cos(min_crossing_angle.value)){

	p.resize(l+1);
	(p[l]).resize(2);

	p[l] = q_temp;

	q.push_back(q_temp[0]);  
	q.push_back(q_temp[1]);  

	l++;

	cout << new_prefix.value << "Crossing accepted\n";

      }else{

	cout << new_prefix.value << "Crossing not accepted\n";

      }
      
    }
  }

  //r is the minimal distance between crossing points. To find the minimum, here I set r to it largest possible value, obtained when the two points are at the antipodes. I find the pair of crossing points which is closest to each other, and set Position center to one of the Positions in this pair. center will thus represent the approximate astronomical position. I will then run over all the pairs of crossing points in p, p[i], and pick either p[i][0] or p[i][1]: I will pick the one which is closest to center 

  cout << prefix.value << "Distances between pairs of crossing positions:\n";
  r.set(String(""), M_PI*Re, prefix);
  
  for(i=0; i<q.size(); i++){
    for(j=i+1; j<q.size(); j++){

      dummy.str("");
      dummy << "distance between points " << i << " and " << j;
      
      (q[i]).distance((q[j]), &s, String(dummy.str()), new_prefix);

      if(r>s){
	r = s;
	center = (q[i]);
      }

    }
  }

  r.print(String("minimal distance between crossing points"), String("nm"), prefix, cout);
  center.print(String("center crossing"), prefix, cout);

  //I append center to the list of retained crossings, run through all the pairs of crossings except for center, and select the Position in the pair which is closer to center. Crossings are also added to position_list, in such a way that they are shown in the plot
  q.clear();
  q.push_back(center);
  position_list.push_back(q[q.size()-1]);
  for(i=0; i<p.size(); i++){

    if(!( ((p[i][0])==center) || ((p[i][1])==center) )){
      
      center.distance(p[i][0], &r, String(""), new_prefix);
      center.distance(p[i][1], &s, String(""), new_prefix);

      if(r>s){

	q.push_back(p[i][1]);

      }else{

	q.push_back(p[i][0]);

      }
    
      position_list.push_back(q[q.size()-1]);

    }
    
  }

  //compute astronomical position by averaging on all viable crossing points
  center.lambda.value = 0.0;
  center.phi.value = 0.0;
  for(i=0 ; i<q.size(); i++){

    (center.lambda.value) += ((q[i]).lambda.value);
    (center.phi.value) += ((q[i]).phi.value);

  }
  (center.lambda.value)/=((double)(q.size()));
  (center.phi.value)/=((double)(q.size()));
  center.label.set(String(""), String("astronomical position"),  prefix);

  //compute error on astronomical position
  (r.value) = 0.0;
  for(i=0 ; i<q.size(); i++){
    for(j=i+1 ; j<q.size(); j++){


      (q[i]).distance(q[j], &s, String(""), new_prefix);
      r= r+s;
      
    }
  }
  (r.value) /= ((double)((q.size())*((q.size())-1)/2));

  //computes the circle of equal altitude which represents the error of the sight
  (error_circle.type) = String("c");
  (error_circle.GP) = center;
  (error_circle.omega.value) = (r.value)/Re;
  (error_circle.label) = String("error on astronomical position");
  (error_circle.related_sight) = -1;
  
  center.print(String("astronomical position"), prefix, cout);
  r.print(String("error on astronomical position"), String("nm"), prefix, cout);
	  
  position_list.push_back(center);
  route_list.push_back(error_circle);
  
  p.clear();
  q.clear();
  q_temp.clear();
  
}

void Plot::print(bool print_all_routes, String prefix, ostream& ostr){

  if(sight_list.size()>0){
    print_sights(prefix, ostr);
  }

  if(route_list.size()>0){
    print_routes(print_all_routes, prefix, ostr);
  }
  
  if(position_list.size()>0){
    print_positions(prefix, ostr);
  }



  
}

void Plot::print_sights(String prefix, ostream& ostr){

  stringstream name;
  unsigned int i;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
    
  ostr << prefix.value << "Sights in the plot:\n";
  for(i=0; i<sight_list.size(); i++){
    name.str("");
    name <<  "Sight #" << i+1;
    (sight_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
  }
  

}

void Plot::print_positions(String prefix, ostream& ostr){

  stringstream name;
  unsigned int i;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
   
  ostr << prefix.value << "Positions in the plot:\n";
  for(i=0; i<position_list.size(); i++){
    name.str("");
    name << "Position #" << i+1;
    (position_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
  }


}

void Plot::print_routes(bool print_all_routes, String prefix, ostream& ostr){

  stringstream name;
  unsigned int i, j;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
   
  ostr << prefix.value << "Routes in the plot:\n";
  
  for(i=0, j=0; i<route_list.size(); i++){
    
    //if print_all_routes = false, I only print routes which are linke to a sight
    if(((route_list[i].related_sight) == -1) || print_all_routes){
      
      name.str("");
      name << "Route #" << j+1;
      
      (route_list[i]).print(String(name.str().c_str()), new_prefix, ostr);

      j++;
      
    }
    
  }


}




bool Plot::add_sight(String prefix){

  //create a new sight and new route in the respective lists
  sight_list.resize(sight_list.size()+1);
  route_list.resize(route_list.size()+1);
  
  bool check = true;
  
  (sight_list[sight_list.size()-1]).enter((*catalog), String("new sight"), prefix);
  check &= ((sight_list[sight_list.size()-1]).reduce(&(route_list[route_list.size()-1]), prefix));
  
  //I link the sight to the route, and the route to the sight
  (sight_list[sight_list.size()-1]).related_route = route_list.size()-1;
  (route_list[route_list.size()-1]).related_sight = sight_list.size()-1;
  
  
  if(check){
    (sight_list[sight_list.size()-1]).print(String("Sight"), prefix, cout);
  
    cout << prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";
    cout << prefix.value << "Route added as route #" << route_list.size() << ".\n";
  }

  return check;
  
}

void Plot::add_position(String prefix){

  Position position;
  
  position.enter(String("new position"), prefix);
  
  position_list.push_back(position);
  cout << prefix.value << "Position added as position #" << position_list.size() << ".\n";

 
}

void Plot::add_route(String prefix){

  Route route;
  
  route.enter(String("new route"), prefix);
  
  route_list.push_back(route);
  cout << prefix.value << "Route added as position #" << route_list.size() << ".\n";

 
}



void Plot::remove_sight(unsigned int i, String prefix){

  stringstream name;
  unsigned int j;
  Answer remove_related_route;
  int i_related_route;

  i_related_route = ((sight_list[i]).related_route);

  name.str("");
  name << "Sight to be removed: Sight #" << i+1;
  
  (sight_list[i]).print(String(name.str().c_str()), prefix, cout);
  
  sight_list.erase(sight_list.begin()+i);

  //update the linking indexed of routes in accordance with the deletion of the sight
  for(j=0; j<route_list.size(); j++){
    
    if(((route_list[j]).related_sight != -1) && ((route_list[j]).related_sight >= ((int)i))){
      
      if((route_list[j]).related_sight == ((int)i)){
	(route_list[j]).related_sight = -1;
      }else{
	((route_list[j]).related_sight)--;
      }

    }
      
  }
  
  cout << prefix.value << "Sight removed.\n";
  

  if(i_related_route != -1){
    
    remove_related_route.enter(String("whether you want to remove the route related to this sight"), prefix);
    if((remove_related_route.value) == 'y'){

      remove_route(i_related_route, prefix);
      
    }

  }

}

void Plot::remove_position(unsigned int i, String prefix){

  stringstream name;

  name.str("");
  name << "Position to be removed: #" << i+1;
  
  (position_list[i]).print(String(name.str().c_str()), prefix, cout);
  
  position_list.erase(position_list.begin()+i);
  
  cout << prefix.value << "Position removed.\n";

}

void Plot::remove_route(unsigned int i, String prefix){

  unsigned int j;
  int i_related_sight;
  stringstream name;
  Answer remove_related_sight;

  i_related_sight = ((route_list[i]).related_sight);
  
  name.str("");
  name << "Route to be removed: #" << i+1;
  
  (route_list[i]).print(String(name.str().c_str()), prefix, cout);
  
  route_list.erase(route_list.begin()+i);

  //update the linking indexed of sights in accordance with the deletion of the route
  for(j=0; j<sight_list.size(); j++){
    
    if(((sight_list[j]).related_route != -1) && ((sight_list[j]).related_route >= ((int)i))){
      
      if((sight_list[j]).related_route == ((int)i)){
	(sight_list[j]).related_route = -1;
      }else{
	((sight_list[j]).related_route)--;
      }

    }
      
  }
  
  cout << prefix.value << "Route removed.\n";
  

  if(i_related_sight != -1){
    
    remove_related_sight.enter(String("whether you want to remove the sight related to the route"), prefix);
    
    if((remove_related_sight.value) == 'y'){

      remove_sight(i_related_sight, prefix);
      
    }

  }


}


void Plot::transport_route(unsigned int i, String prefix){

  stringstream name;
  String new_prefix;
  Route original_route;
  Answer keep_original;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  name.str("");
  name << "Route to be transported: Route #" << i+1;

  original_route = route_list[i];
  
  (route_list[i]).print(String(name.str().c_str()), new_prefix, cout);
  (route_list[i]).transport(prefix);  
  cout << prefix.value << "Route transported.\n";

  keep_original.enter(String("whether you want to keep the original route"), new_prefix);
  if(keep_original.value == 'y'){
    route_list.insert(route_list.begin()+i, original_route);
  }

}


void Plot::transport_position(unsigned int i, String prefix){

  stringstream name;
  String new_prefix;
  Position original_position;
  Answer keep_original;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  name.str("");
  name << "Position to be transported: Position #" << i+1;

  original_position = position_list[i];

  (position_list[i]).print(String(name.str().c_str()), new_prefix, cout); 
  (position_list[i]).transport(prefix);
  cout << prefix.value << "Position transported.\n";

  keep_original.enter(String("whether you want to keep the original position"), new_prefix);
  if(keep_original.value == 'y'){
    position_list.insert(position_list.begin()+i, original_position);
  }

}

// if zoom_out = true, then I delete boundary.txt and make a fresh plot with the boundaries in init file
void Plot::show(bool zoom_out, String prefix){

  stringstream line_ins, /*plot_style contains the chunk of the gnuplot command line containing the style (dashtype, line type, color, ...) of the Routes and Position to be plotted*/plot_style, /*plot_title contains the gnuplot title of the Route to be plotted*/ plot_title;
  string line;
  unsigned int i;
  //t_p(m) are the larger (smaller) value of t where the circle of equal altitude crosses the meridian lambda = pi. 
  Angle t_min, t_max, t_p, t_m, t_s, phi_min, phi_max, lambda_min, lambda_max;
  Position p_min, p_max;
  int status, iter = 0;
  //x_hi(lo)_p(m) are the higher and lower bound of the interval where I will look for t_p(m)
  double x, x_lo_p, x_lo_m, x_hi_p, x_hi_m, x_lo_s, x_hi_s;
  gsl_function F;
  const gsl_root_fsolver_type *T;
  gsl_root_fsolver *s;
  Int n_points_plot_coastline, width_plot_window, height_plot_window, n_points_routes, n_intervals_tics;
  String new_prefix;
  File file_init;


  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  file_init.set_name(String(path_file_init));
  file_init.open(String("in"), prefix);

  
  T = gsl_root_fsolver_brent;
  s = gsl_root_fsolver_alloc (T);


  
  //if job_id = -1 this means that there is no gnuplot script running in the background, thus there is no need to stop it. Otherwise, the gnuplot script running in the background is stopped. 
  if(job_id != -1){
    
    command.str("");
    command << "kill -9 " << job_id;
    system(command.str().c_str());
    
  }

  file_id.remove(prefix);
  file_gnuplot.remove(prefix);

  //replace line with number of intervals for tics in plot_dummy.plt
  cout << prefix.value << YELLOW << "Reading number of intervals for tics from file " << file_init.name.value << " ...\n" << RESET;
  plot_command.str("");
  command.str("");
  n_intervals_tics.read_from_file(String("number of intervals for tics"), file_init, true, new_prefix); 
  command << "LANG=C sed 's/#number of intervals for tics/n_intervals_tics = " << n_intervals_tics.value << ";/g' plot_dummy.plt >> plot_temp.plt \n";
  system(command.str().c_str());
  cout << prefix.value << YELLOW << "... done.\n" << RESET;

 
  //replace line with number of points for routes in plot_dummy.plt
  cout << prefix.value << YELLOW << "Reading number of points for routes from file " << file_init.name.value << " ...\n" << RESET;
  plot_command.str("");
  command.str("");
  n_points_routes.read_from_file(String("number of points for routes"), file_init, true, new_prefix); 
  command << "LANG=C sed 's/#number of points for routes/n_points_routes = " << n_points_routes.value << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
  system(command.str().c_str());
  cout << prefix.value << YELLOW << "... done.\n" << RESET;

  
  //replace line with n_points_plot_coastline in plot_dummy.plt
  cout << prefix.value << YELLOW << "Reading number of points coastline from file " << file_init.name.value << " ...\n" << RESET;
  plot_command.str("");
  command.str("");
  n_points_plot_coastline.read_from_file(String("number of points coastline"), file_init, true, new_prefix); 
  command << "LANG=C sed 's/#n_points_coastline/n_points_coastline = " << n_points_plot_coastline.value << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
  system(command.str().c_str());
  cout << prefix.value << YELLOW << "... done.\n" << RESET;


  //read from init_file with and height of plot window
  cout << prefix.value << YELLOW << "Reading width and height of plot window from file " << file_init.name.value << " ...\n" << RESET;
  width_plot_window.read_from_file(String("width of plot window"), file_init, true, new_prefix); 
  height_plot_window.read_from_file(String("height of plot window"), file_init, true, new_prefix); 
  cout << prefix.value << YELLOW << "... done.\n" << RESET;

  
  //replace line with min_latitude in plot_dummy.plt
  //

  if((!file_boundary.check_if_exists(prefix)) || zoom_out){
    //in this case, there is no boundary file boundary.txt ot zoom_out: a plot is made for the frist time or the boundaries need to be reset to the ones from the init file -> the boundaries of the plot are thus read from the init file. 

    if(!file_boundary.check_if_exists(prefix)){
      
      cout << prefix.value << YELLOW << "I found no boundary file.\n" << RESET;
      
    }else{
      
      cout << prefix.value  << "Fully zooming out.\n";

      if(zoom_out){
	file_boundary.remove(prefix);
      }

    }
    
    cout << prefix.value << YELLOW << "Reading minimal and maximal latitude and longitude from file " << file_init.name.value << " ...\n" << RESET;
  
    lambda_min.read_from_file(String("minimal longitude"), file_init, true, new_prefix); 
    lambda_max.read_from_file(String("maximal longitude"), file_init, true, new_prefix); 
    phi_min.read_from_file(String("minimal latitude"), file_init, true, new_prefix); 
    phi_max.read_from_file(String("maximal latitude"), file_init, true, new_prefix);

    cout << prefix.value << YELLOW << "... done.\n" << RESET;


  }else{
    //in this case, there is a boundary file boundary.txt: a plot has been already made before, and its boudaries are stored in the boudnary file > the boundaries of the plot are thus read from this boundary file so as to keep the same plotting window.

    cout << new_prefix.value << "I found a boundary file.\n" << RESET;

    file_boundary.open(String("in"), prefix);
    
    lambda_min.read_from_file(String("minimal longitude"), file_boundary, true, new_prefix); 
    lambda_max.read_from_file(String("maximal longitude"), file_boundary, true, new_prefix); 
    phi_min.read_from_file(String("minimal latitude"), file_boundary, true, new_prefix); 
    phi_max.read_from_file(String("maximal latitude"), file_boundary, true, new_prefix);
    
    file_boundary.close(prefix);
  }

  //

  //in either case of the if above, I write the boundary values which I have read in plot_temp.plt
  command.str("");
  command << "LANG=C sed 's/#min_longitude/lambda_min = " << (K*lambda_min.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
  command << "LANG=C sed 's/#max_longitude/lambda_max = " << (K*lambda_max.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
  command << "LANG=C sed 's/#min_latitude/phi_min = " << (K*phi_min.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
  command << "LANG=C sed 's/#max_latitude/phi_max = " << (K*phi_max.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
  
  system(command.str().c_str());


  
  //plot routes
  
  plot_command.str("");
  command.str("");
  //set the key position on the screen
  plot_command << "set key top right\\\n";
  for(i=0; i<(route_list.size()); i++){

    //I start a brand new multiline plot command only if I am looking at the first route plot
    if(i==0){
      plot_command << "plot ";
    }
    
    //set a  plot style for "error on astronomical position curve" and another one for all other curves
    plot_style.str("");
    if(((route_list[i]).label) == String("error on astronomical position")){
      plot_style << "w l dashtype " << i+1 << " linecolor rgb \"purple\"";
    }else{
      plot_style << "w l dashtype " << i+1 << " lt " << i+1;
    }

    switch(((route_list[i]).type.value)[0]){

    case 'l':
      //plot a loxodrome
      {

	//I assume that  the loxodrome is not cut through the meridian lambda = M_PI, and I make a single plot
	plot_command << "[0.:" << (route_list[i]).l.value << "] \"+\" u (xe(K*lambda_lox(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) : (ye(K*phi_lox(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) " << plot_style.str()  << " ti \"type = " << (route_list[i]).type.value << ", start = " << (route_list[i]).start.to_string(display_precision).str().c_str() << ", heading = " << (route_list[i]).alpha.to_string(display_precision).str().c_str() << "\"";

	break;
      }

    case 'o':
      //plot an orthodrome
      {

	//I assume that the orthordrome is not cut through the meridian lambda = M_PI, and I make a single plot
	plot_command << "[0.:" << (route_list[i]).l.value << "] \"+\" u (xe(K*lambda_ort(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) : (ye(K*phi_ort(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) " << plot_style.str()  << " ti \"type = " << (route_list[i]).type.value << ", start = " << (route_list[i]).start.to_string(display_precision).str().c_str() << ", heading = " << (route_list[i]).alpha.to_string(display_precision).str().c_str() << "\"";

	break;
      }


    case 'c':
      //plot a circle of equal altitude
      {

	//this is the title for all curves which will be plotted with a non-empty title in their legent. Curves plotted with an empty title in their legend will be plotted with 'noti' in the plot command. 
	plot_title.str("");
	if(!((route_list[i]).label == String("error on astronomical position"))){
	  plot_title << " ti \"" << (route_list[i]).label.value << "\"";
	}else{
	  plot_title << " noti";
	}
	
	//if abs(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))) < 1.0, then there exists a value of t = t_{max} (t_{min}) such that (route_list[i]).GP.lambda vs. t has a maximum (minimum). In this case, I proceed and compute this maximum and minimum, and see whether the interval [(route_list[i]).GP.lambda_{t = t_{min}} and (route_list[i]).GP.lambda_{t = t_{max}}] embraces lambda = \pi. If it does, I modify the gnuplot command so as to avoid the horizontal line in the graph output. 
	if(abs(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))) < 1.0){
    
	  //compute the values of the parametric Angle t, t_min and t_max, which yield the position with the largest and smallest longitude (p_max and p_min) on the circle of equal altitude 
	  t_max.set(String(""), acos(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))), new_prefix);
	  t_min.set(String(""), 2.0*M_PI - acos(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))), new_prefix);

	  //p_max =  circle of equal altitude computed at t_max
	  ((route_list[i]).l.value) = Re * sin(((route_list[i]).omega.value)) * (t_max.value);
	  (route_list[i]).compute_end(new_prefix);
	  p_max = ((route_list[i]).end);
	
	  ((route_list[i]).l.value) = Re * sin(((route_list[i]).omega.value)) * (t_min.value);
	  (route_list[i]).compute_end(new_prefix);
	  p_min = ((route_list[i]).end);
	  //p_min =  circle of equal altitude computed at t_min

	  /* p_max.print(String("p_max"), new_prefix, cout); */
	  /* p_min.print(String("p_min"), new_prefix, cout); */

	  if((p_max.lambda.value < M_PI) && (p_min.lambda.value > M_PI)){
	    cout << prefix.value << YELLOW << "Circle of equal altitude is cut!\n" << RESET;
	    //in this case, the circle of equal altitude is cut through the meridian lambda = M_PI

	    if((route_list[i]).GP.lambda.value > M_PI){
	      //in this case, the two values of t, t_p and t_m, at which the circle of equal altitude intersects the meridian lambda = M_PI, lie in the interval [0,M_PI]

	      cout << prefix.value << "Case I:\n";

	      // interval where I know that there will be t_p
	      x_lo_p = (t_max.value);
	      x_hi_p = M_PI;

	      //interval where I know that there will be t_m
	      x_lo_m = 0.0;
	      x_hi_m = (t_max.value);

	    }else{
	      //in this case, the two values of t, t_p and t_m, at which the circle of equal altitude intersects the meridian lambda = M_PI, lie in the interval [M_PI,2*M_PI]
	      //here I select an interval where I know that there will be t_m

	      cout << prefix.value << "Case II:\n";

	      // interval where I know that there will be t_p
	      x_lo_p = (t_min.value);
	      x_hi_p = 2.0*M_PI;

	      //interval where I know that there will be t_m
	      x_lo_m = M_PI;
	      x_hi_m = (t_min.value);

	    }

	    (route_list[i]).temp_prefix = prefix;
	    F.params = &(route_list[i]);
	    F.function = &((route_list[i]).lambda_minus_pi);



	    //solve for t_p
      
	    gsl_root_fsolver_set(s, &F, x_lo_p, x_hi_p);

	    cout << prefix.value << "Extreme values = " << GSL_FN_EVAL(&F,x_lo_p) << " " << GSL_FN_EVAL(&F,x_hi_p) << "\n";
          
	    cout << prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
	    cout << new_prefix.value << "iter" <<  " [lower" <<  ", upper] " <<  "root " << "err(est)\n";

	    iter = 0;
	    do{
      
	      iter++;
	      status = gsl_root_fsolver_iterate(s);
      
	      x = gsl_root_fsolver_root(s);
	      x_lo_p = gsl_root_fsolver_x_lower(s);
	      x_hi_p = gsl_root_fsolver_x_upper(s);
	      status = gsl_root_test_interval(x_lo_p, x_hi_p, 0.0, epsrel);
	      if(status == GSL_SUCCESS){
		cout << new_prefix.value << "Converged:\n";
	      }
	      cout << new_prefix.value << iter << " [" << x_lo_p << ", " << x_hi_p << "] " << x << " " << x_hi_p-x_lo_p << "\n";
	    }
	    while((status == GSL_CONTINUE) && (iter < max_iter));

	    t_p.value = (x_lo_p+x_hi_p)/2.0;
	    t_p.print(String("t_+"), new_prefix, cout);



      

	    //solve for t_m
      
	    gsl_root_fsolver_set(s, &F, x_lo_m, x_hi_m);

	    cout << prefix.value << "Extreme values = " << GSL_FN_EVAL(&F,x_lo_m) << " " << GSL_FN_EVAL(&F,x_hi_m) << "\n";
          
	    cout << prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
	    cout << new_prefix.value << "iter" <<  " [lower" <<  ", upper] " <<  "root " << "err(est)\n";

	    iter = 0;
	    do{
      
	      iter++;
	      status = gsl_root_fsolver_iterate(s);
      
	      x = gsl_root_fsolver_root(s);
	      x_lo_m = gsl_root_fsolver_x_lower(s);
	      x_hi_m = gsl_root_fsolver_x_upper(s);
	      status = gsl_root_test_interval(x_lo_m, x_hi_m, 0.0, epsrel);
	      if(status == GSL_SUCCESS){
		cout << new_prefix.value << "Converged:\n";
	      }
	      cout << new_prefix.value << iter << " [" << x_lo_m << ", " << x_hi_m << "] " << x << " " << x_hi_m-x_lo_m << "\n";
	    }
	    while((status == GSL_CONTINUE) && (iter < max_iter));

	    t_m.value = (x_lo_m+x_hi_m)/2.0;
	    t_m.print(String("t_-"), new_prefix, cout);

	    //the  - epsilon is added because in plot_dummy.plt lambda_min = 180.0 - epsilon. If one does not include this - epsilon, then the last part of the curve goest to the other edge of the plot and a horizontal line appears. Similarly for the - and + epsilon below
      
	    plot_command << "[0.:" << t_m.value << " - epsilon] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << plot_title.str() << ",\\\\\\\n";
	    //maybe wrong
	    plot_command << "[" << t_m.value << " + epsilon:" << t_p.value << " - epsilon] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << " noti,\\\\\\\n";
	    //maybe wrong
	    plot_command << "[" << t_p.value << " + epsilon:2.*pi] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << " noti";

	  }else{
	    //in this case, the circle of equal altitude is not cut through the meridian lambda = M_PI, and I make a single plot

	    plot_command << "[0.:2.*pi] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << plot_title.str();

	  }
      
	}else{
	  //in this case (route_list[i]).GP.lambda.value is a monotonically increasing function of t: I find the value of t = t_s such that (route_list[i]).GP.lambda.value = M_PI and split the gnuplot plot  in two plots so as to avoid the horizontal line

	  // interval where I know that there will be t_s
	  if((-sin(((route_list[i]).omega.value))/cos(((route_list[i]).GP.phi.value) - (((route_list[i]).omega.value)))) > 0.0){
	    //in this case lambda'(t = 0) > 0.0 -> lambda'(t) > 0.0  for all t
	    if((route_list[i]).GP.lambda.value < M_PI){
	      //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to 0.0 <= t< M_PI
	      x_lo_s = 0.0;
	      x_hi_s = M_PI;
	    }else{
	      //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to M_PI <= t< 2*M_PI
	      x_lo_s = M_PI;
	      x_hi_s = 2.0*M_PI;
	    }
	  }else{
	    //in this case lambda'(t = 0) < 0.0 -> lambda'(t) < 0.0  for all t
	    if((route_list[i]).GP.lambda.value < M_PI){
	      //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to M_PI <= t< 2*M_PI
	      x_lo_s = M_PI;
	      x_hi_s = 2.0*M_PI;
	    }else{
	      //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to 0.0 <= t< M_PI
	      x_lo_s = 0.0;
	      x_hi_s = M_PI;
	    }

	  }

	  (route_list[i]).temp_prefix = prefix;
	  F.params = &(route_list[i]);
	  F.function = &((route_list[i]).lambda_minus_pi);

	  //solve for t_s
      
	  gsl_root_fsolver_set(s, &F, x_lo_s, x_hi_s);

	  cout << prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
	  cout << new_prefix.value << "iter" <<  " [lower" <<  ", upper] " <<  "root " << "err(est)\n";

	  iter = 0;
	  do{
      
	    iter++;
	    status = gsl_root_fsolver_iterate(s);
      
	    x = gsl_root_fsolver_root(s);
	    x_lo_s = gsl_root_fsolver_x_lower(s);
	    x_hi_s = gsl_root_fsolver_x_upper(s);
	    status = gsl_root_test_interval(x_lo_s, x_hi_s, 0.0, epsrel);
	    if(status == GSL_SUCCESS){
	      cout << new_prefix.value << "Converged:\n";
	    }
	    cout << new_prefix.value << iter << " [" << x_lo_s << ", " << x_hi_s << "] " << x << " " << x_hi_s-x_lo_s << "\n";
	  }
	  while((status == GSL_CONTINUE) && (iter < max_iter));

	  t_s.value = (x_lo_s+x_hi_s)/2.0;
	  t_s.print(String("t_*"), new_prefix, cout);


	  //the  - epsilon is added because in plot_dummy.plt lambda_min = 180.0 - epsilon. If one does not include this - epsilon, then the last part of the curve goest to the other edge of the plot and a horizontal line appears. Similarly for the - and + epsilon below
      
	  plot_command << "[0.:" << t_s.value << " - epsilon] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << plot_title.str() << " ,\\\\\\\n";

	  plot_command << "[" << t_s.value << " + epsilon:2.*pi] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << " noti";
	  
	}

      }
    
    }

    //if there is a route or a position plot after the one that I just added, then I add a ,\ for it
    if((i+1<route_list.size()) || ((i+1 == route_list.size()) && (position_list.size() > 0))){
      
      plot_command << ",\\\\";
      
    }

    //if I did not reach the last route plot, I add a newline. If I reached the last plot, no need to add a newline because the sed replacement commands will do it
    if(i+1<route_list.size()){
      
      plot_command << "\\\n";
      
    }
    
  }
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "LANG=C sed 's/#route_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";


  
  //plot positions
  
  plot_command.str("");
  for(i=0; i<position_list.size(); i++){

    //I start a brand new multiling plotting command only if I am looking at the first position plot and if there were no previous route plots
    if((i==0) && (route_list.size() == 0)){
      plot_command << "plot ";
    }

    //set a  plot style for "astronomical position" Position and another one for all other Positions
    plot_style.str("");
    if(!(((position_list[i]).label) == String("crossing"))){

      if(!(((position_list[i]).label) == String("astronomical position"))){

	plot_style << "lt " << i+1 << " ti \"" << (position_list[i]).label.value << "\"";
  
      }else{

	plot_style << "linecolor rgb \"purple\" " << " ti \"" << (position_list[i]).label.value << "\"";
  
      }

      
    }else{
      
      plot_style << "lt " << 1 << "linecolor rgb \"gray\" noti";
      
    }


    plot_command << "\"+\" u (xe(K*(" << (position_list[i]).lambda.value << "))):(ye(K*(" << (position_list[i]).phi.value << "))) w p lw 2 " << plot_style.str();




      

    if(i+1<position_list.size()){

      plot_command << ",\\\\";
      
    }

    plot_command << "\\\n";
    
  }
  
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "LANG=C sed 's/#position_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> " << ((file_gnuplot.name).value) << "\n";


  //add the overall plotting command to command string
  command << "gnuplot -geometry " << (width_plot_window.value) << "x" << (height_plot_window.value) << " '" << ((file_gnuplot.name).value) << "' & \n echo $! >> " << ((file_id.name).value) << "\n";
  command << "rm -rf plot_temp.plt";

  
  //execute the command string
  system(command.str().c_str());

  //read the job id from file_id
  if(file_id.open(String("in"), String("\t"))){
    getline(file_id.value, line);
    line_ins << line;
    line_ins >> job_id;
  }

  file_id.close(prefix);
  file_id.remove(prefix);
  file_init.close(prefix);

  cout << prefix.value << "Job id = "<< job_id << "\n";

}

bool Sight::enter(Catalog catalog, String name, String prefix){

  //pointer to init.txt to read fixed sight data from in there
  File file_init;
  String new_prefix;
  bool check = true;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  file_init.set_name(String(path_file_init));
  check &= (file_init.open(String("in"), prefix));
  
  cout << prefix.value << "Enter " << name.value << ":\n";
  
  body.enter(catalog, new_prefix);
  //GP.label.set("geographic position", new_prefix);

  if(body.type.value != "star"){
    limb.enter(String("limb"), new_prefix);
  }
  //here I assume that the sextant altitude is positive: if you want to trop this, true -> false
  H_s.enter(String("sextant altitude"), true, new_prefix);
  //read index error from init file
  cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
  index_error.read_from_file(String("index error"), file_init, true, new_prefix);
  cout << prefix.value << YELLOW << "... done.\n" << RESET;
  cout << prefix.value << YELLOW << "Reading artificial horizon from file " << file_init.name.value << " ...\n" << RESET;
  artificial_horizon.read_from_file(String("artificial horizon"), file_init, true, new_prefix);
  cout << prefix.value << YELLOW << "... done.\n" << RESET;
  if(artificial_horizon.value == 'n'){
    height_of_eye.enter(String("height of eye"), String("m"), new_prefix);
  }
  
  do{
  
    master_clock_date_and_hour.enter(String("master-clock date and hour of sight"), new_prefix);
    time = master_clock_date_and_hour;
    
    cout << prefix.value << YELLOW << "Reading use of stopwatch from file " << file_init.name.value << " ...\n" << RESET;
    use_stopwatch.read_from_file(String("use of stopwatch"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    if(use_stopwatch.value == 'y'){
        
      stopwatch.enter(String("stopwatch reading"), new_prefix);
      time.add(stopwatch);
    
    }

    //read TAI_minus_UTC from data/index.txt
    cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
    TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    time.add(TAI_minus_UTC);
    time.print(String("TAI date and hour of sight"), new_prefix, cout);

  }while(!check_data_time_interval(prefix));

  label.enter(String("label"), new_prefix);

  //given that the sight is not yet linke to a route, I set
  related_route = -1;
  
  file_init.close(prefix);


  if(!check){
    cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
  }

  return check;

}

bool Sight::reduce(Route* circle_of_equal_altitude, String prefix){

  bool check = true;
  String new_prefix;
  stringstream temp;

  temp.clear();

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  ((*circle_of_equal_altitude).type.value) = 'c';
  
  compute_H_a(new_prefix);
  check &= get_coordinates(circle_of_equal_altitude, new_prefix);

  //link the circle of equal altitude (*circle_of_equal_altitude) to sight (*this)
  temp <<  (*this).body.name.value << " " << (*this).time.to_string(display_precision).str().c_str() << " TAI, " << (*this).label.value;
  ((*circle_of_equal_altitude).label).set(String(""), String(temp.str()), new_prefix);
   
  check &= compute_H_o(new_prefix);
  ((*circle_of_equal_altitude).omega.value) = M_PI/2.0 - (H_o.value);

  if(!check){
    
    cout << prefix.value << RED << "Sight cannot be reduced!\n" << RESET;
    
  }

  return check;
  
}


void Sight::compute_H_a(String prefix){
  
  if(artificial_horizon.value == 'y'){
    H_a = (H_s-index_error)/2.0;
    H_a.print(String("apparent altitude"), prefix, cout);

  }else{
    compute_DH_dip(prefix);
    H_a = H_s-index_error+DH_dip;
    H_a.print(String("apparent altitude"), prefix, cout);
  }
  
}


bool Sight::compute_H_o(String prefix){

  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  check &= compute_DH_refraction(new_prefix);

  if(check){
    compute_DH_parallax_and_limb(new_prefix);
    H_o = H_a + DH_refraction + DH_parallax_and_limb;
    H_o.print(String("observed altitude"), new_prefix, cout);
  }else{
    cout << prefix.value << RED << "H_o cannot be computed!\n" << RESET;
  }

  return check;
  
}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(String prefix){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  H_i = H_a + DH_refraction;
  H_i.print(String("intermediate altitude"), prefix, cout);

  if(body.type.value != "star"){

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
 
	cout << new_prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
	
	cout << new_prefix.value << "iter" <<  "[lower" <<  ", upper] " <<  "root " << "err(est)\n";
	
	iter = 0;
	do{
      
	  iter++;
	  status = gsl_root_fsolver_iterate (s);
      
	  x = gsl_root_fsolver_root(s);
	  x_lo = gsl_root_fsolver_x_lower(s);
	  x_hi = gsl_root_fsolver_x_upper(s);
	  status = gsl_root_test_interval (x_lo, x_hi, 0.0, epsrel);
	  if(status == GSL_SUCCESS){
	    cout << new_prefix.value << "Converged.\n";
	  }
	  cout << new_prefix.value << iter << " [" << x_lo << ", " << x_hi << "] " << x << " " << x_hi-x_lo << "\n";

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

    DH_parallax_and_limb.print(String("parallax and limb correction"), prefix, cout);

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

//this function returns the longitude lambda of a circle of equal altitude (*this) - pi
double Route::lambda_minus_pi(double t, void* route){

  Route* r = (Route*)route;
  String new_prefix;

  //append \t to prefix
  new_prefix = ((*r).temp_prefix).append(String("\t"));

  ((*r).l.value) = Re * sin(((*r).omega.value)) * t;
  (*r).compute_end(new_prefix);

  return(((*r).end.lambda.value) - M_PI);

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
  earth_radius.value = Re;

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



void Body::print(String name_in, String prefix, ostream& ostr){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  ostr << prefix.value << name_in.value << ":\n";
  
  ostr << new_prefix.value << "Type = " << type.value << "\n";
  ostr << new_prefix.value << "Name = " << name.value << "\n";
  
  if(type.value == "star"){
    RA.print(String("Right ascension"), new_prefix, ostr);
    d.print(String("Declination"), new_prefix, ostr);
  }else{
    radius.print(String("Radius"), String("nm"), new_prefix, ostr);
  }
 
}


void Body::enter(Catalog catalog, String prefix){

  unsigned int i;
  bool check;
  string s;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  
  do{

    s.clear();
    
    cout << prefix.value << "Enter name of body:";
    getline(cin >> ws, s);

    for(i=0, check=true; (i<(catalog).list.size()) && check; i++){if((((catalog).list)[i]).name.value == s){check=false;}}
    if(check){cout << prefix.value << RED << "Body not found in catalog!\n" << RESET;}
      
  }while(check);
  
  i--;
  (*this) = (catalog.list)[i];
  print(String("body"), prefix, cout);
  
}






Sight::Sight(void){

  items = {String("body"), String("limb"), String("sextant altitude"), String("time"), String("label"), String("related route")};

  atmosphere.set();
  related_route = -1;
  
}

//this function simplifies the atmosphere between z=0 and z=eight of eye as a single layer, where within this layer the index of refracion is independent of z. Refine this in the future. 
void Sight::compute_DH_dip(String prefix){

  Length zero_Length;
  zero_Length.value = 0.0;

  DH_dip.set(String("Dip correction"),
	     -acos( atmosphere.n(zero_Length)/atmosphere.n(height_of_eye)*((atmosphere.earth_radius.value)/((atmosphere.earth_radius.value)+(height_of_eye.value)) ) ), prefix);

}


bool Sight::compute_DH_refraction(String prefix){

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
    DH_refraction.set(String("refraction correction"), result, prefix);
  }else{
    check &= false;
    cout << prefix.value << RED << "GSL integration failed!\n" << RESET;
  }
  
  gsl_integration_workspace_free(w);
  
  return check;
  
}

void Length::set(String name, double x, String prefix){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  value = x;

  print(name, String("nm"), prefix, cout);
  check_valid(name, new_prefix);
  
}

//enter a length in meters
void Length::enter(String name, String unit, String prefix){

  stringstream temp;

  temp.clear();
  temp << name.value;
  if(unit.value == "nm"){
    temp  << " [nm]";
  }else{
    temp << " [m]";
  }

  do{
    
    enter_double(&value, false, 0.0, 0.0, temp.str(), prefix);
    
  }while(!check_valid(name, prefix));

  //if the length has been entered in units of m, convert it to nautical miles
  if(unit.value == "m"){
    value/=(1e3*nm);
  }
  
  print(name, unit, prefix, cout); 
  
}

void Length::print(String name, String unit, String prefix, ostream& ostr){

  if((name.value) != ""){
  
    ostr << prefix.value << name.value << " = ";
    if(unit.value == "nm"){
      ostr << value << " nm\n";
    }else{
      ostr << value*nm*1e3 << " m\n";
    }

  }
 
}

bool Sight::get_coordinates(Route* circle_of_equal_altitude, String prefix){

  File file;
  stringstream filename, line_ins;
  string line, dummy, temp;
  int l, l_min, l_max;
  double MJD_tab[(unsigned int)N], GHA_tab[(unsigned int)N], d_tab[(unsigned int)N], sum;
  gsl_interp_accel* acc = gsl_interp_accel_alloc ();
  gsl_spline *interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)), *interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));
  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  if((body.type.value) != "star"){
    filename << "data/" << body.name.value << ".txt";
  }else{
    filename << "data/j2000_to_itrf93.txt";
  }  
  temp = filename.str();

  
  file.set_name(String(temp.c_str())); 
  if(file.open(String("in"), new_prefix)){

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


    if((body.type.value) != "star"){
      //in this case I am getting the coordinate of a body with a non-zero size

      //if the body is not a star

      double r_tab[(unsigned int)N];
      gsl_spline *interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));
     
      for(; l<l_max; l++){
	
	line.clear();
	line_ins.clear();
	
	getline((file.value), line);
	line_ins << line;
	cout << new_prefix.value << line << "\n";
	line_ins >> dummy >> dummy >> dummy >> GHA_tab[l-l_min] >> d_tab[l-l_min] >> r_tab[l-l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;
	
	MJD_tab[l-l_min] = ((double)(l-l_min))/L;
	
      }

      file.close(new_prefix);

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

  
      cout << new_prefix.value << "Read values:\n";
      for(l=0; l<N; l++){
	cout << new_prefix.value << MJD_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
      }

      if(gsl_spline_eval_e(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((((*circle_of_equal_altitude).GP).lambda).value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	(((*circle_of_equal_altitude).GP).lambda).normalize();
	(((*circle_of_equal_altitude).GP).lambda).print(String("GHA"), new_prefix, cout);
      }	
      //(((*circle_of_equal_altitude).GP).lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);


      if(gsl_spline_eval_e(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((((*circle_of_equal_altitude).GP).phi).value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	(((*circle_of_equal_altitude).GP).phi).normalize();
	(((*circle_of_equal_altitude).GP).phi).print(String("d"), new_prefix, cout);
      }	
      //(((*circle_of_equal_altitude).GP).phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

      if(gsl_spline_eval_e(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc, &(r.value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	if((r.check_valid(String("r"), new_prefix))){
	  r.print(String("r"), String("nm"), new_prefix, cout);
	}else{
	  check &= false; 
	}
      }
      //r.set("r", gsl_spline_eval(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

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
	cout << new_prefix.value << line << "\n";
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

      file.close(new_prefix);

 

      //remove discontinuous jumps in GHA to allow for interpolation
      for(sum=0.0, l=0; l<N-1; l++){
	//cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
	if(((GHA_tab[l]-sum) > M_PI) && (GHA_tab[l+1] < M_PI)){
	  sum += 2.0*M_PI;
	}
	GHA_tab[l+1] += sum;
      }

      cout << new_prefix.value << "Read values:\n";
      for(l=0; l<N; l++){
	cout << new_prefix.value << MJD_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
      }

      if(gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;}
      if(gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS){check &= false;}

      
      if(gsl_spline_eval_e(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((((*circle_of_equal_altitude).GP).lambda).value)) != GSL_SUCCESS){
	check &= false;
      }else{
	(((*circle_of_equal_altitude).GP).lambda).normalize();
	(((*circle_of_equal_altitude).GP).lambda).print(String("GHA"), new_prefix, cout);
      }

      if(gsl_spline_eval_e(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((((*circle_of_equal_altitude).GP).phi).value)) != GSL_SUCCESS){
	check &= false;
      }else{
	(((*circle_of_equal_altitude).GP).phi).normalize();
	(((*circle_of_equal_altitude).GP).phi).print(String("d"), new_prefix, cout);
      }

    }

  }else{
    check &= false;
  }

  if(!check){
    cout << prefix.value << RED << "Cannot obtain coordinates!\n" << RESET;
  }
  
  gsl_interp_accel_free(acc);
  gsl_spline_free(interpolation_GHA);
  gsl_spline_free(interpolation_d);

  return check;
  
}

void Angle::set(String name, double x, String prefix){

  value = x;
  normalize();
  if(name.value != ""){print(name, prefix, cout);}
  
}

void Angle::enter(String name, bool is_positive, String prefix){

  unsigned int ad;
  double am;
  String new_prefix;
  string input;
  stringstream input_unit;
  size_t pos;
  bool check;
  char s;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  

  if(is_positive){
    //in this case the angle is positive defined and there is no '+' or '-' sign in front of it

    input_unit.clear();
    input_unit << name.value << " [ddd mm.m]";

    do{
  
      check = true;

      //set the sign
      s = '+';

      //enter the input string, containing degrees and arcminutes with a ' ' in between
      cout << new_prefix.value << "Enter " << input_unit.str() << ":";
      getline(cin >> ws, input);
      
      //check whether the string contains only one ' '
      check &= (count((input).begin(), (input).end(), ' ') == 1);

      if(check){
 
	pos = (input).find(" ");

	//check whether degree part is formatted correctly
	check &= check_unsigned_int((input).substr(0, pos).c_str(), NULL, true, 0, 360);
      
	//check whether arcminute part is formatted correctly
	check &= check_double((input).substr(pos+1, (input).size()-pos).c_str(), NULL, true, 0.0, 60.0);
      
      }
    
      if(!check){
	cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
      }

    }while(!check);

  }else{
    //in this case the angle is not positive defined and there is a '+' or '-' sign in front of it

    input_unit.clear();
    input_unit << name.value << " [s ddd mm.m]";

    do{
  
      check = true;

      //enter the input string, containing degrees and arcminutes with a ' ' in between
      cout << new_prefix.value << "Enter " << input_unit.str() << ":";
      getline(cin >> ws, input);

      //check whether the string contains only two ' ', one after the sign, one adter the degrees
      check &= (count((input).begin(), (input).end(), ' ') == 2);

      if(check){

	//find the position of the first ' ' 
	pos = (input).find(" ");

	//check whether the sign is formatted correctly
	check &= ( ( ((input).substr(0, pos).c_str())[0] == '+' ) || ( ((input).substr(0, pos).c_str())[0] == '-' ) );

	//set the sign
	s = ((input).substr(0, pos).c_str())[0];  

	//now I am no longer interested in the sign, the string runs from the degrees to the end of arcminutes
	(input)  =  ((input).substr(pos+1).c_str());

	//find the position of the second ' ' 
	pos = (input).find(" ");

	//check whether degree part is formatted correctly
	check &= check_unsigned_int((input).substr(0, pos).c_str(), NULL, true, 0, 360);
      
	//check whether arcminute part is formatted correctly
	check &= check_double((input).substr(pos+1, (input).size()-pos).c_str(), NULL, true, 0.0, 60.0);
      
      }
    
      if(!check){
	cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
      }

    }while(!check);

  }

  //set degrees and arcminutes
  ad = stoi((input).substr(0, pos).c_str(), NULL, 10);
  am = stod((input).substr(pos+1, (input).size()-pos).c_str());
  
  value = k*(((double)ad) + am/60.0);
  if(s == '-'){
    value *= -1.0;
  }
  
  normalize();
  
  print(name, prefix, cout);

}

void Position::enter(String name, String prefix){

  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter " << name.value << ":\n";

  do{
    phi.enter(String("latitude"), false, new_prefix);
    if(!(((0.0 <= phi.value) && (M_PI/2.0 >= phi.value)) || ((3.0*M_PI/2.0 <= phi.value) && (2.0*M_PI >= phi.value)))){
      cout << new_prefix.value << RED << "Entered value is not valid!\n" << RESET;
      check = true;
    }else{
      check = false;
    }
  }while(check);
  
  lambda.enter(String("longitude"), false, new_prefix);
  label.enter(String("label"), new_prefix);
  
}

void Position::print(String name, String prefix, ostream& ostr){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  ostr << prefix.value << name.value << ":\n";

  phi.print(String("latitude"), new_prefix, ostr);
  lambda.print(String("longitude"), new_prefix, ostr);

  label.print(String("label"), new_prefix, ostr);

}


void Angle::normalize(void){

  value = value - 2.0*M_PI*floor(value/(2.0*M_PI));

}


void Angle::print(String name, String prefix, ostream& ostr){

  normalize();
  ostr << prefix.value << name.value << " = " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'\n";

}

stringstream Angle::to_string(unsigned int precision){

  stringstream output;
  
  output.precision(precision);

  normalize();
  output << floor(K*value - 360.0*floor(K*value/360.0)) << "'\260' " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'\\''";

  return output;
  
}

void Limb::enter(String name, String prefix){

  bool check;

  do{
    cout << prefix.value << "Enter " << name.value << " [u/l/c]:";
    cin >> value;
    
    if((value=='u') || (value=='l') || (value=='c')){check = true;}
    else{
      cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
      check = false;
    }
  }while(!check);
  
  print(name, prefix, cout);
  
}


void Limb::print(String name, String prefix, ostream& ostr){

  ostr << prefix.value << name.value << " = " << value << "\n";
  
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

void Date::print(String name, String prefix, ostream& ostr){

  ostr << prefix.value << "date of " << name.value << " = " << to_string().str().c_str() << "\n";

};

void Date::enter(String name, String prefix) {

  string input;
  String new_prefix;
  bool check;
  size_t pos;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  
  do{

    check = true;

    cout << prefix.value << "Enter " << name.value << " [YYYY MM DD]:";
    getline(cin >> ws, input);

    //check whether the string contains two ' '
    check &= (count(input.begin(), input.end(), ' ') == 2);

    if(check){

      pos = (input).find(" ");

      //check whether year part is formatted correctly
      check &= check_unsigned_int(input.substr(0, pos).c_str(), &Y, false, 0, 0);

      if(check){
	//in this case, the year's format is valid

	//I check whether Y is a leap year and compute days_per_month
	check_leap_year();
	if(Y_is_leap_year){
	  
	  days_per_month = days_per_month_leap;
	  cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET;
	  
	}else{
	  
	  days_per_month = days_per_month_common;
	  cout << new_prefix.value << "Entered a common year\n";
	  
	}

	//now I am no longer interested in the year, the string runs from the month to days
	input  =  (input.substr(pos+1).c_str());

	//find the position of the second ' ' 
	pos = input.find(" ");

	//check whether month part is formatted correctly
	check &= check_unsigned_int(input.substr(0, pos).c_str(), &M, true, 1, 12+1);

	if(check){
	  //in this case the month part is formatted correctly
	
	  //now I am no longer interested in the month, the string runs from the days to the end of the string
	  input  =  (input.substr(pos+1).c_str());

	  //check whether day part is formatted correctly
	  check &= check_unsigned_int(input.c_str(), &D, true, 1, days_per_month[M-1]+1);

	}

      }

    }

    if(!check){
      
      cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
      
    }
    
  }while(!check);

 
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


stringstream Date::to_string(void){

  stringstream output;
  
  output << Y << "-";
  if(M<10){output << 0;}
  output << M << "-";
  if(D<10){output << 0;}
  output << D;

  return output;
  
}


void Chrono::print(String name, String prefix, ostream& ostr){

  unsigned int precision;

  //if I am printing to terminal, I print with display_precision. Otherwise, I print with data_precision
  if(ostr.rdbuf() == cout.rdbuf()){
    precision = display_precision;
  }else{
    precision = data_precision;
  }
  
  ostr << prefix.value << "hour of " << name.value << " = " << to_string(precision).str().c_str() << "\n";

};
void Chrono::enter(String name, String prefix) {

  String new_prefix;
  bool check;
  string input;
  size_t pos;


  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  do{

    check = true;
  
    cout << prefix.value << "Enter " << name.value << " [hh mm ss.s]:";
    getline(cin >> ws, input);

    //check whether the string contains two ' '
    check &= (count(input.begin(), input.end(), ' ') == 2);

    if(check){
      //in this case there are two ' ' in input, thus I can proceed checkin the other fields

      //find the position of the first ' '
      pos = (input).find(" ");

      //check whether hour part is formatted correctly
      check &= check_unsigned_int(input.substr(0, pos).c_str(), &h, true, 0, 24);

      if(check){
	//in this case, the hour's format is valid

	//now I am no longer interested in the hour, the string runs from the minutes to seconds
	input  =  (input.substr(pos+1).c_str());

	//find the position of the second ' ' 
	pos = input.find(" ");

	//check whether minute part is formatted correctly
	check &= check_unsigned_int(input.substr(0, pos).c_str(), &m, true, 0, 60);

	if(check){
	  //in this case the minute part is formatted correctly

	  //now I am no longer interested in the minutes, the string runs from the seconds to the end of the string
	  input  =  (input.substr(pos+1).c_str());

	  //check whether the part with seconds is formatted correctly
	  check &= check_double(input.c_str(), &s, true, 0.0, 60.0);
	  
	}

      }

    }

    if(!check){
      
      cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
      
    }

    
  }while(!check);

  //  enter_unsigned_int(&h, true, 0, 24, String("hh"), new_prefix);
  //enter_unsigned_int(&m, true, 0, 60, String("mm"), new_prefix);
  //enter_double(&s, true, 0.0, 60.0, String("ss.s"), new_prefix);
 
}





