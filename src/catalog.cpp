//
//  catalog.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include "catalog.h"
#include "generic.h"
#include "file_r.h"


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
    stringstream temp;
    String new_prefix;
    FileR body_file;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    //check whether the next line in the file has reached the end of file
    while ((body.read_from_stream<S>(String("read body"), input_stream, false, prefix)) == true) {

        //if the next line in the file has not reached the end of file, I set *(file.value) to its old position and keep reading the file
        my_push_back(&list, body);
        
        
        //count the number of lines in file relative to body and push it back into number_of_lines
        temp.clear();
        temp.str("");
        
        if ((*(body.type)) != String("star")) {
            
            temp << (wxGetApp().data_directory.value) << (body.name->value) << ".txt";
            
        }else{
            
            temp << (wxGetApp().data_directory.value) << "j2000_to_itrf93.txt";
            
        }
        
        body_file.set_name(temp.str());
        if(body_file.check_if_exists(new_prefix)){
            
            body_file.count_lines(new_prefix);
            number_of_lines_bodies.push_back(body_file.number_of_lines);
            
        }else{
            
            number_of_lines_bodies.push_back(0);
            
        }

        
    }

}

template void Catalog::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);


//return the position in this->list of BodyName x. If x is not found in this->list, it returns this->list.size()
unsigned int Catalog::position_in_list(const BodyName& x){
    
    unsigned int i;
    
    for(i=0; i<list.size(); i++){
        if((x.value) == ((list[i]).name->value)){
            break;
        }
    }
    
    return i;
    
}


//return a vector containing the names of the Bodies in *this
vector<BodyName> Catalog::get_body_names(void){
    
    unsigned int i;
    vector<BodyName> output;
    
    for(i=0, output.clear(); i<list.size(); i++){
        my_push_back(&output, BodyName(((list[i]).name->value)));
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

    //    list.push_back(body);
    my_push_back(&list, body);

    cout << "Added body to catalog:\n";
    body.print(String("body"), String("\t"), cout);

}


//set the content of *this (not the memory addresses) equal to the content of x
void Catalog::set(const Catalog& x){
    
    my_vector_memcpy<Body>(&list, (x.list));
    
}

