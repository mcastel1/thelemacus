//
//  generic.cpp
//  minimal_cocoa
//
//  Created by Michele on 20/06/2024.
//

#include "generic.h"




//given a wxListCtrl* list_control, if * list_control has at least one item, it finds on what element of *list_control the mouse is currently on and writes the id of this element in i. Otherwise, it writes wxNOT_FOUND in *i
void MousePositionOnListControl(wxListCtrl* list_control, int* i){
    
    if((list_control->GetItemCount()) > 0){
        //*list_control has a non-zero size -> write in *i the number of the item on which the mouse is hovering
        
        wxPoint p;
        wxRect r;
        int hit_test_flag;
        
        hit_test_flag = wxLIST_HITTEST_ONITEM;
        
        p = (list_control->ScreenToClient(wxGetMousePosition()));
        
     
#ifdef __APPLE__
        //on APPLE operating system I need to correct the Mouse position in the following way

        //obtain the position of the rectangle of the first visible item in *this and store it in r
        list_control->GetItemRect(list_control->GetTopItem(), r, wxLIST_RECT_BOUNDS);

        //decrease the mouse position with respect to the origin of *this located on the bottom edge of the gray band on the top by r.y on the y axis -> now p is the mouse position with respect to the top-left origin of the white area of *this
        (p.y) -= r.y;
#endif
        
        //store in i the # of the item on which the mouse is hovering
        (*i) = ((int)(list_control->HitTest(p, hit_test_flag)));
        
        //    cout << "\nMouse is on item # " << (*i);
        
    }else{
        //*list_control has zero size -> write in *t wxNOT_FOUND
        
        (*i) = wxNOT_FOUND;
        
    }
    
}


//read from file the content after 'name = ' and writes it into the *object (the element of class C).
// This works for any class C. On WIN32, if mode = 'RW' then filename is filepath  such as "C:/a/b.txt", while if mode = 'R' then filename is a resource name such as my_resource_file.
template<class C> void read_from_file(C* object, String name, String filename, [[maybe_unused]] String mode, [[maybe_unused]] String prefix) {

#ifdef __APPLE__
    //I am on APPLE operating system->the file is located in a folder in the .app package and I read it from there

    FileRW file;

    file.set_name(filename);
    file.open(String("in"), prefix);

    object->template read_from_stream<fstream>(name, file.value, true, prefix);

    file.close(prefix);

#endif

#ifdef _WIN32
    //I am on WIN32 operating system->I create an istringstream that containts the data of file (which is incorporated in the .exe file as a WIN32 resource),  and I read it from there


    if (mode == String("RW")) {
        //I am reading form a FileRW and filename is a file path

        FileRW file;

        file.set_name(filename);
        file.open(String("in"), prefix);

        object->template read_from_stream<fstream>(name, file.value, true, prefix);

        file.close(prefix);

    }
    else {

        if (mode == String("R")) {
            //I am reading from a FileR  and filename is a resource id

            FileR file;

            file.set_name(filename);
            file.open(prefix);

            object->template read_from_stream<istringstream>(name, file.value, true, prefix);

            file.close(prefix);

        }
        else {

            cout << prefix.value << RED << "Cannor read from file beacuse mode is invalid!\n" << RESET;

        }

    }


#endif

}

// explicit instantiations
template void read_from_file<Angle>(Angle*, String, String, String, String);
template void read_from_file<Answer>(Answer*, String, String, String, String);
template void read_from_file<Catalog>(Catalog*, String, String, String, String);
template void read_from_file<Chrono>(Chrono*, String, String, String, String);
template void read_from_file<Double>(Double*, String, String, String, String);
template void read_from_file<Data>(Data*, String, String, String, String);
template void read_from_file<Int>(Int*, String, String, String, String);
template void read_from_file<Length>(Length*, String, String, String, String);
template void read_from_file<Position>(Position*, String, String, String, String);
template void read_from_file<Rotation>(Rotation*, String, String, String, String);
template void read_from_file<Route>(Route*, String, String, String, String);
template void read_from_file<String>(String*, String, String, String, String);



//this function checks whether the  unsigned int in string s is formatted correctly and, if check_interval = true, it also checks whether the this unsigned int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_unsigned_int(string s, unsigned int* i, bool check_interval, unsigned int min, unsigned int sup){
    
    bool check;
    unsigned int j = 0;
    
    check = false;
    
    if(/*here I check that the string s is not empty*/(!s.empty()) && /*here I check whether the quantity entered in s is an unsigned integer, i.e., it contains only the characters 0123456789*/ ((s.find_first_not_of(chars_unsigned_int)) == (std::string::npos))){
        
        j = stoi(s, NULL, 10);
        
        if(check_interval){
            
            check = ((j >= min) && (j < sup));
            
        }else{
            
            check = true;
        }
        
    }else{
        
        check = false;
        
    }
    
    if((i != NULL) && check){
        
        (*i) = j;
        
    }
    
    return check;
    
}



//this function checks whether the int in string s is formatted correctly and, if check_interval = true, it also checks whether the this  int lies in [min, sup). If i != NULL and the check is ok, it also writes the value of the unsigned int read from s into (*i)
bool check_int(string s, int* i, bool check_interval, int min, int sup){
    
    bool check;
    int j = 0;
    
    if(/*here I check whether the quantity entered in s is an integer, i.e., it contains only the characters +-0123456789*/ ((s.find_first_not_of(chars_int)) == (std::string::npos))){
        
        j = stoi(s, NULL, 10);
        
        if(check_interval){
            
            check = ((j >= min) && (j < sup));
            
        }else{
            
            check = true;
            
        }
        
    }else{
        
        check = false;
        
    }
    
    if((i != NULL) && check){
        
        (*i) = j;
        
    }
    
    return check;
    
}


//this function asks the user to enter an unsigned int from keyboard and checks whether the entered value is an unsigned int and, if check_interval = true, that the entered value lies in [min, sup)
void enter_unsigned_int(unsigned int* i, bool check_interval, unsigned int min, unsigned int sup, String name, String prefix){
    
    string s;
    bool check;
    
    do{
        
        s.clear();
        
        cout << prefix.value << "Enter " << name.value << ":";
        getline(cin >> ws, s);
        
        check = check_unsigned_int(s, i, check_interval, min, sup);
        
        if(!check){
            
            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
            
        }
        
    }while(!check);
    
}





//this function checks whether the double in string s is formatted correctly and, if check_interval = true, it also checks whether the this double lies in [min, sup). If x != NULL and the check is ok, it also writes the value of the double read from s into (*x)
bool check_double(string s, double* x, bool check_interval, double min, double sup){
    
    bool check, /*it is true if s can be converted to double, false otherwise*/ can_convert_to_double;
    double y = 0.0;
    
    check = false;
    can_convert_to_double =
    /*check that s is not empty*/ (!s.empty())
    && /*check that s contains at least one numerical character (i.e. at least one character in chars_unsigned_int)*/ ((s.find_first_of(chars_unsigned_int)) != (std::string::npos))
    && /*check that s contains only the allowed characters in chars_double*/ (((s.find_first_not_of(chars_double)) == (std::string::npos)))
    && /*check that '.' occurs zero or one time*/ (count(s.begin(), s.end(), '.') <= 1)
    && /*check that '+' occurs zero or one time*/ (count(s.begin(), s.end(), '+') <= 1)
    && /*check that '-' occurs zero or one time*/ (count(s.begin(), s.end(), '-') <= 1);
    
    if(can_convert_to_double){
        
        //if the check above passed, then I proceed and write s into y
        y = stod(s);
        
        if(check_interval){
            
            check = ((y >= min) && (y < sup));
            
        }else{
            
            check = true;
            
        }
        
    }else{
        
        check = false;
        
    }
    
    //if x == NULL, then this function is meant to be used to check the correct format of s only, not to write its value to x.
    if((x != NULL) && check){
        
        (*x) = y;
        
    }
    
    return check;
    
}


//this function asks the user to enter a double from keyboard and checks whether the entered value contains the allowed chars for double and, if check_interval = true, that the entered value lies in [min, sup)
void enter_double(double* x, bool check_interval, double min, double sup, String name, String prefix){
    
    string s;
    bool check;
    
    do{
        
        s.clear();
        
        cout << prefix.value << "Enter " << name.value << ":";
        getline(cin >> ws, s);
        
        check = check_double(s, x, check_interval, min, sup);
        
        if(!check){
            
            cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
            
        }
        
    }while(!check);
    
}


//find the  position in v of element with value x and return the position. If no element is found, return v.size(). Note that this function is different from address_position_in_vector
template<class T> unsigned int position_in_vector(T x, const vector<T>& v){
    
    unsigned int i;
    
    for (i=0; (i<(v.size())) && (x != v[i]); i++){}
    
    return i;
    
}

//explicit instantiation
template unsigned int position_in_vector<Projection>(Projection, const vector<Projection>&);



//convert element by element a vector whose entries are of type A into a vector whose entries are of type B and return the latter. This make sense if A can be re-casted into B
template<class A, class B> vector<B> convert_vector(const vector<A>& x){
    
    vector<B> output(x.size());
    
    for(unsigned int i=0; i<x.size(); i++){
        output[i] = ((B)x[i]);
    }
    
    return output;
    
}

template std::__1::vector<String, std::__1::allocator<String>> convert_vector<Projection, String>(std::__1::vector<Projection, std::__1::allocator<Projection>> const&);


//find the  position in v of element with memory address x and return the position. If no element is found, return v.size(). Note that this function is different from position_in_vector
template<class T> unsigned int address_position_in_vector(T* x, const vector<T>& v){
    
    unsigned int i;
    
    for (i=0; (i<(v.size())) && (x != (v.data() + i)); i++) {}
    
    return i;
    
}

template unsigned int address_position_in_vector<Route>(Route*, std::__1::vector<Route, std::__1::allocator<Route>> const&);
template unsigned int address_position_in_vector<Position>(Position*, std::__1::vector<Position, std::__1::allocator<Position>> const&);


//delete duplicates from vector *v by removing entries in *v which are equal
template<class T> void delete_duplicates(vector<T>* v){
    
    sort(v->begin(), v->end());
    v->erase(unique(v->begin(), v->end()), v->end());
    
}

// explicit instantiations
template void delete_duplicates<unsigned long long>(vector<unsigned long long>*);


//return true(false) if the relative difference between a and b is smaller, in absolute value, than epsilon_double. If the relative difference cannot be compute because it would imply dividing by zero, return a.value == b.value
template<class T> bool equal_rel_epsilon_double(const T& a, const T& b){
    
    if((a.value)+(b.value) != 0.0){
        
        return (fabs(((a.value) - (b.value))/(((a.value) + (b.value))/2.0)) < epsilon_double);
       
    }else{
        
        return ((a.value) == (b.value));
        
    }
    
}

//explicit instantiation
template bool equal_rel_epsilon_double<Angle>(const Angle&, const Angle&);


/*
 delete duplicates from vector *v, by removing entries in *v which are equal according to the comparator. A pointer to the comparator needs to be supplied as second argument. For example, if I define a comparator such as
 
 template<class T> bool compare_epsilon(const T& a, const T& b){
     
     if(fabs(a.value - b.value) < 0.1){return true;}
     else{return false;}
     
 }
 
 then I call this method with     delete_duplicates(&t, &compare_epsilon);

 
 */
template<class T> void delete_duplicates(vector<T>* v, bool (*comparator)(const T&, const T&)){
    
    sort(v->begin(), v->end());
    v->erase(unique(v->begin(), v->end(), comparator), v->end());
    
}

// explicit instantiations
template  void delete_duplicates<Angle>(vector<Angle>*, bool (*)(const Angle&, const Angle&));



//return the size of *this if shown in the wxWindow (e.g. a wxtextctr, a wxliscontrol, etc...). This is equivalent to the method String::get_size(const String&, wxWindow*)
wxSize get_size(const String& s, wxWindow* p) {

    wxClientDC dc(p);

    return (dc.GetTextExtent(wxString(s.value)));

}


//return the size of this as displayed in the wxDC dc. This is equivalent to the method String::get_size(const String&, wxDC*)
wxSize get_size(const String& s, wxDC* dc) {

    return (dc->GetTextExtent(wxString(s.value)));

}




string to_string(const Position& p, unsigned int precision) {

    Position temp;
    
    temp = p;
    
    return (temp.to_string(precision));

}




//sets fore/background colors to a generic object of type T
template<class T> void SetColor(T* object) {

    object->SetForegroundColour(wxGetApp().foreground_color);
    object->SetBackgroundColour(wxGetApp().background_color);

}

//I reset control by storing its current value into temp, resetting its list of items, and resetting its value from temp
template <class T> void Reset(T* control) {

    int i;
    wxString value;
    wxArrayString list;

    value = (control->GetValue());
    for (i = 0; i < ((int)(control->GetCount())); i++) {
        list.Add(control->GetString(i), 1);
    }

    control->Set(list);
    control->SetValue(value);

}

template void Reset<wxComboBox>(wxComboBox*);



//rescales *image to fit into size, by including the border given by length_border_over_length_screen, and by keeping its proprtions, and writes the result into *image
wxImage RescaleProportionally(wxImage image, const wxSize size) {

    wxSize original_size, size_minus_margins;
    wxImage output;
    Double scaling_factor;

    output = image;

    original_size = (output.GetSize());
    size_minus_margins = wxSize((size.GetWidth()) - ((wxGetApp().border).value), (size.GetHeight()) - ((wxGetApp().border).value));

    scaling_factor.set(String(""),
        (
            ((original_size.GetWidth()) > (original_size.GetHeight()))
            ? ((double)((size_minus_margins.GetWidth())) / ((double)(original_size.GetWidth())))
            : ((double)((size_minus_margins.GetHeight())) / ((double)(original_size.GetHeight())))
            )
        ,
        String("")
    );

    output.Rescale(
        ((int)(((double)(original_size.GetWidth())) * (scaling_factor.value))),
        ((int)(((double)(original_size.GetHeight())) * (scaling_factor.value))),
        wxIMAGE_QUALITY_HIGH
    );

    return output;

}



//reads from file the content after 'name = ' and returns it. If mode = 'RW' ('R') it reads from a FileRW (FileR)
String read_from_file(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    String temp;

    temp.read_from_file_to(name, filename, mode, prefix);

    return temp;

}


void my_cout(int n, ...){
    
    unsigned int i;
    va_list args;
    
    va_start(args, n);
    
    for(i=0; i<n; ++i){
        
        cout << "read argument = " << va_arg(args, int) << endl;
        
    }

    va_end(args);

    
}




bool operator < (const Length& l, const Length& s) {
    
    
    if((l.unit.value) == (s.unit.value)){
        //l and s have the same units -> just compare their values
        
        return(((l.value) < (s.value)));
        
    }else{
        //l and s have different units -> convert l to the units of s and compare
        
        Length temp;
        
        temp = l;
        temp.convert_to(s.unit);
        
        return((temp.value) < (s.value));

    }

}

//read a string of the form 'name = 3 4 12 34241 12 ' in input_stream and stores 3 in result[0], 4 in result[1], etc ... Search (do no search) the entire stream if search_entire_stream = true (false). Clear *result and re-allocate it
template<class S> void read_list_from_stream(String name, S* input_stream, bool search_entire_stream, vector<int>* result) {

    size_t pos_a, pos_b;
    String temp;

    result->clear();

    temp.read_from_stream(name, input_stream, search_entire_stream, String(""));
    pos_a = 0;
    do {

        pos_b = temp.value.find(" ", pos_a);
        result->push_back(stoi(temp.subString(pos_a, pos_b - pos_a).value, NULL, 10));

        pos_a = pos_b + 1;

    } while (pos_b != string::npos);

}

template void read_list_from_stream<std::__1::basic_fstream<char, std::__1::char_traits<char>>>(String, std::__1::basic_fstream<char, std::__1::char_traits<char>>*, bool, std::__1::vector<int, std::__1::allocator<int>>*);


bool operator < (const Angle& x, const Angle& y) {

    return(((x.value) < (y.value)));

}


bool operator < (const Angle& x, const double& y) {

    return(((x.value) < y));

}


//this function adjusts the width of a wxComboBox according to its largest entry
void AdjustWidth(wxComboBox* control) {

    unsigned int i;
    int max_width, width, additional;

    //this is the additional width occupied by the sqare with the arrow
    control->GetTextExtent(wxString("-----"), &additional, NULL);

    for (max_width = 0, i = 0; i < (control->GetCount()); i++) {

        control->GetTextExtent(control->GetString(i), &width, NULL);

        if (width > max_width) {
            max_width = width;
        }

    }

    control->SetMinSize(wxSize(max_width + additional, -1));

}
