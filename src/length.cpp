//
//  length.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "length.h"

#include <sstream>

#include "constants.h"
#include "generic.h"
#include "length_unit.h"
#include "limb.h"
#include "my_string.h"
#include "units.h"



//default constructor, which sets value to 0.0 by default and the unit to the first entry of LengthUnit_types
Length::Length() {

    value = 0.0;
    unit = LengthUnit(LengthUnit_types[0]);

}

//constructor which sets value to x  and the unit to the first entry of LengthUnit_types
Length::Length(double x) {

    value = x;
    unit = LengthUnit(LengthUnit_types[0]);

}


//constructor which sets value to x  and the unit to unit_in
Length::Length(double value_in, const LengthUnit& unit_in) {

    value = value_in;
    unit = unit_in;

}


//construct the Length *this frome time and speed, by setting it equal to time x speed and its unit equal to LengthUnit_types[0]. This method takes into account the units in which speed is expressed (which are stored into speed.unit)
Length::Length(Chrono time, Speed speed) {

    //conversion factor
    double c = 0.0;
    
    //consider all possible units in which speed is expressed
    switch (speed.unit.position_in_list(SpeedUnit_types)) {
            
        case 0: {
            //speed.unit = SpeedUnit_types[0]
            
            c = 1.0;
            
            break;
            
        }

        case 1: {
            //speed.unit = SpeedUnit_types[1]

            c = 1.0/nm_to_km;
            
            break;
            
        }
            
        case 2: {
            //speed.unit = SpeedUnit_types[2]
            
            //[m]/[s] = 1e-3 3600 [km]/[h] = 1e-3 3600 / nm_to_km [nm]/[h] = 1e-3 3600 / nm_to_km [kt]
            c = (1e-3)*60.0*60.0/nm_to_km;
            
            break;
            
        }


    }
    
    set(c * (time.get()) * (speed.value));
    unit.set(LengthUnit_types[0]);

}


//set the value of *this equal to x (expressed in units LengthUnit_types[0]). The unit is not modified
//inline 
void Length::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(x);

    if(name != String("")){
        print(name, prefix, cout);
    }
    check(name, new_prefix);

}


//same as Length::set(String name, double x, [[maybe_unused]] String prefix)  but without printing out anything
inline void Length::set(double x) {
    
    value = x;
}


//set the value of *this equal to x and the units equal to unit_in
inline void Length::set(double value_in, const LengthUnit& unit_in) {
    
    value = value_in;
    unit = unit_in;
    
}


//convert *this to string with numerical precision precision
string Length::to_string(unsigned int precision){
    
    stringstream output;
    
    output.precision(precision);
    
    output << fixed << value << " " << unit.value;
    
    return(output.str().c_str());
    
}


//convert *this to string by printing it in the unit of measure unit_in, with numerical precision precision
string Length::to_string(const LengthUnit& output_unit, unsigned int precision) {

    Length temp;
    
    temp = (*this);
    temp.convert_to(output_unit);
    
    return(temp.to_string(precision));

}


//print *this and its unit of measure
void Length::print(String name, String prefix, ostream& ostr) {
    
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


//convert *this to unit of measure unit_in, set unit = unit_in and write the result in *this
//inline 
void Length::convert_to(const LengthUnit& output_unit){
    
    //the value of this in units of measure LengthUnit_types[0]
    double value0 = 0.0;

    
    //1. convert *this to unit LengthUnit_types[0] and write the result in value_in_LengthUnit_types0
    switch (unit.position_in_list(LengthUnit_types)) {
            
        case 0:{
            //unit = LengthUnit_types[0]
            
            value0 = value;
            
            break;
            
        }

        case 1:{
            //unit = LengthUnit_types[1]
            
            value0 = value * m_to_nm;
            
            break;
            
        }
            
        case 2:{
            //unit = LengthUnit_types[2]
            
            value0 = value * ft_to_nm;
            
            break;
            
        }
            
    }
    
    
    //2. convert *this to unit output_unit and write the result in *this
    switch (String(output_unit).position_in_list(LengthUnit_types)) {
            
        case 0:{
            //output_unit = LengthUnit_types[0]
            
            value = value0;
            
            break;
            
        }

        case 1:{
            //output_unit = LengthUnit_types[1]
            
            value = value0 * nm_to_m;

            break;
            
        }
            
        case 2:{
            //output_unit = LengthUnit_types[2]
            
            value = value0 * nm_to_ft;

            break;
            
        }
            
    }
    
    unit = output_unit;
    
}


//same as convert_to, but it returns the result
inline Length Length::convert(const LengthUnit& output_unit){
    
    Length result;
    
    result = (*this);
    result.convert_to(output_unit);
    
    return result;
    
}

//return true of both the value and the unit of *this are valid, false otherwise
bool Length::check(String name, [[maybe_unused]] String prefix) {

    bool check = true;

    if (value < 0.0) {
        check &= false;
        cout << prefix.value << RED << "Entered value of " << name.value << " is not valid!\n" << RESET;
    }

    check &= (unit.check());
    
    return check;

}

//reads from file the content after 'name = ' and writes it into this. This function requires file to be correctly set and open
template<class S> void Length::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

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
    //pos2-1 contains the last character of th enumerical value
    pos2 = line.find(" ", pos1);
    
    //thus I store the numerical value in to value ...
    value = stod(line.substr(pos1, pos2 - pos1).c_str());
    
    // .. and the unit inot unit
    pos1 = pos2+1;
    unit.set(line.substr(pos1));

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);

}

//reads from file the content after 'name = ' and writes it into this.
void Length::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Length>(this, name, filename, mode, prefix);

}





//add *this to length and write the result in *this. This works no matter what the units of *this and length. After this method is called, the units of *this are the same as before this method had been called
void Length::operator += (const Length& length) {
    
    if(unit == (length.unit)){
        //*this and length have the same units -> simply sum their values
        
        value += (length.value);
        
    }else{
        //*this and length have different units -> a unit conversion is neede
        
        LengthUnit unit_saved;
        
        //save the unit of measure of *this into unit_saved
        unit_saved = unit;
        
        //convert *this to the unit of measure of length : this will change unit to length.unit
        convert_to(length.unit);
        //now that *this and length have the same units, I can sum their values
        value += (length.value);
        //convert back *this to its original units, which had been saved inot unit_saved
        convert_to(unit_saved);
        
    }

}


//substract *this to length and write the result in *this. This works no matter what the units of *this and length. After this method is called, the units of *this are the same as before this method had been called
void Length::operator -= (const Length& length) {
    
    if(unit == (length.unit)){
        //*this and length have the same units -> simply sum their values
        
        value -= (length.value);
        
    }else{
        //*this and length have different units -> a unit conversion is neede
        
        LengthUnit unit_saved;
        
        //save the unit of measure of *this into unit_saved
        unit_saved = unit;
        
        //convert *this to the unit of measure of length : this will change unit to length.unit
        convert_to(length.unit);
        //now that *this and length have the same units, I can sum their values
        value -= (length.value);
        //convert back *this to its original units, which had been saved inot unit_saved
        convert_to(unit_saved);
        
    }

}


//multiply *this by x (which is supposed to be dimensionless) and write the result in *this by keeping the units of *this
void Length::operator *= (const double& x) {
    
    value *= x;

}


//divide *this by x (which is supposed to be dimensionless) and write the result in *this by keeping the units of *this
void Length::operator /= (const double& x) {
    
    value /= x;

}


//evaluates whether Length (*this) is > than r
inline bool Length::operator > (const Length& r) {
    
    if(unit == r.unit){
        //*this and r have the same units -> just compare their values
        
        return((value > (r.value)));
        
    }else{
        //*this and r have different units -> convert r to the units of *this and compare
        
        return(((this->convert(r.unit)) > (r.value)));

    }

}


//evaluates whether Length (*this) is <= r, see Length::operator >
inline bool Length::operator <= (const Length& r) {

    return(!((*this) > r));

}


//evaluates whether Length (*this) is < than r
inline bool Length::operator < (const Length& r) {
    
    if(unit == r.unit){
        //*this and r have the same units -> just compare their values
        
        return((value < (r.value)));
        
    }else{
        //*this and r have different units -> convert r to the units of *this and compare
        
        return(((this->convert(r.unit)) < (r.value)));

    }

}

//evaluates whether Length (*this) is >= r
inline bool Length::operator >= (const Length& r) {

    return(!((*this) < r));

}


//evaluates whether Length (*this) is > than the double r (this supposes that r represents a Length in the same units as this->unit)
inline bool Length::operator > (const double& r) {

    return((value > r));

}


//evaluates whether Length (*this) is >= than the double r (this supposes that r represents a Length in the same units as this->unit)
inline bool Length::operator >= (const double& r) {

    return(!((*this) < r));

}


//evaluates whether Length (*this) is <= than the double r (this supposes that r represents a Length in the same units as this->unit)
inline bool Length::operator <= (const double& r) {

    return(!((*this) > r));

}


//evaluates whether Length (*this) is smaller than the double r (this supposes that r represents a Length in the same units as this->unit)
inline bool Length::operator < (const double& r) {

    return((value < r));

}


//compute the sum between *this and l and return the result in the units of *this
Length Length::operator + (const Length& l) {

    Length s;
    
    s = (*this);
    s+= l;

    return s;

}


//compute the difference between *this and l and return the result in the units of *this
Length Length::operator - (const Length& l) {

    Length s;
    
    s = (*this);
    s -= l;

    return s;

}


//compute the product between *this and the double x (which is interpreted as a dimensinless quantity) and return the result as a Length with the same units as *this
Length Length::operator * (const double& x) {

    Length s;

    (s.value) = value * x;

    return s;

}


//compute the ratio between *this and the double x (which is interpreted as a dimensinless quantity) and return the result as a Length with the same units as *this
Length Length::operator / (const double& x) {

    Length s;

    (s.value) = value / x;

    return s;

}



//return true if both the value and unit of *this and length coincide, false otherwise
inline bool Length::operator == (const Length& length) {

    return((value == (length.value)) && (unit == (length.unit)));

}


inline bool Length::operator != (const Length& length) {

    return (!((*this) == length));

}

