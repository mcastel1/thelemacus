//
//  body.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "body.h"
#include "generic.h"
#include "my_string.h"


Body::Body(void){
    
    name = new BodyName;
    type = new String;
    radius = new Length;
    RA = new Angle;
    d = new Angle;
    
}

bool Body::operator==(const Body& body) {

    return (name == (body.name));

}

//read a Body from file, and it returns true if it has not reached the end of file, false otherwise
template<class S> bool Body::read_from_stream(String name_in, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    size_t pos;

    cout << prefix.value << name_in.value << ":\n";

    //read first line with no information
    getline((*input_stream), line);

    if (!(input_stream->eof())) {
        //*input_stream has not reached the end of file


        //read (*type)
        line.clear();
        getline(*input_stream, line);
        pos = line.find(" = ");
        (*type) = line.substr(pos + 3, line.size() - (pos + 3));
        cout << new_prefix.value << "Type = " << type->value << "\n";


        //read name
        line.clear();
        getline(*input_stream, line);
        pos = line.find(" = ");
        
        name->set(line.substr(pos + 3, line.size() - (pos + 3)));
        cout << new_prefix.value << "Name = " << name->value << "\n";


        if ((*type) == String("star")) {
            RA->read_from_stream<S>(String("right ascension"), input_stream, false, new_prefix);
            d->read_from_stream<S>(String("declination"), input_stream, false, new_prefix);
        }
        else {
            radius->read_from_stream<S>(String("radius"), input_stream, false, new_prefix);
        }

        return true;

    }
    else {
        //*input_stream has reached the end of stream

        return false;

    }

}

template bool Body::read_from_stream<basic_istringstream<char, char_traits<char>, allocator<char>>>(String, basic_istringstream<char, char_traits<char>, allocator<char>>*, bool, String);


//set the *content* (not the memory adresses) of *this equal to the content of input
void Body::set(const Body& input){
    
    name->set((*(input.name)));
    type->set((*(input.type)));
        
    radius->set((*(input.radius)));
    
    RA->set((*(input.RA)));
    d->set((*(input.d)));
    
}


//if input is a valid BodyName, set *this equal to the Body with name input
void Body::set_from_name(const BodyName& input){
    
    unsigned int i;
    
    i = position_in_vector(input, wxGetApp().catalog->get_body_names());
    
    if(i != (wxGetApp().catalog->get_body_names().size())){
        //input is a valid BodyName
        
        set((wxGetApp().catalog->list)[i]);
        
    }
    
}


template bool Body::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);


void Body::print(String name_in, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name_in.value << ":\n";

    ostr << new_prefix.value << "Type = " << type->value << "\n";
    ostr << new_prefix.value << "Name = " << name->value << "\n";

    if((*type) == String("star")){
        RA->print(String("Right ascension"), new_prefix, ostr);
        d->print(String("Declination"), new_prefix, ostr);
    }else{
        radius->print(String("Radius"), new_prefix, ostr);
    }

}




//this function checks whether the Body's name  is found in the body list comprised in catalog
bool Body::check(unsigned int* j, Catalog catalog, [[maybe_unused]] String prefix) {

    unsigned int i;
    bool check;

    for (i = 0, check = false; (i < (catalog).list.size()) && (!check); i++) {

        if ((((catalog.list)[i]).name) == name) {
            check = true;
        }

    }

    if (check) {
        (*j) = i - 1;
    }
    else {
        cout << prefix.value << RED << "Body not found in catalog!\n" << RESET;
    }

    return check;

}
