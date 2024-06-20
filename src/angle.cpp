//
//  angle.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include "angle.h"
#include "constants.h"
#include "generic.h"


//constructor of Angle, which does not set the value of the angle
Angle::Angle(void) {

}

//constructor of Angle, which sets the value of the angle to x, where x is in radians
Angle::Angle(const double& x) {

    value = x;
    normalize();

}

//constructor of Angle, which sets the value of the angle to deg° min'
Angle::Angle(const unsigned int& deg, const double& min) {

    value = deg_to_rad * (((double)deg) + min / 60.0);
    normalize();

}


//constructor of Angle, which sets the value of the angle to x and eventually prints *this
Angle::Angle(String name, const double& x, [[maybe_unused]] const String& prefix) {

    value = x;
    normalize();
    if (name != String("")) { print(name, prefix, cout); }

}

inline bool Angle::operator == (const Angle& x) const{

    return((value == (x.value)));

}

inline bool Angle::operator!=(const Angle& x) {

    return(!((*this) == x));

}

inline bool Angle::operator==(const double& x) {

    return((value == x));

}

inline bool Angle::operator!=(const double& x) {

    return((value != x));

}


bool Angle::operator>(const Angle& x) {

    return((value > (x.value)));

}

bool Angle::operator>(const double& x) {

    return((value > x));

}

//I added the booleian variable search_entire_stream. If true, then this function rewinds the file pointer to the beginning of file and goes through the file until it finds the quantity 'name'. If false, it reads the angle at the position where 'file' was when it was passed to this function
template<class S> void Angle::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos1, pos2, pos3;

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
    pos2 = line.find("° ");
    pos3 = line.find("'");

    value = deg_to_rad * (stod(line.substr(pos1 + 3, pos2 - (pos1 + 3)).c_str()) + stod(line.substr(pos2 + 2, pos3 - (pos2 + 2))) / 60.0);

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);

}

//reads from file the content after 'name = ' and writes it into this.
void Angle::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Angle>(this, name, filename, mode, prefix);

}


Angle Angle::operator + (const Angle& angle) {

    Angle temp;

    (temp.value) = value + (angle.value);
    temp.normalize();

    return temp;

}


void Angle::operator += (const Angle& angle) {

    value += (angle.value);
    normalize();

}


void Angle::operator += (const double& x) {

    value += x;
    normalize();

}


void Angle::operator -= (const Angle& angle) {

    value -= (angle.value);
    normalize();

}


void Angle::operator -= (const double& x) {

    value -= x;
    normalize();

}


Angle Angle::operator- (const Angle& angle) {
    Angle temp;

    (temp.value) = value - (angle.value);
    temp.normalize();

    return temp;
}

Angle Angle::operator* (const double& x) {

    Angle temp;

    temp.value = value * x;
    temp.normalize();

    return temp;

}

Angle Angle::operator/ (const double& x) {

    Angle temp;

    temp.value = value / x;
    temp.normalize();

    return temp;

}


//set the value of *this equal to x and normalize *this
inline void Angle::set(double x){
    
    value = x;
    normalize();

}

//call Angle::set(x) and print name with prefix
inline void Angle::set(String name, double x, [[maybe_unused]] String prefix) {

    set(x);
    if(name != String("")) { print(name, prefix, cout); }

}

void Angle::normalize(void) {

    value = value - 2.0 * M_PI * floor(value / (2.0 * M_PI));

}

Angle Angle::normalize_ret(void) {

    Angle temp;

    temp.set(value - 2.0 * M_PI * floor(value / (2.0 * M_PI)));
    
    return temp;
}


//put the angle *this in the interval [-pi, pi) and write the result in *this
void Angle::normalize_pm_pi(void) {

    normalize();
    if (value > M_PI) { value -= 2.0 * M_PI; }

}

//returns the angular span between *this and x, where it must be (*this).value < x.value, by taking into account the periodicity of *this and x with respect to 2 pi
Angle Angle::span(Angle x) {

    Angle delta;

    if (((*this) < M_PI) && (x > M_PI)) {

        delta.set((value + (2.0 * M_PI)) - (x.value));

    }
    else {

        delta.set(value - (x.value));

    }

    return delta;

}

//puts the angle in the interval [-pi, pi), it does not alter *this and returns the result
inline Angle Angle::normalize_pm_pi_ret(void) {

    Angle temp;

    temp = (*this);
    temp.normalize_pm_pi();

    return temp;

}



void Angle::print(String name, String prefix, ostream& ostr) {

    normalize();
    ostr << prefix.value << name.value << " = " << floor(K * value - 360.0 * floor(K * value / 360.0)) << (wxGetApp().degree_symbol.value) << " " << (K * value - 360.0 * floor(K * value / 360.0) - floor(K * value - 360.0 * floor(K * value / 360.0))) * 60.0 << "'\n";

}


//return true is *this is close to 0 or 2 pi within precision epsilon_double, false otherwise
inline bool Angle::is_zero_epsilon_double(void){
    
    return((fabs(value) < epsilon_double) || (fabs(value-2.0*M_PI) < epsilon_double));

}


//convert the angle contained in (*this).value to degrees and minutes format, storted in deg and min
void Angle::to_deg_min(unsigned int* deg, double* min) {

    normalize();
    (*deg) = (unsigned int)floor(K * value - 360.0 * floor(K * value / 360.0));
    (*min) = (K * value - 360.0 * floor(K * value / 360.0) - floor(K * value - 360.0 * floor(K * value / 360.0))) * 60.0;

}


//convert the angle contained in (*this).value to degrees and minutes format, storted in deg and min,  round up the minutes part with precision `precision`, and re-format deg and min in order to avoid results of the form *deg = 23, *min = 60.0
void Angle::to_deg_min(unsigned int* deg, double* min, unsigned int precision) {

    to_deg_min(deg, min);
    (*min) = round_with_precision((*min), precision);
    if((*min) == 60.0){
        (*min) = 0.0;
        (*deg)++;
    }
    
}


//convert the angle stored in degrees and minutes format in deg an min in to (*this).vaule
void Angle::from_sign_deg_min(char sign, unsigned int deg, double min) {

    value = deg_to_rad * (((double)deg) + min / 60.0);
    if (sign == '-') { value *= -1.0; }

    normalize();

}

//this function converts an Angle to a string. If add_spaces = true, then instead of "9° 2.3'" I output "  9°  2.3'", i.e., I fill the spaces with blank spaces, so all angles will have the same format when converted to strings
string Angle::to_string(String mode, unsigned int precision, bool add_spaces) {

    stringstream output;
    stringstream deg, min;
    int i;
    double x;
    //a temporary variable where to store this->value and modifyi it without altering this->value
    double value_temp;

    min.precision(precision);

    normalize();
    value_temp = value;


    if ((mode != String("")) && (value_temp > M_PI)) {
        value_temp -= 2.0 * M_PI;
        value_temp = fabs(value_temp);
    }

    //write the arcdegree part of the Angle into deg
    deg.str("");
    i = floor(K * value_temp);
    if (add_spaces) {
        if (i < 10) {
            deg << "  ";
        }
        else {
            if (i < 100) {
                deg << " ";
            }
        }
    }

    
    //write the arcminute part of the Angle into min
    min.str("");
    
    //round up to the precision `precision`
    x = round_with_precision((K * value_temp - floor(K * value_temp)) * 60.0, precision);
    //if, after the round up, x is equal to 60.0 (exactly), I set x to 0.0 and increase the defrees by one: in this way, I will never have weird angle vlaues such as 1 degree 60.0'
    if(x == 60.0){
        x = 0.0;
        i++;
    }
    
    if (add_spaces) {
        //sets the fixed precision min, so all angles that are printed out have the same number of decimal points
        min << fixed;
        if (x < 10.0) {
            min << " ";
        }
    }
    
    deg << i;
    min << x;



    output << deg.str().c_str() << (wxGetApp().degree_symbol).value << " " << min.str().c_str() << "'";

    if ((mode != String("")) && !is_zero_epsilon_double()) {
        //mode is not "" and the angle is nonzero -> I print out its 'sign' (N, S, E, W, ...)

        if (mode == String("NS")) {
            //in this case, I output the sign of the angle in the North/South format (North = +, South = -)

            if (value < M_PI) { output << " N"; }
            else { output << " S"; }
        }
        if (mode == String("EW")) {
            //in this case, I output the sign of the angle in the East/West format (West = +, East = -)

            if (value < M_PI) { output << " W"; }
            else { output << " E"; }
        }

    }

    return (output.str().c_str());

}


//this function prints out only the integer degree closest to this
string Angle::deg_to_string(String mode, [[maybe_unused]] unsigned int precision) {

    stringstream output;

    //    output.precision(precision);

    normalize();

    if (mode == String("")) {
        //in this case, I print out the angle in the format >=0° and <360°
        output << round(K * value) << wxGetApp().degree_symbol.value;

    }
    else {
        //in this case, I print out the angle in the format >=-180° and <180°


        //I append NS or EW only if the angle is != 0, otherwise it is pointless to add these labels
        if (!is_zero_epsilon_double()) {

            if (mode == String("NS")) {
                //in this case, I output the sign of the angle in the North/South format (North = +, South = -)

                if (value < M_PI) {

                    if (value < M_PI_2) {

                        output << round(fabs(K * value)) << wxGetApp().degree_symbol.value << " N";

                    }
                    else {

                        output << round(fabs(K * (M_PI - value))) << wxGetApp().degree_symbol.value << " N";

                    }

                }
                else {

                    if (value < 3.0 * M_PI_2) {

                        output << round(fabs(K * (-M_PI + value))) << wxGetApp().degree_symbol.value << " S";

                    }
                    else {

                        output << round(fabs(K * (2.0 * M_PI - value))) << wxGetApp().degree_symbol.value << " S";

                    }

                }

            }
            else {
                //in this case, I output the sign of the angle in the East/West format (West = +, East = -)

                if (value > M_PI) { value -= 2.0 * M_PI; }
                output << round(fabs(K * value)) << wxGetApp().degree_symbol.value;

                if (value > 0.0) { output << " W"; }
                else { output << " E"; }

            }

        }
        else {

            output << "0" << wxGetApp().degree_symbol.value;

        }

    }

    return (output.str().c_str());

}

//this function prints out only the arcminute part of this
string Angle::min_to_string(String mode, unsigned int precision) {

    stringstream output;

    output.precision(precision);

    normalize();

    if (mode != String("")) {
        //in this case, I print out the angle in the format >=-180° and <180°
        if (value > M_PI) { value -= 2.0 * M_PI; }
    }

    output << (fabs(K * value) - floor(fabs(K * value))) * 60.0 << "'";

    return (output.str().c_str());

}


//returns true if a.normalize_pm_pi_ret().value < b.normalize_pm_pi_ret().value and false otherwise
inline bool Angle::strictly_smaller_normalize_pm_pi_ret(Angle& a, Angle& b){
    
    return((a.normalize_pm_pi_ret().value) < (b.normalize_pm_pi_ret().value));
    
}

//returns true if a.normalize_pm_pi_ret().value > b.normalize_pm_pi_ret().value and false otherwise
inline bool Angle::strictly_larger_normalize_pm_pi_ret(Angle& a, Angle& b){
    
    return((a.normalize_pm_pi_ret().value) > (b.normalize_pm_pi_ret().value));
    
}


