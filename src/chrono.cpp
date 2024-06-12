//
//  chrono.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "chrono.h"



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
