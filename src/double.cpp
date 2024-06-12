//
//  double.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "double.h"
#include "constants.h"
#include "generic.h"
#include <cstdint>


//void constructor
inline Double::Double(void){}

//constructor that sets value to  x
inline Double::Double(const double &x){
    
    value = x;
    
}


void Double::set(double x) {

    value = x;

}


void Double::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(x);

    if (name != String("")) { print(name, prefix, cout); }

}


//returns true if *this is equal to y within the numerical accuracy accuracy_equal_approx
bool Double::equal_approx(Double x) {

    bool check;
    unsigned int i;
    uint64_t value_uint64_t, xvalue_uint64_t;


    memcpy(&value_uint64_t, &value, 8);
    memcpy(&xvalue_uint64_t, &(x.value), 8);


    for (check = true, i = 0; i < 52; i++) {
        check &= (((value_uint64_t >> i) & one_uint64_t) & ((xvalue_uint64_t >> i) & one_uint64_t));
    }

    return check;


}

template<class S> void Double::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

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
    value = stod(line.substr(pos + 3, line.size() - (pos + 3)).c_str(), NULL);

    print(name, prefix, cout);

}


//reads from file the content after 'name = ' and writes it into this. This function opens a new file, sets its name to filename and opens it
void Double::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Double>(this, name, filename, mode, prefix);


}








void Double::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}

Double Double::operator + (const Double& x) {

    Double s;

    (s.value) = value + (x.value);

    return s;

}


bool Double::operator < (const Double& x){
    
    return (value < x.value);
    
}

bool Double::operator < (const double& x){
    
    return (value < x);
    
}

bool Double::operator > (const Double& x){
    
    return (value > x.value);
    
}

bool Double::operator > (const double& x){
    
    return (value > x);
    
}
