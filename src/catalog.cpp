//
//  catalog.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include "catalog.h"
#include "generic.h"

Catalog::Catalog(String filename, [[maybe_unused]] String prefix) {

    read_from_file_to(String("catalog"), filename, String("R"), prefix);


}

//reads from file the Catalog and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Catalog::read_from_file_to([[maybe_unused]] String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Catalog>(this, name, filename, mode, prefix);

}

//read from stream input_stream the content and writes it into this
template<class S> void Catalog::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    Body body;

    //check whether the next line in the file has reached the end of file
    while ((body.read_from_stream<S>(String("read body"), input_stream, false, prefix)) == true) {

        //if the next line in the file has not reached the end of file, I set *(file.value) to its old position and keep reading the file
        //THE ERROR IS HERE: all entried of list contain the same memory adresses within the Body class, which are the adresses of `body` declared in `Body body` above
//        list.push_back(body);
        list.resize(list.size()+1);
        list.back().set(body);

    }

}

template void Catalog::read_from_stream<std::__1::basic_fstream<char, std::__1::char_traits<char>>>(String, std::__1::basic_fstream<char, std::__1::char_traits<char>>*, bool, String);


//return a vector containing the names of the Bodies in *this
vector<String> Catalog::get_names(void){
    
    unsigned int i;
    vector<String> output;
    
    for(i=0, output.clear(); i<list.size(); i++){
        output.push_back((*((list[i]).name)));
    }
    
    return output;
    
    
}


void Catalog::print(String prefix, ostream& ostr) {

    unsigned int i;
    stringstream name;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Bodies in the catalog:\n";
    for (i = 0; i < list.size(); i++) {

        name.str("");
        name << "Body #" << i;
        (list[i]).print(name.str(), new_prefix, ostr);

    }

}

void Catalog::add(String type, String name, double radius) {

    Body body;

    (*(body.type)) = type;
    (*(body.name)) = name;
    body.radius->value = radius;

    list.push_back(body);

    cout << "Added body to catalog:\n";
    body.print(String("body"), String("\t"), cout);

}


