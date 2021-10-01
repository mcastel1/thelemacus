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
//earth radius
#define Re (6371.0/nm)
#define RED     "\033[1;31m"    
#define YELLOW     "\033[1;33m"   
#define CYAN "\033[1;36m"      
#define RESET   "\033[0m"
//all possible chars that can enter in an unsigned integer
#define chars_unsigned_int "0123456789"
//all possible chars that can enter in a non-negative double
#define chars_double "0123456789."
#define days_per_month_leap {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
#define days_per_month_common {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}


//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal

class Catalog;
class File;

class String{

 public:
  string value;

  String();
  String(string);
  void enter(String, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, String);
  void set(String, String);
  String append(String);
  String prepend(String);

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
  void remove(void);
  void count_lines(String);
  
};


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
  
//this function asks the user to enter an unsigned int from keyboard and checks whether the entered value is an unsigned int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_unsigned_int(unsigned int* i, bool check_interval, unsigned int min, unsigned int sup, String name, String prefix){

  string s;
  bool check;

  do{
    
    s.clear();

    cout << prefix.value << "Enter " << name.value << ":";
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
      cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
    }
    
  }while(!check);

}


//this function asks the user to enter a double from keyboard and checks whether the entered value contains the allowed chars for double and, if check_interval = true, that the entered value lies in [min, sup)
void enter_double(double* x, bool check_interval, double min, double sup, String name, String prefix){

  string s;
  bool check;

  do{
    
    s.clear();

    cout << prefix.value << "Enter " << name.value << ":";
    getline(cin >> ws, s);

    if((/*here I check whether the quantity entered in s contains the allowed chars for double, i.e., it contains only the characters 0123456789.*/ ((s.find_first_not_of(chars_double)) == (std::string::npos))) && /*here I count whether the dot occurs zero or one time*/(count(s.begin(), s.end(), '.') <= 1)){

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
  void read_from_file(String, File&, String);
  bool check_valid(String, String);

};



String::String(){

  value = "";
  
}


String::String(string s){

  value = s;
  
}


File::File(){

  value.precision(data_precision);
  
}

void String::read_from_file(String name, File& file, String prefix){

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
  void enter(String, String);
  //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
  void print(String, String, ostream&);
  void read_from_file(String, File&, String);

};



void Answer::read_from_file(String name, File& file, String prefix){

  string line;
  size_t pos;

  line.clear();
  getline(file.value, line);
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

  ostr << prefix.value << name.value << " = " << value << "\n";
  
}

void String::set(String name, String prefix){

  value = name.value;
  print(name, prefix, cout);

}



class Angle{

 public:
 
  double value;
  void normalize(void);
  void enter(String, String);
  void set(String, double, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, String);
  stringstream to_string(unsigned int);

  Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&);
  
};



void Angle::read_from_file(String name, File& file, String prefix){

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
  
  void enter(String, String);
  void print(String, String, ostream&);
  void read_from_file(File&, String);
  void transport(String);

};

class Route{

 public:
  String type;
  //starting point of the route
  Point start, end;
  //the angle that the vector tangent to the route describes with the local meridian at start
  Angle alpha;
  //the length of the route
  Length l;

  void enter(String, String);
  void print(String, String, ostream&);
  void compute_end(String);
  
};

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


void Point::transport(String prefix){

  Route route;
  stringstream temp_label;
  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter route:\n";

  do{
    route.type.enter(String("type [l(=loxodrome)/o(=orthodrome)]"), new_prefix);
    check = ((route.type.value == "l") || (route.type.value == "o"));
    if(!check){
      cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
    }
  }while(!check);
  route.start = (*this); 
  route.alpha.enter(String("starting heading"), new_prefix);
  route.l.enter(String("length"), String("nm"), new_prefix);

  route.print(String("transport"), prefix, cout);
  
  route.compute_end(new_prefix);

  temp_label << label.value << " tr. w " << route.type.value << ", " << route.alpha.to_string(display_precision).str().c_str() << ", l = " << route.l.value << " nm";
  (route.end.label).set(temp_label.str(), prefix);

  (*this) = route.end;

  print(String("transported point"), prefix, cout);

}

void Point::read_from_file(File& file, String prefix){

  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  phi.read_from_file(String("latitude"), file, new_prefix);
  lambda.read_from_file(String("longitude"), file, new_prefix);
  label.read_from_file(String("label"), file, new_prefix);

}


//returns a point on the Route at length l along the Route from start
void Route::compute_end(String prefix){

  stringstream label_end;

  if(type.value == "o"){

    //end of orthodrome route
  
    (end.phi.value) = asin(cos((alpha.value)) * cos((start.phi.value)) * sin((l.value)/Re) + cos((l.value)/Re) * sin((start.phi.value)));
    (end.phi).normalize();

    (end.lambda.value) = -atan((cos((start.lambda.value)) * sin((l.value)/Re) * sin((alpha.value)) + sin((start.lambda.value)) * (-cos((l.value)/Re) * cos((start.phi.value)) +  cos((alpha.value)) * sin((l.value)/Re) * sin((start.phi.value))))/( cos((l.value)/Re) * cos((start.lambda.value)) * cos((start.phi.value)) +  sin((l.value)/Re) * (sin((alpha.value)) * sin((start.lambda.value)) -  cos((alpha.value)) * cos((start.lambda.value)) * sin((start.phi.value)))));

    if(cos((l.value)/Re) * cos((start.lambda.value)) * cos((start.phi.value)) + sin((l.value)/Re) * (sin((alpha.value)) * sin((start.lambda.value)) - cos((alpha.value)) * cos((start.lambda.value)) * sin((start.phi.value))) < 0.0){(end.lambda.value) += M_PI;}

    (end.lambda).normalize();

  }else{

    //end of loxodrome route

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
    
  }

  label_end << start.label.value << " transported";
  (end.label.value) = label_end.str();

}

void Route::print(String name, String prefix, ostream& ostr){

  String s, new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Route " << name.value << ":\n";

  type.print(String("type"), new_prefix, ostr);
  start.print(String("start point"), new_prefix, ostr);
  //end.print("end point", new_prefix, ostr);
  alpha.print(String("starting heading"), new_prefix, ostr);
  l.print(String("length"), String("nm"), new_prefix, ostr);
  
}


void Route::enter(String name, String prefix){

  string s;
  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter " << name.value << ":\n";

  do{
    type.enter(String("type [l(=loxodrome)/o(=orthodrome)]"), new_prefix);
    check = ((type.value == "l") || (type.value == "o"));
    if(!check){
      cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
    }
  }while(!check);
  start.enter(String("starting point"), new_prefix);
  alpha.enter(String("starting heading"), new_prefix);
  l.enter(String("length"), String("nm"), new_prefix);
  
}


class Date{

 public:
  unsigned int Y, M, D;
  bool Y_is_leap_year;
  vector<unsigned int> days_per_month;

  void print(String, String, ostream&);
  void enter(String, String);
  bool read_from_file(String, File&, String);
  stringstream to_string(void);
  void check_leap_year(void);

};

class Chrono{

 public:
  unsigned int h, m;
  double s;

  void print(String, String, ostream&);
  void enter(String, String);
  bool read_from_file(String, File&, String);
  stringstream to_string(unsigned int);

};

















bool Chrono::read_from_file(String name, File& file, String prefix){

  string line;
  stringstream new_prefix;
  bool check = true;
  
  //prepend \t to prefix
  new_prefix << "\t" << prefix.value;

  size_t pos = 0;

  //read type
  line.clear();
  getline(file.value, line);
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
  
};





//this function returns true if the date read is consistent, false if it is not 
bool Date::read_from_file(String name, File& file, String prefix){

  string line;
  stringstream new_prefix;
  bool check = true;

  //prepend \t to prefix
  new_prefix << "\t" << prefix.value;

  size_t pos = 0;

  //read type
  line.clear();
  getline(file.value, line);
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
  if(!(date.read_from_file(name, file, new_prefix))){ 
    check &= false;
  }

  //read chrono
  if(!(chrono.read_from_file(name, file, new_prefix))){
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
  file_number_of_lines.remove();
  
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


void File::remove(void){

  stringstream command;

  command.str("");
  command << "rm -rf " << (name.value) << "> /dev/null 2>&1";
  system(command.str().c_str());

  
}

void File::set_name(String filename){

  (name.value) = filename.value;
  
}

bool File::open(String mode, String prefix){


  if(mode.value == "in"){
    value.open(name.value, ios::in);
  }else{
    value.open(name.value, ios::out);
  }
  
  cout << prefix.value << "Opening " << (name.value) << " in mode '" << mode.value << "' ... \n";
  
  if(!value){
    
    cout << prefix.value << RED << "Error opening file " << (name.value) << "!\n" << RESET;
    return 0;
    
  }else{
    
    cout << prefix.value <<  "File " << (name.value) << " opened.\n";
    return 1;
     
  }

}

void File::close(String prefix){
  
  value.close();
  cout << prefix.value << "File " << (name.value) << " closed.\n";
     

}


bool Length::check_valid(String name, String prefix){

  bool check = true;
  
  if(value<0.0){
    check &= false;
    cout << prefix.value << RED << "Entered value of " << name.value << " is not valid!\n" << RESET;
  }

  return check;
  
}

void Length::read_from_file(String name, File& file, String prefix){

  string line;
  stringstream new_prefix;
  size_t pos1, pos2;

  //prepend \t to prefix
  new_prefix << "\t" << prefix.value;

  line.clear();
  getline(file.value, line);
  pos1 = line.find(" = ");
  pos2 = line.find(" nm");

  value = stod(line.substr(pos1+3, pos2 - (pos1+3)).c_str());
  
  print(String("radius"), String("nm"), prefix, cout);

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
  void enter(String, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, String);
  
};

void Limb::read_from_file(String name, File& file, String prefix){

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
  String name, type;
  Length radius;
  Angle RA, d; 
  void enter(Catalog, String);
  void print(String, String, ostream&);
  void read_from_file(String, File&, String);
  
};

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
    RA.read_from_file(String("right ascension"), file, new_prefix);
    d.read_from_file(String("declination"), file, new_prefix);
  }else{
    radius.read_from_file(String("radius"), file, new_prefix);
  }
  
}


class Catalog{

 public:
  vector<Body> list;
  Catalog(String);
  void add(String, String, double);
  void print(String, ostream&);

};

Catalog::Catalog(String filename){

  File file;
  string line;
  stringstream line_ins;
  Body temp;


  file.set_name(filename);
  if(file.open(String("in"), String(""))){

    getline((file.value), line);

    line.clear();
    line_ins.clear();
    getline((file.value), line);

    while(!(file.value).eof()){

      line_ins << line;
      line_ins >>  temp.type.value >> temp.name.value >> temp.radius.value >> temp.RA.value >> temp.d.value;

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
  
  
    file.close(String(""));

  }

}

void Catalog::print(String prefix, ostream& ostr){

  unsigned int i;
  stringstream name;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
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


class Atmosphere{

 public:
  Length earth_radius;
  unsigned int n_layers;
  double A, B, P_dry_0, alpha, beta, gamma, T0;
  vector<double> h, lambda, t;
  void set(void);
  double T(Length), n(Length), dTdz(Length), dndz(Length);

};


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
  static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*), lambda_circle_of_equal_altitude_minus_pi(double, void*);
  bool get_coordinates(String);
  void compute_DH_dip(String);
  bool compute_DH_refraction(String);
  void compute_DH_parallax_and_limb(String);

  void compute_H_a(String);
  bool compute_H_o(String);

  bool enter(Catalog, String, String);
  void print(String, String, ostream&);
  bool read_from_file(File&, String);
  bool reduce(String);
  bool check_data_time_interval(String);

  Point circle_of_equal_altitude(Angle);

  void transport(String);

   
};

void Sight::transport(String prefix){

  Route route;
  stringstream temp_label;
  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter route:\n";

  do{
    route.type.enter(String("type [l(=loxodrome)/o(=orthodrome)]"), new_prefix);
    check = ((route.type.value == "l") || (route.type.value == "o"));
    if(!check){
      cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
    }
  }while(!check);
  route.start = GP; 
  route.alpha.enter(String("starting heading"), new_prefix);
  route.l.enter(String("length"), String("nm"), new_prefix);

  route.print(String("transport"), prefix, cout);
  
  route.compute_end(new_prefix);

  GP = route.end;

  //append 'translated to ...' to the label of sight, and make this the new label of sight
  temp_label << label.value << " tr. w " << route.type.value << ", " << route.alpha.to_string(display_precision).str().c_str() << ", l = " << route.l.value << " nm";
  label.set(temp_label.str(), prefix);

  print(String("transported sight"), prefix, cout);

}

//For a given value of the parameter angle t, this function returns a Point which lies on the circle of equal altitude described by GP and H_o. By varying t, the whole circle can be traced. 
Point Sight::circle_of_equal_altitude(Angle t){

  Point p;

  (p.phi.value) = M_PI/2.0-acos(cos(M_PI/2.0 - (H_o.value)) * sin((GP.phi.value)) - cos((GP.phi.value)) * cos((t.value)) * sin(M_PI/2.0 - (H_o.value)));

  (p.phi).normalize();

  
  (p.lambda.value) = -atan( (-sin((GP.lambda.value)) * (cos((GP.phi.value)) * cos(M_PI/2.0 - (H_o.value)) + cos((t.value)) * sin((GP.phi.value)) * sin(M_PI/2.0 - (H_o.value))) + cos((GP.lambda.value)) * sin(M_PI/2.0 - (H_o.value)) * sin((t.value)))
			    / (cos((GP.phi.value)) * cos((GP.lambda.value)) * cos(M_PI/2.0 - (H_o.value)) + sin(M_PI/2.0 - (H_o.value)) * (cos((GP.lambda.value)) * cos((t.value)) * sin((GP.phi.value)) + sin((GP.lambda.value)) * sin((t.value)))) );
  if( cos((GP.phi.value)) * cos((GP.lambda.value)) * cos(M_PI/2.0 - (H_o.value)) + sin(M_PI/2.0 - (H_o.value)) * (cos((GP.lambda.value)) * cos((t.value)) * sin((GP.phi.value)) + sin((GP.lambda.value)) * sin((t.value))) < 0.0){(p.lambda.value) += M_PI;}

  (p.lambda).normalize();
  
  return p;

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
    limb.read_from_file(String("limb"), file, new_prefix);
  }
  H_s.read_from_file(String("sextant altitude"), file, new_prefix);
  index_error.read_from_file(String("index error"), file, new_prefix);
  artificial_horizon.read_from_file(String("artificial horizon"), file, new_prefix);
  if((artificial_horizon.value) == 'n'){
    height_of_eye.read_from_file(String("height of eye"), file, new_prefix);
  }
  
  check &= master_clock_date_and_hour.read_from_file(String("master-clock date and hour of sight"), file, new_prefix);
  if(!check){
    cout << prefix.value << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
  }
  time = master_clock_date_and_hour;
 
  use_stopwatch.read_from_file(String("use of stopwatch"), file, new_prefix);

  if(use_stopwatch.value == 'y'){
      
    stopwatch.read_from_file(String("stopwatch"), file, new_prefix);
    time.add(stopwatch);

  }
  
  TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file, new_prefix);
  time.add(TAI_minus_UTC);
  time.print(String("TAI date and hour of sight"), new_prefix, cout);

  //check whether the date and hour of sight falls within the time window covered by JPL data files
  check &= check_data_time_interval(prefix);

  label.read_from_file(String("label"), file, new_prefix);

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
    cout << prefix.value << RED << "Time lies outside interval of NASA's JPL data files!\n" << RESET;
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
  bool add_sight(String);
  void transport_sight(unsigned int, String);
  void add_point(String);
  void remove_sight(unsigned int, String);
  void transport_point(unsigned int, String);
  void remove_point(unsigned int, String);
  bool read_from_file(String, String);
  void print(String, ostream&);
  void print_sights(String, ostream&);
  void print_points(String, ostream&);
  void show(String);
  void menu(String);

};

bool Plot::read_from_file(String filename, String prefix){

  File file;
  stringstream line_ins;
  string line;
  size_t pos;
  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 
  file.set_name(filename.value);
  
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

      //if I find a sight which returns an error message when read from file, to be conservative I do not add any of the following sights in the file to sight_list because they may contain other errors
      check &= (sight.read_from_file(file, new_prefix));
      if(check){
	  
	check &= (sight.reduce(new_prefix));

	if(check){
	  sight.print(String("New sight"), new_prefix, cout);
    
	  sight_list.push_back(sight);
	  cout << new_prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";
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
    
      cout << new_prefix.value << "Found new point!\n";
  
      //read the point block
      Point point;

      point.read_from_file(file, new_prefix);
	  
      point.print(String("New point"), new_prefix, cout);
    
      point_list.push_back(point);
      cout << new_prefix.value << "Point added as point #" << point_list.size() << ".\n";
	  
      line.clear();
      //read dummyt text line 
      getline(file.value, line);
      pos = line.find("Point #");
 
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
  unsigned int i;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  cout << "You can:\n";
  for(i=0; i<choices.size(); i++){
    cout << "\t(" << i+1 << ") " << choices[i] << "\n";
  }
  enter_unsigned_int(&i, true, 1, choices.size()+1, String("choice #"), prefix);

  
  
  switch(i){

  case 1:{
    
    add_sight(new_prefix);
    print(new_prefix, cout);
    show(new_prefix);
    menu(prefix);  

  }
    break;


  case 2:{

    if(sight_list.size() > 0){

      print_sights(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, sight_list.size()+1, String("# of sight that you want to transport"), new_prefix);	
      i--;
   
      transport_sight(i, new_prefix);
      print(new_prefix, cout);
      show(new_prefix);

    }else{
      cout << RED << "There are no sights to transport!\n" << RESET;
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
      print(new_prefix, cout);
      show(new_prefix);

    }else{
      cout << RED << "There are no sights to delete!\n" << RESET;
    }

    menu(prefix);  
   
  }
    break;

  case 4:{

    add_point(new_prefix);
    print(new_prefix, cout);
    show(new_prefix);
    menu(prefix);  

  }
    break;

    
  case 5:{
    
    if(point_list.size() > 0){

      print_points(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, point_list.size()+1, String("# of point that you want to transport"), new_prefix);
      i--;

      transport_point(i, new_prefix);
      print(new_prefix, cout);
      show(new_prefix);

    }else{
      cout << RED << "There are no points to transport!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;

  case 6:{

    if(point_list.size() > 0){


      print_points(new_prefix, cout);

      enter_unsigned_int(&i, true, 1, point_list.size()+1, String("# of point that you want to delete"), new_prefix);
      i--;
	
      remove_point(i, new_prefix);
      print(new_prefix, cout);
      show(new_prefix);

    }else{
      cout << RED << "There are no points to delete!\n" << RESET;
    }
    
    menu(prefix);  

  }
    break;

  case 7:{

    if(sight_list.size() + point_list.size() > 0){
  
      File file;
      stringstream temp;
    
      file.name.enter(String("name of file (without extension)"), new_prefix);
      //add the extension .sav to name of file
      temp.str("");
      temp << file.name.value << ".sav";
      file.set_name(temp.str());

      file.open(String("out"),new_prefix);    
      print(new_prefix, file.value);
      file.close(new_prefix);

      command.str("");
      command << "mv plot.plt " << "'plot " << temp.str() << "'";
      system(command.str().c_str());

    }else{    
      cout << RED << "There are no sights nor points to save!\n" << RESET;
    }
    menu(prefix);
    
  }
    break;

  case 8:{

    String filename;
    stringstream line_ins;
    
    filename.enter(String("name of file (without extension)"), new_prefix);
    line_ins << filename.value << ".sav"; 
    filename.value = line_ins.str();
    
    if(read_from_file(filename, new_prefix)){
      print(new_prefix, cout);
      show(new_prefix);
    }
        
    menu(prefix);  

  }
    break;
    
    
  case 9:{

    File file;
    String line;

    //get date and time, which will be used for filename 
    get_date_hour(line, new_prefix);
    line = line.append(String(".sav"));

    //print all plots to file with the filename above
    ((file.name).value) = line.value;
    file.open(String("out"), new_prefix);
    print(new_prefix, file.value);
    file.close(new_prefix);

    //if plot.plt has been filled, here I save it with the name 'plot' + filename above
    if(sight_list.size() + point_list.size() >0){
      command.str("");
      command << "mv plot.plt " << "'plot " << line.value.c_str() << "'";
      system(command.str().c_str());
    }
    
    cout << prefix.value << CYAN << "Fair winds, following seas...\n" << RESET;
  }
    break;


  }

    
}

Plot::Plot(Catalog* cata){

  catalog = cata;
  job_id = -1;

  plot_command.precision(data_precision);
  command.precision(data_precision);

  file_id.set_name(String("job_id.txt"));
  file_gnuplot.set_name(String("plot.plt"));

  choices = {"Add a sight", "Transport a sight", "Delete a sight", "Add a point", "Transport a point", "Delete a point", "Save to file", "Read from file", "Exit"};
  
}

/*
  Plot::~Plot(){

  //file_gnuplot.remove();
  file_id.remove();
  
  }
*/

void Plot::print(String prefix, ostream& ostr){

  if(sight_list.size()>0){
    print_sights(prefix, ostr);
  }

  if(point_list.size()>0){
    print_points(prefix, ostr);
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

void Plot::print_points(String prefix, ostream& ostr){

  stringstream name;
  unsigned int i;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
   
  ostr << prefix.value << "Points in the plot:\n";
  for(i=0; i<point_list.size(); i++){
    name.str("");
    name << "Point #" << i+1;
    (point_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
  }


}





bool Plot::add_sight(String prefix){

  Sight sight;
  bool check = true;
  
  sight.enter((*catalog), String("new sight"), prefix);
  check &= (sight.reduce(prefix));

  if(check){
    sight.print(String("Sight"), prefix, cout);
  
    sight_list.push_back(sight);
    cout << prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";
  }

  return check;
  
}

void Plot::add_point(String prefix){

  Point point;
  
  point.enter(String("new point"), prefix);
  
  point_list.push_back(point);
  cout << prefix.value << "Point added as point #" << point_list.size() << ".\n";

 
}


void Plot::remove_sight(unsigned int i, String prefix){

  stringstream name;

  name.str("");
  name << "Sight to be removed: Sight #" << i+1;
  
  (sight_list[i]).print(String(name.str().c_str()), prefix, cout);
  
  sight_list.erase(sight_list.begin()+i);
  
  cout << prefix.value << "Sigh removed.\n";

}

void Plot::remove_point(unsigned int i, String prefix){

  stringstream name;

  name.str("");
  name << "Point to be removed: Point #" << i+1;
  
  (point_list[i]).print(String(name.str().c_str()), prefix, cout);
  
  point_list.erase(point_list.begin()+i);
  
  cout << prefix.value << "Point removed.\n";

}

void Plot::transport_sight(unsigned int i, String prefix){

  stringstream name;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  name.str("");
  name << "Sight to be transported: Sight #" << i+1;
  
  (sight_list[i]).print(String(name.str().c_str()), new_prefix, cout);
  
  (sight_list[i]).transport(prefix);
  
  cout << prefix.value << "Sight transported.\n";

}


void Plot::transport_point(unsigned int i, String prefix){

  stringstream name;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  name.str("");
  name << "Point to be transported: Point #" << i+1;
  
  (point_list[i]).print(String(name.str().c_str()), new_prefix, cout);
  
  (point_list[i]).transport(prefix);
  
  cout << prefix.value << "Point transported.\n";

}


void Plot::show(String prefix){

  stringstream line_ins;
  string line;
  unsigned int i;
  //t_p(m) are the larger (smaller) value of t where the circle of equal altitude crosses the meridian lambda = pi. 
  Angle t_min, t_max, t_p, t_m, t_s;
  Point p_min, p_max;
  int status, iter = 0;
  //x_hi(lo)_p(m) are the higher and lower bound of the interval where I will look for t_p(m)
  double x, x_lo_p, x_lo_m, x_hi_p, x_hi_m, x_lo_s, x_hi_s;
  gsl_function F;
  const gsl_root_fsolver_type *T;
  gsl_root_fsolver *s;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  T = gsl_root_fsolver_brent;
  s = gsl_root_fsolver_alloc (T);


  
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

    cout << "Sight # " << i+1 << "\n";

    //set the key in the correct position for the circle of equal altitude that will be plotted 
    plot_command << "\\\n set key at graph key_x, graph key_y - " << ((double)(i+1)) << "*key_spacing\\\n";

    //if abs(-tan((sight_list[i]).GP.phi.value)*tan(M_PI/2.0 - ((sight_list[i]).H_o.value))) < 1.0, then there exists a value of t = t_{max} (t_{min}) such that (sight_list[i]).GP.lambda vs. t has a maximum (minimum). In this case, I proceed and compute this maximum and minimum, and see whether the interval [(sight_list[i]).GP.lambda_{t = t_{min}} and (sight_list[i]).GP.lambda_{t = t_{max}}] embraces lambda = \pi. If it does, I modify the gnuplot command so as to avoid the horizontal line in the graph output. 
    if(abs(-tan((sight_list[i]).GP.phi.value)*tan(M_PI/2.0 - ((sight_list[i]).H_o.value))) < 1.0){
    
      //compute the values of the parametric Angle t, t_min and t_max, which yield the point with the largest and smallest longitude (p_max and p_min) on the circle of equal altitude 
      t_max.set(String("t_{max}"), acos(-tan((sight_list[i]).GP.phi.value)*tan(M_PI/2.0 - ((sight_list[i]).H_o.value))), new_prefix);
      t_min.set(String("t_{min}"), 2.0*M_PI - acos(-tan((sight_list[i]).GP.phi.value)*tan(M_PI/2.0 - ((sight_list[i]).H_o.value))), new_prefix);

      p_max = (sight_list[i]).circle_of_equal_altitude(t_max);
      p_min = (sight_list[i]).circle_of_equal_altitude(t_min);

      p_max.print(String("p_max"), new_prefix, cout);
      p_min.print(String("p_min"), new_prefix, cout);

      if((p_max.lambda.value < M_PI) && (p_min.lambda.value > M_PI)){
	cout << prefix.value << YELLOW << "Circle of equal altitude is cut!\n" << RESET;
	//in this case, the circle of equal altitude is cut through the meridian lambda = M_PI

	if((sight_list[i]).GP.lambda.value > M_PI){
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

	F.function = &((sight_list[i]).lambda_circle_of_equal_altitude_minus_pi);
	F.params = &(sight_list[i]);



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
      
	plot_command << "plot [0.:" << t_m.value << " - epsilon] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp dashtype " << i+1 << " lt " << i+1 << " ti \"" << (sight_list[i]).body.name.value << " " << (sight_list[i]).time.to_string(display_precision).str().c_str() << " TAI, " << (sight_list[i]).label.value << "\"\\\n";
      
	plot_command << "plot [" << t_m.value << " + epsilon:" << t_p.value << " - epsilon] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp dashtype " << i+1 << " lt " << i+1 << " noti \\\n";

	plot_command << "plot [" << t_p.value << " + epsilon:2.*pi] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp dashtype " << i+1 << " lt " << i+1 << " noti \\\n";

      }else{
	//in this case, the circle of equal altitude is not cut through the meridian lambda = M_PI, and I make a single plot

	plot_command << "plot [0.:2.*pi] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp dashtype " << i+1 << " lt " << i+1 << " ti \"" << (sight_list[i]).body.name.value << " " << (sight_list[i]).time.to_string(display_precision).str().c_str() << " TAI, " << (sight_list[i]).label.value << "\"\\\n";


      }
      
    }else{
      //in this case (sight_list[i]).GP.lambda.value is a monotonically increasing function of t: I find the value of t = t_s such that (sight_list[i]).GP.lambda.value = M_PI and split the gnuplot plot  in two plots so as to avoid the horizontal line

      // interval where I know that there will be t_s
      if((-sin(M_PI/2.0 - ((sight_list[i]).H_o.value))/cos(((sight_list[i]).GP.phi.value) - (M_PI/2.0 - ((sight_list[i]).H_o.value)))) > 0.0){
	//in this case lambda'(t = 0) > 0.0 -> lambda'(t) > 0.0  for all t
	if((sight_list[i]).GP.lambda.value < M_PI){
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
	if((sight_list[i]).GP.lambda.value < M_PI){
	  //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to M_PI <= t< 2*M_PI
	  x_lo_s = M_PI;
	  x_hi_s = 2.0*M_PI;
	}else{
	  //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to 0.0 <= t< M_PI
	  x_lo_s = 0.0;
	  x_hi_s = M_PI;
	}

      }

      F.function = &((sight_list[i]).lambda_circle_of_equal_altitude_minus_pi);
      F.params = &(sight_list[i]);

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
      
	plot_command << "plot [0.:" << t_s.value << " - epsilon] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp dashtype " << i+1 << " lt " << i+1 << " ti \"" << (sight_list[i]).body.name.value << " " << (sight_list[i]).time.to_string(display_precision).str().c_str() << " TAI, " << (sight_list[i]).label.value << "\"\\\n";
      
	plot_command << "plot [" << t_s.value << " + epsilon:2.*pi] xe(K*Lambda(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")), ye(K*Phi(t, " << (sight_list[i]).GP.phi.value << ", " << (sight_list[i]).GP.lambda.value << ", " << M_PI/2.0 - ((sight_list[i]).H_o.value) << ")) smo csp dashtype " << i+1 << " lt " << i+1 << " noti \\\n"; 

    }
    
  } 
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "LANG=C sed 's/#sight_plots/" << plot_command.str().c_str() << "/g' plot_dummy.plt >> plot_temp.plt \n";


  
  //replace line with point plots
  
  plot_command.str("");
  for(i=0; i<point_list.size(); i++){
    //set the key in the correct position for the point that will be plotted 
    plot_command << "\\\n set key at graph key_x, graph key_y - " << ((double)(sight_list.size()+i+1)) << "*key_spacing\\\n";

    plot_command << "plot \"+\" u (xe(K*(" << (point_list[i]).lambda.value << "))):(ye(K*(" << (point_list[i]).phi.value << "))) w p lw 2 lt " << i+1 << " ti \"" << (point_list[i]).label.value << "\"\\\n";
  }
  //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
  command << "LANG=C sed 's/#point_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> " << ((file_gnuplot.name).value) << "\n";


  command << "gnuplot '" << ((file_gnuplot.name).value) << "' & \n echo $! >> " << ((file_id.name).value) << "\n";
  command << "rm -rf plot_temp.plt";
  
  system(command.str().c_str());

  //read the job id from file_id
  if(file_id.open(String("in"), String("\t"))){
    getline(file_id.value, line);
    line_ins << line;
    line_ins >> job_id;
  }

  file_id.close(String("\t"));
  file_id.remove();
  
  cout << prefix.value << "Job id = "<< job_id << "\n";
  
}

bool Sight::enter(Catalog catalog, String name, String prefix){

  //pointer to init.txt to read fixed sight data from in there
  File file_init;
  String new_prefix;
  bool check = true;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  file_init.set_name(String("data/init.txt"));
  check &= (file_init.open(String("in"), prefix));
  
  cout << prefix.value << "Enter " << name.value << ":\n";
  
  body.enter(catalog, new_prefix);
  //GP.label.set("geographic position", new_prefix);

  if(body.type.value != "star"){
    limb.enter(String("limb"), new_prefix);
  }
  H_s.enter(String("sextant altitude"), new_prefix);
  //read index error from data/init.txt
  index_error.read_from_file(String("index error"), file_init, new_prefix);
  artificial_horizon.enter(String("artificial horizon"), new_prefix);
  if(artificial_horizon.value == 'n'){
    height_of_eye.enter(String("height of eye"), String("m"), new_prefix);
  }
  
  do{
  
    master_clock_date_and_hour.enter(String("master-clock date and hour of sight"), new_prefix);
    time = master_clock_date_and_hour;
    
    use_stopwatch.enter(String("use of stopwatch"), new_prefix);

    if(use_stopwatch.value == 'y'){
        
      stopwatch.enter(String("stopwatch reading"), new_prefix);
      time.add(stopwatch);
    
    }

    //TAI_minus_UTC.enter(String("TAI - UTC at time of master-clock synchronization with UTC"), new_prefix);
    //read TAI_minus_UTC from data/index.txt
    TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, new_prefix);
    time.add(TAI_minus_UTC);
    time.print(String("TAI date and hour of sight"), new_prefix, cout);

  }while(!check_data_time_interval(prefix));

  label.enter(String("label"), new_prefix);
  
  file_init.close(prefix);


  if(!check){
    cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
  }

  return check;

}

bool Sight::reduce(String prefix){

  bool check = true;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  compute_H_a(new_prefix);
  check &= get_coordinates(new_prefix);
  check &= compute_H_o(new_prefix);

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

double Sight::lambda_circle_of_equal_altitude_minus_pi(double x, void* sight){

  Sight* a = (Sight*)sight;
  
  Angle t;
  (t.value) = x;
  
  return((((*a).circle_of_equal_altitude(t)).lambda.value) - M_PI);

  
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

  atmosphere.set();
  
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
  
  if(check_valid(name, new_prefix)){
    print(name, String("nm"), prefix, cout); 
  }
  
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

  ostr << prefix.value << name.value << " = ";
  if(unit.value == "nm"){
    ostr << value << " nm\n";
  }else{
    ostr << value*nm*1e3 << " m\n";
  }
 
}

bool Sight::get_coordinates(String prefix){

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

      if(gsl_spline_eval_e(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.lambda).value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	(GP.lambda).normalize();
	(GP.lambda).print(String("GHA"), new_prefix, cout);
      }	
      //(GP.lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);


      if(gsl_spline_eval_e(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.phi).value)) != GSL_SUCCESS){
	check &= false; 
      }else{
	(GP.phi).normalize();
	(GP.phi).print(String("d"), new_prefix, cout);
      }	
      //(GP.phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

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

      
      if(gsl_spline_eval_e(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.lambda).value)) != GSL_SUCCESS){
	check &= false;
      }else{
	(GP.lambda).normalize();
	(GP.lambda).print(String("GHA"), new_prefix, cout);
      }

      if(gsl_spline_eval_e(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc, &((GP.phi).value)) != GSL_SUCCESS){
	check &= false;
      }else{
	(GP.phi).normalize();
	(GP.phi).print(String("d"), new_prefix, cout);
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
  print(name, prefix, cout);
  
}

void Angle::enter(String name, String prefix){

  string s;
  unsigned int ad;
  double am;
  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));


  cout << prefix.value << "Enter " << name.value << " [s ddd mm.m]:\n";

  do{
    
    s.clear();

    cout << prefix.value << "\tEnter s: ";
    cin >> s;
    
    if((s=="+") || (s=="-")){check = true;}
    else{
      cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
      check = false;
    }
  }while(!check);



  enter_unsigned_int(&ad, true, 0, 360, String("ddd"), new_prefix);
  enter_double(&am, true, 0.0, 60.0, String("mm.m"), new_prefix);
 
  value = k*(((double)ad) + am/60.0);
  if(s=="-"){value*=-1.0;}
  normalize();
  print(name, prefix, cout);


}

void Point::enter(String name, String prefix){

  bool check;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));

  cout << prefix.value << "Enter " << name.value << ":\n";

  do{
    phi.enter(String("latitude"), new_prefix);
    if(!(((0.0 <= phi.value) && (M_PI/2.0 >= phi.value)) || ((3.0*M_PI/2.0 <= phi.value) && (2.0*M_PI >= phi.value)))){
      cout << new_prefix.value << RED << "Entered value is not valid!\n" << RESET;
      check = true;
    }else{
      check = false;
    }
  }while(check);
  
  lambda.enter(String("longitude"), new_prefix);
  label.enter(String("label"), new_prefix);
  
}

void Point::print(String name, String prefix, ostream& ostr){

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

  string s;
  String new_prefix;

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
 

  cout << prefix.value << "Enter " << name.value << " [YYYY-MM-DD]\n";

  enter_unsigned_int(&Y, false, 0, 0, String("YYYY"), prefix);
 
  check_leap_year();
  if((Y_is_leap_year)){
    (days_per_month) = days_per_month_leap;
    cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET;
  }else{
    (days_per_month) = days_per_month_common;
    cout << new_prefix.value << "Entered a common year\n";
  }

  enter_unsigned_int(&M, true, 1, 12+1, String("MM"), prefix);
  
  enter_unsigned_int(&D, true, 1, days_per_month[M-1]+1, String("DD"), prefix);

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

  //append \t to prefix
  new_prefix = prefix.append(String("\t"));
  
  cout << prefix.value << "Enter " << name.value << " [hh-mm-ss]\n";

  enter_unsigned_int(&h, true, 0, 24, String("hh"), new_prefix);
  enter_unsigned_int(&m, true, 0, 60, String("mm"), new_prefix);
  enter_double(&s, true, 0.0, 60.0, String("ss.s"), new_prefix);
 
}



stringstream Time::to_string(unsigned int precision){

  stringstream output;

  output << date.to_string().str() << " " << chrono.to_string(precision).str();
 
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
  
  cout << prefix.value << "Enter master-clock date and hour\n";
  
  date.enter(String("date"), new_prefix);
  chrono.enter(String("hour"), new_prefix);
  
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


