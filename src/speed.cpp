//
//  speed.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>


#include "speed.h"
#include "constants.h"
#include "units.h"


//constructor of Speed that takes no arguments, and set the unit to the first entry of SpeedUnit_types
Speed::Speed(void) {
    
    unit = SpeedUnit(SpeedUnit_types[0]);
    
}

//constructor of Speed that sets value to value_in, and  the unit to the first entry of SpeedUnit_types
Speed::Speed(double value_in) {

    value = value_in;
    unit = SpeedUnit(SpeedUnit_types[0]);

}


//constructor which sets value to x  and the unit to unit_in
Speed::Speed(double value_in, const SpeedUnit& unit_in) {

    value = value_in;
    unit = unit_in;

}


void Speed::print(String name_in, String unit_in, String prefix, ostream& ostr) {

    if ((name_in.value) != "") {

        ostr << prefix.value << name_in.value << " = ";
        if (unit_in == SpeedUnit_types[0]) {
            //units are kt

            ostr << value << " nm\n";

        }
        else {

            if (unit_in == SpeedUnit_types[1]) {
                //units are km/h

                ostr << value * nm_to_km << " km/h\n";

            }
            else {

                if (unit_in == SpeedUnit_types[2]) {
                    //units are m/s

                    ostr << value * nm_to_km * 1e3 / 3600.0 << " m/s\n";

                }

            }

        }

    }

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
    pos2 = line.find(" kt");

    if (line.find(" kt") != (string::npos)) {
        //the units of the speed read is kt
        cout << prefix.value << "Unit is in kt\n";
        pos2 = line.find(" kt");
        unit_temp = SpeedUnit_types[0];
    }
    if (line.find(" km/h") != (string::npos)) {
        //the units of the length read is km/h
        cout << prefix.value << "Unit is in km/h\n";
        pos2 = line.find(" km/h");
        unit_temp = SpeedUnit_types[1];
    }
    if (line.find(" m/s") != (string::npos)) {
        //the units of the length read is m/s
        cout << prefix.value << "Unit is in m/s\n";
        pos2 = line.find(" m/s");
        unit_temp = SpeedUnit_types[2];
    }

    //X [km/h] = X [nm]/nm_to_km/[h] = X/nm_to_km [kt] = X 1000/3600 [m/s]

    value = stod(line.substr(pos1 + 3, pos2 - (pos1 + 3)).c_str());
    if (unit_temp == SpeedUnit_types[1]) {
        value /= nm_to_km;
    }
    if (unit_temp == SpeedUnit_types[2]) {
        value /= (1e3) * nm_to_km / 3600.0;
    }

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, SpeedUnit_types[0], prefix, cout);

}


void Speed::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << " kt\n";

}


void Speed::set(double x){
    
    value = x;
    
}


//set the value of *this equal to x and the units equal to unit_in
inline void Speed::set(double value_in, const SpeedUnit& unit_in) {
    
    value = value_in;
    unit = unit_in;
    
}


//set the value of *this to x, where x is in kt
void Speed::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(x);
    
    if (name != String("")) { print(name, SpeedUnit_types[0], prefix, cout); }

}
