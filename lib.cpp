#include "lib.h"

bool String::operator==(const String& s){
    
    return((((*this).value) == (s.value)));
    
}

bool String::operator!=(const String& s){
    
    return(!((*this) == s));
    
}

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
    
    if(((*this) != String(""))){ostr << prefix.value << name.value << " = " << value << "\n";}
    
}

void String::set(String name, String input_string, String prefix){
    
    value = (input_string.value);
    if(!(name == String(""))){print(name, prefix, cout);}
    
}

bool Angle::operator==(const Angle& x){
    
    return((((*this).value) == (x.value)));
    
}

bool Angle::operator>(const Angle& x){
    
    return((((*this).value) > (x.value)));
    
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

//creates an element in listcontrol and writes into this element the values of all the fields containes in this
void Position::add_to_wxListCtrl(long list_position, wxListCtrl* listcontrol){
    
    unsigned int i;
    wxListItem item;
    
    if(list_position == -1){
        i = (listcontrol->GetItemCount());
    }else{
        i = list_position;
        listcontrol->DeleteItem(i);
    }
    
    item.SetId(i);
    item.SetText(wxT(""));
    
    listcontrol->InsertItem(item);
    
    //set latitude column
    listcontrol->SetItem(i, 0, wxString(phi.to_string(String("NS"), display_precision)));
    
    //set longitude column
    listcontrol->SetItem(i, 1, wxString(lambda.to_string(String("EW"), display_precision)));
    
    //set label column
    listcontrol->SetItem(i, 2, wxString(label.value));
    
}

//constructs a brand new Route object and thus sets its related sight to -1, because this Route is not related to any sight yet
Route::Route(void){
    
    related_sight.set(String(""), -1, String(""));
    
    
}

void Route::add_to_wxListCtrl(long list_position, wxListCtrl* listcontrol){
    
    unsigned int i, j;
    wxListItem item;
    
    if(list_position == -1){
        //in this case, I add a new element at the end of listcontrol
        
        i = (listcontrol->GetItemCount());
    }else{
        //in this case, I delete the i-th elment in listcontrol and replace it
        
        i = list_position;
        listcontrol->DeleteItem(i);
    }
    
    item.SetId(i);
    item.SetText(wxT(""));
    
    listcontrol->InsertItem(item);
    
    j=0;
    //set type column: I write the extended type names, not the short ones 'l', 'o' and 'c'
    if(type == String("l")){
        listcontrol->SetItem(i, j++, wxString("loxodrome"));
    }
    if(type == String("o")){
        listcontrol->SetItem(i, j++, wxString("orthodrome"));
    }
    if(type == String("c")){
        listcontrol->SetItem(i, j++, wxString("circle of equal altitude"));
    }
    
    if(type == String("c")){
        //in this case the type of this is 'circle of equal altitde': the first three fields are thus empty, and I fill in only the last two fields
        
        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));
        
        listcontrol->SetItem(i, j++, wxString(GP.to_string(display_precision)));
        listcontrol->SetItem(i, j++, wxString(omega.to_string(String(""), display_precision)));
        
    }else{
        //in this case the type of this is 'loxodrome' or 'orthodrome': the last two fields are empty, and I fill in only the first three fields
        
        listcontrol->SetItem(i, j++, wxString(start.to_string(display_precision)));
        listcontrol->SetItem(i, j++, wxString(alpha.to_string(String(""), display_precision)));
        listcontrol->SetItem(i, j++, wxString(l.to_string(String("nm"), display_precision)));
        
        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));
        
        
    }
    
    listcontrol->SetItem(i, j++, wxString(label.value));
    
    
    if((related_sight.value) == -1){
        //if the route is not connected to a sight, I leave the column field empty
        
        listcontrol->SetItem(i, j++, wxString(""));
        
    }else{
        //if the route is connected to a sight, I write the # of the related sight in the column field
        
        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_sight.value)+1));
        
    }
    
    
}

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
    (related_sight.value) = -1;
    
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
    
    //I deleted UTC at the end of string for the sake of shortness
    output << date.to_string() << " " << chrono.to_string(precision);
    
    return (output.str().c_str());
    
}

string Position::to_string(unsigned int precision){
    
    stringstream output;
    
    output << phi.to_string(String("NS"), precision) << " " << lambda.to_string(String("EW"), precision);
    
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
    ((route.related_sight).value) = -1;
    
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
    
    temp_label << label.value << "tr. w. " << route.type.value << ", COG = " << route.alpha.to_string(String(""), display_precision) << ", l = " << (route.l).value << " nm";
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


bool Body::operator==(const Body& body){
    
    return (name == (body.name));
    
}

bool Limb::operator==(const Limb& limb){
    
    return(value == limb.value);
    
}

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
                    time+=stopwatch;
                    
                }
                
                time+=TAI_minus_UTC;
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
                    time+=stopwatch;
                    
                    items.insert(find(items.begin(), items.end(), all_items[6])+1, String(all_items[7]));
                    
                }
                
                time+=TAI_minus_UTC;
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
                time+=stopwatch;
                
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
    
    
    if((related_sight.value) != -1){
        
        cout << new_prefix.value << "Related sight # = " << (related_sight.value)+1 << "\n";
        
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
    (related_sight.value) = -1;
    
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

void Time::operator += (const Chrono& chrono){
    
    to_MJD();
    MJD += (((double)(chrono.h)) + ((double)(chrono.m))/60.0 + ((double)(chrono.s))/(60.0*60.0))/24.0;
    to_TAI();
    
}



void Time::operator -= (const Chrono& chrono){
    
    to_MJD();
    MJD -= (((double)(chrono.h)) + ((double)(chrono.m))/60.0 + ((double)(chrono.s))/(60.0*60.0))/24.0;
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
        temp_label << label.value << ", tr. w. " << transporting_route.type.value << ", COG = " << transporting_route.alpha.to_string(String(""), display_precision) << ", l = " << transporting_route.l.value << " nm";
        label.set(String(""), temp_label.str(), prefix);
        //given that I transported the Route object, this object is no longer directly connected to its Sight object, thus I set
        (related_sight.value) = -1;
        
        print(String("transported route"), prefix, cout);
        
    }else{
        
        cout << prefix.value << RED << "I cannot transport routes different from circles of equal altitude\n" << RESET;
        
    }
    
}

void Sight::add_to_wxListCtrl(long list_position, wxListCtrl* listcontrol){
    
    unsigned int i, j;
    wxListItem item;
    Time time_UTC;
    
    if(list_position == -1){
        i = (listcontrol->GetItemCount());
    }else{
        i = list_position;
        listcontrol->DeleteItem(i);
    }
    
    item.SetId(i);
    item.SetText(wxT(""));
    
    listcontrol->InsertItem(item);
    
    j=0;
    //set body column
    listcontrol->SetItem(i, j++, wxString(body.name.value));
    
    //set limb column
    if((body.name == String("sun")) || (body.name == String("moon"))){
        
        if(wxString(limb.value) == 'u'){listcontrol->SetItem(i, j++, wxString("upper"));}
        if(wxString(limb.value) == 'l'){listcontrol->SetItem(i, j++, wxString("lower"));}
        if(wxString(limb.value) == 'c'){listcontrol->SetItem(i, j++, wxString("center"));}
        
    }else{
        listcontrol->SetItem(i, j++, wxString(""));
    }
    
    //set artificial horizon column
    listcontrol->SetItem(i, j++, wxString(artificial_horizon.value));
    
    //set sextant altitude column
    listcontrol->SetItem(i, j++, wxString((H_s).to_string(String(""), display_precision)));
    
    //set index error
    listcontrol->SetItem(i, j++, wxString((index_error).to_string(String(""), display_precision)));
    
    //set height of eye column
    if(artificial_horizon.value == 'n'){listcontrol->SetItem(i, j++, wxString(height_of_eye.to_string(String("m"), display_precision)));}
    else{listcontrol->SetItem(i, j++, wxString(""));}
    
    //set column of master-clock date and hour of sight
    //I add to master_clock_date_and_hour the value stopwatch (if any): I write the result in time_UTC and I write in the GUI object  time_UTC
    time_UTC = master_clock_date_and_hour;
    //    if((use_stopwatch.value)=='y'){time_UTC += stopwatch;}
    listcontrol->SetItem(i, j++, wxString(time_UTC.to_string(display_precision)));
    
    //set use of stopwatch
    listcontrol->SetItem(i, j++, wxString((use_stopwatch.value)));
    
    //set stopwatch reading
    if((use_stopwatch.value) == 'y'){
        listcontrol->SetItem(i, j++, wxString((stopwatch).to_string(display_precision)));
    }else{
        listcontrol->SetItem(i, j++, wxString(""));
    }
    
    //set TAI-UTC
    listcontrol->SetItem(i, j++, wxString((TAI_minus_UTC).to_string(display_precision)));
    
    //set label
    listcontrol->SetItem(i, j++, wxString((label).value));
    
    if((related_route.value) == -1){
        //if the sight is not connected to a route, I leave the column field empty
        
        listcontrol->SetItem(i, j++, wxString(""));
        
    }else{
        //if the sight is connected to a route, I write the # of the related route in the column field
        
        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_route.value)+1));
        
    }
    
    
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
        time+=stopwatch;
        
    }
    
    TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file, false, new_prefix);
    time+=TAI_minus_UTC;
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


// this function plots the Routes of type String("c") in route_list in kml format
void Plot::print_to_kml(String prefix){
    
    stringstream line_ins, /*plot_title contains the  title of the Route to be plotted*/ plot_title;
    string line;
    unsigned int i, j;
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
        
        //read dummy text line '    Sights in the plot:"
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
                    ((route_list[route_list.size()-1].related_sight).value) = sight_list.size()-1;
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
            
            //I commented this out because the way to enteer the sight has changed with the GUI
            //            add_sight_and_reduce(new_prefix);
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
                
                //this  needs one additional argument: I commented it out
                //                remove_sight(i, new_prefix);
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
            
            //            I commented this out because now add_route takes an additional argument
            //            add_route(new_prefix);
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
                
                //I commented this out because now remove_route has an additional argument
                //                remove_route(i, new_prefix);
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
                    //I commented this out because now remove_route takes one additional argument
                    //                    remove_route(i, new_prefix);
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
                ((route.related_sight).value) = -1;
                
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
    
    String new_prefix;
    
    new_prefix = prefix.append(String("\t"));
    
    
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
    
    
    //read paramters from init file
    
    file_init.open(String("in"), prefix);
    
    //read number of intervals for ticks from file_init
    cout << prefix.value << YELLOW << "Reading number of intervals for ticks from file " << file_init.name.value << " ...\n" << RESET;
    n_intervals_ticks_preferred.read_from_file(String("preferred number of intervals for ticks"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    //read number of points for routes from file_init
    cout << prefix.value << YELLOW << "Reading number of points for routes from file " << file_init.name.value << " ...\n" << RESET;
    n_points_routes.read_from_file(String("number of points for routes"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    
    //read n_points_plot_coastline from file_init
    cout << prefix.value << YELLOW << "Reading number of points coastline from file " << file_init.name.value << " ...\n" << RESET;
    n_points_plot_coastline.read_from_file(String("number of points coastline"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    
    //read lambda_min, ...., phi_max from file_init
    cout << prefix.value << YELLOW << "Reading minimal and maximal latitude and longitude from file " << file_init.name.value << " ...\n" << RESET;
    lambda_min.read_from_file(String("minimal longitude"), file_init, true, new_prefix);
    lambda_max.read_from_file(String("maximal longitude"), file_init, true, new_prefix);
    phi_min.read_from_file(String("minimal latitude"), file_init, true, new_prefix);
    phi_max.read_from_file(String("maximal latitude"), file_init, true, new_prefix);
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    file_init.close(prefix);
    
    
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
    ((error_circle.related_sight).value) = -1;
    
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
        if(((((route_list[i]).related_sight).value) == -1) || print_all_routes){
            
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



bool Plot::add_sight_and_reduce(Sight* sight_in, String prefix){
    
    
    bool check = true;
    
    //create a new route in the respective list
    route_list.resize(route_list.size()+1);
    
    //push back sight_in into sight_list
    sight_list.push_back(*sight_in);
    
    
    
    //I commented this out because now the sight is enetered through the GUI
    //    (sight_list[sight_list.size()-1]).enter((*catalog), String("new sight"), prefix);
    check &= ((sight_list[sight_list.size()-1]).reduce(&(route_list[route_list.size()-1]), prefix));
    
    //I link the sight to the route, and the route to the sight
    ((sight_list[sight_list.size()-1]).related_route.value) = route_list.size()-1;
    (((route_list[route_list.size()-1]).related_sight).value) = sight_list.size()-1;
    
    
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

//adds to Plot-> this the Route written in *route_in
void Plot::add_route(Route* route_in, String prefix){
    
    
    //    route.enter(String("new route"), prefix);
    
    route_list.push_back(*route_in);
    cout << prefix.value << "Route added as position #" << route_list.size() << ".\n";
    
    
}


//removes sight #i from sight_list by updating all the connections to between sights and routes. If remove_related_route = 'y', it removes also the route related to sight i
void Plot::remove_sight(unsigned int i, Answer remove_related_route, String prefix){
    
    stringstream name;
    unsigned int j;
    Int i_related_route;
    
    i_related_route = ((sight_list[i]).related_route);
    
    name.str("");
    name << "Sight to be removed: Sight #" << i+1;
    
    (sight_list[i]).print(String(name.str().c_str()), prefix, cout);
    
    sight_list.erase(sight_list.begin()+i);
    
    //update the linking indexed of routes in accordance with the deletion of the sight
    for(j=0; j<route_list.size(); j++){
        
        if(((((route_list[j]).related_sight).value) != -1) && ((((route_list[j]).related_sight).value) >= ((int)i))){
            
            if((((route_list[j]).related_sight).value) == ((int)i)){
                (((route_list[j]).related_sight).value) = -1;
            }else{
                (((route_list[j]).related_sight).value)--;
            }
            
        }
        
    }
    
    cout << prefix.value << "Sight removed.\n";
    
    
    if(((i_related_route.value) != -1) && (remove_related_route == Answer('y', prefix))){
        
        remove_route((i_related_route.value), Answer('n', prefix), prefix);
        
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

//removes route #i from route_list by updating all the connections to between sights and routes. If remove_related_sight = 'y', it removes also the sight related to route i
void Plot::remove_route(unsigned int i, Answer remove_related_sight, String prefix){
    
    unsigned int j;
    Int i_related_sight;
    stringstream name;
    
    (i_related_sight.value) = (((route_list[i]).related_sight).value);
    
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
    
    
    if(((i_related_sight.value) != -1) && (remove_related_sight == Answer('y', prefix))){
        
        remove_sight(i_related_sight.value, Answer('n', prefix), prefix);
        
        
        
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
    //    unsigned int i;
    //t_p(m) are the larger (smaller) value of t where the circle of equal altitude crosses the meridian lambda = pi.
    Position p_min, p_max;
    //    int status, iter = 0;
    //x_hi(lo)_p(m) are the higher and lower bound of the interval where I will look for t_p(m)
    //    double x, x_lo_p, x_lo_m, x_hi_p, x_hi_m, x_lo_s, x_hi_s;
    //    gsl_function F;
    //    const gsl_root_fsolver_type *T;
    //    gsl_root_fsolver *s;
    
    //append \t to prefix
    
    
    
    //    T = gsl_root_fsolver_brent;
    //    s = gsl_root_fsolver_alloc(T);
    
    
    
    
    //    //replace line with window size in plot_dummy.plt
    //    cout << prefix.value << YELLOW << "Reading width and height of plot window from file " << file_init.name.value << " ...\n" << RESET;
    //    width_plot_window.read_from_file(String("width of plot window"), file_init, true, new_prefix);
    //    height_plot_window.read_from_file(String("height of plot window"), file_init, true, new_prefix);
    //    command.str("");
    //    command << "LANG=C sed 's/#window size/set terminal qt size " << width_plot_window.value << "," << height_plot_window.value << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    //    system(command.str().c_str());
    //    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    
    
    
    
    //
    //
    //    //in either case of the if above, I write the boundary values which I have read in plot_temp.plt
    //    command.str("");
    //    command << "LANG=C sed 's/#min_longitude/lambda_min = " << (K*lambda_min.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    //    command << "LANG=C sed 's/#max_longitude/lambda_max = " << (K*lambda_max.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    //    command << "LANG=C sed 's/#min_latitude/phi_min = " << (K*phi_min.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    //    command << "LANG=C sed 's/#max_latitude/phi_max = " << (K*phi_max.value) << ";/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    //
    //    system(command.str().c_str());
    //
    
    
    //
    //
    //    //plot routes
    //
    //    plot_command.str("");
    //    command.str("");
    //    //set the key position on the screen
    //    plot_command << "set key top right\\\n";
    //    for(i=0; i<(route_list.size()); i++){
    //
    //        //I start a brand new multiline plot command only if I am looking at the first route plot
    //        if(i==0){
    //            plot_command << "plot ";
    //        }
    //
    //        //set a  plot style for "error on astronomical position curve" and another one for all other curves
    //        plot_style.str("");
    //        if(((route_list[i]).label) == String("error on astronomical position")){
    //            plot_style << "w l dashtype " << i+1 << " linecolor rgb \"purple\"";
    //        }else{
    //            plot_style << "w l dashtype " << i+1 << " lt " << i+1;
    //        }
    //
    //        switch(((route_list[i]).type.value)[0]){
    //
    //            case 'l':
    //                //plot a loxodrome
    //            {
    //
    //                //I assume that  the loxodrome is not cut through the meridian lambda = M_PI, and I make a single plot
    //                plot_command << "[0.:" << (route_list[i]).l.value << "] \"+\" u (xe(K*lambda_lox(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) : (ye(K*phi_lox(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) " << plot_style.str()  << " ti \"type = " << (route_list[i]).type.value << ", start = " << (route_list[i]).start.to_string(display_precision) << ", heading = " << (route_list[i]).alpha.to_string(display_precision) << "\"";
    //
    //                break;
    //            }
    //
    //            case 'o':
    //                //plot an orthodrome
    //            {
    //
    //                //I assume that the orthordrome is not cut through the meridian lambda = M_PI, and I make a single plot
    //                plot_command << "[0.:" << (route_list[i]).l.value << "] \"+\" u (xe(K*lambda_ort(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) : (ye(K*phi_ort(t, " << (route_list[i]).start.phi.value << ", " << (route_list[i]).start.lambda.value << ", " << (route_list[i]).alpha.value << ", " << Re << "))) " << plot_style.str()  << " ti \"type = " << (route_list[i]).type.value << ", start = " << (route_list[i]).start.to_string(display_precision) << ", heading = " << (route_list[i]).alpha.to_string(display_precision) << "\"";
    //
    //                break;
    //            }
    //
    //
    //            case 'c':
    //                //plot a circle of equal altitude
    //            {
    //
    //                //this is the title for all curves which will be plotted with a non-empty title in their legent. Curves plotted with an empty title in their legend will be plotted with 'noti' in the plot command.
    //                plot_title.str("");
    //                if(!((route_list[i]).label == String("error on astronomical position"))){
    //                    plot_title << " ti \"" << (route_list[i]).label.value << "\"";
    //                }else{
    //                    plot_title << " noti";
    //                }
    //
    //                //if abs(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))) < 1.0, then there exists a value of t = t_{max} (t_{min}) such that (route_list[i]).GP.lambda vs. t has a maximum (minimum). In this case, I proceed and compute this maximum and minimum, and see whether the interval [(route_list[i]).GP.lambda_{t = t_{min}} and (route_list[i]).GP.lambda_{t = t_{max}}] embraces lambda = \pi. If it does, I modify the gnuplot command so as to avoid the horizontal line in the graph output.
    //                if(abs(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))) < 1.0){
    //
    //                    //compute the values of the parametric Angle t, t_min and t_max, which yield the position with the largest and smallest longitude (p_max and p_min) on the circle of equal altitude
    //                    t_max.set(String(""), acos(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))), new_prefix);
    //                    t_min.set(String(""), 2.0*M_PI - acos(-tan((route_list[i]).GP.phi.value)*tan(((route_list[i]).omega.value))), new_prefix);
    //
    //                    //p_max =  circle of equal altitude computed at t_max
    //                    ((route_list[i]).l.value) = Re * sin(((route_list[i]).omega.value)) * (t_max.value);
    //                    (route_list[i]).compute_end(new_prefix);
    //                    p_max = ((route_list[i]).end);
    //
    //                    ((route_list[i]).l.value) = Re * sin(((route_list[i]).omega.value)) * (t_min.value);
    //                    (route_list[i]).compute_end(new_prefix);
    //                    p_min = ((route_list[i]).end);
    //                    //p_min =  circle of equal altitude computed at t_min
    //
    //                    /* p_max.print(String("p_max"), new_prefix, cout); */
    //                    /* p_min.print(String("p_min"), new_prefix, cout); */
    //
    //                    if((p_max.lambda.value < M_PI) && (p_min.lambda.value > M_PI)){
    //                        cout << prefix.value << YELLOW << "Circle of equal altitude is cut!\n" << RESET;
    //                        //in this case, the circle of equal altitude is cut through the meridian lambda = M_PI
    //
    //                        if((route_list[i]).GP.lambda.value > M_PI){
    //                            //in this case, the two values of t, t_p and t_m, at which the circle of equal altitude intersects the meridian lambda = M_PI, lie in the interval [0,M_PI]
    //
    //                            cout << prefix.value << "Case I:\n";
    //
    //                            // interval where I know that there will be t_p
    //                            x_lo_p = (t_max.value);
    //                            x_hi_p = M_PI;
    //
    //                            //interval where I know that there will be t_m
    //                            x_lo_m = 0.0;
    //                            x_hi_m = (t_max.value);
    //
    //                        }else{
    //                            //in this case, the two values of t, t_p and t_m, at which the circle of equal altitude intersects the meridian lambda = M_PI, lie in the interval [M_PI,2*M_PI]
    //                            //here I select an interval where I know that there will be t_m
    //
    //                            cout << prefix.value << "Case II:\n";
    //
    //                            // interval where I know that there will be t_p
    //                            x_lo_p = (t_min.value);
    //                            x_hi_p = 2.0*M_PI;
    //
    //                            //interval where I know that there will be t_m
    //                            x_lo_m = M_PI;
    //                            x_hi_m = (t_min.value);
    //
    //                        }
    //
    //                        (route_list[i]).temp_prefix = prefix;
    //                        F.params = &(route_list[i]);
    //                        F.function = &((route_list[i]).lambda_minus_pi);
    //
    //
    //
    //                        //solve for t_p
    //
    //                        gsl_root_fsolver_set(s, &F, x_lo_p, x_hi_p);
    //
    //                        cout << prefix.value << "Extreme values = " << GSL_FN_EVAL(&F,x_lo_p) << " " << GSL_FN_EVAL(&F,x_hi_p) << "\n";
    //
    //                        cout << prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
    //                        cout << new_prefix.value << "iter" <<  " [lower" <<  ", upper] " <<  "root " << "err(est)\n";
    //
    //                        iter = 0;
    //                        do{
    //
    //                            iter++;
    //                            status = gsl_root_fsolver_iterate(s);
    //
    //                            x = gsl_root_fsolver_root(s);
    //                            x_lo_p = gsl_root_fsolver_x_lower(s);
    //                            x_hi_p = gsl_root_fsolver_x_upper(s);
    //                            status = gsl_root_test_interval(x_lo_p, x_hi_p, 0.0, epsrel);
    //                            if(status == GSL_SUCCESS){
    //                                cout << new_prefix.value << "Converged:\n";
    //                            }
    //                            cout << new_prefix.value << iter << " [" << x_lo_p << ", " << x_hi_p << "] " << x << " " << x_hi_p-x_lo_p << "\n";
    //                        }
    //                        while((status == GSL_CONTINUE) && (iter < max_iter));
    //
    //                        t_p.value = (x_lo_p+x_hi_p)/2.0;
    //                        t_p.print(String("t_+"), new_prefix, cout);
    //
    //
    //
    //
    //
    //                        //solve for t_m
    //
    //                        gsl_root_fsolver_set(s, &F, x_lo_m, x_hi_m);
    //
    //                        cout << prefix.value << "Extreme values = " << GSL_FN_EVAL(&F,x_lo_m) << " " << GSL_FN_EVAL(&F,x_hi_m) << "\n";
    //
    //                        cout << prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
    //                        cout << new_prefix.value << "iter" <<  " [lower" <<  ", upper] " <<  "root " << "err(est)\n";
    //
    //                        iter = 0;
    //                        do{
    //
    //                            iter++;
    //                            status = gsl_root_fsolver_iterate(s);
    //
    //                            x = gsl_root_fsolver_root(s);
    //                            x_lo_m = gsl_root_fsolver_x_lower(s);
    //                            x_hi_m = gsl_root_fsolver_x_upper(s);
    //                            status = gsl_root_test_interval(x_lo_m, x_hi_m, 0.0, epsrel);
    //                            if(status == GSL_SUCCESS){
    //                                cout << new_prefix.value << "Converged:\n";
    //                            }
    //                            cout << new_prefix.value << iter << " [" << x_lo_m << ", " << x_hi_m << "] " << x << " " << x_hi_m-x_lo_m << "\n";
    //                        }
    //                        while((status == GSL_CONTINUE) && (iter < max_iter));
    //
    //                        t_m.value = (x_lo_m+x_hi_m)/2.0;
    //                        t_m.print(String("t_-"), new_prefix, cout);
    //
    //                        //the  - epsilon is added because in plot_dummy.plt lambda_min = 180.0 - epsilon. If one does not include this - epsilon, then the last part of the curve goest to the other edge of the plot and a horizontal line appears. Similarly for the - and + epsilon below
    //
    //                        plot_command << "[0.:" << t_m.value << " - epsilon] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << plot_title.str() << ",\\\\\\\n";
    //                        //maybe wrong
    //                        plot_command << "[" << t_m.value << " + epsilon:" << t_p.value << " - epsilon] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << " noti,\\\\\\\n";
    //                        //maybe wrong
    //                        plot_command << "[" << t_p.value << " + epsilon:2.*pi] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << " noti";
    //
    //                    }else{
    //                        //in this case, the circle of equal altitude is not cut through the meridian lambda = M_PI, and I make a single plot
    //
    //                        plot_command << "[0.:2.*pi] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << plot_title.str();
    //
    //                    }
    //
    //                }else{
    //                    //in this case (route_list[i]).GP.lambda.value is a monotonically increasing function of t: I find the value of t = t_s such that (route_list[i]).GP.lambda.value = M_PI and split the gnuplot plot  in two plots so as to avoid the horizontal line
    //
    //                    // interval where I know that there will be t_s
    //                    if((-sin(((route_list[i]).omega.value))/cos(((route_list[i]).GP.phi.value) - (((route_list[i]).omega.value)))) > 0.0){
    //                        //in this case lambda'(t = 0) > 0.0 -> lambda'(t) > 0.0  for all t
    //                        if((route_list[i]).GP.lambda.value < M_PI){
    //                            //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to 0.0 <= t< M_PI
    //                            x_lo_s = 0.0;
    //                            x_hi_s = M_PI;
    //                        }else{
    //                            //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to M_PI <= t< 2*M_PI
    //                            x_lo_s = M_PI;
    //                            x_hi_s = 2.0*M_PI;
    //                        }
    //                    }else{
    //                        //in this case lambda'(t = 0) < 0.0 -> lambda'(t) < 0.0  for all t
    //                        if((route_list[i]).GP.lambda.value < M_PI){
    //                            //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to M_PI <= t< 2*M_PI
    //                            x_lo_s = M_PI;
    //                            x_hi_s = 2.0*M_PI;
    //                        }else{
    //                            //in this case, it is easy to show that the interval of t which embraces t_s such that lambda(t_s) = M_PI is equal to 0.0 <= t< M_PI
    //                            x_lo_s = 0.0;
    //                            x_hi_s = M_PI;
    //                        }
    //
    //                    }
    //
    //                    (route_list[i]).temp_prefix = prefix;
    //                    F.params = &(route_list[i]);
    //                    F.function = &((route_list[i]).lambda_minus_pi);
    //
    //                    //solve for t_s
    //
    //                    gsl_root_fsolver_set(s, &F, x_lo_s, x_hi_s);
    //
    //                    cout << prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";
    //                    cout << new_prefix.value << "iter" <<  " [lower" <<  ", upper] " <<  "root " << "err(est)\n";
    //
    //                    iter = 0;
    //                    do{
    //
    //                        iter++;
    //                        status = gsl_root_fsolver_iterate(s);
    //
    //                        x = gsl_root_fsolver_root(s);
    //                        x_lo_s = gsl_root_fsolver_x_lower(s);
    //                        x_hi_s = gsl_root_fsolver_x_upper(s);
    //                        status = gsl_root_test_interval(x_lo_s, x_hi_s, 0.0, epsrel);
    //                        if(status == GSL_SUCCESS){
    //                            cout << new_prefix.value << "Converged:\n";
    //                        }
    //                        cout << new_prefix.value << iter << " [" << x_lo_s << ", " << x_hi_s << "] " << x << " " << x_hi_s-x_lo_s << "\n";
    //                    }
    //                    while((status == GSL_CONTINUE) && (iter < max_iter));
    //
    //                    t_s.value = (x_lo_s+x_hi_s)/2.0;
    //                    t_s.print(String("t_*"), new_prefix, cout);
    //
    //
    //                    //the  - epsilon is added because in plot_dummy.plt lambda_min = 180.0 - epsilon. If one does not include this - epsilon, then the last part of the curve goest to the other edge of the plot and a horizontal line appears. Similarly for the - and + epsilon below
    //
    //                    plot_command << "[0.:" << t_s.value << " - epsilon] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << plot_title.str() << " ,\\\\\\\n";
    //
    //                    plot_command << "[" << t_s.value << " + epsilon:2.*pi] \"+\" u (xe(K*lambda_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) : (ye(K*phi_cea(t, " << (route_list[i]).GP.phi.value << ", " << (route_list[i]).GP.lambda.value << ", " << ((route_list[i]).omega.value) << "))) " << plot_style.str()  << " noti";
    //
    //                }
    //
    //            }
    //
    //        }
    //
    //        //if there is a route or a position plot after the one that I just added, then I add a ,\ for it
    //        if((i+1<route_list.size()) || ((i+1 == route_list.size()) && (position_list.size() > 0))){
    //
    //            plot_command << ",\\\\";
    //
    //        }
    //
    //        //if I did not reach the last route plot, I add a newline. If I reached the last plot, no need to add a newline because the sed replacement commands will do it
    //        if(i+1<route_list.size()){
    //
    //            plot_command << "\\\n";
    //
    //        }
    //
    //    }
    //    //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
    //    command << "LANG=C sed 's/#route_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> plot_temp_2.plt \n" << "mv plot_temp_2.plt plot_temp.plt \n";
    //
    //
    //
    //    //plot positions
    //
    //    plot_command.str("");
    //    for(i=0; i<position_list.size(); i++){
    //
    //        //I start a brand new multiling plotting command only if I am looking at the first position plot and if there were no previous route plots
    //        if((i==0) && (route_list.size() == 0)){
    //            plot_command << "plot ";
    //        }
    //
    //        //set a  plot style for "astronomical position" Position and another one for all other Positions
    //        plot_style.str("");
    //        if(!(((position_list[i]).label) == String("crossing"))){
    //
    //            if(!(((position_list[i]).label) == String("astronomical position"))){
    //
    //                plot_style << "lt " << i+1 << " ti \"" << (position_list[i]).label.value << "\"";
    //
    //            }else{
    //
    //                plot_style << "linecolor rgb \"purple\" " << " ti \"" << (position_list[i]).label.value << "\"";
    //
    //            }
    //
    //
    //        }else{
    //
    //            plot_style << "lt " << 1 << "linecolor rgb \"gray\" noti";
    //
    //        }
    //
    //
    //        plot_command << "\"+\" u (xe(K*(" << (position_list[i]).lambda.value << "))):(ye(K*(" << (position_list[i]).phi.value << "))) w p lw 2 " << plot_style.str();
    //
    //
    //
    //
    //
    //
    //        if(i+1<position_list.size()){
    //
    //            plot_command << ",\\\\";
    //
    //        }
    //
    //        plot_command << "\\\n";
    //
    //    }
    //
    //    //add the line to plot.plt which contains the parametric plot of the circle of equal altitude
    //    command << "LANG=C sed 's/#position_plots/" << plot_command.str().c_str() << "/g' plot_temp.plt >> " << ((file_gnuplot.name).value) << "\n";
    //
    //
    //    //add the overall plotting command to command string
    //    //here -0+0 is to open the xterm window on the top right edge of the screen
    //    command << "gnuplot '" << ((file_gnuplot.name).value) << "' & \n echo $! >> " << ((file_id.name).value) << "\n";
    //    command << "rm -rf plot_temp.plt";
    //
    //
    //    //execute the command string
    //    system(command.str().c_str());
    //
    //    //read the job id from file_id
    //    if(file_id.open(String("in"), String("\t"))){
    //        getline(file_id.value, line);
    //        line_ins << line;
    //        line_ins >> job_id;
    //    }
    
    //    file_id.close(prefix);
    //    file_id.remove(prefix);
    //    gsl_root_fsolver_free(s);
    
    //    cout << prefix.value << "Job id = "<< job_id << "\n";
    
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
            time+=stopwatch;
            
        }
        
        //read TAI_minus_UTC from /Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/data/index.txt
        cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
        TAI_minus_UTC.read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        time+=TAI_minus_UTC;
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

//reports the angle in the interval [-pi, pi)
void Angle::normalize_pm_pi(void){
    
    normalize();
    if(value > M_PI){value-=2.0*M_PI;}
    
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


string Angle::to_string(String mode, unsigned int precision){
    
    stringstream output;
    //a temporary variable where to store this->value and modifyi it without altering this->value
    double value_temp;
    
    output.precision(precision);
    
    normalize();
    value_temp = value;
    
    if(mode == String("")){
        //in this case, I print out the angle in the format >=0° and <360°
        output << floor(K*value_temp) << "° " << (K*value_temp - floor(K*value_temp))*60.0 << "'";
        
    }else{
        //in this case, I print out the angle in the format >=-180° and <180°
        
        if(value_temp>M_PI){value_temp-=2.0*M_PI;}
        output << floor(fabs(K*value_temp)) << "° " <<  (fabs(K*value_temp) - floor(fabs(K*value_temp)))*60.0<< "'";
        
        if(mode == String("NS")){
            //in this case, I output the sign of the angle in the North/South format (North = +, South = -)
            
            if(value_temp>0.0){output << " N";}
            else{output << " S";}
        }
        if(mode == String("EW")){
            //in this case, I output the sign of the angle in the East/West format (West = +, East = -)
            
            if(value_temp>0.0){output << " W";}
            else{output << " E";}
        }
        
    }
    
    return (output.str().c_str());
    
}


//this function prints out only the integer degree closest to this
string Angle::deg_to_string(String mode, unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    normalize();
    
    if(mode == String("")){
        //in this case, I print out the angle in the format >=0° and <360°
        output << round(K*value) << "°";
        
    }else{
        //in this case, I print out the angle in the format >=-180° and <180°
        
        if(value>M_PI){value-=2.0*M_PI;}
        output << round(fabs(K*value)) << "°";
        
        //I append NS or EW only if the angle is != 0, otherwise it is pointless to add these labels
        if(value != 0.0){
            
            if(mode == String("NS")){
                //in this case, I output the sign of the angle in the North/South format (North = +, South = -)
                
                if(value>0.0){output << " N";}
                else{output << " S";}
            }
            if(mode == String("EW")){
                //in this case, I output the sign of the angle in the East/West format (West = +, East = -)
                
                if(value>0.0){output << " W";}
                else{output << " E";}
            }
            
        }
        
    }
    
    return (output.str().c_str());
    
}

//this function prints out only the arcminute part of this
string Angle::min_to_string(String mode, unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    normalize();
    
    if(mode != String("")){
        //in this case, I print out the angle in the format >=-180° and <180°
        if(value>M_PI){value-=2.0*M_PI;}
    }
    
    output << (fabs(K*value) - floor(fabs(K*value)))*60.0 << "'";
    
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



//this function adjusts the width of a wxComboBox according to its largest entry
void AdjustWidth(wxComboBox *control){
    
    unsigned int i;
    int max_width, width, additional;
    
    //this is the additional width occupied by the sqare with the arrow
    control->GetTextExtent(wxString("-----"), &additional, NULL);
    
    for(max_width=0, i=0; i<(control->GetCount()); i++){
        
        control->GetTextExtent(control->GetString(i), &width, NULL);
        
        if(width > max_width){
            max_width = width;
        }
        
    }
    
    control->SetMinSize(wxSize(max_width + additional, -1));
    
}

ChartPanel::ChartPanel(ChartFrame* parent_in, const wxPoint& position, const wxSize& size) : wxPanel(parent_in, wxID_ANY, position, size, wxTAB_TRAVERSAL, wxT("")){
    
    parent = parent_in;
    
}


//this function efficiently reads coastline data stored in path_file_coastline_data_blocked from latitudes p to P and longitudes l to L, and writes this data into path_file_selected_coastline_data, writing n_points points at the most
void ChartFrame::GetCoastLineData(void){
    
    
    //    File file_n_line, file_coastline_data_blocked/*, outfile_selected_coastline_data*/;
    //    string data, line;
    //    stringstream ins;
    int i, j, i_min = 0, i_max = 0, j_min = 0, j_max = 0, lambda_min_int, lambda_max_int, phi_min_int, phi_max_int;
    unsigned int l, n = 0, every = 0, n_points_grid = 0;
    double x_temp, y_temp;
    
    //set x_min, ..., y_max for the following
    draw_panel->Update_x_y_min_max();
    
    
    //transform the values phi_min_int, phi_max_int in a format appropriate for GetCoastLineData: normalize the minimal and maximal latitudes in such a way that they lie in the interval [-pi, pi], because this is the format which is taken by GetCoastLineData
    ((parent->plot)->phi_min).normalize_pm_pi();
    ((parent->plot)->phi_max).normalize_pm_pi();
    
    lambda_min_int = ceil(K*(((parent->plot)->lambda_min).value));
    lambda_max_int = floor(K*(((parent->plot)->lambda_max).value));
    phi_min_int = floor(K*(((parent->plot)->phi_min).value));
    phi_max_int = ceil(K*(((parent->plot)->phi_max).value));
    
    //transform the values lambda_min_int, lambda_max_int in a format appropriate for data_x and data_y
    if((lambda_min_int < 180) && (lambda_max_int >= 180)){
        j_min = lambda_max_int;
        j_max = 360 + lambda_min_int;
    }else{
        j_min = lambda_max_int;
        j_max = lambda_min_int;
    }
    
    i_min = phi_min_int;
    i_max = phi_max_int;
    
    //    cout << "\n\n\n\n\nCoordinates: " << phi_min_int << " " << phi_max_int << " " << lambda_min_int << " " << lambda_max_int << "\n";
    
    
    n_points_grid = (i_max - i_min + 1 ) * (j_max - j_min + 1);
    
    x.clear();
    y.clear();
    for(i=i_min; i<i_max; i++){
        
        //        cout << "\n i = " << i;
        
        for(j=j_min; j<j_max; j++){
            
            
            
            //            cout << "\nCalled data_x[" << i - floor_min_lat << "][" << j % 360;
            //            flush(cout);
            
            //count how many datapoints are in data_x[i] and in data_y[i]
            n = (data_x[i - floor_min_lat][j % 360]).size();
            
            every = (unsigned int)(((double)n)/((double)(((parent->plot)->n_points_plot_coastline).value))*((double)n_points_grid));
            if(every == 0){every = 1;}
            
            for(l=0; l<(data_x[i - floor_min_lat][j % 360]).size(); l++){
                
                //                cout << "\n l = " << l;
                
                
                x_temp = data_x[i - floor_min_lat][j % 360][l];
                y_temp = data_y[i - floor_min_lat][j % 360][l];
                
                //I write points in data to outfile_selected_coastline_data in such a way to write (((parent->plot)->n_points_coastline).value) points to the most
                if((l % every) == 0){
                    
                    if(/*take account of the periodicity around longitudes here, otherwise this condition will never be satisfied*/((draw_panel->x_min) <= x_temp) && (x_temp <= (draw_panel->x_max)) && ((draw_panel->y_min) <= y_temp) && (y_temp <= (draw_panel->y_max))){
                        
                        x.push_back(x_temp);
                        y.push_back(y_temp);
                        
                    }
                    
                }
                
                
            }
            
        }
        
    }
    
}


//this function fetches the data in path_file_coastline_data_blocked and stores them in data_x, data_y so that they can be read fastly
void ChartFrame::GetAllCoastLineData(void){
    
    File file_n_line, file_coastline_data_blocked;
    string data, line;
    stringstream ins;
    int i, j;
    //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
    vector<unsigned int> n_line(360*(floor_max_lat-floor_min_lat+1));
    unsigned int l, n = 0;
    char* buffer = NULL;
    size_t pos_beg, pos_end;
    bool check;
    double lambda_temp, phi_temp;
    
    
    file_n_line.set_name(String(path_file_n_line));
    file_coastline_data_blocked.set_name(String(path_file_coastline_data_blocked));
    
    //read file n_line and store it into vector n_line
    file_n_line.open(String("in"), String(""));
    i=0;
    while(!(file_n_line.value.eof())){
        
        line.clear();
        ins.clear();
        
        getline(file_n_line.value, line);
        ins << line;
        ins >> (n_line[i++]);
        
        //        cout << "\nn_line[" << i-1 << "] = " << n_line[i-1];
        
    }
    file_n_line.close(String(""));
    
    
    //read in map_conv_blocked.csv the points with i_min <= latitude <= i_max, and j_min <= longitude <= j_max
    file_coastline_data_blocked.open(String("in"), String(""));
    
    
    
    check = true;
    i=0;
    while(!(file_coastline_data_blocked.value.eof())){
        
        
        data_x.resize(i+1);
        data_y.resize(i+1);
        
        (data_x[i]).resize(360);
        (data_y[i]).resize(360);
        
        for(j=0; j<360; j++){
            
            // read data as a block:
            file_coastline_data_blocked.value.seekg(n_line[360*i+j], file_coastline_data_blocked.value.beg);
            
            l = n_line[360*i+j + 1] - n_line[360*i+j] - 1;
            if(buffer != NULL){delete [] buffer;}
            buffer = new char [l];
            
            (file_coastline_data_blocked.value).read(buffer, l);
            string data(buffer, l);
            
            if(!(file_coastline_data_blocked.value)){
                check = false;
            }
            
            //count how many datapoints are in data
            n = count(data.begin(), data.end(), ',');
            
            l=0;
            pos_beg = 0;
            pos_end = data.find(" ", pos_beg);
            while(pos_end != (string::npos)){
                
                line.clear();
                line = data.substr(pos_beg, pos_end - pos_beg + 1).c_str();
                
                replace(line.begin(), line.end(), ' ', '\n');
                replace(line.begin(), line.end(), ',', ' ');
                
                ins.clear();
                ins << line;
                ins >> phi_temp >> lambda_temp;
                
                (data_x[i][j]).push_back(x_mercator(lambda_temp));
                (data_y[i][j]).push_back(y_mercator(phi_temp));
                
                pos_beg = pos_end+1;
                pos_end = data.find(" ", pos_beg);
                
                l++;
                
            };
            
            data.clear();
            
        }
        
        i++;
        
    }
    
    cout << "\nxxxxxx    i at the end of loop = " << i;
    
    
    //    for(i=0; i<10; i++){
    //        cout << "\ni = " << i << "\n";
    //        for(j=0; j<data_x[i].size(); j++){
    //            cout << "\t"<< data_x[i][j];
    //        }
    //
    //    }
    
    //    for(i=data_x.size()-10; i<data_x.size(); i++){
    //        cout << "\ni = " << i << "\n";
    //        for(j=0; j<data_x[i].size(); j++){
    //            cout << "\t"<< data_x[i][j];
    //        }
    //
    //    }
    
    
    
    if(check){
        
        cout << "All characters read successfully\n";
        
    }else{
        
        cout << RED << "Error: not all characters could be read\n" << RESET;
        
    }
    
    file_coastline_data_blocked.close(String(""));
    n_line.clear();
    
    
}

void ChartFrame::SetIdling(bool b){
    
    idling = b;
    
}

DrawPanel::DrawPanel(ChartPanel* parent_in) : wxPanel(parent_in){
    
    unsigned int i;
    
    //when the DrawPan is created there is no open selection rectangle and the mouse is not being dragged.
    selection_rectangle = false;
    mouse_dragging = false;
    
    parent = (parent_in->parent);
    plot = ((parent->parent)->plot);
    
    SetCursor(*wxCROSS_CURSOR);
    tic_length_over_width_plot_area = 0.01;
    
    
    //allocates points_route_list
    points_route_list.resize((plot->route_list).size());
    for(i=0; i<(plot->route_list).size(); i++){
        (points_route_list[i]).clear();
    }
    
    
    print_error_message = new PrintErrorMessage<DrawPanel>(this);
    
    
    //text for the coordinates of the mouse cursor relative to the corners of the selection rectangle
    text_position_start = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    text_position_end = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //    sizer_h->Add(text_phi);
    //    sizer_h->Add(text_lambda);
    //
    //    SetSizer(sizer_h);
    
    
    
}


BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
// some useful events
/*
 EVT_MOTION(DrawPanel::mouseMoved)
 EVT_LEFT_DOWN(DrawPanel::mouseDown)
 EVT_LEFT_UP(DrawPanel::mouseReleased)
 EVT_RIGHT_DOWN(DrawPanel::rightClick)
 EVT_LEAVE_WINDOW(DrawPanel::mouseLeftWindow)
 EVT_KEY_DOWN(DrawPanel::keyPressed)
 EVT_KEY_UP(DrawPanel::keyReleased)
 EVT_MOUSEWHEEL(DrawPanel::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(DrawPanel::PaintEvent)

END_EVENT_TABLE()


void DrawPanel::PaintEvent(wxPaintEvent & evt){
    
    wxPaintDC dc(this);
    Render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 *
 * In most cases, this will not be needed at all; simply handling
 * paint events and calling Refresh() when a refresh is needed
 * will do the job.
 */
void DrawPanel::PaintNow(){
    
    wxClientDC dc(this);
    Render(dc);
    
    //sets the size of the DrawPanel and of the ChartFrame which is its parent and fit the size of ChartFrame parent in such a way that it just fits its content
    this->SetMinSize(wxSize(c->getWidth(), c->getHeight()));
    //        (parent->panel)->SetMinSize(wxSize(
    //                                 (c->getWidth()) + (((parent->slider)->GetSize()).GetWidth()),
    //                                 (c->getHeight()) + (((parent->text_position_now)->GetSize()).GetHeight())
    //                                 ));
    parent->SetMinSize(wxSize(
                              (c->getWidth()) + ((parent->slider)->GetSize().GetWidth()),
                              (c->getHeight()) + (((parent->text_position_now)->GetSize()).GetHeight())
                              ));
    parent->SetSizerAndFit(parent->sizer_v);
    
    
}

//remember that any Draw command in this function takes as coordinates the coordinates relative to the position of the DrawPanel object!
void DrawPanel::Render(wxDC&  dc){
    
    Angle lambda, phi;
    wxPoint p;
    double dummy, thickness;
    stringstream s;
    wxString wx_string;
    //this = true if, while drawing the x or y axis labels, the label that I one is about to draw is the first one
    bool first_label;
    unsigned int i, j;
    
    wxBrush brush(wxColour(/*the first three entries are the rgb code for the color*/255, 0, 0, /*the last is the degree of transparency of the color*/25));
    //    brush.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    dc.SetBrush(brush);
    
    
    //draw coastlines
    dc.DrawBitmap(wxBitmap(path_file_chart, wxBITMAP_TYPE_PNG), 0, 0);
    
    
    //draw routes
    for(i=0; i<(plot->route_list).size(); i++){
        
        if(i == ((parent->parent)->highlighted_route)){
            thickness = max((int)(((parent->large_thickness_over_length_screen).value)/2.0 * ((parent->parent)->rectangle_display).GetWidth()), 1);
        }else{
            thickness = max((int)(((parent->standard_thickness_over_length_screen).value)/2.0 * ((parent->parent)->rectangle_display).GetWidth()), 1);
        }
        
        dc.SetPen(wxPen(((parent->parent)->color_list)[i % (((parent->parent)->color_list).size())], thickness) );
        
        //draw the roues as lines
        //       dc.DrawLines((points_route_list[i]).size(), (points_route_list[i]).data() , 0, 0);
        //draw the routes as points
        for(j=0; j<(points_route_list[i]).size(); j++){
            //            dc.DrawPoint(points_route_list[i][j]);
            dc.DrawCircle(points_route_list[i][j], thickness);
            
        }
        
    }
    
    //draw positions
    for(i=0; i<(plot->position_list).size(); i++){
        
        if(i == ((parent->parent)->highlighted_position)){
            thickness = max((int)(((parent->large_thickness_over_length_screen).value)/2.0 * ((parent->parent)->rectangle_display).GetWidth()), 1);
        }else{
            thickness = max((int)(((parent->standard_thickness_over_length_screen).value)/2.0 * ((parent->parent)->rectangle_display).GetWidth()), 1);
        }
        
        dc.SetPen(wxPen(((parent->parent)->color_list)[i % (((parent->parent)->color_list).size())], thickness) );
        
        
        if(GeoToPlot((plot->position_list)[i], &p)){
            //if the point returned from GeoToPlot falls within the plot area, then I plot it
            
            dc.DrawCircle(p, 4.0*thickness);
        }
        
        
    }
    
    //   reset the pen to its default parameters
    dc.SetPen(wxPen(wxColor(255,175,175), 1 ) ); // 1-pixels-thick pink outline
    
    
    if(selection_rectangle){
        dc.DrawRectangle(
                         position_start_selection.x - (position_draw_panel.x),
                         position_start_selection.y - (position_draw_panel.y),
                         (position_screen_now.x)-(position_start_selection.x),
                         (position_screen_now.y)-(position_start_selection.y)
                         );
        
    }
    
    //draw labels on the x axis
    //set the initial value of dummy
    dummy = x_mercator((floor((K*(((plot->lambda_min).value)))/delta_lambda))*delta_lambda);
    
    
    //starts the loop which draws the labels
    for(first_label = true; dummy <= x_max; dummy+=k*delta_lambda){
        
        s.str("");
        lambda.set(String(""), k*lambda_mercator(dummy), String(""));
        
        if(/*If this condition is true, then lambda.value*K is an integer multiple of one degree. I use delta_lambda to check this condition rather tahn lambda itself, because delta_lambda is not subject to rounding errors */delta_lambda == round(delta_lambda)){
            //in this case, lambda = n degrees, with n integer: I write on the axis only the degree part of lambda
            s << lambda.deg_to_string(String("EW"), display_precision);
        }else{
            //in this case, delta_lambda  is not an integer multiple of a degree. However, lambda_mercator(dummy) may still be or not be a multiple integer of a degree
            
            if(fabs(lambda_mercator(dummy) - ((double)round(lambda_mercator(dummy)))) < delta_lambda/2.0){
                //in this case, lamba_mercator(dummy) coincides with an integer mulitple of a degree: I print out its arcdegree part only
                
                s << lambda.deg_to_string(String("EW"), display_precision);
                
            }else{
                //in this case, lamba_mercator(dummy) deos not coincide with an integer mulitple of a degree.
                
                
                if(ceil((K*((plot->lambda_max).value)))  - floor((K*((plot->lambda_min).value))) != 1){
                    //in this case, the lambda interval which is plotted spans more than a degree: there will already be at least one tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I print out its arcminute part only.
                    
                    s << lambda.min_to_string(String("EW"), display_precision);
                }else{
                    //in this case, the lambda interval which is plotted spans les than a degree: there will be no tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I add this tic by printing, at the first tic, both the arcdegrees and arcminutes.
                    
                    if(first_label){
                        s << lambda.to_string(String("EW"), display_precision);
                    }else{
                        s << lambda.min_to_string(String("EW"), display_precision);
                    }
                }
                
            }
        }
        wx_string = wxString(s.str().c_str());
        
        dc.DrawRotatedText(
                           wx_string,
                           (position_plot_area.x) + (dummy-x_min)/(x_max-x_min)*width_plot_area - (GetTextExtent(wx_string).GetWidth())/2,
                           (position_plot_area.y) + height_plot_area /*this is the border, to allow some empty space between the text and the axis*/
                           + ((parent->GetSize()).GetWidth())*length_border_over_length_frame,
                           0);
        
        first_label = false;
        
    }
    
    //draw labels on the y axis
    //set first value of dummy
    //    if(y_min > floor((K*(((plot->phi_min).value)))/delta_phi)*delta_phi){
    dummy = ceil((K*(((plot->phi_min).value)))/delta_phi)*delta_phi;
    //    }else{
    //        dummy = floor((K*(((plot->phi_min).value)))/delta_phi)*delta_phi;
    //    }
    //starts for loop which draws the ylabels
    for(first_label = true; dummy<(K*(((plot->phi_max).value))); dummy+= delta_phi){
        
        s.str("");
        phi.set(String(""), k*dummy, String(""));
        phi.normalize_pm_pi();
        
        if(/*If this condition is true, then phi.value*K is an integer multiple of one degree. I use delta_phi to check this condition rather tahn lambda itself, because delta_phi is not subject to rounding errors */delta_phi== round(delta_phi)){
            //in this case, dummy (or, in other words, the latitude phi) = n degrees, with n integer: I write on the axis the value of phi  in degrees
            s << phi.deg_to_string(String("NS"), display_precision);
        }else{
            
            //in this case, delta_phi  is not an integer multiple of a degree. However, dummy may still be or not be a multiple integer of a degree
            if(fabs(dummy - ((double)round(dummy))) < delta_phi/2.0){
                //in this case, dummy coincides with an integer mulitple of a degree: I print out its arcdegree part only
                
                s << phi.deg_to_string(String("NS"), display_precision);
                
            }else{
                //in this case, dummy deos not coincide with an integer mulitple of a degree: I print out its arcminute part only
                
                //                s << phi.min_to_string(String("NS"), display_precision);
                
                if(ceil((K*((plot->phi_max).value)))  - floor((K*((plot->phi_min).value))) != 1){
                    //in this case, the phi interval which is plotted spans more than a degree: there will already be at least one tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I print out its arcminute part only.
                    
                    s << phi.min_to_string(String("NS"), display_precision);
                }else{
                    //in this case, the phi interval which is plotted spans less than a degree: there will be no tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I add this tic by printing, at the first tic, both the arcdegrees and arcminutes.
                    
                    if(first_label){
                        s << phi.to_string(String("NS"), display_precision);
                    }else{
                        s << phi.min_to_string(String("NS"), display_precision);
                    }
                }
                
                
            }
            
        }
        
        wx_string = wxString(s.str().c_str());
        
        dc.DrawRotatedText(
                           wx_string,
                           (position_plot_area.x) - (GetTextExtent(wx_string).GetWidth()) - /*this is the border, to allow some empty space between the text and the axis*/
                           ((parent->GetSize()).GetWidth())*length_border_over_length_frame,
                           (position_plot_area.y) + height_plot_area - ((y_mercator(dummy)-y_min)/(y_max-y_min)*height_plot_area) - (GetTextExtent(wx_string).GetHeight())/2,
                           0);
        
        first_label = false;
        
    }
    
    /*
     Position geo;
     wxPoint screen;
     cout << "A position_screen_now = " << (position_screen_now.x) << " " << (position_screen_now.y) << "\n";
     GetMouseGeoPosition(&geo);
     GeoToScreen(geo, &screen);
     dc.DrawCircle(screen.x - position_draw_panel.x, screen.y - position_draw_panel.y, 10);
     */
    
}


void DrawPanel::Draw(void){
    
    double lambda, phi, x_dummy, y_dummy, phi_span, lambda_span;
    int i, j;
    unsigned int n_intervals_ticks, n_intervals_ticks_max;
    //the total length of each Route
    Length l_tot;
    Angle dummy;
    //this is a pointer to parent->parent->plot, created only to shorten the code
    wxPoint p;
    
    
    
    
    //fetch the data on the region that I am about to plot from the data files.
    parent->GetCoastLineData();
    
    
    /*I set the aspect ratio between height and width equal to the ration between the y and x range: in this way, the aspect ratio of the plot is equal to 1*/
    if((y_max-y_min) > (x_max-x_min)){
        //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
        parent->SetSize(
                        (((((parent->parent)->rectangle_display)).GetSize()).GetHeight())/((y_max-y_min)/(x_max-x_min)),
                        (((((parent->parent)->rectangle_display)).GetSize()).GetHeight())
                        );
        
        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object
        height_chart = length_chart_over_length_chart_frame * (((((parent->parent)->rectangle_display)).GetSize()).GetHeight());
        width_chart = height_chart/((y_max-y_min)/(x_max-x_min));
    }else{
        //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
        parent->SetSize(
                        (((((parent->parent)->rectangle_display)).GetSize()).GetHeight()),
                        (((((parent->parent)->rectangle_display)).GetSize()).GetHeight()) * ((y_max-y_min)/(x_max-x_min))
                        );
        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object
        width_chart = length_chart_over_length_chart_frame * (((((parent->parent)->rectangle_display)).GetSize()).GetHeight());
        height_chart = width_chart*((y_max-y_min)/(x_max-x_min));
    }
    width_plot_area = width_chart*length_plot_area_over_length_chart;
    height_plot_area = height_chart*length_plot_area_over_length_chart;
    tic_length = tic_length_over_width_plot_area*width_plot_area;
    
    //draw coastlines
    
    
    // Create a XYChart object with the appropriate size
    c = new XYChart(width_chart, height_chart);
    //create the plot area of c with the appropriate size
    c->setPlotArea(width_chart*0.15, height_chart*0.1,
                   width_plot_area,
                   height_plot_area,
                   -1, -1, 0xc0c0c0, 0xc0c0c0, -1);
    
    //stores into position_plot_area the screen position of the top-left edge of the plot area.
    position_plot_area = wxPoint((c->getPlotArea())->getLeftX(), (c->getPlotArea())->getTopY());
    //stores in to size_plot_area the size of the plot area
    size_plot_area = wxSize((c->getPlotArea())->getWidth(), (c->getPlotArea())->getHeight());
    
    
    //set meridians
    lambda_span = K*(x_max-x_min);
    
    //I create an angle which has the largest posible label when printed out in the "EW" format, so as to compute the  value of n_interval_ticks which allows the x-axis labels not to superpose
    dummy.from_sign_deg_min('+', 179, 59);
    
    //the number of ticks is given by the minimum between the preferred value and the value allowed by fitting the (maximum) size of each axis label into the witdh of the axis
    
    n_intervals_ticks_max = ((unsigned int)floor(((double)(size_plot_area.x))/((double)(GetTextExtent(wxString((dummy.to_string(String("EW"), display_precision)))).GetWidth()))));
    n_intervals_ticks = min(
                            (unsigned int)((plot->n_intervals_ticks_preferred).value),
                            n_intervals_ticks_max
                            );
    
    //    if(((plot->n_intervals_ticks_preferred).value) > n_intervals_ticks_max){
    //        cout << "xxxxxxxxx      I have to reduce the number of ticks!\n";
    //    }
    
    //set delta_lambda
    if(lambda_span > 1.0){gamma_lambda = 1.0;}
    else{gamma_lambda = 60.0;}
    
    delta_lambda=1.0/gamma_lambda;
    while(n_intervals_ticks*delta_lambda<lambda_span){
        if(delta_lambda == 1.0/gamma_lambda){delta_lambda = delta_lambda + 4.0/gamma_lambda;}
        else{delta_lambda = delta_lambda + 5.0/gamma_lambda;}
    }
    //    if(delta_lambda > 1.0/gamma_lambda){
    //        if(delta_lambda == 5.0/gamma_lambda){delta_lambda = delta_lambda - 4.0/gamma_lambda;}
    //        else{delta_lambda = delta_lambda - 5.0/gamma_lambda;}
    //    }
    //    cout <<  "... delta_lambda = " << delta_lambda << "\n";
    
    lambda = ((int)((K*(((plot->lambda_min).value)))/delta_lambda))*delta_lambda;
    for(x_dummy = x_mercator(lambda) - k*delta_lambda; x_dummy <= x_max; x_dummy+=k*delta_lambda){
        
        if((x_dummy >= x_min) && (x_dummy <= x_max)){
            
            c->addLine(
                       (position_plot_area.x) + (x_dummy-x_min)/(x_max-x_min)*width_plot_area,
                       (position_plot_area.y),
                       (position_plot_area.x) + (x_dummy-x_min)/(x_max-x_min)*width_plot_area,
                       (position_plot_area.y) + height_plot_area,
                       0x808080, 1);
            
        }
        
        if(gamma_lambda == 60.0){
            
            //plot the xticks from lambda to the next lambda (lambda + dlambda)
            for(i=0; (((double)i)/10.0)/60.0 < delta_lambda; i++){
                
                if((x_dummy + k*(((double)i)/10.0)/60.0 >= x_min) && (x_dummy + k*(((double)i)/10.0)/60.0 <= x_max)){
                    //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
                    
                    c->addLine(
                               (position_plot_area.x) + ((x_dummy + k*(((double)i)/10.0)/60.0)-x_min)/(x_max-x_min)*width_plot_area,
                               (position_plot_area.y) + height_plot_area,
                               (position_plot_area.x) + ((x_dummy + k*(((double)i)/10.0)/60.0)-x_min)/(x_max-x_min)*width_plot_area,
                               (position_plot_area.y) + height_plot_area - height_plot_area*tic_length_over_width_plot_area,
                               0x0000ff, 1);
                    
                }
                
            }
            
        }
        
    }
    
    
    
    
    
    
    
    
    //set parallels
    phi_span = K*(((plot->phi_max).value) - ((plot->phi_min).value));
    
    //I create an angle which has the largest posible label when printed out in the "NS" format, so as to compute the  value of n_interval_ticks which allows the y-axis labels not to superpose
    dummy.from_sign_deg_min('+', 89, 59);
    
    //the number of ticks is given by the minimum between the preferred value and the value allowed by fitting the (maximum) size of each axis label into the witdh of the axis
    
    n_intervals_ticks_max = ((unsigned int)floor(((double)(size_plot_area.y))/((double)(GetTextExtent(wxString((dummy.to_string(String("NS"), display_precision)))).GetHeight()))));
    n_intervals_ticks = min(
                            (unsigned int)((plot->n_intervals_ticks_preferred).value),
                            n_intervals_ticks_max
                            );
    
    
    //gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if(phi_span > 1.0){gamma_phi = 1.0;}
    else{gamma_phi = 60.0;}
    
    delta_phi=1.0/gamma_phi;
    while(((plot->n_intervals_ticks_preferred).value)*delta_phi<phi_span){
        //print delta_phi;
        if(delta_phi == 1.0/gamma_phi){delta_phi = delta_phi + 4.0/gamma_phi;}
        else{delta_phi = delta_phi + 5.0/gamma_phi;}
    }
    //    if(delta_phi > 1.0/gamma_phi){
    //        if(delta_phi == 5.0/gamma_phi){delta_phi = delta_phi - 4.0/gamma_phi;}
    //        else{delta_phi = delta_phi - 5.0/gamma_phi;}
    //    }
    //    cout << "... delta_phi = "  << delta_phi << "\n";
    
    
    for(phi = (((int)((K*(((plot->phi_min).value)))/delta_phi))-1)*delta_phi; phi<(K*(((plot->phi_max).value))); phi+= delta_phi){
        
        y_dummy = y_mercator(phi);
        
        if((y_dummy >= y_min) && (y_dummy <= y_max)){
            
            c->addLine(
                       (position_plot_area.x),
                       (position_plot_area.y) + height_plot_area - ((y_dummy-y_min)/(y_max-y_min)*height_plot_area),
                       (position_plot_area.x) + width_plot_area,
                       (position_plot_area.y) + height_plot_area - ((y_dummy-y_min)/(y_max-y_min)*height_plot_area),
                       0x808080, 1);
            
        }
        
        if(gamma_phi == 60.0){
            
            //plot the yticks from phi to the next phi (phi + dphi)
            for(i=0; (((double)i)/10.0)*1.0/60.0 < delta_phi; i++){
                
                if((phi + (((double)i)/10.0)*1.0/60.0 >= (K*(((plot->phi_min).value)))) && (phi + (((double)i)/10.0)*1.0/60.0 <= (K*(((plot->phi_max).value))))){
                    //set custom-made minor yticks every tenths (i/10.0) of arcminutes (60.0)
                    
                    c->addLine(
                               (position_plot_area.x),
                               (position_plot_area.y) + height_plot_area - (( y_mercator(phi + ((double)i)/10.0/60.0)  -y_min)/(y_max-y_min)*height_plot_area),
                               (position_plot_area.x) + width_plot_area*tic_length_over_width_plot_area,
                               (position_plot_area.y) + height_plot_area - ((y_mercator(phi + ((double)i)/10.0/60.0)-y_min)/(y_max-y_min)*height_plot_area),
                               0x0000ff, 1);
                    
                }
                
            }
            
        }
        
        
        
    }
    
    //
    
    
    // Add a legend box at (50, 30) (top of the chart) with horizontal layout. Use 12pt Times Bold
    // Italic font. Set the background and border color to Transparent.
    //    c->addLegend(50, 30, false, "Times New Roman Bold Italic", 12)->setBackground(Chart::Transparent);
    
    // Add a title to the x axis using 12pt Arial Bold Italic font
    //    (c->xAxis())->setTitle("lambda", "Arial", 12);
    //set the interval of the x axis, and disables the xticks with the last NoValue argument
    (c->xAxis())->setLinearScale(x_min, x_max, 1.7E+308);
    
    (c->yAxis())->setLinearScale(y_min, y_max, 1.7E+308);
    
    // Set the axes line width to 3 pixels
    c->xAxis()->setWidth(2);
    c->yAxis()->setWidth(2);
    
    // Add an orange (0xff9933) scatter chart layer, using 13 pixel diamonds as symbols
    c->addScatterLayer(DoubleArray((parent->x).data(), (parent->x).size()), DoubleArray((parent->y).data(), (parent->y).size()), "", Chart::CircleSymbol, 1, 000000);
    
    c->makeChart(path_file_chart);
    
    
    
    
    //draw routes
    
    
    for(i=0; i<(plot->route_list).size(); i++){
        
        
        if((((plot->route_list)[i]).type) == String("c")){
            //if the Route under consideration is a circle of equal altitde, its total length is the length of the circle itself, which reads:
            
            l_tot.set(String(""), 2.0*M_PI*(Re*sin((((plot->route_list)[i]).omega.value))), String(""));
            
        }else{
            //otherwise, the total length is simply written in the l object
            
            l_tot.set(String(""), (((plot->route_list)[i]).l).value, String(""));
            
        }
        
        for((points_route_list[i]).clear(), j=0; j<(unsigned int)((plot->n_points_routes).value); j++){
            
            
            
            //across the for loop over j, I set the length of the route equal to a temporary value, which spans between 0 and  l_tot
            (((plot->route_list)[i]).l).set(
                                            String(""),
                                            (l_tot.value)*((double)j)/((double)(((plot->n_points_routes).value)-1)),
                                            String(""));
            
            //I compute the coordinate of the endpoint of (plot->route_list)[i] for the length above
            ((plot->route_list)[i]).compute_end(String(""));
            
            if(GeoToPlot(((plot->route_list)[i]).end, &p)){
                (points_route_list[i]).push_back(p);
                
                
            }
            
        }
        
    }
    
    //free up resources
    (parent->x).clear();
    (parent->y).clear();
    
    //center the parent in the middle of the screen because the plot shape has changed and the plot may thus be misplaced on the screen
    //    parent->CenterOnScreen();
    
}


ChartFrame::ChartFrame(ListFrame* parent_input, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    stringstream s;
    String new_prefix;
    //empty wxStaticTexts to fill the empty spaces of the wxGridSizer sizer_buttons
    wxStaticText* empty_text_1, *empty_text_2, *empty_text_3, *empty_text_4, *empty_text_5;
    
    parent = parent_input;
    
    file_init.set_name(String(path_file_init));
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    
    (parent->plot)->show(true, String(""));
    
    GetAllCoastLineData();
    
    
    file_init.open(String("in"), prefix);
    //read value_slider_max from file_init
    cout << prefix.value << YELLOW << "Reading maximal zoom factor from file " << file_init.name.value << " ...\n" << RESET;
    value_slider_max.read_from_file(String("maximal zoom factor"), file_init, true, String(""));
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    //read relative_displacement from file_init
    cout << prefix.value << YELLOW << "Reading relative displacement from file " << file_init.name.value << " ...\n" << RESET;
    relative_displacement.read_from_file(String("relative displacement"), file_init, true, String(""));
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    //read standard_thickness_over_length_screen from file_init
    cout << prefix.value << YELLOW << "Reading standard thickness over length screen from file " << file_init.name.value << " ...\n" << RESET;
    standard_thickness_over_length_screen.read_from_file(String("standard thickness over length screen"), file_init, true, String(""));
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    //read large_thickness_over_length_screen from file_init
    cout << prefix.value << YELLOW << "Reading large thickness over length screen from file " << file_init.name.value << " ...\n" << RESET;
    large_thickness_over_length_screen.read_from_file(String("large thickness over length screen"), file_init, true, String(""));
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    
    file_init.close(prefix);
    
    idling = false;
    print_error_message = new PrintErrorMessage<ChartFrame>(this);
    
    panel = new ChartPanel(this, wxDefaultPosition, wxDefaultSize);
    draw_panel = new DrawPanel(panel);
    
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_slider = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxGridSizer(3, 3, 0, 0);
    
    //text field showing the latitude and longitude of the intantaneous (now) mouse position on the chart
    text_position_now = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    
    //image
    wxPNGHandler *handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    
    
    draw_panel->Draw();
    
    //stores the x_min .. y_max, width_chart, height chart the first time that the chart is shown into x_min_0 ... height_chart_0
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);
    (draw_panel->width_chart_0) = (draw_panel->width_chart);
    (draw_panel->height_chart_0) = (draw_panel->height_chart);
    
    
    //initialize the variable neededed for slider
    value_slider_old = 1;
    //allocate the slider
    slider = new wxSlider(panel, wxID_ANY, value_slider_old, 1, (value_slider_max.value), wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);
    
    //text field showing the current value of the zoom slider
    s.str("");
    s << "1:" << value_slider_old;
    text_slider = new wxStaticText(panel, wxID_ANY, wxString(s.str().c_str()), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //navigation buttons
    button_up = new wxButton(panel, wxID_ANY, wxT("N"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_down = new wxButton(panel, wxID_ANY, wxT("S"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_left = new wxButton(panel, wxID_ANY, wxT("W"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_right = new wxButton(panel, wxID_ANY, wxT("E"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    
    button_up->Bind(wxEVT_BUTTON, &ChartFrame::MoveUp<wxCommandEvent>, this);
    button_down->Bind(wxEVT_BUTTON, &ChartFrame::MoveDown<wxCommandEvent>, this);
    button_left->Bind(wxEVT_BUTTON, &ChartFrame::MoveLeft<wxCommandEvent>, this);
    button_right->Bind(wxEVT_BUTTON, &ChartFrame::MoveRight<wxCommandEvent>, this);
    
    
    
    
    //    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(path_file_chart, wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    draw_panel->Bind(wxEVT_MOTION, wxMouseEventHandler(DrawPanel::OnMouseMovement), draw_panel);
    draw_panel->Bind(wxEVT_RIGHT_DOWN, wxMouseEventHandler(DrawPanel::OnMouseRightDown), draw_panel);
    draw_panel->Bind(wxEVT_LEFT_DOWN, wxMouseEventHandler(DrawPanel::OnMouseLeftDown), draw_panel);
    draw_panel->Bind(wxEVT_LEFT_UP, wxMouseEventHandler(DrawPanel::OnMouseLeftUp), draw_panel);
    draw_panel->Bind(wxEVT_MOTION, wxMouseEventHandler(DrawPanel::OnMouseDrag), draw_panel);
    
    slider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, wxScrollEventHandler(DrawPanel::OnScroll), draw_panel);
    
    draw_panel->SetMinSize(wxSize((draw_panel->c)->getWidth(),(draw_panel->c)->getHeight()));
    
    empty_text_1 = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    empty_text_2 = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    empty_text_3 = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    empty_text_4 = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    empty_text_5 = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    sizer_buttons->Add(empty_text_1);
    sizer_buttons->Add(button_up);
    sizer_buttons->Add(empty_text_2);
    sizer_buttons->Add(button_left);
    sizer_buttons->Add(empty_text_3);
    sizer_buttons->Add(button_right);
    sizer_buttons->Add(empty_text_4);
    sizer_buttons->Add(button_down);
    sizer_buttons->Add(empty_text_5);
    
    sizer_slider->Add(slider, 0, wxALIGN_CENTER | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_slider->Add(text_slider, 0, wxALIGN_CENTER | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_slider->Add(sizer_buttons, 0, wxALIGN_CENTER | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    
    sizer_h->Add(draw_panel, 0, wxALIGN_TOP | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_h->Add(sizer_slider, 0, wxALIGN_TOP | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    sizer_v->Add(text_position_now, 0, wxALIGN_LEFT | wxALL, ((this->GetSize()).GetWidth())*length_border_over_length_frame);
    //    sizer_v->Fit(panel);
    
    
    
    
    Maximize(panel);
    SetSizerAndFit(sizer_v);
    
    
    
    
    //    panel->SetSizer(sizer_v);
    //    sizer_v->Fit(this);
    
    //    SetSize((draw_panel->c)->getWidth() + ((draw_panel->c)->getWidth())*0.01, (draw_panel->c)->getHeight() + ((draw_panel->c)->getWidth())*0.01);
    
    //    SetAutoLayout(true);
    
    //    Fit();
    CentreOnScreen();
    
}

//moves (makes slide) up the chart
template<class T> void ChartFrame::MoveUp(T& event){
    
    double delta;
    
    delta = (relative_displacement.value) * ((draw_panel->y_max)-(draw_panel->y_min));
    
    if(((draw_panel->y_max)+delta < y_mercator(floor_max_lat)) && ((draw_panel->y_min)+delta > y_mercator(ceil_min_lat))){
        //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart
        
        //update y_min, y_max according to the drag.
        (draw_panel->y_min) += delta;
        (draw_panel->y_max) += delta;
        
        draw_panel->Update_lambda_phi_min_max();
        
        //re-draw the chart
        draw_panel->Draw();
        draw_panel->PaintNow();
        
    }
    
    event.Skip(true);
    
}

//moves (makes slide) down the chart
template<class T> void ChartFrame::MoveDown(T& event){
    
    double delta;
    
    delta = (relative_displacement.value) * ((draw_panel->y_max)-(draw_panel->y_min));
    
    if(((draw_panel->y_max)+delta < y_mercator(floor_max_lat)) && ((draw_panel->y_min)+delta > y_mercator(ceil_min_lat))){
        //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart
        
        //update y_min, y_max according to the drag.
        (draw_panel->y_min) -= delta;
        (draw_panel->y_max) -= delta;
        
        draw_panel->Update_lambda_phi_min_max();
        
        //re-draw the chart
        draw_panel->Draw();
        draw_panel->PaintNow();
        
    }
    
    
    event.Skip(true);
    
}

//moves (makes slide) left the chart
template<class T> void ChartFrame::MoveLeft(T& event){
    
    double delta;
    
    delta = (relative_displacement.value) * ((draw_panel->x_max)-(draw_panel->x_min));
    
    //update x_min, x_max according to the drag.
    (draw_panel->x_min) -= delta;
    (draw_panel->x_max) -= delta;
    
    draw_panel->Update_lambda_phi_min_max();
    
    //re-draw the chart
    draw_panel->Draw();
    draw_panel->PaintNow();
    
    event.Skip(true);
    
}

//moves (makes slide) up the chart
template<class T> void ChartFrame::MoveRight(T& event){
    
    double delta;
    
    delta = (relative_displacement.value) * ((draw_panel->x_max)-(draw_panel->x_min));
    
    //update x_min, x_max according to the drag.
    (draw_panel->x_min) += delta;
    (draw_panel->x_max) += delta;
    
    draw_panel->Update_lambda_phi_min_max();
    
    //re-draw the chart
    draw_panel->Draw();
    draw_panel->PaintNow();
    
    event.Skip(true);
    
}

void DrawPanel::SetIdling(bool b){
    
    idling = b;
    
}

//this function computes lambda_min, ... phi_max from x_min ... y_max
void DrawPanel::Update_lambda_phi_min_max(void){
    
    (((parent->parent)->plot)->lambda_min).set(String(""), k*lambda_mercator(x_min), String(""));
    (((parent->parent)->plot)->lambda_max).set(String(""), k*lambda_mercator(x_max), String(""));
    
    (((parent->parent)->plot)->phi_min).set(String(""), k*phi_mercator(y_min), String(""));
    (((parent->parent)->plot)->phi_max).set(String(""), k*phi_mercator(y_max), String(""));
    
}

//this function computes x_min, ... y_max from lambda_min ... phi_max
void DrawPanel::Update_x_y_min_max(void){
    
    x_min = x_mercator(K*((((parent->parent)->plot)->lambda_min).value));
    x_max = x_mercator(K*((((parent->parent)->plot)->lambda_max).value));
    y_min = y_mercator(K*((((parent->parent)->plot)->phi_min).value));
    y_max = y_mercator(K*((((parent->parent)->plot)->phi_max).value));
    
}

void ChartFrame::UpdateSliderLabel(void){
    
    stringstream s;
    
    s.str("");
    s << "1:" << value_slider_old;
    text_slider->SetLabel(s.str().c_str());
    
}

//computes the zoom factor of the chart based on the currenct value of delta_x = x_max - x_min and writes it as a double into *f. It returns true if the zooming factor is smaller than value_slider_max, and false otherwise
bool ChartFrame::ZoomFactor(double delta_x, double* f){
    
    double x;
    
    x = ((double)(draw_panel->width_chart))/((double)(draw_panel->width_chart_0))*((draw_panel->x_max_0)-(draw_panel->x_min_0))/delta_x;
    
    //if f != NULL, then I write the zoom factor into f
    if(f){(*f) = x;}
    
    return(((unsigned int)x) < (value_slider_max.value));
    
}

//this function updates the slider according to the zooming factor of the chart.
void ChartFrame::UpdateSlider(void){
    
    double f;
    
    //compute the zooming factor of the chart and write it into value_slider_old
    ZoomFactor(((draw_panel->x_max)-(draw_panel->x_min)), &f);
    value_slider_old = ((unsigned int)f);
    
    
    slider->SetValue(value_slider_old);
    
    UpdateSliderLabel();
    
}


template<class T>void CheckBody::operator()(T& event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        //I check whether the name in the GUI field body matches one of the body names in catalog
        for(check = false, i=0; (i<((p->catalog)->list).size()) && (!check); i++){
            if(String(((p->name)->GetValue().ToStdString())) == ((((p->catalog)->list)[i]).name)){
                check = true;
            }
        }
        i--;
        
        if(check){
            
            //            (*(p->body)) = ((p->catalog)->list)[i];
            
            if((((p->catalog)->list)[i].name == String("sun")) || (((p->catalog)->list)[i].name == String("moon"))){
                ((f->limb)->name)->Enable(true);
            }else{
                ((f->limb)->name)->Enable(false);
            }
            
            (p->name)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->name);
            ((f->print_error_message)->title) = String("Body not found in catalog!");
            ((f->print_error_message)->message) = String("Body must be in catalog.");
            f->CallAfter(*(f->print_error_message));
            
            (p->ok) = false;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}


template<class T> void CheckLimb::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        
        bool check;
        String s;
        
        
        s = String(((p->name)->GetValue().ToStdString()));
        //I check whether the name in the GUI field body matches one of the valid limb names
        check = (s == String("upper")) || (s == String("lower")) || (s == String("center"));
        
        if(check){
            
            
            
            (p->name)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->name);
            ((f->print_error_message)->title) = String("Limb not valid!");
            ((f->print_error_message)->message) = String("Limb must be upper, lower or center.");
            f->CallAfter(*(f->print_error_message));
            
            (p->ok) = false;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}


//writes the value contained in the GUI field into the non-GUI field
template<class T> void LimbField::get(T &event){
    
    if(ok){
        
        //I set the char in (limb->value) to the first letter in the string contained in the GUI field
        (limb->value) = ((String((name->GetValue().ToStdString()))).value)[0];
        
    }
    
    event.Skip(true);
    
}


//checks the value of the sign in the GUI field
template<class P> template <class T> void CheckSign<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is in idling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        //I check whether the name in the GUI field sign matches one of the sign values in the list signs
        if((p->format) == String("")){
            //if the AngleField p has no sign, the check is ok
            
            check=true;
            
        }else{
            //if the AngleField p has a sign, I check it
            
            for(check = false, i=0; (i<((p->signs).GetCount())) && (!check); i++){
                if(((p->sign)->GetValue()) == (p->signs)[i]){
                    check = true;
                }
            }
            
        }
        
        if(check){
            
            (p->sign)->SetBackgroundColour(*wxWHITE);
            (p->sign_ok) = true;
            
            //            p->get(event);
            
        }else{
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->sign);
            ((f->print_error_message)->title) = String("Sign is not valid!");
            ((f->print_error_message)->message) = String("Sign must be +-, NS or EW.");
            f->CallAfter(*(f->print_error_message));
            
            (p->sign_ok) = false;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

//converts the point p on the screen (which is supposed to lie in the plot area), to the relative geographic position q
void DrawPanel::ScreenToGeo(wxPoint p, Position *q){
    
    //updates the position of the draw pane this
    position_draw_panel = (this->GetScreenPosition());
    
    (q->lambda).set(String(""), k*lambda_mercator(x_min+ (((double)(p.x)-((position_draw_panel.x)+(position_plot_area.x)))/((double)(size_plot_area.x)))*(x_max - x_min)), String(""));
    (q->phi).set(String(""), k*(phi_mercator(y_min - (((double)((p.y)-((position_draw_panel.y)+(position_plot_area.y)+(size_plot_area.y))))/((double)(size_plot_area.y)))*(y_max - y_min) )), String(""));
    
    
}

//converts the point p on the screen (which is supposed to lie in the plot area), to the  Mercator projection (x,y) of the relative geographic position
void DrawPanel::ScreenToMercator(wxPoint p, double* x, double* y){
    
    //updates the position of the draw pane this
    position_draw_panel = (this->GetScreenPosition());
    
    if(x){
        (*x) = x_min + (((double)(p.x)-((position_draw_panel.x)+(position_plot_area.x)))/((double)(size_plot_area.x)))*(x_max - x_min);
        
    }
    if(y){
        (*y) = y_min - (((double)((p.y)-((position_draw_panel.y)+(position_plot_area.y)+(size_plot_area.y))))/((double)(size_plot_area.y)))*(y_max - y_min);
    }
    
    
}


//this function converts the geographic position p into the screen position p
void DrawPanel::GeoToScreen(Position q, wxPoint *p){
    
    //updates the position of the draw pane this
    position_draw_panel = (this->GetScreenPosition());
    
    (p->x) = (position_draw_panel.x) + (position_plot_area.x) + (x_mercator(K*((q.lambda).value))-x_min)/(x_max-x_min)*width_plot_area;
    (p->y) = (position_draw_panel.y) + (position_plot_area.y) + (height_plot_area) - ((y_mercator(K*((q.phi).value))-y_min)/(y_max-y_min)*height_plot_area);
    
    //    cout << "B = screen = " << (p->x) << " " << (p->y) << "\n";
    
    
}

//this function converts the geographic position p into the  position p with respect to the origin of the plot area
bool DrawPanel::GeoToPlot(Position q, wxPoint *p){
    
    double x_temp, y_temp;
    
    x_temp = x_mercator(K*((q.lambda).value));
    y_temp = y_mercator(K*((q.phi).value));
    
    if(((x_temp > x_min) && (x_temp < x_max)) && ((y_temp > y_min) && (y_temp < y_max))){
        //if the point falls within the plot area, write it into p
        
        (p->x) = (position_plot_area.x) + (x_temp-x_min)/(x_max-x_min)*width_plot_area;
        (p->y) = (position_plot_area.y) + height_plot_area - ((y_temp-y_min)/(y_max-y_min)*height_plot_area);
        
        return true;
        
    }else{
        
        return false;
    }
    
    
}

//This function obtains the geographical Position p of the mouse hovering on the map of the world
void DrawPanel::GetMouseGeoPosition(Position* p){
    
    position_screen_now = wxGetMousePosition();
    ScreenToGeo(position_screen_now, p);
    
}

void DrawPanel::OnMouseMovement(wxMouseEvent &event){
    
    Position p;
    wxPoint q;
    stringstream s;
    unsigned int i, j;
    
    //    cout << "\nMouse moved";
    
    GetMouseGeoPosition(&p);
    
    //update the instantaneous position of the mouse on the chart
    s.str("");
    s << (p.phi).to_string(String("NS"), display_precision) << " " << (p.lambda).to_string(String("EW"), display_precision);
    (parent->text_position_now)->SetLabel(wxString(s.str().c_str()));
    
    //if a selection rectangle is being drawn, update the instantaneous position of the final corner of the rectangle
    if(selection_rectangle){
        s.str("");
        s << (p.phi).to_string(String("NS"), display_precision) << " " << (p.lambda).to_string(String("EW"), display_precision);
        text_position_end->SetLabel(wxString(s.str().c_str()));
        text_position_end->SetPosition(wxPoint((position_screen_now.x)-(position_draw_panel.x), (position_screen_now.y)-(position_draw_panel.y)));
        PaintNow();
    }
    
    
    //I run over all the routes, check if the mouse is hovering over one of them, and change the background color of the related position in listcontrol_routes
    //I compute the position of the mouse with respect to the origin of the DrawPanel, so I can compare it with points_route_list[i][j], which are also with respect to the origin of the draw panel
    position_draw_panel_now = position_screen_now - position_draw_panel;
    
    for(i=0; i<(plot->route_list).size(); i++){
        
        //set the beckgorund color of the Route in listcontrol_routes and of its related sight to white
        ((parent->parent)->listcontrol_routes)->SetItemBackgroundColour(i, wxColour(255,255,255));
        if((((plot->route_list)[i]).related_sight).value != -1){
            ((parent->parent)->listcontrol_sights)->SetItemBackgroundColour((((plot->route_list)[i]).related_sight).value, wxColour(255,255,255));
        }
        
        
        for(j=0; j<(points_route_list[i]).size(); j++){
            //if the mouse is hovering over one of the points of route #j, I set the background color of route j in listcontrol_routes to a color different from white, to highlight it, and I highlight also the related sight in listcontrol_sights
            
            if(sqrt(gsl_pow_2((position_draw_panel_now.x) - ((points_route_list[i][j]).x)) + gsl_pow_2((position_draw_panel_now.y) - ((points_route_list[i][j]).y))) <
               (((parent->standard_thickness_over_length_screen).value)/2.0 * ((parent->parent)->rectangle_display).GetWidth())){
                
                //set the beckgorund color of the Route in listcontrol_routes and of its related sight to a highlight color
                ((parent->parent)->listcontrol_routes)->SetItemBackgroundColour(i, wxColour(51,153,255));
                if((((plot->route_list)[i]).related_sight).value != -1){
                    ((parent->parent)->listcontrol_sights)->SetItemBackgroundColour((((plot->route_list)[i]).related_sight).value, wxColour(51,153,255));
                }
                
            }
            
        }
        
    }
    
    
    //I run over all the positions, check if the mouse is hovering over one of them, and change the background color of the related position in listcontrol_positions
    for(i=0; i<(plot->position_list).size(); i++){
        
        GeoToScreen((plot->position_list)[i], &q);
        
        if(sqrt(gsl_pow_2((position_screen_now.x) - (q.x)) + gsl_pow_2((position_screen_now.y) - (q.y))) <
           4.0 * (((parent->standard_thickness_over_length_screen).value)/2.0 * ((parent->parent)->rectangle_display).GetWidth())){
            
            ((parent->parent)->listcontrol_positions)->SetItemBackgroundColour(i, wxColour(51,153,255));
            
        }else{
            
            ((parent->parent)->listcontrol_positions)->SetItemBackgroundColour(i, wxColour(255,255,255));
            
        }
        
    }
    
    
    event.Skip(true);
    
}

//if the left button of the mouse is pressed, I record its position as the starting position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftDown(wxMouseEvent &event){
    
    position_start_drag = wxGetMousePosition();
    
    //I store the boundaries of the plot at the beginning of the drag, so if the drag is aborted I will restore these boundaries
    x_min_start_drag = x_min;
    x_max_start_drag = x_max;
    y_min_start_drag = y_min;
    y_max_start_drag = y_max;
    
    Position geo;
    ScreenToGeo(position_start_drag, &geo);
    geo.print(String("Position start drag"), String("************ "), cout);
    
    event.Skip(true);
    
}

//if the left button of the mouse is released, I record its position as the ending position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftUp(wxMouseEvent &event){
    
    SetCursor(*wxCROSS_CURSOR);
    
    position_end_drag = wxGetMousePosition();
    
    Position geo;
    ScreenToGeo(position_now_drag, &geo);
    
    geo.print(String("Position end drag"), String("************ "), cout);
    
    double delta_x, delta_y;
    
    delta_x = ((double)((position_end_drag.x)-(position_start_drag.x)))/((double)width_plot_area) * (x_max-x_min);
    delta_y = ((double)((position_end_drag.y)-(position_start_drag.y)))/((double)height_plot_area) * (y_max-y_min);
    
    if(!((y_max+delta_y < y_mercator(floor_max_lat)) && (y_min+delta_y > y_mercator(ceil_min_lat)))){
        //if the drag operation brings the chart out of the min and max latitude contained in the data files, I reset x_min, ..., y_max to the values at the beginning of the drag, and set lambda_min, ..., phi_max accordingly.
        
        x_min = x_min_start_drag;
        x_max = x_max_start_drag;
        y_min = y_min_start_drag;
        y_max = y_max_start_drag;
        
        Update_lambda_phi_min_max();
        
        //re-draw the chart
        Draw();
        PaintNow();
        
        
        //        set the wxControl, title and message for the functor print_error_message, and then call the functor
        (print_error_message->control) = NULL;
        (print_error_message->title) = String("Chart outside  boundaries!");
        (print_error_message->message) = String("The chart must lie within the boundaries.");
        (*print_error_message)();
        
        
    }
    
    event.Skip(true);
    
}

void DrawPanel::OnMouseRightDown(wxMouseEvent &event){
    
    stringstream s;
    
    //changes the 'sign' of selection rectangle
    selection_rectangle = (!selection_rectangle);
    
    if(selection_rectangle){
        
        stringstream s;
        
        cout << "You started drawing\n";
        //        ((parent->plot)->lambda_min) = (p.lambda);
        //        ((parent->plot)->phi_min) = (p.phi);
        //
        
        GetMouseGeoPosition(&p_start);
        position_start_selection = position_screen_now;
        //stores the x at the beginning of the selection process, to compute the zoom factor later
        ScreenToMercator(position_start_selection, &x_start_selection, NULL);
        
        s.clear();
        s << (p_start.phi).to_string(String("NS"), display_precision) << " " << (p_start.lambda).to_string(String("EW"), display_precision);
        text_position_start->SetLabel(wxString(s.str().c_str()));
        text_position_start->SetPosition(wxPoint((position_start_selection.x)-(position_draw_panel.x), (position_start_selection.y)-(position_draw_panel.y)));
        
        cout << "p_start = {" << (p_start.lambda).to_string(String("EW"), display_precision) << " , " << (p_start.phi).to_string(String("NS"), display_precision) << " }\n";
        
    }else{
        
        cout << "You ended drawing\n";
        
        GetMouseGeoPosition(&p_end);
        position_end_selection = position_screen_now;
        //stores the x at the end of the selection process, to compute the zoom factor later
        ScreenToMercator(position_end_selection, &x_end_selection, NULL);
        
        if((parent->ZoomFactor(fabs(x_end_selection-x_start_selection), NULL))){
            //if the zoom factor of the map resulting from the selection is valid, I update x_min, ... , y_max
            
            cout << "p_end = {" << (p_end.lambda).to_string(String("EW"), display_precision) << " , " << (p_end.phi).to_string(String("NS"), display_precision) << " }\n";
            
            //reinitialize c and sets the new values of lambda_min, lambda_max, phi_min and phi_max
            delete c;
            //I convert all the angles to the format between -pi and pi, so I can sort them numerically
            (p_start.phi).normalize_pm_pi();
            (p_start.lambda).normalize_pm_pi();
            (p_end.phi).normalize_pm_pi();
            (p_end.lambda).normalize_pm_pi();
            //I assign the values of lambda_min and lamba_max, phi_min and phi_max from the vluaes of p_start.lambda, ... p_end.phi in such a way that lambda_min correspnds to the longitude of the leftmost edge x_min of the mercator projection, lambda_max to the rightmost one, etc.
            if((p_start.lambda)>(p_end.lambda)){
                (((parent->parent)->plot)->lambda_min) = (p_start.lambda);
                (((parent->parent)->plot)->lambda_max) = (p_end.lambda);
            }else{
                (((parent->parent)->plot)->lambda_min) = (p_end.lambda);
                (((parent->parent)->plot)->lambda_max) = (p_start.lambda);
            }
            if((p_start.phi)>(p_end.phi)){
                (((parent->parent)->plot)->phi_max) = (p_start.phi);
                (((parent->parent)->plot)->phi_min) = (p_end.phi);
            }else{
                (((parent->parent)->plot)->phi_min) = (p_start.phi);
                (((parent->parent)->plot)->phi_max) = (p_end.phi);
            }
            //I normalize lambda_min, ..., phi_max for future use.
            (((parent->parent)->plot)->lambda_min).normalize();
            (((parent->parent)->plot)->lambda_max).normalize();
            (((parent->parent)->plot)->phi_min).normalize();
            (((parent->parent)->plot)->phi_max).normalize();
            
            Draw();
            PaintNow();
            
            parent->UpdateSlider();
            
        }else{
            //if the zoom factor is not valid, then I print an error message
            
            s.str("");
            s << "Zoom level must be >= 1 and <= " << ((parent->value_slider_max).value) << ".";
            
            //set the title and message for the functor print_error_message, and then call the functor
            (print_error_message->control) = NULL;
            (print_error_message->title) = String("Zoom level exceeded its maximal value!");
            (print_error_message->message) = String(s.str().c_str());
            (*print_error_message)();
            
        }
        
        
        
        //I set to empty the text fields of the geographical positions of the selek÷ction triangle, which is now useless
        text_position_start->SetLabel(wxString(""));
        text_position_end->SetLabel(wxString(""));
        
    }
    
    event.Skip(true);
    
}

void DrawPanel::OnMouseDrag(wxMouseEvent &event){
    
    if(wxGetMouseState().LeftIsDown()){
        
        mouse_dragging = true;
        
        SetCursor(wxCURSOR_HAND);
        
        double delta_x, delta_y;
        
        position_now_drag = wxGetMousePosition();
        
        delta_x = ((double)((position_now_drag.x)-(position_start_drag.x)))/((double)width_plot_area) * (x_max-x_min);
        delta_y = ((double)((position_now_drag.y)-(position_start_drag.y)))/((double)height_plot_area) * (y_max-y_min);
        
        if((y_max+delta_y < y_mercator(floor_max_lat)) && (y_min+delta_y > y_mercator(ceil_min_lat))){
            //if the drag operation does not bring the chart out of the min and max latitude contained in the data files, I update x_min, ..., y_max and update the chart
            
            //update x_min, ..., y_max according to the drag.
            x_min -= delta_x;
            x_max -= delta_x;
            y_min += delta_y;
            y_max += delta_y;
            
            Update_lambda_phi_min_max();
            
            //re-draw the chart
            Draw();
            
            PaintNow();
            
        }
        
    }
    
    event.Skip(true);
    
}

void DrawPanel::OnScroll(wxScrollEvent &event){
    
    double r;
    
    r = ((double)(parent->value_slider_old)) / ((double)((parent->slider)->GetValue()));
    
    //    cout << "Slider = " << value_slider_from_scroll << "\n";
    
    //store the values of x_min ... y_max before the scrolling event into x_min_old .... y_max_old. The value of the slider before the sliding event is already stored in value_slider_old
    x_min_old = x_min;
    x_max_old = x_max;
    y_min_old = y_min;
    y_max_old = y_max;
    
    
    //update x_min, ..., y_max according to the zoom (scroll) and lambda_min, ..., phi_max
    x_min = (x_max_old + x_min_old)/2.0 - ( (x_max_old-x_min_old)/2.0 * r );
    x_max = (x_max_old + x_min_old)/2.0 + ( (x_max_old-x_min_old)/2.0 * r );
    y_min = (y_max_old + y_min_old)/2.0 - ( (y_max_old-y_min_old)/2.0 * r );
    y_max = (y_max_old + y_min_old)/2.0 + ( (y_max_old-y_min_old)/2.0 * r );
    
    if(!((y_max < y_mercator(floor_max_lat)) && (y_min > y_mercator(ceil_min_lat)) && (x_min > -M_PI) && (x_max < M_PI))){
        //if the drag operation brings the chart out of the min and max latitude contained in the data files, I reset x_min, ..., y_max and the value of the slider to the values at the beginning of the drag, and set lambda_min, ..., phi_max accordingly.
        
        x_min = x_min_old;
        x_max = x_max_old;
        y_min = y_min_old;
        y_max = y_max_old;
        
        Update_lambda_phi_min_max();
        
        //put the slider back to the value before the scroll
        parent->UpdateSlider();
        
        //        set the wxControl, title and message for the functor print_error_message, and then call the functor
        (print_error_message->control) = NULL;
        (print_error_message->title) = String("You moved the slider: Chart outside  boundaries!");
        (print_error_message->message) = String("The chart must lie within the boundaries.");
        (*print_error_message)();
        

        
    }else{
        //if the slide operation is valid, I update everything and re-draw the chart
        
        Update_lambda_phi_min_max();
        
        //update parent->value_slider_old
        (parent->value_slider_old) = ((parent->slider)->GetValue());
        
        Draw();
        PaintNow();
        parent->UpdateSliderLabel();
        
    }
    
    event.Skip(true);
    
}

//writes to the non-GUI field angle the values written in the GUI fields sign, deg and min
template<class P> template <class T> void AngleField<P>::get(T &event){
    
    if(is_ok()){
        
        
        double min_temp;
        char c;
        
        if(format == String("")){
            //in this case there is no sign in AngleField->this:
            
            c='+';
            
        }else{
            //in this case there is a sign in AngleField->this: I write the sign in c
            
            wxString s;
            s = (sign->GetValue());
            
            if(format == String("+-")){
                
                if(s==wxString("+")){c='+';}
                else{c='-';}
                
            }
            
            if(format == String("EW")){
                
                if(s==wxString("W")){c='+';}
                else{c='-';}
                
            }
            
            if(format == String("NS")){
                
                if(s==wxString("N")){c='+';}
                else{c='-';}
                
            }
            
        }
        
        
        (min->GetValue()).ToDouble(&min_temp);
        
        angle->from_sign_deg_min(c, wxAtoi(deg->GetValue()), min_temp);
        
    }
    
    event.Skip(true);
    
}


//constructor of the struct, which initializes the Answer remove_related_route. If remove_related_route.value = 'y', then DeleteSight::operator() will delete both the sight and the related route. If remove_related_route.value = 'n', then it will remove the sight only.
DeleteSight::DeleteSight(ListFrame* f_in, Answer remove_related_route_in){
    
    f = f_in;
    remove_related_route = remove_related_route_in;
    
}

void DeleteSight::operator()(wxCommandEvent& event){
    
    int i_related_route;
    
    i_related_route = ((((f->plot)->sight_list)[i_sight_to_remove]).related_route).value;
    
    
    //remove the route related to the sight which I am about to remove from the GUI object listcontrol_routes
    if((i_related_route != -1) && (remove_related_route == Answer('y', String("")))){
        
        (f->listcontrol_routes)->DeleteItem(i_related_route);
        
    }
    
    //I remove the sight and the related route from both the non-GUI object plot
    (f->plot)->remove_sight(i_sight_to_remove, remove_related_route, String(""));
    
    f->UpdateRelatedSightsAndRoutes();
    
    f->plot->print(true, String("--------- "), cout);
    
    
    event.Skip(true);
    
}

//constructor of the struct, which initializes the Answer remove_related_sight. If remove_related_sight.value = 'y', then DeleteRoute::operator() will delete both the route and the related sight. If remove_related_sight.value = 'n', then it will remove the route only.
DeleteRoute::DeleteRoute(ListFrame* f_in, Answer remove_related_sight_in){
    
    f = f_in;
    remove_related_sight = remove_related_sight_in;
    
}

void DeleteRoute::operator()(wxCommandEvent& event){
    
    int i_related_sight;
    
    i_related_sight = ((((f->plot)->route_list)[i_route_to_remove]).related_sight).value;
    
    //remove the sight related to the route which I am about to remove from the GUI object listcontrol_sights
    if((i_related_sight != -1) && (remove_related_sight == Answer('y', String("")))){
        
        (f->listcontrol_sights)->DeleteItem(i_related_sight);
        
    }
    
    //I remove the route and the related sight from both the non-GUI object plot
    (f->plot)->remove_route(i_route_to_remove, remove_related_sight, String(""));
    
    f->UpdateRelatedSightsAndRoutes();
    
    f->plot->print(true, String("--------- "), cout);
    
    event.Skip(true);
    
}




template<class P> template <class T> void SetStringToCurrentTime<P>::operator()(T& event){
    
    //if the label is empty, I replace it with the local time and date
    if(((p->value)->GetValue()).IsEmpty()){
        
        Time time_temp;
        
        time_temp.set_current(String(""));
        //I write in the non-GUI object (p->string)
        (*(p->string)) = String(time_temp.to_string(display_precision));
        
        p->set();
        
    }
    
    event.Skip(true);
    
}

CheckBody::CheckBody(BodyField* p_in){
    
    p = p_in;
    
}

template<class F> CloseFrame<F>::CloseFrame(F* frame_in){
    
    frame = frame_in;
    
}

//closes a frame of type F
template<class F> template <class T> void CloseFrame<F>::operator()(T& event){
    
    event.Skip(true);
    
    frame->Close(true);
    
}


//this functor does nothing, delete it in the future
template<class P> template<class T> void CheckString<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //    (p->string)->set(String(""), String(((p->value)->GetValue()).ToStdString()), String(""));
    
    f->TryToEnableOk();
    
    event.Skip(true);
    
}


//I write in the non-GUI object string the value entered in the GUI object value
template<class P> template<class T> void StringField<P>::get(T &event){
    
    //here I don't check whether the StringField is ok, because any value in the string field is ok
    (*string) = String((value->GetValue().ToStdString()));
    
    event.Skip(true);
    
}

template<class P> CheckAngle<P>::CheckAngle(AngleField<P>* p_in){
    
    p = p_in;
    
    (check_sign.p) = p;
    (check_arc_degree.p) = p;
    (check_arc_minute.p) = p;
    
}

//this functor checks the whole angle field by calling the check on its sign, arcdegree and arcminute parts‰
template<class P> template <class T> void CheckAngle<P>::operator()(T& event){
    
    check_sign(event);
    check_arc_degree(event);
    check_arc_minute(event);
    
    event.Skip(true);
    
}

CheckDate::CheckDate(DateField* p_in){
    
    p = p_in;
    
    check_year = new CheckYear(p);
    check_month = new CheckMonth(p);
    check_day = new CheckDay(p);
    
    
    
}

//this functor checks the whole date field by calling the check on its year, month and day parts
template <class T> void CheckDate::operator()(T& event){
    
    check_year(event);
    check_month(event);
    check_day(event);
    
    event.Skip(true);
    
}

//this functor writes the values written inthe whole GUI date field (year, month and day) in the respective non-GUI object date->D, date->M, date->D
template <class T> void DateField::get(T& event){
    
    if(year_ok && (year->IsEnabled()) && month_ok && (month->IsEnabled()) && day_ok && (month->IsEnabled())){
        
        (date->Y) = (unsigned int)wxAtoi(year->GetValue());
        (date->M) = (unsigned int)wxAtoi(month->GetValue());
        (date->D) = (unsigned int)wxAtoi(day->GetValue());
        
    }
    
    event.Skip(true);
    
}


template<class P> template<class T> void CheckArcDegree<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->deg)->GetValue()).ToStdString(), NULL, true, 0, 360)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->deg), String("Entered value is not valid!\nArcdegrees must be unsigned integer numbers >= 0° and < 360°"));
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->deg);
            ((f->print_error_message)->title) = String("Entered value is not valid!");
            ((f->print_error_message)->message) = String("Arcdegrees must be unsigned integer numbers >= 0° and < 360°");
            f->CallAfter(*(f->print_error_message));
            
            (p->deg_ok) = false;
            
        }else{
            
            (p->deg)->SetBackgroundColour(*wxWHITE);
            
            //            if((p->min_ok)){
            //
            //                double min_temp;
            //
            //                ((p->min)->GetValue()).ToDouble(&min_temp);
            //
            //                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
            //
            //            }
            
            (p->deg_ok) = true;
            
            //            p->get(event);
            
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

template<class P> template <class T> void CheckArcMinute<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->min)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->min, String("Entered value is not valid!\nArcminutes must be floating-point numbers >= 0' and < 60'"));
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->min);
            ((f->print_error_message)->title) = String("Entered value is not valid!");
            ((f->print_error_message)->message) = String("Arcminutes must be floating-point numbers >= 0' and < 60'");
            f->CallAfter(*(f->print_error_message));
            
            (p->min_ok) = false;
            
        }else{
            (p->min)->SetBackgroundColour(*wxWHITE);
            //            if((p->deg_ok)){
            //
            //                double min_temp;
            //
            //                ((p->min)->GetValue()).ToDouble(&min_temp);
            //
            //                (p->angle)->from_sign_deg_min(*((const char*)(((p->sign)->GetValue()).mb_str())) , wxAtoi((p->deg)->GetValue()), min_temp);
            //
            //
            //            }
            (p->min_ok) = true;
            
            //            p->get(event);
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

//checks the value in the GUI field in LengthField
template<class P> template <class T> void CheckLengthValue<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->value)->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX) && ((p->value)->IsEnabled())){
            
            if(!(p->just_enabled)){
                //if the content of the GUI field p is invalid and p has not been just enabled, then I am authorized to prompt an error message
                
                //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
                ((f->print_error_message)->control) = (p->value);
                ((f->print_error_message)->title) = String("Entered value is not valid!");
                ((f->print_error_message)->message) = String("Lengths must be floating-point numbers >= 0 m");
                f->CallAfter(*(f->print_error_message));
                
            }else{
                //if the LengthField p has just been enabled, I do not print any error message even if the content of p is invalid: this is because I want to give the opportunity to the user to enter the content of the GUI field before complaining that the content of the GUI field is invalid. However, I set just_enabled to false, because p is no longer just enabled.
                
                (p->just_enabled) = false;
                
            }
            
            (p->value_ok) = false;
            
        }else{
            
            (p->value)->SetBackgroundColour(*wxWHITE);
            (p->value_ok) = true;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

//checks the unit in the GUI field in LengthField
template<class P> template <class T> void CheckLengthUnit<P>::operator()(T &event){
    
    P* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        //I check whether the name in the GUI field unit matches one of the unit names in units
        for(check = false, i=0; (i<(p->units).size()) && (!check); i++){
            if(((p->box_unit)->GetValue()) == (p->units)[i]){
                check = true;
            }
        }
        i--;
        
        if(check){
            
            
            (p->box_unit)->SetBackgroundColour(*wxWHITE);
            (p->box_unit_ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->box_unit);
            ((f->print_error_message)->title) = String("Unit not found in list!");
            ((f->print_error_message)->message) = String("Unit must be in list.");
            f->CallAfter(*(f->print_error_message));
            
            (p->box_unit_ok) = false;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

template<class P> CheckLength<P>::CheckLength(LengthField<P>* p_in){
    
    p = p_in;
    
    (check_length_value.p) = p;
    (check_length_unit.p) = p;
    
}


//writes the value of the GUI field in LengthField into the non-GUI field length
template<class P> template <class T> void LengthField<P>::get(T &event){
    
    if(is_ok()){
        
        double length_temp;
        
        value->GetValue().ToDouble(&length_temp);
        
        if(box_unit->GetValue() == wxString("nm")){
            
            length->set(String(""), /*the length is entered in the GUI field is already in nm, thus no need to convert it*/length_temp, String(""));
            
        }
        if(box_unit->GetValue() == wxString("m")){
            
            length->set(String(""), /*the length is entered in the GUI field in meters, thus I convert it to nm here*/length_temp/(1e3*nm), String(""));
            
        }
        
    }
    
    event.Skip(true);
    
}



//if an item in listcontrol_sights is selected, then the modify_sight and delete_sight buttons are enabled
template<class T> void OnSelectInListControlSights::operator()(T& event){
    
    (f->button_modify_sight)->Enable(true);
    (f->button_delete_sight)->Enable(true);
    
    event.Skip(true);
    
}

//if an item in listcontrol_positions is selected, then the modify_position and delete_position buttons are enabled
template<class T> void OnSelectInListControlPositions::operator()(T& event){
    
    (f->button_modify_position)->Enable(true);
    (f->button_delete_position)->Enable(true);
    
    event.Skip(true);
    
}

//if an item in listcontrol_routes is selected, then the modify_route and delete_route buttons are enabled
template<class T> void OnSelectInListControlRoutes::operator()(T& event){
    
    (f->button_modify_route)->Enable(true);
    (f->button_delete_route)->Enable(true);
    
    event.Skip(true);
    
}


template<class T> void PrintErrorMessage<T>::operator()(void){
    
    MessageFrame* message_frame;
    
    //I may be about to prompt a temporary dialog window, thus I set f->idling to true
    f->SetIdling(true);
    
    if(control != NULL){
        
        if(((control->GetBackgroundColour()) != *wxRED)){
            
            message_frame = new MessageFrame(f, title.value, message.value, wxDefaultPosition, wxDefaultSize, String(""));
            message_frame ->Show(true);
            
            control->SetFocus();
            control->SetBackgroundColour(*wxRED);
            
        }
        
    }else{
        
        message_frame = new MessageFrame(f, title.value, message.value, wxDefaultPosition, wxDefaultSize, String(""));
        message_frame ->Show(true);
        
    }
    
    //The temporary dialog window may have been closed, thus I set f->idling to false
    f->SetIdling(false);
    
    
    
}



SightFrame::SightFrame(ListFrame* parent_input, Sight* sight_in, long list_position_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    parent = parent_input;
    
    //pointer to init.txt to read fixed sight data from in there
    File file_init;
    String new_prefix;
    unsigned int i, deg, common_width;
    double min;
    bool check = true;
    
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    idling = false;
    print_error_message = new PrintErrorMessage<SightFrame>(this);
    
    file_init.set_name(String(path_file_init));
    check &= (file_init.open(String("in"), prefix));
    
    catalog = new Catalog(String(path_file_catalog), String(""));
    
    //if this SightFrame has been constructed with sight_in = NULL, then I allocate a new Sight object with the pointer this->sight and set list_position to a 'NULL' value (list_position = -1). Otherwise, the pointer sight_in points to a valid Sight object -> I let this->sight point to sight_in, and set list_position to list_position_in.
    if(sight_in != NULL){
        sight = sight_in;
        list_position = list_position_in;
    }else{
        sight = new Sight();
        list_position = -1;
    }
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    
    
    
    
    sizer_grid_measurement = new wxFlexGridSizer(6, 2, 0, 0);
    sizer_grid_time = new wxFlexGridSizer(4, 2, 0, 0);
    sizer_grid_label = new wxFlexGridSizer(1, 2, 0, 0);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer = new wxBoxSizer(wxVERTICAL);
    
    //First off, I need to set TAI_minus_UTC, which will be used in the following. If sight_in = NULL,  I read it from from file_init
    if(sight_in==NULL){
        
        cout << prefix.value << YELLOW << "Reading TAI - UTC at time of master-clock synchronization with UTC from file " << file_init.name.value << " ...\n" << RESET;
        (sight->TAI_minus_UTC).read_from_file(String("TAI - UTC at time of master-clock synchronization with UTC"), file_init, true, new_prefix);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
        
    }
    
    
    
    for(i=0; i<((*catalog).list).size(); i++){
        bodies.Add((((*catalog).list)[i]).name.value.c_str());
    }
    wxStaticText* text_combo_body = new wxStaticText(panel, wxID_ANY, wxT("Celestial body"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    body = new BodyField(this, &(sight->body), catalog);
    
    wxStaticText* text_limb = new wxStaticText(panel, wxID_ANY, wxT("Limb"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //    combo_limb = new wxComboBox(panel, ID_combo_limb, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    limb = new LimbField(this, &(sight->limb));
    
    //sextant altitude
    wxStaticText* text_H_s = new wxStaticText(panel, wxID_ANY, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    H_s = new AngleField<SightFrame>(this, &(sight->H_s), String("+-"));
    
    //index error
    wxStaticText* text_index_error = new wxStaticText(panel, wxID_ANY, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //If sight_in = NULL, read index error from init file
    if(sight_in == NULL){
        cout << prefix.value << YELLOW << "Reading index error from file " << file_init.name.value << " ...\n" << RESET;
        (sight->index_error).read_from_file(String("index error"), file_init, true, new_prefix);
        (sight->index_error).to_deg_min(&deg, &min);
        cout << prefix.value << YELLOW << "... done.\n" << RESET;
    }
    index_error = new AngleField<SightFrame>(this, &(sight->index_error), String("+-"));
    index_error->set();
    
    //artificial horizon
    wxStaticText* text_artificial_horizon_check = new wxStaticText(panel, wxID_ANY, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    artificial_horizon_check = new CheckField< LengthField<SightFrame> >(this, &(sight->artificial_horizon), NULL, false);
    
    //height of eye
    wxStaticText* text_height_of_eye = new wxStaticText(panel, wxID_ANY, wxT("Height of eye"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    height_of_eye = new LengthField<SightFrame>(this, &(sight->height_of_eye), String("m"));
    //now that height_of_eye has been allocatd, I link artificial_horizon_check to height_of_eye
    (artificial_horizon_check->related_field) = height_of_eye;
    
    //master-clock date
    //sets  sight.master_clock_date_and_hour.date and sight.time.date to the current UTC date if this constructor has been called with sight_in = NULL
    if(sight_in == NULL){
        (sight->master_clock_date_and_hour).date.set_current(prefix);
        (sight->master_clock_date_and_hour).chrono.set_current(prefix);
        (sight->time).date.set_current(prefix);
        (sight->time).chrono.set_current(prefix);
    }
    wxStaticText* text_date = new wxStaticText(panel, wxID_ANY, wxT("Master-clock UTC date and hour of sight"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_date = new DateField(this, &(sight->master_clock_date_and_hour.date));
    master_clock_date->set();
    
    //master-clock chrono
    wxStaticText* text_space_1 = new wxStaticText(panel, wxID_ANY, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    master_clock_chrono = new ChronoField(this, &(sight->master_clock_date_and_hour.chrono));
    
    //if sight_in != NULL, then I initialize the GUI filed master_clock_chrono with the one written in sight_in. Otherwise, master_clock_chrono is initialized as empty.
    if(sight_in!=NULL){
        
        master_clock_chrono->set(sight->master_clock_date_and_hour.chrono);
        
    }else{
        
        //if sight_in == NULL, I have previously set the non-GUI object (sight->master_clock_date_and_hour).chrono to the current hour, and I write this value into the GUI object master_clock_chrono
        master_clock_chrono->set((sight->master_clock_date_and_hour).chrono);
        
    }
    
    //check/uncheck stopwatch
    wxStaticText* text_stopwatch_check = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    stopwatch_check = new CheckField<ChronoField>(this, &(sight->use_stopwatch), NULL, true);
    
    //stopwatch reading
    wxStaticText* text_stopwatch_reading = new wxStaticText(panel, wxID_ANY, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    //    stopwatch_reading = new ChronoField(this, &(sight.stopwatch));
    stopwatch_reading = new ChronoField(this, &(sight->stopwatch));
    //now that stopwatch_reading has been allocatd, I link stopwatch_check to stopwatch_reading
    (stopwatch_check->related_field) = stopwatch_reading;
    
    
    //initialize stopwatch_check and stopwatch_reading
    (stopwatch_check->checkbox)->SetValue(false);
    stopwatch_reading->Enable(false);
    
    wxStaticText* text_TAI_minus_UTC = new wxStaticText(panel, wxID_ANY, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    TAI_minus_UTC = new ChronoField(this, &(sight->TAI_minus_UTC));
    TAI_minus_UTC->set(sight->TAI_minus_UTC);
    
    //label
    wxStaticText* text_label = new wxStaticText(panel, wxID_ANY, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    label = new StringField<SightFrame>(this, &(sight->label));
    
    
    //buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_cancel->Bind(wxEVT_BUTTON, &SightFrame::OnPressCancel, this);
    
    button_reduce = new wxButton(panel, wxID_ANY, "Reduce", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    //I bind reduce button to label->set_string_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_reduce->Bind(wxEVT_BUTTON, &SightFrame::OnPressReduce, this);
    button_reduce->Bind(wxEVT_BUTTON, label->set_string_to_current_time);
    
    //If I press reduce, I want all the fields in this SightFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_reduce->Bind(wxEVT_BUTTON, &BodyField::get<wxCommandEvent>, body);
    button_reduce->Bind(wxEVT_BUTTON, &LimbField::get<wxCommandEvent>, limb);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, H_s);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, index_error);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField< LengthField<SightFrame> >::get<wxCommandEvent>, artificial_horizon_check);
    button_reduce->Bind(wxEVT_BUTTON, &LengthField<SightFrame>::get<wxCommandEvent>, height_of_eye);
    button_reduce->Bind(wxEVT_BUTTON, &DateField::get<wxCommandEvent>, master_clock_date);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField::get<wxCommandEvent>, master_clock_chrono);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<ChronoField>::get<wxCommandEvent>, stopwatch_check);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField::get<wxCommandEvent>, stopwatch_reading);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField::get<wxCommandEvent>, TAI_minus_UTC);
    button_reduce->Bind(wxEVT_BUTTON, &StringField<SightFrame>::get<wxCommandEvent>, label);
    
    
    //I enable the reduce button only if sight_in is a valid sight with the entries propely filled, i.e., only if sight_in != NULL
    button_reduce->Enable((sight_in != NULL));
    
    sizer_grid_measurement->Add(text_combo_body);
    body->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    //    sizer_grid_measurement->Add(combo_body);
    
    sizer_grid_measurement->Add(text_limb);
    limb->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_measurement->Add(text_H_s);
    H_s->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_measurement->Add(text_index_error);
    index_error->InsertIn<wxBoxSizer>(box_sizer_3);
    sizer_grid_measurement->Add(box_sizer_3);
    
    sizer_grid_measurement->Add(text_artificial_horizon_check);
    artificial_horizon_check->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_measurement->Add(text_height_of_eye);
    height_of_eye->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_time->Add(text_date);
    master_clock_date->InsertIn<wxBoxSizer>(box_sizer_4);
    box_sizer_4->Add(text_space_1);
    master_clock_chrono->InsertIn<wxBoxSizer>(box_sizer_4);
    sizer_grid_time->Add(box_sizer_4);
    
    sizer_grid_time->Add(text_stopwatch_check);
    stopwatch_check->InsertIn<wxFlexGridSizer>(sizer_grid_time);
    
    sizer_grid_time->Add(text_stopwatch_reading);
    stopwatch_reading->InsertIn<wxFlexGridSizer>(sizer_grid_time);
    
    sizer_grid_time->Add(text_TAI_minus_UTC);
    TAI_minus_UTC->InsertIn<wxFlexGridSizer>(sizer_grid_time);
    
    sizer_grid_label->Add(text_label);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);
    
    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer_2->Add(button_reduce, 0, wxALIGN_BOTTOM);
    
    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Measurement");
    sizer_box_time = new wxStaticBoxSizer(wxVERTICAL, panel, "Time");
    
    sizer_box_measurement->Add(sizer_grid_measurement);
    sizer_box_time->Add(sizer_grid_time);
    
    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Master-clock UTC date and hour of sight"
    common_width = GetTextExtent(wxS("Master-clock UTC date and hour of sight   ")).GetWidth();
    text_combo_body->SetMinSize(wxSize(common_width,-1));
    text_date->SetMinSize(wxSize(common_width,-1));
    text_label->SetMinSize(wxSize(common_width,-1));
    
    //add the various elements to sizer, by inserting a border of 5 in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_box_time, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, 5);
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, 5);
    
    
    //panel->SetSizer(sizer);
    Maximize(panel);
    
    CreateStatusBar();
    SetStatusText( "Welcome to Michele's text editor!" );
    
    SetSizerAndFit(sizer);
    //Maximize();
    
    file_init.close(prefix);
    
    if(!check){
        cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
    }
    
    /*
     //here I read a sample sight from file_sample_sight, store into sight and set all the fields in this to the data in sight with set()
     File file_sample_sight;
     file_sample_sight.set_name(String("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample_sight.txt"));
     file_sample_sight.open(String("in"), String(""));
     sight->read_from_file(file_sample_sight, String(""));
     file_sample_sight.close(String(""));
     set();
     */
    
    if(sight_in != NULL){set();}
    
    Centre();
    
    
}






PositionFrame::PositionFrame(ListFrame* parent_input, Position* position_in, long list_position_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    parent = parent_input;
    
    String new_prefix, /*the label which appears on button_ok*/label_button_ok;
    unsigned int common_width;
    
    
    bool check = true;
    
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    idling = false;
    print_error_message = new PrintErrorMessage<PositionFrame>(this);
    
    
    //    wxMenu *menuFile = new wxMenu;
    
    //if this PositionFrame has been constructed with position_in = NULL, then I allocate a new Position object with the pointer this->position and set list_position to a 'NULL' value (list_position = -1). Otherwise, the pointer position_in points to a valid Position object -> I let this->position point to position_in, and set list_position to list_position_in.
    if(position_in != NULL){
        position = position_in;
        list_position = list_position_in;
    }else{
        position = new Position();
        list_position = -1;
    }
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    sizer_grid_measurement = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_grid_label = new wxFlexGridSizer(1, 2, 0, 0);
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    
    
    //latitude
    wxStaticText* text_lat = new wxStaticText(panel, wxID_ANY, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    lat = new AngleField<PositionFrame>(this, &(position->phi), String("NS"));
    
    //longitude
    wxStaticText* text_lon = new wxStaticText(panel, wxID_ANY, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    lon = new AngleField<PositionFrame>(this, &(position->lambda), String("EW"));
    
    //label
    wxStaticText* text_label = new wxStaticText(panel, wxID_ANY, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    label = new StringField<PositionFrame>(this, &(position->label));
    
    
    //buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_cancel->Bind(wxEVT_BUTTON, &PositionFrame::OnPressCancel, this);
    
    //if I am adding a brand new position, I name button_ok 'Add'. Otherwise I name it "Modify"
    if(position_in == NULL){
        label_button_ok.set(String(""), String("Add"), String(""));
    }else{
        label_button_ok.set(String(""), String("Modify"), String(""));
    }
    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value, wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    
    
    //I bind reduce button to label->set_string_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_ok->Bind(wxEVT_BUTTON, label->set_string_to_current_time);
    //If I press reduce, I want all the fields in this PositionFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lat);
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lon);
    button_ok->Bind(wxEVT_BUTTON, &StringField<PositionFrame>::get<wxCommandEvent>, label);
    button_ok->Bind(wxEVT_BUTTON, &::PositionFrame::OnPressOk, this);
    
    
    //I enable the reduce button only if position_in is a valid position with the entries propely filled, i.e., only if position_in != NULL
    button_ok->Enable((position_in != NULL));
    
    sizer_grid_measurement->Add(text_lat, 0, wxALIGN_CENTER_VERTICAL);
    lat->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_measurement->Add(text_lon, 0, wxALIGN_CENTER_VERTICAL);
    lon->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);
    
    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer_2->Add(button_ok, 0, wxALIGN_BOTTOM);
    
    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Coordinates");
    
    sizer_box_measurement->Add(sizer_grid_measurement);
    
    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_lat->SetMinSize(wxSize(common_width,-1));
    text_lon->SetMinSize(wxSize(common_width,-1));
    text_label->SetMinSize(wxSize(common_width,-1));
    
    //add the various elements to sizer, by inserting a border of 5 in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, 5);
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, 5);
    
    
    //panel->SetSizer(sizer);
    Maximize(panel);
    
    CreateStatusBar();
    SetStatusText( "Welcome to Michele's text editor!" );
    
    SetSizerAndFit(sizer);
    //Maximize();
    
    
    if(!check){
        cout << prefix.value << RED << "Cannot read position!\n" << RESET;
    }
    
    
    
    if(position_in != NULL){set();}
    
    Centre();
    
    
}

RouteFrame::RouteFrame(ListFrame* parent_input, Route* route_in, long list_position_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size){
    
    parent = parent_input;
    
    String new_prefix, label_button_ok;
    unsigned int common_width;
    
    bool check = true;
    
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    idling = false;
    print_error_message = new PrintErrorMessage<RouteFrame>(this);
    
    //if this RouteFrame has been constructed with route_in = NULL, then I allocate a new Route object with the pointer this->route and set list_route to a 'NULL' value (list_route = -1). Otherwise, the pointer route_in points to a valid Route object -> I let this->route point to route_in, and set list_route to list_route_in.
    if(route_in != NULL){
        route = route_in;
        list_position = list_position_in;
    }else{
        route = new Route();
        list_position = -1;
    }
    
    
    
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    sizer_grid_type = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_grid_alpha = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_grid_l = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_grid_start = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_grid_GP = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_grid_omega = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_grid_label = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_box_data = new wxStaticBoxSizer(wxVERTICAL, panel, "Data");
    sizer_box_start = new wxStaticBoxSizer(wxVERTICAL, panel, "Start position");
    sizer_box_GP = new wxStaticBoxSizer(wxVERTICAL, panel, "Ground position");
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    
    //type:a wxComboBox which indicates the type of the route (loxodrome, orthordrome or circle of equal altitude)
    wxStaticText* text_type = new wxStaticText(panel, wxID_ANY, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    type = new RouteTypeField(this, &(route->type));
    
    //alpha
    wxStaticText* text_alpha = new wxStaticText(panel, wxID_ANY, wxT("Alpha"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    alpha = new AngleField<RouteFrame>(this, &(route->alpha), String(""));
    
    //l
    wxStaticText* text_l = new wxStaticText(panel, wxID_ANY, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    l = new LengthField<RouteFrame>(this, &(route->l), String("nm"));
    
    
    //start
    //start_phi
    wxStaticText* text_start_phi = new wxStaticText(panel, wxID_ANY, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    start_phi = new AngleField<RouteFrame>(this, &((route->start).phi), String("NS"));
    //start_lambda
    wxStaticText* text_start_lambda = new wxStaticText(panel, wxID_ANY, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    start_lambda = new AngleField<RouteFrame>(this, &((route->start).lambda), String("EW"));
    
    //GP (ground position)
    //GP_phi
    wxStaticText* text_GP_phi = new wxStaticText(panel, wxID_ANY, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    GP_phi = new AngleField<RouteFrame>(this, &((route->GP).phi), String("NS"));
    //GP_lambda
    wxStaticText* text_GP_lambda = new wxStaticText(panel, wxID_ANY, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    GP_lambda = new AngleField<RouteFrame>(this, &((route->GP).lambda), String("EW"));
    
    //omega
    wxStaticText* text_omega = new wxStaticText(panel, wxID_ANY, wxT("Omega"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    omega = new AngleField<RouteFrame>(this, &(route->omega), String(""));
    
    //label
    wxStaticText* text_label = new wxStaticText(panel, wxID_ANY, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    label = new StringField<RouteFrame>(this, &(route->label));
    
    
    //buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_cancel->Bind(wxEVT_BUTTON, &RouteFrame::OnPressCancel, this);
    
    //if I am adding a brand new route, I name button_ok 'Add'. Otherwise I name it "Modify"
    if(route_in == NULL){
        label_button_ok.set(String(""), String("Add"), String(""));
    }else{
        label_button_ok.set(String(""), String("Modify"), String(""));
    }
    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value , wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    //I bind reduce button to label->set_string_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    //    button_add->Bind(wxEVT_BUTTON, label->set_string_to_current_time);
    
    //If I press reduce, I want all the fields in this RouteFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    //    button_add->Bind(wxEVT_BUTTON, &AngleField<RouteFrame>::get<wxCommandEvent>, alpha);
    //    button_add->Bind(wxEVT_BUTTON, &AngleField<RouteFrame>::get<wxCommandEvent>, omega);
    //    button_add->Bind(wxEVT_BUTTON, &StringField<RouteFrame>::get<wxCommandEvent>, label);
    button_ok->Bind(wxEVT_BUTTON, &RouteFrame::OnPressOk, this);
    
    //If the user is about to enter a brand new route, then these fields are disable until a route type si specified
    if(route_in == NULL){
        
        alpha->Enable(false);
        l->Enable(false);
        start_phi->Enable(false);
        start_lambda->Enable(false);
        GP_phi->Enable(false);
        GP_lambda->Enable(false);
        omega->Enable(false);
        
    }
    
    //I enable the add button only if route_in is a valid route with the entries propely filled, i.e., only if route_in != NULL
    button_ok->Enable((route_in != NULL));
    
    sizer_grid_type->Add(text_type, 0, wxALIGN_CENTER_VERTICAL);
    type->InsertIn<wxFlexGridSizer>(sizer_grid_type);
    
    sizer_grid_alpha->Add(text_alpha, 0, wxALIGN_CENTER_VERTICAL);
    alpha->InsertIn<wxFlexGridSizer>(sizer_grid_alpha);
    
    sizer_grid_l->Add(text_l, 0, wxALIGN_CENTER_VERTICAL);
    l->InsertIn<wxFlexGridSizer>(sizer_grid_l);
    
    sizer_grid_omega->Add(text_omega, 0, wxALIGN_CENTER_VERTICAL);
    omega->InsertIn<wxFlexGridSizer>(sizer_grid_omega);
    
    sizer_grid_start->Add(text_start_phi);
    start_phi->InsertIn<wxFlexGridSizer>(sizer_grid_start);
    sizer_grid_start->Add(text_start_lambda);
    start_lambda->InsertIn<wxFlexGridSizer>(sizer_grid_start);
    
    sizer_box_start->Add(sizer_grid_start);
    
    sizer_grid_GP->Add(text_GP_phi);
    GP_phi->InsertIn<wxFlexGridSizer>(sizer_grid_GP);
    sizer_grid_GP->Add(text_GP_lambda);
    GP_lambda->InsertIn<wxFlexGridSizer>(sizer_grid_GP);
    
    sizer_box_GP->Add(sizer_grid_GP);
    
    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);
    
    sizer_box_data->Add(sizer_grid_type);
    sizer_box_data->Add(sizer_grid_alpha);
    sizer_box_data->Add(sizer_grid_l);
    sizer_box_data->Add(sizer_box_start);
    sizer_box_data->Add(sizer_box_GP);
    sizer_box_data->Add(sizer_grid_omega);
    
    box_sizer->Add(button_cancel, 0, wxALIGN_BOTTOM);
    box_sizer->Add(button_ok, 0, wxALIGN_BOTTOM);
    
    
    
    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_alpha->SetMinSize(wxSize(common_width,-1));
    text_omega->SetMinSize(wxSize(common_width,-1));
    text_label->SetMinSize(wxSize(common_width,-1));
    
    //add the various elements to sizer, by inserting a border of 5 in all directions
    sizer->Add(sizer_box_data, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, 5);
    sizer->Add(box_sizer, 1, wxALIGN_RIGHT | wxALL, 5);
    
    
    //panel->SetSizer(sizer);
    Maximize(panel);
    
    CreateStatusBar();
    SetStatusText( "Welcome to Michele's text editor!" );
    
    SetSizerAndFit(sizer);
    //Maximize();
    
    
    if(!check){
        cout << prefix.value << RED << "Cannot read route!\n" << RESET;
    }
    
    
    
    if(route_in != NULL){set();}
    
    Centre();
    
    
}



//set all the GUI fields in this equal to those in the non-GUI object this->position
void PositionFrame::set(void){
    
    lat->set();
    lon->set();
    label->set();
    
}

//set all the non-GUI objects in this->position  equal to those in the GUI fields
template<class T> void PositionFrame::get(T &event){
    
    lat->get(event);
    lon->get(event);
    label->get(event);
    
}

void PositionFrame::SetIdling(bool b){
    
    idling = b;
    
}

void PositionFrame::OnPressCancel(wxCommandEvent& event){
    
    //I am about to close the frame,  thus I set f->idling to true
    SetIdling(true);
    
    Close(TRUE);
    
}


//this function is triggered when button_ok is pressed
void PositionFrame::OnPressOk(wxCommandEvent& event){
    
    stringstream s;
    
    //writes the values of the GUI fields in the non-GUI fields
    get(event);
    
    position->print(String("position entered via GUI"), String(""), cout);
    
    //if the constructor of PositionFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of position_list
    if(list_position==-1){
        (((this->parent)->plot)->position_list).push_back(*position);
    }
    
    position->add_to_wxListCtrl(list_position, ((this->parent)->listcontrol_positions));
    
    //I call PaintNow() because the positions have changed, so I need to re-draw the chart
    ((parent->chart_frame)->draw_panel)->Draw();
    ((parent->chart_frame)->draw_panel)->PaintNow();
    
    event.Skip(true);
    
    Close(TRUE);
    
}


void RouteFrame::OnPressOk(wxCommandEvent& event){
    
    
    stringstream s;
    
    //writes the values of the GUI fields in the non-GUI fields
    get(event);
    
    route->print(String("route entered via GUI"), String(""), cout);
    
    
    if(list_position==-1){
        //if the constructor of RouteFrame has been called with route_in = NULL, then I push back the newly allocated route to the end of route_list and reduce it
        
        ((this->parent)->plot)->add_route(route, String(""));
        
    }
    
    //if I am adding a new Route, I resize points_route_list to add a new element to it
    if(list_position == -1){
        ((parent->chart_frame->draw_panel)->points_route_list).resize(((parent->chart_frame->draw_panel)->points_route_list).size() + 1);
    }
    
    parent->UpdateRelatedSightsAndRoutes();
    
    route->add_to_wxListCtrl(list_position, ((this->parent)->listcontrol_routes));
    
    //    parent->plot->print(true, String(""), cout);
    
    //I call PaintNow() because the positions have changed, so I need to re-draw the chart
    ((parent->chart_frame)->draw_panel)->Draw();
    ((parent->chart_frame)->draw_panel)->PaintNow();
    
    
    event.Skip(true);
    
    Close(TRUE);
}

void RouteFrame::OnPressCancel(wxCommandEvent& event){
    
    //I am about to close the frame,  thus I set f->idling to true
    SetIdling(true);
    
    Close(TRUE);
    
}

void RouteFrame::TryToEnableOk(void){
    
    button_ok->Enable((type->is_ok()) &&
                      (
                       ( ( (((type->name)->GetValue()) == wxString("loxodrome")) || (((type->name)->GetValue()) == wxString("orthodrome")) ) &&
                        ((alpha->is_ok()) && (start_phi->is_ok()) && (start_lambda->is_ok()) && (l->is_ok()) ))
                       ||
                       ( (((type->name)->GetValue()) == wxString("circle of equal altitude")) &&
                        ((omega->is_ok()) && (GP_phi->is_ok()) && (GP_lambda->is_ok()) ))
                       )
                      );
    
    
    
    
}

//sets the values in all the GUI fields equal to the values in the respective non-GUI fields
void RouteFrame::set(void){
    
    type->set();
    
    if(((type->name)->GetValue()) == wxString("circle of equal altitude")){
        //I disable the GUI fields which do not define a circle of equal altitude and set the others
        
        alpha->Enable(false);
        start_phi->Enable(false);
        start_lambda->Enable(false);
        l->Enable(false);
        
        GP_phi->set();
        GP_lambda->set();
        omega->set();
        
    }else{
        //I disable the GUI fields which do not define a loxodrome or orthodrome and set the others
        
        alpha->set();
        start_phi->set();
        start_lambda->set();
        l->set();
        
        GP_phi->Enable(false);
        GP_lambda->Enable(false);
        omega->Enable(false);
        
    }
    
    label->set();
    
    
}

//sets the values in all the non-GUI fields equal to the values in the respective GUI fields
template<class T> void RouteFrame::get(T& event){
    
    type->get(event);
    
    if(((type->name)->GetValue()) == wxString("circle of equal altitude")){
        
        GP_phi->get(event);
        GP_lambda->get(event);
        omega->get(event);
        
    }else{
        
        alpha->get(event);
        start_phi->get(event);
        start_lambda->get(event);
        l->get(event);
        
    }
    
    label->get(event);
    
    
}

void RouteFrame::SetIdling(bool b){
    
    idling = b;
    
}


//this function checks whether all the fields in PositionFrame are ok, and if they are, it enables the button_add
void PositionFrame::TryToEnableOk(void){
    
    button_ok->Enable((lat->is_ok()) && (lon->is_ok()));
    
}



MessageFrame::MessageFrame(wxWindow* parent, const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent, wxID_ANY, title, pos, size){
    
    wxDisplay display;
    wxPNGHandler *handler;
    wxRect rectangle;
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    //image
    handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*1./1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*1./1000.0);
    
    //allocate sizers
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(3, 1, 0, 0);
    
    
    wxStaticText* text = new wxStaticText(panel, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //buttons
    button_ok = new wxButton(panel, wxID_ANY, "Ok!", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_ok->Bind(wxEVT_BUTTON, &MessageFrame::OnPressOk, this);
    
    
    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(path_file_app_icon, wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    
    
    sizer_grid->Add(text, 0, wxALIGN_CENTER);
    sizer_grid->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    sizer_buttons->Add(button_ok, 0, wxALIGN_CENTER);
    sizer_grid->Add(sizer_buttons, 0, wxALIGN_CENTER);
    
    sizer_h->Add(sizer_grid, 0, wxALIGN_CENTER_VERTICAL);
    sizer_v->Add(sizer_h, 0, wxALIGN_CENTER);
    //  Maximize(panel);
    
    sizer_v->Fit(panel);
    panel->SetSizer(sizer_v);
    
    //    SetSize(wxSize(100,100));
    
    //    CreateStatusBar();
    //    SetStatusText( "Welcome to Michele's text editor!" );
    
    //SetSizerAndFit(sizer_v);
    //Maximize();
    
    CentreOnScreen();
    
    
}

template<typename F_YES, typename F_NO> QuestionFrame<F_YES, F_NO>::QuestionFrame(wxWindow* parent, F_YES* f_yes_in, F_NO* f_no_in, const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent, wxID_ANY, title, pos, size){
    
    wxDisplay display;
    wxPNGHandler *handler;
    wxRect rectangle;
    //initialize the functor to close thie QuestionFrame when button_yes or button_no will be pressed
    CloseFrame<QuestionFrame>* close_frame;
    
    f_yes = f_yes_in;
    f_no = f_no_in;
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< QuestionFrame<F_YES, F_NO> >(this);
    
    //image
    handler = new wxPNGHandler;
    wxImage::AddHandler(handler);
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth())*1./1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight())*1./1000.0);
    
    //allocate sizers
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(3, 1, 0, 0);
    
    wxStaticText* text = new wxStaticText(panel, wxID_ANY, message, wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //buttons
    button_yes = new wxButton(panel, wxID_ANY, "Yes", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_yes->Bind(wxEVT_BUTTON, *f_yes);
    button_yes->Bind(wxEVT_BUTTON, *close_frame);
    button_no = new wxButton(panel, wxID_ANY, "No", wxDefaultPosition, GetTextExtent(wxS("00000000000")), wxBU_EXACTFIT);
    button_no->Bind(wxEVT_BUTTON, *f_no);
    button_no->Bind(wxEVT_BUTTON, *close_frame);
    
    
    image = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(path_file_app_icon, wxBITMAP_TYPE_PNG), wxDefaultPosition, wxDefaultSize);
    
    sizer_grid->Add(text, 0, wxALIGN_CENTER);
    sizer_grid->Add(image, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    sizer_buttons->Add(button_yes, 0, wxALIGN_CENTER);
    sizer_buttons->Add(button_no, 0, wxALIGN_CENTER);
    sizer_grid->Add(sizer_buttons, 0, wxALIGN_CENTER);
    
    sizer_h->Add(sizer_grid, 0, wxALIGN_CENTER_VERTICAL);
    sizer_v->Add(sizer_h, 0, wxALIGN_CENTER);
    //  Maximize(panel);
    
    sizer_v->Fit(panel);
    panel->SetSizer(sizer_v);
    
    //    SetSize(wxSize(100,100));
    
    //    CreateStatusBar();
    //    SetStatusText( "Welcome to Michele's text editor!" );
    
    //SetSizerAndFit(sizer_v);
    //Maximize();
    
    CentreOnScreen();
    
}

template<class T> PrintErrorMessage<T>::PrintErrorMessage(T* f_in){
    
    f = f_in;
    
}


ListFrame::ListFrame(const wxString& title, const wxString& message, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(NULL, wxID_ANY, title, pos, size){
    
    unsigned int i, total_column_width /*, margin_h = 10*/, margin_v = 5, red, green, blue;
    OnSelectInListControlSights* on_select_in_listcontrol_sights;
    OnSelectInListControlPositions* on_select_in_listcontrol_positions;
    OnSelectInListControlRoutes* on_select_in_listcontrol_routes;
    wxListItem column, item;
    String s;
    //pos_open denotes the positions, in the string s composed of the color '(i,j,k)', of '(', pos_comma_1 of the first ',', pos_comma_2 of the second ',', and pos_close of ')'.
    size_t pos_end;
    
    
    plot = new Plot(catalog, String(""));
    
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle_display = (display.GetClientArea());
    
    
    
    n_columns_listcontrol_sights = 12;
    n_columns_listcontrol_positions = 3;
    n_columns_listcontrol_routes = 8;
    
    file_init.set_name(String(path_file_init));
    
    
    
    //read color list from file_init
    file_init.open(String("in"), prefix);
    cout << prefix.value << YELLOW << "Reading color list from file " << file_init.name.value << " ...\n" << RESET;
    s.read_from_file(String("color list"), file_init, true, String(""));
    cout << prefix.value << YELLOW << "... done.\n" << RESET;
    file_init.close(prefix);
    
    //in file_init, each color is written as '(i,j,k) ', where i, j, k are the integers for the levels of red, green and blue. To cound the number of colors, I thus count the number of '(' in the string
    color_list.resize(count((s.value).begin(), (s.value).end(), '('));
    
    
    for(i=0; i<color_list.size(); i++){
        
        //        //find the positions of (, ) and , to single ot the integers defining the color
        //        pos_open = (s.value).find("(");
        //        pos_comma_1 = (s.value).find(",");
        //        pos_comma_2 = (s.value).find(",");
        //        pos_close = (s.value).find(")");
        //
        //        //extract these integers, create a wxColor and append it to color_list
        //
        
        //        int b=                  stod((s.value).substr(pos_comma_1, pos_comma_2 - pos_comma_1).c_str());
        //        int c=                    stod((s.value).substr(pos_comma_2, pos_close - pos_comma_2).c_str());
        
        //get rid of everything that comes before and at '(' at the beginnign of s
        pos_end = (s.value).find("(");
        s.set(String(""), String((s.value).substr(pos_end+1).c_str()), String(""));
        //look for the first ','
        
        pos_end = (s.value).find(",");
        
        //read red
        red = stoi(((s.value).substr(0, pos_end)).c_str());
        
        //get rid of the first ','
        s.set(String(""), String((s.value).substr(pos_end+1).c_str()), String(""));
        
        pos_end = (s.value).find(",");
        
        green = stoi((s.value).substr(0, pos_end).c_str());
        
        //get rid of the second ','
        s.set(String(""), String((s.value).substr(pos_end+1).c_str()), String(""));
        
        pos_end = (s.value).find(")");
        //get rid of '('
        blue = stoi((s.value).substr(0, pos_end+1).c_str());
        
        //write the color that I just read in color_list
        color_list[i] = wxColor(red, green, blue);
        
    }
    
    //no positions nor routes are highlighted when ListFrame is constructed
    highlighted_route = -1;
    highlighted_position = -1;
    
    
    on_select_in_listcontrol_sights = new OnSelectInListControlSights(this);
    on_select_in_listcontrol_positions = new OnSelectInListControlPositions(this);
    on_select_in_listcontrol_routes = new OnSelectInListControlRoutes(this);
    
    //initialize delete_sight, which defines the functor to delete the sight but not its related route (it is called when the user answers 'n' to QuestionFrame)
    delete_sight = new DeleteSight(this, Answer('n', String("")));
    //initialize delete_sight_and_related_route, which defines the functor to delete the sight and its related route (it is called when the user answers 'y' to QuestionFrame)
    delete_sight_and_related_route = new DeleteSight(this, Answer('y', String("")));
    
    //initialize delete_route, which defines the functor to delete the route but not its related sight (it is called when the user answers 'n' to QuestionFrame)
    delete_route = new DeleteRoute(this, Answer('n', String("")));
    //initialize delete_route_and_related_sight, which defines the functor to delete the route and its related sight (it is called when the user answers 'y' to QuestionFrame)
    delete_route_and_related_sight = new DeleteRoute(this, Answer('y', String("")));
    
    catalog = new Catalog(String(path_file_catalog), String(""));
    
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_buttons_sight = new wxBoxSizer(wxHORIZONTAL);
    sizer_buttons_position = new wxBoxSizer(wxHORIZONTAL);
    sizer_buttons_route = new wxBoxSizer(wxHORIZONTAL);
    
    sizer_box_sight = new wxStaticBoxSizer(wxVERTICAL, panel, "Sights");
    sizer_box_position = new wxStaticBoxSizer(wxVERTICAL, panel, "Positions");
    sizer_box_route = new wxStaticBoxSizer(wxVERTICAL, panel, "Routes");
    
    //
    //here I read a sample sight from file_sample_sight, store into sight and set all the fields in this to the data in sight with set()
    File file_sample_sight;
    file_sample_sight.set_name(String("/Users/macbookpro/Documents/navigational_astronomy/sight_reduction_program/sample_sight.txt"));
    plot->read_from_file(file_sample_sight, String(""));
    plot->print(true, String(""), cout);
    //
    
    
    
    //listcontrol_sights with sights
    listcontrol_sights = new ListControl(panel, wxDefaultPosition, wxSize((this->GetSize()).GetWidth()*0.95 ,  -1));
    listcontrol_sights->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_sights);
    listcontrol_sights->Bind(wxEVT_MOTION, wxMouseEventHandler(ListFrame::OnMouseOnListControlSights), this);
    
    i=0;
    
    listcontrol_sights->PushBackColumn(wxString("Body"));
    listcontrol_sights->PushBackColumn(wxString("Limb"));
    listcontrol_sights->PushBackColumn(wxString("Artificial horizon"));
    listcontrol_sights->PushBackColumn(wxString("Sextant altitude"));
    listcontrol_sights->PushBackColumn(wxString("Index error"));
    listcontrol_sights->PushBackColumn(wxString("Height of eye"));
    listcontrol_sights->PushBackColumn(wxString("Master-clock date and hour (UTC)"));
    listcontrol_sights->PushBackColumn(wxString("Stopwatch"));
    listcontrol_sights->PushBackColumn(wxString("Stopwatch reading"));
    listcontrol_sights->PushBackColumn(wxString("TAI - UTC"));
    listcontrol_sights->PushBackColumn(wxString("Label"));
    listcontrol_sights->PushBackColumn(wxString("Related route"));
    
    
    
    //write the sights into plot->sight_list into listcontrol_sights
    for(i=0; i<((plot->sight_list).size()); i++){
        ((plot->sight_list)[i]).add_to_wxListCtrl(-1, listcontrol_sights);
    }
    
    //    set the column width to the width of its longest item
    for(i=0; i<(listcontrol_sights->GetColumnCount()); i++){
        listcontrol_sights->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER );
    }
    for(total_column_width=0, i=0; i<(listcontrol_sights->GetColumnCount()); i++){
        total_column_width += (listcontrol_sights->GetColumnWidth(i));
    }
    
    listcontrol_sights->SetMinSize(wxSize(total_column_width,-1));
    
    sizer_box_sight->Add(listcontrol_sights, 0,  wxALL, margin_v);
    
    
    //listcontrol_positions with positions
    listcontrol_positions = new ListControl(panel, wxDefaultPosition, wxSize((this->GetSize()).GetWidth()*0.95 ,  -1));
    listcontrol_positions->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_positions);
    listcontrol_positions->Bind(wxEVT_MOTION, wxMouseEventHandler(ListFrame::OnMouseOnListControlPositions), this);
    
    listcontrol_positions->PushBackColumn(wxString("Latitude"));
    listcontrol_positions->PushBackColumn(wxString("Longitude"));
    listcontrol_positions->PushBackColumn(wxString("Label"));
    
    //write the positions into plot->position_list into listcontrol_sights
    for(i=0; i<((plot->position_list).size()); i++){
        ((plot->position_list)[i]).add_to_wxListCtrl(-1, listcontrol_positions);
    }
    
    //    set the column width to the width of its longest item
    for(i=0; i<(listcontrol_positions->GetColumnCount()); i++){
        listcontrol_positions->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }
    for(total_column_width=0, i=0; i<(listcontrol_positions->GetColumnCount()); i++){
        total_column_width += (listcontrol_positions->GetColumnWidth(i));
    }
    
    listcontrol_positions->SetMinSize(wxSize(total_column_width,-1));
    
    sizer_box_position->Add(listcontrol_positions, 0,  wxALL, margin_v);
    
    
    
    //listcontrol routes with routes
    listcontrol_routes = new ListControl(panel, wxDefaultPosition, wxSize((this->GetSize()).GetWidth()*0.95 ,  -1));
    listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_routes);
    listcontrol_routes->Bind(wxEVT_MOTION, wxMouseEventHandler(ListFrame::OnMouseOnListControlRoutes), this);
    
    listcontrol_routes->PushBackColumn(wxString("Type"));
    listcontrol_routes->PushBackColumn(wxString("Start"));
    listcontrol_routes->PushBackColumn(wxString("Alpha"));
    listcontrol_routes->PushBackColumn(wxString("Length"));
    listcontrol_routes->PushBackColumn(wxString("GroundPosition"));
    listcontrol_routes->PushBackColumn(wxString("Omega"));
    listcontrol_routes->PushBackColumn(wxString("Label"));
    listcontrol_routes->PushBackColumn(wxString("Related Sight"));
    
    //write the routes into plot->route_list into listcontrol_routes
    for(i=0; i<((plot->route_list).size()); i++){
        ((plot->route_list)[i]).add_to_wxListCtrl(-1, listcontrol_routes);
    }
    
    //    set the column width to the width of its longest item
    for(i=0; i<(listcontrol_routes->GetColumnCount()); i++){
        listcontrol_routes->SetColumnWidth(i, wxLIST_AUTOSIZE_USEHEADER);
    }
    for(total_column_width=0, i=0; i<(listcontrol_routes->GetColumnCount()); i++){
        total_column_width += (listcontrol_routes->GetColumnWidth(i));
    }
    
    listcontrol_routes->SetMinSize(wxSize(total_column_width,-1));
    
    sizer_box_route->Add(listcontrol_routes, 0,  wxALL, margin_v);
    //listcontrol routes with routes
    
    
    //buttons
    //image for buttons
    wxImage::AddHandler(new wxPNGHandler);
    wxBitmap my_bitmap = wxBitmap(wxT(path_file_pencil_icon), wxBITMAP_TYPE_PNG);
    wxImage my_image = my_bitmap.ConvertToImage();
    my_image.Rescale(20,20);
    
    //button to add a sight
    button_add_sight = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_add_sight->Bind(wxEVT_BUTTON, &ListFrame::OnAddSight, this);
    
    //button to add a position
    button_add_position = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_add_position->Bind(wxEVT_BUTTON, &ListFrame::OnAddPosition, this);
    
    //button to add a route
    button_add_route = new wxButton(panel, wxID_ANY, "+", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_add_route->Bind(wxEVT_BUTTON, &ListFrame::OnAddRoute, this);
    
    
    //button to modify a sight
    button_modify_sight = new wxBitmapButton(panel, wxID_ANY, wxBitmap(my_image), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT   | wxBORDER_NONE);
    button_modify_sight->Bind(wxEVT_BUTTON, &ListFrame::OnModifySight, this);
    button_modify_sight->Enable(false);
    
    //button to modify a position
    button_modify_position = new wxBitmapButton(panel, wxID_ANY, wxBitmap(my_image), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT   | wxBORDER_NONE);
    button_modify_position->Bind(wxEVT_BUTTON, &ListFrame::OnModifyPosition, this);
    button_modify_position->Enable(false);
    
    //button to modify a route
    button_modify_route = new wxBitmapButton(panel, wxID_ANY, wxBitmap(my_image), wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT   | wxBORDER_NONE);
    button_modify_route->Bind(wxEVT_BUTTON, &ListFrame::OnModifyRoute, this);
    button_modify_route->Enable(false);
    
    
    //button to delete a sight
    button_delete_sight = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_delete_sight->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteSight, this);
    button_delete_sight->Enable(false);
    
    //button to delete a position
    button_delete_position = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_delete_position->Bind(wxEVT_BUTTON, &ListFrame::OnDeletePosition, this);
    button_delete_position->Enable(false);
    
    //button to delete a route
    button_delete_route = new wxButton(panel, wxID_ANY, "-", wxDefaultPosition, wxSize(20,20), wxBU_EXACTFIT);
    button_delete_route->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteRoute, this);
    button_delete_route->Enable(false);
    
    
    sizer_buttons_sight->Add(button_add_sight, 0, wxALIGN_CENTER);
    sizer_buttons_sight->Add(button_modify_sight, 0, wxALIGN_CENTER);
    sizer_buttons_sight->Add(button_delete_sight, 0, wxALIGN_CENTER);
    sizer_box_sight->Add(sizer_buttons_sight, 0, wxALIGN_LEFT | wxALL, margin_v);
    
    sizer_buttons_position->Add(button_add_position, 0, wxALIGN_CENTER);
    sizer_buttons_position->Add(button_modify_position, 0, wxALIGN_CENTER);
    sizer_buttons_position->Add(button_delete_position, 0, wxALIGN_CENTER);
    sizer_box_position->Add(sizer_buttons_position, 0, wxALIGN_LEFT | wxALL, margin_v);
    
    sizer_buttons_route->Add(button_add_route, 0, wxALIGN_CENTER);
    sizer_buttons_route->Add(button_modify_route, 0, wxALIGN_CENTER);
    sizer_buttons_route->Add(button_delete_route, 0, wxALIGN_CENTER);
    sizer_box_route->Add(sizer_buttons_route, 0, wxALIGN_LEFT | wxALL, margin_v);
    
    
    //
    
    //resize uniformly all column
    //    for(i=0; i<(listcontrol_sights->GetColumnCount()); ++i){
    //        listcontrol_sights->SetColumnWidth(i, ((listcontrol_sights->GetSize()).GetWidth())/(listcontrol_sights->GetColumnCount()));
    //    }
    sizer_v->Add(sizer_box_sight, 0,  wxALL, margin_v);
    sizer_v->Add(sizer_box_position, 0,  wxALL, margin_v);
    sizer_v->Add(sizer_box_route, 0,  wxALL, margin_v);
    //    sizer_v->Add(button_modify_sight, 0,  wxALIGN_LEFT | wxALL, 5);
    //    sizer_v->Add(button_delete_sight, 0, wxALIGN_LEFT | wxALL, 5);
    //    sizer_h->Add(listcontrol_sights, 0, wxALIGN_TOP);
    
    Maximize(panel);
    SetSizerAndFit(sizer_v);
    //    panel->SetSizer(sizer_v);
    
    //    panel->SetSize(wxSize(total_column_width+4*margin_v,-1));
    //    this->SetSize(wxSize(total_column_width+6*margin_v,-1));
    //
}

//writes the ids of the related sights and route in the GUI fields in ListFrame -> this, by reading them from the non-GUI object Plot
void ListFrame::UpdateRelatedSightsAndRoutes(void){
    
    unsigned int i;
    
    for(i=0; i<(listcontrol_sights->GetItemCount()); i++){
        
        if(((((plot->sight_list)[i]).related_route).value) == -1){
            
            listcontrol_sights->SetItem(i, n_columns_listcontrol_sights-1, wxString(""));
            
        }else{
            
            listcontrol_sights->SetItem(
                                        i,
                                        n_columns_listcontrol_sights-1,
                                        wxString::Format(wxT("%i"),
                                                         ((((plot->sight_list)[i]).related_route).value) + 1
                                                         )
                                        );
            
        }
        
    }
    
    for(i=0; i<(listcontrol_routes->GetItemCount()); i++){
        
        if(((((plot->route_list)[i]).related_sight).value) == -1){
            
            
            listcontrol_routes->SetItem(i, n_columns_listcontrol_routes-1, wxString(""));
            
        }else{
            
            listcontrol_routes->SetItem(
                                        i,
                                        n_columns_listcontrol_routes-1,
                                        wxString::Format(wxT("%i"),
                                                         ((((plot->route_list)[i]).related_sight).value)+1
                                                         )
                                        );
            
        }
        
    }
    
    
}

void ListFrame::OnAddSight(wxCommandEvent& event){
    
    SightFrame *sight_frame = new SightFrame(this, NULL, -1, "New sight", wxDefaultPosition, wxDefaultSize, String(""));
    sight_frame->Show(true);
    
    event.Skip(true);
    
}

void ListFrame::OnAddPosition(wxCommandEvent& event){
    
    PositionFrame *position_frame = new PositionFrame(this, NULL, -1, "New position", wxDefaultPosition, wxDefaultSize, String(""));
    position_frame->Show(true);
    
    event.Skip(true);
    
}

void ListFrame::OnAddRoute(wxCommandEvent& event){
    
    RouteFrame *route_frame = new RouteFrame(this, NULL, -1, "New route", wxDefaultPosition, wxDefaultSize, String(""));
    route_frame->Show(true);
    
    event.Skip(true);
    
}

void ListFrame::OnModifySight(wxCommandEvent& event){
    
    long item;
    item = listcontrol_sights->GetNextItem(-1,
                                           wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);
    
    if(item != -1){
        
        stringstream s;
        
        s.str("");
        s << "Sight #" << item;
        
        SightFrame *sight_frame = new SightFrame(this, &((plot->sight_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        //        (sight_frame->sight) = &((plot->sight_list)[item]);
        sight_frame->Show(true);
        
        
    }
    
    
    event.Skip(true);
    
}

void ListFrame::OnModifyPosition(wxCommandEvent& event){
    
    long item;
    item = listcontrol_positions->GetNextItem(-1,
                                              wxLIST_NEXT_ALL,
                                              wxLIST_STATE_SELECTED);
    
    if(item != -1){
        
        stringstream s;
        
        s.str("");
        s << "Position #" << item;
        
        PositionFrame *position_frame = new PositionFrame(this, &((plot->position_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        position_frame->Show(true);
        
    }
    
    event.Skip(true);
    
}

void ListFrame::OnModifyRoute(wxCommandEvent& event){
    
    
    long item;
    item = listcontrol_routes->GetNextItem(-1,
                                           wxLIST_NEXT_ALL,
                                           wxLIST_STATE_SELECTED);
    
    if(item != -1){
        
        stringstream s;
        
        s.str("");
        s << "Route #" << item;
        
        RouteFrame *route_frame = new RouteFrame(this, &((plot->route_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        route_frame->Show(true);
        
    }
    
    
    event.Skip(true);
    
}



void ListFrame::OnPressDeleteSight(wxCommandEvent& event){
    
    int i_sight_to_remove = (listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
    
    //the id of the sight to removed is the one of the sight selected in listcontrol_sights: I write it in delete_sight_and_related_route and in delete_sight
    (delete_sight_and_related_route->i_sight_to_remove) = i_sight_to_remove;
    (delete_sight->i_sight_to_remove) = i_sight_to_remove;
    
    //remove the sight from the GUI object listcontrol_sights
    listcontrol_sights->DeleteItem((delete_sight->i_sight_to_remove));
    
    if( ((((plot->sight_list)[i_sight_to_remove]).related_route).value) != -1){
        //if the sight which I am about to remove is related to a route, I ask the user whether he wants to remove the related route too by showing  question_frame
        
        
        //remove the sight from the non-GUI object plot
        //ask the user whether he/she wants to remove the related route as well: if the answer is yes, then QuestionFrame calls the functor delete_sight_and_related_route. If no, it calls the functor delete_sight.
        QuestionFrame<DeleteSight, DeleteSight>* question_frame = new QuestionFrame<DeleteSight, DeleteSight>(NULL,
                                                                                                              delete_sight_and_related_route,
                                                                                                              delete_sight,
                                                                                                              "",
                                                                                                              "Do you want to remove the route related to this sight?",
                                                                                                              wxDefaultPosition,
                                                                                                              wxDefaultSize,
                                                                                                              String(""));
        question_frame->Show(true);
        
    }else{
        
        //if not, I simply delete teh sight
        
        (*delete_sight)(event);
        
    }
    
    event.Skip(true);
    
}

void ListFrame::OnDeletePosition(wxCommandEvent& event){
    
    long item;
    
    item = listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    listcontrol_positions->DeleteItem(item);
    plot->remove_position(item, String(""));
    
    
    event.Skip(true);
    
}

void ListFrame::OnPressDeleteRoute(wxCommandEvent& event){
    
    int i_route_to_remove = (listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
    
    //the id of the route to removed is the one of the route selected in listcontrol_routes: I write it in delete_route_and_related_route and in delete_route
    (delete_route_and_related_sight->i_route_to_remove) = i_route_to_remove;
    (delete_route->i_route_to_remove) = i_route_to_remove;
    
    //remove the route from the GUI object listcontrol_routes
    listcontrol_routes->DeleteItem((delete_route->i_route_to_remove));
    
    
    if( ((((plot->route_list)[i_route_to_remove]).related_sight).value) != -1){
        //if the route which I am about to remove is related to a sight, I ask the user whether he wants to remove the related sight too by showing  question_frame
        
        //remove the route from the non-GUI object plot
        //ask the user whether he/she wants to remove the related sight as well: if the answer is yes, then QuestionFrame calls the functor delete_route_and_related_sight. If no, it calls the functor delete_route.
        QuestionFrame<DeleteRoute, DeleteRoute>* question_frame = new QuestionFrame<DeleteRoute, DeleteRoute>(NULL,
                                                                                                              delete_route_and_related_sight,
                                                                                                              delete_route,
                                                                                                              "",
                                                                                                              "Do you want to remove the sight related to this route?",
                                                                                                              wxDefaultPosition,
                                                                                                              wxDefaultSize,
                                                                                                              String(""));
        
        question_frame->Show(true);
        
    }else{
        //if not, I simply delete teh route
        
        (*delete_route)(event);
        
    }
    
    
    
    event.Skip(true);
    
}


//draw the routes stored in this->plot->list_routes
void ListFrame::DrawRoutes(void){
    
    
    
}

//when the mouse hovers over a given element of listcontrol_sights, sets highlighted_route equal to the id of the route related to that sight, if any
void ListFrame::OnMouseOnListControlSights(wxMouseEvent& event){
    
    int i;
    
    MousePositionOnListControl(listcontrol_sights, &i);
    
    highlighted_route = ((((plot->sight_list)[i]).related_route).value);
    
    (chart_frame->draw_panel)->PaintNow();
    
    event.Skip(true);
    
}


//when the mouse hovers over a given element of listcontrol_routes, sets highlighted_route equal to the id of that route
void ListFrame::OnMouseOnListControlRoutes(wxMouseEvent& event){
    
    MousePositionOnListControl(listcontrol_routes, &highlighted_route);
    
    (chart_frame->draw_panel)->PaintNow();
    
    event.Skip(true);
    
}

//when the mouse hovers over a given element of listcontrol_positions sets highlighted_position equal to the id of that position
void ListFrame::OnMouseOnListControlPositions(wxMouseEvent& event){
    
    MousePositionOnListControl(listcontrol_positions, &highlighted_position);
    
    (chart_frame->draw_panel)->PaintNow();
    
    event.Skip(true);
    
}


//write into all the non-GUI objects the values of the GUI fields
template<class T> void SightFrame::get(T& event){
    
    body->get(event);
    limb->get(event);
    artificial_horizon_check->get(event);
    H_s->get(event);
    index_error->get(event);
    
    if(!((artificial_horizon_check->checkbox)->GetValue())){
        height_of_eye->get(event);
    }
    
    master_clock_date->get(event);
    master_clock_chrono->get(event);
    
    //while setting the non-GUI fields equal to the values in the GUI fields, I set the value of sight->time: I firs set time to master_clock_date_and_hour ...
    (sight->time) = (sight->master_clock_date_and_hour);
    
    stopwatch_check->get(event);
    
    //.. then I add to it sight->stopwatch, if any ....
    if((sight->use_stopwatch) == Answer('y', String(""))){
        (sight->time)+=(sight->stopwatch);
    }
    
    
    if(((stopwatch_check->checkbox)->GetValue())){
        stopwatch_reading->get(event);
    }
    
    TAI_minus_UTC->get(event);
    
    //... then I add to it TAI_minus_UTC, to convert it from the UTC to the TAI scale.
    (sight->time)+=(sight->TAI_minus_UTC);
    
    
    label->get(event);
    
    event.Skip(true);
    
}

//set all the GUI fields in this equal to those in the non-GUI object this->sight
void SightFrame::set(void){
    
    Time time_UTC;
    
    body->set();
    
    if(((body->name)->GetValue() == wxString("sun") || (body->name)->GetValue() == wxString("moon"))){
        //if  body is sun or moon, then I write the value in the non-GUI field Limb into the GUI LimbField
        
        limb->set();
        
    }else{
        //if  body is not sun or moon, then I set the limb GUI field to empty and disable it
        
        (limb->name)->SetValue("");
        limb->Enable(false);
    }
    
    
    artificial_horizon_check->set();
    H_s->set();
    index_error->set();
    
    if(!((artificial_horizon_check->checkbox)->GetValue())){
        height_of_eye->Enable(true);
        height_of_eye->set();
    }else{
        height_of_eye->Enable(false);
    }
    
    master_clock_date->set();
    
    //(sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    time_UTC = (sight->time);
    time_UTC -= (sight->TAI_minus_UTC);
    master_clock_chrono->set(time_UTC.chrono);
    
    stopwatch_check->set();
    
    if(((stopwatch_check->checkbox)->GetValue())){
        stopwatch_reading->Enable(true);
        stopwatch_reading->set(sight->stopwatch);
    }else{
        stopwatch_reading->Enable(false);
    }
    
    TAI_minus_UTC->set(sight->TAI_minus_UTC);
    label->set();
    
}

void SightFrame::SetIdling(bool b){
    
    idling = b;
    
}





//this function checks whether all the fields in SightFrame are ok, and if they are, it enables the button_reduce
void SightFrame::TryToEnableOk(void){
    
    
    button_reduce->Enable(
                          (body->is_ok()) &&
                          ((!( ((body->name->GetValue()) == wxString("sun")) || ((body->name->GetValue()) == wxString("moon")) )) || (limb->is_ok())) &&
                          (H_s->is_ok()) &&
                          (index_error->is_ok()) &&
                          ((((artificial_horizon_check->checkbox)->GetValue())) || (height_of_eye->is_ok())) &&
                          (master_clock_date->is_ok()) &&
                          (master_clock_chrono->is_ok()) &&
                          ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) &&
                          (TAI_minus_UTC->is_ok()));
    
}





void SightFrame::OnPressCancel(wxCommandEvent& event){
    
    //I am about to close the frame,  thus I set f->idling to true
    SetIdling(true);
    
    
    Close(TRUE);
}


CheckYear::CheckYear(DateField* p_in){
    
    p = p_in;
    (tabulate_days.p) = p;
    
}

CheckMonth::CheckMonth(DateField* p_in){
    
    p = p_in;
    (tabulate_days.p) = p;
    
    
}

CheckDay::CheckDay(DateField* p_in){
    
    p = p_in;
    
}


template<class T> void CheckYear::operator()(T&event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->year)->GetValue()).ToStdString(), NULL, false, 0, 0)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->year, String("Entered value is not valid!\nYear must be an unsigned integer"));
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->year);
            ((f->print_error_message)->title) = String("Entered value is not valid!");
            ((f->print_error_message)->message) = String("Year must be an unsigned integer");
            f->CallAfter(*(f->print_error_message));
            
            (p->year_ok) = false;
            (p->day)->Enable(false);
            
            
        }else{
            
            (p->year)->SetBackgroundColour(*wxWHITE);
            (p->year_ok) = true;
            
            if(p->month_ok){
                tabulate_days(event);
                (p->day)->Enable(true);
            }
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

template<class T> void CheckMonth::operator()(T&event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->month)->GetValue()).ToStdString(), NULL, true, 1, 12+1)){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->month, String("Entered value is not valid!\nMonth must be an unsigned integer >= 1 and <= 12"));
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->month);
            ((f->print_error_message)->title) = String("Entered value is not valid!");
            ((f->print_error_message)->message) = String("Month must be an unsigned integer >= 1 and <= 12");
            f->CallAfter(*(f->print_error_message));
            
            (p->month_ok) = false;
            (p->day)->Enable(false);
            
        }else{
            
            (p->month)->SetBackgroundColour(*wxWHITE);
            (p->month_ok) = true;
            
            if(p->year_ok){
                
                tabulate_days(event);
                (p->day)->Enable(true);
                
            }
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}


template<class T> void CheckDay::operator()(T& event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        //this variable = true if the day field is formatted correctly
        bool ok;
        
        
        //to check whether the p->day is formatted correctly, I first check whether p->year and p->month are formatted correctly, so I can extract a valid value of p->month. Then, I check whether p-> day is an unsigned int formatted correctly with check_unsigned_int, and whether this unsigned int lies in the correct interval relative to p->month
        if((p->year_ok) && (p->month_ok)){
            
            (p->date)->check_leap_year();
            
            if((p->date)->Y_is_leap_year){
                
                ok = check_unsigned_int(((p->day)->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi((p->month)->GetValue()))-1]+1);
                
            }else{
                
                ok = check_unsigned_int(((p->day)->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi((p->month)->GetValue()))-1]+1);
                
            }
            
            if(ok){
                
                (p->day)->Enable(true);
                (p->day)->SetBackgroundColour(*wxWHITE);
                (p->day_ok) = true;
                
            }else{
                
                //            f->CallAfter(&SightFrame::PrintErrorMessage, p->day, String("Entered value is not valid!\nDay must be an unsigned integer comprised between the days of the relative month"));
                
                //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
                ((f->print_error_message)->control) = (p->day);
                ((f->print_error_message)->title) = String("Entered value is not valid!");
                ((f->print_error_message)->message) = String("Day must be an unsigned integer comprised between the days of the relative month");
                f->CallAfter(*(f->print_error_message));
                
                (p->day)->Enable(true);
                (p->day_ok) = false;
                
            }
            
            
        }else{
            
            (p->day)->Enable(false);
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}






template<class T> void TabulateDays::operator()(T& event){
    
    unsigned int i;
    SightFrame* f = (p->parent_frame);
    
    if((p->year_ok) && (p->month_ok)){
        
        //read the year
        ((f->sight)->master_clock_date_and_hour).date.Y = ((unsigned int)wxAtoi((p->year)->GetValue()));
        ((f->sight)->master_clock_date_and_hour).date.check_leap_year();
        
        //read the month
        ((f->sight)->master_clock_date_and_hour).date.M = ((unsigned int)wxAtoi((p->month)->GetValue()));
        
        if(((f->sight)->master_clock_date_and_hour).date.Y_is_leap_year){
            //in this case the year is a leap year: I fill the list of days from days_per_month_leap
            
            for((p->days).Clear(), i=0; i<days_per_month_leap[(((f->sight)->master_clock_date_and_hour).date.M)-1]; i++){
                (p->days).Add(wxString::Format(wxT("%i"),i+1));
            }
            
        }else{
            //in this case the year is a common year: I fill the list of days from days_per_month_common
            
            for((p->days).Clear(), i=0; i<days_per_month_common[(((f->sight)->master_clock_date_and_hour).date.M)-1]; i++){
                (p->days).Add(wxString::Format(wxT("%i"),i+1));
            }
            //
        }
        
        (p->day)->Set((p->days));
        (p->day)->SetValue(wxString("1"));
        (p->day)->Enable(true);
        
    }else{
        
        (p->day)->Enable(false);
        
    }
    
    event.Skip(true);
    
}



//this function reads the value in the GUI box checkbox, and enables/disables the related_field accordingly
template<class T> template<class R> void CheckCheck<T>::operator()(R& event){
    
    //I enable/disable related_field according to whether checkbox is checked or not, and according to the value of direct_reverse
    if((((p->checkbox)->GetValue()) ^ (!(p->direct_reverse)))){
        (p->related_field)->Enable(true);
        //I write into the related_field by setting its variable just_enabled to true: this means that no error message will be prompted when the user sets its focus to the related field GUIs
        ((p->related_field)->just_enabled) = true;
    }else{
        (p->related_field)->Enable(false);
    }
    
    (p->parent_frame)->TryToEnableOk();
    
    event.Skip(true);
    
}


//this function writes into the non-GUI field answer the value entered in the GUI box
template<class T> template<class S> void CheckField<T>::get(S& event){
    
    //I set p->answer to the value entered in the GUI checkbox
    if(checkbox->GetValue()){
        (answer->value) = 'y';
    }else{
        (answer->value) = 'n';
    }
    
    event.Skip(true);
    
}

CheckHour::CheckHour(ChronoField* p_in){
    
    p = p_in;
    
}

template<class T> void CheckHour::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24) && ((p->hour)->IsEnabled())){
            
            if(!(p->just_enabled)){
                //if the content of the GUI field p is invalid and p has not been just enabled, then I am authorized to prompt an error message
                
                //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
                ((f->print_error_message)->control) = (p->hour);
                ((f->print_error_message)->title) = String("Entered value is not valid!");
                ((f->print_error_message)->message) = String("Hours must be unsigned integer numbers >= 0 and < 24");
                f->CallAfter(*(f->print_error_message));
                
            }else{
                //if the ChronoField p has just been enabled, I do not print any error message even if the content of p is invalid: this is because I want to give the opportunity to the user to enter the content of the GUI field before complaining that the content of the GUI field is invalid. However, I set just_enabled to false, because p is no longer just enabled.
                
                
                (p->just_enabled) = false;
                
                
            }
            
            
            (p->hour_ok) = false;
            
        }else{
            
            (p->hour)->SetBackgroundColour(*wxWHITE);
            (p->hour_ok) = true;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

CheckMinute::CheckMinute(ChronoField* p_in){
    
    p = p_in;
    
}

template<class T> void CheckMinute::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60) && ((p->minute)->IsEnabled())){
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, (p->minute), String("Entered value is not valid!\nMinutes must be unsigned integer numbers >= 0 and < 60"));
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->minute);
            ((f->print_error_message)->title) = String("Entered value is not valid!");
            ((f->print_error_message)->message) = String("Minutes must be unsigned integer numbers >= 0 and < 60");
            f->CallAfter(*(f->print_error_message));
            
            (p->minute_ok) = false;
            
        }else{
            
            (p->minute)->SetBackgroundColour(*wxWHITE);
            (p->minute_ok) = true;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}

CheckSecond::CheckSecond(ChronoField* p_in){
    
    p = p_in;
    
}

template<class T> void CheckSecond::operator()(T &event){
    
    SightFrame* f = (p->parent_frame);
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        if(!check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0) && ((p->second)->IsEnabled())){
            
            
            //        f->CallAfter(&SightFrame::PrintErrorMessage, p->second, String("Entered value is not valid!\nSeconds must be floating-point numbers >= 0.0 and < 60.0"));
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->second);
            ((f->print_error_message)->title) = String("Entered value is not valid!");
            ((f->print_error_message)->message) = String("Seconds must be floating-point numbers >= 0.0 and < 60.0");
            f->CallAfter(*(f->print_error_message));
            
            (p->second_ok) = false;
            
        }else{
            
            
            (p->second)->SetBackgroundColour(*wxWHITE);
            (p->second_ok) = true;
            
        }
        
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}


CheckChrono::CheckChrono(ChronoField* p_in){
    
    p = p_in;
    
    check_hour = new CheckHour(p);
    check_minute = new CheckMinute(p);
    check_second = new CheckSecond(p);
    
}

//this function writes into sight.stopwatch the value written into the respective GUI box
template <class T> void CheckChrono::operator()(T& event){
    
    check_hour(event);
    check_minute(event);
    check_second(event);
    
    event.Skip(true);
    
}

CheckRouteType::CheckRouteType(RouteTypeField* p_in){
    
    p = p_in;
    
    
}

//this functor checks the wxComboBox containing the Route type, and if it is equal to loxodrome or orthodrome, it enables only  the length, alpha and start fields in RouteFrame. If it is equal to circle of equal altitude, it enables only the GP and omege fields.
template<class T>void CheckRouteType::operator()(T& event){
    
    RouteFrame* f = (p->parent_frame);
    bool enable;
    
    //I proceed only if the progam is not is indling mode
    if(!(f->idling)){
        
        unsigned int i;
        bool check;
        
        //I check whether the name in the GUI field body matches one of the body names in catalog
        for(check = false, i=0; (i<((p->types).size())) && (!check); i++){
            if(((p->name)->GetValue()) == ((p->types)[i])){
                check = true;
            }
        }
        i--;
        
        if(check){
            
            
            enable = ((((p->types)[i]) == wxString("loxodrome")) || (((p->types)[i]) == wxString("orthodrome")));
            
            
            (f->alpha)->Enable(enable);
            (f->start_phi)->Enable(enable);
            (f->start_lambda)->Enable(enable);
            (f->l)->Enable(enable);
            
            (f->GP_phi)->Enable(!enable);
            (f->GP_lambda)->Enable(!enable);
            (f->omega)->Enable(!enable);
            
            
            (p->name)->SetBackgroundColour(*wxWHITE);
            (p->ok) = true;
            
        }else{
            
            //set the wxControl, title and message for the functor print_error_message, and then call the functor with CallAfter
            ((f->print_error_message)->control) = (p->name);
            ((f->print_error_message)->title) = String("Route type not found in list!");
            ((f->print_error_message)->message) = String("Route type must be loxodrome, orthodrome, or circle of equal altitude.");
            f->CallAfter(*(f->print_error_message));
            
            (p->ok) = false;
            
        }
        
        f->TryToEnableOk();
        
    }
    
    event.Skip(true);
    
}


//this function writes into the non-GUI fields in chrono the value written into the respective GUI fields hour, minute and second
template <class T> void ChronoField::get(T& event){
    
    if(hour_ok && (hour->IsEnabled()) && minute_ok && (minute->IsEnabled()) && second_ok && (second->IsEnabled())){
        //I write only if hour, minute and second are ok and enabled
        
        double s_temp;
        
        (chrono->h) = ((unsigned int)wxAtoi(hour->GetValue()));
        ((chrono)->m) = ((unsigned int)wxAtoi(minute->GetValue()));
        ((second)->GetValue()).ToDouble(&s_temp);
        ((chrono)->s) = s_temp;
        
    }
    
    event.Skip(true);
    
}


//writes to the non-GUI field angle the values written in the GUI field name
template <class T> void RouteTypeField::get(T &event){
    
    
    if(ok){
        
        if(String((name->GetValue()).ToStdString()) == String("loxodrome")){
            
            type->set(String(""), String("l"), String(""));
            
        }
        if(String((name->GetValue()).ToStdString()) == String("orthodrome")){
            type->set(String(""), String("o"), String(""));
            
            
        }
        if(String((name->GetValue()).ToStdString()) == String("circle of equal altitude")){
            
            type->set(String(""), String("c"), String(""));
            
        }
        
    }
    
    event.Skip(true);
    
}


void SightFrame::OnPressReduce(wxCommandEvent& event){
    
    stringstream s;
    
    //writes the values of the GUI fields in the non-GUI fields
    get(event);
    
    sight->print(String("sight entered via GUI"), String(""), cout);
    
    
    if(list_position==-1){
        //if the constructor of SightFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of sight_list and reduce it
        
        ((this->parent)->plot)->add_sight_and_reduce(sight, String(""));
        
    }else{
        //if the constructor of SightFrame has been called with sight_in != NULL, then I am modifying an existing sight, and I reduce it and write the result in the related route, which already exists
        
        sight->reduce(&((((this->parent)->plot)->route_list)[(sight->related_route).value]), String(""));
        
    }
    
    parent->UpdateRelatedSightsAndRoutes();
    
    sight->add_to_wxListCtrl(list_position, ((this->parent)->listcontrol_sights));
    
    parent->plot->print(true, String(""), cout);
    
    //I call PaintNow() because the positions have changed, so I need to re-draw the chart
    ((parent->chart_frame)->draw_panel)->Draw();
    ((parent->chart_frame)->draw_panel)->PaintNow();
    
    event.Skip(true);
    
    Close(TRUE);
    
}


//constructor of a BodyField object, based on the parent frame frame
BodyField::BodyField(SightFrame* frame, Body* p, Catalog* c){
    
    unsigned int i;
    parent_frame = frame;
    //I link the internal pointers p and c to the respective body and body catalog
    body = p;
    catalog = c;
    
    for(bodies.Clear(), i=0; i<(catalog->list).size(); i++){
        bodies.Add(((catalog->list)[i]).name.value.c_str());
    }
    
    check = new CheckBody(this);
    
    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, bodies, wxCB_DROPDOWN);
    name->SetValue("");
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, *check);
    
    ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);
    
}


//sets the value in the GUI object name equal to the value in the non-GUI Body object body
void BodyField::set(void){
    
    name->SetValue((body->name).value);
    ok = true;
    
}

//sets the value in the non-GUI object body equal to the value in the GUI  object name
template<class T> void BodyField::get(T& event){
    
    unsigned int i;
    bool check;
    
    if(ok){
        //If the GUI field's content is ok...
        
        //I find the position of the content of the GUI field in the list of  the body names in catalog
        for(check=false, i=0; (i<(catalog->list).size()) && (!check); i++){
            if(String((name->GetValue().ToStdString())) == (((catalog->list)[i]).name)){
                check = true;
            }
        }
        i--;
        
        //I set the value of the non-GUI object body to the value obtained from the GUI object.
        (*body) = (catalog->list)[i];
        
    }
    
    event.Skip(true);
    
}

//sets the value in the GUI object name equal to the value in the non-GUI limb object limb
void LimbField::set(void){
    
    if((limb->value) == 'u'){
        name->SetValue("upper");
    }
    if((limb->value) == 'l'){
        name->SetValue("lower");
    }
    if((limb->value) == 'c'){
        name->SetValue("center");
    }
    
    ok = true;
    
}

//this function enables/disable the LimbField
void LimbField::Enable(bool is_enabled){
    
    name->Enable(is_enabled);
    
}

//sets the value in the GUI object check equal to the value in the non-GUI limb object answer
template<class T> void CheckField<T>::set(void){
    
    if((answer->value) == 'y'){
        checkbox->SetValue(true);
    }
    
    if((answer->value) == 'n'){
        checkbox->SetValue(false);
    }
    
}

//sets the value in the GUI objects deg and min equal to the value in the non-GUI limb object angle
template <class P> void AngleField<P>::set(void){
    
    unsigned int deg_temp;
    double min_temp;
    Angle angle_temp;
    
    
    if(format == String("")){
        
        angle->to_deg_min(&deg_temp, &min_temp);
        
    }else{
        //in this case format = +-, EW or NS
        
        if((angle->value) < M_PI){
            
            if(format==String("+-")){sign->SetValue(wxString("+"));}
            if(format==String("EW")){sign->SetValue(wxString("W"));}
            if(format==String("NS")){sign->SetValue(wxString("N"));}
            
            angle->to_deg_min(&deg_temp, &min_temp);
            
        }else{
            
            if(format==String("+-")){sign->SetValue(wxString("-"));}
            if(format==String("EW")){sign->SetValue(wxString("E"));}
            if(format==String("NS")){sign->SetValue(wxString("S"));}
            
            (angle_temp.value) = 2.0*M_PI - (angle->value);
            angle_temp.to_deg_min(&deg_temp, &min_temp);
            
        }
        
    }
    
    //all the cases above must share these lines, so I put them here
    deg->SetValue(wxString::Format(wxT("%i"), deg_temp));
    min->SetValue(wxString::Format(wxT("%f"), min_temp));
    
    sign_ok = true;
    deg_ok = true;
    min_ok = true;
    
}


//sets the value in the GUI object value equal to the value in the non-GUI  object length
template<class P> void LengthField<P>::set(void){
    
    if(unit == String("nm")){
        
        value->SetValue(wxString::Format(wxT("%f"), (length->value)));
        box_unit->SetValue(wxString("nm"));
        
    }else{
        
        value->SetValue(wxString::Format(wxT("%f"), /*I convert the lenght from nm to meters*/(length->value)*1e3*nm));
        box_unit->SetValue(wxString("m"));
        
    }
    
    value_ok = true;
    box_unit_ok = true;
    
}

//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date
void DateField::set(void){
    
    Time time_UTC;
    
    //((parent_frame->sight)->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    time_UTC = ((parent_frame->sight)->time);
    time_UTC -= ((parent_frame->sight)->TAI_minus_UTC);
    
    year->SetValue(wxString::Format(wxT("%i"), (time_UTC.date).Y));
    month->SetValue(wxString::Format(wxT("%i"), (time_UTC.date).M));
    day->SetValue(wxString::Format(wxT("%i"), (time_UTC.date).D));
    
    year_ok = true;
    month_ok = true;
    day_ok = true;
    
}

//sets the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object chrono_in
void ChronoField::set(Chrono chrono_in){
    
    hour->SetValue(wxString::Format(wxT("%i"), chrono_in.h));
    minute->SetValue(wxString::Format(wxT("%i"), chrono_in.m));
    second->SetValue(wxString::Format(wxT("%f"), chrono_in.s));
    
    hour_ok = true;
    minute_ok = true;
    second_ok = true;
    
}

//sets the value in the GUI object equal to the value in the non-GUI  object string
void RouteTypeField::set(void){
    
    if((*type) == String("l")){
        name->SetValue(wxString("loxodrome"));
    }
    if((*type) == String("o")){
        name->SetValue(wxString("orthodrome"));
    }
    if((*type) == String("c")){
        name->SetValue(wxString("circle of equal altitude"));
    }
    
    
    ok = true;
    
}

//sets the value in the GUI object value equal to the value in the non-GUI String object string
template<class P> void StringField<P>::set(void){
    
    value->SetValue(wxString(string->value));
    
}



//constructor of a LimbField object, based on the parent frame frame
LimbField::LimbField(SightFrame* frame, Limb* p){
    
    parent_frame = frame;
    //I link the internal pointers p the respective Limb object
    limb = p;
    
    //initialize check
    (check.p) = this;
    
    limbs.Clear();
    limbs.Add(wxT("upper"));
    limbs.Add(wxT("lower"));
    limbs.Add(wxT("center"));
    
    
    
    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, limbs, wxCB_DROPDOWN);
    //name->SetInitialSize(name->GetSizeFromTextSize(name->GetTextExtent(wxS("000"))));
    //name->SetValue("");
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, check);
    
    //    body->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);
    
    
    name->SetValue(wxString(""));
    ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);
    
}



//constructor of a CheckField object, based on the parent frame frame
template<class T> CheckField<T>::CheckField(SightFrame* frame, Answer* p, T* related_field_in, bool direct_reverse_in){
    
    parent_frame = frame;
    //I link the internal pointers p and c to the respective Answer object
    answer = p;
    related_field = related_field_in;
    direct_reverse = direct_reverse_in;
    
    (check.p) = this;
    
    checkbox = new wxCheckBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    checkbox->Bind(wxEVT_CHECKBOX, check);
    
    checkbox->SetValue(false);
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(checkbox, 0, wxALIGN_CENTER);
    
}



//constructor of an AngleField object, based on the parent frame frame
template <class P> AngleField<P>::AngleField(P* parent_in, Angle* p, String format_in){
    
    unsigned int i;
    parent_frame = parent_in;
    angle = p;
    
    format = format_in;
    
    signs.Clear();
    degrees.Clear();
    if(format == String("+-")){
        //tabulate signs
        signs.Add(wxString("+"));
        signs.Add(wxString("-"));
    }
    if(format == String("NS")){
        signs.Add(wxString("N"));
        signs.Add(wxString("S"));
    }
    if(format == String("EW")){
        //here W <-> + sign, E <-> - sign
        signs.Add(wxString("E"));
        signs.Add(wxString("W"));
    }
    
    //tabuate degrees
    if((format == String(""))){
        for(i=0; i<360; i++){
            degrees.Add(wxString::Format(wxT("%i"), i));
        }
    }else{
        if(format == String("NS")){
            for(i=0; i<=90; i++){
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }else{
            for(i=0; i<=180; i++){
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }
    }
    
    
    
    //initialize check and its objects
    check_angle = new CheckAngle<P>(this);
    
    //here the allocation of sign is inserted in the code in such a way that if format = "+-" the sign is allocated before deg, text_deg, min, text_min: In this way, when the user tabs through the fields in PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if(format == String("+-")){
        sign = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
        AdjustWidth(sign);
        sign->Bind(wxEVT_KILL_FOCUS, (check_angle->check_sign));
    }
    
    deg = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    AdjustWidth(deg);
    deg->Bind(wxEVT_KILL_FOCUS, (check_angle->check_arc_degree));
    
    text_deg = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("° "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    min = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS(sample_width_floating_point_field))));
    min->Bind(wxEVT_KILL_FOCUS, (check_angle->check_arc_minute));
    
    text_min = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("' "), wxDefaultPosition, wxDefaultSize, 0, wxT(""));
    
    //here the allocation of sign is inserted in the code in such a way that if format = "NS" || "EW" the sign is allocated after deg, text_deg, min, text_min: In this way, when the user tabs through the fields in the PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if((format == String("NS")) || (format == String("EW"))){
        sign = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN);
        AdjustWidth(sign);
        sign->Bind(wxEVT_KILL_FOCUS, (check_angle->check_sign));
    }
    
    if(format != String("")){sign->SetValue(wxString(""));}
    deg->SetValue(wxString(""));
    min->SetValue(wxString(""));
    sign_ok = false;
    deg_ok = false;
    min_ok = false;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    if(format != String("")){
        
        if(format == String("+-")){
            
            //in this case I display the sign before the numerical value of the angle
            sizer_h->Add(sign, 0, wxALIGN_LEFT);
            
            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);
            
        }else{
            
            //in this case I display the sign after the numerical value of the angle
            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);
            
            sizer_h->Add(sign, 0, wxALIGN_LEFT);
            
        }
    }else{
        
        //in this case I display only the numerical value of the angle
        sizer_h->Add(deg, 0, wxALIGN_CENTER);
        sizer_h->Add(text_deg);
        sizer_h->Add(min, 0, wxALIGN_CENTER);
        sizer_h->Add(text_min);
        
    }
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
}

//constructor of a LengthField object, based on the parent frame frame
template<class P> LengthField<P>::LengthField(P* frame, Length* p, String unit_in){
    
    parent_frame = frame;
    length = p;
    unit = unit_in;
    
    //    ((parent_frame->check_height_of_eye).p) = this;
    
    //initialize check
    check = new CheckLength<P>(this);
    
    //tabulate the possible units of measure
    units.Clear();
    units.Add(wxT("nm"));
    units.Add(wxT("m"));
    
    
    
    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->Bind(wxEVT_KILL_FOCUS, check->check_length_value);
    
    box_unit = new wxComboBox((parent_frame->panel), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, units, wxCB_DROPDOWN);
    AdjustWidth(box_unit);
    box_unit->Bind(wxEVT_KILL_FOCUS, check->check_length_unit);
    
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Length object
    value->SetValue(wxString(""));
    value_ok = false;
    //I set the value of box_unit to the unit of measure with with this LengthField was called in its constructor, and set its value to ok because that is a valid unit of measure
    box_unit->SetValue(unit.value);
    box_unit_ok = true;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);
    sizer_h->Add(box_unit, 0, wxALIGN_CENTER);
    
}


//constructor of a StringField object, based on the parent frame frame
template<class P> StringField<P>::StringField(P* parent_in, String* p){
    
    parent_frame = parent_in;
    string = p;
    
    //initialize check
    (check.p) = this;
    
    (set_string_to_current_time.p) = this;
    
    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_string_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, check);
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);
    
}



//checks whether the contents of the GUI fiels in AngleField are valid
template<class P> bool AngleField<P>::is_ok(void){
    
    bool output;
    
    if(format == String("")){
        //if the angle is in format "", the sign field is not used, so there is no need to check whether it is ok
        
        output = (deg_ok && min_ok);
        
    }else{
        //if the angle is in format "NS", "EW" or "+-" the sign field is  used, so I check that it is ok
        
        output = (sign_ok && deg_ok && min_ok);
        
        
    }
    
    return(output);
    
}

template<class P> bool LengthField<P>::is_ok(void){
    
    return(value_ok && box_unit_ok);
    
}

bool ChronoField::is_ok(void){
    
    return(hour_ok && minute_ok && second_ok);
    
}

//constructor of a DateField object, based on the parent frame frame
DateField::DateField(SightFrame* frame, Date* p){
    
    unsigned int i;
    parent_frame = frame;
    date = p;
    
    
    //initialize check and its objects
    check = new CheckDate(this);
    //    (check.p) = this;
    //    ((check->check_year)->p) = this;
    //    (((check->check_year)->tabulate_days).p) = this;
    //    ((check->check_month)->p) = this;
    //    (((check->check_month)->tabulate_days).p) = this;
    //    ((check->check_day)->p) = this;
    
    for(months.Clear(), months.Add(wxT("")), i=0; i<12; i++){
        months.Add(wxString::Format(wxT("%i"), i+1));
    }
    
    year = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    year->SetInitialSize(year->GetSizeFromTextSize(year->GetTextExtent(wxS("0000"))));
    year->Bind(wxEVT_KILL_FOCUS, *(check->check_year));
    
    text_hyphen_1 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
    
    month = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN);
    //    month->SetInitialSize(month->GetSizeFromTextSize(month->GetTextExtent(wxS("00"))));
    AdjustWidth(month);
    month->Bind(wxEVT_KILL_FOCUS, *(check->check_month));
    
    text_hyphen_2 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize);
    
    
    day = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN);
    //I create a temporary days list to set the size of the wxComboBox day with AdjustWidth, and then destroy this temporary days list
    for(days.Clear(), i=0; i<31; i++){
        days.Add(wxString::Format(wxT("%i"), i+1));
    }
    day->Set(days);
    AdjustWidth(day);
    days.Clear();
    day->Bind(wxEVT_KILL_FOCUS, *(check->check_day));
    
    
    year->SetValue(wxString(""));
    year_ok = false;
    
    month->SetValue(wxString(""));
    month_ok = false;
    
    day->SetValue(wxString(""));
    day_ok = false;
    
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
ChronoField::ChronoField(SightFrame* frame, Chrono* p){
    
    unsigned int i;
    parent_frame = frame;
    chrono = p;
    
    check = new CheckChrono(this);
    //    (check.p) = this;
    
    for(hours.Clear(), hours.Add(wxT("")), i=0; i<24; i++){
        hours.Add(wxString::Format(wxT("%i"), i+1));
    }
    for(minutes.Clear(), minutes.Add(wxT("")), i=0; i<60; i++){
        minutes.Add(wxString::Format(wxT("%i"), i+1));
    }
    
    hour = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN);
    //    hour->SetInitialSize(hour->GetSizeFromTextSize(hour ->GetTextExtent(wxS("00"))));
    AdjustWidth(hour);
    hour->Bind(wxEVT_KILL_FOCUS, *(check->check_hour));
    
    text_colon_1 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize);
    
    minute = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN);
    AdjustWidth(minute);
    //    minute->SetInitialSize(minute->GetSizeFromTextSize(minute->GetTextExtent(wxS("00"))));
    minute->Bind(wxEVT_KILL_FOCUS, *(check->check_minute));
    
    text_colon_2 = new wxStaticText((parent_frame->panel), wxID_ANY, wxT(":"), wxDefaultPosition, wxDefaultSize);
    
    second = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxCB_DROPDOWN);
    second->SetInitialSize(second->GetSizeFromTextSize(second->GetTextExtent(wxS(sample_width_floating_point_field))));
    second->Bind(wxEVT_KILL_FOCUS, *(check->check_second));
    
    
    hour->SetValue(wxString(""));
    hour_ok = false;
    
    minute->SetValue(wxString(""));
    minute_ok = false;
    
    second->SetValue(wxString(""));
    second_ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(hour, 0, wxALIGN_CENTER);
    sizer_h->Add(text_colon_1, wxALIGN_CENTER);
    sizer_h->Add(minute, 0, wxALIGN_CENTER);
    sizer_h->Add(text_colon_2, wxALIGN_CENTER);
    sizer_h->Add(second, 0, wxALIGN_CENTER);
    
}

//constructor of a RouteTypeField object, based on the parent frame frame
RouteTypeField::RouteTypeField(RouteFrame* frame, String* s){
    
    parent_frame = frame;
    //I link the internal pointers p and c to the respective non-GUI object string
    type = s;
    
    types.Clear();
    //tabulate types of Routes
    types.Add(wxString("loxodrome"));
    types.Add(wxString("orthodrome"));
    types.Add(wxString("circle of equal altitude"));
    
    check = new CheckRouteType(this);
    
    name = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, types, wxCB_DROPDOWN);
    AdjustWidth(name);
    name->Bind(wxEVT_KILL_FOCUS, *check);
    
    name->SetValue("");
    ok = false;
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);
    
}




//this functor quits the MessageFrame when Ok button is pressed
void MessageFrame::OnPressOk(wxCommandEvent& event){
    
    Close(TRUE);
    
}

////this is called when the yes button is pressed in QuestionFrame
//template<typename F> void QuestionFrame<F>::OnPressYes(wxCommandEvent& event){
//
//    //set the answer variable to yes
//    answer->set(String("answer set to "), 'y', String("//////////////// "));
//
//    //calls the functor which is supposed to be called when button_yes is pressed
//    (*f_yes)(event);
//
//
//    event.Skip(true);
//
//    Close(TRUE);
//
//}
//
////this is called when the button No is pressed in QuestionFrame
//template<typename F> void QuestionFrame<F>::OnPressNo(wxCommandEvent& event){
//
////    answer->set(String(""), 'n', String(""));
//
//    Close(TRUE);
//
//}

//this function enables/disable all the fields in AngleField
template<class P> void AngleField<P>::Enable(bool is_enabled){
    
    if(format != String("")){
        sign->Enable(is_enabled);
    }
    deg->Enable(is_enabled);
    min->Enable(is_enabled);
    
}

//this function enables/disable the LengthField
template<class P> void LengthField<P>::Enable(bool is_enabled){
    
    value->Enable(is_enabled);
    box_unit->Enable(is_enabled);
    
}

//this function enables/disable the whole ChronoField
void ChronoField::Enable(bool is_enabled){
    
    hour->Enable(is_enabled);
    minute->Enable(is_enabled);
    second->Enable(is_enabled);
    
}

bool BodyField::is_ok(void){
    
    return(ok);
    
}

bool LimbField::is_ok(void){
    
    return(ok);
    
}

bool DateField::is_ok(void){
    
    return(year_ok && month_ok && day_ok);
    
}

bool RouteTypeField::is_ok(void){
    
    return(ok);
    
}
template<class T> void BodyField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void LimbField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> template<class R> void CheckField<T>::InsertIn(R* host){
    
    host->Add(sizer_v);
    
}


template<class P> template<class T> void AngleField<P>::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class P> template<class T> void LengthField<P>::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}


template<class T> void DateField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class P> template<class T> void StringField<P>::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void ChronoField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}

template<class T> void RouteTypeField::InsertIn(T* host){
    
    host->Add(sizer_v);
    
}


OnSelectInListControlSights::OnSelectInListControlSights(ListFrame* f_in){
    
    f = f_in;
    
}

OnSelectInListControlPositions::OnSelectInListControlPositions(ListFrame* f_in){
    
    f = f_in;
    
}

OnSelectInListControlRoutes::OnSelectInListControlRoutes(ListFrame* f_in){
    
    f = f_in;
    
}

ListControl::ListControl(wxWindow* parent_in, const wxPoint& pos, const wxSize& size) : wxListCtrl(parent_in, wxID_ANY, pos, size, wxLC_REPORT){
    
    
}

//this pushes back a column to ListControl
void ListControl::PushBackColumn(wxString name){
    
    wxListItem column;
    
    column.SetId(GetColumnCount());
    column.SetText(name);
    column.SetAlign(wxLIST_FORMAT_LEFT);
    column.SetWidth(((this->GetSize()).GetWidth())/((this->GetColumnCount())+1));
    InsertColumn(GetColumnCount(), column);
    
}
