//
//  file_rw.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "file_rw.h"
#include "constants.h"


FileRW::FileRW() {

    //allocate value
    value = new fstream;
    name = new String();
    folder = new String();
    extension = new String();
    

    //set the precision of *value
    value->precision((data_precision.value));

}

//set the full path of the file and write it into name
void FileRW::set_name(String path) {

    (name.value) = (path.value);
    name.split_file_path(&folder, &name_without_folder_nor_extension, &extension, String(""));

}

//open the file *this in mode 'mode' and returns value pointing to it
bool FileRW::open(String mode, [[maybe_unused]] String prefix) {

    if (mode == String("in")) {
        value->open(name.value, ios::in);
    }
    else {
        value->open(name.value, ios::out);
    }

    cout << prefix.value << "Opening " << (name.value) << " in mode '" << mode.value << "' ... \n";

    if (!value) {

        cout << prefix.value << RED << "... error opening file " << (name.value) << "!\n" << RESET;
        return false;

    }
    else {

        cout << prefix.value << "... done.\n";
        return true;

    }

}

void FileRW::close(String prefix) {

    value->close();
    cout << prefix.value << "File " << (name.value) << " closed.\n";

}

//delete file *this from disk
void FileRW::remove(String prefix) {

    boost::filesystem::remove(name.value);

    cout << prefix.value << "File " << name.value << " removed\n";

}

void FileRW::count_lines(String prefix) {

    ifstream temp;

    temp.open(name.value);
    number_of_lines = ((unsigned int)count(istreambuf_iterator<char>(temp), istreambuf_iterator<char>(), '\n'));
    temp.close();

    cout << prefix.value << "Number of lines in file " << (name.value) << " = " << number_of_lines << "\n";

}

bool FileRW::check_if_exists(String prefix) {

    bool output;

    value->open(name.value, ios::in);

    if (value) {

        cout << prefix.value << "File " << (name.value) << " exists.\n";
        value->close();
        output = true;

    }
    else {

        cout << prefix.value << RED << "File " << (name.value) << " does not exist!\n" << RESET;
        output = false;

    }

    return output;

}
