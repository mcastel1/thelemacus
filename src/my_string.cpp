//
//  my_string.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include <sstream>

#include <boost/filesystem.hpp>

#include "generic.h"
#include "my_string.h"

//default constructor, which sets value to an empty string by default
String::String() {

    value = "";

}

//constructor which sets value to s
String::String(const string& s) {

    value = s;

}

//constructs a string made of the char c
String::String(const char& c) {

    stringstream temp;

    temp.str("");
    temp << c;

    value = (temp.str().c_str());

}


bool String::operator==(const String& s) {

    return((value == (s.value)));

}

bool String::operator!=(const String& s) {

    return(!((*this) == s));

}

//determine whether *this is a file path: if *this contains either '/' or '.' it is a file path (return true), otherwise it is not a file path (return false)
bool String::is_a_file_path([[maybe_unused]] String prefix) {

    return(((value.find("/")) != string::npos) || ((value.find(".")) != string::npos));

}

//return the size of *this if shown in the wxWindow (e.g. a wxtextctr, a wxliscontrol, etc...)
wxSize String::get_size(wxWindow* p) {

    wxClientDC dc(p);

    return (dc.GetTextExtent(wxString(value)));

}

//return the size of this as displayed in the wxDC dc
wxSize String::get_size(wxDC* dc) {

    return (dc->GetTextExtent(wxString(value)));

}


//split *this into multiple strings which were separated by a "\n" into *this, and return a vector containig them
vector<String> String::split(void) {

    size_t pos1, pos2;
    vector<String> result;


    result.resize(0);
    pos1 = 0;
    do {

        pos2 = value.find("\n", pos1);
        my_push_back(&result, (this->subString(pos1, pos2 - pos1)));
        pos1 = pos2 + 1;

    }while(pos2 != ((string::npos)));

    return result;

}


//check whether *this is equal to an element in: if *this is equal to the i-th element in list, retun i. If *this isn't equal to any element in list, return list.size()
//inline 
int String::position_in_list(const vector<String>& list){
    
    unsigned int i;
    
    for(i=0; i<list.size(); i++){
        if((*this) == list[i]){
            break;
        }
    }
    
    return i;
    
}


//checks whether *this is equal to an element in: if *this is equal to the i-th element in list, retun i. If *this isn't equal to any element in list, return list.size()
//inline 
int String::position_in_list(const wxArrayString& list){
    
    unsigned int i;
    
    for(i=0; i<list.size(); i++){
        if(value == list[i]){
            break;
        }
    }
    
    return i;
    
}


//return the number of characters in *this
unsigned int String::get_length(void){
    
    return(((unsigned int)(value.length())));
    
}


//read from stream input_stream the content after 'name = ' and writes it into this
template<class S> void String::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    cout << prefix.value << "Reading " << name.value << " from stream " << input_stream << "... \n";

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

    //read the string after ' = ' until the end of line string and store it into value
    value = line.substr(pos + 3, line.size() - (pos + 3)).c_str();

    cout << prefix.value << "... done.\n";

    print(name, true, prefix, cout);

}

template void String::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);
template void String::read_from_stream<basic_istringstream<char, char_traits<char>, allocator<char>>>(String, basic_istringstream<char, char_traits<char>, allocator<char>>*, bool, String);


//read from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void String::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<String>(this, name, filename, mode, prefix);

}


//writes to file the content of string after 'name = '
void String::write_to_file(String name, FileRW& file, [[maybe_unused]] String prefix) {

    long i;
    string line;
    stringstream s, temp;

    //count the number of lines infile so I kno when to stop when reading it
    file.count_lines(prefix);
    //open the file in mode "in" to read the lines corresponding to "name", which will be then copied and pasted to a new 'file' with the same name
    file.open(String("in"), prefix);


    //rewind the file pointer
    file.value->clear();                 // clear fail and eof bits
    file.value->seekg(0, std::ios::beg); // back to the start!

    for (temp.str(""), i = 0; (i < ((long)(file.number_of_lines))) && (!(*(file.value)).eof()); i++) {

        line.clear();
        getline(*(file.value), line);

        if (((line.find(name.value)) == (string::npos)) /*I ignore comment lines which start with '#'*/ || (line[0] == '#')) {

            //in this case 'name' has not been found in the line under consideration, or the line under consideration is a comment

            //I copy and paste the line that I read from file to temp
            temp << line << "\n";


        }
        else {
            //in this case 'name' has been found in the line under consideration

            //I write into s 'name = [new content of the string that I want to write on file]'
            s.str("");
            s << (name.value) << " = " << value;

            //I write s to file temp
            temp << (s.str()) << "\n";

        }

    }


    //move remove old 'file' and write the content of temp into a new 'file' with the same name
    file.close(prefix);
    boost::filesystem::remove(file.name->value);
    file.open(String("out"), prefix);
    (*(file.value)) << temp.str().c_str();
    file.close(prefix);

    //    boost::filesystem::rename(temp.name.value, file.name.value);

}


//attempt to print the string name to ostr. If print_if_empty the string is printed even if it is empty, while if print_if_empty = false the string is not printed if empty
void String::print(String name, bool print_if_empty, String prefix, ostream& ostr) {

    if (name != String("")) {
        
        if (((*this) != String("")) || print_if_empty) {
            
            ostr << prefix.value << name.value << " = " << value << "\n";
            
        }
        
    }

}


//set the value of String *this, titled name, to input_string, by printing out everything
//inline 
void String::set(String name, String input_string, [[maybe_unused]] String prefix) {

    set(input_string);

    print(name, true, prefix, cout);

}


//set the value of String *this to input_string, without printing out anything
//inline 
void String::set(const String& input_string) {

    value = (input_string.value);

}


void String::set(string input_string) {

    value = input_string;

}


//append s to *this and returns the result
String String::append(String s) {

    String output;
    stringstream temp;

    //append \t to prefix
    temp << value << s.value;

    output.value = temp.str();

    return output;

}


//append s to *this and writes the result in *this
void String::appendto(String s) {

    (*this) = (this->append(s));

}


//prepend s to *this and returns the result
String String::prepend(String s) {

    String output;
    stringstream temp;

    //append \t to prefix
    temp << s.value << value;

    output.value = temp.str();

    return output;

}

//replace all the occurrences of a with b in *this and write the result in *this
void String::replace_to(const char& a, const char& b){
    
//    int l = s.length();
    
    // loop to traverse in the string
    for (unsigned int i = 0; i < get_length(); i++){
        
        // check for a and replace
        if(value[i] == a){
            value[i] = b;
        }
        
    }
    
}

//return substring of *this from start with length length
String String::subString(size_t start, size_t length) {

    return String(value.substr(start, length));

}

//splits the file path *this into the folrder path (with '/' at the end), filename (without extension) and extension part (with no '.'), by writing them into *folder_path (if folder_path != NULL), *filename (if != NULL), and *extension (if != NULL), respectively.
// It returns true/false if the operation could be completed succesfully/not succesfully
bool String::split_file_path(String* folder_path, String* filename, String* extension, [[maybe_unused]] String prefix) {

    //the positions of '/' and of '.' in *this
    size_t slash_position, dot_position;

    cout << prefix.value << "Splitting String" << value << "...\n";

    slash_position = value.find_last_of("/");
    dot_position = value.find_last_of(".");

    if ((slash_position != (string::npos)) && (dot_position != (string::npos))) {

        if (folder_path != NULL) { folder_path->set(String("folder path"), String(value.substr(0, slash_position + 1)), prefix); }
        if (filename != NULL) { filename->set(String("filename"), String(value.substr(slash_position + 1, dot_position - slash_position - 1)), prefix); }
        if (extension != NULL) { extension->set(String("extension"), String(value.substr(dot_position + 1)), prefix); }

        cout << "...done.\n";

        return true;

    }
    else {

        cout << prefix.value << RED << "... String is not valid!\n" << RESET;

        return false;

    }

}

//assuming that output is a filename (including [folder]+ [name of file without folder nor extension] + [extension], return [name of file without folder nor extension]
String String::filename_without_folder_nor_extension([[maybe_unused]] String prefix) {

    String output;

    split_file_path(NULL, &output, NULL, prefix);

    return output;

}



//set *this to the current time
void String::set_to_current_time(void) {

    Time now;

    now.set_current_utc();
    //I write in the non-GUI object *this

    set(String("String set to current time"), String(now.to_string(data_precision.get(), true)), String(""));

}
