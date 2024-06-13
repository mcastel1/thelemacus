//
//  string.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef string_h
#define string_h

#include "wx/wxprec.h"
#include <wx/gdicmn.h>
#include <wx/window.h>



#include "file_rw.h"


class FileRW;

using namespace std;


class String{
    
public:
    string value;
    
    String();
    String(const string&);
    String(const char&);
    void print(String, bool, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void write_to_file(String, FileRW&, String);
    void set(String, String, String);
    void set(String);
    void set(string);
    void set_to_current_time(void);
    String append(String);
    void appendto(String);
    String prepend(String);
    void replace_to(const char&, const char&);
    String subString(size_t, size_t);
    bool split_file_path(String*, String*, String*, String);
    String filename_without_folder_nor_extension(String);
    bool is_a_file_path(String);
    wxSize get_size(wxWindow*);
    wxSize get_size(wxDC*);
    unsigned int get_length(void);
    vector<String> split(void);
    int position_in_list(const vector<String>&);
    int position_in_list(const wxArrayString&);

    bool operator == (const String&), operator != (const String&);
    
};

#endif
