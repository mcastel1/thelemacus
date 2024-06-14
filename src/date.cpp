//
//  date.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include "constants.h"
#include "date.h"
#include "my_string.h"



//this function returns true if the date read is consistent, false if it is not
template<class S> bool Date::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

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

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }

    pos = line.find(" = ");

    Y = stoi(line.substr(pos + 3, 4).c_str(), NULL, 10);

    check_leap_year();
    if ((Y_is_leap_year)) {
        (days_per_month) = days_per_month_leap;
        cout << new_prefix.str() << YELLOW << "Entered a leap year\n" << RESET;
    }
    else {
        (days_per_month) = days_per_month_common;
        cout << new_prefix.str() << "Entered a common year\n";
    }

    M = stoi(line.substr(pos + 3 + 5, 2).c_str(), NULL, 10);

    if (!((1 <= M) && (M < 12 + 1))) {
        check &= false;
        cout << new_prefix.str() << RED << "\tValue of MM is not valid!\n" << RESET;
    }

    D = stoi(line.substr(pos + 3 + 5 + 3, 2).c_str());

    if (!((1 <= D) && (D < days_per_month[M - 1] + 1))) {
        check &= false;
        cout << prefix.value << RED << "\tValue of DD is not valid!\n" << RESET;
    }

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}









void Date::check_leap_year(void) {

    if ((Y % 4) != 0) {

        Y_is_leap_year = false;

    }
    else {

        if ((Y % 100) != 0) {

            Y_is_leap_year = true;

        }
        else {
            if ((Y % 400) != 0) {

                Y_is_leap_year = false;

            }
            else {

                Y_is_leap_year = true;

            }

        }

    }

}




void Date::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << to_string() << "\n";

};


//this function sets (*this) to the current UTC date
void Date::set_current(void) {

    (wxGetApp().local_time) = (boost::posix_time::second_clock::local_time());

    Y = (wxGetApp().local_time).date().year();
    M = (wxGetApp().local_time).date().month().as_number();
    D = (wxGetApp().local_time).date().day();

}



void Date::enter(String name, String prefix) {

    string input;
    String new_prefix;
    bool check;
    size_t pos;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    do {

        check = true;

        cout << prefix.value << "Enter " << name.value << " [YYYY MM DD], or press enter for current UTC date:";
        getline(cin, input);

        if (input.empty()) {

            cout << prefix.value << YELLOW << "Entered an empty date, setting it to current UTC date!\n" << RESET;
            set_current();
            print(String("entered date"), prefix, cout);

        }
        else {

            //check whether the string contains two ' '
            check &= (count(input.begin(), input.end(), ' ') == 2);

            if (check) {

                pos = (input).find(" ");

                //check whether year part is formatted correctly
                check &= check_unsigned_int(input.substr(0, pos).c_str(), &Y, false, 0, 0);

                if (check) {
                    //in this case, the year's format is valid

                    //I check whether Y is a leap year and compute days_per_month
                    check_leap_year();
                    if (Y_is_leap_year) {

                        days_per_month = days_per_month_leap;
                        cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET;

                    }
                    else {

                        days_per_month = days_per_month_common;
                        cout << new_prefix.value << "Entered a common year\n";

                    }

                    //now I am no longer interested in the year, the string runs from the month to days
                    input = (input.substr(pos + 1).c_str());

                    //find the position of the second ' '
                    pos = input.find(" ");

                    //check whether month part is formatted correctly
                    check &= check_unsigned_int(input.substr(0, pos).c_str(), &M, true, 1, 12 + 1);

                    if (check) {
                        //in this case the month part is formatted correctly

                        //now I am no longer interested in the month, the string runs from the days to the end of the string
                        input = (input.substr(pos + 1).c_str());

                        //check whether day part is formatted correctly
                        check &= check_unsigned_int(input.c_str(), &D, true, 1, days_per_month[M - 1] + 1);

                    }

                }

            }

        }

        if (!check) {

            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;

        }

    } while (!check);


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


string Date::to_string(void) {

    stringstream output;

    output << Y << "-";
    if (M < 10) { output << 0; }
    output << M << "-";
    if (D < 10) { output << 0; }
    output << D;

    return (output.str().c_str());

}

