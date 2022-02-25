//lengths are in nm, time is in hours, temperature in Kelvin, Pressure in Pascal
//this is the high precision used for storing data and making calculations with it 
#define data_precision 32
//this is the low precision used for displaying data
#define display_precision 4
#define k (2.0*M_PI/360.0)
#define K (1.0/k)
//MJD_min corresponds to Jan 1 2016 00-00-26.00 TAI, i.e., Jan 1 2016 00-00-00.00 UTC  
#define MJD_min 57388.00041666666666666664
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
//all possible chars that can enter in a signed integer
#define chars_int "+-0123456789"
//all possible chars that can enter in an unsigned integer
#define chars_unsigned_int "0123456789"
//all possible chars that can enter in a non-negative double
#define chars_double "+-0123456789."

static const int days_per_month_leap_temp[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_leap(days_per_month_leap_temp, days_per_month_leap_temp + sizeof(days_per_month_leap_temp)/sizeof(days_per_month_leap_temp[0]));

static const int days_per_month_common_temp[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
vector<unsigned int> days_per_month_common(days_per_month_common_temp, days_per_month_common_temp + sizeof(days_per_month_common_temp)/sizeof(days_per_month_common_temp[0]));


#define path_file_utc_date_and_time "utc.txt"
#define path_file_init "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/init.txt"
#define path_file_catalog "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/catalog.txt"
//these are the color codes in kml file format for a few populat colors (red, etc...);
string kml_colors[] = {"ff0000ff", "ffff0000", "ff336699", "ff00ff00", "ff0080ff", "ffff00ff"};
string hex_colors[] = {"#000000", "#0000FF", "#00FF00", "#663300", "#3399FF", "#0000CC"};
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
    
    bool operator==(const String&), operator!=(const String&);
    
};

//this function checks whether the  unsigned int in string s is formatted correctly and, if check_interval = true, it also checks whether the this unsigned int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_unsigned_int(string s, unsigned int* i, bool check_interval, unsigned int min, unsigned int sup){
    
    bool check;
    unsigned int j;
    
    if(/*here I check that the string s is not empty*/(!s.empty()) && /*here I check whether the quantity entered in s is an unsigned integer, i.e., it contains only the characters 0123456789*/ ((s.find_first_not_of(chars_unsigned_int)) == (std::string::npos))){
        
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



//this function checks whether the int in string s is formatted correctly and, if check_interval = true, it also checks whether the this  int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_int(string s, int* i, bool check_interval, int min, int sup){
    
    bool check;
    int j;
    
    if(/*here I check whether the quantity entered in s is an integer, i.e., it contains only the characters +-0123456789*/ ((s.find_first_not_of(chars_int)) == (std::string::npos))){
        
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



//this function asks the user to enter an  int from keyboard and checks whether the entered value is an  int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_int(int* i, bool check_interval, int min, int sup, String name, String prefix){
    
    string s;
    bool check;
    
    do{
        
        s.clear();
        
        cout << prefix.value << "Enter " << name.value << ":";
        getline(cin >> ws, s);
        
        check = check_int(s, i, check_interval, min, sup);
        
        if(!check){
            
            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
            
        }
        
    }while(!check);
    
}


//this function checks whether the double in string s is formatted correctly and, if check_interval = true, it also checks whether the this double lies in [min, sup). If x != NULL and the check is ok, it also writes the value of the double read from s into (*x)
bool check_double(string s, double* x, bool check_interval, double min, double sup){
    
    bool check;
    double y;
    
    if(/*here I check that the string s is not empty*/(!s.empty()) && (/*here I check whether the quantity in s contains the allowed chars for double, i.e., it contains only the characters in chars_double*/ ((s.find_first_not_of(chars_double)) == (std::string::npos))) && /*here I count whether the '.' occurs zero or one time*/(count(s.begin(), s.end(), '.') <= 1) && /*here I count whether the '+' occurs zero or one time*/(count(s.begin(), s.end(), '+') <= 1)  && /*here I count whether the '-' occurs zero or one time*/(count(s.begin(), s.end(), '-') <= 1)){
        
        //if the check above passed, then I proceed and write s into y
        y = stod(s);
        
        if(check_interval){
            
            check = ((y >= min) && (y < sup));
            
        }else{
            
            check = true;
            
        }
        
    }else{
        
        check = false;
        
    }
    
    //if x == NULL, then this function is meant to be used to check the correct format of s only, not to write its value to x.
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

bool String::operator==(const String& s){
    
    return((((*this).value) == (s.value)));
    
}

bool String::operator!=(const String& s){
    
    return(!((*this) == s));
    
}





class Int{
    
public:
    int value;
    
    void read_from_file(String, File&, bool, String);
    void enter(String, String);
    void set(String, int, String);
    void print(String, String, ostream&);
    
    bool operator==(const Int&), operator!=(const Int&);
    
};


void Int::set(String name, int i, String prefix){
    
    String new_prefix;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    value = i;
    
    print(name, prefix, cout);
    
}


bool Int::operator==(const Int& i){
    
    return (value == (i.value));
    
}

bool Int::operator!=(const Int& i){
    
    return (!((*this) == i));
    
}


//enter an Int
void Int::enter(String name, String prefix){
    
    enter_int(&value, false, 0, 0, name, prefix);
    print(name, prefix, cout);
    
}



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
    
    
    if(mode == String("in")){
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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
        
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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
        
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





class Length{
    
public:
    double value;
    
    void set(String, double, String);
    void enter(String, String, String);
    void print(String, String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    bool check_valid(String, String);
    string to_string(String, unsigned int);
    bool operator> (const Length&), operator==(const Length&), operator!=(const Length&);
    Length operator + (const Length&);
    
};

bool Length::operator==(const Length& length){
    
    return (value == (length.value));
    
}

bool Length::operator!=(const Length& length){
    
    return (!((*this) == length));
    
}


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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
        
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
    
    Answer();
    Answer(char, String);
    void enter(String, String);
    bool set(String, char, String);
    //the print function takes an arbitrary ostream for output, which can be equal to cout if we want to print otuput to terminal, or to a file ofstream if we want to print the output to a file
    void print(String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    bool operator==(const Answer&), operator !=(const Answer&);
    
};

Answer::Answer(void){
    //this is the default constructor, sets value to the default value, 'n' char
    
    value = 'n';
    
}

Answer::Answer(char c, String prefix){
    
    if((c=='y') || (c=='n')){
        
        value = c;
        
    }else{
        //if the entered value is not valid, set value to 'n' and prints out this info
        
        value = 'n';
        cout << prefix.value << RED << "Value of answer is not valid, setting it to 'n'!\n" << RESET;
        
    }
    
}


bool Answer::operator==(const Answer& s){
    
    return((((*this).value) == (s.value)));
    
}

bool Answer::operator!=(const Answer& s){
    
    return(!((*this)==s));
    
}


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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
        
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
    
    //If I am printing to cout, I print only if the content of the string is not empty, to avoid filling up the command line with useless stuff. If I am printing somewhere else, I print even if the content of the String is empty
    if(((*this) != String("")) || (&ostr != &cout)){ostr << prefix.value << name.value << " = " << value << "\n";}
    
}

void String::set(String name, String input_string, String prefix){
    
    value = (input_string.value);
    if(!(name == String(""))){print(name, prefix, cout);}
    
}



class Angle{
    
public:
    
    double value;
    void normalize(void);
    void enter(String, String);
    void set(String, double, String);
    void print(String, String, ostream&);
    void to_deg_min(unsigned int*, double*);
    void from_sign_deg_min(char, unsigned int, double);
    void read_from_file(String, File&, bool, String);
    string to_string(unsigned int);
    
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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
        
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
    string to_string(unsigned int);
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
    
    items.push_back(String("latitude"));
    items.push_back(String("longitude"));
    items.push_back(String("label"));
    
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
            
            phi.enter(String("latitude"), new_prefix);
            
        }
            break;
            
            
        case 2:{
            
            lambda.enter(String("longitude"), new_prefix);
            
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
    bool closest_point_to(Position*, Angle*, Position, String);
    
};

//Given the route (*this), this function returns the point on the Route which is closest to Position q, and writes this position and the corresponding value of t in p and tau, respectively.
bool Route::closest_point_to(Position* p, Angle* tau, Position q, String prefix){
    
    String new_prefix;
    bool check;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    check = true;
    
    if(type == String("c")){
        
        //these are the two values of the parametric angle t of the Route (*this), for which the distance between q and a point on (*this) vs. t has a maximum or a minimum
        Angle t_1, t_2;
        Position p_1, p_2;
        Length s_1, s_2;
        
        t_1.set(String(""),
                
                
                atan((cos((GP.lambda.value) - (q.lambda.value))*cos((q.phi.value))*sin((GP.phi.value)) - cos((GP.phi.value))*sin((q.phi.value)))/
                     sqrt(gsl_pow_int(cos((GP.lambda.value) - (q.lambda.value)),2)*gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.phi.value)),2) +
                          gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.lambda.value) - (q.lambda.value)),2) -
                          2*cos((GP.phi.value))*cos((GP.lambda.value) - (q.lambda.value))*cos((q.phi.value))*sin((GP.phi.value))*sin((q.phi.value)) +
                          gsl_pow_int(cos((GP.phi.value)),2)*gsl_pow_int(sin((q.phi.value)),2)),
                     (cos((q.phi.value))*sin((GP.lambda.value) - (q.lambda.value)))/
                     sqrt(gsl_pow_int(cos((GP.lambda.value) - (q.lambda.value)),2)*gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.phi.value)),2) +
                          gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.lambda.value) - (q.lambda.value)),2) -
                          2*cos((GP.phi.value))*cos((GP.lambda.value) - (q.lambda.value))*cos((q.phi.value))*sin((GP.phi.value))*sin((q.phi.value)) +
                          gsl_pow_int(cos((GP.phi.value)),2)*gsl_pow_int(sin((q.phi.value)),2)))
                
                , new_prefix);
        
        
        t_2.set(String(""),
                
                atan((-(cos((GP.lambda.value) - (q.lambda.value))*cos((q.phi.value))*sin((GP.phi.value))) + cos((GP.phi.value))*sin((q.phi.value)))/
                     sqrt(gsl_pow_int(cos((GP.lambda.value) - (q.lambda.value)),2)*gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.phi.value)),2) +
                          gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.lambda.value) - (q.lambda.value)),2) -
                          2*cos((GP.phi.value))*cos((GP.lambda.value) - (q.lambda.value))*cos((q.phi.value))*sin((GP.phi.value))*sin((q.phi.value)) +
                          gsl_pow_int(cos((GP.phi.value)),2)*gsl_pow_int(sin((q.phi.value)),2)),
                     -((cos((q.phi.value))*sin((GP.lambda.value) - (q.lambda.value)))/
                       sqrt(gsl_pow_int(cos((GP.lambda.value) - (q.lambda.value)),2)*gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.phi.value)),2) +
                            gsl_pow_int(cos((q.phi.value)),2)*gsl_pow_int(sin((GP.lambda.value) - (q.lambda.value)),2) -
                            2*cos((GP.phi.value))*cos((GP.lambda.value) - (q.lambda.value))*cos((q.phi.value))*sin((GP.phi.value))*sin((q.phi.value)) +
                            gsl_pow_int(cos((GP.phi.value)),2)*gsl_pow_int(sin((q.phi.value)),2))))
                
                , new_prefix);
        
        //determine which one between the point on (*this) at t_1 and the one at t_2 is the one with minimum distance with respect to q, and store this point into (*p)
        l.set(String(""), (t_1.value)*Re*sin(omega.value), new_prefix);
        compute_end(new_prefix);
        p_1 = end;
        q.distance(p_1, &s_1, String("Distance with respect to p_1"), new_prefix);
        
        l.set(String(""), (t_2.value)*Re*sin(omega.value), new_prefix);
        compute_end(new_prefix);
        p_2 = end;
        q.distance(p_2, &s_2, String("Distance with respect to p_2"), new_prefix);
        
        if(s_2 > s_1){
            (*p) = p_1;
            (*tau) = t_1;
        }else{
            (*p) = p_2;
            (*tau) = t_2;
        }
        
        
    }else{
        
        check &= false;
        
    }
    
    if(check){
        
        (*p).print(String("Closest point"), prefix, cout);
        
    }else{
        
        cout << prefix.value << RED << "Closest point could not be computed!\n" << RESET;
        
    }
    
    return check;
    
}

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
    bool check;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    check = true;
    
    if(!(((*this).type == String("c")) && (route.type == String("c")))){
        
        cout << prefix.value << "Routes are not circles of equal altitude: this code only computes intersects between circles of equal altitudes\n";
        check &= false;
        
    }else{
        
        Angle theta, t_temp;
        Length r, s;
        
        theta.set(String("angle between the two GPs"), acos(cos(((*this).GP.phi.value))*cos((route.GP.phi.value))*cos(((*this).GP.lambda.value) - (route.GP.lambda.value)) + sin(((*this).GP.phi.value))*sin((route.GP.phi.value))), prefix);
        
        if((abs(((*this).omega.value)-(route.omega.value)) < (theta.value)) && ((theta.value) < ((*this).omega.value)+(route.omega.value))){
            //in this case routes intersect
            
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
            
            (route.l).set(String("l of intersection"), Re * sin(route.omega.value) * ((u[0]).value), prefix);
            route.compute_end(prefix);
            
            check &= ((*p)[0]).distance(route.end, &r, String(""), prefix);
            check &= ((*p)[1]).distance(route.end, &s, String(""), prefix);
            
            if(check){
                
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
                
                t.clear();
                u.clear();
                
            }
            
        }else{
            
            cout << prefix.value << "Routes do no intersect\n";
            check &= false;
            
        }
        
    }
    
    return check;
    
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
    string to_string(void);
    void check_leap_year(void);
    bool set_current(String);
    
};

class Chrono{
    
public:
    unsigned int h, m;
    double s;
    
    void print(String, String, ostream&);
    bool set(String, double, String);
    bool set_current(String);
    void enter(String, String);
    bool read_from_file(String, File&, bool, String);
    string to_string(unsigned int);
    
    bool operator==(const Chrono&), operator!=(const Chrono&);
    
};

bool Chrono::operator==(const Chrono& chrono){
    
    return((h == (chrono.h)) && (m == (chrono.m)) && (s == (chrono.s)));
    
}

bool Chrono::operator!=(const Chrono& chrono){
    
    return (!((*this)==chrono));
    
}


//sets the Chrono object to the time x, which is expressed in hours
bool Chrono::set(String name, double x, String prefix){
    
    String new_prefix;
    bool check;
    
    check = true;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    check &= (x >= 0.0);
    if(check){
        
        h = ((unsigned int)floor(x));
        m = (unsigned int)((x - ((double)h))*60.0);
        s = (((x - ((double)h))*60.0) - ((double)m))*60.0;
        
        print(name, prefix, cout);
        
    }else{
        
        cout << new_prefix.value << RED << "Set value is not valid!\n" << RESET;
        
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
    bool set_current(String);
    bool read_from_file(String, File&, String);
    
    void to_MJD(void);
    void to_TAI(void);
    void add(Chrono);
    
    string to_string(unsigned int);
    bool operator==(const Time&), operator!=(const Time&), operator> (const Time&);
    
};

//evaluates whether Time (*this) is equal to t
bool Time::operator==(const Time& t){
    
    Time s;
    
    (*this).to_MJD();
    s = t;
    s.to_MJD();
    
    return((((*this).MJD) == (s.MJD)));
    
}

//evaluates whether Time (*this) is different from t
bool Time::operator!=(const Time& t){
    
    return(!((*this) == t));
    
}




//evaluates whether Time (*this) is larger than t
bool Time::operator>(const Time& t){
    
    Time s;
    
    (*this).to_MJD();
    s = t;
    s.to_MJD();
    
    return((((*this).MJD) > (s.MJD)));
    
}

string Time::to_string(unsigned int precision){
    
    stringstream output;
    
    output << date.to_string() << " " << chrono.to_string(precision) << " UTC";
    
    return (output.str().c_str());
    
}

string Position::to_string(unsigned int precision){
    
    stringstream output;
    
    output << phi.to_string(precision) << " " << lambda.to_string(precision);
    
    return (output.str().c_str());
    
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
        check = ((route.type == String("l")) || (route.type == String("o")));
        if(!check){
            cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
        }
    }while(!check);
    route.start = (*this);
    route.alpha.enter(String("Course Over Ground"), new_prefix);
    
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
    
    temp_label << label.value << "tr. w. " << route.type.value << ", COG = " << route.alpha.to_string(display_precision) << ", l = " << (route.l).value << " nm";
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
    void enter(String, Catalog, String);
    bool check(unsigned int*, Catalog, String);
    void print(String, String, ostream&);
    void read_from_file(String, File&, String);
    
    bool operator==(const Body&);
    
    
};

bool Body::operator==(const Body& body){
    
    return (name == (body.name));
    
}


class Limb{
    
public:
    char value;
    void enter(String, String);
    void print(String, String, ostream&);
    void read_from_file(String, File&, bool, String);
    
    bool operator==(const Limb&);
    
};

bool Limb::operator==(const Limb& limb){
    
    return(value == limb.value);
    
}

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
    Int related_route;
    //all_items is a list of all the possible items which are part of a Sight object (master_clock_date_and_hour, body, ...). items is the list of items specific to a given Sight object: items may not include all the elements of all_items
    vector<String> items, all_items;
    
    
    Sight();
    static double dH_refraction(double, void*), rhs_DH_parallax_and_limb(double, void*);
    bool get_coordinates(Route*, String);
    void compute_DH_dip(String);
    bool compute_DH_refraction(String);
    void compute_DH_parallax_and_limb(String);
    
    void compute_H_a(String);
    bool compute_H_o(String);
    
    bool enter(Catalog, String, String);
    bool modify(Catalog, String);
    void print(String, String, ostream&);
    bool read_from_file(File&, String);
    bool reduce(Route*, String);
    bool check_data_time_interval(String);
    
    void add_to_wxListCtrl(wxListCtrl*);
    
};

class Catalog{
    
public:
    vector<Body> list;
    Catalog(String, String);
    void add(String, String, double);
    void print(String, ostream&);
    
};


bool Sight::modify(Catalog catalog, String prefix){
    
    unsigned int i;
    String new_prefix, new_new_prefix;
    bool check;
    vector<String>::iterator p;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    new_new_prefix = new_prefix.append(String("\t"));
    
    check = true;
    
    cout << new_prefix.value << "Enter the item that you want to modify:\n";
    for(i=0; i<items.size(); i++){
        cout << new_prefix.value << "\t(" << i+1 << ") " << (items[i]).value << "\n";
    }
    
    enter_unsigned_int(&i, true, 1, items.size()+1, String("choice #"), new_prefix);
    //decrease i because the user entered it in format starting from i=1;
    i--;
    
    p = find(all_items.begin(), all_items.end(), items[i]);
    
    switch(/*this is the iterator p converted to int: if this quantity = 0 then I am considering the first element in all_items, if its = 1 I am considering the second element, etc... */distance(all_items.begin(), p)){
            
        case 0:{
            //in this case I modify the body
            
            Body old_body;
            
            old_body = body;
            
            body.print(String("old body"), new_new_prefix, cout);
            body.enter(String("new body"), catalog, new_new_prefix);
            
            if(!(body == old_body)){
                
                if((body.type == String("star")) && (!(old_body.type == String("star")))){
                    //in this case, the old body was not a star, while the new (mofidied) body is a star -> I remove the limb entry (all_items[1]) in items
                    
                    items.erase(find(items.begin(), items.end(), all_items[1]));
                    
                }
                if((old_body.type == String("star")) && (!(body.type == String("star")))){
                    //in this case, the old body was  a star, while the new (mofidied) body is not a star -> I ask the user to enter the limb of the new body and add the limb entry (all_items[1]) in items, right after the body entry (all_items[0])
                    
                    limb.enter(String("limb of new body"), new_new_prefix);
                    items.insert(find(items.begin(), items.end(), all_items[0])+1, all_items[1]);
                    
                }
                
                cout << new_prefix.value << "Body modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New body is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
        case 1:{
            //in this case I modify the limb
            
            Limb old_limb;
            
            old_limb = limb;
            
            limb.print(String("old limb"), new_new_prefix, cout);
            limb.enter(String("new limb"), new_new_prefix);
            
            if(!(old_limb == limb)){
                
                cout << new_prefix.value << "Limb modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New limb is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
            
        case 2:{
            //in this case I modify the sextant altitude
            
            Angle old_H_s;
            
            old_H_s = H_s;
            
            H_s.print(String("old sextant altitude"), new_new_prefix, cout);
            //here I assume that the sextant altitude is positive: if you want to trop this, true -> false
            H_s.enter(String("new sextant altitude"), new_new_prefix);
            
            if(!(old_H_s == H_s)){
                
                cout << new_prefix.value << "Sextant altitude modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New sextant altitude is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
        case 3:{
            //in this case I modify the artificial horizon
            
            Answer old_artificial_horizon;
            
            old_artificial_horizon = artificial_horizon;
            
            artificial_horizon.print(String("old artificial_horizon"), new_new_prefix, cout);
            artificial_horizon.enter(String("new artificial horizon"), new_new_prefix);
            
            if(old_artificial_horizon != artificial_horizon){
                
                
                if((artificial_horizon == Answer('y', new_new_prefix)) && (old_artificial_horizon == Answer('n', new_new_prefix))){
                    //in this case,  old artificial_horizon = n, while the new (mofidied) artificial_horizon = y -> I remove the height of eye entry (all_items[4]) in items and set the height of eye (which is now useless) to zero
                    
                    items.erase(find(items.begin(), items.end(), all_items[4]));
                    height_of_eye.set(String("removed height of eye"), 0.0, new_new_prefix);
                    
                }
                
                if((artificial_horizon == Answer('n', new_new_prefix)) && (old_artificial_horizon == Answer('y', new_new_prefix))){
                    //in this case,  old artificial_horizon = y, while the new (mofidied) artificial_horizon = n -> I add the height of eye entry (all_items[4]) in items right after the entry "artificial horizon" (all_items[3]), and let the user enter the height of eye
                    
                    height_of_eye.enter(String("height of eye"), String("m"), new_new_prefix);
                    items.insert(find(items.begin(), items.end(), all_items[3])+1, String(all_items[4]));
                    
                }
                
                cout << new_prefix.value << "Artificial horizon modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New artificial horizon is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
        case 4:{
            //in this case I modify the height of eye
            
            Length old_height_of_eye;
            
            old_height_of_eye = height_of_eye;
            
            height_of_eye.print(String("old height of eye"), String("m"), new_new_prefix, cout);
            height_of_eye.enter(String("new height of eye"), String("m"), new_new_prefix);
            
            if(old_height_of_eye != height_of_eye){
                
                cout << new_prefix.value << "Height of eye modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New height of eye is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
            
        case 5:{
            //in this case I modify the master-clock date and hour of sight
            
            Time old_master_clock_date_and_hour;
            
            old_master_clock_date_and_hour = master_clock_date_and_hour;
            
            master_clock_date_and_hour.print(String("old master-clock date and hour of sight"), new_new_prefix, cout);
            master_clock_date_and_hour.enter(String("new master-clock date and hour of sight"), new_new_prefix);
            
            if(old_master_clock_date_and_hour != master_clock_date_and_hour){
                //if the new master-clock date and hour of sight is different from the old one, I write its value into (*this).time, and add to itt the stopwatch time if use_stopwatch.value == 'y'
                
                time = master_clock_date_and_hour;
                
                if(use_stopwatch == Answer('y', new_new_prefix)){
                    
                    //the stopwatch value has changed -> I update the time object in the Sight
                    time.add(stopwatch);
                    
                }
                
                time.add(TAI_minus_UTC);
                time.print(String("TAI date and hour of sight"), new_prefix, cout);
                
                cout << new_prefix.value << "master-clock date and hour of sight modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New master-clock date and hour of sight is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
            
        case 6:{
            //in this case I modify use of stopwatch
            
            Answer old_use_stopwatch;
            
            old_use_stopwatch = use_stopwatch;
            
            use_stopwatch.print(String("old use of stopwatch"), new_new_prefix, cout);
            use_stopwatch.enter(String("new use of stopwatch"), new_new_prefix);
            
            if(old_use_stopwatch != use_stopwatch){
                
                if((use_stopwatch == Answer('n', new_new_prefix)) && (old_use_stopwatch == Answer('y', new_new_prefix))){
                    //in this case,  old use_stopwatch = y, while the new (mofidied) use_stopwatch = n -> I remove the  (all_items[7]) in items and set stopwatch reading (which is now useless) to zero
                    
                    stopwatch.set(String("removed stopwatch reading"), 0.0, new_new_prefix);
                    //the stopwatch value has changed -> I update the time object in the Sight
                    time = master_clock_date_and_hour;
                    
                    items.erase(find(items.begin(), items.end(), all_items[7]));
                    
                }
                
                if((use_stopwatch == Answer('y', new_new_prefix)) && (old_use_stopwatch == Answer('n', new_new_prefix))){
                    //in this case,  old use_stopwatch = n, while the new (mofidied) use_stopwatch = y -> I add the stopwatch reading (all_items[7]) in items right after the entry "use of stopwatch" (all_items[6]), and let the user enter the stopwatch reading
                    
                    stopwatch.enter(String("stopwatch"), new_new_prefix);
                    //the stopwatch value has changed -> I update the time object in the Sight
                    time = master_clock_date_and_hour;
                    time.add(stopwatch);
                    
                    items.insert(find(items.begin(), items.end(), all_items[6])+1, String(all_items[7]));
                    
                }
                
                time.add(TAI_minus_UTC);
                time.print(String("TAI date and hour of sight"), new_prefix, cout);
                
                
                cout << new_prefix.value << "Stopwatch reading modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New use of stopwatch is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
        case 7:{
            //in this case I modify stopwatch reading
            
            Chrono old_stopwatch;
            
            old_stopwatch = stopwatch;
            
            stopwatch.print(String("old stopwatch reading"), new_new_prefix, cout);
            stopwatch.enter(String("new stopwatch reading"), new_new_prefix);
            
            if(old_stopwatch != stopwatch){
                
                //the stopwatch value has changed -> I update the time object in the Sight
                time = master_clock_date_and_hour;
                time.add(stopwatch);
                
                cout << new_prefix.value << "Stopwatch reading modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New stopwatch reading is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
        case 8:{
            //in this case I modify the label
            
            String old_label;
            
            old_label = label;
            
            label.print(String("old label"), new_new_prefix, cout);
            label.enter(String("new label"), new_new_prefix);
            
            if(old_label != label){
                
                cout << new_prefix.value << "Label modified\n";
                
            }else{
                
                check &= false;
                cout << new_new_prefix.value << YELLOW << "New label is equal to old one!\n" << RESET;
                
            }
            
        }
            break;
            
            
            //the # of related route cannot be modified because it is automatically assigned.
            /*
             case 9:{
             //in this case I modify the # of related route
             
             Int old_related_route;
             
             old_related_route = related_route;
             
             related_route.print(String("old # of related route"), new_new_prefix, cout);
             related_route.enter(String("new # of related route"), new_new_prefix);
             
             if(old_related_route != related_route){
             
             cout << new_prefix.value << "# of related route modified\n" << new_prefix.value << YELLOW << "Beware: route # " << related_route.value << " has not been automatically pointed to this sight!\n" << RESET;
             
             }else{
             
             check &= false;
             cout << new_new_prefix.value << YELLOW << "New # of related route is equal to old one!\n" << RESET;
             
             }
             
             }
             break;
             */
            
            
    }
    
    if(check){
        
        cout << new_prefix.value << "Item modified\n";
        
    }else{
        
        cout << new_prefix.value << RED << "I could not modify item!\n" << RESET;
        
    }
    
    cout << new_prefix.value << "Items after modification:\n";
    for(i=0; i<items.size(); i++){
        cout << new_prefix.value << "\t(" << i+1 << ") " << (items[i]).value << "\n";
    }
    
    
    return check;
    
}

void Route::print(String name, String prefix, ostream& ostr){
    
    String s, new_prefix;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    ostr << prefix.value << name.value << ":\n";
    
    type.print(String("type"), new_prefix, ostr);
    
    if((type == String("l")) || (type == String("o"))){
        
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
        check = ((type == String("l")) || (type == String("o")) || (type == String("c")));
        if(!check){
            cout << new_prefix.value << RED << "\tEntered value of type is not valid!\n" << RESET;
        }
    }while(!check);
    
    if((type == String("l")) || (type == String("o"))){
        //if the route is a loxodrome or an orthodrome, I enter its starting point and  starting heading (the ground position GP and aperture angle remain unused)
        
        start.enter(String("starting position"), new_prefix);
        alpha.enter(String("starting heading"), new_prefix);
        l.enter(String("length"), String("nm"), new_prefix);
        
        
    }else{
        //if the route is a circle of equal altitude, I enter its ground position and its aperture angle (alpha remains unused) ...
        GP.enter(String("ground position"), new_prefix);
        omega.enter(String("aperture angle"), new_prefix);
        
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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
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
    if(unit == String("m")){
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
            
        }while(((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));
        
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
    
    
    if(type == String("star")){
        RA.read_from_file(String("right ascension"), file, false, new_prefix);
        d.read_from_file(String("declination"), file, false, new_prefix);
    }else{
        radius.read_from_file(String("radius"), file, false, new_prefix);
    }
    
}



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
        temp_label << label.value << ", tr. w. " << transporting_route.type.value << ", COG = " << transporting_route.alpha.to_string(display_precision) << ", l = " << transporting_route.l.value << " nm";
        label.set(String(""), temp_label.str(), prefix);
        //given that I transported the Route object, this object is no longer directly connected to its Sight object, thus I set
        related_sight = -1;
        
        print(String("transported route"), prefix, cout);
        
    }else{
        
        cout << prefix.value << RED << "I cannot transport routes different from circles of equal altitude\n" << RESET;
        
    }
    
}

void Sight::add_to_wxListCtrl(wxListCtrl* listcontrol){
    
    wxListItem item;
    unsigned int i;
    
    i = (listcontrol->GetItemCount());
    
    item.SetId(i);
    item.SetText(wxT(""));
    
    listcontrol->InsertItem(item);
    
    //set body column
    listcontrol->SetItem(i, 0, wxString(body.name.value));
    
    //set limb column
    if((body.name == String("sun")) || (body.name == String("moon"))){
        
        if(wxString(limb.value) == 'u'){listcontrol->SetItem(i, 1, wxString("upper"));}
        if(wxString(limb.value) == 'l'){listcontrol->SetItem(i, 1, wxString("lower"));}
        if(wxString(limb.value) == 'c'){listcontrol->SetItem(i, 1, wxString("center"));}
        
    }else{
        listcontrol->SetItem(i, 1, wxString(""));
    }
    
    //set artificial horizon column
    listcontrol->SetItem(i, 2, wxString(artificial_horizon.value));
    
    
    //set sextant altitude column
    listcontrol->SetItem(i, 3, wxString((H_s).to_string(display_precision)));
    
    //set index error
    listcontrol->SetItem(i, 4, wxString((index_error).to_string(display_precision)));
    
    //set height of eye column
    if(artificial_horizon.value == 'n'){listcontrol->SetItem(i, 5, wxString(height_of_eye.to_string(String("m"), display_precision)));}
    else{listcontrol->SetItem(i, 5, wxString(""));}
    
    //set column of master-clock date and hour of sight
    listcontrol->SetItem(i, 6, wxString((time).to_string(display_precision)));
    
    //set use of stopwatch
    listcontrol->SetItem(i, 7, wxString((use_stopwatch.value)));
    
    //set stopwatch reading
    if((use_stopwatch.value) == 'y'){
        listcontrol->SetItem(i, 8, wxString((stopwatch).to_string(display_precision)));
    }else{
        listcontrol->SetItem(i, 8, wxString(""));
    }
    
    //set TAI-UTC
    listcontrol->SetItem(i, 9, wxString((TAI_minus_UTC).to_string(display_precision)));
    
    //set label
    listcontrol->SetItem(i, 10, wxString((label).value));

    
}


//this function returns true if the reading operation has been performed without errors, false otherwise
bool Sight::read_from_file(File& file, String prefix){
    
    string line;
    bool check = true;
    String new_prefix;
    //this unsigned int counts how many additional entries have been inserted into the vector item
    unsigned int additional_items;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    additional_items = 0;
    
    body.read_from_file(String("body"), file, new_prefix);
    if(body.type.value != "star"){
        items.insert(items.begin()+1+(additional_items++), all_items[1]);
        limb.read_from_file(String("limb"), file, false, new_prefix);
    }
    H_s.read_from_file(String("sextant altitude"), file, false, new_prefix);
    index_error.read_from_file(String("index error"), file, false, new_prefix);
    artificial_horizon.read_from_file(String("artificial horizon"), file, false, new_prefix);
    if(artificial_horizon == Answer('n', new_prefix)){
        items.insert(items.begin()+3+(additional_items++), String("height of eye"));
        height_of_eye.read_from_file(String("height of eye"), file, false, new_prefix);
    }
    
    check &= master_clock_date_and_hour.read_from_file(String("master-clock date and hour of sight"), file, new_prefix);
    if(!check){
        cout << prefix.value << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
    }
    time = master_clock_date_and_hour;
    
    use_stopwatch.read_from_file(String("use of stopwatch"), file, false, new_prefix);
    
    if(use_stopwatch == Answer('y', new_prefix)){
        
        items.insert(items.begin()+5+(additional_items++), String("stopwatch reading"));
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
    (related_route.value) = -1;
    
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
        temp << "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/" << body.name.value << ".txt";
    }else{
        temp << "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/j2000_to_itrf93.txt";
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
    if(artificial_horizon == Answer('n', new_prefix)){
        height_of_eye.print(String("height of eye"), String("m"), new_prefix, ostr);
    }
    master_clock_date_and_hour.print(String("master-clock date and hour of sight"), new_prefix, ostr);
    use_stopwatch.print(String("use of stopwatch"), new_prefix, ostr);
    if(use_stopwatch == Answer('y', new_prefix)){
        stopwatch.print(String("stopwatch"), new_prefix, ostr);
    }
    TAI_minus_UTC.print(String("TAI - UTC at time of master-clock synchronization with UTC"), new_prefix, ostr);
    
    label.print(String("label"), new_prefix, ostr);
    
    if(((related_route.value) != -1) && (&ostr == &cout)){
        ostr << new_prefix.value << "# of related route = " << (related_route.value)+1 << "\n";
    }
    
    /*
     cout << RED << "items:\n";
     for(unsigned int i=0; i<items.size(); i++){
     cout << new_prefix.value << items[i].value << "\n";
     
     }
     cout << RESET << "\n";
     */
    
    
}

class Plot{
    
public:
    Catalog* catalog;
    File file_init, file_id, file_gnuplot, file_kml, file_boundary;
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
    bool modify_sight(unsigned int, String);
    void transport_route(unsigned int, String);
    void add_position(String);
    void add_route(String);
    void remove_sight(unsigned int, String);
    void transport_position(unsigned int, String);
    void remove_position(unsigned int, String);
    void remove_route(unsigned int, String);
    bool read_from_file(File&, String);
    void print(bool, String, ostream&);
    void print_to_kml(String);
    void print_sights(String, ostream&);
    void print_positions(String, ostream&);
    void print_routes(bool, String, ostream&);
    void show(bool, String);
    void menu(String);
    void compute_crossings(String);
    
};

// this function plots the Routes of type String("c") in route_list in kml format
void Plot::print_to_kml(String prefix){
    
    stringstream line_ins, /*plot_title contains the  title of the Route to be plotted*/ plot_title;
    string line;
    unsigned int i, j;
    Int n_points_routes;
    double lambda_kml, phi_kml;
    String new_prefix;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    file_init.open(String("in"), prefix);
    file_kml.remove(prefix);
    
    //replace line with number of points for routes in plot_dummy.plt
    cout << prefix.value << YELLOW << "Reading number of points for routes from file " << file_init.name.value << " ...\n" << RESET;
    plot_command.str("");
    command.str("");
    n_points_routes.read_from_file(String("number of points for routes"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    
    
    //plot routes
    
    plot_command.str("");
    command.str("");
    for(i=0; i<(route_list.size()); i++){
        
        if(route_list[i].type == String("c")){
            
            //this is the opening of a path code in kml format
            plot_command << "\\\t<Style id=\\\"" << i << "\\\">\\\n\\\t<LineStyle>\\\n\\\t\\\t<color>" << /*I use the remainder of i in this way, so if i > size of kml_colors, I start back reading from the beginning of kml_colors*/ kml_colors[i % (sizeof(kml_colors)/sizeof(*kml_colors))] << "<\\/color>\\\n\\\t\\\t<width>2<\\/width>\\\n\\\t<\\/LineStyle>\\\n\\\t<\\/Style>\\\n\\\t<Placemark>\\\n\\\t\\\t<name>"
            << (route_list[i]).label.value
            << "<\\/name>\\\n\\\t\\\t<styleUrl>#" << i << "<\\/styleUrl>\\\n\\\t\\\t<description>"
            << (route_list[i]).label.value
            << "<\\/description>\\\n\\\t\\\t<LineString>\\\n\\\t\\\t\\\t<extrude>1<\\/extrude>\\\n\\\t\\\t\\\t<tessellate>0<\\/tessellate>\\\n\\\t\\\t\\\t<altitudeMode>absolute<\\/altitudeMode>\\\n\\\t\\\t\\\t<coordinates>\\\n";
            
            
            
            for(j=0; j<(unsigned int)(n_points_routes.value); j++){
                
                //I set equal to a temporary value of the length of the route, which spans between 0 and 2.0*M_PI*(Re*sin(((route_list[i]).omega.value))) across the for loop over j
                ((route_list[i]).l).set(String(""), 2.0*M_PI*(Re*sin(((route_list[i]).omega.value)))*((double)j)/((double)(n_points_routes.value-1)), new_prefix);
                
                //I compute the coordinate of the endpoint of route_list[i] for the ((route_list[i]).l) above
                (route_list[i]).compute_end(new_prefix);
                
                //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude, and in the third altitude (I am not interested in altitude, thus is set it to 0); The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.
                
                lambda_kml = -K*((route_list[i]).end.lambda.value);
                if(lambda_kml < -180.0){
                    lambda_kml += 360.0;
                }
                
                phi_kml = K*((route_list[i]).end.phi.value);
                if(phi_kml > 270.0){
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
    for(i=0; i<(position_list.size()); i++){
        
        //this is the opening of a path code in kml format
        plot_command << "\\\n\\\t<Placemark>\\\n\\\t\\\t<Style>\\\n\\\t\\\t\\\t<IconStyle>\\\n\\\t\\\t\\\t\\\t<color>7733ff66<\\/color>\\\n\\\t\\\t\\\t\\\t<IconStyleSimpleExtensionGroup radius=\\\"3\\\" points=\\\"Infinity\\\" strokeColor=\\\"" << /*I use the remainder of i in this way, so if i > size of hex_colors, I start back reading from the beginning of hex_colors*/ hex_colors[i % (sizeof(hex_colors)/sizeof(*hex_colors))] << "\\\" strokeWidth=\\\"2\\\" lineDash=\\\"undefined\\\"\\/>\\\n\\\t\\\t\\\t<\\/IconStyle>\\\n\\\t\\\t\\\t<PolyStyle xmlns=\\\"\\\">\\\n\\\t\\\t\\\t\\\t<outline>0<\\/outline>\\\n\\\t\\\t\\\t<\\/PolyStyle>\\\n\\\t\\\t<\\/Style>\\\n\\\t\\\t<description>"
        << (position_list[i]).label.value << "<\\/description>\\\n\\\t\\\t<ExtendedData>\\\n\\\t\\\t\\\t<Data name=\\\"scStyle\\\"\\/>\\\n\\\t\\\t<\\/ExtendedData>\\\n\\\t\\\t<Point>\\\n\\\t\\\t\\\t<coordinates>";
        
        
        //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude. The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.
        
        lambda_kml = -K*((position_list[i]).lambda.value);
        if(lambda_kml < -180.0){
            lambda_kml += 360.0;
        }
        
        phi_kml = K*((position_list[i]).phi.value);
        if(phi_kml > 270.0){
            phi_kml -= 360.0;
        }
        
        plot_command << lambda_kml << "," << phi_kml << "<\\/coordinates>\\\n\\\t\\\t<\\/Point>\\\n\\\t<\\/Placemark>";
        
    }
    
    //add the line to plot_kml.kml which contains the parametric plot of the positions
    command << "LANG=C sed 's/\\/\\/position\\_plots/" << plot_command.str().c_str() << "/g' kml_temp.kml >> " << file_kml.name.value << "\nrm -rf kml_temp.kml\n";
    
    //execute the command string
    system(command.str().c_str());
    
    
    
    file_init.close(prefix);
    
}


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
                    (sight_list[sight_list.size()-1].related_route.value) = route_list.size()-1;
                    
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
    for(i=11; i<15; i++){
        cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
    }
    
    
    cout << prefix.value << BOLD << "Files:" << RESET << "\n";
    for(i=15; i<17; i++){
        cout << new_prefix.value << "(" << i+1 << ") " << (choices[i]).value << "\n";
    }
    
    i=17;
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
                
                modify_sight(i, new_prefix);
                print(true, new_prefix, cout);
                show(false, new_prefix);
                
            }else{
                
                cout << RED << "There are no sights to modify!\n" << RESET;
                
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
                cout << RED << "There are no sights to delete!\n" << RESET;
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
                cout << RED << "There are no positions to modify!\n" << RESET;
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
                cout << RED << "There are no positions to transport!\n" << RESET;
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
                cout << RED << "There are no positions to delete!\n" << RESET;
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
                
                cout << RED << "There are no routes to transport!\n" << RESET;
                
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
                
                cout << RED << "There are not enough routes to compute crossings!\n" << RESET;
                
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
                cout << RED << "There are no routes to delete!\n" << RESET;
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
                
                cout << RED << "There are no routes nor positions to clear!\n" << RESET;
                
            }
            
            menu(prefix);
            
        }
            break;
            
            
        case 15:{
            
            unsigned int i, j;
            
            for(i=0, j=0; i<route_list.size(); i++){
                if((route_list[i]).type == String("c")){j++;}
            }
            
            if(j > 0){
                
                Position p, q;
                Angle /*this is the value of the parametric angle t which corresponds to the point on Route (route_list[i]) closest to q*/t, /*this is the azimuth of the line of position*/Z;
                
                print_routes(false, new_prefix, cout);
                cout << new_prefix.value << "Enter the # of the route that you want to transform into a line of position:";
                cin >> i;
                i--;
                
                q.enter(String("assumed position"), new_prefix);
                (route_list[i]).closest_point_to(&p, &t, q, new_prefix);
                p.label.set(String(""), String("closest point to assumed position"), new_prefix);
                
                Z.set(String(""), acos( cos(((route_list[i]).GP.phi.value))*cos((p.phi.value))*sin((t.value)) + (cos(((route_list[i]).GP.lambda.value) - (p.lambda.value))*sin(((route_list[i]).GP.phi.value))*sin((t.value)) - cos((t.value))*sin(((route_list[i]).GP.lambda.value) - (p.lambda.value)))*sin((p.phi.value)) ), new_prefix);
                
                if( -(cos(((route_list[i]).GP.phi.value))*cos((t.value))*cot(((route_list[i]).omega.value))) - sin(((route_list[i]).GP.phi.value)) > 0.0 ){(Z.value) = 2.0*M_PI - (Z.value);}
                Z.normalize();
                Z.print(String("azimuth"), new_prefix, cout);
                
                //
                //create a loxodrome Route through p with azimuth Z
                Route route;
                route.type = String("l");
                route.start = p;
                route.alpha = Z;
                route.l.value = 100.0;
                route.label = String("LOP");
                route.related_sight = -1;
                
                route_list.push_back(route);
                //
                
                position_list.push_back(q);
                position_list.push_back(p);
                
                print(true, new_prefix, cout);
                show(false, new_prefix);
                
            }else{
                
                cout << RED << "There are no circles of equal altitude!\n" << RESET;
                
            }
            
            menu(prefix);
            
        }
            break;
            
            
        case 16:{
            
            if(sight_list.size() + route_list.size() + position_list.size() > 0){
                
                File file;
                stringstream temp, temp_kml;
                Answer answer;
                
                //answer says whether one should create a new file on which the data will be saved
                answer.set(String(""), 'y', new_prefix);
                
                
                file.name.enter(String("name of data file (without extension)"), new_prefix);
                //add the extension .sav to name of sav file and the extension .kml to the name kml file
                temp.str("");
                temp << file.name.value << ".sav";
                temp_kml << file.name.value << ".kml";
                file.set_name(temp.str());
                file_kml.set_name(temp_kml.str());
                
                //If the file already exists, I ask whether the user want to overwrite it
                if((file.check_if_exists(new_prefix))){
                    
                    cout << new_prefix.value << YELLOW << "You may be overwriting data!\n" << RESET;
                    answer.enter(String("whether you want to overwrite the file"), new_prefix);
                    
                }
                
                if(answer == Answer('y', new_prefix)){
                    
                    file.remove(new_prefix);
                    file.open(String("out"), new_prefix);
                    //save everything to data file
                    print(false, new_prefix, file.value);
                    //save everything to kml file
                    print_to_kml(new_prefix);
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
            
        case 17:{
            
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
            
            
        case 18:{
            
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
    
    file_init.set_name(String(path_file_init));
    file_id.set_name(String("job_id.txt"));
    file_gnuplot.set_name(String("plot.plt"));
    file_boundary.set_name(String("boundary.txt"));
    file_boundary.remove(prefix);
    
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
    Angle min_crossing_angle;
    double x;
    Route error_circle;
    
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    cout << prefix.value << YELLOW << "Reading minimal crossing angle between circles of equal altitude from file " << file_init.name.value << " ...\n" << RESET;
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
            
            if((route_list[crossing_route_list[i]]).crossing((route_list[crossing_route_list[j]]), &q_temp, &x, new_prefix)){
                //in this case, the two routes under consideration intercept with no error message
                
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
        
        //if print_all_routes = false, I only print routes which are not linked to a sight. This is to avoid doubles: If I print also Routes which are related to a Sight, then when the file to which I am saving will be read again, the code will reduce them and create double Routes identical to the ones already present in the file.
        if((((route_list[i]).related_sight) == -1) || print_all_routes){
            
            name.str("");
            name << "Route #" << j+1;
            
            (route_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
            
            j++;
            
        }
        
    }
    
    
}


bool Plot::modify_sight(unsigned int i, String prefix){
    
    bool check;
    
    check = true;
    
    check &= ((sight_list[i]).modify((*catalog), prefix));
    
    if(check){
        
        check &= ((sight_list[i]).reduce(&(route_list[(sight_list[i]).related_route.value]), prefix));
        
    }
    
    
    if(check){
        
        (sight_list[i]).print(String("Sight modified"), prefix, cout);
        
    }else{
        
        cout << prefix.value << RED << "I could not modify sight!\n" << RESET;
        
    }
    
    return check;
    
}



bool Plot::add_sight(String prefix){
    
    //create a new sight and new route in the respective lists
    sight_list.resize(sight_list.size()+1);
    route_list.resize(route_list.size()+1);
    
    bool check = true;
    
    (sight_list[sight_list.size()-1]).enter((*catalog), String("new sight"), prefix);
    check &= ((sight_list[sight_list.size()-1]).reduce(&(route_list[route_list.size()-1]), prefix));
    
    //I link the sight to the route, and the route to the sight
    ((sight_list[sight_list.size()-1]).related_route.value) = route_list.size()-1;
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
    Int i_related_route;
    
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
    
    
    if((i_related_route.value) != -1){
        
        remove_related_route.enter(String("whether you want to remove the route related to this sight"), prefix);
        if(remove_related_route == Answer('y', prefix)){
            
            remove_route((i_related_route.value), prefix);
            
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
        
        if((((sight_list[j]).related_route.value) != -1) && (((sight_list[j]).related_route.value) >= ((int)i))){
            
            if(((sight_list[j]).related_route.value) == ((int)i)){
                ((sight_list[j]).related_route.value) = -1;
            }else{
                ((sight_list[j]).related_route.value)--;
            }
            
        }
        
    }
    
    cout << prefix.value << "Route removed.\n";
    
    
    if(i_related_sight != -1){
        
        remove_related_sight.enter(String("whether you want to remove the sight related to the route"), prefix);
        
        if(remove_related_sight == Answer('y', prefix)){
            
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
    if(keep_original == Answer('y', new_prefix)){
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
    if(keep_original == Answer('y', new_prefix)){
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
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    file_init.open(String("in"), prefix);
    
    
    T = gsl_root_fsolver_brent;
    s = gsl_root_fsolver_alloc(T);
    
    
    
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
    
    
    //replace line with window size in plot_dummy.plt
    cout << prefix.value << YELLOW << "Reading width and height of plot window from file " << file_init.name.value << " ...\n" << RESET;
    width_plot_window.read_from_file(String("width of plot window"), file_init, true, new_prefix);
    height_plot_window.read_from_file(String("height of plot window"), file_init, true, new_prefix);
    command.str("");
    command << "LANG=C sed 's/#window size/set terminal qt size " << width_plot_window.value << "," << height_plot_window.value << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    system(command.str().c_str());
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
                plot_command << "[0.:" << (route_list[i]).l.value << "] \"+\" u (xe(K*lambda_lox(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) : (ye(K*phi_lox(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) " << plot_style.str()  << " ti \"type = " << (route_list[i]).type.value << ", start = " << (route_list[i]).start.to_string(display_precision) << ", heading = " << (route_list[i]).alpha.to_string(display_precision) << "\"";
                
                break;
            }
                
            case 'o':
                //plot an orthodrome
            {
                
                //I assume that the orthordrome is not cut through the meridian lambda = M_PI, and I make a single plot
                plot_command << "[0.:" << (route_list[i]).l.value << "] \"+\" u (xe(K*lambda_ort(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) : (ye(K*phi_ort(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) " << plot_style.str()  << " ti \"type = " << (route_list[i]).type.value << ", start = " << (route_list[i]).start.to_string(display_precision) << ", heading = " << (route_list[i]).alpha.to_string(display_precision) << "\"";
                
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
    //here -0+0 is to open the xterm window on the top right edge of the screen
    command << "gnuplot '" << ((file_gnuplot.name).value) << "' & \n echo $! >> " << ((file_id.name).value) << "\n";
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
    gsl_root_fsolver_free(s);
    
    cout << prefix.value << "Job id = "<< job_id << "\n";
    
}

bool Sight::enter(Catalog catalog, String name, String prefix){
    
    //pointer to init.txt to read fixed sight data from in there
    File file_init;
    String new_prefix;
    bool check = true;
    //this unsigned int counts how many additional entries have been inserted into the vector item
    unsigned int additional_items;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    additional_items = 0;
    
    file_init.set_name(String(path_file_init));
    check &= (file_init.open(String("in"), prefix));
    
    cout << prefix.value << "Enter " << name.value << ":\n";
    
    body.enter(String("body"), catalog, new_prefix);
    //GP.label.set("geographic position", new_prefix);
    
    if(body.type.value != "star"){
        items.insert(items.begin()+1+(additional_items++), all_items[1]);
        
        limb.enter(String("limb"), new_prefix);
    }
    H_s.enter(String("sextant altitude"), new_prefix);
    //read index error from init file
    cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
    index_error.read_from_file(String("index error"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    cout << prefix.value << YELLOW << "Reading artificial horizon from file " << file_init.name.value << " ...\n" << RESET;
    artificial_horizon.read_from_file(String("artificial horizon"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    if(artificial_horizon == Answer('n', new_prefix)){
        items.insert(items.begin()+3+(additional_items++), String("height of eye"));
        height_of_eye.enter(String("height of eye"), String("m"), new_prefix);
    }
    
    do{
        
        master_clock_date_and_hour.enter(String("master-clock date and hour of sight"), new_prefix);
        time = master_clock_date_and_hour;
        
        cout << prefix.value << YELLOW << "Reading use of stopwatch from file " << file_init.name.value << " ...\n" << RESET;
        use_stopwatch.read_from_file(String("use of stopwatch"), file_init, true, new_prefix);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        
        if(use_stopwatch == Answer('y', new_prefix)){
            
            stopwatch.enter(String("stopwatch reading"), new_prefix);
            time.add(stopwatch);
            
        }
        
        //read TAI_minus_UTC from /Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/index.txt
        cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
        TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        time.add(TAI_minus_UTC);
        time.print(String("TAI date and hour of sight"), new_prefix, cout);
        
    }while(!check_data_time_interval(prefix));
    
    
    //if the sight has use_stopwatch = 'y', then I add to the list of its items the stopwatch reading
    if(use_stopwatch == Answer('y', new_prefix)){
        items.insert(items.begin()+5+(additional_items++), String("stopwatch reading"));
    }
    
    label.enter(String("label"), new_prefix);
    
    //given that the sight is not yet linked to a route, I set
    (related_route.value) = -1;
    
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
    
    cout << prefix.value << "Reducing sight ...\n";
    
    ((*circle_of_equal_altitude).type.value) = 'c';
    
    compute_H_a(new_prefix);
    check &= get_coordinates(circle_of_equal_altitude, new_prefix);
    
    //link the circle of equal altitude (*circle_of_equal_altitude) to sight (*this)
    temp <<  (*this).body.name.value << " " << (*this).time.to_string(display_precision) << " TAI, " << (*this).label.value;
    ((*circle_of_equal_altitude).label).set(String(""), String(temp.str()), new_prefix);
    
    check &= compute_H_o(new_prefix);
    ((*circle_of_equal_altitude).omega.value) = M_PI/2.0 - (H_o.value);
    
    if(!check){
        
        cout << prefix.value << RED << "Sight cannot be reduced!\n" << RESET;
        
    }else{
        
        cout << prefix.value << "... done\n";
        
    }
    
    return check;
    
}


void Sight::compute_H_a(String prefix){
    
    if(artificial_horizon == Answer('y', prefix)){
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
    
    cout << prefix.value << "Computing observed altitude ...\n";
    
    check &= compute_DH_refraction(new_prefix);
    
    if(check){
        compute_DH_parallax_and_limb(new_prefix);
        H_o = H_a + DH_refraction + DH_parallax_and_limb;
        cout << prefix.value << "...done\n";
        H_o.print(String("observed altitude"), prefix, cout);
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
    
    if(type == String("star")){
        RA.print(String("Right ascension"), new_prefix, ostr);
        d.print(String("Declination"), new_prefix, ostr);
    }else{
        radius.print(String("Radius"), String("nm"), new_prefix, ostr);
    }
    
}


void Body::enter(String name, Catalog catalog, String prefix){
    
    unsigned int i;
    string s;
    String new_prefix;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    
    do{
        
        
        cout << prefix.value << "Enter " << name.value << ":";
        s.clear();
        getline(cin >> ws, s);
        
        ((*this).name) = String(s);
        
    }while(!check(&i, catalog, prefix));
    
    (*this) = (catalog.list)[i];
    
    print(name, prefix, cout);
    
}

//this function checks whether the Body's name  is found in the body list comprised in catalog
bool Body::check(unsigned int* j, Catalog catalog, String prefix){
    
    unsigned int i;
    bool check;
    
    for(i=0, check=false; (i<(catalog).list.size()) && (!check); i++){
        
        if(((((catalog).list)[i]).name) == name){
            check=true;
        }
        
    }
    
    if(check){
        (*j)=i-1;
    }else{
        cout << prefix.value << RED << "Body not found in catalog!\n" << RESET;
    }
    
    return check;
    
}





Sight::Sight(void){
    
    //this is the list of all the possible items that a Sight object can have: some Sight objects may have an item list with fewer elements than all_items. For instance, a star Sight does not have the "limb" element.
    all_items.push_back(String("body"));
    all_items.push_back(String("limb"));
    all_items.push_back(String("sextant altitude"));
    all_items.push_back(String("artificial horizon"));
    all_items.push_back(String("height of eye"));
    all_items.push_back(String("master-clock date and hour of sight"));
    all_items.push_back(String("use of stopwatch"));
    all_items.push_back(String("stopwatch reading"));
    all_items.push_back(String("label"));
    
    items.push_back(all_items[0]);
    items.push_back(all_items[2]);
    items.push_back(all_items[3]);
    items.push_back(all_items[5]);
    items.push_back(all_items[6]);
    items.push_back(all_items[8]);
    
    //initiazlie the limb to a 'n/a' value
    limb.value = 'n';
    atmosphere.set();
    (related_route.value) = -1;
    
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
    if(unit == String("nm")){
        temp  << " [nm]";
    }else{
        temp << " [m]";
    }
    
    do{
        
        enter_double(&value, false, 0.0, 0.0, temp.str(), prefix);
        
    }while(!check_valid(name, prefix));
    
    //if the length has been entered in units of m, convert it to nautical miles
    if(unit == String("m")){
        value/=(1e3*nm);
    }
    
    print(name, unit, prefix, cout);
    
}

string Length::to_string(String unit, unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    if(unit == String("nm")){output << value << " nm";}
    if(unit == String("m")){output << value*1e3*nm << " m";}

    return (output.str().c_str());
    
}

void Length::print(String name, String unit, String prefix, ostream& ostr){
    
    if((name.value) != ""){
        
        ostr << prefix.value << name.value << " = ";
        if(unit == String("nm")){
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
    
    cout << prefix.value << "Fetching ephemerides' data ...\n";
    
    if((body.type.value) != "star"){
        filename << "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/" << body.name.value << ".txt";
    }else{
        filename << "/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/j2000_to_itrf93.txt";
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
        
        //set the length of the circle of equal altitude
        //(*circle_of_equal_altitude).l.set(String("length of circle of equal altitude"), 2.0*M_PI*Re*sin((*circle_of_equal_altitude).omega.value), new_prefix);
        
    }else{
        check &= false;
    }
    
    if(!check){
        
        cout << prefix.value << RED << "Cannot fetch ephemerides' data!\n" << RESET;
        
    }else{
        
        cout << prefix.value << "...done\n";
        
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

//this fucnction takes an angle entered from keyboard. the angle may or may not contain a sign in front of it. If it does not contain a sign, the sign is interpreted as '+'
void Angle::enter(String name, String prefix){
    
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
    
    
    
    input_unit.clear();
    input_unit << name.value << " [(s )ddd mm.m]";
    
    do{
        
        check = true;
        
        //enter the input string, containing degrees and arcminutes with a ' ' in between
        cout << prefix.value << "Enter " << input_unit.str() << ":";
        getline(cin >> ws, input);
        
        //find the position of the first ' '
        pos = (input).find(" ");
        
        if(( ((input).substr(0, pos)) == "+" ) || ( ((input).substr(0, pos)) == "-" )){
            //in this case, there is a '+' or '-' sign in front the angle
            
            //check whether the string contains only two ' ', one after the sign, one adter the degrees
            check &= (count((input).begin(), (input).end(), ' ') == 2);
            
            //set the sign
            s = ((input).substr(0, pos).c_str())[0];
            
            //now I am no longer interested in the sign, the string runs from the degrees to the end of arcminutes
            (input)  =  ((input).substr(pos+1).c_str());
            
        }else{
            //in this case, there is no '+' nor '-' in front of the angle -> the sign is set to '+'
            
            //set the sign
            s = '+';
            
            cout << prefix.value << YELLOW << "No sign has been entered. Interpreting it as +!\n" << RESET;
            
            //check whether the string contains only one ' ',  after the degrees
            check &= (count((input).begin(), (input).end(), ' ') == 1);
            
        }
        
        if(check){
            
            //find the position of the  ' ' between degrees and arcminutes
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
    ostr << prefix.value << name.value << " = " << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60.0 << "'\n";
    
}

//convert the angle contained in (*this).value to degrees and minutes format, storted in deg and min
void Angle::to_deg_min(unsigned int* deg, double* min){
    
    normalize();
    (*deg) = (unsigned int) floor(K*value - 360.0*floor(K*value/360.0));
    (*min) = (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60.0;
    
}

//convert the angle stored in degrees and minutes format in deg an min in to (*this).vaule
void Angle::from_sign_deg_min(char sign, unsigned int deg, double min){
    
    value = k*(((double)deg) + min/60.0);
    if(sign == '-'){value*=-1.0;}
    
    normalize();
    
}


string Angle::to_string(unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    normalize();
    output << floor(K*value - 360.0*floor(K*value/360.0)) << "° " << (K*value - 360.0*floor(K*value/360.0) - floor(K*value - 360.0*floor(K*value/360.0))) * 60 << "'";
    
    return (output.str().c_str());
    
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




string Chrono::to_string(unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    if(h<10){output << 0;}
    output << h << ":";
    if(m<10){output << 0;}
    output << m << ":";
    if(s<10.0){output << 0;}
    output << s;
    
    return (output.str().c_str());
    
}

void Date::print(String name, String prefix, ostream& ostr){
    
    ostr << prefix.value << name.value << " = " << to_string() << "\n";
    
};

//this function sets (*this) to the current UTC date and time
bool Time::set_current(String prefix){
    
    String new_prefix;
    bool check;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    check=true;
    
    check &= (date.set_current(new_prefix));
    check &= (chrono.set_current(new_prefix));

    return check;
    
}

//this function sets (*this) to the current UTC date
bool Date::set_current(String prefix){
    
    String new_prefix;
    stringstream line_ins;
    string input;
    File file_utc_date;
    size_t pos;
    bool check;
 
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
     
    check = true;

    file_utc_date.set_name(String(path_file_utc_date_and_time));
    file_utc_date.remove(prefix);
    
    line_ins.str("");
    line_ins << "date -u \"+%Y-%m-%d\"  >> " << path_file_utc_date_and_time;
    
    //execute the date command in the terminal and writes the UTC date to file_utc_date
    system(line_ins.str().c_str());
    
    //reads the utc date from file_utc_date
    cout << prefix.value << YELLOW << "Reading utc date from file " << file_utc_date.name.value << " ...\n" << RESET;
    
    check &= (file_utc_date.open(String("in"), new_prefix));

    if(check){
        
        getline(file_utc_date.value, input);
        
        //read the part of input containing the year
        pos = input.find("-");
        Y = stoi(input.substr(0, pos).c_str(), NULL, 10);
        
        //now I am no longer interested in the year, the string runs from the month to days
        input  =  (input.substr(pos+1).c_str());
        //find the position of the second '-'
        pos = input.find("-");
        //check whether month part is formatted correctly
        M = stoi(input.substr(0, pos).c_str(), NULL, 10);
        
        //now I am no longer interested in the month, the string runs from the days to the end of the string
        input  =  (input.substr(pos+1).c_str());
        D = stoi(input.c_str());
        
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        
    }
    
    file_utc_date.close(new_prefix);
    file_utc_date.remove(new_prefix);
    
    return check;

    
}

//this function sets (*this) to the current UTC time
bool Chrono::set_current(String prefix){
    
    stringstream line_ins;
    string input;
    File file_utc_time;
    size_t pos;
    String new_prefix;
    bool check;
 
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
     
    check = true;

    file_utc_time.set_name(String(path_file_utc_date_and_time));
    file_utc_time.remove(prefix);
    
    line_ins.str("");
    line_ins << "date -u \"+%H:%M:%S\"  >> " << path_file_utc_date_and_time;
    
    //execute the date command in the terminal and writes the UTC date to file_utc_time
    system(line_ins.str().c_str());
    
    //reads the utc date from file_utc_time
    cout << prefix.value << YELLOW << "Reading utc time from file " << file_utc_time.name.value << " ...\n" << RESET;
    
    check &= (file_utc_time.open(String("in"), new_prefix));

    if(check){
        
        getline(file_utc_time.value, input);
        
        //read the part of input containing the hour
        pos = input.find(":");
        h = stoi(input.substr(0, pos).c_str(), NULL, 10);
        
        //now I am no longer interested in the hour, the string runs from the minutes to seconds
        input  =  (input.substr(pos+1).c_str());
        //find the position of the second ':'
        pos = input.find(":");
        //check whether the minute part is formatted correctly
        m = stoi(input.substr(0, pos).c_str(), NULL, 10);
        
        //now I am no longer interested in the minute, the string runs from the days to the end of the string
        input  =  (input.substr(pos+1).c_str());
        s = stod(input.c_str());
        
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        
    }
    
    file_utc_time.close(new_prefix);
    file_utc_time.remove(new_prefix);
    
    return check;

    
}



void Date::enter(String name, String prefix) {
    
    string input;
    String new_prefix;
    bool check;
    size_t pos;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
        
    do{
        
        check = true;
        
        cout << prefix.value << "Enter " << name.value << " [YYYY MM DD], or press enter for current UTC date:";
        getline(cin, input);
        
        if(input.empty()){
            
            cout << prefix.value << YELLOW << "Entered an empty date, setting it to current UTC date!\n" << RESET;
            check &= set_current(new_prefix);
            print(String("entered date"), prefix, cout);
            
        }else{
            
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


string Date::to_string(void){
    
    stringstream output;
    
    output << Y << "-";
    if(M<10){output << 0;}
    output << M << "-";
    if(D<10){output << 0;}
    output << D;
    
    return (output.str().c_str());
    
}


void Chrono::print(String name, String prefix, ostream& ostr){
    
    unsigned int precision;
    
    //if I am printing to terminal, I print with display_precision. Otherwise, I print with data_precision
    if(ostr.rdbuf() == cout.rdbuf()){
        precision = display_precision;
    }else{
        precision = data_precision;
    }
    
    ostr << prefix.value << "hour of " << name.value << " = " << to_string(precision) << "\n";
    
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





