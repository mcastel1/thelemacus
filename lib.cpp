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
