//
//  answer.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "answer.h"



Answer::Answer(void) {
    //this is the default constructor, sets value to the default value, 'n' char

    value = 'n';

}

Answer::Answer(char c, [[maybe_unused]] String prefix) {

    if ((c == 'y') || (c == 'n')) {

        value = c;

    }
    else {
        //if the entered value is not valid, set value to 'n' and prints out this info

        value = 'n';
        cout << prefix.value << RED << "Value of answer is not valid, setting it to 'n'!\n" << RESET;

    }

}


bool Answer::operator==(const Answer& s) {

    return((value == (s.value)));

}

bool Answer::operator!=(const Answer& s) {

    return(!((*this) == s));

}


bool Answer::set(String name, char c, [[maybe_unused]] String prefix) {

    bool check;

    check = true;
    check &= ((c == 'y') || (c == 'n'));

    if (check) {

        value = c;

        if (name != String("")) {

            cout << prefix.value << name.value << " = " << c << "\n";

        }

    }
    else {

        cout << prefix.value << RED << "Value of answer is not valid!\n" << RESET;

    }

    return check;

}
//reads *this from file whose path is filename, by looking through the entire file
void Answer::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Answer>(this, name, filename, mode, prefix);

}



template<class S> void Answer::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << "... \n" << RESET;

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



    pos = line.find(" = ");

    value = line[pos + 3];

    cout << prefix.value << YELLOW << "... done.\n" << RESET;


    print(name, prefix, cout);


}


void Answer::enter(String name, [[maybe_unused]] String prefix) {

    bool check;
    string temp;

    do {

        temp.clear();

        cout << prefix.value << "Enter " << name.value << " [y/n]:";

        getline(cin >> ws, temp);

        if (((temp[0] == 'y') || (temp[0] == 'n')) && (temp.size() == 1)) {
            value = temp[0];
            check = true;
        }
        else {
            cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
            check = false;
        }
    } while (!check);

    print(name, prefix, cout);

}

void Answer::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}
