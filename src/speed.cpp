//
//  speed.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>


#include "speed.h"
#include "constants.h"
#include "generic.h"
#include "units.h"


//constructor of Speed that takes no arguments, and set the unit to the first entry of SpeedUnit_types
Speed::Speed(void) {
    
    unit = new SpeedUnit(SpeedUnit_types[0]);
    
}

//constructor of Speed that sets value to value_in, and  the unit to the first entry of SpeedUnit_types
Speed::Speed(double value_in) {

    value = value_in;
    unit = new SpeedUnit(SpeedUnit_types[0]);

}


//constructor which sets value to x  and the unit to unit_in
Speed::Speed(double value_in, const SpeedUnit& unit_in) {

    value = value_in;
    unit = new SpeedUnit;
    
    unit->set(unit_in);

}


//reads from file the content after 'name = ' and writes it into this. This function requires file to be correctly set and open
template<class S> void Speed::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    size_t pos1, pos2;
    String unit_temp;

    
    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline(*input_stream, line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline(*input_stream, line);

    }

    pos1 = line.find(" = ");
    pos1 += 3;
    //from now on pos1 is the starting position of the  numerical value
    //pos2-1 contains the last character of the numerical value
    pos2 = line.find(" ", pos1);
    
    //thus I store the numerical value in to value ...
    value = stod(line.substr(pos1, pos2 - pos1).c_str());
    
    // .. and the unit into unit
    pos1 = pos2+1;
    unit->set(line.substr(pos1));

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);


}

template void Speed::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);


void Speed::print(String name, String prefix, ostream& ostr) {
    
    unsigned int precision;

    //if I am printing to terminal, I print with display_precision. Otherwise, I print with (data_precision.value)
    if (ostr.rdbuf() == cout.rdbuf()) {
        precision = (display_precision.value);
    }
    else {
        precision = (data_precision.value);
    }

    if ((name.value) != "") {

        ostr << prefix.value << name.value << " = " << to_string(precision) << endl;
  
    }

}


//set the content (not the memory adresses) of *this equal to the content of x
void Speed::set(const Speed& x){
    
    value = (x.value);
    unit->set((*(x.unit)));
    
}

void Speed::set(double x){
    
    value = x;
    
}


//set the value of *this equal to x and the units equal to unit_in
//inline 
void Speed::set(double value_in, const SpeedUnit& unit_in) {
    
    value = value_in;
    unit->set(unit_in);
    
}


//set the value of *this to x, where x is in kt
void Speed::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(x);
    
    print(name, prefix, cout); 

}


//convert *this to string by printing it in the unit of measure unit_in, with numerical precision precision
string Speed::to_string(const SpeedUnit& output_unit, unsigned int precision) {

    Speed temp;
    
    temp = (*this);
    temp.convert_to(output_unit);
    
    return(temp.to_string(precision));

}


//convert *this to string with numerical precision precision
string Speed::to_string(unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    output << fixed << value << " " << unit->value;
    
    return(output.str().c_str());
    
}



//convert *this to unit of measure unit_in, set unit = unit_in and write the result in *this
//inline
void Speed::convert_to(const SpeedUnit& output_unit){
    
    //the value of this in units of measure SpeedUnit_types[0]
    double value0 = 0.0;

    
    //1. convert *this to unit SpeedUnit_types[0] and write the result in value_in_SpeedUnit_types0
    switch (position_in_vector((*unit), SpeedUnit_types)) {
            
        case 0:{
            //unit = SpeedUnit_types[0]
            
            value0 = value;
            
            break;
            
        }

        case 1:{
            //unit = SpeedUnit_types[1]
            
            value0 = value * kmh_to_kt;
            
            break;
            
        }
            
        case 2:{
            //unit = SpeedUnit_types[2]
            
            value0 = value * ms_to_kt;
            
            break;
            
        }
            
    }
    
    
    //2. convert *this to unit output_unit and write the result in *this
    switch (position_in_vector(output_unit, SpeedUnit_types)) {
            
        case 0:{
            //output_unit = SpeedUnit_types[0]
            
            value = value0;
            
            break;
            
        }

        case 1:{
            //output_unit = SpeedUnit_types[1]
            
            value = value0 * kt_to_kmh;

            break;
            
        }
            
        case 2:{
            //output_unit = SpeedUnit_types[2]
            
            value = value0 * kt_to_ms;

            break;
            
        }
            
    }
    
    unit->set(output_unit);
    
}
