//
//  generic.cpp
//  minimal_cocoa
//
//  Created by Michele on 20/06/2024.
//

#include "generic.h"
#include "listcontrol_type.h"


#ifdef _WIN32
#include "file_r.h"
#endif


//given a wxListCtrl* list_control, if * list_control has at least one item, it finds on what element of *list_control the mouse is currently on and writes the id of this element in i. Otherwise, it writes wxNOT_FOUND in *i
void MousePositionOnListControl(wxListCtrl* list_control, Int* i){
    
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
        i->set(((int)(list_control->HitTest(p, hit_test_flag))));
        
        //    cout << "\nMouse is on item # " << (*i);
        
    }else{
        //*list_control has zero size -> write in *t wxNOT_FOUND
        
        i->set(wxNOT_FOUND);
        
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
template unsigned int position_in_vector<String>(String, vector<String, allocator<String>> const&);
template unsigned int position_in_vector<LengthUnit>(LengthUnit, vector<LengthUnit, allocator<LengthUnit> > const&);
template unsigned int position_in_vector<RouteType>(RouteType, vector<RouteType, allocator<RouteType> > const&);
template unsigned int position_in_vector<SpeedUnit>(SpeedUnit, vector<SpeedUnit, allocator<SpeedUnit> > const&);
template unsigned int position_in_vector<ListControlType>(ListControlType, vector<ListControlType, allocator<ListControlType> > const&);
template unsigned int position_in_vector<BodyName>(BodyName, vector<BodyName, allocator<BodyName> > const&);


//convert element by element a vector whose entries are of type A into a vector whose entries are of type B and return the latter. This make sense if A can be re-casted into B
template<class A, class B> vector<B> convert_vector(const vector<A>& x){
    
    vector<B> output(x.size());
    
    for(unsigned int i=0; i<x.size(); i++){
        output[i] = ((B)x[i]);
    }
    
    return output;
    
}

template vector<String, allocator<String>> convert_vector<Projection, String>(vector<Projection, allocator<Projection>> const&);


//find the  position in v of element with memory address x and return the position. If no element is found, return v.size(). Note that this function is different from position_in_vector
template<class T> unsigned int address_position_in_vector(T* x, const vector<T>& v){
    
    unsigned int i;
    
    for (i=0; (i<(v.size())) && (x != (v.data() + i)); i++) {}
    
    return i;
    
}

template unsigned int address_position_in_vector<Route>(Route*, vector<Route, allocator<Route>> const&);
template unsigned int address_position_in_vector<Position>(Position*, vector<Position, allocator<Position>> const&);


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
    size_minus_margins = wxSize((size.GetWidth()) - (wxGetApp().border.get()), (size.GetHeight()) - (wxGetApp().border.get()));

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


//void my_cout(int n, ...){
//    
//    unsigned int i;
//    va_list args;
//    
//    va_start(args, n);
//    
//    for(i=0; i<n; ++i){
//        
//        cout << "read argument = " << va_arg(args, int) << endl;
//        
//    }
//
//    va_end(args);
//
//    
//}




bool operator < (const Length& l, const Length& s) {
    
    
    if((l.unit->value) == (s.unit->value)){
        //l and s have the same units -> just compare their values
        
        return(((l.value) < (s.value)));
        
    }else{
        //l and s have different units -> convert l to the units of s and compare
        
        Length temp;
        
        temp = l;
        temp.convert_to((*(s.unit)));
        
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

template void read_list_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, vector<int, allocator<int>>*);
template void read_list_from_stream<basic_istringstream<char, char_traits<char>, allocator<char>>>(String, basic_istringstream<char, char_traits<char>, allocator<char>>*, bool, vector<int, allocator<int>>*);


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



//inline
double cos(const Angle& x) {

    return cos(x.value);

}


//inline
double cot(double x){
    
    return 1.0/tan(x);
    
}

//inline
double csc(double x){
    
    return 1.0/sin(x);
    
}

//returns alpha = arctan(y/x), with alpha lying in the same quadrant as the vector x, y
//inline
double atan(double x, double y){
    
    if(x != 0.0){
        
        if(x > 0.0){
            
            return atan(y/x);
            
        }else{
            
            return(atan(y/x)+M_PI);
        }
        
    }else{
        
        if(y > 0.0){
            
            return(M_PI/2.0);
            
        }else{
            
            return(3.0*M_PI/2.0);
            
        }
        
    }
    
}

//the floor of the exponential of x
//inline
int floor_exp(double x){
    
    return(floor(exp(x)));
    
}


//round the floating point number x with precision `precision`
double round_with_precision(double x, unsigned int precision){
    
    return round(x * gsl_pow_int(10.0, precision)) / gsl_pow_int(10.0, precision);
    
}


//inline
double sin(const Angle& x) {

    return sin(x.value);

}

//inline
double asin(const Double& x) {

    return asin(x.value);

}


//inline
double tan(const Angle& x) {

    return tan(x.value);

}

//inline
double csc(const Angle& x) {

    return csc(x.value);

}

//inline
double sec(const Angle& x) {

    return (1.0 / cos(x));

}

//inline
double cot(const Angle& x) {

    return cot(x.value);

}

//inline
double acos(const Double& x) {

    return acos(x.value);

}


//put the angle x in the interval [-pi, pi), it does not alter *this and returns the result. This is equivalent to Angle::normalize_pm_pi_ret
//inline
Angle normalize_pm_pi_ret(const Angle& x){
        
    Angle temp;

    temp = x;

    return(temp.normalize_pm_pi_ret());

}


//return the mean between a.value and b.value
double mean_value(Angle& a, Angle& b){
    
    return(((a.value) + (b.value))/2.0);
    
}


//normalize a and b between -pi and pi, and return  the algebraic mean between a.value and b.value
double mean_pm_pi(Angle& a, Angle& b){
    
    return(((a.normalize_pm_pi_ret().value) + (b.normalize_pm_pi_ret().value))/2.0);
    
}

//retun the longitude in the middle of a rectangle which has one N/S side at longitude a, and another N/S side at longitude b, and write the result in *result
void mean_longitude(Angle& a, Angle& b, Angle* result){
    
    Angle a_, b_;
    
    a_ = a.normalize_pm_pi_ret();
    b_ = b.normalize_pm_pi_ret();
    
    result->set(mean_value(a_, b_));
    
    if((a_.value) * (b_.value) < 0.0){
        //a and b lie in different hemispheres
        
        if(fabs((a_.value)-(b_.value)) > M_PI){
            (*result) += M_PI;
        }
        
    }
    
}

//push back the *content* (not the memory adresses) of x into v. This methods calls the set() method of class T, which needs to be defined. This method requires v to be allocated (but *v may have size 0)
template<class T> void my_push_back(vector<T>* v, const T& x){
    
    v->resize((v->size())+1);
    v->back().set(x);
    
}

template void my_push_back<Body>(vector<Body, allocator<Body>>*, Body const&);
template void my_push_back<Route>(vector<Route, allocator<Route>>*, Route const&);
template void my_push_back<String>(vector<String, allocator<String>>*, String const&);
template void my_push_back<Sight>(vector<Sight, allocator<Sight>>*, Sight const&);
template void my_push_back<Position>(vector<Position, allocator<Position>>*, Position const&);
template void my_push_back<PositionProjection>(vector<PositionProjection, allocator<PositionProjection>>*, PositionProjection const&);
template void my_push_back<Angle>(vector<Angle, allocator<Angle>>*, Angle const&);
template void my_push_back<BodyName>(vector<BodyName, allocator<BodyName> >*, BodyName const&);


//copy the *content* (not the memory adresses) of source into dest. This methods calls the set() method of class T, which needs to be defined.
template<class T> void my_vector_memcpy(vector<T>* dest, const vector<T> source){
    
    dest->resize(source.size());
    
    for(unsigned int i=0; i<(dest->size()); i++){
        
        ((*dest)[i]).set(source[i]);
        
    }
    
}

template void my_vector_memcpy<Body>(vector<Body, allocator<Body>>*, vector<Body, allocator<Body>>);
template void my_vector_memcpy<String>(vector<String, allocator<String>>*, vector<String, allocator<String>>);


//compute the cross product between the three-dimensional vectors a and b, and write the result into c, which is cleared and re-allocated. It returs true if the size of both a and b is 3, and false otherwise. If false is returned, r is not touched.
//inline
bool my_cross(const gsl_vector* a, const gsl_vector* b, gsl_vector** r) {

    if (((a->size) == 3) && ((b->size) == 3)) {

        if (((*r) != NULL) && (((*r)->size) != 0)) {

            gsl_vector_free(*r);

        }

        (*r) = gsl_vector_alloc(3);



        gsl_vector_set(*r, 0, gsl_vector_get(a, 1) * gsl_vector_get(b, 2) - gsl_vector_get(a, 2) * gsl_vector_get(b, 1));
        gsl_vector_set(*r, 1, gsl_vector_get(a, 2) * gsl_vector_get(b, 0) - gsl_vector_get(a, 0) * gsl_vector_get(b, 2));
        gsl_vector_set(*r, 2, gsl_vector_get(a, 0) * gsl_vector_get(b, 1) - gsl_vector_get(a, 1) * gsl_vector_get(b, 0));


        return true;

    }
    else {

        return false;

    }


}


//checks whether s is present into wxArrayString, and writes true/false into check if its present/absent. If i!=NULL: if it is present, it writes the position of s in v in *i, if it is not present, i is not touched
//inline
void find_and_replace_case_insensitive(wxComboBox* control, wxArrayString v, bool* check, unsigned int* i) {

    unsigned int j;

    for ((*check) = false, j = 0; (j < v.size()) && (!(*check)); j++) {

        if ((bool)(boost::iequals((control->GetValue()).ToStdString(), (v[j]).ToStdString()))) {

            (*check) = true;
            //I write in control the proper text value (i.e. with the correct upper/lower case, as taken from the list v)
            control->SetValue(v[j]);

        }

    }

    if (i != NULL) { (*i) = j - 1; }

}


//return the longitude value (expressed in radians, positive towards W) of the inverse spherical Mercator projection from the rectangular x value
//inline
double lambda_mercator(double x){
    
    return (-x - two_M_PI*floor((x+M_PI)/two_M_PI));
    
}

//return the latitude value (expressed in radians) of the inverse spherical Mercator projection from the rectangular y value
//inline
double phi_mercator(double y){
    
    return(atan(sinh(y)));
    
}


//constuct a wxImage for the splash screen of the app. Doesn't work well for alpha compositing
wxImage alphaToBlackAndWhiteMask (wxImage img) {

    // Some image types (e.g. gif) have a mask for the transparent pixels
    // instead of an alpha channel.
    if (!img.HasAlpha() && img.HasMask()) {
        img.InitAlpha();
    }

    if (img.HasAlpha()) {
        unsigned char *rgb = img.GetData();
        unsigned char *alpha = img.GetAlpha();

        for (int x = 0, y = 0; x < img.GetWidth()*img.GetHeight()*3; x+=3, y++) {
            // If alpha pixel, make white. Else make black.
            // An alpha value of 0 corresponds to a transparent pixel (null opacity)
            // while a value of 255 means that the pixel is 100% opaque.
            if (alpha[y] < 20) { // Using some threshold for almost transparent pixels.
                rgb[x] = 0xff;// red
                rgb[x+1] = 0xff;// green
                rgb[x+2] = 0xff;// blue
            } else {
                rgb[x] = 0x00;// red
                rgb[x+1] = 0x00;// green
                rgb[x+2] = 0x00;// blue
            }
        }

        // Remove alpha channel so that the pixels turned white will show
        img.ClearAlpha();
    }

    return img;
}


//read the image in path in WIN32 resources from a raw data file written in the resources, and load it into *image. Return true if the image has been loaded correctly, false otherwise
bool read_image_from_resource_data(String path, wxImage* image){
    

#ifdef __APPLE__

    
    return (image->LoadFile(wxString(path.value), wxBITMAP_TYPE_GIF, -1));

    
#endif
    
#ifdef _WIN32

    
    const void* data = NULL;
    size_t size;
    
    if (!wxLoadUserResource(&data, &size, path.filename_without_folder_nor_extension(String("")).value, L"MYDATA") ) {
        
        return false;
        
    }else{
        // Use the data in any way, for example:
        wxMemoryInputStream input_stream(data, size);
        
        return(image->LoadFile(input_stream, wxBITMAP_TYPE_GIF));
        
    }
    
#endif
    
}
