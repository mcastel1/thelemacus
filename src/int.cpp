//
//  int.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "gsl_math.h"

#include "int.h"
#include "generic.h"



//set value equal to i
//inline 
void Int::set(String name, int i, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(i);

    if (name != String("")) { print(name, prefix, cout); }

}


//set the content (not the memory adresses) of *this equal to the content of x
void Int::set(const Int& x){
    
    value = (x.value);
    
}


//same as Int::set(String name, int i, [[maybe_unused]] String prefix) {, but without printing out anything
//inline 
void Int::set(int i) {

    value = i;

}


//round up *this in such a way that when it is prnted out, only precision.value signifiant digits are shown
void Int::my_round(Int precision){
    
    unsigned int n_digits, rounding_coefficient;
    
    //compute the number of digits of *this
    n_digits = round(log10(value)) + 1;
    
    //round up *this in such a way that only precision digits will be displayed in *this
    rounding_coefficient = gsl_pow_int(10.0, n_digits - precision.value);
    
    value = round((((double)value)/rounding_coefficient)) * rounding_coefficient;
    
}


//convert *this to String by adding a space every three digits to improve readability and return the result
String Int::to_string_spaces(void){
    
    unsigned int i;
    stringstream s;
    String output;
    
    s.str("");
    s << value;
    
    output = String("");
    
    for(i=0; i<s.str().length(); i++){
        
        
        output.appendto(String(s.str()[i]));

        if(((s.str().length()-1 - i) > 0) && (((s.str().length()-1 - i) % 3) == 0)){
            
            output.appendto(String(" "));
            
        }
        
    }
    
    return output;
    
}


bool Int::operator == (const Int& i) {

    return (value == (i.value));

}

bool Int::operator != (const Int& i) {

    return (!((*this) == i));

}

bool Int::operator == (const int& i) {

    return (value == i);

}

bool Int::operator != (const int& i) {

    return (!((*this) == i));

}

bool Int::operator > (const Int& i) {

    return(value > (i.value));

}

bool Int::operator > (const int& i) {

    return(value > i);

}



//reads an Int from File file, which must be already open, and it search the file from the beginning if search_entire_stream = true, does not search the file from the beginning otherwise. Writes the result in *this
template<class S> void Int::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));


    }
    else {

        line.clear();
        getline((*input_stream), line);

    }


    pos = line.find(" = ");

    //read the string after ' = ' until the end of line string and store it into value
    value = stoi(line.substr(pos + 3, line.size() - (pos + 3)).c_str(), NULL);

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);


}

template void Int::read_from_stream<std::__1::basic_fstream<char, std::__1::char_traits<char>>>(String, std::__1::basic_fstream<char, std::__1::char_traits<char>>*, bool, String);


//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Int::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Int>(this, name, filename, mode, prefix);

}


void Int::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}
