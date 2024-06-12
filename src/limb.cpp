//
//  limb.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "limb.h"



template<class S> void Limb::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

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

    value = line[pos + 3];

    print(name, prefix, cout);


}


void Limb::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}



bool Limb::operator == (const Limb& limb) {

    return(value == limb.value);

}
