//
//  file_r.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef file_r_h
#define file_r_h

//an inherited class from File class, for files that can be read only
class FileR: public File{
    
public:
    
#ifdef __APPLE__
    //I am on APPLE operating system: the file will be read from a path in the .app package and read with the ifstream value (read only) -> I include also ame, folder and extension as members of this class
    ifstream* value;
    String /*the full path of the file, including the folder, filename and extension*/name, /*the path of the folder where the file is located*/folder, /*the file extension, without the '.'*/extension;
    
#endif
#ifdef _WIN32
    //I am on WIN32 operating system: the file will be loaded from a resource incoporeated in the .exe file and read with the istringstream value
    istringstream* value;
#endif
    
    
    FileR();
    void set_name(String);
    bool open(String);
    void close(String);
    bool check_if_exists(String);
    void count_lines(String);
    //istringstream* create_istringstream(String);
    
    //    wxDECLARE_EVENT_TABLE();
    
};

#endif
