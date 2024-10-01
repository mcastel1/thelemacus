//
//  chrono.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include <boost/date_time.hpp>


#include "chrono.h"
#include "generic.h"
#include "my_app.h"


bool Chrono::operator==(const Chrono& chrono) {

    return((h == (chrono.h)) && (m == (chrono.m)) && (s == (chrono.s)));

}

bool Chrono::operator!=(const Chrono& chrono) {

    return (!((*this) == chrono));

}

bool Chrono::operator<(const Chrono& chrono) {

    Chrono temp;

    temp = chrono;

    return((this->get()) < (temp.get()));

}

bool Chrono::operator>(const Chrono& chrono) {

    Chrono temp;

    temp = chrono;

    return((this->get()) > (temp.get()));

}

//sets the Chrono object to the time x, which is expressed in hours
bool Chrono::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;
    bool check;

    check = true;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    check &= (x >= 0.0);
    if (check) {

        h = ((unsigned int)floor(x));
        m = (unsigned int)((x - ((double)h)) * 60.0);
        s = (((x - ((double)h)) * 60.0) - ((double)m)) * 60.0;

        if (name != String("")) { print(name, prefix, cout); }

    }
    else {

        cout << new_prefix.value << RED << "Set value is not valid!\n" << RESET;

    }

    return check;


}


//set the content (not the memory adresses) of *this equal to the content of x
void Chrono::set(const Chrono& x){
    
    h = (x.h);
    m = (x.m);
    s = (x.s);
    
}


template<class S> bool Chrono::read_from_stream([[maybe_unused]] String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    bool check = true;
    size_t pos;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    pos = 0;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while ((line.find(name.value)) == (string::npos));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }



    pos = line.find(" = ");

    //read hours
    h = stoi(line.substr(pos + 3, 2).c_str(), NULL, 10);
    if (!((0 <= h) && (h < 24))) {

        check &= false;
        cout << prefix.value << RED << "\tValue of hh is not valid!\n" << RESET;

    }

    //read minutes
    m = stoi(line.substr(pos + 3 + 3, 2).c_str(), NULL, 10);
    if (!((0 <= m) && (m < 60))) {

        check &= false;
        cout << prefix.value << RED << "\tValue of mm is not valid!\n" << RESET;

    }

    //read seconds
    s = stod(line.substr(pos + 3 + 3 + 3, line.size() - (pos + 3 + 3 + 3)).c_str());
    if (!((0.0 <= s) && (s < 60.0))) {

        check &= false;
        cout << prefix.value << RED << "\tValue of mm is not valid!\n" << RESET;

    }

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}

template bool Chrono::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);
template bool Chrono::read_from_stream< basic_istringstream<char,struct char_traits<char>, allocator<char> > >( String, basic_istringstream<char,struct char_traits<char>, allocator<char> > *,bool, String);



//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Chrono::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Chrono>(this, name, filename, mode, prefix);

}



//convert *this to a string. If time_zone == true, then I add at the end of the string the time zone compared to UTC written in wxGetApp().time_zone
string Chrono::to_string(unsigned int precision, bool time_zone) {

    stringstream output;

    output.precision(precision);

    if (h < 10) { output << 0; }
    output << h << ":";
    if (m < 10) { output << 0; }
    output << m << ":";
    if (s < 10.0) { output << 0; }
    output << s;
    if (time_zone) {
        output << " UTC" << (wxGetApp().time_zone > 0 ? "+" : "-") << fabs(wxGetApp().time_zone.value);
    }

    return (output.str().c_str());

}


//return the value of *this expresser in hours
double Chrono::get(void) {

    return(((double)h) + ((double)m) / 60.0 + ((double)s) / (60.0 * 60.0));

}


//this function sets (*this) to the current UTC time +- time_zone
void Chrono::set_current_utc(void) {

    (wxGetApp().utc_time) = (boost::posix_time::second_clock::universal_time());

    h = ((unsigned int)((wxGetApp().utc_time).time_of_day().hours()));
    m = ((unsigned int)((wxGetApp().utc_time).time_of_day().minutes()));
    s = (wxGetApp().utc_time).time_of_day().seconds();

}


//print *this to ostr
void Chrono::print(String name, String prefix, ostream& ostr) {

    unsigned int precision;

    //if I am printing to terminal, I print with display_precision. Otherwise, I print with (data_precision.get())
    if (ostr.rdbuf() == cout.rdbuf()) {
        precision = (display_precision.get());
    }
    else {
        precision = (data_precision.get());
    }

    ostr << prefix.value << (name.value) << " = " << to_string(precision, false) << "\n";

};


//return *this expressed in milliseconds
unsigned int Chrono::to_milliseconds(void){
    
    return ((h * 60.0 * 60.0 + m * 60.0 + s) * 1000.0);
    
}
