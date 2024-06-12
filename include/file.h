//
//  file.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef file_h
#define file_h

#include "string.h"

//a general class for File objects
class File{
    
public:
    
    String  /*the name of the file without the folder path (before it) and without the file extension (after it)*/name_without_folder_nor_extension;
    unsigned int number_of_lines;
    
    File();
    
};

#endif
