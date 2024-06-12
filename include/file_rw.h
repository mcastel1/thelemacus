//
//  file_rw.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef file_rw_h
#define file_rw_h


//a inherited class from File class, for files on disk that can be read and written to
class FileRW: public File{
    
public:
    
    //the strean for reading and writing
    fstream* value;
    String /*the full path of the file, including the folder, filename and extension*/name, /*the path of the folder where the file is located*/folder, /*the file extension, without the '.'*/extension;
    
    
    
    FileRW();
    void set_name(String);
    bool open(String, String);
    void close(String);
    void remove(String);
    void count_lines(String);
    bool check_if_exists(String);
    
    
    
    //    wxDECLARE_EVENT_TABLE();
    
};

#endif
