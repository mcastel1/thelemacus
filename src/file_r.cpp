//
//  file_r.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "file_r.h"
#include "constants.h"


#ifdef _WIN32
#include <sstream>      // std::istringstream
#endif

FileR::FileR() {

#ifdef __APPLE__
    value = new ifstream;
#endif
#ifdef _WIN32
    value = new istringstream;
#endif

    //allocate value

    //set the precision of *value
    value->precision((data_precision.value));

}

//set the name of *this from input. On Win32: if input is a file path, it gets from it the [file name without folder nor extension] and write it
// into name_without_folder_nor_extension, while if input is not a  file path, it writes it directly into name_without_folder_nor_extension
void FileR::set_name(String input) {

#ifdef __APPLE__
    //I am on APPLE -> input = [full file path] -> I set name, folder, name_without_folder_nor_extension and extension

    (name.value) = (input.value);
    name.split_file_path(&folder, name_without_folder_nor_extension, &extension, String(""));

#endif

#ifdef _WIN32
    //I am on WIN32->input = [file name without folder nor extenion] -> I set name_without_folder_nor_extension from input

    if (input.is_a_file_path(String(""))) {

        input.split_file_path(NULL, name_without_folder_nor_extension, NULL, String(""));

    }
    else {

        (name_without_folder_nor_extension->value) = (input.value);

    }

#endif

}

//open the file *this and returns value pointing to it
bool FileR::open([[maybe_unused]] String prefix) {

#ifdef __APPLE__

    value->open(name.value, ios::in);

    cout << prefix.value << "Opening " << (name.value) << " in mode in ... \n";

    if (!value) {

        cout << prefix.value << RED << "... error opening file " << (name.value) << "!\n" << RESET;
        return false;

    }
    else {

        cout << prefix.value << "... done.\n";
        return true;

    }

#endif
#ifdef _WIN32
    //in WIN32 I can open (from the resources incorporated in the .exe file) files in read mode only


    char* bytes;
    HMODULE hModule;
    HRSRC hResource;
    HGLOBAL hMemory;
    DWORD dwSize;
    LPVOID lpAddress;
    LPCWSTR resource_id;
    wstring temp;

    temp = wstring((name_without_folder_nor_extension->value).begin(), (name_without_folder_nor_extension->value).end());

    //the resource id in WIN32 resource file is equal to name_without_folder_nor_extension
    resource_id = (temp.c_str());

    hModule = GetModuleHandle(NULL);
    hResource = FindResource(hModule, resource_id, L"DATA");

    if (hResource != NULL) {
        //the resource has been found

        hMemory = LoadResource(hModule, hResource);
        dwSize = SizeofResource(hModule, hResource);
        lpAddress = LockResource(hMemory);

        bytes = new char[dwSize];
        memcpy(bytes, lpAddress, dwSize);
        value = new istringstream(bytes);

        return true;

    }
    else {
        //the resource has not been found

        cout << prefix.value << RED << "Error opening resource " << (name_without_folder_nor_extension->value) << "!\n" << RESET;

        return false;

    }


#endif


}

void FileR::close(String prefix) {

#ifdef __APPLE__

    value->close();
    cout << prefix.value << "File " << (name.value) << " closed.\n";

#endif
#ifdef _WIN32

    cout << prefix.value << "Close method has been called on a resource file, there is nothing to do.\n";

#endif

}


bool FileR::check_if_exists(String prefix) {

    bool output;

#ifdef __APPLE__
    //on APPLE operating system, *this is a file on disk -> open it to check if exists

    value->open(name.value, ios::in);

    if (value) {

        cout << prefix.value << "File " << (name.value) << " exists.\n";
        value->close();
        output = true;

    }
    else {

        cout << prefix.value << RED << "FileR " << (name.value) << " does not exist!\n" << RESET;
        output = false;

    }

#endif
#ifdef _WIN32
    //on WIN32 operating system, *this is a resrouce -> check the output of FindResource to check whether *this exists

    HMODULE hModule;
    HRSRC hResource;
    LPCWSTR resource_id;
    wstring temp;


    temp = wstring((name_without_folder_nor_extension.value).begin(), (name_without_folder_nor_extension.value).end());
    //the resource id in WIN32 resource file is equal to name_without_folder_nor_extension
    resource_id = (temp.c_str());

    hModule = GetModuleHandle(NULL);
    hResource = FindResource(hModule, resource_id, L"DATA");

    if (hResource != NULL) {

        cout << prefix.value << "FileR " << (name_without_folder_nor_extension.value) << " exists.\n";
        output = true;

    }
    else {

        cout << prefix.value << RED << "FileR " << (name_without_folder_nor_extension.value) << " does not exist!\n" << RESET;
        output = false;

    }


#endif

    return output;

}

//count the number of lines in *this and writes it in number_of_lines
void FileR::count_lines(String prefix) {

    FileR temp;

#ifdef __APPLE__
    //I am on APPLE operating system -> FileR is a file with an actual path -> I set the path of temp equal to that of *this
    temp.set_name(name);
#endif

#ifdef  _WIN32
    //I am on WIN32 operating system -> FileR is a resource -> I set the resource id of temp equal to that of *this
    temp.set_name((*name_without_folder_nor_extension));
#endif

    temp.open(prefix);
    number_of_lines = ((unsigned int)count(istreambuf_iterator<char>((*(temp.value))), istreambuf_iterator<char>(), '\n'));
    temp.close(prefix);

    cout << prefix.value << "Number of lines in file " << (name_without_folder_nor_extension->value) << " = " << number_of_lines << "\n";

}
