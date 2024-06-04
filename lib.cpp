//example of variadic function: it takes a non-variadic argument n and then an arbitrary number of arguments `...`
void my_cout(int n, ...){
    
    unsigned int i;
    va_list args;
    
    va_start(args, n);
    
    for(i=0; i<n; ++i){
        
        cout << "read argument = " << va_arg(args, int) << endl;
        
    }

    va_end(args);

    
}



//round the floating point number x with precision `precision`
inline double round_with_precision(double x, unsigned int precision)
{
    return round(x * gsl_pow_int(10.0, precision)) / gsl_pow_int(10.0, precision);
}

inline double sin(const Angle& x) {

    return sin(x.value);

}

inline double asin(const Double& x) {

    return asin(x.value);

}

inline double cos(const Angle& x) {

    return cos(x.value);

}

inline double tan(const Angle& x) {

    return tan(x.value);

}

inline double csc(const Angle& x) {

    return csc(x.value);

}

inline double sec(const Angle& x) {

    return (1.0 / cos(x));

}

inline double cot(const Angle& x) {

    return cot(x.value);

}

inline double acos(const Double& x) {

    return acos(x.value);

}

//find the  position in v of element with value x and return the position. If no element is found, return v.size(). Note that this function is different from address_position_in_vector
template<class T> unsigned int position_in_vector(T x, const vector<T>& v){
    
    unsigned int i;
    
    for (i=0; (i<(v.size())) && (x != v[i]); i++){}
    
    return i;
    
}


//convert element by element a vector whose entries are of type A into a vector whose entries are of type B and return the latter. This make sense if A can be re-casted into B
template<class A, class B> vector<B> convert_vector(const vector<A>& x){
    
    vector<B> output(x.size());
    
    for(unsigned int i=0; i<x.size(); i++){
        output[i] = ((B)x[i]);
    }
    
    return output;
    
}

//find the  position in v of element with memory address x and return the position. If no element is found, return v.size(). Note that this function is different from position_in_vector
template<class T> unsigned int address_position_in_vector(T* x, const vector<T>& v){
    
    unsigned int i;
    
    for (i=0; (i<(v.size())) && (x != (v.data() + i)); i++) {}
    
    return i;
    
}



//delete duplicates from vector *v by removing entries in *v which are equal
template<class T> void delete_duplicates(vector<T>* v){
    
    sort(v->begin(), v->end());
    v->erase(unique(v->begin(), v->end()), v->end());
    
}


//return true(false) if the relative difference between a and b is smaller, in absolute value, than epsilon_double. If the relative difference cannot be compute because it would imply dividing by zero, return a.value == b.value
template<class T> bool equal_rel_epsilon_double(const T& a, const T& b){
    
    if((a.value)+(b.value) != 0.0){
        
        return (fabs(((a.value) - (b.value))/(((a.value) + (b.value))/2.0)) < epsilon_double);
       
    }else{
        
        return ((a.value) == (b.value));
        
    }
    
}

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


//return the size of *this if shown in the wxWindow (e.g. a wxtextctr, a wxliscontrol, etc...). This is equivalent to the method String::get_size(const String&, wxWindow*)
wxSize get_size(const String& s, wxWindow* p) {

    wxClientDC dc(p);

    return (dc.GetTextExtent(wxString(s.value)));

}


//return the size of this as displayed in the wxDC dc. This is equivalent to the method String::get_size(const String&, wxDC*)
wxSize get_size(const String& s, wxDC* dc) {

    return (dc->GetTextExtent(wxString(s.value)));

}


//put the angle x in the interval [-pi, pi), it does not alter *this and returns the result. This is equivalent to Angle::normalize_pm_pi_ret
inline Angle normalize_pm_pi_ret(const Angle& x){
        
    Angle temp;

    temp = x;

    return(temp.normalize_pm_pi_ret());

}


//normalize a and b between -pi and pi, and return  the algebraic mean between a.value and b.value
inline double mean_pm_pi(Angle& a, Angle& b){
    
    return(((a.normalize_pm_pi_ret().value) + (b.normalize_pm_pi_ret().value))/2.0);
    
}


string to_string(const Position& p, unsigned int precision) {

    Position temp;
    
    temp = p;
    
    return (temp.to_string(precision));

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


//compute the cross product between the three-dimensional vectors a and b, and write the result into c, which is cleared and re-allocated. It returs true if the size of both a and b is 3, and false otherwise. If false is returned, r is not touched.
inline bool my_cross(const gsl_vector* a, const gsl_vector* b, gsl_vector** r) {

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
inline void find_and_replace_case_insensitive(wxComboBox* control, wxArrayString v, bool* check, unsigned int* i) {

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

//sets fore/background colors to a generic object of type T
template<class T> void SetColor(T* object) {

    object->SetForegroundColour((wxGetApp()).foreground_color);
    object->SetBackgroundColour((wxGetApp()).background_color);

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

bool String::operator==(const String& s) {

    return((value == (s.value)));

}

bool String::operator!=(const String& s) {

    return(!((*this) == s));

}

//reads from file the content after 'name = ' and returns it. If mode = 'RW' ('R') it reads from a FileRW (FileR)
String read_from_file(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    String temp;

    temp.read_from_file_to(name, filename, mode, prefix);

    return temp;

}


//set value equal to i
inline void Int::set(String name, int i, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    value = i;

    if (name != String("")) { print(name, prefix, cout); }

}


//same as Int::set(String name, int i, [[maybe_unused]] String prefix) {, but without printing out anything
inline void Int::set(int i) {

    value = i;

}


//round up *this in such a way that when it is prnted out, only precision.value signifiant digits are shown
void Int::my_round(Int precision){
    
    unsigned int n_digits, rounding_coefficient;
    
    //compute the number of digits of *this
    n_digits = round(log10(value)) + 1;
    
    //round up *this in such a way that only precision digits will be displayed in *this
    rounding_coefficient = gsl_pow_int(10.0, n_digits - precision.value);
    
    value = round((((double)value)/rounding_coefficient)) * rounding_coefficient;
    
}


//convert *this to String by adding a space every three digits to improve readability and return the result
String Int::to_string_spaces(void){
    
    unsigned int i;
    stringstream s;
    String output;
    
    s.str("");
    s << value;
    
    output = String("");
    
    for(i=0; i<s.str().length(); i++){
        
        
        output.appendto(String(s.str()[i]));

        if(((s.str().length()-1 - i) > 0) && (((s.str().length()-1 - i) % 3) == 0)){
            
            output.appendto(String(" "));
            
        }
        
    }
    
    return output;
    
}


bool Int::operator == (const Int& i) {

    return (value == (i.value));

}

bool Int::operator != (const Int& i) {

    return (!((*this) == i));

}

bool Int::operator == (const int& i) {

    return (value == i);

}

bool Int::operator != (const int& i) {

    return (!((*this) == i));

}

bool Int::operator > (const Int& i) {

    return(value > (i.value));

}

bool Int::operator > (const int& i) {

    return(value > i);

}


File::File() {

    //    //allocate value
    //    value = new fstream;
    //    
    //    value->precision((data_precision.value));

}


FileRW::FileRW() {

    //allocate value
    value = new fstream;

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
    name.split_file_path(&folder, &name_without_folder_nor_extension, &extension, String(""));

#endif

#ifdef _WIN32
    //I am on WIN32->input = [file name without folder nor extenion] -> I set name_without_folder_nor_extension from input

    if (input.is_a_file_path(String(""))) {

        input.split_file_path(NULL, &name_without_folder_nor_extension, NULL, String(""));

    }
    else {

        (name_without_folder_nor_extension.value) = (input.value);

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

    temp = wstring((name_without_folder_nor_extension.value).begin(), (name_without_folder_nor_extension.value).end());

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

        cout << prefix.value << RED << "Error opening resource " << (name_without_folder_nor_extension.value) << "!\n" << RESET;

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
    temp.set_name(name_without_folder_nor_extension);
#endif

    temp.open(prefix);
    number_of_lines = ((unsigned int)count(istreambuf_iterator<char>((*(temp.value))), istreambuf_iterator<char>(), '\n'));
    temp.close(prefix);

    cout << prefix.value << "Number of lines in file " << (name_without_folder_nor_extension.value) << " = " << number_of_lines << "\n";

}


//empty constructor
inline Double::Double(void){}

//constructor that sets value to  x
inline Double::Double(const double &x){
    
    value = x;
    
}


void Double::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    value = x;

    if (name != String("")) { print(name, prefix, cout); }

}


//returns true if *this is equal to y within the numerical accuracy accuracy_equal_approx
bool Double::equal_approx(Double x) {

    bool check;
    unsigned int i;
    uint64_t value_uint64_t, xvalue_uint64_t;


    memcpy(&value_uint64_t, &value, 8);
    memcpy(&xvalue_uint64_t, &(x.value), 8);


    for (check = true, i = 0; i < 52; i++) {
        check &= (((value_uint64_t >> i) & one_uint64_t) & ((xvalue_uint64_t >> i) & one_uint64_t));
    }

    return check;


}

template<class S> void Double::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));


    }
    else {

        line.clear();
        getline((*input_stream), line);

    }


    pos = line.find(" = ");

    //read the string after ' = ' until the end of line string and store it into value
    value = stod(line.substr(pos + 3, line.size() - (pos + 3)).c_str(), NULL);

    print(name, prefix, cout);

}


//reads from file the content after 'name = ' and writes it into this. This function opens a new file, sets its name to filename and opens it
void Double::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Double>(this, name, filename, mode, prefix);


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
        result.push_back((this->subString(pos1, pos2 - pos1)));
        pos1 = pos2 + 1;

    } while (pos2 != ((string::npos)));

    return result;

}


//checks whether *this is equal to an element in: if *this is equal to the i-th element in list, retun i. If *this isn't equal to any element in list, return list.size()
inline int String::position_in_list(const vector<String>& list){
    
    unsigned int i;
    
    for(i=0; i<list.size(); i++){
        if((*this) == list[i]){
            break;
        }
    }
    
    return i;
    
}


//checks whether *this is equal to an element in: if *this is equal to the i-th element in list, retun i. If *this isn't equal to any element in list, return list.size()
inline int String::position_in_list(const wxArrayString& list){
    
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


void Double::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}

Double Double::operator + (const Double& x) {

    Double s;

    (s.value) = value + (x.value);

    return s;

}


bool Double::operator < (const Double& x){
    
    return (value < x.value);
    
}

bool Double::operator < (const double& x){
    
    return (value < x);
    
}

bool Double::operator > (const Double& x){
    
    return (value > x.value);
    
}

bool Double::operator > (const double& x){
    
    return (value > x);
    
}

//reads an Int from File file, which must be already open, and it search the file from the beginning if search_entire_stream = true, does not search the file from the beginning otherwise. Writes the result in *this
template<class S> void Int::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));


    }
    else {

        line.clear();
        getline((*input_stream), line);

    }


    pos = line.find(" = ");

    //read the string after ' = ' until the end of line string and store it into value
    value = stoi(line.substr(pos + 3, line.size() - (pos + 3)).c_str(), NULL);

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);


}

//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Int::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Int>(this, name, filename, mode, prefix);

}


void Int::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}

inline bool Length::operator == (const Length& length) {

    return (value == (length.value));

}

inline bool Length::operator != (const Length& length) {

    return (!((*this) == length));

}

Length& Length::operator += (const Length& length) {

    value += (length.value);

    return (*this);

}

Length& Length::operator -= (const Length& length) {

    value -= (length.value);

    return (*this);

}

//evaluates whether Length (*this) is larger than r
inline bool Length::operator > (const Length& r) {

    return((value > (r.value)));

}

bool operator < (const Length& l, const Length& s) {

    return((l.value) < (s.value));

}

//evaluates whether Length (*this) is <= r
inline bool Length::operator <= (const Length& r) {

    return(!((*this) > r));

}

//evaluates whether Length (*this) is >= r
inline bool Length::operator >= (const Length& r) {

    return(!((*this) < r));

}

//evaluates whether Length (*this) is > than the double r
inline bool Length::operator > (const double& r) {

    return((value > r));

}

//evaluates whether Length (*this) is >= than the double r
inline bool Length::operator >= (const double& r) {

    return(!((*this) < r));

}

//evaluates whether Length (*this) is smaller than the double r
inline bool Length::operator < (const double& r) {

    return((value < r));

}


Length Length::operator + (const Length& l) {

    Length s;

    (s.value) = value + (l.value);

    return s;

}

Length Length::operator - (const Length& l) {

    Length s;

    if (value >= (l.value)) {

        (s.value) = value - (l.value);

    }
    else {

        cout << RED << "Length resulting from substraction is negative!\n" << RESET;

        (s.value) = 0.0;

    }

    return s;

}


Length Length::operator * (const double& x) {

    Length s;

    (s.value) = value * x;

    return s;

}


Length Length::operator / (const double& x) {

    Length s;

    (s.value) = value / x;

    return s;

}



void Speed::print(String name_in, String unit_in, String prefix, ostream& ostr) {

    if ((name_in.value) != "") {

        ostr << prefix.value << name_in.value << " = ";
        if (unit_in == String("kt")) {
            //units are kt

            ostr << value << " nm\n";

        }
        else {

            if (unit_in == String("km/h")) {
                //units are km/h

                ostr << value * nm << " km/h\n";

            }
            else {

                if (unit_in == String("m/s")) {
                    //units are m/s

                    ostr << value * nm * 1e3 / 3600.0 << " m/s\n";

                }

            }

        }

    }

}


//reads from file the content after 'name = ' and writes it into this. This function requires file to be correctly set and open
template<class S> void Speed::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    size_t pos1, pos2;
    String unit_temp;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

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

    pos1 = line.find(" = ");
    pos2 = line.find(" kt");

    if (line.find(" kt") != (string::npos)) {
        //the units of the speed read is kt
        cout << prefix.value << "Unit is in kt\n";
        pos2 = line.find(" kt");
        unit_temp = String("kt");
    }
    if (line.find(" km/h") != (string::npos)) {
        //the units of the length read is km/h
        cout << prefix.value << "Unit is in km/h\n";
        pos2 = line.find(" km/h");
        unit_temp = String("km/h");
    }
    if (line.find(" m/s") != (string::npos)) {
        //the units of the length read is m/s
        cout << prefix.value << "Unit is in m/s\n";
        pos2 = line.find(" m/s");
        unit_temp = String("m/s");
    }

    //X [km/h] = X [nm]/nm/[h] = X/nm [kt] = X 1000/3600 [m/s]

    value = stod(line.substr(pos1 + 3, pos2 - (pos1 + 3)).c_str());
    if (unit_temp == String("km/h")) {
        value /= nm;
    }
    if (unit_temp == String("m/s")) {
        value /= (1e3) * nm / 3600.0;
    }

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, String("kt"), prefix, cout);

}


void Speed::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << " kt\n";

}

//constructor of Speed that takes no arguments, and set the unit to the first entry of SpeedUnit_types
Speed::Speed(void) {
    
    unit = SpeedUnit(SpeedUnit_types[0]);
    
}

//constructor of Speed that sets value to value_in, and  the unit to the first entry of SpeedUnit_types
Speed::Speed(double value_in) {

    value = value_in;
    unit = SpeedUnit(SpeedUnit_types[0]);

}


//constructor which sets value to x  and the unit to unit_in
Speed::Speed(double value_in, const SpeedUnit& unit_in) {

    value = value_in;
    unit = unit_in;

}

//set the value of *this to x, where x is in kt
void Speed::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    value = x;

    if (name != String("")) { print(name, String("kt"), prefix, cout); }

}


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

//reads from file the color written after 'name = ' and writes it into this. If mode  = 'RW' ('R') it reads from a FileRW (FileR)
void Color::read_from_file(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    String new_prefix, s;
    size_t pos_end;
    int red, green, blue;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    s.read_from_file_to(name, filename, mode, new_prefix);

    //get rid of everything that comes before and at '(' at the beginnign of s
    pos_end = (s.value).find("(");
    s.set(String(""), String((s.value).substr(pos_end + 1).c_str()), String(""));
    //look for the first ','

    pos_end = (s.value).find(",");

    //read red
    red = stoi(((s.value).substr(0, pos_end)).c_str());

    //get rid of the first ','
    s.set(String(""), String((s.value).substr(pos_end + 1).c_str()), String(""));

    pos_end = (s.value).find(",");

    green = stoi((s.value).substr(0, pos_end).c_str());

    //get rid of the second ','
    s.set(String(""), String((s.value).substr(pos_end + 1).c_str()), String(""));

    pos_end = (s.value).find(")");
    //get rid of '('
    blue = stoi((s.value).substr(0, pos_end + 1).c_str());

    (*this) = Color(red, green, blue);


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
    boost::filesystem::remove(file.name.value);
    file.open(String("out"), prefix);
    (*(file.value)) << temp.str().c_str();
    file.close(prefix);

    //	boost::filesystem::rename(temp.name.value, file.name.value);

}

Answer::Answer(void) {
    //this is the default constructor, sets value to the default value, 'n' char

    value = 'n';

}

Answer::Answer(char c, [[maybe_unused]] String prefix) {

    if ((c == 'y') || (c == 'n')) {

        value = c;

    }
    else {
        //if the entered value is not valid, set value to 'n' and prints out this info

        value = 'n';
        cout << prefix.value << RED << "Value of answer is not valid, setting it to 'n'!\n" << RESET;

    }

}


bool Answer::operator==(const Answer& s) {

    return((value == (s.value)));

}

bool Answer::operator!=(const Answer& s) {

    return(!((*this) == s));

}


bool Answer::set(String name, char c, [[maybe_unused]] String prefix) {

    bool check;

    check = true;
    check &= ((c == 'y') || (c == 'n'));

    if (check) {

        value = c;

        if (name != String("")) {

            cout << prefix.value << name.value << " = " << c << "\n";

        }

    }
    else {

        cout << prefix.value << RED << "Value of answer is not valid!\n" << RESET;

    }

    return check;

}
//reads *this from file whose path is filename, by looking through the entire file
void Answer::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Answer>(this, name, filename, mode, prefix);

}



template<class S> void Answer::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << "... \n" << RESET;

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

    value = line[pos + 3];

    cout << prefix.value << YELLOW << "... done.\n" << RESET;


    print(name, prefix, cout);


}

//Tries to print the string name to ostr. If print_if_empty the string is printed even if it is empty, while if print_if_empty = false the string is not printed if empty
void String::print(String name, bool print_if_empty, String prefix, ostream& ostr) {

    if (((*this) != String("")) || print_if_empty) {

        ostr << prefix.value << name.value << " = " << value << "\n";

    }

}

//set the value of String *this, titled name, to input_string, by printing out everything
void String::set(String name, String input_string, [[maybe_unused]] String prefix) {

    value = (input_string.value);

    if (name != String("")) { print(name, true, prefix, cout); }

}

//set the value of String *this to input_string, without printing out anything
void String::set(String input_string) {

    value = (input_string.value);

}


//constructor of a Rotation instance
Rotation::Rotation(void) {

    //allocate and set the rotation matrix
    matrix = gsl_matrix_alloc(3, 3);

}

//sets the Euler angles and matrix of *this from the Euler angles a_in, b_in, c_in
void Rotation::set(Angle a_in, Angle b_in, Angle c_in) {

    a = a_in;
    b = b_in;
    c = c_in;

    gsl_matrix_set(matrix, 0, 0, cos(a) * cos(c) - cos(b) * sin(a) * sin(c));
    gsl_matrix_set(matrix, 0, 1, -(cos(c) * sin(a)) - cos(a) * cos(b) * sin(c));
    gsl_matrix_set(matrix, 0, 2, -(sin(b) * sin(c)));

    gsl_matrix_set(matrix, 1, 0, cos(b) * cos(c) * sin(a) + cos(a) * sin(c));
    gsl_matrix_set(matrix, 1, 1, cos(a) * cos(b) * cos(c) - sin(a) * sin(c));
    gsl_matrix_set(matrix, 1, 2, cos(c) * sin(b));

    gsl_matrix_set(matrix, 2, 0, -(sin(a) * sin(b)));
    gsl_matrix_set(matrix, 2, 1, -(cos(a) * sin(b)));
    gsl_matrix_set(matrix, 2, 2, cos(b));

}

//sets matrix and Euler angles of *this from the matrix m
void Rotation::set(gsl_matrix* m) {

    gsl_matrix_memcpy(matrix, m);

    //extract the Euler angles from the matrix m
    b = acos(gsl_matrix_get(m, 2, 2));
    a = atan(gsl_matrix_get(m, 2, 1) / (-sin(b)), gsl_matrix_get(m, 2, 0) / (-sin(b)));
    c = atan(gsl_matrix_get(m, 1, 2) / sin(b), gsl_matrix_get(m, 0, 2) / (-sin(b)));


}


//set the content of *this eqaul to the content of r by copying the content of r into the memory of *this : the memory adresses of *this and r will be left unchanged
inline void Rotation::set(const Rotation& r){
    
    a = (r.a);
    b = (r.b);
    c = (r.c);
    
    gsl_matrix_memcpy(matrix, r.matrix);
    
}


//constructor of a Rotation instance which sets the rotation matrix according to three Euler angles
Rotation::Rotation(Angle a_in, Angle b_in, Angle c_in) {

    //allocate and set the rotation matrix
    matrix = gsl_matrix_alloc(3, 3);

    set(a_in, b_in, c_in);

}

//constructor of a Rotation instance which sets the rotation matrix equal to the rotation which brings Position p to q
Rotation::Rotation(Position p, Position q) {

    if (p != q) {
        //if p!= q, I compute the rotation which brings Position p to Position q

        double cos_rotation_angle;
        Angle rotation_angle;
        Position rotation_axis;
        Cartesian r_p, r_q, omega;

        //transform p and q into cartesian cordinates and write them into r_p and r_q, respectively
        p.getCartesian(&r_p);
        q.getCartesian(&r_q);

        gsl_blas_ddot((r_p.r), (r_q.r), &cos_rotation_angle);
        rotation_angle.set(String(""), acos(cos_rotation_angle), String(""));


        my_cross((r_p.r), (r_q.r), &(omega.r));
        gsl_vector_scale((omega.r), 1.0 / fabs(sin(rotation_angle)));

        rotation_axis.setCartesian(String(""), omega, String(""));


        (*this) = (Rotation(
            Angle(String(""), 0.0, String("")),
            Angle(String(""), M_PI_2 - (((rotation_axis).phi).value), String("")),
            Angle(String(""), -((((rotation_axis).lambda).value) + M_PI_2), String(""))
        )
            * Rotation(
                Angle(String(""), (((rotation_axis).lambda).value) + M_PI_2, String("")),
                Angle(String(""), -(M_PI_2 - (((rotation_axis).phi).value)), String("")),
                rotation_angle
            ));

        //writes the Euler angles from matrix
        this->set(matrix);

    }
    else {
        //if start = end, I return the identity as rotation

        (*this) = (Rotation(
            Angle(String(""), 0.0, String("")),
            Angle(String(""), 0.0, String("")),
            Angle(String(""), 0.0, String(""))
        ));

    }

}



//returns the inverse of the rotation (*this)
Rotation Rotation::inverse(void) {

    Rotation t;
    Angle temp;

    //set the euler angles corredponding to the inverse rotation
    b = b * (-1.0);
    temp = c;
    c = a * (-1.0);
    a = temp * (-1.0);

    //transposes (inverts) this->matrix and copies the result into t.matrix
    gsl_matrix_transpose_memcpy(t.matrix, matrix);

    return t;

}


//composition of two rotations: this yields the rotation given by this . s
Rotation Rotation::operator *(const Rotation& s) {

    Rotation t;

//    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, matrix, s.matrix, 0.0, t.matrix);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 3, 3, 3, 1, matrix->data, 3, s.matrix->data, 3, 0, t.matrix->data, 3);


    //extract the Euler angles from the matrix t and write them into t
    t.set(t.matrix);

    //    (t.b) = acos(gsl_matrix_get(t.matrix, 2, 2));
    //    (t.a) = atan(gsl_matrix_get(t.matrix, 2, 1)/(-sin(t.b)), gsl_matrix_get(t.matrix, 2, 0)/(-sin(t.b)));
    //    (t.c) = atan(gsl_matrix_get(t.matrix, 1, 2)/sin(t.b), gsl_matrix_get(t.matrix, 0, 2)/(-sin(t.b)));
    //
    return t;

}


void Rotation::print(String name, String prefix, ostream& ostr) {

    unsigned int i, j;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    ostr << prefix.value << name.value << "\n" << prefix.value << "Euler angles : \n";


    a.print(String("a"), new_prefix, ostr);
    b.print(String("b"), new_prefix, ostr);
    c.print(String("c"), new_prefix, ostr);


    ostr << prefix.value << "matrix : \n";

    for (i = 0; i < 3; i++) {

        for (ostr << new_prefix.value, j = 0; j < 3; j++) {

            ostr << gsl_matrix_get(matrix, i, j) << "\t";

        }

        ostr << "\n";

    }

}


//reads the Rotation from file whose path is filename, by looking through the entire file
template<class S> void Rotation::read_from_stream(String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    Angle alpha, beta, gamma;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

    if (search_entire_stream) {


        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }

    alpha.read_from_stream<S>(String("alpha"), input_stream, false, new_prefix);
    beta.read_from_stream<S>(String("beta"), input_stream, false, new_prefix);
    gamma.read_from_stream<S>(String("gamma"), input_stream, false, new_prefix);

    set(alpha, beta, gamma);

}

//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Rotation::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Rotation>(this, name, filename, mode, prefix);

}



//constructor of Angle, which does not set the value of the angle
Angle::Angle(void) {

}

//constructor of Angle, which sets the value of the angle to x, where x is in radians
Angle::Angle(const double& x) {

    value = x;
    normalize();

}

//constructor of Angle, which sets the value of the angle to deg° min'
Angle::Angle(const unsigned int& deg, const double& min) {

    value = k * (((double)deg) + min / 60.0);
    normalize();

}


//constructor of Angle, which sets the value of the angle to x and eventually prints *this
Angle::Angle(String name, const double& x, [[maybe_unused]] const String& prefix) {

    value = x;
    normalize();
    if (name != String("")) { print(name, prefix, cout); }

}

inline bool Angle::operator == (const Angle& x) const{

    return((value == (x.value)));

}

inline bool Angle::operator!=(const Angle& x) {

    return(!((*this) == x));

}

inline bool Angle::operator==(const double& x) {

    return((value == x));

}

inline bool Angle::operator!=(const double& x) {

    return((value != x));

}


bool Angle::operator>(const Angle& x) {

    return((value > (x.value)));

}

bool Angle::operator>(const double& x) {

    return((value > x));

}

bool operator<(const Angle& x, const Angle& y) {

    return(((x.value) < (y.value)));

}

bool operator<(const Angle& x, const double& y) {

    return(((x.value) < y));

}


//I added the booleian variable search_entire_stream. If true, then this function rewinds the file pointer to the beginning of file and goes through the file until it finds the quantity 'name'. If false, it reads the angle at the position where 'file' was when it was passed to this function
template<class S> void Angle::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos1, pos2, pos3;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

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

    pos1 = line.find(" = ");
    pos2 = line.find("° ");
    pos3 = line.find("'");

    value = k * (stod(line.substr(pos1 + 3, pos2 - (pos1 + 3)).c_str()) + stod(line.substr(pos2 + 2, pos3 - (pos2 + 2))) / 60.0);

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, prefix, cout);

}

//reads from file the content after 'name = ' and writes it into this.
void Angle::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Angle>(this, name, filename, mode, prefix);

}

//evaluates whether Position (*this) is equal to Position p
bool Position::operator==(const Position& p) {

    bool check;

    check = true;

    check &= (((*this).phi) == (p.phi));
    check &= (((*this).lambda) == (p.lambda));
    check &= (((*this).label) == (p.label));

    return check;

}

//evaluates whether Position (*this) is different from Position p
bool Position::operator!=(const Position& p) {

    return (!((*this) == p));

}


Position::Position(void) {

}

Position::Position(Angle lambda_in, Angle phi_in) {

    lambda = lambda_in;
    phi = phi_in;

}

//here name is the name of the distance that I am computing; for example 'distance between positions A and B'
bool Position::distance(Position p, Length* l, String name, [[maybe_unused]] String prefix) {

    bool check;
    check = true;

    (l->value) = Re * acos(cos((lambda)-(p.lambda)) * cos(phi) * cos(p.phi) + sin(phi) * sin(p.phi));

    check &= !isnan(l->value);

    if (check) {

        Angle a;
        a.set(String(""), (l->value) / Re, prefix);
        l->print(name, String("nm"), prefix, cout);

    }
    else {

        cout << prefix.value << RED << "\tI could not compute distance!\n" << RESET;

    }

    return check;

}

//if route is a circle of equal altitude, it returns true if *this is into the circle, and zero otherwise. If route is not a circle of equal altitude, it prints an error message and returns false.
bool Position::is_in(Route route, [[maybe_unused]] String prefix) {

    if ((route.type) == (Route_types[2])) {

        Length d;

        distance(route.reference_position, &d, String(""), prefix);

        return(d <= (Re * ((route.omega).value)));


    }
    else {

        cout << prefix.value << RED << "route is not a circle of equal altitude: I cannot tell whether the position is into route!\n" << RESET;

        return false;

    }


}




//creates an element in listcontrol and writes into this element the values of all the fields contained in this
void Position::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    unsigned int i;
    wxListItem item;

    if (position_in_listcontrol == -1) {
        i = (listcontrol->GetItemCount());
    }
    else {
        i = ((unsigned int)position_in_listcontrol);
        listcontrol->DeleteItem(i);
    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_wxListCtrl(i, listcontrol);

}

//updates all the values in the GUI fields of item #i of listcontrol with the relative values of the non-GUI Position this
void Position::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

    int j;

    j = 0;

    //update number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //update latitude column
    listcontrol->SetItem(i, j++, wxString(phi.to_string(String("NS"), (display_precision.value), true)));

    //update longitude column
    listcontrol->SetItem(i, j++, wxString(lambda.to_string(String("EW"), (display_precision.value), true)));

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol
    //
    listcontrol->SetItem(i, j++, wxString(label.value));

    //    }else{
    //        //the label in *this is empty -> I write as label in listcontrol  and in *this the current date and time
    //
    //        Time now;
    //
    //        now.set_current(String(""));
    //
    //        label.set(String("Label replacing empty label"), now.to_string(display_precision.value), String(""));
    //        listcontrol->SetItem(i, j++, wxString(now.to_string(display_precision.value)));
    //
    //    }
    //
}


PositionRectangle::PositionRectangle(void) {


}


//constructor which constructs p_NW and p_SE from p_NW_in and p_SE_in. For this to work, p_NW_in must lie at the NW of p_SE_in
PositionRectangle::PositionRectangle(Position p_NW_in, Position p_SE_in, [[maybe_unused]] String prefix) {

    p_NW = p_NW_in;
    p_SE = p_SE_in;

}


//returns true/false if p is containted in *this
bool PositionRectangle::Contains(Position p) {

    bool check_lambda;

    if ((p_NW.lambda) > (p_SE.lambda)) {
        //'normal' configuration where *this does not encompass the anti-greenwich meridian

        check_lambda = (((p.lambda) < (p_NW.lambda)) && ((p.lambda) > (p_SE.lambda)));

    }
    else {
        //'non-normal' configuration where *this  encompasses the anti-greenwich meridian

        check_lambda = (((p.lambda) < (p_NW.lambda)) || ((p.lambda) > (p_SE.lambda)));

    }

    return(check_lambda &&
        (((p.phi).normalize_pm_pi_ret() < ((p_NW.phi).normalize_pm_pi_ret())) &&
            ((p.phi).normalize_pm_pi_ret() > ((p_SE.phi).normalize_pm_pi_ret()))));

}


//if p !=NULL , compute rectangle of *this in units of x * y of the Mercator projection and writes it into *p and returns true, otherwise return false
bool PositionRectangle::SizeMercator(PositionProjection* p){
        
    if(p){
        
        Position p_NE, p_SW;
        PositionProjection q_A, q_B;

        p_NE = Position(p_SE.lambda, p_NW.phi);
        p_SW = Position(p_NW.lambda, p_SE.phi);
        q_A.NormalizeAndSetMercator(p_NE);
        q_B.NormalizeAndSetMercator(p_SW);
        
        (*p) = q_A-q_B;
    
        return true;
        
    }else{
        
        return false;
    }
    
}


//construct a brand new Route object and thus sets its related sight to -1, because this Route is not related to any sight yet. length_format_t_v is set to false: as the Route is created, lengths are written in l rather than in t and v
Route::Route(void) {

    related_sight.set(String(""), -1, String(""));
    length_format.set((LengthFormat_types[1]));

}

//constructs a brand new Route. where lengths are written in time * speed than in l. The route type is either ((Route_types[0]).value) or ((Route_types[1]).value) and thus set its related sight to -1, because this Route cannot be related to any sight.
Route::Route(RouteType type_in, Position reference_position_in, Angle Z_in, Chrono time_in, Speed speed_in) {
    
    length_format.set((LengthFormat_types[0]));
    type = type_in;
    
    time = time_in;
    speed = speed_in;
    set_length_from_time_speed();
    
    reference_position = reference_position_in;
    Z = Z_in;
    related_sight.set(String(""), -1, String(""));

}


//constructs a brand new Route. where lengths are written as a simple length rather than as time * speed. The route type is either ((Route_types[0]).value) or ((Route_types[1]).value) and thus set its related sight to -1, because this Route cannot be related to any sight.
Route::Route(RouteType type_in, Position reference_position_in, Angle Z_in, Length l_in) {

    length_format.set((LengthFormat_types[1]));
    type = type_in;
    
    length = l_in;
    
    reference_position = reference_position_in;
    Z = Z_in;
    related_sight.set(String(""), -1, String(""));

}


//build a Route of type type that connects position_start and position_end
Route::Route(const RouteType& type_in,  Position p_start,  Position p_end){
    
    type = type_in;
    
    reference_position = p_start;
    (length_format.value) = ((LengthFormat_types[1]).value);
    
    switch (type.position_in_list(Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            PositionProjection projection_start, projection_end;
            
            projection_start.SetMercator(p_start);
            projection_end.SetMercator(p_end);
            
            
            if(p_start.phi != p_end.phi){
                //I am not in the special case where p_start and p_end have the same latitude
                
                
                //            reference_position.phi.normalize_pm_pi();
                
                //set Z
                Z.set(
                      String(""),
                      -GSL_SIGN((projection_end.x) - (projection_start.x))*acos(sqrt(1.0/(1.0 + gsl_pow_2(((projection_end.x) - (projection_start.x))/((projection_end.y) - (projection_start.y)))))),
                      String("")
                      );
                if((projection_end.y) > (projection_start.y)){
                    Z = Z*(-1.0);
                }else{
                    Z = Z + M_PI;
                }
                
            }else{
                //I am in the special case where p_start and p_end have the same latitude
                
                //set Z
                Z.set(
                      String(""),
                      -GSL_SIGN((projection_end.x) - (projection_start.x))*M_PI_2 + M_PI,
                      String("")
                      );
                
            }
            
            //set length according to t* (see notes)
            set_length_from_input(fabs((p_end.lambda.value) - (p_start.lambda.value)));
            
            
            break;
            
        }
            
        case 1:{
            //*this is an orthodrome
            
            //p_start and p_end in Cartesian coordinates
            Cartesian r_start, r_end, s;
            Angle phi, z;
            Position p_a, p_b, end_1, end_2;
            
            p_start.getCartesian(&r_start);
            p_end.getCartesian(&r_end);
            
            reference_position = p_start;
            
            
            //set the legnth as the length of the shortest great circle joining p_start and p_end
            phi.set(String(""), acos(r_start.dot(r_end)), String(""));
            length.set(String(""), Re*(phi.value), String(""));
            
            //set the tentative solution for the azimuth angle z: Z may be either z  (solkution 1) or -z (solution 2), I will pick the correct solution later
            z.set(String(""),
                  acos(-csc(phi) * sec(p_start.phi) * (cos(phi) * sin(p_start.phi) - sin(p_end.phi)) ),
                  String(""));

            //consider solution 1, compute end with this solution and store it in end_1
            Z.set(String(""), z.value, String(""));
            compute_end(String());
            end_1 = end;
            
            //consider solution 2, compute end with this solution and store it in end_2
            Z.set(String(""), -z.value, String(""));
            compute_end(String());
            end_2 = end;
            
            //check which one among end_1 and end_2 has a longitude closer to p_end and pick the correct solution accordingly
            if(fabs(end_1.lambda.value - p_end.lambda.value) < fabs(end_2.lambda.value - p_end.lambda.value)){
                Z.set(String(""), z.value, String(""));
            }else{
                Z.set(String(""), -z.value, String(""));
            }
            

            break;
            
        }
            
        case 2:{
            //*this is a circle of equal altitude
            
            cout << RED << "Cannot create a circle of equal altitute taht connects two positions!\n" << RESET;
            break;}
            
    }
    
}


//construct a brand new Route object of type ((Route_types[2]).value) and thus sets its related sight to -1, because this Route is not related to any sight yet.  length_format_t_v is set to false: as the Route is created, lengths are written in l rather than in t and v
Route::Route(RouteType type_in, Position reference_position_in, Angle omega_in) {

    type = type_in;
    reference_position = reference_position_in;
    omega = omega_in;

    length_format.set((LengthFormat_types[1]));
    //the lenght of the circle of equal altitude is set by default
    length.set(String(""), 2.0 * M_PI * Re * sin(omega), String(""));

    related_sight.set(String(""), -1, String(""));

}

//add the Route this to the wxListCtrl listcontrol
void Route::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    unsigned int i;
    wxListItem item;

    if (position_in_listcontrol == -1) {
        //in this case, I add a new element at the end of listcontrol

        i = (listcontrol->GetItemCount());

    }
    else {
        //in this case, I delete the i-th elment in listcontrol and replace it

        i = ((unsigned int)position_in_listcontrol);
        listcontrol->DeleteItem(i);

    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_wxListCtrl(i, listcontrol);

}


inline void Route::DrawOld(unsigned int n_points, DrawPanel* draw_panel, vector< vector<wxPoint> >* v, [[maybe_unused]] String prefix) {

    wxPoint p;
    bool end_connected;
    unsigned int i;

    
    set_length_from_time_speed();

    //tabulate the Route points
    for (/*this is true if at the preceeding step in the loop over i, I encountered a point which does not lie in the visible side of the chart, and thus terminated a connectd component of dummy_route*/v->clear(), end_connected = true, i = 0; i < n_points; i++) {

        compute_end(Length((length.value) * ((double)i) / ((double)(n_points - 1))), String(""));
        
        //treat the first and last point as a special one because it may be at the boundary of rectangle_observer-> check if they are and, if they are, put them back into rectangle_observer
        if((i==0) || (i==n_points-1)){
            end.put_back_in(draw_panel);
        }

        if ((draw_panel->GeoToDrawPanel)(end, &p, false)) {

            if (end_connected) {

                v->resize(v->size() + 1);
                end_connected = false;

            }

            ((*v)[v->size() - 1]).push_back(p);

        }
        else {

            end_connected = true;

        }

    }

}


//draws into draw_panel the Route this, by tabulating the Route with n points and connecting them with an spline. The route is drawn with color 'color' and width 'width'. If width = -1, then the Route is drawn with default width
inline void Route::DrawOld(unsigned int n_points, Color color, int width, wxDC* dc, DrawPanel* draw_panel) {

    vector< vector<wxPoint> > p;
    wxPoint temp;
    bool end_connected;
    unsigned int i;
    Length s;

    //sets color and width of memory_dc to the ones supported as arguments of PreRender
    dc->SetPen(wxPen(color, width));
    dc->SetBrush(wxBrush(wxGetApp().background_color, wxBRUSHSTYLE_TRANSPARENT));

    set_length_from_time_speed();

    //tabulate the Route points
    for (/*this is true if at the preceeding step in the loop over i, I encountered a point which does not lie in the visible side of the sphere, and thus terminated a connectd component of dummy_route*/end_connected = true, i = 0; i < n_points; i++) {

        //handle special cases i=0 and i = n_points-1 to avoind roundoff error
        if ((i > 0) && (i < n_points - 1)) {

            s.set(String(""), (length.value) * ((double)i) / ((double)(n_points - 1)), String(""));

        }
        else {

            if (i == 0) {
                s.set(String(""), 0.0, String(""));
            }
            else {
                s = length;
            }

        }

        compute_end(s, String(""));
        
        //treat the first and last point as a special one because it may be at the boundary of rectangle_observer-> check if they are and, if they are, put them back into rectangle_observer
        if((i==0) || (i==n_points-1)){
            end.put_back_in(draw_panel);
        }

        if ((draw_panel->GeoToDrawPanel)(end, &temp, false)) {

            if (end_connected) {

                p.resize(p.size() + 1);
                end_connected = false;

            }

            (p[p.size() - 1]).push_back(temp);

        }
        else {

            end_connected = true;

        }

    }

    //run all the connected components of the tabulated Route and draw each of them in draw_panel
    for (i = 0; i < p.size(); i++) {

        if (((p[i]).size()) > 1) {

            dc->DrawSpline((int)((p[i]).size()), (p[i]).data());

        }

    }

    //put back original parameters of memory_dc
    dc->SetPen(wxPen(wxGetApp().foreground_color, 1));

}

inline void Route::DrawOld(unsigned int n_points, wxDC* dc, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {

    int i;
    vector< vector<wxPoint> > v;

    DrawOld(n_points, draw_panel, &v, prefix);

    for (i = 0; i < (v.size()); i++) {
        //run over all chunks of the Route written in v
        if (((v[i]).size()) > 1) {
            //the i-th chunk v[i] has at least two points -> draw it
            dc->DrawSpline((int)((v[i]).size()), (v[i]).data());
        }
    }


}


//draws the Route *this into draw_panel, with any projection. n_points, color and width is the number of points, the line color and the width of the line used to draw *this, respectively
inline void Route::Draw(unsigned int n_points, Color foreground_color, Color background_color, int width, wxDC* dc, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {

    unsigned int i;
    vector<wxPoint> p;
    wxPoint temp, q;
    vector<Length> s;

    //sets color and width of memory_dc to the ones supported as arguments of PreRender
    dc->SetPen(wxPen(foreground_color, width));
    dc->SetBrush(wxBrush(background_color, wxBRUSHSTYLE_TRANSPARENT));

    //comoute the end values of l and writes them in s
    compute_l_ends(&s, NULL, draw_panel, prefix);

    if (type == (Route_types[0])) {
        //*this is a loxodrome

        cout << prefix.value << RED << "Cannot execute Draw: the Route is not an orthodrome nor a circle of equal altitude!\n" << RESET;

    }
    else {
        //*this is an orthodrome/circle of equal altitude

        //the following code holds for all projections

        bool compute_l_ends_ok;

        compute_l_ends(&s, &compute_l_ends_ok, draw_panel, prefix);

        if (compute_l_ends_ok) {
            //*this is included in the area of the chart visible by the observer

            unsigned int j;
            bool check;
            //a temporary length used to computed end
            Length r;

            for (j = 0; j < (s.size()) - 1; j++) {

                //clear up vectors where I am about to write
                p.clear();

                //tabulate the Route points
                for (i = 0; i < n_points; i++) {


                    //to avoid rounding problems, I set r exactly to s[j] (s[j+1]) for i=0 (i=n_points-2) and use the linear formula in between
                    if (i == 0) {
                        r = s[j];
                    }
                    else {
                        if (i < n_points - 1) {
                            r = Length(((s[j]).value) + (((s[j + 1]) - (s[j])).value) * ((double)i) / ((double)(n_points - 1)));
                        }
                        else {
                            r = s[j + 1];
                        }
                    }

                    compute_end(r, String(""));
                    
                    //treat the first and last point as a special one because it may be at the boundary of rectangle_observer-> check if they are and, if they are, put them back into rectangle_observer
                    if((i==0) || (i==n_points-1)){
                        end.put_back_in(draw_panel);
                    }

                    check = (draw_panel->GeoToDrawPanel)(end, &temp, true);

                    if (check) {
                        //temp is a valid point

                        p.push_back(temp);

                    }
                    else {
                        //temp is not a valid point

                        if (!(draw_panel->PutBackIn(temp, &q))) {
                            //temp does not fall in the plot area

                            p.push_back(temp);

                        }

                    }

                }

                if ((p.size()) > 1) {
                    dc->DrawSpline((int)(p.size()), p.data());
                }

                //free up memory
                p.clear();

            }

        }

    }

}

inline void Route::Draw(unsigned int n_points, wxDC* dc, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {

    int i;
    vector< vector<wxPoint> > v;

    Draw(n_points, draw_panel, &v, prefix);

    for (i = 0; i < (v.size()); i++) {
        //run over all chunks of the Route written in v
        if (((v[i]).size()) > 1) {
            //the i-th chunk v[i] has at least two points -> draw it
            dc->DrawSpline((int)((v[i]).size()), (v[i]).data());
        }
    }

}

//tabulate the points of Route *this in any projection of draw_panel and writes them into v
inline void Route::Draw(unsigned int n_points, DrawPanel* draw_panel, vector< vector<wxPoint> >* v, [[maybe_unused]] String prefix) {

    unsigned int i, j;
    wxPoint p;
    Angle lambda_a, lambda_b;
    vector<Length> s;
    bool compute_l_ends_ok;

    compute_l_ends(&s, &compute_l_ends_ok, draw_panel, prefix);
 
    //comoute the end values of l and writes them in s. If compute_l_ends returns true, than the endpoints have been computed correclty, and I can proceed
    if (compute_l_ends_ok) {
        
        bool check;
        //a vector where I will store the tentative points of each chunk of *this
        vector<wxPoint> v_tentative;
        //the number of points of each chunk for which GeoToDrawPanel returns true (without recurring to put_back_in)
        unsigned int n_points_check_ok;

         //run over all chunks of *this which are visible
         //given that s contains the number of intersection points of *this and that each pair of intersection point delimits a chunk, and that v contains the chunks, the size of v is equal to thte size of s minus one.
        for (j=0; j<(s.size()) - 1; j++) {
            //run over all chunks
 
            
            //tabulate the Route points of the jth chunk and store them in v_proposed
            for (v_tentative.clear(), n_points_check_ok=0, i = 0; i < n_points; i++) {

                //I slightly increase s[j] and slightly decrease s[j+1] (both by epsilon_double) in order to plot a chunk of the Route *this which is slightly smaller than the chunk [s[j], s[j+1]] and thus avoid  the odd lines that cross the whole plot area in the Mercator projection and that connect two points of the same chunk that are far from each other  on the plot area
                compute_end(Length(((s[j]).value) * (1.0 + epsilon_double) + (((s[j + 1]).value) * (1.0 - epsilon_double) - ((s[j]).value) * (1.0 + epsilon_double)) * ((double)i) / ((double)(n_points - 1))), String(""));
                
                check = (draw_panel->GeoToDrawPanel)(end, &p, false);
                
                if (check) {
                    //end is a valid point -> convert it to a Position with GeoToDrawPanel

                    v_tentative.push_back(p);
                    n_points_check_ok++;

                }else{
                    //end is not a valid point
                    
                    if((i==0) || (i==n_points-1)){
                        //the non-valid point is the first or last point in the Route chunk -> the point may be non valid because it lies on the edge, i.e., because of a rounding error -> put it back in and, if the Position that has been put_back_in is valid, convert it to a Position with GeoToDrawPanel
                        
                        end.put_back_in(draw_panel);
                        
                        if((draw_panel->GeoToDrawPanel)(end, &p, false)){
                            
                            v_tentative.push_back(p);
                            
                        }

                    }else{
                        //the non-valid point lies in the middle of the Route chunk -> the reason why the point is non-valid cannot be a rounding error -> do not push the point to v_tentaive and break the for loop over i to terminate drawing the route chunk and switch to the next one
                        
                        break;
                        
                    }
     
                }
                
            }
            
            //now I decide if v_proposed is a valid chunk (a chunk to be plotted), and thus if I sholud push it back to v or not
            if(n_points_check_ok>0){
                //v_tentative containts at least one point for which GeoToDrawPanel evaluated to true (without recurring to put_back_in) -> it is a valid chunk -> I add it to v. On the other hand, if n_points_check_ok == 0, then the only points in v_tentative may be the first and the last, which have been pushed back to v_tentative by put_back_in, and the chunk will be an odd chunk with only two points put into rectangle_observer by put_back_in -> This may lead to odd diagonal lines in the Mercator projection 
                
                v->push_back(v_tentative);
                
            }

        }

    }else {

        //        cout << prefix.value << RED << "I could not compute ends of Route!\n" << RESET;

    }

}


//compute the values of the Length l for Route *this at which *this crosses draw_panel->circle/rectangle_observer, and writes them in *s. For (*s)[i] < l < (*s)[i+1], the Route *this lies within draw_panel -> circle/ draw_panel->rectangle_observer, and it is thus visible. If success != NULL, it writes true in *success if the values of the length above could be computed succesfully, and false otherwise.
inline void Route::compute_l_ends(vector<Length>* s, bool* success, DrawPanel* draw_panel, [[maybe_unused]] String prefix) {
    
    vector<Angle> t;
    
    switch (type.position_in_list(Route_types)) {
            
        case 0: {
            //the Route this is a loxodrome
            
            cout << prefix.value << RED << "Cannot execute compute_l_ends: the Route is not an orthodrome nor a circle of equal altitude!\n" << RESET;
            
            if (success != NULL) {
                (*success) = false;
            }
            
            break;
            
        }
            
        case 1: {
            //the Route this is an orthodrome
            
            int check = -1;
            
            switch (position_in_vector(Projection((draw_panel->parent->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    check = inclusion(draw_panel->rectangle_observer, true, &t, String(""));
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    check = inclusion(draw_panel->circle_observer, true, &t, String(""));
                    
                    break;
                    
                }
                    
            }
            
            
            if (check == 1) {
                //there is a part of *this which is included in circle/rectangle_observer -> some part of *this will lie on the visible part of the earth
                
                unsigned int i;
                
                //                s->resize(2);
                //                ((*s)[0]).set(String(""), Re*((t[0]).value), String(""));
                //                ((*s)[1]).set(String(""), Re*((t[1]).value), String(""));
                
                for (s->resize(t.size()), i = 0; i < (t.size()); i++) {
                    
                    ((*s)[i]).set(String(""), ((t[i]).value) * Re, String(""));
                    
                }
                
                
                t.clear();
                
                if (success != NULL) {
                    (*success) = true;
                }
                
            }
            else {
                //no part of this is included in circle/rectagle observer -> no part of this lies on the visible part of the earth
                
                if (success != NULL) {
                    (*success) = false;
                }
                
            }
            
            break;
            
        }
            
        case 2: {
            //the Route this is a circle of equal altitde.  its total length is the length of the circle itself, which reads:
            
            switch (position_in_vector(Projection(draw_panel->parent->projection->name->GetValue().ToStdString()), Projection_types)) {
                    
                case 0: {
                    //I am using the Projection_types[0] projection
                    
                    if (inclusion(draw_panel->rectangle_observer, true, &t, String("")) == 1) {
                        //*this is included in rectangle_observer
                        
                        if ((t[0] == 0.0) && (t[1] == 0.0)) {
                            //*this is fully included into rectangle_observer and does not interscet with circle_observer: in this case, I draw the full circle of equal altitude *this
                            
                            s->resize(2);
                            ((*s)[0]).set(String(""), 0.0, String(""));
                            ((*s)[1]).set(String(""), 2.0 * M_PI * Re * sin(omega), String(""));
                            
                        }
                        else {
                            
                            unsigned int i;
                            
                            //*this is partially included into rectangle_observer and it interscets rectangle_observer-> I write in s the values of the parametric length of *this at which these intersections occur
                            
                            for (s->resize(t.size()), i = 0; i < (t.size()); i++) {
                                
                                ((*s)[i]).set(String(""), ((t[i]).value) * Re * sin(omega), String(""));
                                
                            }
                            
                        }
                        
                        if (success != NULL) {
                            (*success) = true;
                        }
                        
                    }
                    else {
                        //*this is not included in rectangle_observer
                        
                        if (success != NULL) {
                            (*success) = false;
                        }
                        
                    }
                    
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using the Projection_types[1] projection
                    
                    if (inclusion(draw_panel->circle_observer, true, &t, String("")) == 1) {
                        //there is a part of *this which is included in circle_observer -> some part of *this will lie on the visible part of the earth
                        
                        s->resize(2);
                        
                        if ((t[0] == 0.0) && (t[1] == 0.0)) {
                            //*this is fully included into circle_observer and does not interscet with circle_observer: in this case, I draw the full circle of equal altitude *this
                            
                            ((*s)[0]).set(String(""), 0.0, String(""));
                            ((*s)[1]).set(String(""), 2.0 * M_PI * Re * sin(omega), String(""));
                            
                        }
                        else {
                            //*this intersects with circle_observer: I draw only a chunk of the circle of equal altitutde *this
                            
                            Length l1, l2;
                            
                            //here I decide whether the chunk with average t t[0]+t[1]/2 or the chunk with average t t[0]+t[1]/2+pi is the one included in circle_observer
                            //note that here doing the average as ((((t[0]).value)+((t[1]).value)))/2.0 and doing it as ((t[0]+t[1]).value)/2.0
                            compute_end(
                                        Length(
                                               ((Angle(((((t[0]).value) + ((t[1]).value))) / 2.0)).value) * (Re * sin(omega))
                                               ),
                                        String(""));
                            ((draw_panel->circle_observer).reference_position).distance(end, &l1, String(""), String(""));
                            
                            compute_end(
                                        Length(
                                               ((Angle(((((t[0]).value) + ((t[1]).value))) / 2.0 + M_PI)).value) * (Re * sin(omega))
                                               ),
                                        String(""));
                            ((draw_panel->circle_observer).reference_position).distance(end, &l2, String(""), String(""));
                            
                            if (l2 > l1) {
                                
                                ((*s)[0]).set(String(""), ((t[0]).value) * (Re * sin(omega)), String(""));
                                ((*s)[1]).set(String(""), ((t[1]).value) * (Re * sin(omega)), String(""));
                                
                            }
                            else {
                                
                                ((*s)[0]).set(String(""), ((t[1]).value) * (Re * sin(omega)), String(""));
                                ((*s)[1]).set(String(""), (2.0 * M_PI + ((t[0]).value)) * (Re * sin(omega)), String(""));
                                
                            }
                            
                        }
                        
                        t.clear();
                        
                        if (success != NULL) {
                            (*success) = true;
                        }
                        
                    }
                    else {
                        
                        if (success != NULL) {
                            (*success) = false;
                        }
                        
                    }
                    
                    break;
                    
                }
                    
            }
            
            break;
            
        }
            
    }
    
}

void Route::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

    unsigned int j;


    j = 0;
    //set the number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //set type column: I write the extended type names, not the short ones 'l', 'o' and 'c'
    if (type == (Route_types[0])) {
        listcontrol->SetItem(i, j++, wxString(((Route_types[0]).value)));
    }
    if (type == (Route_types[1])) {
        listcontrol->SetItem(i, j++, wxString(((Route_types[1]).value)));
    }
    if (type == (Route_types[2])) {
        listcontrol->SetItem(i, j++, wxString(((Route_types[2]).value)));
    }

    if (type == (Route_types[2])) {
        //in this case the type of this is 'circle of equal altitde': the first three fields are thus empty, and I fill in only the last two fields

        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));

        listcontrol->SetItem(i, j++, wxString(reference_position.to_string((display_precision.value))));
        listcontrol->SetItem(i, j++, wxString(omega.to_string(String(""), (display_precision.value), true)));

    }
    else {
        //in this case the type of this is 'loxodrome' or 'orthodrome': the last two fields are empty, and I fill in only the first three fields

        listcontrol->SetItem(i, j++, wxString(reference_position.to_string((display_precision.value))));
        listcontrol->SetItem(i, j++, wxString(Z.to_string(String(""), (display_precision.value), false)));
        
        set_length_from_time_speed();
        listcontrol->SetItem(i, j++, wxString(length.to_string(String("nm"), (display_precision.value))));

        listcontrol->SetItem(i, j++, wxString(""));
        listcontrol->SetItem(i, j++, wxString(""));


    }

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol

    listcontrol->SetItem(i, j++, wxString(label.value));

    //    }else{
    //        //the label in *this is empty -> I write as label in listcontrol the current date and time
    //
    //        Time now;
    //
    //        now.set_current(String(""));
    //
    //        label.set(String("Label replacing empty label"), now.to_string(display_precision.value), String(""));
    //        listcontrol->SetItem(i, j++, wxString(now.to_string(display_precision.value)));
    //
    //    }


    if ((related_sight.value) == -1) {
        //if the route is not connected to a sight, I leave the column field empty

        listcontrol->SetItem(i, j++, wxString(""));

    }
    else {
        //if the route is connected to a sight, I write the # of the related sight in the column field

        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_sight.value) + 1));

    }


}



//Given the route (*this), this function returns the point on the Route which is closest to Position q, and writes this position and the corresponding value of t in p and tau, respectively.
bool Route::closest_point_to(Position* p, Angle* tau, Position q, [[maybe_unused]] String prefix) {

    String new_prefix;
    bool check;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    check = true;

    if (type == (Route_types[2])) {

        //these are the two values of the parametric angle t of the Route (*this), for which the distance between q and a point on (*this) vs. t has a maximum or a minimum
        Angle t_1, t_2;
        Position p_1, p_2;
        Length s_1, s_2;

        t_1.set(String(""),


            atan((cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) - cos(reference_position.phi) * sin(q.phi)) /
                sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2)),
                (cos(q.phi) * sin((reference_position.lambda) - (q.lambda))) /
                sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2)))

            , new_prefix);


        t_2.set(String(""),

            atan((-(cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi)) + cos(reference_position.phi) * sin(q.phi)) /
                sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                    gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                    2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                    gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2)),
                -((cos(q.phi) * sin((reference_position.lambda) - (q.lambda))) /
                    sqrt(gsl_pow_int(cos((reference_position.lambda) - (q.lambda)), 2) * gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin(reference_position.phi), 2) +
                        gsl_pow_int(cos(q.phi), 2) * gsl_pow_int(sin((reference_position.lambda) - (q.lambda)), 2) -
                        2 * cos(reference_position.phi) * cos((reference_position.lambda) - (q.lambda)) * cos(q.phi) * sin(reference_position.phi) * sin(q.phi) +
                        gsl_pow_int(cos(reference_position.phi), 2) * gsl_pow_int(sin(q.phi), 2))))

            , new_prefix);

        //determine which one between the point on (*this) at t_1 and the one at t_2 is the one with minimum distance with respect to q, and store this point into (*p)
        compute_end(Length((t_1.value) * Re * sin(omega)), new_prefix);
        p_1 = end;
        q.distance(p_1, &s_1, String("Distance with respect to p_1"), new_prefix);

        compute_end(Length((t_2.value) * Re * sin(omega)), new_prefix);
        p_2 = end;
        q.distance(p_2, &s_2, String("Distance with respect to p_2"), new_prefix);

        if (s_2 > s_1) {
            (*p) = p_1;
            (*tau) = t_1;
        }
        else {
            (*p) = p_2;
            (*tau) = t_2;
        }


    }
    else {

        check &= false;

    }

    if (check) {

        (*p).print(String("Closest point"), prefix, cout);

    }
    else {

        cout << prefix.value << RED << "Closest point could not be computed!\n" << RESET;

    }

    return check;

}

//obtain the size of *this in the Mercator projection : consider the smallest rectangle that contains *this entirely, and say that this rectangle has, in the Mercator projection, bottom-left and top-right points (0,0) and *p, respectively -> compute the top-right point and write it in *p
void Route::size_Mercator(PositionProjection* p){
    
    PositionProjection q;
    

    //if the length of *this is expresed as time x speed, compute length from time and speed, otherwise the length of *this is already written in then and there is nothing to do
    set_length_from_time_speed();
    
    //in what follows, I store the two points representing the corners of the rectangle ennclosing *this in the Mercator projection in *p and q
    
    if(type == Route_types[0]){
        //*this is a loxodrome -> for loxodromes, latitude and longitude are either constantly increasing or decreasing along the Route -> I compute the points of maximal and minimal latitude / longitude from the extremal Positions on *this
        
        compute_end(String(""));
        p->NormalizeAndSetMercator(end);
        q.NormalizeAndSetMercator(reference_position);
 
        
    }else{
        //this is an orthodrome or a circle of equal altitude: latitude and longitude are not necessarily monotonic functions of the coordinate t along the curve-> compute them with lambda_min_max and phi_min_max
        
        Angle lambda_min, lambda_max, phi_min, phi_max;
        
        lambda_min_max(&lambda_min, &lambda_max, String(""));
        phi_min_max(&phi_min, &phi_max, String(""));
        
        q.NormalizeAndSetMercator(Position(lambda_min, phi_min));
        p->NormalizeAndSetMercator(Position(lambda_max, phi_max));
        
        
    }
    
    //I substract q to *p and store the absolute value of the result in p -> this is the size that *this occupies in the Mercator projection
    
    (*p) -= q;
    (p->x) = fabs(p->x);
    (p->y) = fabs(p->y);
    
}


//If circle is not a circle of equal altitude, it returns -1 (error code). Otherwise, if the type of *this is not valid, it returns -1. Otherwise, the type of *this is valid-> if a part of *this is included into  circle, it returns 1, and 0 otherwise. If 1 is returned and write_t = true, it writes in t the value of the parametric angle of *this at which *this intersects circle and, if *this lies within circle and write_t = true, it sets t[0] = t[1] = 0.0
int Route::inclusion(Route circle, bool write_t, vector<Angle>* t, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if (((circle.type) == (Route_types[2]))) {
        //circle is a circle of equal altittude

        if (type == Route_types[0]) {
            //*this is a loxodrome

            cout << prefix.value << RED << "Cannot determine whether *this is included in circle, because *this is a loxodrome!\n" << RESET;

            return -1;

        }
        else {
            //*this is either an orthodrome or a loxodrome

            if (type == Route_types[1]) {
                //*this is an orthodrome

                if (intersection(circle, true, t, new_prefix) == 0) {
                    //*this and circle do not intersect: check whether *this is fully included into circle

                    if (reference_position.is_in(circle, prefix)) {
                        //reference_position is included into the circle of circle, thus *this is included into circle

                        if (write_t) {
                            
                            set_length_from_time_speed();

                            t->resize(2);
                            ((*t)[0]).set(String(""), 0.0, new_prefix);
                            ((*t)[1]).set(String(""), (length.value) / Re, new_prefix);

                        }

                        return 1;

                    }
                    else {
                        //reference_position is not included into the circle of circle, thus *this is not included into circle

                        return 0;

                    }

                }
                else {
                    //*this and circle intersect ->  a part of *this is included into circle

                    if (write_t) {

                        switch (t->size()) {

                        case 1: {
                            //there is one intersection point

                            if (reference_position.is_in(circle, prefix)) {
                                //this->reference position is included into the circle of circle -> the part of *this comprised into circle is the one with 0 <= t <= (*t)[0]

                                t->insert(t->begin(), Angle(String(""), 0.0, new_prefix));

                            }
                            else {
                                //this->reference position is not included into the circle of circle -> this->end must be included into the circle of circle -> the part of *this comprised into circle is the one with  (*t)[0] <= t <= (l.value)/Re

                                set_length_from_time_speed();
                                t->push_back(Angle(String(""), (length.value) / Re, new_prefix));

                            }

                            break;

                        }

                        case 2: {
                            //there are two intersection points -> the part of *this comprised into circle is the one with (*t)[0] < t <(*t)[1] -> all I need to do is sort t

                            sort(t->begin(), t->end());

                            compute_end(Length(Re * (((((*t)[0]).value) + (((*t)[1]).value)) / 2.0)), String(""));

                            if (!(end.is_in(circle, String("")))) {
                                //the midpoints on *this between t[0] and t[1] is not comprised into circle

                                //I add 0 and 2*M_PI to the vector t, so I create two chunks of the curve *this which are comprised into circle
                                t->push_back(Angle(0.0));
                                t->push_back(Angle(2.0 * M_PI));
                                //                                    (t->back()).value = 2.0*M_PI;

                                sort(t->begin(), t->end());

                            }


                            break;

                        }

                        }

                    }

                    return 1;

                }

            }
            else {

                if (type == Route_types[2]) {
                    //*this is a circle of equal altitude

                    Length d;

                    reference_position.distance(circle.reference_position, &d, String(""), new_prefix);

                    if (d < (Re * ((omega + (circle.omega)).value))) {
                        //the circles have a common area

                        if (write_t) {

                            if (intersection(circle, true, t, new_prefix) == 0) {
                                //the circles do no intersect: I set t[0] = t[1] = 0.0

                                t->resize(2);
                                ((*t)[0]).set(String(""), 0.0, new_prefix);
                                ((*t)[1]).set(String(""), 0.0, new_prefix);

                            }
                            else {
                                //the circles intersect: here you should compute t: method intersection called in the condition intersection(circle, true, t, new_prefix) == 0 has written in t the intersection angles



                            }

                        }

                        return 1;

                    }
                    else {
                        //the circles don't have a common area

                        return 0;

                    }

                }
                else {

                    cout << prefix.value << RED << "Cannot determine whether Route is included in circle, Route type is invalid!\n" << RESET;

                    return -1;

                }

            }

        }

    }
    else {

        cout << prefix.value << RED << "Cannot determine whether Route is included in circle, because circle is not a circle of equal altitude!\n" << RESET;

        return -1;

    }

}

//If *this is a loxodrome, return -1 because I don't know how to determine whetehr the loxodrome is included in a PositionRectangle. Otherwise, if *this is included into the PositionRectangle rectangle it returns 1, and 0 otherwise. If 1 is returned and write_t = true, it reallocates t and writes in t the value of the parametric angle of *this at which *this intersects rectangle and, if *this entirely lies within circle and write_t = true, it returns t[0] = t[1] = 0.0
int Route::inclusion(PositionRectangle rectangle, bool write_t, vector<Angle>* t, [[maybe_unused]] String prefix) {


    if (type == (Route_types[0])) {
        //*this is a loxodrome

        cout << prefix.value << RED << "Cannot determine whether *this is included in rectangle, because *this is a loxodrome!\n" << RESET;

        return -1;

    }else {
        //*this is an orthodrome or a circle of equal altitude

        Angle lambda_span, phi_span;
        Route side_N, side_S, side_E, side_W;
        vector<Angle> u, temp;
        bool  /*this is true if the entire Route *this is included in rectangle, and false otherwise*/is_fully_included;
        unsigned int i;
        int output;


        lambda_span = ((rectangle.p_NW).lambda).span((rectangle.p_SE).lambda);
        phi_span = ((rectangle.p_NW).phi).span((rectangle.p_SE).phi);

        //the parallel of latitude going through the North side of rectangle
        side_N = Route(
            RouteType(Route_types[2]),
            Position(Angle(0.0), Angle(GSL_SIGN((((rectangle.p_NW).phi).normalize_pm_pi_ret()).value) * M_PI_2)),
            Angle(M_PI_2 - fabs(((((rectangle.p_NW).phi).normalize_pm_pi_ret()).value)))
        );

        //the parallel of latitude going through the S side of rectangle
        side_S = Route(
            RouteType(Route_types[2]),
            Position(Angle(0.0), Angle(GSL_SIGN((((rectangle.p_SE).phi).normalize_pm_pi_ret()).value) * M_PI_2)),
            Angle(M_PI_2 - fabs(((((rectangle.p_SE).phi).normalize_pm_pi_ret()).value)))
        );

        //the meridian going through the W side of rectangle
        side_W = Route(
            RouteType(Route_types[2]),
            Position(((rectangle.p_NW).lambda) + M_PI_2, Angle(0.0)),
            Angle(M_PI_2)
        );

        //the meridian going through the E side of rectangle
        side_E = Route(
            RouteType(Route_types[2]),
            Position(((rectangle.p_SE).lambda) + M_PI_2, Angle(0.0)),
            Angle(M_PI_2)
        );


        //compute the intersections between *this and side_N/S/E/W, and writes in temp the values of the parametric angle t of *this at which *this crosses side_N/S/E/W: temps are then appended to u, which, at the end, will contain all intersections
        intersection(side_N, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());
        temp.clear();

        intersection(side_S, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());
        temp.clear();

        intersection(side_E, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());
        temp.clear();

        intersection(side_W, true, &temp, String(""));
        u.insert(u.end(), temp.begin(), temp.end());

        u.push_back(Angle(0.0));


        //push back into u the angle which corresponds to the endpoint of Route *this
        if (type == (Route_types[1])) {
            set_length_from_time_speed();
            u.push_back(Angle((length.value) / Re));
        }

        //push back into u the angle which corresponds to the endpoint of Route *this
        if (type == (Route_types[2])) {
            u.push_back(Angle(2.0 * M_PI));
            (u.back()).value = 2.0 * M_PI;
        }

        sort(u.begin(), u.end());


        //run over all chunks of *this in between the intersections and find out whether some fall within rectangle
        if (write_t) { t->resize(0); }
        for (output = 0, is_fully_included = true, i = 0; i < (u.size()) - 1; i++) {

            //compute the midpoint between two subsequesnt intersections, and write it into this->end. I use u[(i+1) % (u.size())] in such a way that, when i = u.size() -1, this equals u[0], because the last chunk that I want to consider is the one between the last and the first intersection
            if (type == (Route_types[1])) {
                compute_end(Length(Re * (((u[i]).value) + ((u[i + 1]).value)) / 2.0), String(""));
            }
            if (type == (Route_types[2])) {
                compute_end(Length(Re * sin(omega) * (((u[i]).value) + ((u[i + 1]).value)) / 2.0), String(""));
            }

            if (rectangle.Contains(end)) {
                //if rectangle contains the midpoint, then the chunk of *this with u[i] < t < u[1+1] is included in rectangle -> I return 1

                output = 1;

                //If write_t == true, write into t the value of the intersections which delimit the chunk of *this which is included in rectangle
                if (write_t) {

                    t->push_back(u[i]);
                    t->push_back(u[i + 1]);

                }

            }
            else {

                is_fully_included = false;

            }

        }

        if (write_t) {

            //I push back into t the last value of u, wich corresponds to the endpoint of *this  and which has not been pushed back by the loop above
            t->push_back(u.back());
            
  
            if ((type == (Route_types[2])) && is_fully_included && (t->size() == 2)) {
                //*this is  of type "circle fo equal altitude", its fully included in rectangle and it does not intersect rectangle

                //I set t[1].value = 0.0, so t[0].value = t[1].value = 0.0
                ((*t)[1]).normalize();

            }else{
                
                //delete duplicates from t
                delete_duplicates(t, &equal_rel_epsilon_double);                
                
            }


            
            //            set<Angle> t_temp(t->begin(), t->end());
            //            t->assign(t_temp.begin(), t_temp.end());
            //
        }

        return output;

    }


}


//If route is not a circle of equal altitide,  returs -1 (error code). Othwewise, a) If *this and route intersect,  return 1 and, if write_t = true, it also allocates t and it writes in t the  values of the parametric angles t of (*this), at which (*this) crosses route. b) If *this and route do not intersect, it returns 0 and does nothing with t c) if the type of *this is such that the intersection cannot be computed, it returns -1
int Route::intersection(Route route, bool write_t, vector<Angle>* t, [[maybe_unused]] String prefix) {

    String new_prefix;
    Angle t_a, t_b;
    Length d;
    Double cos_ts;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if ((route.type) == (Route_types[2])) {
        //route is a circle of equal altitude

        if (type == (Route_types[1])) {
            //*this is an orthodrome -> I check whether route and *this intersect: I compute the minimal distance between a point on *this and the GP (reference position) of route. I do this by checking the distance at the two extrema (at the beginning and at the end of *this), and by looking for an extremum in the middle of *this

            vector<Length> s(2);

            //case 1: the starting point (or reference_position) of *this
            reference_position.distance(route.reference_position, s.data(), String(""), prefix);

            //case 2: the end point of *this
            compute_end(prefix);
            end.distance(route.reference_position, (s.data()) + 1, String(""), prefix);

            cos_ts.set(String(""),
                (cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos((reference_position.phi)) * cos(((route.reference_position).phi)) + sin((reference_position.phi)) * sin(((route.reference_position).phi))) / sqrt(gsl_sf_pow_int(cos(((route.reference_position).phi)) * sin(Z) * sin((reference_position.lambda) - ((route.reference_position).lambda)) - cos(Z) * cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos(((route.reference_position).phi)) * sin((reference_position.phi)) + cos(Z) * cos((reference_position.phi)) * sin(((route.reference_position).phi)), 2) + gsl_sf_pow_int(cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos((reference_position.phi)) * cos(((route.reference_position).phi)) + sin((reference_position.phi)) * sin(((route.reference_position).phi)), 2)),
                prefix
            );

            //case 3:  extremum n. 1 in the middle of *this
            d.set(String(""), Re * acos(cos_ts.value), prefix);
            if (compute_end(d, prefix)) {

                s.resize(s.size() + 1);
                end.distance(route.reference_position, &(s.back()), String(""), prefix);

            }

            //case 4: extremum n. 2 in the middle of *this
            d.set(String(""), Re * (M_PI - acos(cos_ts.value)), prefix);
            if (compute_end(d, prefix)) {

                s.resize(s.size() + 1);
                end.distance(route.reference_position, &(s.back()), String(""), prefix);

            }

            //obtain the minimum distance across all cases, which may be 2, 3, or 4, and chekwhetehr it is smaller than Re * apertur angle of route
            if ((*min_element(s.begin(), s.end())) < Re * ((route.omega).value)) {
                //in this case, *this and route intersect: I compute the values of the parametric angle t which parametrizes *this and at which the distance betweeen (point on *this at t) and (GP of route) is equal to Re*(angular aperture of route)

                Double a, b, square_root, cos_t_p, cos_t_m;
                int output;


                a.set(String(""),
                    -(cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos((reference_position.phi)) * cos(((route.reference_position).phi))) - sin((reference_position.phi)) * sin(((route.reference_position).phi)),
                    prefix);

                b.set(String(""),
                    -(cos(((route.reference_position).phi)) * sin(Z) * sin((reference_position.lambda) - ((route.reference_position).lambda))) + cos(Z) * cos((reference_position.lambda) - ((route.reference_position).lambda)) * cos(((route.reference_position).phi)) * sin((reference_position.phi)) - cos(Z) * cos((reference_position.phi)) * sin(((route.reference_position).phi)),
                    prefix);


                square_root.set(String(""), sqrt(gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2) - gsl_sf_pow_int(cos(route.omega), 2)), String(""));

                //these are the values of cos(t) such that the distance between this->end at t  and route.reference_position equals Re*(route.omega), i.e., it is the value of cos(t) such that end(t) lies on route. There are two of them.
                cos_t_p.set(String(""), (-((a.value) * cos(route.omega)) + (square_root.value) * fabs((b.value))) / (gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2)), prefix);
                cos_t_m.set(String(""), (-((a.value) * cos(route.omega)) - (square_root.value) * fabs((b.value))) / (gsl_sf_pow_int((a.value), 2) + gsl_sf_pow_int((b.value), 2)), prefix);

                //this will be the output of this function: it is set to false for starters
                output = 0;

                //clear up t because I will write in i in what follows
                if (write_t) { t->clear(); }

                if ((/*when I solve the equations a cos t + b * sqrt(1-(cos t)^2)  = - cos(route.omega), I manipulate the euqation and then square both sides, thus introducing spurious solutions. This condition allows me to check which one among the spurious solutions is valid. */-((a.value) * (cos_t_p.value) + cos(route.omega)) / (b.value) > 0.0) && compute_end(Length(Re * acos(cos_t_p)), prefix)) {

                    if (write_t) {
                        t->resize((t->size()) + 1);
                        (t->back()).set(String(""), acos(cos_t_p), prefix);
                    }

                    //if I find a viable instersection point, I set output to 1
                    output = 1;

                    if (compute_end(Length(Re * (2.0 * M_PI - acos(cos_t_p))), prefix)) {

                        if (write_t) {
                            t->resize((t->size()) + 1);
                            (t->back()).set(String(""), 2.0 * M_PI - acos(cos_t_p), prefix);
                        }

                        //if I find a viable instersection point, I set output to true
                        output = 1;

                    }

                }

                if ((/*when I solve the equations a cos t + b * sqrt(1-(cos t)^2)  = - cos(route.omega), I manipulate the euqation and then square both sides, thus introducing spurious solutions. This condition allows me to check which one among the spurious solutions is valid. */-((a.value) * (cos_t_m.value) + cos(route.omega)) / (b.value) > 0.0) && compute_end(Length(Re * acos(cos_t_m)), prefix)) {

                    if (write_t) {
                        t->resize((t->size()) + 1);
                        (t->back()).set(String(""), acos(cos_t_m), prefix);
                    }

                    //if I find a viable instersection point, I set output to 1
                    output = 1;

                    if (compute_end(Length(Re * (2.0 * M_PI - acos(cos_t_m))), prefix)) {

                        if (write_t) {
                            t->resize((t->size()) + 1);
                            (t->back()).set(String(""), 2.0 * M_PI - acos(cos_t_m), prefix);
                        }

                        //if I find a viable instersection point, I set output to 1
                        output = 1;

                    }

                }

                return output;

            }
            else {
                //in this case, *this and route do not intersect

                return 0;

            }

        }
        else {

            if (type == (Route_types[2])) {
                //*this is a circle of equal altitude -> I check check whetehr *this and route intersect

                reference_position.distance(route.reference_position, &d, String(""), new_prefix);

                if (/*this is the condition that *this and route intersect*/(d > Re * fabs((omega.value) - ((route.omega).value))) && (d < Re * ((omega + (route.omega)).value))) {
                    //in this case, *this and route intersect

                    if (write_t) {

                        t->resize(2);

                        if (((route.reference_position.phi) != M_PI_2) && ((route.reference_position.phi) != 3.0 * M_PI_2)) {
                            //theg general case where route.reference_position.phi != +-pi/2

                            t_a.value = atan((8 * cos((route.reference_position).phi) * ((cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi)) - cos(((reference_position).phi)) * sin((route.reference_position).phi)) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                abs(sin((reference_position.lambda) - (route.reference_position.lambda))) * cos((route.reference_position).phi) * sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) +
                                    2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                    gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) - 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                    2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2)))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))), (8 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) *
                                        sin((reference_position.lambda) - (route.reference_position.lambda)) - 8 * abs(sin((reference_position.lambda.value) - (route.reference_position.lambda.value))) * cos((route.reference_position).phi) * csc((reference_position.lambda.value) - (route.reference_position.lambda.value)) * (cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi)) - cos(((reference_position).phi)) * sin((route.reference_position).phi)) *
                                        sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) + 2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) -
                                            gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) + 2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                            gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) -
                                            2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                            gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))));


                            t_b.value = atan((-8 * cos((route.reference_position).phi) * ((-(cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi))) + cos(((reference_position).phi)) * sin((route.reference_position).phi)) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                abs(sin((reference_position.lambda) - (route.reference_position.lambda))) * cos((route.reference_position).phi) * sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) +
                                    2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                    gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) - 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                    2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) +
                                    gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2)))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))), (8 * cos((route.reference_position).phi) * sin((reference_position.lambda) - (route.reference_position.lambda)) *
                                        (cos((route.reference_position).phi) * (cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * cot(omega) - cos(route.omega) * csc(omega) + cot(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                            abs(sin((reference_position.lambda) - (route.reference_position.lambda))) * gsl_sf_pow_int(csc((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * (cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * sin(((reference_position).phi)) - cos(((reference_position).phi)) * sin((route.reference_position).phi)) *
                                            sqrt(-(gsl_sf_pow_int(cos(route.omega), 2) * gsl_sf_pow_int(csc(omega), 2)) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) +
                                                2 * cos(route.omega) * cot(omega) * csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi) - gsl_sf_pow_int(cot(omega), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                                2 * cos(((reference_position).phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) * csc(omega) * (cos(route.omega) * cot(omega) - csc(omega) * sin(((reference_position).phi)) * sin((route.reference_position).phi)) +
                                                gsl_sf_pow_int(cos(((reference_position).phi)), 2) * (-(gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position.lambda.value) - (route.reference_position.lambda.value)), 2) * gsl_sf_pow_int(cot(omega), 2)) + gsl_sf_pow_int(sin((route.reference_position).phi), 2)) +
                                                gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((route.reference_position).lambda), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) - 2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * sin((reference_position).lambda) * sin((route.reference_position).lambda) +
                                                2 * gsl_sf_pow_int(cos((route.reference_position).phi), 2) * cos((reference_position).lambda) * cos((route.reference_position).lambda) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * sin((reference_position).lambda) * sin((route.reference_position).lambda) + gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(cos((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2) +
                                                gsl_sf_pow_int(cos((route.reference_position).phi), 2) * gsl_sf_pow_int(sin(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((reference_position).lambda), 2) * gsl_sf_pow_int(sin((route.reference_position).lambda), 2)))) /
                                (gsl_sf_pow_int(cos((route.reference_position).phi), 2) * (-6 + 2 * cos(2 * (reference_position.phi.value)) + 2 * cos(2 * (reference_position.lambda.value) - 2 * (route.reference_position.lambda.value)) + cos(2 * ((reference_position.phi.value) + (reference_position.lambda.value) - (route.reference_position.lambda.value))) + cos(2 * ((reference_position.phi.value) - (reference_position.lambda.value) + (route.reference_position.lambda.value)))) - 8 * gsl_sf_pow_int(cos(((reference_position).phi)), 2) * gsl_sf_pow_int(sin((route.reference_position).phi), 2) +
                                    4 * cos((reference_position.lambda) - (route.reference_position.lambda)) * sin(2 * (reference_position.phi.value)) * sin(2 * (route.reference_position.phi.value))));

                        }
                        else {
                            //the special case where  route.reference_position.phi = +- pi/2

                            t_a.set(String(""), acos(-GSL_SIGN((((route.reference_position).phi).normalize_pm_pi_ret()).value) * (cos(route.omega) * csc(omega) * sec(reference_position.phi)) + cot(omega) * tan(reference_position.phi)), String(""));
                            t_b.set(String(""), -acos(-GSL_SIGN((((route.reference_position).phi).normalize_pm_pi_ret()).value) * (cos(route.omega) * csc(omega) * sec(reference_position.phi)) + cot(omega) * tan(reference_position.phi)), String(""));


                        }

                        //normalize t_a and t_b to put them in a proper form and then properly compare their values
                        t_a.normalize();
                        t_b.normalize();

                        //write t_a, t_b in t by sorting them in ascending order.
                        if (t_a < t_b) {

                            ((*t)[0]).set(String(""), (t_a.value), new_prefix);
                            ((*t)[1]).set(String(""), (t_b.value), new_prefix);

                        }
                        else {

                            ((*t)[0]).set(String(""), (t_b.value), new_prefix);
                            ((*t)[1]).set(String(""), (t_a.value), new_prefix);

                        }

                    }

                    return 1;

                }
                else {
                    //in this case, *this and route do not intersect

                    return 0;

                }

            }
            else {

                if (type == (Route_types[0])) {

                    cout << new_prefix.value << RED << "Route is a loxodrome, I cannot compute intersection for loxodromes!\n" << RESET;

                    return -1;

                }
                else {

                    cout << new_prefix.value << RED << "Route type is invalid, I cannot compute intersection!\n" << RESET;

                    return -1;

                }

            }

        }

    }
    else {
        //in this case, *this and route are not circles of equal altitude

        cout << prefix.value << "Route is not a circle of equal altitude: I can only compute intersections if *this is a circle of equal altitude!\n";

        return -1;

    }

}

//reads from file the content after 'Route = ...' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Route::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Route>(this, name, filename, mode, prefix);

}


//read *this from the stream *input_stream. Here search_entire_stream is provided as argument only to match the argument pattern of read_from_stream in other classes, and it is not used (this function reads the Route at the current position of *input_stream)
template<class S> void Route::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    String new_prefix;
    string line;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    type.read_from_stream<S>(String("type"), input_stream, false, new_prefix);

    line.clear();
    getline(*input_stream, line);


    if (type == Route_types[2]) {

        reference_position.read_from_stream<S>(String("reference position"), input_stream, false, new_prefix);
        omega.read_from_stream<S>(String("omega"), input_stream, false, new_prefix);
        length.set(String("length"), 2.0 * M_PI * Re * sin(omega), new_prefix);

    }
    else {

        reference_position.read_from_stream<S>(String("reference position"), input_stream, false, new_prefix);

        Z.read_from_stream<S>(String("starting heading"), input_stream, false, new_prefix);

        length_format.read_from_stream(String("length format"), input_stream, false, new_prefix);

        if (length_format == (LengthFormat_types[0])) {
            //read time and speed, and set l accordingly

            time.read_from_stream(String("time"), input_stream, false, new_prefix);
            speed.read_from_stream(String("speed"), input_stream, false, new_prefix);
            set_length_from_time_speed();

        }
        else {

            length.read_from_stream<S>(String("length"), input_stream, false, new_prefix);

        }


    }

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }


    //when a sight is read from file, it is not yet linked to any route, thus I set
    (related_sight.value) = -1;

}


//this function computes the crossings between Route (*this) and Route route: it writes the two crossing points in p, and the cosing of the crossing angle in cos_crossing_angle. If the intersection cannot be computed it returns -1 (error code), othwerwise it returns 1 (0) if the Routes intersect (do not interesect).
int Route::crossing(Route route, vector<Position>* p, double* cos_crossing_angle, [[maybe_unused]] String prefix) {

    //these are the two lengths along Route (*this) which correspond to the two crossings with route
    String new_prefix;
    bool check;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    if (!((type == (Route_types[2])) && (route.type == (Route_types[2])))) {

        cout << prefix.value << "Routes are not circles of equal altitude: this code only computes intersects between circles of equal altitudes\n";
        return (-1);

    }
    else {

        Angle theta, t_temp;
        Length r, s;

        check = true;

        theta.set(String("angle between the two GPs"), acos(cos((reference_position.phi)) * cos((route.reference_position).phi) * cos((reference_position.lambda.value) - (route.reference_position.lambda.value)) + sin((reference_position.phi)) * sin((route.reference_position).phi)), prefix);

        if ((abs(((*this).omega.value) - (route.omega.value)) < (theta.value)) && ((theta.value) < ((*this).omega.value) + (route.omega.value))) {
            //in this case routes intersect

            //t contains the parametric angle of Route (*this) where (*this) crosses route
            //u contains the parametric angle of Route route where route crosses (*this)
            vector<Angle> t, u;

            cout << prefix.value << "Routes intersect\n";

            intersection(route, true, &t, new_prefix);
            route.intersection((*this), true, &u, new_prefix);

            (*this).compute_end(Length(Re * sin((*this).omega.value) * ((t[0]).value)), new_prefix);
            (*p)[0] = end;
            ((*p)[0]).label.set(String(""), String("crossing"), prefix);

            (*this).compute_end(Length(Re * sin((*this).omega.value) * ((t[1]).value)), new_prefix);
            (*p)[1] = end;
            ((*p)[1]).label.set(String(""), String("crossing"), prefix);

            route.compute_end(Length(Re * sin(route.omega.value) * ((u[0]).value)), prefix);

            check &= ((*p)[0]).distance(route.end, &r, String(""), prefix);
            check &= ((*p)[1]).distance(route.end, &s, String(""), prefix);

            if (check) {

                if (r > s) {

                    cout << new_prefix.value << "Exchanging ts!\n";

                    t_temp = u[0];
                    u[0] = u[1];
                    u[1] = t_temp;

                }

                (*this).compute_end(Length(Re * sin((*this).omega.value) * ((t[0]).value)), prefix);
                end.print(String("position of intersection 1 for Route 1"), prefix, cout);

                route.compute_end(Length(Re * sin(route.omega.value) * ((u[0]).value)), prefix);
                (route.end).print(String("position of intersection 1 for Route 2"), prefix, cout);

                (*cos_crossing_angle) = cos((reference_position.phi)) * cos((route.reference_position).phi) * sin(t[0]) * sin(u[0]) + (cos(t[0]) * sin(reference_position.lambda) - cos(reference_position.lambda) * sin((reference_position.phi)) * sin(t[0])) * (cos(u[0]) * sin((route.reference_position).lambda) - cos((route.reference_position).lambda) * sin((route.reference_position).phi) * sin(u[0])) +
                    (cos(reference_position.lambda) * cos(t[0]) + sin((reference_position.phi)) * sin(reference_position.lambda) * sin(t[0])) * (cos((route.reference_position).lambda) * cos(u[0]) + sin((route.reference_position).phi) * sin((route.reference_position).lambda) * sin(u[0]));

                cout << prefix.value << "cos(angle 1 between tangents) = " << (*cos_crossing_angle) << "\n";

                t.clear();
                u.clear();

            }

        }
        else {
            //routes do not intersect

            cout << prefix.value << "Routes do no intersect\n";
            check &= false;

        }

        if (check) { return 1; }
        else { return 0; }

    }


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


bool Chrono::operator==(const Chrono& chrono) {

    return((h == (chrono.h)) && (m == (chrono.m)) && (s == (chrono.s)));

}

bool Chrono::operator!=(const Chrono& chrono) {

    return (!((*this) == chrono));

}

bool Chrono::operator<(const Chrono& chrono) {

    Chrono temp;

    temp = chrono;

    return((this->get()) < (temp.get()));

}

bool Chrono::operator>(const Chrono& chrono) {

    Chrono temp;

    temp = chrono;

    return((this->get()) > (temp.get()));

}

//sets the Chrono object to the time x, which is expressed in hours
bool Chrono::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;
    bool check;

    check = true;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    check &= (x >= 0.0);
    if (check) {

        h = ((unsigned int)floor(x));
        m = (unsigned int)((x - ((double)h)) * 60.0);
        s = (((x - ((double)h)) * 60.0) - ((double)m)) * 60.0;

        if (name != String("")) { print(name, prefix, cout); }

    }
    else {

        cout << new_prefix.value << RED << "Set value is not valid!\n" << RESET;

    }

    return check;


}

//evaluates whether Time (*this) is equal to t
bool Time::operator==(const Time& t) {

    Time temp;

    (*this).to_MJD();
    temp = t;
    temp.to_MJD();

    return((((*this).MJD) == (temp.MJD)));

}

//evaluates whether Time (*this) is different from t
bool Time::operator!=(const Time& t) {

    return(!((*this) == t));

}




//evaluates whether Time (*this) is larger than t
bool Time::operator>(const Time& t) {

    Time temp;

    (*this).to_MJD();
    temp = t;
    temp.to_MJD();

    return((((*this).MJD) > (temp.MJD)));

}

//convert *this to a string by adding the time zone if time_zone = true
string Time::to_string(unsigned int precision, bool time_zone) {

    stringstream output;

    //I deleted UTC at the end of string for the sake of shortness
    output << date.to_string() << " " << chrono.to_string(precision, time_zone);

    return (output.str().c_str());

}

string Position::to_string(unsigned int precision) {

    stringstream output;

    output << phi.to_string(String("NS"), precision, true) << " " << lambda.to_string(String("EW"), precision, true);

    return (output.str().c_str());

}


void Time::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    date.print(String("date"), new_prefix, ostr);
    chrono.print(String("hour"), new_prefix, ostr);

};


void Time::to_TAI(void) {
    //int &day, int &month, int &year, double &hour)
    /*
     Calculate the calendar date from the Modified Julian Date

     INPUT :
     MJD : Modified Julian Date (Julian Date - 2400000.5)

     OUTPUT :
     day, month, year : corresponding date
     hour : corresponding hours of the above date
     */

    int Yt = 0, Mt = 0, Dt = 0;
    double ht;

    long int b, c, d, e, f, jd0;

    jd0 = long(MJD + 2400001.0);
    if (jd0 < 2299161) c = jd0 + 1524;    /* Julian calendar */
    else
    {                                /* Gregorian calendar */
        b = long((jd0 - 1867216.25) / 36524.25);
        c = jd0 + b - (b / 4) + 1525;
    };

    if (MJD < -2400001.0)  // special case for year < -4712
    {
        if (MJD == floor(MJD)) jd0 = jd0 + 1;
        c = long((-jd0 - 0.1) / 365.25);
        c = c + 1;
        Yt = -4712 - c;
        d = c / 4;
        d = c * 365 + d;  // number of days from JAN 1, -4712
        f = d + jd0;  // day of the year
        if ((c % 4) == 0) e = 61;
        else e = 60;
        if (f == 0)
        {
            Yt = Yt - 1;
            Mt = 12;
            Dt = 31;
            f = 500;  // set as a marker
        };
        if (f < e)
        {
            if (f < 32)
            {
                Mt = 1;
                Dt = f;
            }
            else
            {
                Mt = 2;
                Dt = f - 31;
            };
        }
        else
        {
            if (f < 500)
            {
                f = f - e;
                Mt = long((f + 123.0) / 30.6001);
                Dt = f - long(Mt * 30.6001) + 123;
                Mt = Mt - 1;
            };
        };
    }
    else   // normal case
    {
        d = long((c - 122.1) / 365.25);
        e = 365 * d + (d / 4);
        f = long((c - e) / 30.6001);
        Dt = c - e - long(30.6001 * f);
        Mt = f - 1 - 12 * (f / 14);
        Yt = d - 4715 - ((7 + Mt) / 10);
    };

    ht = 24.0 * (MJD - floor(MJD));


    (date.Y) = ((unsigned int)Yt);
    (date.M) = ((unsigned int)Mt);
    (date.D) = ((unsigned int)Dt);

    (chrono.h) = (unsigned int)(floor(ht));
    (chrono.m) = floor((ht - floor(ht)) * 60.0);
    (chrono.s) = (((ht - floor(ht)) * 60.0) - floor((ht - floor(ht)) * 60.0)) * 60.0;

}

void Time::to_MJD(void)

/*
 Modified Julian Date ( MJD = Julian Date - 2400000.5)
 valid for every date
 Julian Calendar up to 4-OCT-1582,
 Gregorian Calendar from 15-OCT-1582.
 */
{

    long int b, c;
    int Yt = (date.Y), Mt = (date.M), Dt = (date.D);

    MJD = 10000.0 * Yt + 100.0 * Mt + Dt;
    if (Mt <= 2)
    {
        Mt = Mt + 12;
        Yt = Yt - 1;
    };
    if (MJD <= 15821004.1)
    {
        b = ((Yt + 4716) / 4) - 1181;
        if (Yt < -4716)
        {
            c = Yt + 4717;
            c = -c;
            c = c / 4;
            c = -c;
            b = c - 1182;
        };
    }
    else b = (Yt / 400) - (Yt / 100) + (Yt / 4);
    //     { b = -2 + floor((Yt+4716)/4) - 1179;}
    // else {b = floor(Yt/400) - floor(Yt/100) + floor(Yt/4);};

    MJD = 365.0 * Yt - 679004.0;
    //comment this out if you want to include hours, minutes and seconds in MJD
    MJD = MJD + b + int(30.6001 * (Mt + 1)) + Dt + (chrono.get()) / 24.0;


}


//transport the Position *this with the Route route and write the result in *this. If route is not a circle of equal altitude, the transport is done and true is returned, otherwise no transport is done and false is returned.
bool Position::transport_to(Route route, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    if ((route.type) != (Route_types[2])) {
        //route.type = ((Route_types[0]).value) or ((Route_types[1]).value) -> I can transport *this

        Route temp;

        temp = route;
        (temp.reference_position) = (*this);
        temp.compute_end(new_prefix);
        (*this) = temp.end;

        return true;

    }
    else {
        //route.type = "circle fo equal altitude" -> I cannot transport *this

        cout << prefix.value << RED << "Cannot transport a position with a circle of equal altitude!\n" << RESET;

        return false;

    }

}


//check whether phi and lambda are equal to phi_min ... lambda_max within accuracy epsilon_double and, if they are, set them equal to phi_min ... lambda_max, respectively
inline void Position::put_back_in(DrawPanel* draw_panel){
        
    switch (position_in_vector(Projection((draw_panel->parent->projection->name->GetValue()).ToStdString()), Projection_types)) {
            
        case 0: {
            //I am using the mercator projection
            
            if(equal_rel_epsilon_double(phi, draw_panel->parent->phi_min)){
                phi = (draw_panel->parent->phi_min);
            }
            
            if(equal_rel_epsilon_double(phi, draw_panel->parent->phi_max)){
                phi = (draw_panel->parent->phi_max);
            }
            
            if(equal_rel_epsilon_double(lambda, draw_panel->parent->lambda_min)){
                lambda = (draw_panel->parent->lambda_min);
            }
            
            if(equal_rel_epsilon_double(lambda, draw_panel->parent->lambda_max)){
                lambda = (draw_panel->parent->lambda_max);
            }
            
            break;
            
        }
            
        case 1: {
            //I am using the 3D projection
            
        }
            
    }
    
    
    
}


//return the antipode of *this on the earth
Position Position::half(void){
    
    Position result;
    
    result.lambda = lambda/2.0;
    result.phi = phi/2.0;
    
    return result;
    
}

//return the antipode (with respect to the longitude only) of *this on the earth 
Position Position::antipode_lambda(void){
    
    Position result;
    
    result.lambda = lambda + M_PI;
    result.phi = phi;
    
    return result;
    
}


//transport the Position *this with the Route route and write the result in *result. If route is not a circle of equal altitude, the transport is done and true is returned, otherwise no transport is done and false is returned. result needs to be allocated before this method is called
bool Position::transport(Position* result, Route route, [[maybe_unused]] String prefix) {
        
    (*result) = (*this);

    return((result->transport_to(route, prefix)));

}


//rotates the Position (*this) according to the Rotation s, and writes the result in *p
void Position::rotate(const String& name, const Rotation& r, Position* p, [[maybe_unused]] const String& prefix) {

    Cartesian u, s;

    //write (*this) into u in cartesian coordinates
    gsl_vector_set((u.r), 0, cos(lambda) * cos(phi));
    gsl_vector_set((u.r), 1, -(cos(phi) * sin(lambda)));
    gsl_vector_set((u.r), 2, sin(phi));

    //rotate u according to r and write the result in s and then in (*this)
    //    gsl_blas_dgemv(CblasNoTrans, 1.0, (r.matrix), (u.r), 0.0, (s.r));
    cblas_dgemv(CblasRowMajor, CblasNoTrans, 3, 3, 1, r.matrix->data, 3, u.r->data, 1, 0, s.r->data, 1);


    //     cout << "\tNorm of u = " << gsl_blas_dnrm2(u);
    //     cout << "\tNorm of s = " << gsl_blas_dnrm2(s);

    p->setCartesian(name, s, prefix);

}


inline Cartesian::Cartesian(void) {

    r = gsl_vector_alloc(3);

}


//construct *this setting its coordinates from the coordinates of the geographic Position p
inline Cartesian::Cartesian(const Position& p) {

    r = gsl_vector_alloc(3);

    gsl_vector_set(r, 0, cos(p.phi) * cos(p.lambda));
    gsl_vector_set(r, 1, -cos(p.phi) * sin(p.lambda));
    gsl_vector_set(r, 2, sin(p.phi));


}


//set *this from the grographic Position p
inline void Cartesian::setPosition(const Position& p){
    

    gsl_vector_set(r, 0, cos(p.phi) * cos(p.lambda));
    gsl_vector_set(r, 1, -cos(p.phi) * sin(p.lambda));
    gsl_vector_set(r, 2, sin(p.phi));
    
    
}


//return the dot product between *this and s
inline double Cartesian::dot(const Cartesian& s){
    
    double result;
    
    gsl_blas_ddot(r, s.r, &result);
    
    return result;
    
}


//return the cross product between the vector of this and that of s
inline Cartesian Cartesian::cross(const Cartesian& s){
    
    Cartesian result;
    
    my_cross(r, s.r, &(result.r));
    
    return result;
    
}




void Cartesian::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << ": {" <<
        gsl_vector_get(r, 0) << " , " <<
        gsl_vector_get(r, 1) << " , " <<
        gsl_vector_get(r, 2) << " }\n";

}


//copies the content of x.r into this->r. Be careful: without this operator being defined, if you sed a Cartesian a equal to a Cartesian b, the memory adress of a.r is set equal to the memory address of b.r -> as soon as b.r is changed, a.r will be changed as well
void Cartesian::operator = (const Cartesian& x) {

    gsl_vector_memcpy(r, x.r);

}


//read from stream input_stream the Position by starting at the current position of input_stream. Here name and search_entire_stream are unused and have been included as arguments of the function in order to match with the format of read_from_stream of other classes and so in order to use template<class C> void read_from_file(C* object, String name, String filename, [[maybe_unused]] String prefix) throughout the code
template<class S> void Position::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    phi.read_from_stream<S>(String("latitude"), input_stream, false, new_prefix);
    lambda.read_from_stream<S>(String("longitude"), input_stream, false, new_prefix);

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }

}


//set length according to time and speed if the lenght format for *this is  time * speed
void Route::set_length_from_time_speed(void){
    
    if(length_format == LengthFormat_types[0]){
        
        length = Length(time, speed);
        
    }
    
}


//set length equal to l(t), where l(t) is the value of the curvilinear length corresponding to the parametric coordinate t
void Route::set_length_from_input(double t){
    
    switch ( type.position_in_list(Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            double C, eta;
        
            C = gsl_pow_2(cos(Z));
            eta = sqrt((1-sin((reference_position.phi)))/(1+sin((reference_position.phi))));

            //set the length format, the length unit and the value of the length from t
            length_format.set(String(""), LengthFormat_types[1], String(""));
            length.unit.set(String(""), LengthUnit_types[0], String(""));
            
            if(fabs(C) > epsilon_double){
                //I am not in the special case where Z = pi/2 or 3 pi /2 (i.e., C = 0)
                
                double s;
                
                s = GSL_SIGN(cos(Z));
                length.set(
                           String(""),
                           s * 2.0*Re/sqrt(C) *( atan(eta) - atan( eta * exp(- s * sqrt(C/(1.0-C)) * t ) ) ),
                           String(""));
                
            }else{
                //I am in the special case where Z = pi/2 or 3 pi /2 (i.e., C = 0) -> set the length by using the analytical limit C->0 for  expression of the length
                
                length.set(
                           String(""),
                           2.0*Re*t*eta/(1.0+gsl_pow_2(eta)),
                           String(""));
                
            }
            
            break;
            
        }
            
        case 1:{
            //*this is an orthodrome
            
            
            break;
            }
            
        case 2:{
            //*this is a circle of equal altitude
            
            break;
            
        }
            
    }
    
    
}


//write into this->end the Position on the Route at length this->length (which needs to be correclty set before this method is called) along the Route from start
void Route::compute_end(String prefix) {
    
    //picks the first (and only) character in string type.value
    switch (type.position_in_list(Route_types)) {
            
            
        case 0:{
            //loxodrome route
            
            //this is the +- sign appearing in \phi'(t)  = +- sqrt{C/(1-C)} cos(phi(t));
            int sigma, tau;
            double C, eta;
            Angle t;
            
            
            eta = sqrt((1.0 - sin(reference_position.phi.value)) / (1.0 + sin(reference_position.phi.value)));
            
            //tau = +-_{notes}
            if (((0.0 <= (Z.value)) && ((Z.value) < M_PI_2)) || ((3.0 * M_PI_2 <= (Z.value)) && ((Z.value) < 2.0 * M_PI))) { tau = +1; }
            else { tau = -1; }
            
            if ((0.0 <= (Z.value)) && ((Z.value) < M_PI)) { sigma = -1; }
            else { sigma = +1; }
            
            C = gsl_pow_2(cos(Z));
            
            /* cout << "sigma = " << sigma << "\n"; */
            /* cout << "tau = " << tau << "\n"; */
            /* cout << "C = " << C << "\n"; */
            
            if (((Z.value) != M_PI_2) && ((Z.value) != 3.0 * M_PI_2)) {
                //this is the general expression of t vs l for Z != pi/2
                
                (t.value) = -tau * sqrt((1.0 - C) / C)
                * log(1.0 / eta * tan(-tau * sqrt(C) * (length.value) / (2.0 * Re) + atan(sqrt((1.0 - sin(reference_position.phi.value)) / (1.0 + sin(reference_position.phi.value))))));
                
            }
            else {
                //this is the limit of the expression above in the case Z -> pi/2
                
                (t.value) = (length.value) * (1.0 + gsl_pow_2(eta)) / (2.0 * Re * eta);
                
            }
            
            /* t.print("t", prefix, cout); */
            
            (end.phi).set(String(""), asin(tanh(tau * sqrt(C / (1.0 - C)) * (t.value) + atanh(sin(reference_position.phi.value)))), prefix);
            
            (end.lambda).set(String(""), (reference_position.lambda.value) + sigma * (t.value), prefix);
            
            break;
            
        }
            
        case 1:{
            
            //orthodrome route
            Angle t;
            
     
            t.set(String(""), (length.value) / Re, prefix);
            
            (end.phi).set(String(""), asin(cos(Z) * cos(reference_position.phi) * sin(t) + cos(t) * sin(reference_position.phi)), prefix);
            (end.lambda).set(String(""),
                             -atan(cos(t) * cos(reference_position.lambda) * cos(reference_position.phi) + sin(t) * (sin(Z) * sin(reference_position.lambda) - cos(Z) * cos(reference_position.lambda) * sin(reference_position.phi))
                                   ,
                                   (cos(reference_position.lambda) * sin(t) * sin(Z) + sin(reference_position.lambda) * (-cos(t) * cos(reference_position.phi) + cos(Z) * sin(t) * sin(reference_position.phi)))),
                             prefix);
            
            break;
            
        }
            
        case 2:{
            
            Angle t;
            //compute the parametric angle for the circle of equal altitude starting from the length l of the curve, omega  and the Earth's radius
            //R sin omega = r, r t = l, t = l / (R sin omega)
            
            //compute the length of *this from time and speed, if the length is stored in *this as a time * speed
            set_length_from_time_speed();
            t.set(String(""), (length.value) / (Re * sin(omega)), prefix);
            
            
            (end.phi).set(String(""), M_PI_2 - acos(cos((omega.value)) * sin(reference_position.phi) - cos(reference_position.phi) * cos((t.value)) * sin((omega.value))), prefix);
            
            (end.lambda).set(String(""), -(atan((-sin(reference_position.lambda) * (cos(reference_position.phi) * cos((omega.value)) + cos((t.value)) * sin(reference_position.phi) * sin((omega.value))) + cos(reference_position.lambda) * sin((omega.value)) * sin((t.value))) / (cos(reference_position.phi) * cos(reference_position.lambda) * cos((omega.value)) + sin((omega.value)) * (cos(reference_position.lambda) * cos((t.value)) * sin(reference_position.phi) + sin(reference_position.lambda) * sin((t.value)))))), prefix);
            if (cos(reference_position.phi) * cos(reference_position.lambda) * cos((omega.value)) + sin((omega.value)) * (cos(reference_position.lambda) * cos((t.value)) * sin(reference_position.phi) + sin(reference_position.lambda) * sin((t.value))) <= 0.0) {
                (end.lambda) -= M_PI;
            }
            
            break;
            
        }
            
    }
    
    (end.label.value) = "";
    
}


//This is an overload of compute_end: if d <= (this->l), it writes into this->end the position on the Route at length d along the Route from start and it returns true. If d > (this->l), it returns false
bool Route::compute_end(Length d, [[maybe_unused]] String prefix) {
    
    set_length_from_time_speed();

    if ((type == (Route_types[2])) || (d <= length)) {

        Length l_saved;

        l_saved = length;
        length = d;
        compute_end(prefix);
        length = l_saved;

        return true;

    }
    else {

        //        cout << prefix.value << RED << "Length is larger than Route length!\n" << RESET;

        return false;

    }

}



bool Body::operator==(const Body& body) {

    return (name == (body.name));

}

bool Limb::operator==(const Limb& limb) {

    return(value == limb.value);

}


void Route::print(String name, String prefix, ostream& ostr) {

    String s, new_prefix, new_new_prefix;

    //append \t to prefix and \t\t to new_new_prefix
    new_prefix = prefix.append(String("\t"));
    new_new_prefix = new_prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    type.print(String("type"), true, new_prefix, ostr);

    if ((type == (Route_types[0])) || (type == (Route_types[1]))) {

        reference_position.print(String("start position"), new_prefix, ostr);
        Z.print(String("starting heading"), new_prefix, ostr);

        length_format.print(String("length format"), false, new_prefix, ostr);
        if (length_format == (LengthFormat_types[1])) {

            length.print(String("length"), String("nm"), new_new_prefix, ostr);

        }else {

            time.print(String("time"), new_new_prefix, ostr);
            speed.print(String("speed"), new_new_prefix, ostr);

        }

    }
    else {

        reference_position.print(String("ground position"), new_prefix, ostr);
        omega.print(String("aperture angle"), new_prefix, ostr);

    }

    label.print(String("label"), true, new_prefix, ostr);


    if ((related_sight.value) != -1) {

        cout << new_prefix.value << "Related sight # = " << (related_sight.value) + 1 << "\n";

    }

}


template<class S> bool Chrono::read_from_stream([[maybe_unused]] String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    bool check = true;
    size_t pos;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    pos = 0;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while ((line.find(name.value)) == (string::npos));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }



    pos = line.find(" = ");

    //read hours
    h = stoi(line.substr(pos + 3, 2).c_str(), NULL, 10);
    if (!((0 <= h) && (h < 24))) {

        check &= false;
        cout << prefix.value << RED << "\tValue of hh is not valid!\n" << RESET;

    }

    //read minutes
    m = stoi(line.substr(pos + 3 + 3, 2).c_str(), NULL, 10);
    if (!((0 <= m) && (m < 60))) {

        check &= false;
        cout << prefix.value << RED << "\tValue of mm is not valid!\n" << RESET;

    }

    //read seconds
    s = stod(line.substr(pos + 3 + 3 + 3, line.size() - (pos + 3 + 3 + 3)).c_str());
    if (!((0.0 <= s) && (s < 60.0))) {

        check &= false;
        cout << prefix.value << RED << "\tValue of mm is not valid!\n" << RESET;

    }

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}

//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Chrono::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Chrono>(this, name, filename, mode, prefix);

}

//this function returns true if the date read is consistent, false if it is not
template<class S> bool Date::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    bool check = true;
    size_t pos;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    pos = 0;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }

    pos = line.find(" = ");

    Y = stoi(line.substr(pos + 3, 4).c_str(), NULL, 10);

    check_leap_year();
    if ((Y_is_leap_year)) {
        (days_per_month) = days_per_month_leap;
        cout << new_prefix.str() << YELLOW << "Entered a leap year\n" << RESET;
    }
    else {
        (days_per_month) = days_per_month_common;
        cout << new_prefix.str() << "Entered a common year\n";
    }

    M = stoi(line.substr(pos + 3 + 5, 2).c_str(), NULL, 10);

    if (!((1 <= M) && (M < 12 + 1))) {
        check &= false;
        cout << new_prefix.str() << RED << "\tValue of MM is not valid!\n" << RESET;
    }

    D = stoi(line.substr(pos + 3 + 5 + 3, 2).c_str());

    if (!((1 <= D) && (D < days_per_month[M - 1] + 1))) {
        check &= false;
        cout << prefix.value << RED << "\tValue of DD is not valid!\n" << RESET;
    }

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}









void Date::check_leap_year(void) {

    if ((Y % 4) != 0) {

        Y_is_leap_year = false;

    }
    else {

        if ((Y % 100) != 0) {

            Y_is_leap_year = true;

        }
        else {
            if ((Y % 400) != 0) {

                Y_is_leap_year = false;

            }
            else {

                Y_is_leap_year = true;

            }

        }

    }

}

template<class S> bool Time::read_from_stream(String name, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    //read dummy line
    getline((*input_stream), line);

    cout << prefix.value << name.value << ":\n";

    //read date
    if (!(date.read_from_stream<S>(String("date"), input_stream, false, new_prefix))) {
        check &= false;
    }

    //read chrono
    if (!(chrono.read_from_stream<S>(String("hour"), input_stream, false, new_prefix))) {
        check &= false;
    }

    to_MJD();

    if (check) {
        print(name, prefix, cout);
    }

    return check;

}

void Time::operator += (const Chrono& chrono_in) {

    Chrono temp;

    temp = chrono_in;

    to_MJD();
    MJD += temp.get() / 24.0;
    to_TAI();

}


void Time::operator -= (const Chrono& chrono_in) {

    Chrono temp;

    temp = chrono_in;


    to_MJD();
    MJD -= temp.get() / 24.0;
    to_TAI();

}


bool Length::check_valid(String name, [[maybe_unused]] String prefix) {

    bool check = true;

    if (value < 0.0) {
        check &= false;
        cout << prefix.value << RED << "Entered value of " << name.value << " is not valid!\n" << RESET;
    }

    return check;

}

//reads from file the content after 'name = ' and writes it into this. This function requires file to be correctly set and open
template<class S> void Length::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    stringstream new_prefix;
    size_t pos1, pos2;
    String unit_temp;

    //prepend \t to prefix
    new_prefix << "\t" << prefix.value;

    cout << prefix.value << YELLOW << "Reading " << name.value << " from stream " << input_stream << " ...\n" << RESET;

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

    pos1 = line.find(" = ");
    pos2 = line.find(" nm");

    if (line.find(" nm") != (string::npos)) {
        //in this case the units of the length read is nm
        cout << prefix.value << "Unit is in nm\n";
        pos2 = line.find(" nm");
        unit_temp = String("nm");
    }
    if (line.find(" m") != (string::npos)) {
        //in this case the units of the length read is m
        cout << prefix.value << "Unit is in m\n";
        pos2 = line.find(" m");
        unit_temp = String("m");
    }
    if (line.find(" ft") != (string::npos)) {
        //in this case the units of the length read is ft
        cout << prefix.value << "Unit is in ft\n";
        pos2 = line.find(" ft");
        unit_temp = String("ft");
    }

    value = stod(line.substr(pos1 + 3, pos2 - (pos1 + 3)).c_str());
    if (unit_temp == String("m")) {
        value /= (1e3 * nm);
    }
    if (unit_temp == String("ft")) {
        value /= nm_ft;
    }

    cout << prefix.value << YELLOW << "... done.\n" << RESET;

    print(name, String("nm"), prefix, cout);

}

//reads from file the content after 'name = ' and writes it into this.
void Length::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Length>(this, name, filename, mode, prefix);

}


Angle Angle::operator + (const Angle& angle) {

    Angle temp;

    (temp.value) = value + (angle.value);
    temp.normalize();

    return temp;

}

Angle& Angle::operator += (const Angle& angle) {


    value += (angle.value);
    normalize();

    return (*this);

}

Angle& Angle::operator += (const double& x) {


    value += x;
    normalize();

    return (*this);

}

Angle& Angle::operator -= (const Angle& angle) {


    value -= (angle.value);
    normalize();

    return (*this);

}

Angle& Angle::operator -= (const double& x) {


    value -= x;
    normalize();

    return (*this);

}



Angle Angle::operator- (const Angle& angle) {
    Angle temp;

    (temp.value) = value - (angle.value);
    temp.normalize();

    return temp;
}

Angle Angle::operator* (const double& x) {

    Angle temp;

    temp.value = value * x;
    temp.normalize();

    return temp;

}

Angle Angle::operator/ (const double& x) {

    Angle temp;

    temp.value = value / x;
    temp.normalize();

    return temp;

}



template<class S> void Limb::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;

    if (search_entire_stream) {

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline((*input_stream), line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));

    }
    else {

        line.clear();
        getline((*input_stream), line);

    }
    pos = line.find(" = ");

    value = line[pos + 3];

    print(name, prefix, cout);


}


//read a Body from file, and it returns true if it has not reached the end of file, false otherwise
template<class S> bool Body::read_from_stream(String name, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    size_t pos;

    cout << prefix.value << name.value << ":\n";

    //read first line with no information
    getline((*input_stream), line);

    if (!(*input_stream).eof()) {
        //*input_stream has not reached the end of file


        //read type
        line.clear();
        getline(*input_stream, line);
        pos = line.find(" = ");
        type = line.substr(pos + 3, line.size() - (pos + 3));
        cout << new_prefix.value << "Type = " << type.value << "\n";


        //read name
        line.clear();
        getline(*input_stream, line);
        pos = line.find(" = ");
        ((*this).name) = line.substr(pos + 3, line.size() - (pos + 3));
        cout << new_prefix.value << "Name = " << ((*this).name).value << "\n";


        if (type == String("star")) {
            RA.read_from_stream<S>(String("right ascension"), input_stream, false, new_prefix);
            d.read_from_stream<S>(String("declination"), input_stream, false, new_prefix);
        }
        else {
            radius.read_from_stream<S>(String("radius"), input_stream, false, new_prefix);
        }

        return true;

    }
    else {
        //*input_stream has reached the end of stream

        return false;

    }

}



Catalog::Catalog(String filename, [[maybe_unused]] String prefix) {

    read_from_file_to(String("catalog"), filename, String("R"), prefix);

    //	FileRW file;
    //	string line;
        //    streampos old_position;


    //	file.set_name(filename);
    //	if (file.open(String("in"), String(""))) {



    //		file.close(String(""));

    //	}

}

//reads from file the Catalog and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Catalog::read_from_file_to([[maybe_unused]] String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Catalog>(this, name, filename, mode, prefix);

}

//read from stream input_stream the content and writes it into this
template<class S> void Catalog::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    Body body;

    //check whether the next line in the file has reached the end of file
    while ((body.read_from_stream<S>(String("read body"), input_stream, false, prefix)) == true) {

        //if the next line in the file has not reached the end of file, I set *(file.value) to its old position and keep reading the file
        list.push_back(body);

    }

}


//return a vector containing the names of the Bodies in *this
vector<String> Catalog::get_names(void){
    
    unsigned int i;
    vector<String> output;
    
    for(i=0, output.clear(); i<list.size(); i++){
        output.push_back((list[i]).name);
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

    body.type = type;
    body.name = name;
    body.radius.value = radius;

    list.push_back(body);

    cout << "Added body to catalog:\n";
    body.print(String("body"), String("\t"), cout);

}




void Answer::enter(String name, [[maybe_unused]] String prefix) {

    bool check;
    string temp;

    do {

        temp.clear();

        cout << prefix.value << "Enter " << name.value << " [y/n]:";

        getline(cin >> ws, temp);

        if (((temp[0] == 'y') || (temp[0] == 'n')) && (temp.size() == 1)) {
            value = temp[0];
            check = true;
        }
        else {
            cout << prefix.value << RED << "Entered value is not valid!\n" << RESET;
            check = false;
        }
    } while (!check);

    print(name, prefix, cout);

}

void Answer::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}


void Sight::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

    unsigned int j;
    Time time_UTC;


    j = 0;
    //set number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //set body column
    listcontrol->SetItem(i, j++, wxString(body.name.value));

    //set limb column
    if ((body.name == String("sun")) || (body.name == String("moon"))) {

        if (wxString(limb.value) == 'u') { listcontrol->SetItem(i, j++, wxString((Limb_types[0].value))); }
        if (wxString(limb.value) == 'l') { listcontrol->SetItem(i, j++, wxString((Limb_types[2].value))); }
        if (wxString(limb.value) == 'c') { listcontrol->SetItem(i, j++, wxString((Limb_types[1].value))); }

    }
    else {

        listcontrol->SetItem(i, j++, wxString(""));

    }

    //set artificial horizon column
    listcontrol->SetItem(i, j++, wxString(artificial_horizon.value));

    //set sextant altitude column
    listcontrol->SetItem(i, j++, wxString((H_s).to_string(String(""), (display_precision.value), true)));

    //set index error
    listcontrol->SetItem(i, j++, wxString((index_error).to_string(String(""), (display_precision.value), true)));

    //set height of eye column
    if (artificial_horizon.value == 'n') {

        listcontrol->SetItem(i, j++, wxString(height_of_eye.to_string(String("m"), (display_precision.value))));

    }
    else {

        listcontrol->SetItem(i, j++, wxString(""));

    }

    //set column of master-clock date and hour of sight
    //I add to master_clock_date_and_hour the value stopwatch (if any): I write the result in time_UTC and I write in the GUI object  time_UTC
    time_UTC = master_clock_date_and_hour;
    //    if((use_stopwatch.value)=='y'){time_UTC += stopwatch;}
    listcontrol->SetItem(i, j++, wxString(time_UTC.to_string(display_precision.value, false)));

    //set use of stopwatch
    listcontrol->SetItem(i, j++, wxString((use_stopwatch.value)));

    //set stopwatch reading
    if ((use_stopwatch.value) == 'y') {
        listcontrol->SetItem(i, j++, wxString((stopwatch).to_string(display_precision.value, false)));
    }
    else {
        listcontrol->SetItem(i, j++, wxString(""));
    }

    //set TAI-UTC
    listcontrol->SetItem(i, j++, wxString((TAI_minus_UTC).to_string((display_precision.value), false)));

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol

    listcontrol->SetItem(i, j++, wxString(label.value));

    //    }else{
    //        //the label in *this is empty -> I write as label in listcontrol the current date and time
    //
    //        Time now;
    //
    //        now.set_current(String(""));
    //
    //        label.set(String("Label replacing empty label"), now.to_string(display_precision.value), String(""));
    //        listcontrol->SetItem(i, j++, wxString(now.to_string(display_precision.value)));
    //
    //    }

    if ((related_route.value) == -1) {
        //if the sight is not connected to a route, I leave the column field empty

        listcontrol->SetItem(i, j++, wxString(""));

    }
    else {
        //if the sight is connected to a route, I write the # of the related route in the column field

        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_route.value) + 1));

    }


}


void Sight::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    long i;
    wxListItem item;

    if (position_in_listcontrol == -1) {

        i = (listcontrol->GetItemCount());

    }
    else {

        i = position_in_listcontrol;
        listcontrol->DeleteItem(i);

    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_wxListCtrl(i, listcontrol);

}


//this function returns true if the reading operation has been performed without errors, false otherwise
template<class S> bool Sight::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    bool check = true;
    String new_prefix;
    //this unsigned int counts how many additional entries have been inserted into the vector item
    unsigned int additional_items;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    additional_items = 0;

    body.read_from_stream<S>(String("body"), input_stream, false, new_prefix);
    if (body.type.value != "star") {
        items.insert(items.begin() + 1 + (additional_items++), all_items[1]);
        limb.read_from_stream<S>(String("limb"), input_stream, false, new_prefix);
    }
    H_s.read_from_stream<S>(String("sextant altitude"), input_stream, false, new_prefix);
    index_error.read_from_stream<S>(String("index error"), input_stream, false, new_prefix);
    artificial_horizon.read_from_stream<S>(String("artificial horizon"), input_stream, false, new_prefix);
    if (artificial_horizon == Answer('n', new_prefix)) {
        items.insert(items.begin() + 3 + (additional_items++), String("height of eye"));
        height_of_eye.read_from_stream<S>(String("height of eye"), input_stream, false, new_prefix);
    }

    check &= master_clock_date_and_hour.read_from_stream<S>(String("master-clock date and hour of sight"), input_stream, false, new_prefix);
    if (!check) {
        cout << prefix.value << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
    }
    time = master_clock_date_and_hour;

    use_stopwatch.read_from_stream<S>(String("use of stopwatch"), input_stream, false, new_prefix);

    if (use_stopwatch == Answer('y', new_prefix)) {

        items.insert(items.begin() + 5 + (additional_items++), String("stopwatch reading"));
        stopwatch.read_from_stream<S>(String("stopwatch"), input_stream, false, new_prefix);
        time += stopwatch;

    }

    TAI_minus_UTC.read_from_stream<S>(String("TAI - UTC at time of master-clock synchronization with UTC"), input_stream, false, new_prefix);
    time += TAI_minus_UTC;
    time.print(String("TAI date and hour of sight"), new_prefix, cout);

    //check whether the date and hour of sight falls within the time window covered by JPL data files
    check &= check_time_interval(prefix);

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }

    //given that the sight is not yet related to a route, I set
    (related_route.value) = -1;

    if (!check) {
        cout << prefix.value << RED << "Error reading sight!\n" << RESET;
    }

    return check;

}

//compute this->time and returns true if time lies within the data file of NASA JPL ephemerides files, and false otherwise. This function requires that body.name is specified: if body.name is "", it returns false.
bool Sight::check_time_interval(String prefix) {

    String new_prefix;
    stringstream temp;
    FileR data_file;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //data_file is the file where that data relative to body are stored: I count the number of lines in this file and store them in data_file.number_of_lines
    temp.clear();
    if ((body.type) != String("star")) {
        temp << (wxGetApp().data_directory).value << (body.name.value) << ".txt";
    }
    else {
        temp << (wxGetApp().data_directory).value << "j2000_to_itrf93.txt";
    }
    data_file.set_name(temp.str());


    if (data_file.check_if_exists(new_prefix)) {
        //the ephemerides file data_file exists -> check the time interval

        int l_min, l_max;
        bool check;

        //compute this->time

        //I first set time to master_clock_date_and_hour ...
        time = master_clock_date_and_hour;
        //.. then I add to it sight->stopwatch, if any ....
        if (use_stopwatch == Answer('y', String(""))) {
            time += stopwatch;
        }
        //... then I add to it TAI_minus_UTC, to convert it from the UTC to the TAI scale
        time += TAI_minus_UTC;

        data_file.count_lines(new_prefix);

        //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
        l_min = (int)(L * ((time.MJD) - MJD_min)) - (int)(N / 2.0);
        //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
        l_max = (int)(L * ((time.MJD) - MJD_min)) + (int)(N / 2.0);

        //check whether the lines from l_min to l_max, which are used for the data interpolation, are present in the file where data relative to the body are stored
        if ((l_min >= 0) && (l_max < (int)(data_file.number_of_lines))) {
            check = true;
        }
        else {
            check = false;
            cout << new_prefix.value << RED << "Time lies outside interval of NASA's JPL data files!\n" << RESET;
        }

        return check;

    }
    else {
        //the ephemerides file data_file does not exist

        cout << new_prefix.value << RED << "Cannot check time interval because ephemerides' file does not exist!\n" << RESET;

        return false;

    }

}

void Sight::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    body.print(String("body"), new_prefix, ostr);
    if (body.type.value != "star") {
        limb.print(String("limb"), new_prefix, ostr);
    }
    H_s.print(String("sextant altitude"), new_prefix, ostr);
    index_error.print(String("index error"), new_prefix, ostr);
    artificial_horizon.print(String("artificial horizon"), new_prefix, ostr);
    if (artificial_horizon == Answer('n', new_prefix)) {
        height_of_eye.print(String("height of eye"), String("m"), new_prefix, ostr);
    }
    master_clock_date_and_hour.print(String("master-clock date and hour of sight"), new_prefix, ostr);
    use_stopwatch.print(String("use of stopwatch"), new_prefix, ostr);
    if (use_stopwatch == Answer('y', new_prefix)) {
        stopwatch.print(String("stopwatch"), new_prefix, ostr);
    }
    TAI_minus_UTC.print(String("TAI - UTC at time of master-clock synchronization with UTC"), new_prefix, ostr);

    label.print(String("label"), true, new_prefix, ostr);

    if (((related_route.value) != -1) && (&ostr == &cout)) {
        ostr << new_prefix.value << "# of related route = " << (related_route.value) + 1 << "\n";
    }

    /*
     cout << RED << "items:\n";
     for(unsigned int i=0; i<items.size(); i++){
     cout << new_prefix.value << items[i].value << "\n";

     }
     cout << RESET << "\n";
     */

}

//default constructor
PositionProjection::PositionProjection(void) {

}

//custom constructor, which initialized the point with coordinates x, y
PositionProjection::PositionProjection(const double x_in, const double y_in) {

    x = x_in;
    y = y_in;

}

inline PositionProjection PositionProjection::operator + (const PositionProjection& q) {

    PositionProjection p;

    (p.x) = x + (q.x);
    (p.y) = y + (q.y);

    return p;

}

inline PositionProjection PositionProjection::operator - (const PositionProjection& q) {

    PositionProjection p;

    (p.x) = x - (q.x);
    (p.y) = y - (q.y);

    return p;

}

//divide both members of *this by the number x and return the resulting  PositionProjection
inline PositionProjection PositionProjection::operator / (const double& q) {

    PositionProjection p;

    (p.x) = x/q;
    (p.y) = y/q;

    return p;

}


inline PositionProjection PositionProjection::operator / (const Double& q){
    
    return ((*this)/(q.value));
    
}

//multiply both members of *this by the number x and return the resulting  PositionProjection
inline PositionProjection PositionProjection::operator * (const double& q) {

    PositionProjection p;

    (p.x) = x*q;
    (p.y) = y*q;

    return p;

}


inline void PositionProjection::operator += (const PositionProjection& q) {

    (*this) = (*this) + q;
    
}


inline void PositionProjection::operator -= (const PositionProjection& q) {

    (*this) = (*this) - q;
    
}


inline void PositionProjection::operator *= (const double& a) {

    (*this) = (*this) * a;
    
}


inline void PositionProjection::operator /= (const double& a) {

    (*this) = (*this) / a;
    
}


//return true if *this is falls wihtin the plot area of *draw_panel in the Mercator projection, and false otherwise
inline bool PositionProjection::CheckMercator(DrawPanel* draw_panel){
    
    bool output;
    

    if ((draw_panel->x_min) <= (draw_panel->x_max)) {
        //this is the 'normal' configuration where the boundaries of the chart do not encompass the meridian lambda = pi

        output = (((draw_panel->x_min) <= x) && (x <= (draw_panel->x_max)));

    }else {
        //this is the 'non-normal' configuration where the boundaries of the chart encompass the meridian lambda = pi

        output = ((((draw_panel->x_min) <= x) && (x <= (draw_panel->x_max) + 2.0*M_PI)) ||  (((draw_panel->x_min) - 2.0*M_PI <= x) && (x <= (draw_panel->x_max))));

    }

    output &= (((draw_panel->y_min) <= y) && (y <= (draw_panel->y_max)));
    
    return output;

}


//normalize p.lambda, then set x and y equal to the Mercator projections of the Position p
inline void PositionProjection::SetMercator(const Position& p){
    
    x = -(p.lambda.value);
    y = log(1.0 / cos((p.phi)) + tan((p.phi)));
    
}


//normalize p.lambda, then set x and y equal to the Mercator projections of the Position p
inline void PositionProjection::NormalizeAndSetMercator(const Position& p){
    
    Position temp;
    
    temp = p;
    temp.lambda.normalize_pm_pi();
    
    SetMercator(temp);
     
}


// this function plots the Routes of type (Route_types[2]) in route_list in kml forma. WARNING: THIS FUNCTION USES THE SYSTEM() COMMAND AND THUS IT IS NOT PORTABLE ACROSS PLATFORMS. Also, this functions used file_kml, which has been removed from the code, and it should be revised.
void Data::print_to_kml(String prefix) {

    stringstream line_ins, /*plot_title contains the  title of the Route to be plotted*/ plot_title;
    string line;
    unsigned int i, j;
    double lambda_kml, phi_kml;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //	file_init.open(prefix);
    //	file_kml.remove(prefix);

        //replace line with number of points for routes in plot_dummy.plt
    plot_command.str("");
    command.str("");
    wxGetApp().n_points_routes.read_from_file_to(String("number of points for routes"), (wxGetApp().path_file_init), String("R"), new_prefix);



    //plot routes

    plot_command.str("");
    command.str("");
    for (i = 0; i < (route_list.size()); i++) {

        if (route_list[i].type == (Route_types[2])) {

            //this is the opening of a path code in kml format
            plot_command << "\\\t<Style id=\\\"" << i << "\\\">\\\n\\\t<LineStyle>\\\n\\\t\\\t<color>" << /*I use the remainder of i in this way, so if i > size of kml_colors, I start back reading from the beginning of kml_colors*/ kml_colors[i % (sizeof(kml_colors) / sizeof(*kml_colors))] << "<\\/color>\\\n\\\t\\\t<width>2<\\/width>\\\n\\\t<\\/LineStyle>\\\n\\\t<\\/Style>\\\n\\\t<Placemark>\\\n\\\t\\\t<name>"
                << (route_list[i]).label.value
                << "<\\/name>\\\n\\\t\\\t<styleUrl>#" << i << "<\\/styleUrl>\\\n\\\t\\\t<description>"
                << (route_list[i]).label.value
                << "<\\/description>\\\n\\\t\\\t<LineString>\\\n\\\t\\\t\\\t<extrude>1<\\/extrude>\\\n\\\t\\\t\\\t<tessellate>0<\\/tessellate>\\\n\\\t\\\t\\\t<altitudeMode>absolute<\\/altitudeMode>\\\n\\\t\\\t\\\t<coordinates>\\\n";



            for (j = 0; j < (unsigned int)(wxGetApp().n_points_routes.value); j++) {

                //I consider a Length equal to a temporary value of the length of the route, which spans between 0 and 2.0*M_PI*(Re*sin(((route_list[i]).omega.value))) across the for loop over j
                //I compute the coordinate of the endpoint of route_list[i] for the ((route_list[i]).l) above
                (route_list[i]).compute_end(Length(2.0 * M_PI * (Re * sin(((route_list[i]).omega.value))) * ((double)j) / ((double)(wxGetApp().n_points_routes.value - 1))), new_prefix);

                //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude, and in the third altitude (I am not interested in altitude, thus is set it to 0); The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.

                lambda_kml = -K * ((route_list[i]).end.lambda.value);
                if (lambda_kml < -180.0) {
                    lambda_kml += 360.0;
                }

                phi_kml = K * ((route_list[i]).end.phi.value);
                if (phi_kml > 270.0) {
                    phi_kml -= 360.0;
                }

                plot_command << "\\\t\\\t\\\t\\\t" << lambda_kml << "," << phi_kml << ",0.0\\\n";

            }

            //this is the closing of a path code in kml format
            plot_command << "\\\t\\\t\\\t<\\/coordinates>\\\n\\\t\\\t<\\/LineString>\\\n\\\t<\\/Placemark>\\\n";

        }

    }

    //add the line to plot_kml.kml which contains the parametric plot of the routes
    command << "LANG=C sed 's/\\/\\/route\\_plots/" << plot_command.str().c_str() << "/g' kml_dummy.kml >> kml_temp.kml \n";

    //execute the command string
    system(command.str().c_str());




    //plot positions

    plot_command.str("");
    command.str("");
    for (i = 0; i < (position_list.size()); i++) {

        //this is the opening of a path code in kml format
        plot_command << "\\\n\\\t<Placemark>\\\n\\\t\\\t<Style>\\\n\\\t\\\t\\\t<IconStyle>\\\n\\\t\\\t\\\t\\\t<color>7733ff66<\\/color>\\\n\\\t\\\t\\\t\\\t<IconStyleSimpleExtensionGroup radius=\\\"3\\\" points=\\\"Infinity\\\" strokeColor=\\\"" << /*I use the remainder of i in this way, so if i > size of hex_colors, I start back reading from the beginning of hex_colors*/ hex_colors[i % (sizeof(hex_colors) / sizeof(*hex_colors))] << "\\\" strokeWidth=\\\"2\\\" lineDash=\\\"undefined\\\"\\/>\\\n\\\t\\\t\\\t<\\/IconStyle>\\\n\\\t\\\t\\\t<PolyStyle xmlns=\\\"\\\">\\\n\\\t\\\t\\\t\\\t<outline>0<\\/outline>\\\n\\\t\\\t\\\t<\\/PolyStyle>\\\n\\\t\\\t<\\/Style>\\\n\\\t\\\t<description>"
            << (position_list[i]).label.value << "<\\/description>\\\n\\\t\\\t<ExtendedData>\\\n\\\t\\\t\\\t<Data name=\\\"scStyle\\\"\\/>\\\n\\\t\\\t<\\/ExtendedData>\\\n\\\t\\\t<Point>\\\n\\\t\\\t\\\t<coordinates>";


        //I write the coordinates (longitude = lambda_kml, latitude = phi_kml) in plot_command, and thus in the kml file, in degrees with decimal points. In the first column there is longitude, in the second  latitude. The - sign in lambda_kml is added because kml adopt the convention that longitude is positive towards the east, while in this library it is positive towards the west. 360 is substracted to lambda_kml and phi_kml in such a way that -180 < lambda_kml < 180 and -90 < phi < 90.

        lambda_kml = -K * ((position_list[i]).lambda.value);
        if (lambda_kml < -180.0) {
            lambda_kml += 360.0;
        }

        phi_kml = K * ((position_list[i]).phi.value);
        if (phi_kml > 270.0) {
            phi_kml -= 360.0;
        }

        plot_command << lambda_kml << "," << phi_kml << "<\\/coordinates>\\\n\\\t\\\t<\\/Point>\\\n\\\t<\\/Placemark>";

    }

    //add the line to plot_kml.kml which contains the parametric plot of the positions
    command << "LANG=C sed 's/\\/\\/position\\_plots/" << plot_command.str().c_str() << "/g' kml_temp.kml >> '" /*<< file_kml.name.value*/ << "'\nrm -rf kml_temp.kml\n";

    //execute the command string
    system(command.str().c_str());



    //	file_init.close(prefix);

}


Data::Data(Catalog* cata, [[maybe_unused]] String prefix) {

    String new_prefix;

    new_prefix = prefix.append(String("\t"));


    catalog = cata;
    job_id = -1;

    plot_command.precision((data_precision.value));
    command.precision((data_precision.value));

    //	file_init.set_name((wxGetApp().path_file_init));

    choices.push_back(String("Add a sight"));
    choices.push_back(String("Modify a sight"));
    choices.push_back(String("Delete a sight"));
    choices.push_back(String("Add a position"));
    choices.push_back(String("Modify a position"));
    choices.push_back(String("Transport a position"));
    choices.push_back(String("Delete a position"));
    choices.push_back(String("Add a route"));
    choices.push_back(String("Transport a route"));
    choices.push_back(String("Compute route crossings"));
    choices.push_back(String("Delete a route"));
    choices.push_back(String("Replot"));
    choices.push_back(String("Full zoom out"));
    choices.push_back(String("Clear"));
    choices.push_back(String("Line of position on paper chart"));
    choices.push_back(String("Save to file"));
    choices.push_back(String("Read from file"));
    choices.push_back(String("Exit"));


    //read paramters from init file

//	file_init.open(String("in"), prefix);

    //read number of intervals for ticks from file_init
    wxGetApp().n_intervals_ticks_preferred.read_from_file_to(String("preferred number of intervals for ticks"), wxGetApp().path_file_init, String("R"), new_prefix);

    //read number of points for routes from file_init
    wxGetApp().n_points_routes.read_from_file_to(String("number of points for routes"), wxGetApp().path_file_init, String("R"), new_prefix);

    //read n_points_plot_coastline_* from file_init
    wxGetApp().n_points_plot_coastline_Mercator.read_from_file_to(String("number of points coastline Mercator"), wxGetApp().path_file_init, String("R"), new_prefix);
    wxGetApp().n_points_plot_coastline_3D.read_from_file_to(String("number of points coastline 3D"), wxGetApp().path_file_init, String("R"), new_prefix);

}


/*compute the astronomical position by wriitng it into center, and the circle of uncertainty by writing it into error_cirocle. Then I push back center and error_circle to position_list and route_list, respectively.
 Return value:
    * If all crossings have been used to compute the astronomical position
        - return 0 if [ # crossings used ] >= 2
        - return 2 if [ # crossings used ] = 1
    * If only part of the crossings have been used to compute the astronomical position
        - return 1 if [ # crossings used ] >= 2
        - return 2 if [ # crossings used ] = 1
    * If no crossings could be used to compute the astronomical position , return -1
 */
 
int Data::compute_position(String prefix) {

    unsigned int i, j, l;
    String new_prefix;
    stringstream dummy;
    Length r, s;
    Position center;
    double x;
    Route error_circle;
    int output;


    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if (crossing_route_list.size() > 1) {
        //there are enough Routes in crossing_route_list -> I compute the crossing

        vector< vector<Position> > p;
        vector<Position> q, q_temp(2);

        cout << prefix.value << "Computing crossings between routes :\n";
        for (i = 0; i < crossing_route_list.size(); i++) {
            cout << new_prefix.value << "\t" << (crossing_route_list[i]).label.value << "\n";
        }

        //I run over all the pairs of circles of equal altitude and write their crossing points into p
        l = 0;
        for (i = 0; i < crossing_route_list.size(); i++) {
            for (j = i + 1; j < crossing_route_list.size(); j++) {

                //                cout << prefix.value << "Computing crossing between routes " << crossing_route_list[i]+1 << " and " << crossing_route_list[j]+1 << "\n";

                if (((crossing_route_list[i]).crossing(crossing_route_list[j], &q_temp, &x, new_prefix)) >= 0) {
                    //in this case, the two routes under consideration intercept with no error message

                    //if the two routes under consideration are not too parallel (i.e., |cos(their crossing angle)| < cos((wxGetApp().min_crossing_angle)), then I add this crossing to the list of sensible crossings
                    if (fabs(x) < cos((wxGetApp().min_crossing_angle))) {

                        p.resize(l + 1);
                        (p[l]).resize(2);

                        p[l] = q_temp;

                        q.push_back(q_temp[0]);
                        q.push_back(q_temp[1]);

                        l++;

                        cout << new_prefix.value << "Crossing is valid.\n";

                    }
                    else {

                        cout << new_prefix.value << "Crossing is not valid.\n";

                    }

                }

            }
        }


        if (l > 0) {
            //there is at least one valid crossing -> the astronomical Position can be computed


            //r is the minimal distance between crossing points. To find the minimum, here I set r to it largest possible value, obtained when the two points are at the antipodes. I find the pair of crossing points which is closest to each other, and set Position center to one of the Positions in this pair. center will thus represent the approximate astronomical position. I will then run over all the pairs of crossing points in p, p[i], and pick either p[i][0] or p[i][1]: I will pick the one which is closest to center

            cout << prefix.value << "Distances between pairs of crossing positions:\n";
            r.set(String(""), M_PI * Re, prefix);

            for (i = 0; i < q.size(); i++) {
                for (j = i + 1; j < q.size(); j++) {

                    dummy.str("");
                    dummy << "distance between points " << i << " and " << j;

                    (q[i]).distance((q[j]), &s, String(dummy.str()), new_prefix);

                    if (r > s) {
                        r = s;
                        center = (q[i]);
                    }

                }
            }

            r.print(String("minimal distance between crossing points"), String("nm"), prefix, cout);
            center.print(String("center crossing"), prefix, cout);

            //I append center to the list of retained crossings, run through all the pairs of crossings except for center, and select the Position in the pair which is closer to center
            q.clear();
            q.push_back(center);

            for (i = 0; i < p.size(); i++) {

                if (!(((p[i][0]) == center) || ((p[i][1]) == center))) {

                    center.distance(p[i][0], &r, String(""), new_prefix);
                    center.distance(p[i][1], &s, String(""), new_prefix);

                    if (r > s) {

                        q.push_back(p[i][1]);

                    }
                    else {

                        q.push_back(p[i][0]);

                    }

                }

            }
            

            //compute astronomical position by averaging on all viable crossing points
            center.lambda.value = 0.0;
            center.phi.value = 0.0;
            for (i = 0; i < q.size(); i++) {

                (center.lambda.value) += ((q[i]).lambda.value);
                (center.phi.value) += ((q[i]).phi.value);

            }
            (center.lambda.value) /= ((double)(q.size()));
            (center.phi.value) /= ((double)(q.size()));
            center.label.set(String(""), String("astronomical position"), prefix);
            center.print(String("astronomical position"), prefix, cout);

            position_list.push_back(center);

            
            if(q.size() > 1){
                //there are >= 2 crossings -> the error on the astronomical position can be computed
                
                //compute error on astronomical position
                (r.value) = 0.0;
                for (i = 0; i < q.size(); i++) {
                    for (j = i + 1; j < q.size(); j++) {
                        
                        (q[i]).distance(q[j], &s, String(""), new_prefix);
                        r = r + s;
                        
                    }
                }
                (r.value) /= ((double)((q.size()) * ((q.size()) - 1) / 2));
                
                //computes the circle of equal altitude which represents the error of the sight
                (error_circle.type) = RouteType(((Route_types[2]).value));
                (error_circle.reference_position) = center;
                (error_circle.omega.value) = (r.value) / Re;
                (error_circle.label) = String("error on astronomical position");
                ((error_circle.related_sight).value) = -1;
                
                r.print(String("error on astronomical position"), String("nm"), prefix, cout);
                route_list.push_back(error_circle);
                
                
                if (l == (crossing_route_list.size()) * ((crossing_route_list.size()) - 1) / 2) {
                    //all Routes in crossing_route_list have been used to get the position
                    
                    if(l > 1){
                        // all crossings have been used and there are >= 2 crossings used to compute the astronomical Position -> the error on the astronomical Position could be computed
                        
                        output = 0;
                        
                    }else{
                        // all crossings have been used and there is 1 crossing used to compute the astronomical Position  -> the error on the astronomical Position could not be computed

                        output = 2;
                        
                    }

                }else {
                    //only some Routes in crossing_route_list have been used to get the position

                    if(l > 1){
                        // only some crossings have been used  and there are >= 2 crossings used to compute the astronomical Position -> the error on the astronomical Position could be computed
                        
                        output = 1;
                        
                    }else{
                        // only some crossings have been used  there is 1 crossing used to compute the astronomical Position -> the error on the astronomical Position could not be computed

                        output = 2;
                        
                    }
                    
                }

            }else{
                
                cout << prefix.value << RED << "I could not compute the error on the astronomical position because there are not enough valid crossings!\n" << RESET;
                
                output = 2;
                
            }

        }else {

            cout << prefix.value << RED << "I could not compute the position because there are no valid crossings!\n" << RESET;
            output = -1;

        }

        p.clear();
        q.clear();
        q_temp.clear();


    }else {
        //there are not enough Routes in crossing_route_list -> I cannot compute the crossing

        cout << prefix.value << RED << "I could not compute the position because there are no valid Routes!\n" << RESET;
        
        output = -1;
        
    }
    
    return output;

}

//print all the data in data to ostr
void Data::print(bool print_all_routes, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Data:\n";

    print_sights(new_prefix, ostr);
    print_routes(print_all_routes, new_prefix, ostr);
    print_positions(new_prefix, ostr);

    print_recent_items(recent_bodies, String("bodies"), prefix, ostr);
    print_recent_items(recent_projections, String("projections"), prefix, ostr);
    print_recent_items(recent_length_formats, String("length formats"), prefix, ostr);
    print_recent_items(recent_route_types, String("route types"), prefix, ostr);
    print_recent_items(recent_route_for_transport_types, String("route for transport types"), prefix, ostr);
    print_recent_items(recent_length_units, String("length units"), prefix, ostr);
    print_recent_items(recent_speed_units, String("speed units"), prefix, ostr);

}

void Data::print_sights(String prefix, ostream& ostr) {

    stringstream name;
    unsigned int i;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Sights:\n";
    for (i = 0; i < sight_list.size(); i++) {
        name.str("");
        name << "Sight #" << i + 1;
        (sight_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
    }


}

void Data::print_positions(String prefix, ostream& ostr) {

    stringstream name;
    unsigned int i;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Positions:\n";
    for (i = 0; i < position_list.size(); i++) {
        name.str("");
        name << "Position #" << i + 1;
        (position_list[i]).print(String(name.str().c_str()), new_prefix, ostr);
    }


}

void Data::print_routes(bool print_all_routes, String prefix, ostream& ostr) {

    stringstream name;
    unsigned int i, j;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << "Routes:\n";

    for (i = 0, j = 0; i < (route_list.size()); i++) {

        //if print_all_routes = false, I only print routes which are not linked to a sight. This is to avoid doubles: If I print also Routes which are related to a Sight, then when the file to which I am saving will be read again, the code will reduce them and create double Routes identical to the ones already present in the file.
        if (((((route_list[i]).related_sight).value) == -1) || print_all_routes) {

            name.str("");
            name << "Route #" << j + 1;

            (route_list[i]).print(String(name.str().c_str()), new_prefix, ostr);

            j++;

        }

    }


}


bool Data::add_sight_and_reduce(Sight* sight_in, [[maybe_unused]] String prefix) {


    bool check = true;

    //I link the sight to the route, and the route to the sight
    //create a new route in the respective list
    route_list.resize(route_list.size() + 1);
    (((*sight_in).related_route).value) = ((int)(route_list.size())) - 1;
    //push back sight_in into sight_list
    sight_list.push_back(*sight_in);
    (((route_list[route_list.size() - 1]).related_sight).value) = ((int)(sight_list.size())) - 1;

    //I commented this out because now the sight is enetered through the GUI
    //    (sight_list[sight_list.size()-1]).enter((*catalog), String("new sight"), prefix);
    check &= ((sight_list[sight_list.size() - 1]).reduce(&(route_list[route_list.size() - 1]), prefix));

    //I link the sight to the route, and the route to the sight
    ((sight_list[sight_list.size() - 1]).related_route.value) = ((int)route_list.size()) - 1;
    ((*sight_in).related_route.value) = ((int)route_list.size()) - 1;
    (((route_list[route_list.size() - 1]).related_sight).value) = ((int)sight_list.size()) - 1;


    if (check) {
        (sight_list[sight_list.size() - 1]).print(String("Sight"), prefix, cout);

        cout << prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";
        cout << prefix.value << "Route added as route #" << route_list.size() << ".\n";

    }

    return check;

}


//adds to Data-> this the Route written in *route_in
void Data::add_route(Route* route_in, [[maybe_unused]] String prefix) {


    //    route.enter(String("new route"), prefix);

    route_list.push_back(*route_in);
    cout << prefix.value << "Route added as position #" << route_list.size() << ".\n";


}



//removes sight #i from sight_list by updating all the connections to between sights and routes. If remove_related_route = 'y', it removes also the route related to sight i
void Data::remove_sight(unsigned int i, Answer remove_related_route, [[maybe_unused]] String prefix) {

    stringstream name;
    unsigned int j;
    Int i_related_route;

    i_related_route = ((sight_list[i]).related_route);

    name.str("");
    name << "Sight to be removed: Sight #" << i + 1;

    (sight_list[i]).print(String(name.str().c_str()), prefix, cout);

    sight_list.erase(sight_list.begin() + i);

    //update the linking indexed of routes in accordance with the deletion of the sight
    for (j = 0; j < route_list.size(); j++) {

        if (((((route_list[j]).related_sight).value) != -1) && ((((route_list[j]).related_sight).value) >= ((int)i))) {

            if ((((route_list[j]).related_sight).value) == ((int)i)) {
                (((route_list[j]).related_sight).value) = -1;
            }
            else {
                (((route_list[j]).related_sight).value)--;
            }

        }

    }

    cout << prefix.value << "Sight removed.\n";


    if ((i_related_route.value) != -1) {

        if (remove_related_route == Answer('y', prefix)) {
            //the related route must be removed -> I remove it

            remove_route((i_related_route.value), Answer('n', prefix), prefix);

        }
        else {
            //the related route must not be removed: given that its related sight has been deleted, I set its related_sight.value to -1

            (route_list[i_related_route.value]).related_sight.set(String(""), -1, String(""));

        }

    }

}

void Data::remove_position(unsigned int i, [[maybe_unused]] String prefix) {

    stringstream name;

    name.str("");
    name << "Position to be removed: #" << i + 1;

    (position_list[i]).print(String(name.str().c_str()), prefix, cout);

    position_list.erase(position_list.begin() + i);

    cout << prefix.value << "Position removed.\n";

}

//removes route #i from route_list by updating all the connections to between sights and routes. If remove_related_sight = 'y', it removes also the sight related to route i
void Data::remove_route(unsigned int i, Answer remove_related_sight, [[maybe_unused]] String prefix) {

    unsigned int j;
    Int i_related_sight;
    stringstream name;

    (i_related_sight.value) = (((route_list[i]).related_sight).value);

    name.str("");
    name << "Route to be removed: #" << i + 1;

    (route_list[i]).print(String(name.str().c_str()), prefix, cout);

    route_list.erase(route_list.begin() + i);

    //update the linking indexed of sights in accordance with the deletion of the route
    for (j = 0; j < sight_list.size(); j++) {

        if ((((sight_list[j]).related_route.value) != -1) && (((sight_list[j]).related_route.value) >= ((int)i))) {

            if (((sight_list[j]).related_route.value) == ((int)i)) {
                ((sight_list[j]).related_route.value) = -1;
            }
            else {
                ((sight_list[j]).related_route.value)--;
            }

        }

    }

    cout << prefix.value << "Route removed.\n";


    if (((i_related_sight.value) != -1) && (remove_related_sight == Answer('y', prefix))) {

        remove_sight(i_related_sight.value, Answer('n', prefix), prefix);

    }

}


//read from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Data::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Data>(this, name, filename, mode, prefix);

}

/*
bool Data::read_from_file_to(FileRW& file, [[maybe_unused]] String prefix) {

    stringstream line_ins;
    string line;
    size_t pos;
    bool check;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    check = true;


    if (!(file.open(String("in"), new_prefix))) {

        check &= false;

    }
    else {

        //1. Here I read sights

        //read dummy text line '    Sights:"
        getline(*(file.value), line);

        line.clear();
        //read dummyt text line
        getline(*(file.value), line);
        pos = line.find("Sight #");

        //if I have found 'Sight #' in the line above, then I proceed and read the relative sight
        while (pos != (string::npos)) {

            cout << new_prefix.value << "Found new sight!\n";

            //read the sight block
            Sight sight;
            Route route;

            //if I find a sight which returns an error message when read from file, to be conservative I do not add any of the following sights in the file to sight_list because they may contain other errors
            check &= (sight.read_from_stream<fstream>(String("sight"), file.value, false, new_prefix));
            if (check) {

                check &= (sight.reduce(&route, new_prefix));

                if (check) {
                    sight.print(String("New sight"), new_prefix, cout);

                    sight_list.push_back(sight);
                    cout << new_prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";

                    route_list.push_back(route);
                    cout << new_prefix.value << "Route added as route #" << route_list.size() << ".\n";

                    //I link the sight to the route, and the route to the sight
                    ((route_list[route_list.size() - 1].related_sight).value) = ((int)(sight_list.size())) - 1;
                    ((sight_list[sight_list.size() - 1].related_route).value) = ((int)(route_list.size())) - 1;

                }

            }

            line.clear();
            //read dummyt text line
            getline(*(file.value), line);
            pos = line.find("Sight #");

        }

        //2. Here I read routes

        line.clear();
        //read dummy text line
        getline(*(file.value), line);
        pos = line.find("Route #");

        //if I have found 'Route #' in the line above, then I proceed and read the relative position
        while (pos != (string::npos)) {

            cout << new_prefix.value << "Found new route!\n";

            //read the position block
            Route route;

            route.read_from_stream<fstream>(String("route"), file.value, false, new_prefix);

            route.print(String("New route"), new_prefix, cout);

            route_list.push_back(route);
            cout << new_prefix.value << "Route added as position #" << route_list.size() << ".\n";

            line.clear();
            //read dummyt text line
            getline(*(file.value), line);
            pos = line.find("Route #");

        }


        //3. Here I read positions

        line.clear();
        //read dummy text line
        getline(*(file.value), line);
        pos = line.find("Position #");

        //if I have found 'Position #' in the line above, then I proceed and read the relative position
        while (pos != (string::npos)) {

            cout << new_prefix.value << "Found new position!\n";

            //read the position block
            Position position;

            position.read_from_stream<fstream>(String("position"), (file.value), false, new_prefix);

            position.print(String("New position"), new_prefix, cout);

            position_list.push_back(position);
            cout << new_prefix.value << "Position added as position #" << position_list.size() << ".\n";

            line.clear();
            //read dummyt text line
            getline(*(file.value), line);
            pos = line.find("Position #");

        }


        file.close(new_prefix);

    }

    if (!check) {
        cout << prefix.value << RED << "There was an error while reading file!\n" << RESET;
    }

    return check;

}
 */

template<class S> void Data::read_from_stream(String name, S* input_stream, bool search_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    size_t pos;
    String new_prefix, temp;
    bool check;


    check = true;

    cout << prefix.value << "Reading " << name.value << " from stream " << input_stream << "... \n";


    if (search_entire_stream) {
        //rewind *input_stream, look for the beginning of the data block corresponding to name, and read its first line

        //rewind the file pointer
        input_stream->clear();                 // clear fail and eof bits
        input_stream->seekg(0, std::ios::beg); // back to the start!

        do {

            line.clear();
            getline(*input_stream, line);

        } while (((line.find(name.value)) == (string::npos)) /*I run through the entire file by ignoring comment lines which start with '#'*/ || (line[0] == '#'));


    }
    else {
        //read the first line of the data block corresponding to name

        line.clear();
        getline(*input_stream, line);

    }

    cout << prefix.value << "... done.\n";



    //1. Read Sights

    //read dummy text line '    Sights:"
    getline((*input_stream), line);

    line.clear();
    //read dummyt text line
    getline((*input_stream), line);
    pos = line.find("Sight #");

    //if I have found 'Sight #' in the line above, then I proceed and read the relative sight
    while (pos != (string::npos)) {

        cout << new_prefix.value << "Found new sight!\n";

        //read the sight block
        Sight sight;
        Route route;

        //if I find a sight which returns an error message when read from file, to be conservative I do not add any of the following sights in the file to sight_list because they may contain other errors
        check &= (sight.read_from_stream<S>(String("sight"), input_stream, false, new_prefix));
        if (check) {

            check &= (sight.reduce(&route, new_prefix));

            if (check) {
                sight.print(String("New sight"), new_prefix, cout);

                sight_list.push_back(sight);
                cout << new_prefix.value << "Sight added as sight #" << sight_list.size() << ".\n";

                route_list.push_back(route);
                cout << new_prefix.value << "Route added as route #" << route_list.size() << ".\n";

                //I link the sight to the route, and the route to the sight
                ((route_list[route_list.size() - 1].related_sight).value) = ((int)(sight_list.size())) - 1;
                ((sight_list[sight_list.size() - 1].related_route).value) = ((int)(route_list.size())) - 1;

            }

        }

        line.clear();
        //read dummyt text line
        getline((*input_stream), line);
        pos = line.find("Sight #");

    }

    //2. read Routes

    line.clear();
    //read dummy text line
    getline((*input_stream), line);
    pos = line.find("Route #");

    //if I have found 'Route #' in the line above, then I proceed and read the relative position
    while (pos != (string::npos)) {

        cout << new_prefix.value << "Found new route!\n";

        //read the position block
        Route route;

        route.read_from_stream<S>(String("route"), input_stream, false, new_prefix);

        route.print(String("New route"), new_prefix, cout);

        route_list.push_back(route);
        cout << new_prefix.value << "Route added as route #" << route_list.size() << ".\n";

        line.clear();
        //read dummyt text line
        getline((*input_stream), line);
        pos = line.find("Route #");

    }


    //3. read Positions

    line.clear();
    //read dummy text line
    getline((*input_stream), line);
    pos = line.find("Position #");

    //if I have found 'Position #' in the line above, then I proceed and read the relative position
    while (pos != (string::npos)) {

        cout << new_prefix.value << "Found new position!\n";

        //read the position block
        Position position;

        position.read_from_stream<S>(String("position"), input_stream, false, new_prefix);

        position.print(String("New position"), new_prefix, cout);

        position_list.push_back(position);
        cout << new_prefix.value << "Position added as position #" << position_list.size() << ".\n";

        line.clear();
        //read dummyt text line
        getline((*input_stream), line);
        pos = line.find("Position #");

    }

    //4. Read recent items
    //read recent bodies and projections and ...
    read_list_from_stream<S>(String("Recent bodies"), input_stream, true, &recent_bodies);
    read_list_from_stream<S>(String("Recent projections"), input_stream, true, &recent_projections);
    read_list_from_stream<S>(String("Recent length formats"), input_stream, true, &recent_length_formats);
    read_list_from_stream<S>(String("Recent route types"), input_stream, true, &recent_route_types);
    read_list_from_stream<S>(String("Recent route for transport types"), input_stream, true, &recent_route_for_transport_types);
    read_list_from_stream<S>(String("Recent length units"), input_stream, true, &recent_length_units);
    read_list_from_stream<S>(String("Recent speed units"), input_stream, true, &recent_speed_units);

}


//print recent_items to ostr. Here name is the "[plural name of the item]"  : for example, if I am printing a list of Bodies, name.value = "bodies". This method is used for GUI fields of the format MultipleItemField
void Data::print_recent_items(const vector<int>& recent_items, String name, String prefix, ostream& ostr) {

    unsigned int i;
    stringstream temp;


    for (temp.str(""), i = 0; i < (recent_items.size()) - 1; i++) {
        temp << recent_items[i] << " ";
    }
    temp << recent_items.back();
    String(temp.str().c_str()).print(String("Recent ").append(name), false, prefix, ostr);

}


//insert the item 'item_id' into the vector of items *recent_items.  This method is used for GUI fields of the format MultipleItemField
void Data::insert_recent_item(unsigned int item_id, vector<int>* recent_items) {

    vector<int>::iterator position;

    position = find(recent_items->begin(), recent_items->end(), item_id);

    if (position == recent_items->end()) {
        //in this case, the selected item is not in the recent list: I write it in the recent list and in file_recent

        (*recent_items)[recent_items->size() - 1] = item_id;
        rotate(recent_items->begin(), recent_items->end() - 1, recent_items->end());

    }
    else {
        //the selected item is  in the recent list: I move the element in position to the first place in recent_items

        iter_swap(recent_items->begin(), position);

    }

}



bool Sight::reduce(Route* circle_of_equal_altitude, [[maybe_unused]] String prefix) {

    bool check = true;
    String new_prefix;
    stringstream temp;

    temp.clear();

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Reducing sight ...\n";

    (circle_of_equal_altitude->type.value) = ((Route_types[2]).value);

    compute_H_a(new_prefix);
    check &= get_coordinates(circle_of_equal_altitude, new_prefix);

    //link the circle of equal altitude (*circle_of_equal_altitude) to sight (*this)
    temp << body.name.value << " " << time.to_string(display_precision.value, false) << " TAI, " << label.value;
    (circle_of_equal_altitude->label).set(String(""), String(temp.str()), new_prefix);

    check &= compute_H_o(new_prefix);
    circle_of_equal_altitude->omega.set(String(""), M_PI_2 - (H_o.value), String(""));
    circle_of_equal_altitude->length.set(String(""), 2.0 * M_PI * Re * sin(circle_of_equal_altitude->omega), new_prefix);

    if (!check) {

        cout << prefix.value << RED << "Sight cannot be reduced!\n" << RESET;

    }
    else {

        cout << prefix.value << "... done\n";

    }

    return check;

}


void Sight::compute_H_a(String prefix) {

    if (artificial_horizon == Answer('y', prefix)) {
        H_a = (H_s - index_error) / 2.0;
        H_a.print(String("apparent altitude"), prefix, cout);

    }
    else {
        compute_DH_dip(prefix);
        H_a = H_s - index_error + DH_dip;
        H_a.print(String("apparent altitude"), prefix, cout);
    }

}


bool Sight::compute_H_o(String prefix) {

    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Computing observed altitude ...\n";

    check &= compute_DH_refraction(new_prefix);

    if (check) {
        compute_DH_parallax_and_limb(new_prefix);
        H_o = H_a + DH_refraction + DH_parallax_and_limb;
        cout << prefix.value << "...done\n";
        H_o.print(String("observed altitude"), prefix, cout);
    }
    else {
        cout << prefix.value << RED << "H_o cannot be computed!\n" << RESET;
    }

    return check;

}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    H_i = H_a + DH_refraction;
    H_i.print(String("intermediate altitude"), prefix, cout);

    if (body.type.value != "star") {

        switch ((limb.value)) {

        case 'u':
        {
            int status = 0;
            int iter = 0;
            double x = 0.0, x_lo = 0.0, x_hi = 2.0 * M_PI;
            gsl_function F;
            const gsl_root_fsolver_type* T;
            gsl_root_fsolver* s;

            F.function = &rhs_DH_parallax_and_limb;
            F.params = &(*this);

            T = gsl_root_fsolver_brent;
            s = gsl_root_fsolver_alloc(T);
            gsl_root_fsolver_set(s, &F, x_lo, x_hi);

            cout << new_prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";

            cout << new_prefix.value << "iter" << "[lower" << ", upper] " << "root " << "err(est)\n";

            iter = 0;
            do {

                iter++;
                status = gsl_root_fsolver_iterate(s);

                x = gsl_root_fsolver_root(s);
                x_lo = gsl_root_fsolver_x_lower(s);
                x_hi = gsl_root_fsolver_x_upper(s);
                status = gsl_root_test_interval(x_lo, x_hi, 0.0, epsrel);
                if (status == GSL_SUCCESS) {
                    cout << new_prefix.value << "Converged.\n";
                }
                cout << new_prefix.value << iter << " [" << x_lo << ", " << x_hi << "] " << x << " " << x_hi - x_lo << "\n";

            } while ((status == GSL_CONTINUE) && (iter < max_iter));


            //H_o.value = (x_lo+x_hi)/2.0;
            DH_parallax_and_limb.value = (x_lo + x_hi) / 2.0 - (H_i.value);
            gsl_root_fsolver_free(s);

            break;
        }
        case 'l':
        {
            //    H_o.value = (H_i.value) + asin(((atmosphere.earth_radius.value)*cos(H_i)+(body.radius.value))/(r.value));
            DH_parallax_and_limb.value = asin(((atmosphere.earth_radius.value) * cos(H_i) + (body.radius.value)) / (r.value));
            break;
        }
        case 'c':
        {
            //H_o.value = (H_i.value) + asin((atmosphere.earth_radius.value)*cos(H_i)/(r.value));
            DH_parallax_and_limb.value = asin((atmosphere.earth_radius.value) * cos(H_i) / (r.value));
            break;
        }
        }

        DH_parallax_and_limb.print(String("parallax and limb correction"), prefix, cout);

    }
    else {

        DH_parallax_and_limb.value = 0.0;

    }


}

double Atmosphere::T(Length z) {

    double x = 0.0;
    //cout << "z = " << (z.value) << "\n";

    if ((z.value) <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, check = true; (i < n_layers) && check; i++) {
            if (((z.value) >= h[i]) && ((z.value) < h[i + 1])) {
                x = t[i] + lambda[i] * ((z.value) - h[i]);
                check = false;
            }
        }
        //cout << "i = " << i << "\n";

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return x;


}

double Atmosphere::dTdz(Length z) {

    double x = 0.0;
    //cout << "z = " << (z.value) << "\n";

    if ((z.value) <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, check = true; (i < n_layers) && check; i++) {
            if (((z.value) >= h[i]) && ((z.value) < h[i + 1])) {
                x = lambda[i];
                check = false;
            }
        }

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return x;


}


double Atmosphere::n(Length z) {

    double x = 0.0;

    if ((z.value) <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, x = 0.0, check = true; (i < n_layers) && check; i++) {
            if (((z.value) >= h[i]) && ((z.value) < h[i + 1])) {
                if (lambda[i] != 0.0) {
                    x -= B / lambda[i] * log((t[i] + lambda[i] * ((z.value) - h[i])) / t[i]);
                }
                else {
                    x -= B * ((z.value) - h[i]) / t[i];
                }
                check = false;
            }
            else {
                if (lambda[i] != 0.0) {
                    x -= B / lambda[i] * log((t[i] + lambda[i] * (h[i + 1] - h[i])) / t[i]);
                }
                else {
                    x -= B * (h[i + 1] - h[i]) / t[i];
                }
            }
        }

        /*
         int_0^z dz/(t_n+lambda_n*(z-h_n)) = log()
         */

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return (A * P_dry_0 / T(z) * exp(x) / (1e6) + 1.0);


}

double Atmosphere::dndz(Length z) {

    return (-1.0 / T(z) * dTdz(z) * (n(z) - 1.0) - (n(z) - 1.0) * B / T(z));

}


double Sight::dH_refraction(double z, void* sight) {

    Sight* a = (Sight*)sight;
    Length z_Length, zero_Length;
    z_Length.value = z;
    zero_Length.value = 0.0;

    return(-(((*a).atmosphere).earth_radius.value) * (((*a).atmosphere).n(zero_Length)) * cos(((*a).H_a).value) * (((*a).atmosphere).dndz)(z_Length) / (((*a).atmosphere).n)(z_Length) / sqrt(gsl_pow_2(((((*a).atmosphere).earth_radius.value) + z) * (((*a).atmosphere).n)(z_Length)) - gsl_pow_2((((*a).atmosphere).earth_radius.value) * (((*a).atmosphere).n)(zero_Length) * cos(((*a).H_a).value))));


}


//this function returns the longitude lambda of a circle of equal altitude (*this) - pi
double Route::lambda_minus_pi(double t, void* route) {

    Route* r = (Route*)route;
    String new_prefix;

    //append \t to prefix
    new_prefix = ((*r).temp_prefix).append(String("\t"));

    (r->length.value) = Re * sin(((*r).omega.value)) * t;
    r->compute_end(new_prefix);

    return(((*r).end.lambda.value) - M_PI);

}


//comppute the extremal (min and max) longitudes taken by the points lying on *this, and write them in *lambda_min and *lambda_max 
void Route::lambda_min_max(Angle* lambda_min, Angle* lambda_max, [[maybe_unused]] String prefix) {
    
    String new_prefix;
    Angle t_min, t_max, temp;
    Position p_min, p_max;
    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    if (type == (Route_types[2])) {
        //*this is a circle of equal altitude
        
        if (abs(-tan(reference_position.phi.value) * tan((omega.value))) < 1.0) {
            //im this case ( abs(-tan(reference_position.phi.value)*tan((omega.value))) < 1.0) there exists a value of t = t_{max} (t_{min}) such that reference_position.lambda vs. t has a maximum (minimum). In this case, I proceed and compute this maximum and minimum, and write reference_position.lambda_{t = t_{min}} and reference_position.lambda_{t = t_{max}}] in lambda_min, lambda_max
            
            //compute the values of the parametric Angle t, t_min and t_max, which yield the position with the largest and smallest longitude (p_max and p_min) on the circle of equal altitude
            t_max.set(String(""), acos(-tan(reference_position.phi.value) * tan((omega.value))), new_prefix);
            t_min.set(String(""), 2.0 * M_PI - acos(-tan(reference_position.phi.value) * tan((omega.value))), new_prefix);
            
            //p_max =  Position on the circle of equal altitude  at t = t_max
            (length.value) = Re * sin((omega.value)) * (t_max.value);
            compute_end(new_prefix);
            p_max = end;
            
            //p_min =  Position on circle of equal altitude  at t = t_min
            (length.value) = Re * sin((omega.value)) * (t_min.value);
            compute_end(new_prefix);
            p_min = end;
            
            //set lambda_min/max in this order, which is eventually rectified at the end of this function
            (*lambda_min) = (p_min.lambda);
            (*lambda_max) = (p_max.lambda);
            
            
            /* p_max.print(String("p_max"), new_prefix, cout); */
            /* p_min.print(String("p_min"), new_prefix, cout); */
            
        }else {
            //in this case, reference_position.lambda vs. t has no minimum nor maximum: lambda_min/max are simly given by
            
            //set lambda_min/max in this order, meaning that *this spans all longitudes, from 0 to 2 pi
            (*lambda_min).set(String(""), 0.0, String(""));
            (*lambda_max).set(String(""), 0.0, String(""));
            
        }
        
        //sort lambda_min and lambda_max
        if ((*lambda_min) > (*lambda_max)) {
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
        //eventually swap lambda_min/max in such a way that lambda_min lies on the left and lambda_max lies on the right as seen from the observer's position looking at the earth's center
        if ((((*lambda_min).value) < M_PI) && (((*lambda_max).value) > M_PI)) {
            
            if (!(((reference_position.lambda) < (*lambda_min)) || ((reference_position.lambda) > (*lambda_max)))) {
                
                temp = (*lambda_min);
                (*lambda_min) = (*lambda_max);
                (*lambda_max) = temp;
                
            }
            
        }else {
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
        
    }else {
        //*this is a loxodrome or an orthodrome: in this case, the longitude along the Route *this is either a constantly increasing or a constantly decreasing function of the parameteric coordiante t -> set the minimal / maximal longitudes as the longitudes of the start and end point of this (or vice-versa)
        
        (*lambda_min) = (reference_position.lambda);
        
        compute_end(String(""));
        (*lambda_max) = (end.lambda);
        
        if((*lambda_min) > (*lambda_max)){
            
            temp = (*lambda_min);
            (*lambda_min) = (*lambda_max);
            (*lambda_max) = temp;
            
        }
        
    }
    
}


//comppute the extremal latitudes taken by the points lying on *this, if the type of *this allows it, and write them in *phi_min/max. If the extremal latitudes could not be computed, an error is printed, false is returned, and phi_min /max are not touched; otherwise the maximal and minimal latitude are stored into *phi_min/max, and true is returned
bool Route::phi_min_max(Angle* phi_min, Angle* phi_max, [[maybe_unused]] String prefix) {

    String new_prefix;
    Angle temp;
    Position p_min, p_max;
    bool check = false;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    
    switch (type.position_in_list(Route_types)) {
            
        case 0:{
            //*this is a loxodrome
            
            cout << prefix.value << RED << "Route is a loxodrome: phi min/max cannot be computed  for a loxodrome!\n" << RESET;
            check = false;

            break;
            
        }
        
        case 1:{
            //*this is an orthodrome
            
            double ts;
            //the candidate latitudes for the max and min latitude will be stored in phi
            vector<Angle> phi;
            
            set_length_from_time_speed();
            
            phi.clear();
            
            ts = atan(sin(reference_position.phi), cos(Z)*cos(reference_position.phi));
            
            //inlude in phi the latitude of the starting point of *this
            compute_end(Length(0.0), String(""));
            phi.push_back(end.phi.normalize_pm_pi_ret());
            
            
            //there are two potential stationary points for the latitude vs t: include in phi the first one, if it lies on *this
            if((0.0 <= Re*ts) && (length >= Re*ts)){
                
                //                t.push_back(Angle(ts));
                
                compute_end(Length(Re*ts), String(""));
                phi.push_back(end.phi.normalize_pm_pi_ret());
                
            }
            
            //there are two potential stationary points for the latitude vs t: include in phi the second one, if it lies on *this
            if((0.0 <= Re*(ts+M_PI)) && (length >= Re*(ts+M_PI))){
                
                //                t.push_back(Angle(ts+M_PI));
                
                compute_end(Length(Re*(ts+M_PI)), String(""));
                phi.push_back(end.phi.normalize_pm_pi_ret());
                    
            }
            
            //*include in *phi the latitude of the endpoint of *this
            compute_end(String(""));
            phi.push_back(end.phi.normalize_pm_pi_ret());
            
            
            //write the min/max element into *phi_min/max, respectively
            (*phi_min) = *min_element(
                                      phi.begin(),
                                      phi.end(),
                                      Angle::strictly_smaller_normalize_pm_pi_ret
                                      );
            
            (*phi_max) = *max_element(
                                      phi.begin(),
                                      phi.end(),
                                      Angle::strictly_smaller_normalize_pm_pi_ret
                                      );
            
            check = true;
            
            break;
            
        }
        
        case 2:{
            //*this is a circle of equal altitude
                
            (length.value) = Re * sin((omega.value)) * 0.0;
            compute_end(new_prefix);
            p_max = end;
            
            (length.value) = Re * sin((omega.value)) * M_PI;
            compute_end(new_prefix);
            p_min = end;
            
            //set lambda_min/max in this order, which is eventually rectified at the end of this function
            (*phi_min) = (p_min.phi);
            (*phi_max) = (p_max.phi);
            
            //sort phi_min and phi_max
            if ((*phi_min) > (*phi_max)) {
                
                temp = (*phi_min);
                (*phi_min) = (*phi_max);
                (*phi_max) = temp;
                
            }
            
            check = true;
            
            break;
            
        }
            
    }
    
    return check;
    
}



double Sight::rhs_DH_parallax_and_limb(double h, void* sight) {

    Sight* a = (Sight*)sight;

    //  (((*a).atmosphere).earth_radius.value)      R
    //(((*a).body).radius.value)                   r
    //(((*a).r).value)          d

    return(-(((*a).H_i).value) + h + asin((((*a).body).radius.value) / sqrt(gsl_pow_2((((*a).r).value)) + gsl_pow_2((((*a).atmosphere).earth_radius.value)) - 2.0 * (((*a).r).value) * (((*a).atmosphere).earth_radius.value) * sin(h))) - atan(((((*a).atmosphere).earth_radius.value) * cos(h)) / ((((*a).r).value) - (((*a).atmosphere).earth_radius.value) * sin(h))));

}

void Atmosphere::set(void) {

    unsigned int i;
    double x;
    bool check;

    //cout << "Atmosphere model: US 1976.\n";

    n_layers = 7;
    A = 0.7933516713545163;
    B = 34.16 * nm;
    P_dry_0 = 101325.0;
    alpha = -6.5 * nm;
    beta = 2.8 * nm;
    gamma = -2.8 * nm;
    T0 = 288.15;
    earth_radius.value = Re;

    h.resize(n_layers + 1);
    lambda.resize(n_layers);
    t.resize(n_layers);

    h[0] = 0.0;
    h[1] = 11.0 / nm;
    h[2] = 20.0 / nm;
    h[3] = 32.0 / nm;
    h[4] = 47.0 / nm;
    h[5] = 51.0 / nm;
    h[6] = 71.0 / nm;
    h[7] = 84.8520 / nm;

    lambda[0] = -6.5 * nm;
    lambda[1] = 0.0 * nm;
    lambda[2] = 1.0 * nm;
    lambda[3] = 2.8 * nm;
    lambda[4] = 0.0 * nm;
    lambda[5] = -2.8 * nm;
    lambda[6] = -2.0 * nm;


    for (i = 0, x = T0, check = true; (i < n_layers) && check; i++) {
        t[i] = x;
        x += lambda[i] * (h[i + 1] - h[i]);
    }


    /* for(int i=0; i<4+1; i++){ */
    /*   cout << "\t\t" << i << " " << h[i] << "\n"; */
    /* } */


}



void Body::print(String name_in, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name_in.value << ":\n";

    ostr << new_prefix.value << "Type = " << type.value << "\n";
    ostr << new_prefix.value << "Name = " << name.value << "\n";

    if (type == String("star")) {
        RA.print(String("Right ascension"), new_prefix, ostr);
        d.print(String("Declination"), new_prefix, ostr);
    }
    else {
        radius.print(String("Radius"), String("nm"), new_prefix, ostr);
    }

}




//this function checks whether the Body's name  is found in the body list comprised in catalog
bool Body::check(unsigned int* j, Catalog catalog, [[maybe_unused]] String prefix) {

    unsigned int i;
    bool check;

    for (i = 0, check = false; (i < (catalog).list.size()) && (!check); i++) {

        if (((((catalog).list)[i]).name) == name) {
            check = true;
        }

    }

    if (check) {
        (*j) = i - 1;
    }
    else {
        cout << prefix.value << RED << "Body not found in catalog!\n" << RESET;
    }

    return check;

}





Sight::Sight(void) {

    //this is the list of all the possible items that a Sight object can have: some Sight objects may have an item list with fewer elements than all_items. For instance, a star Sight does not have the "limb" element.
    all_items.push_back(String("body"));
    all_items.push_back(String("limb"));
    all_items.push_back(String("sextant altitude"));
    all_items.push_back(String("artificial horizon"));
    all_items.push_back(String("height of eye"));
    all_items.push_back(String("master-clock date and hour of sight"));
    all_items.push_back(String("use of stopwatch"));
    all_items.push_back(String("stopwatch reading"));
    all_items.push_back(String("label"));

    items.push_back(all_items[0]);
    items.push_back(all_items[2]);
    items.push_back(all_items[3]);
    items.push_back(all_items[5]);
    items.push_back(all_items[6]);
    items.push_back(all_items[8]);

    //initiazlie the limb to a 'n/a' value
    limb.value = 'n';
    atmosphere.set();
    (related_route.value) = -1;

}

//this function simplifies the atmosphere between z=0 and z=eight of eye as a single layer, where within this layer the index of refracion is independent of z. Refine this in the future.
void Sight::compute_DH_dip(String prefix) {

    Length zero_Length;
    zero_Length.value = 0.0;

    DH_dip.set(String("Dip correction"),
        -acos(atmosphere.n(zero_Length) / atmosphere.n(height_of_eye) * ((atmosphere.earth_radius.value) / ((atmosphere.earth_radius.value) + (height_of_eye.value)))), prefix);

}


bool Sight::compute_DH_refraction(String prefix) {

    gsl_integration_workspace* w = gsl_integration_workspace_alloc(1000);
    gsl_function F;
    double result, error;
    int status;
    bool check = true;



    F.function = &dH_refraction;
    F.params = &(*this);


    /* cout << "Value = " << dH_refraction(1.0, &(*this)); */
    /* cin >> result; */



    status = gsl_integration_qags(&F, (atmosphere.h)[(atmosphere.h).size() - 1], (atmosphere.h)[0], 0.0, epsrel, 1000, w, &result, &error);
    //status = GSL_FAILURE

    if (status == GSL_SUCCESS) {
        DH_refraction.set(String("refraction correction"), result, prefix);
    }
    else {
        check &= false;
        cout << prefix.value << RED << "GSL integration failed!\n" << RESET;
    }

    gsl_integration_workspace_free(w);

    return check;

}

//default constructor, which sets value to 0.0 by default and the unit to the first entry of LengthUnit_types
Length::Length() {

    value = 0.0;
    unit = LengthUnit(LengthUnit_types[0]);

}

//constructor which sets value to x  and the unit to the first entry of LengthUnit_types
Length::Length(double x) {

    value = x;
    unit = LengthUnit(LengthUnit_types[0]);

}


//constructor which sets value to x  and the unit to unit_in
Length::Length(double value_in, const LengthUnit& unit_in) {

    value = value_in;
    unit = unit_in;

}


//construct the Length *this frome time and speed, by setting it equal to time x speed and its unit equal to LengthUnit_types[0]. This method takes into account the units in which speed is expressed (which are stored into speed.unit)
Length::Length(Chrono time, Speed speed) {

    //conversion factor
    double c;
    
    //consider all possible units in which speed is expressed
    switch (speed.unit.position_in_list(SpeedUnit_types)) {
            
        case 0: {
            //speed.unit = SpeedUnit_types[0]
            
            c = 1.0;
            
            break;
            
        }

        case 1: {
            //speed.unit = SpeedUnit_types[1]

            c = 1.0/nm;
            
            break;
            
        }
            
        case 2: {
            //speed.unit = SpeedUnit_types[2]
            
            //[m]/[s] = 1e-3 3600 [km]/[h] = 1e-3 3600 / nm [nm]/[h] = 1e-3 3600 / nm [kt]
            c = (1e-3)*60.0*60.0/nm;
            
            break;
            
        }


    }
    
    set(c * (time.get()) * (speed.value));
    unit.set(LengthUnit_types[0]);

}


//set the value of *this equal to x (expressed in units LengthUnit_types[0]). The unit is not modified
inline void Length::set(String name, double x, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    value = x;

    if(name != String("")){
        print(name, String("nm"), prefix, cout);
    }
    check_valid(name, new_prefix);

}


//same as Length::set(String name, double x, [[maybe_unused]] String prefix)  but without printing out anything
inline void Length::set(double x) {
    
    value = x;
}


//enter a length in meters
void Length::enter(String name_in, String unit_in, [[maybe_unused]] String prefix) {

    stringstream temp;

    temp.clear();
    temp << name_in.value;
    if (unit_in == String("nm")) {
        temp << " [nm]";
    }
    else {
        temp << " [m]";
    }

    do {

        enter_double(&value, false, 0.0, 0.0, temp.str(), prefix);

    } while (!check_valid(name_in, prefix));

    //if the length has been entered in units of m, convert it to nautical miles
    if (unit_in == String("m")) {
        value /= (1e3 * nm);
    }

    print(name_in, unit_in, prefix, cout);

}

string Length::to_string(String unit_in, unsigned int precision) {

    stringstream output;

    output.precision(precision);

    if (unit_in == String("nm")) { output << fixed << value << " nm"; }
    if (unit_in == String("m")) { output << fixed << value * 1e3 * nm << " m"; }

    return(output.str().c_str());

}

void Length::print(String name_in, String unit_in, String prefix, ostream& ostr) {

    if ((name_in.value) != "") {

        ostr << prefix.value << name_in.value << " = ";
        if (unit_in == String("nm")) {
            ostr << value << " nm\n";
        }
        else {
            ostr << value * nm * 1e3 << " m\n";
        }

    }

}

bool Sight::get_coordinates(Route* circle_of_equal_altitude, [[maybe_unused]] String prefix) {

    FileR file;
    stringstream filename, line_ins;
    string line, dummy, temp;
    int l, l_min, l_max;
    double MJD_tab[(unsigned int)N], GHA_tab[(unsigned int)N], d_tab[(unsigned int)N], sum;
    gsl_interp_accel* acc = gsl_interp_accel_alloc();
    gsl_spline* interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N)), * interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));
    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Fetching ephemerides' data ...\n";

    if ((body.type.value) != "star") {
        filename << (wxGetApp().data_directory).value << body.name.value << ".txt";
    }
    else {
        filename << (wxGetApp().data_directory).value << "j2000_to_itrf93.txt";
    }
    temp = filename.str();


    file.set_name(String(temp.c_str()));
    if ((file.open(new_prefix)) && check_time_interval(new_prefix)) {
        //the file corresponding to this->body exists and the time of *this lies within the time interval of NASA JPL ephemerides data


        //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
        l_min = (int)(L * ((time.MJD) - MJD_min)) - (int)(N / 2.0);
        //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
        l_max = (int)(L * ((time.MJD) - MJD_min)) + (int)(N / 2.0);

        /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */

        //dummy read of file data
        for (l = 0; l < l_min; l++) {
            line.clear();
            getline(*(file.value), line);
        }


        if ((body.type.value) != "star") {
            //in this case I am getting the coordinate of a body with a non-zero size

            //if the body is not a star

            double r_tab[(unsigned int)N];
            gsl_spline* interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)N));

            for (; l < l_max; l++) {

                line.clear();
                line_ins.clear();

                getline(*(file.value), line);
                line_ins << line;
                cout << new_prefix.value << line << "\n";
                line_ins >> dummy >> dummy >> dummy >> GHA_tab[l - l_min] >> d_tab[l - l_min] >> r_tab[l - l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;

                MJD_tab[l - l_min] = ((double)(l - l_min)) / L;

            }

            file.close(new_prefix);

            //convert to radians and nm
            for (l = 0; l < N; l++) {

                //add minus sign because in JPL convention longitude is positive when it is E
                GHA_tab[l] *= (-1.0) * k;
                d_tab[l] *= k;
                r_tab[l] /= nm;

            }

            //remove discontinuous jumps in GHA to allow for interpolation
            for (sum = 0.0, l = 0; l < N - 1; l++) {
                //cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
                if (((GHA_tab[l] - sum) > 0.0) && (GHA_tab[l + 1] < 0.0)) {
                    sum += 2.0 * M_PI;
                }
                GHA_tab[l + 1] += sum;
            }

            if (gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; };

            if (gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }
            if (gsl_spline_init(interpolation_r, MJD_tab, r_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }


            cout << new_prefix.value << "Read values:\n";
            for (l = 0; l < N; l++) {
                cout << new_prefix.value << MJD_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
            }

            if (gsl_spline_eval_e(interpolation_GHA, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).lambda).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).lambda).normalize();
                (((*circle_of_equal_altitude).reference_position).lambda).print(String("GHA"), new_prefix, cout);
            }
            //(((*circle_of_equal_altitude).reference_position).lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);


            if (gsl_spline_eval_e(interpolation_d, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).phi).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).phi).normalize();
                (((*circle_of_equal_altitude).reference_position).phi).print(String("d"), new_prefix, cout);
            }
            //(((*circle_of_equal_altitude).reference_position).phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

            if (gsl_spline_eval_e(interpolation_r, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &(r.value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                if ((r.check_valid(String("r"), new_prefix))) {
                    r.print(String("r"), String("nm"), new_prefix, cout);
                }
                else {
                    check &= false;
                }
            }
            //r.set("r", gsl_spline_eval(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

            gsl_spline_free(interpolation_r);


        }
        else {
            //in this case I am getting the coordinate of a body with a zero size (a star)

            //if the body is a star
            double phi3, phi2, phi1;

            for (; l < l_max; l++) {

                line.clear();
                line_ins.clear();

                getline(*(file.value), line);
                line_ins << line;
                cout << new_prefix.value << line << "\n";
                line_ins >> dummy >> dummy >> dummy >> phi3 >> phi2 >> phi1;

                phi1 *= k;
                phi2 *= k;
                phi3 *= k;

                d_tab[l - l_min] = asin(cos(phi2) * sin((body.d.value)) - cos((body.d.value)) * cos(phi1) * sin((body.RA.value)) * sin(phi2) + cos((body.RA.value)) * cos((body.d.value)) * sin(phi1) * sin(phi2));

                GHA_tab[l - l_min] = atan((-cos(phi3) * sin((body.d.value)) * sin(phi2) - cos((body.RA.value)) * cos((body.d.value)) * (-cos(phi2) * cos(phi3) * sin(phi1) - cos(phi1) * sin(phi3)) - cos((body.d.value)) * sin((body.RA.value)) * (cos(phi1) * cos(phi2) * cos(phi3) - sin(phi1) * sin(phi3))) / (sin((body.d.value)) * sin(phi2) * sin(phi3) + cos((body.d.value)) * sin((body.RA.value)) * (cos(phi3) * sin(phi1) + cos(phi1) * cos(phi2) * sin(phi3)) + cos((body.RA.value)) * cos((body.d.value)) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))));
                if ((sin((body.d.value)) * sin(phi2) * sin(phi3) + cos((body.d.value)) * sin((body.RA.value)) * (cos(phi3) * sin(phi1) + cos(phi1) * cos(phi2) * sin(phi3)) + cos((body.RA.value)) * cos((body.d.value)) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))) < 0.0) {
                    GHA_tab[l - l_min] += M_PI;
                }
                GHA_tab[l - l_min] = GHA_tab[l - l_min] - 2.0 * M_PI * floor(GHA_tab[l - l_min] / (2.0 * M_PI));


                MJD_tab[l - l_min] = ((double)(l - l_min)) / L;

            }

            file.close(new_prefix);



            //remove discontinuous jumps in GHA to allow for interpolation
            for (sum = 0.0, l = 0; l < N - 1; l++) {
                //cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
                if (((GHA_tab[l] - sum) > M_PI) && (GHA_tab[l + 1] < M_PI)) {
                    sum += 2.0 * M_PI;
                }
                GHA_tab[l + 1] += sum;
            }

            cout << new_prefix.value << "Read values:\n";
            for (l = 0; l < N; l++) {
                cout << new_prefix.value << MJD_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
            }

            if (gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }
            if (gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)N) != GSL_SUCCESS) { check &= false; }


            if (gsl_spline_eval_e(interpolation_GHA, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).lambda).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).lambda).normalize();
                (((*circle_of_equal_altitude).reference_position).lambda).print(String("GHA"), new_prefix, cout);
            }

            if (gsl_spline_eval_e(interpolation_d, (time.MJD) - MJD_min - ((double)l_min) / L, acc, &((((*circle_of_equal_altitude).reference_position).phi).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (((*circle_of_equal_altitude).reference_position).phi).normalize();
                (((*circle_of_equal_altitude).reference_position).phi).print(String("d"), new_prefix, cout);
            }

        }

        //set the length of the circle of equal altitude
        //(*circle_of_equal_altitude).l.set(String("length of circle of equal altitude"), 2.0*M_PI*Re*sin((*circle_of_equal_altitude).omega.value), new_prefix);

    }
    else {

        check &= false;
    }

    if (!check) {

        cout << prefix.value << RED << "Cannot fetch ephemerides' data!\n" << RESET;

    }
    else {

        cout << prefix.value << "...done\n";

    }

    gsl_interp_accel_free(acc);
    gsl_spline_free(interpolation_GHA);
    gsl_spline_free(interpolation_d);

    return check;

}

//set the value of *this equal to x and normalize *this
inline void Angle::set(double x){
    
    value = x;
    normalize();

}

//call Angle::set(x) and print name with prefix 
inline void Angle::set(String name, double x, [[maybe_unused]] String prefix) {

    set(x);
    if(name != String("")) { print(name, prefix, cout); }

}

//this fucnction takes an angle entered from keyboard. the angle may or may not contain a sign in front of it. If it does not contain a sign, the sign is interpreted as '+'
void Angle::enter(String name, [[maybe_unused]] String prefix) {

    unsigned int ad;
    double am;
    String new_prefix;
    string input;
    stringstream input_unit;
    size_t pos;
    bool check;
    char s;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));



    input_unit.clear();
    input_unit << name.value << " [(s )ddd mm.m]";

    do {

        check = true;

        //enter the input string, containing degrees and arcminutes with a ' ' in between
        cout << prefix.value << "Enter " << input_unit.str() << ":";
        getline(cin >> ws, input);

        //find the position of the first ' '
        pos = (input).find(" ");

        if ((((input).substr(0, pos)) == "+") || (((input).substr(0, pos)) == "-")) {
            //in this case, there is a '+' or '-' sign in front the angle

            //check whether the string contains only two ' ', one after the sign, one adter the degrees
            check &= (count((input).begin(), (input).end(), ' ') == 2);

            //set the sign
            s = ((input).substr(0, pos).c_str())[0];

            //now I am no longer interested in the sign, the string runs from the degrees to the end of arcminutes
            (input) = ((input).substr(pos + 1).c_str());

        }
        else {
            //in this case, there is no '+' nor '-' in front of the angle -> the sign is set to '+'

            //set the sign
            s = '+';

            cout << prefix.value << YELLOW << "No sign has been entered. Interpreting it as +!\n" << RESET;

            //check whether the string contains only one ' ',  after the degrees
            check &= (count((input).begin(), (input).end(), ' ') == 1);

        }

        if (check) {

            //find the position of the  ' ' between degrees and arcminutes
            pos = (input).find(" ");

            //check whether degree part is formatted correctly
            check &= check_unsigned_int((input).substr(0, pos).c_str(), NULL, true, 0, 360);

            //check whether arcminute part is formatted correctly
            check &= check_double((input).substr(pos + 1, (input).size() - pos).c_str(), NULL, true, 0.0, 60.0);

        }

        if (!check) {
            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;
        }

    } while (!check);



    //set degrees and arcminutes
    ad = stoi((input).substr(0, pos).c_str(), NULL, 10);
    am = stod((input).substr(pos + 1, (input).size() - pos).c_str());

    value = k * (((double)ad) + am / 60.0);
    if (s == '-') {
        value *= -1.0;
    }

    normalize();

    print(name, prefix, cout);

}


//set the polar coordinates lambda, phi of (*this) from the Cartesian position r
void Position::setCartesian(String name, const Cartesian r, [[maybe_unused]] String prefix) {

    String new_prefix, name_lambda, name_phi;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if (name != String("")) {
        name_lambda = String("longitude");
        name_phi = String("latitude");
    }
    else {
        name_lambda = String("");
        name_phi = String("");
    }

    if (name != String("")) {
        cout << prefix.value << name.value << "\n";
    }

    lambda.set(name_lambda, -atan(gsl_vector_get((r.r), 0), gsl_vector_get((r.r), 1)), String(prefix));
    phi.set(name_phi, asin(gsl_vector_get((r.r), 2) / gsl_blas_dnrm2((r.r))), String(prefix));

}

//write the cartesian components of Position p into *r
void Position::getCartesian(Cartesian* r) {

    gsl_vector_set((r->r), 0, cos(phi) * cos(lambda));
    gsl_vector_set((r->r), 1, -cos(phi) * sin(lambda));
    gsl_vector_set((r->r), 2, sin(phi));

}

//same as Position::getCartesian(Cartesian* r) , but it returns the Cartesian position rather than writing it into *r
Cartesian Position::getCartesian(void){
    
    Cartesian temp;
    
    getCartesian(&temp);
    
    return temp;
    
}

void Position::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    phi.print(String("latitude"), new_prefix, ostr);
    lambda.print(String("longitude"), new_prefix, ostr);

    label.print(String("label"), true, new_prefix, ostr);

}

//reads from file the content after 'Position # ...' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Position::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Position>(this, name, filename, mode, prefix);

}



void Angle::normalize(void) {

    value = value - 2.0 * M_PI * floor(value / (2.0 * M_PI));

}

Angle Angle::normalize_ret(void) {

    Angle temp;

    temp.set(String(""), value - 2.0 * M_PI * floor(value / (2.0 * M_PI)), String(""));
    return temp;
}


//put the angle *this in the interval [-pi, pi) and write the result in *this
void Angle::normalize_pm_pi(void) {

    normalize();
    if (value > M_PI) { value -= 2.0 * M_PI; }

}

//returns the angular span between *this and x, where it must be (*this).value < x.value, by taking into account the periodicity of *this and x with respect to 2 pi
Angle Angle::span(Angle x) {

    Angle delta;

    if (((*this) < M_PI) && (x > M_PI)) {

        delta.set(String(""), (value + (2.0 * M_PI)) - (x.value), String(""));

    }
    else {

        delta.set(String(""), value - (x.value), String(""));

    }

    return delta;

}

//puts the angle in the interval [-pi, pi), it does not alter *this and returns the result
inline Angle Angle::normalize_pm_pi_ret(void) {

    Angle temp;

    temp = (*this);
    temp.normalize_pm_pi();

    return temp;

}



void Angle::print(String name, String prefix, ostream& ostr) {

    normalize();
    ostr << prefix.value << name.value << " = " << floor(K * value - 360.0 * floor(K * value / 360.0)) << (wxGetApp().degree_symbol.value) << " " << (K * value - 360.0 * floor(K * value / 360.0) - floor(K * value - 360.0 * floor(K * value / 360.0))) * 60.0 << "'\n";

}


//return true is *this is close to 0 or 2 pi within precision epsilon_double, false otherwise
inline bool Angle::is_zero_epsilon_double(void){
    
    return((fabs(value) < epsilon_double) || (fabs(value-2.0*M_PI) < epsilon_double));

}


//convert the angle contained in (*this).value to degrees and minutes format, storted in deg and min
void Angle::to_deg_min(unsigned int* deg, double* min) {

    normalize();
    (*deg) = (unsigned int)floor(K * value - 360.0 * floor(K * value / 360.0));
    (*min) = (K * value - 360.0 * floor(K * value / 360.0) - floor(K * value - 360.0 * floor(K * value / 360.0))) * 60.0;

}


//convert the angle contained in (*this).value to degrees and minutes format, storted in deg and min,  round up the minutes part with precision `precision`, and re-format deg and min in order to avoid results of the form *deg = 23, *min = 60.0
void Angle::to_deg_min(unsigned int* deg, double* min, unsigned int precision) {

    to_deg_min(deg, min);
    (*min) = round_with_precision((*min), precision);
    if((*min) == 60.0){
        (*min) = 0.0;
        (*deg)++;
    }
    
}


//convert the angle stored in degrees and minutes format in deg an min in to (*this).vaule
void Angle::from_sign_deg_min(char sign, unsigned int deg, double min) {

    value = k * (((double)deg) + min / 60.0);
    if (sign == '-') { value *= -1.0; }

    normalize();

}

//this function converts an Angle to a string. If add_spaces = true, then instead of "9° 2.3'" I output "  9°  2.3'", i.e., I fill the spaces with blank spaces, so all angles will have the same format when converted to strings
string Angle::to_string(String mode, unsigned int precision, bool add_spaces) {

    stringstream output;
    stringstream deg, min;
    int i;
    double x;
    //a temporary variable where to store this->value and modifyi it without altering this->value
    double value_temp;

    min.precision(precision);

    normalize();
    value_temp = value;


    if ((mode != String("")) && (value_temp > M_PI)) {
        value_temp -= 2.0 * M_PI;
        value_temp = fabs(value_temp);
    }

    //write the arcdegree part of the Angle into deg
    deg.str("");
    i = floor(K * value_temp);
    if (add_spaces) {
        if (i < 10) {
            deg << "  ";
        }
        else {
            if (i < 100) {
                deg << " ";
            }
        }
    }

    
    //write the arcminute part of the Angle into min
    min.str("");
    
    //round up to the precision `precision`
    x = round_with_precision((K * value_temp - floor(K * value_temp)) * 60.0, precision);
    //if, after the round up, x is equal to 60.0 (exactly), I set x to 0.0 and increase the defrees by one: in this way, I will never have weird angle vlaues such as 1 degree 60.0'
    if(x == 60.0){
        x = 0.0;
        i++;
    }
    
    if (add_spaces) {
        //sets the fixed precision min, so all angles that are printed out have the same number of decimal points
        min << fixed;
        if (x < 10.0) {
            min << " ";
        }
    }
    
    deg << i;
    min << x;



    output << deg.str().c_str() << (wxGetApp().degree_symbol).value << " " << min.str().c_str() << "'";

    if ((mode != String("")) && !is_zero_epsilon_double()) {
        //mode is not "" and the angle is nonzero -> I print out its 'sign' (N, S, E, W, ...)

        if (mode == String("NS")) {
            //in this case, I output the sign of the angle in the North/South format (North = +, South = -)

            if (value < M_PI) { output << " N"; }
            else { output << " S"; }
        }
        if (mode == String("EW")) {
            //in this case, I output the sign of the angle in the East/West format (West = +, East = -)

            if (value < M_PI) { output << " W"; }
            else { output << " E"; }
        }

    }

    return (output.str().c_str());

}


//this function prints out only the integer degree closest to this
string Angle::deg_to_string(String mode, [[maybe_unused]] unsigned int precision) {

    stringstream output;

    //    output.precision(precision);

    normalize();

    if (mode == String("")) {
        //in this case, I print out the angle in the format >=0° and <360°
        output << round(K * value) << wxGetApp().degree_symbol.value;

    }
    else {
        //in this case, I print out the angle in the format >=-180° and <180°


        //I append NS or EW only if the angle is != 0, otherwise it is pointless to add these labels
        if (!is_zero_epsilon_double()) {

            if (mode == String("NS")) {
                //in this case, I output the sign of the angle in the North/South format (North = +, South = -)

                if (value < M_PI) {

                    if (value < M_PI_2) {

                        output << round(fabs(K * value)) << wxGetApp().degree_symbol.value << " N";

                    }
                    else {

                        output << round(fabs(K * (M_PI - value))) << wxGetApp().degree_symbol.value << " N";

                    }

                }
                else {

                    if (value < 3.0 * M_PI_2) {

                        output << round(fabs(K * (-M_PI + value))) << wxGetApp().degree_symbol.value << " S";

                    }
                    else {

                        output << round(fabs(K * (2.0 * M_PI - value))) << wxGetApp().degree_symbol.value << " S";

                    }

                }

            }
            else {
                //in this case, I output the sign of the angle in the East/West format (West = +, East = -)

                if (value > M_PI) { value -= 2.0 * M_PI; }
                output << round(fabs(K * value)) << wxGetApp().degree_symbol.value;

                if (value > 0.0) { output << " W"; }
                else { output << " E"; }

            }

        }
        else {

            output << "0" << wxGetApp().degree_symbol.value;

        }

    }

    return (output.str().c_str());

}

//this function prints out only the arcminute part of this
string Angle::min_to_string(String mode, unsigned int precision) {

    stringstream output;

    output.precision(precision);

    normalize();

    if (mode != String("")) {
        //in this case, I print out the angle in the format >=-180° and <180°
        if (value > M_PI) { value -= 2.0 * M_PI; }
    }

    output << (fabs(K * value) - floor(fabs(K * value))) * 60.0 << "'";

    return (output.str().c_str());

}


//returns true if a.normalize_pm_pi_ret().value < b.normalize_pm_pi_ret().value and false otherwise
inline bool Angle::strictly_smaller_normalize_pm_pi_ret(Angle& a, Angle& b){
    
    return((a.normalize_pm_pi_ret().value) < (b.normalize_pm_pi_ret().value));
    
}

//returns true if a.normalize_pm_pi_ret().value > b.normalize_pm_pi_ret().value and false otherwise
inline bool Angle::strictly_larger_normalize_pm_pi_ret(Angle& a, Angle& b){
    
    return((a.normalize_pm_pi_ret().value) > (b.normalize_pm_pi_ret().value));
    
}


void Limb::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << value << "\n";

}



//convert *this to a string. If time_zone == true, then I add at the end of the string the time zone compared to UTC written in wxGetApp().time_zone
string Chrono::to_string(unsigned int precision, bool time_zone) {

    stringstream output;

    output.precision(precision);

    if (h < 10) { output << 0; }
    output << h << ":";
    if (m < 10) { output << 0; }
    output << m << ":";
    if (s < 10.0) { output << 0; }
    output << s;
    if (time_zone) {
        output << " UTC" << (wxGetApp().time_zone > 0 ? "+" : "-") << fabs(wxGetApp().time_zone.value);
    }

    return (output.str().c_str());

}

void Date::print(String name, String prefix, ostream& ostr) {

    ostr << prefix.value << name.value << " = " << to_string() << "\n";

};

//this function sets (*this) to the current UTC date and time
void Time::set_current(void) {

    date.set_current();
    chrono.set_current();

}

//this function sets (*this) to the current UTC date
void Date::set_current(void) {

    (wxGetApp().local_time) = (boost::posix_time::second_clock::local_time());

    Y = (wxGetApp().local_time).date().year();
    M = (wxGetApp().local_time).date().month().as_number();
    D = (wxGetApp().local_time).date().day();

}


//return the value of *this expresser in hours
inline double Chrono::get(void) {

    return(((double)h) + ((double)m) / 60.0 + ((double)s) / (60.0 * 60.0));

}


//this function sets (*this) to the current UTC time +- time_zone
void Chrono::set_current(void) {

    (wxGetApp().local_time) = (boost::posix_time::second_clock::local_time());

    h = ((unsigned int)((wxGetApp().local_time).time_of_day().hours()));
    m = ((unsigned int)((wxGetApp().local_time).time_of_day().minutes()));
    s = (wxGetApp().local_time).time_of_day().seconds();

}


void Date::enter(String name, String prefix) {

    string input;
    String new_prefix;
    bool check;
    size_t pos;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    do {

        check = true;

        cout << prefix.value << "Enter " << name.value << " [YYYY MM DD], or press enter for current UTC date:";
        getline(cin, input);

        if (input.empty()) {

            cout << prefix.value << YELLOW << "Entered an empty date, setting it to current UTC date!\n" << RESET;
            set_current();
            print(String("entered date"), prefix, cout);

        }
        else {

            //check whether the string contains two ' '
            check &= (count(input.begin(), input.end(), ' ') == 2);

            if (check) {

                pos = (input).find(" ");

                //check whether year part is formatted correctly
                check &= check_unsigned_int(input.substr(0, pos).c_str(), &Y, false, 0, 0);

                if (check) {
                    //in this case, the year's format is valid

                    //I check whether Y is a leap year and compute days_per_month
                    check_leap_year();
                    if (Y_is_leap_year) {

                        days_per_month = days_per_month_leap;
                        cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET;

                    }
                    else {

                        days_per_month = days_per_month_common;
                        cout << new_prefix.value << "Entered a common year\n";

                    }

                    //now I am no longer interested in the year, the string runs from the month to days
                    input = (input.substr(pos + 1).c_str());

                    //find the position of the second ' '
                    pos = input.find(" ");

                    //check whether month part is formatted correctly
                    check &= check_unsigned_int(input.substr(0, pos).c_str(), &M, true, 1, 12 + 1);

                    if (check) {
                        //in this case the month part is formatted correctly

                        //now I am no longer interested in the month, the string runs from the days to the end of the string
                        input = (input.substr(pos + 1).c_str());

                        //check whether day part is formatted correctly
                        check &= check_unsigned_int(input.c_str(), &D, true, 1, days_per_month[M - 1] + 1);

                    }

                }

            }

        }

        if (!check) {

            cout << prefix.value << RED << "\tEntered value is not valid!\n" << RESET;

        }

    } while (!check);


    /* check_leap_year(); */
    /* if((Y_is_leap_year)){ */
    /*   (days_per_month) = days_per_month_leap; */
    /*   cout << new_prefix.value << YELLOW << "Entered a leap year\n" << RESET; */
    /* }else{ */
    /*   (days_per_month) = days_per_month_common; */
    /*   cout << new_prefix.value << "Entered a common year\n"; */
    /* } */

    //enter_unsigned_int(&M, true, 1, 12+1, String("MM"), prefix);

    //enter_unsigned_int(&D, true, 1, days_per_month[M-1]+1, String("DD"), prefix);

}


string Date::to_string(void) {

    stringstream output;

    output << Y << "-";
    if (M < 10) { output << 0; }
    output << M << "-";
    if (D < 10) { output << 0; }
    output << D;

    return (output.str().c_str());

}


//print *this to ostr
void Chrono::print(String name, String prefix, ostream& ostr) {

    unsigned int precision;

    //if I am printing to terminal, I print with display_precision. Otherwise, I print with (data_precision.value)
    if (ostr.rdbuf() == cout.rdbuf()) {
        precision = (display_precision.value);
    }
    else {
        precision = (data_precision.value);
    }

    ostr << prefix.value << (name.value) << " = " << to_string(precision, false) << "\n";

};



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

ChartPanel::ChartPanel(ChartFrame* parent_in, const wxPoint& position, const wxSize& size) : wxPanel(parent_in, wxID_ANY, position, size, wxTAB_TRAVERSAL, wxT("")) {

    parent = parent_in;

}


//get the datapoints of coastlines and store them into parent->coastline_polygons_now
void ChartFrame::GetCoastLineData_3D(void) {

    unsigned long long int i, j;
    //integer values of min/max lat/lon to be extractd from p_coastline
    int/*  i_adjusted = 0, j_adjusted = 0,*/ i_min, i_max, j_min, j_max;
//    double /*the cosine of the angle between the vector with latitude and longitude i, j (see below) and the vector that connects the center ofr the Earth to circle_observer.reference_position*/cos;
    PositionProjection temp;
    wxPoint q;
    Cartesian r, s;
    Position u;


    //set i_min/max, j_min/max
    i_min = floor(K * (phi_min.normalize_pm_pi_ret().value));
    i_max = ceil(K * (phi_max.normalize_pm_pi_ret().value));

    if ((lambda_min == 0.0) && (lambda_max == 0.0)) {
        //in this case,Set_lambda_phi_min_max found out that circle_observer spans all longitudes, thus I set

        j_min = 0;
        j_max = 360;

    }
    else {
        //in this case, Set_lambda_phi_min_max found out that there are two finite longitudes which encircle circle_observer, thus I set

        if ((lambda_min < M_PI) && (lambda_max > M_PI)) {

            j_min = floor(K * (lambda_max.value));
            j_max = 360 + ceil(K * (lambda_min.value));

        }
        else {

            j_min = floor(K * (lambda_max.value));
            j_max = ceil(K * (lambda_min.value));

        }

    }
    
    if(i_min < floor_min_lat){i_min = floor_min_lat;}
    if(i_max > ceil_max_lat){i_max = ceil_max_lat;}

    
    if ((parent->show_coastlines) == Answer('y', String(""))) {
        
        unsigned long long int  l, p, m, every, n_added_polygons;
        bool new_polygon;
        
        
        //go through coastline_polygons_map and fetch the polygons that fall within rectangle_observer and store their ids into coastline_polygons_area_observer
        for(parent->coastline_polygons_area_observer.clear(), i=i_min-floor_min_lat; i<i_max-floor_min_lat; i++) {
            for(j=j_min; j<j_max; j++) {
                
                for(l=0; l<(parent->coastline_polygons_map)[i][j % 360].size(); l++){
                    parent->coastline_polygons_area_observer.push_back((parent->coastline_polygons_map)[i][j % 360][l]);
                }
                
            }
        }
        //the procedure above may lead to duplicates into coastline_polygons_area_observer -> delete them
        delete_duplicates(&(parent->coastline_polygons_area_observer));
        
        
        //count the total number of points included in the polygons of coastline_polygons_area_observer and store them in m
        //set every in such a way that the total number of plotted points is n_points_plot_coastline_3D, no matter what the size of rectangle_observer
        //        for(m=0, i=0; i<parent->coastline_polygons_area_observer.size(); i++) {
        //            for(j=0; j<(parent->coastline_polygons_Cartesian)[(parent->coastline_polygons_area_observer)[i]].size(); j++){
        //                
        //                if(((draw_panel->*(draw_panel->CartesianToProjection))((parent->coastline_polygons_Cartesian)[(parent->coastline_polygons_area_observer)[i]][j], NULL, false))){
        //                    m++;
        //                }
        //                
        //            }
        //        }
        //        every = ((unsigned long long int)(((double)m) / ((double)(wxGetApp().n_points_plot_coastline_3D.value))));
        //        if(every==0){every = 1;}
        
        //this is a computationally efficient way of estimating every: the number of coastline points falling within circle_observer is estimated as (parent->n_all_coastline_points)*(draw_panel->circle_observer.omega.value))/M_PI), and every is set accordingly in such a way that, for every circle_observer, the number of plotting points is n_points_plot_coastline_3D
        every = ((unsigned long long int)(((parent->n_all_coastline_points)*(draw_panel->circle_observer.omega.value))/M_PI) / ((double)(wxGetApp().n_points_plot_coastline_3D.value)));
        if(every==0){every = 1;}
        
        
        
        for(p=0, i=0, l=0, n_added_polygons=0, polygon_position_now.clear(); i<parent->coastline_polygons_area_observer.size(); i++) {
            //run through polygons
            
            new_polygon=true;
            n_added_polygons++;
            if(n_added_polygons > polygon_position_now.size()){
                polygon_position_now.resize(n_added_polygons);
            }
            polygon_position_now[n_added_polygons-1] = l;
            
            //the id of the polygon that is being added, i.e. , the # of the polygon as entry of coastline_polygons_Position
            m = (parent->coastline_polygons_area_observer)[i];
            
            for(j=p; j<(parent->coastline_polygons_Cartesian)[m].size(); j+=every){
                //run through points in a polygon
                
                if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[m][j], &q, false)){
                    
                    coastline_polygons_now[l++] = q;
                    new_polygon = false;
                    
                }else{
                    //(parent->coastline_polygons_Position)[i][j] is not a valid point -> I start a new polygon
                    
                    if(!new_polygon){
                        
                        //updated polygon_position_now with the position of the new polygon
                        new_polygon = true;
                        n_added_polygons++;
                        if(n_added_polygons > polygon_position_now.size()){
                            polygon_position_now.resize(n_added_polygons);
                        }
                        polygon_position_now[n_added_polygons-1] = l;
                        
                    }

                }
                
            }
            
            p = j - ((parent->coastline_polygons_Cartesian[m]).size());

        }
        
        if(n_added_polygons+1 > polygon_position_now.size()){
            polygon_position_now.resize(n_added_polygons+1);
        }
        polygon_position_now.back() = l;

        
        
//        every = ((unsigned long long int)(((double)(parent->n_all_coastline_points)) * (  draw_panel->circle_observer.omega.value ) / ( draw_panel->circle_observer_0.omega.value ) ) / ((double)(wxGetApp().n_points_plot_coastline_3D.value)));
//        if(every==0){every = 1;}
        
        
        

//        for(p=0, i=0, l=0; i<parent->coastline_polygons_Cartesian.size(); i++) {
//            //run through polygons
//            
//            polygon_position_now[i] = 0;
//            for(j=p; j<(parent->coastline_polygons_Cartesian[i]).size(); j+=every){
//                //run through points in a polygon
//
//                
//                if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[i][j], &q, false)) {
//                    
//                    coastline_polygons_now[l++] = q;
//                    polygon_position_now[i]++;
//
//                }
//                
//                
//            }
//            
//            p = j - ((parent->coastline_polygons_Cartesian[i]).size());
//
//        }
        
        //set r
//        draw_panel->circle_observer.reference_position.getCartesian(String(""), &r, String(""));
        
//        //
//        for (n=0, i = i_min; i < i_max; i++) {
//            for (j = j_min; j < j_max; j++) {
//                
//                
//                if ((draw_panel->AdjustLatitudeLongitude3D(i, j, &i_adjusted, &j_adjusted))) {
//                    
//                    n += (parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360].size();
//                    
//                }
//                
//                
//            }
//            
//        }
        
//        every = (unsigned long long int)(((double)n) / ((double)(parent->data->n_points_plot_coastline_3D.value)));
//        n_points_per_cell = ((unsigned long long int)((double)n)/((double)n_cells));
//        
//        for (polygon_position_now=0, p=0, i = i_min; i < i_max; i++) {
//            for (j = j_min; j < j_max; j++) {
//                
//                if ((draw_panel->AdjustLatitudeLongitude3D(i, j, &i_adjusted, &j_adjusted))) {
//
//                    
////                    //n =  how many datapoints are in data_x[i][j] and in data_y[i][j]
////                    n = ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size();
//                    
//                
//                    //set s
//                    u.phi.set(String(""), k * ((double)i), String(""));
//                    u.lambda.set(String(""), k * ((double)j), String(""));
//                    u.getCartesian(String(""), &s, String(""));
//                    //compute cos
//                    gsl_blas_ddot((r.r), (s.r), &cos);
//                    if (cos == 0.0) { cos = 1.0; }
//                    
//                    
//                    //I plot every 'every_ij' data points. I include the factor 1/cos in such a way that the farther the point (i,j) from circle_observer.reference_position, the less data points I plot, because plotting more would be pointless. In this way, points (i,j) which are close to circle_observer.reference_position (which are nearly parallel to the plane of the screen and thus well visible) are plotted with a lot of points, and the other way around
////                    every = (unsigned long)(((double)n) / ((double)(((parent->data)->n_points_plot_coastline_3D).value)) * ((double)n_points_grid) / cos);
////                    if (every == 0) { every = 1; }
//                    every_ij =
//                    ceil(((double)every)/cos * ((double)((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size())/
//                    (((double)n)/((double)n_cells)));
//                    if(every_ij == 0){
//                        every_ij = 1;
//                    }
//                    
//                    
//                    
//                    //run over data_x)[i - floor_min_lat][j % 360] by picking one point every every points
//                    for (l = min(p, ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size() - n_points_per_cell); l < ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size(); l += every_ij) {
//                        
//                        if((draw_panel->CartesianToDrawPanel)((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360][l], &q, false)) {
//                            
//                            coastline_polygons_now[polygon_position_now++] = q;
//                            
//                        }
//             
//                    }
//                    
//                    p = l - ((parent->coastline_polygons_Cartesian)[i_adjusted - floor_min_lat][j_adjusted % 360]).size();
//
//                    
//                }
//                
//                
//            }
//            
//        }
        
    }

}


//this function efficiently reads coastline data stored in data_x in the interval of latitudes lambda_min, lambda_max, phi_min, phi_max, and writes this data x and y, writing n_points points at the most. This data is stored into parent->coastline_polygons_now 
void ChartFrame::GetCoastLineData_Mercator(void) {

    int i_min = 0, i_max = 0, j_min = 0, j_max = 0;
    unsigned long long int i, j;
    wxPoint q;

//    //transform the values i_min, i_max in a format appropriate for GetCoastLineData: normalize the minimal and maximal latitudes in such a way that they lie in the interval [-pi, pi], because this is the format which is taken by GetCoastLineData
//    phi_min.normalize_pm_pi();
//    phi_max.normalize_pm_pi();
//
//
    if ((lambda_min < M_PI) && (lambda_max > M_PI)) {

        j_min = floor(K * (lambda_max.value));
        j_max = ceil(K * ((lambda_min.value) + 2.0*M_PI));

    }else {

        if (lambda_min > lambda_max) {

            j_min = floor(K * (lambda_max.value));
            j_max = ceil(K * (lambda_min.value));

        }
        else {

            j_min = floor(K * (lambda_max.value));
            j_max = ceil(K * ((lambda_min.value) + 2.0*M_PI));

        }

    }

    i_min = floor(K * (phi_min.normalize_pm_pi_ret().value));
    i_max = floor(K * (phi_max.normalize_pm_pi_ret().value));


    if ((parent->show_coastlines) == Answer('y', String(""))) {
        
//        PositionProjection p_SW, p_NE, p_SW0, p_NE0;
        unsigned long long int n_added_polygons, l, p, m, every;
        bool new_polygon;
        
//        ( ((phi_max.normalize_pm_pi_ret() - phi_min.normalize_pm_pi_ret()).value)*((lambda_max.normalize_pm_pi_ret() - lambda_min.normalize_pm_pi_ret()).value) ) / ( (ceil_max_lat - floor_min_lat)*2*M_PI );
        
        /*
         the number of points plotted is [number of coastline data points in lambda_min ... phi_max] / every = n_points_plot_coastline_Mercator ->
         every = [number of coastline data points in x_min ... y_max] / n_points_plot_coastline_Mercator ~
         ( (x) * [total number of coastline data points] / n_points_plot_coastline_Mercator
         */
        
//        p_SW.SetMercator(Position(Angle(0.0), phi_min));
//        p_NE.SetMercator(Position(Angle(0.0), phi_max));
//        p_SW0.SetMercator(Position(Angle(0.0), Angle(k*floor_min_lat)));
//        p_NE0.SetMercator(Position(Angle(0.0), Angle(k*ceil_max_lat)));
        
        
        //        double t;
        //        unsigned long long int n_points_in_chart;
        
        //        if((lambda_min < M_PI) && (lambda_max > M_PI)){p_NE.x += 2.0 * M_PI;}
        
        //        t = ( ( (draw_panel->x_span()) * (p_NE.y - p_SW.y) ) / ( (draw_panel->x_span_0) *(p_NE0.y - p_SW0.y) ) );
        
        //        n_points_in_chart = ((unsigned long long int)(((double)(parent->n_all_coastline_points)) * ( ( (draw_panel->x_span()) * (p_NE.y - p_SW.y) ) / ( (draw_panel->x_span_0)  *(p_NE0.y - p_SW0.y) ) )));
        
  
        
        
        //go through coastline_polygons_map and fetch the polygons that fall within rectangle_observer and store their ids into coastline_polygons_area_observer
        for(parent->coastline_polygons_area_observer.clear(), i=i_min-floor_min_lat; i<i_max-floor_min_lat; i++) {
            for(j=j_min; j<j_max; j++) {
                
                for(l=0; l<(parent->coastline_polygons_map)[i][j % 360].size(); l++){
                    parent->coastline_polygons_area_observer.push_back((parent->coastline_polygons_map)[i][j % 360][l]);
                }
                
            }
        }
        //the procedure above may lead to duplicates into coastline_polygons_area_observer -> delete them
        delete_duplicates(&(parent->coastline_polygons_area_observer));
        
        //count the total number of points included in the polygons of coastline_polygons_area_observer and store them in m
        //set every in such a way that the total number of plotted points is n_points_plot_coastline_Mercator, no matter what the size of rectangle_observer
        for(m=0, i=0; i<parent->coastline_polygons_area_observer.size(); i++) {
            for(j=0; j<(parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]].size(); j++){
                
                if((draw_panel->*(draw_panel->ProjectionToDrawPanel))((parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]][j], NULL, false)){
                
//                if(draw_panel->ProjectionToDrawPanel_Mercator((parent->coastline_polygons_Mercator)[(parent->coastline_polygons_area_observer)[i]][j], &q, false)){
                    m++;
                }
                
            }
        }
        every = ((unsigned long long int)(((double)m) / ((double)(wxGetApp().n_points_plot_coastline_Mercator.value))));
        if(every==0){every = 1;}
        
//        every = ((unsigned long long int)(((double)(parent->n_all_coastline_points)) * ( ( (draw_panel->x_span()) * (p_NE.y - p_SW.y) ) / ( (draw_panel->x_span_0)  *(p_NE0.y - p_SW0.y) ) ) / ((double)(wxGetApp().n_points_plot_coastline_Mercator.value))));
//        if(every==0){every = 1;}

        
        for(p=0, i=0, l=0, n_added_polygons=0, polygon_position_now.clear(); i<parent->coastline_polygons_area_observer.size(); i++) {
            //run through polygons
            
            new_polygon=true;
            n_added_polygons++;
            if(n_added_polygons > polygon_position_now.size()){
                polygon_position_now.resize(n_added_polygons);
            }
            polygon_position_now[n_added_polygons-1] = l;
            
            //the id of the polygon that is being added, i.e. , the # of the polygon as entry of coastline_polygons_Position
            m = (parent->coastline_polygons_area_observer)[i];
            
            for(j=p; j<(parent->coastline_polygons_Mercator)[m].size(); j+=every){
                //run through points in a polygon
                
                if((draw_panel->*(draw_panel->ProjectionToDrawPanel))((parent->coastline_polygons_Mercator)[m][j], &q, false)){
                
//                if ((draw_panel->GeoToDrawPanel)((parent->coastline_polygons_Position)[m][j], &q, false)){
                    //(parent->coastline_polygons_Position)[i][j] is a valid point
                    
                    coastline_polygons_now[l++] = q;
                    new_polygon = false;
                    
                }else{
                    //(parent->coastline_polygons_Position)[i][j] is not a valid point -> I start a new polygon
                    
                    if(!new_polygon){
                        
                        //updated polygon_position_now with the position of the new polygon
                        new_polygon = true;
                        n_added_polygons++;
                        if(n_added_polygons > polygon_position_now.size()){
                            polygon_position_now.resize(n_added_polygons);
                        }
                        polygon_position_now[n_added_polygons-1] = l;
                        
                    }

                }
                
            }
            
            p = j - ((parent->coastline_polygons_Position[m]).size());

        }
        
        if(n_added_polygons+1 > polygon_position_now.size()){
            polygon_position_now.resize(n_added_polygons+1);
        }
        polygon_position_now.back() = l;

        
    }
    
}


//this function fetches the data in ((wxGetApp().path_file_coastline_data_blocked).value) and stores them in data_x, data_y, all_coastline_points so that they can be read fastly
void ListFrame::LoadCoastLineData(String prefix) {

    FileR coastline_file;
    Position p_Position;
    Cartesian p_Cartesian;
    PositionProjection p_Mercator;
    string line, temp;
    stringstream ins, message_dialog;
    unsigned long long int i, j;
 
//    string::size_type sz;
    //n_line[k] is the char count to be inserted in seekg to access directly to line k of file output, without going through all the lines in the file
//    vector<unsigned int> n_line;
//    unsigned int l/*, n = 0*/;
//    char* buffer = NULL;
    size_t pos_beg, pos_end;
    double lambda_temp, phi_temp, percentage_dialog;

    if (show_coastlines == Answer('y', String(""))) {
        //in file_init, show coastlines = y

        coastline_file.set_name((wxGetApp().path_coastline_file));
        coastline_file.count_lines(prefix);

        if ((!abort)) {
            
            message_dialog.str("");
            message_dialog << "\nLoading charts ... ";
            
            //a progress dialog to show progress during time-consuming operations
            wxProgressDialog progress_dialog(wxT("Welcome to Thelemacus!"), wxString(message_dialog.str().c_str()), max_dialog, NULL, wxPD_CAN_ABORT | wxPD_AUTO_HIDE | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME | wxPD_APP_MODAL);
            
            coastline_file.open(String(""));
            cout << prefix.value << "Reading file ...\n";
            
            coastline_polygons_Position.clear();
            coastline_polygons_Cartesian.clear();
            coastline_polygons_Mercator.clear();
            
            coastline_polygons_map.resize(ceil_max_lat - floor_min_lat);
            for(i=0; i<coastline_polygons_map.size(); i++){coastline_polygons_map[i].resize(360);}
            
#ifdef _WIN32
            //if I am on WIN32, I set the icon from the icon set in the .rc file
            progress_dialog.SetIcon(wxICON(app_icon));
#endif

            i = 0;
            n_all_coastline_points = 0;
            abort = false;
            
            getline(*(coastline_file.value), line);
            while ((!((coastline_file.value)->eof())) && (!abort)) {
                //run through polygons
                
                coastline_polygons_Position.resize(i + 1);
                coastline_polygons_Cartesian.resize(i + 1);
                coastline_polygons_Mercator.resize(i + 1);

                pos_beg = line.find(":  ", 0)+3;
                do{
                    //run through points of a polygon
                    
                    //read longitude
                    pos_end = line.find(" ", pos_beg);
                    temp = line.substr(pos_beg, pos_end - pos_beg);
                    lambda_temp = stod(temp);
                    
                    //read latitude
                    pos_beg = pos_end+1;
                    pos_end = line.find("  ", pos_beg);
                    temp = line.substr(pos_beg, pos_end - pos_beg);
                    phi_temp = stod(temp);


                    p_Position.lambda.set(String(""), lambda_temp, String(""));
                    p_Position.phi.set(String(""), phi_temp, String(""));
                    p_Cartesian = p_Position.getCartesian();
                    p_Mercator.NormalizeAndSetMercator(p_Position);
                    
                    
                    
                    //push back the position into coastline_polygons_Position
                    coastline_polygons_Position.back().push_back(p_Position);
                    //                  BE CAREFUL ABOUT THIS  BUG THAT MAY ARISE AGAIN IN THE FUTURE
                    //push back the position into coastline_polygons_Cartesian: this is the correct way to push back an element into coastline_polygons_Cartesian: if you use coastline_polygons_Cartesian[i][j].push_back(r_temp), the *memory address of coastline_polygons_Cartesian[i][j].back().r will be set equal to the memory adress of r_temp -> by iterating through the loop, all the entries of coastline_polygons_Cartesian[i][j].r will point to the same adress and thus contain the same value!!
                    //                  BE CAREFUL ABOUT THIS  BUG THAT MAY ARISE AGAIN IN THE FUTURE
                    (coastline_polygons_Cartesian[i]).resize((coastline_polygons_Cartesian[i]).size() + 1);
                    //here I allocate a completely new space for  coastline_polygons_Cartesian[i].back(), so all memory adresses in coastline_polygons_Cartesian[i][0], coastline_polygons_Cartesian[i][1], ... wil be differernt and all the points in there will be different
                    coastline_polygons_Cartesian[i].back() = Cartesian();
                    coastline_polygons_Cartesian[i].back() = p_Cartesian;
                    
                    coastline_polygons_Mercator.back().push_back(p_Mercator);


                    if ((floor(K * (p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat) >=0 ) && (floor(K * (p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat) < coastline_polygons_map.size())) {
                        // polygon #i contains the point p_Position that falls within lat/long K*(p_Position.phi.normalize_pm_pi_ret().value) , floor(K*(p_Position.lambda.value)) -> I add i to coastline_polygons_map[floor(K*(p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat)][floor(K*(p_Position.lambda.value))]
                        coastline_polygons_map[floor(K * (p_Position.phi.normalize_pm_pi_ret().value) - floor_min_lat)][floor(K * (p_Position.lambda.value))].push_back(i);
                        n_all_coastline_points++;

                    }
                    
                    
                    
                    pos_beg = pos_end+2;
                    

                }while(pos_end !=  string::npos);
                
                
                percentage_dialog = 100.0 * ((double)i) / ((double)(coastline_file.number_of_lines));
                message_dialog.str("");
                message_dialog << "\nLoading charts ... " << ((int)percentage_dialog) << "%";
                abort = (!(progress_dialog.Update(percentage_dialog, wxString(message_dialog.str().c_str()))));
                
                getline(*(coastline_file.value), line);
                i++;
                
            }
            
            if ((!abort)) {
                
                progress_dialog.Update(max_dialog);
                cout << prefix.value << "... done.\n";
                
            }

            coastline_file.close(String(""));
            
//            unsigned long long int l;
            //given the way in which I added polygons to coastline_polygons_map, there may be duplicates in oastline_polygons_map[i][j] -> I delete them
            for(i=0/*, l=0*/; i<coastline_polygons_map.size(); i++){
                for(j=0; j<coastline_polygons_map[i].size(); j++){
                    
                    delete_duplicates(&(coastline_polygons_map[i][j]));
                    
//                    sort(coastline_polygons_map[i][j].begin(), coastline_polygons_map[i][j].end());
//                    coastline_polygons_map[i][j].erase(unique( coastline_polygons_map[i][j].begin(), coastline_polygons_map[i][j].end() ), coastline_polygons_map[i][j].end());
//                    
////                    l+= coastline_polygons_map[i][j].size();
                    
                }
                
            }

        }

        //destroy the progress_dialog so if the user aborts the loading process there are no pending frames while closing the application
//        progress_dialog->Destroy();
//        n_line.clear();

    }

}

void ChartFrame::SetIdling(bool b) {

    idling = b;

}

//I call this function when I know that all GUI fields are properly filled in ChartFrame, and thus set the non GUI Angle objects relative to the Euler angles for the rotation of the 3D earth,  and draw everything
void ChartFrame::AllOk(void) {

    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

}


//enable all GUI fields (buttons, slider, etc) in *this if enable  = true, and disable them otherwise
void ChartFrame::EnableAll(bool enable){
    
    button_reset->Enable(enable);
    button_up->Enable(enable);
    button_down->Enable(enable);
    button_left->Enable(enable);
    button_right->Enable(enable);
    slider->Enable(enable);
    projection->Enable(enable);
    
}


DrawPanel::DrawPanel(ChartPanel* parent_in, const wxPoint& position_in, const wxSize& size_in) : wxPanel(parent_in, wxID_ANY, position_in, size_in, wxTAB_TRAVERSAL, wxT("")) {

    String prefix;

    prefix = String("");

    //client_dc = new wxClientDC(this);

    mouse_dragging = false;
    re_draw = true;
    mouse_in_plot_area = false;

    parent = (parent_in->parent);

    SetCursor(*wxCROSS_CURSOR);

    //text field showing the latitude and longitude of the intantaneous (now) mouse position on the chart
    label_position_now = String("");

    circle_observer.omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), prefix);
    thickness_route_selection_over_length_screen.read_from_file_to(String("thickness route selection over length screen"), (wxGetApp().path_file_init), String("R"), prefix);

    rotation.set(Rotation(
                          Angle(String("Euler angle alpha"), -M_PI_2, String("")),
                          Angle(String("Euler angle beta"), 0.0, String("")),
                          Angle(String("Euler angle gamma"), 0.0, String(""))
                          ));

    //specify that circle_observer is a circle of equal altitude
    circle_observer.type = RouteType(((Route_types[2]).value));

    //clears the vector label_phi because tehre are not y labels yet.
    parallels_and_meridians_labels_now.resize(0);
    positions_parallels_and_meridians_labels_now.resize(0);


    idling = false;
    unset_idling = new UnsetIdling<DrawPanel>(this);
    print_error_message = new PrintMessage<DrawPanel, UnsetIdling<DrawPanel> >(this, unset_idling);


    //text for the coordinates of the mouse cursor relative to the corners of the selection rectangle
    (parent->parent->start_label_selection_rectangle) = String("");
    (parent->parent->end_label_selection_rectangle_now) = String("");
#ifdef _WIN32
    (parent->parent->end_label_selection_rectangle_before) = String("");
#endif
    label_dragged_object_now = String("");

    //set the background color of *this to background_color, so there is no need to draw a rectangle filled with background_color every time a paint event is triggered -> the code is faster
    SetBackgroundColour(wxGetApp().background_color);
    //set the border of the chart area 
    SetWindowStyle(wxSIMPLE_BORDER);
    
    this->Bind(wxEVT_PAINT, &DrawPanel::PaintEvent, this);

}


//BEGIN_EVENT_TABLE(DrawPanel, wxPanel)
//
//// catch paint events
//EVT_PAINT(DrawPanel::PaintEvent)
//
//END_EVENT_TABLE()


inline void DrawPanel::PaintEvent([[maybe_unused]] wxPaintEvent& event) {

    wxPaintDC dc(this);

    RenderAll(dc);

}


//render the mouse position with colors foreground_color and background_color
inline void DrawPanel::RenderMousePositionLabel(
                                                wxDC& dc,
                                                const String& label_position,
                                                const wxPoint& position_label_position,
                                                wxColor foreground_color,
                                                wxColor background_color
                                                ) {

    //wipe out position_label_position by writing on top of it a rectangle filled with color backgound_color
    dc.SetPen(background_color);
    dc.SetBrush(wxBrush(background_color, wxBRUSHSTYLE_SOLID));
    dc.DrawRectangle(position_label_position, get_size(label_position, &dc));

    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawText(wxString(label_position.value), position_label_position);


}


//call either Refresh() or RefreshWIN32 according to the operating system
void DrawPanel::MyRefresh(){
    
#ifdef __APPLE__
    //I am on apple operating system-> I use the wxWidgets Refresh() method, which is fast

    
    Refresh();

#endif

#ifdef _WIN32
//I am on windows operating system-> I call RefreshWIN32() because the wxWidgets Refresh() is slow
    
    RefreshWIN32();

#endif

}

//render the coastline by using the set of points points_coastline, meridians, parallels and their labels
inline void DrawPanel::RenderBackground(
                                        wxDC& dc,
                                        const wxPoint& position_plot_area,
                                        const vector< vector< vector<wxPoint> > >& grid,
                                        const vector< vector< vector<wxPoint> > >& ticks,
                                        const vector<wxString>& parallels_and_meridians_labels,
                                        const vector<wxPoint>& positions_parallels_and_meridians_labels,
                                        const vector<unsigned long long int>& polygon_positions,
                                        const vector<wxPoint>& points_coastline,
                                        wxColour foreground_color,
                                        wxColour background_color,
                                        double thickness
) {

    if (re_draw) {

        m_bgbuffer.Create(size_chart, 32);
        m_bgbuffer.UseAlpha();

        wxMemoryDC mdc(m_bgbuffer);

        //        mdc.SetPen(wxPen(foreground_color));
        //        mdc.SetBrush(wxBrush(foreground_color));

        wxGCDC dc_m_bgbuffer(mdc);
        //this needs to be commented out in order to not show a 'trail' when dragging
        //dc_m_bgbuffer.SetBackground(*wxTRANSPARENT_BRUSH);
        //        dc_m_bgbuffer.Clear();

        dc_m_bgbuffer.SetPen(wxPen(foreground_color));
        dc_m_bgbuffer.SetBrush(wxBrush(foreground_color));
        dc_m_bgbuffer.SetTextForeground(foreground_color);
        dc_m_bgbuffer.SetTextBackground(background_color);
        
        (this->*Render)(
                        &dc_m_bgbuffer,
                        position_plot_area,
                        grid,
                        ticks,
                        parallels_and_meridians_labels,
                        positions_parallels_and_meridians_labels,
                        polygon_positions,
                        points_coastline,
                        foreground_color,
                        background_color,
                        thickness
                        );
        
        mdc.SelectObject(wxNullBitmap);

        re_draw = false;
    }

    dc.SetPen(wxPen(foreground_color));
    dc.SetBrush(wxBrush(background_color));
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawBitmap(m_bgbuffer, 0, 0, false);

}


//same as  DrawPanel::RenderSelectionRectangle(wxDC& dc, Position geo_position, wxColour foreground_color, wxColour background_color), but it takes a  position (reckoned with respect to the ordigin of *this) as input rather than a  geographic Position
inline void DrawPanel::RenderSelectionRectangle(wxDC& dc,
                                                const wxPoint& position,
                                                const wxPoint& position_end_label,
                                                const String& end_label,
                                                wxColour foreground_color,
                                                wxColour background_color) {

    Position p;

    if(DrawPanelToGeo(position, &p)){
        RenderSelectionRectangle(dc, p, position_end_label, end_label, foreground_color, background_color);
    }

}


//render a selection rectangle with end Position geo_position (geographic position), foreground color foreground_color and backgrund color background_color, and label at its endpoint end_label located at position_end_label
inline void DrawPanel::RenderSelectionRectangle(wxDC& dc, 
                                                const Position& geo_position,
                                                const wxPoint& position_end_label,
                                                const String& end_label,
                                                wxColour foreground_color,
                                                wxColour background_color) {
    
    Angle lambda_a, lambda_b, lambda_ab_span, Z;


    dc.SetPen(foreground_color);
    dc.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);


    //I draw the four edges of the rectangle in a way that is independent of the projection used
    //right vertical edge of rectangle
    (Route(
        RouteType(((Route_types[1]).value)),
        (parent->parent->geo_position_start),
        Angle(M_PI * (1.0 - GSL_SIGN((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value))) / 2.0),
        Length(Re * fabs((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value)))
    )).Draw(((wxGetApp().n_points_routes).value), &dc, this, String(""));

    //left vertical edge of rectangle
    (Route(
           RouteType(((Route_types[1]).value)),
        geo_position,
        Angle(M_PI * (1.0 + GSL_SIGN((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value))) / 2.0),
        Length(Re * fabs((normalize_pm_pi_ret(geo_position.phi).value) - (parent->parent->geo_position_start.phi.normalize_pm_pi_ret().value)))
    )).Draw(((wxGetApp().n_points_routes).value), &dc, this, String(""));

    
    //top and bottom horizontal edge of rectangle
    lambda_a = (parent->parent->geo_position_start.lambda);
    lambda_b = geo_position.lambda;
    lambda_a.normalize();
    lambda_b.normalize();
    
    switch (position_in_vector(Projection((parent->projection->name->GetValue()).ToStdString()), Projection_types)) {
            
        case 0: {
            //I am using Projection_types[0]
            

            //the following cases follow the derivation in the note 'Selection rectangle'
            if((parent->lambda_min) < (parent->lambda_max)){
                
                if(
                   (((lambda_a > 0.0) && (lambda_a < (parent->lambda_min))) && ((lambda_b > 0.0) && (lambda_b < (parent->lambda_min)))) ||
                   ((lambda_a > (parent->lambda_max)) && (lambda_b > (parent->lambda_max)))
                   ){
                       //{lambda_a in A & lambda_b in A} or {lambda_a in B and lambda_b in B}
                       
                       lambda_ab_span.set(fabs((lambda_b.value) - (lambda_a.value)));
                       Z = Angle(-GSL_SIGN((lambda_b.value) - (lambda_a.value)) * M_PI_2);
                       
                   }else{
                       //{lambda_a in A & lambda_b in B} or {lambda_a in B and lambda_b in A}
                       
                       lambda_ab_span.set(2.0*M_PI - fabs((lambda_b.value) - (lambda_a.value)));
                       Z = Angle(GSL_SIGN((lambda_b.value) - (lambda_a.value)) * M_PI_2);
                       
                   }
                
            }else{
                
                lambda_ab_span.set(fabs((lambda_b.value) - (lambda_a.value)));
                Z = Angle(-GSL_SIGN((lambda_b.value) - (lambda_a.value)) * M_PI_2);
                
            }
            
           
            //now that lambda_span and Z have been set, I draw the Routes corresponding to the top and bottom horizontal edges
            Route(
                  RouteType(((Route_types[0]).value)),
                  geo_position,
                  Z+M_PI,
                  Length(Re * cos(geo_position.phi) * (lambda_ab_span.value))
                  ).DrawOld((wxGetApp().n_points_routes.value), &dc, this, String(""));
            Route(
                  RouteType(((Route_types[0]).value)),
                  parent->parent->geo_position_start,
                  Z,
                  Length(Re * cos(parent->parent->geo_position_start.phi) * (lambda_ab_span.value))
                  ).DrawOld((wxGetApp().n_points_routes.value), &dc, this, String(""));
            
            break;
        }
            
        case 1: {
            //I am using Projection_types[1]
            
            Angle temp, lambda_span_temp, Z_temp;
            
            temp.value = (normalize_pm_pi_ret(geo_position.lambda).value) - (parent->parent->geo_position_start.lambda.normalize_pm_pi_ret().value);
            
            if(fabs(temp.value) < M_PI){
                lambda_span_temp.set(fabs(temp.value));
                Z_temp = Angle(M_PI_2 + M_PI * (1.0 + GSL_SIGN(temp.value)) / 2.0);
            }else{
                lambda_span_temp.set(2.0*M_PI - fabs(temp.value));
                Z_temp = Angle(-(M_PI_2 + M_PI * (1.0 + GSL_SIGN(temp.value)) / 2.0));
            }


            
            //bottom horizontal edge of rectangle
            (Route(
                   RouteType(((Route_types[0]).value)),
                   (parent->parent->geo_position_start),
                   Z_temp,
                   Length(Re * cos(parent->parent->geo_position_start.phi) * (lambda_span_temp.value))
                   )
             ).DrawOld(wxGetApp().n_points_routes.value, &dc, this, String(""));
            
            //            //top horizontal edge of rectangle
            (Route(
                   RouteType(((Route_types[0]).value)),
                geo_position,
                Z_temp+M_PI,
                Length(Re * cos(geo_position.phi) * (lambda_span_temp.value))
            )).DrawOld(wxGetApp().n_points_routes.value, &dc, this, String(""));

            
            break;
        }
            
            
    }
    
    //render the labels of the selection rectangle
    //wipe out the space occupied by the label
    dc.SetPen(wxPen(background_color));
    dc.SetBrush(wxBrush(background_color, wxBRUSHSTYLE_SOLID));
    dc.DrawRectangle(position_end_label, get_size(end_label, &dc));

    //render the rectangle
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawText(wxString(end_label.value), position_end_label);
    dc.DrawText(wxString(parent->parent->start_label_selection_rectangle.value), position_start_label_selection_rectangle);


}


////draw the label of the start and end point of selection_rectangle with foreground and background colrs foreground_color and background_color, respectively
//void DrawPanel::RenderSelectionRectangleLabels(wxDC& dc) {
//
//
//}


inline void DrawPanel::RenderAll(wxDC& dc) {
    
    
    
    (this->*Render)(
                    &dc,
                    position_plot_area_now,
                    grid_now,
                    ticks_now,
                    parallels_and_meridians_labels_now,
                    positions_parallels_and_meridians_labels_now,
                    parent->polygon_position_now,
                    parent->coastline_polygons_now,
                    wxGetApp().foreground_color,
                    wxGetApp().background_color,
                    wxGetApp().standard_thickness.value
                    );
    
    
    RenderRoutes(dc,
                 points_route_list_now,
                 reference_positions_route_list_now,
                 (parent->parent->highlighted_route_now),
                 wxNullColour
                 );
    RenderPositions(dc,
                    points_position_list_now,
                    (parent->parent->highlighted_position_now),
                    wxNullColour
                    );
    RenderMousePositionLabel(
                             dc,
                             label_position_now,
                             position_label_position_now,
                             wxGetApp().foreground_color,
                             wxGetApp().background_color
                             );
    
    //render selection_rectangle and its labels
    if ((parent->parent->selection_rectangle)) {
        RenderSelectionRectangle(dc, parent->parent->geo_position_now, position_end_label_selection_rectangle_now, parent->parent->end_label_selection_rectangle_now, wxGetApp().foreground_color, wxGetApp().background_color);
    }
    
    if ((parent->parent->dragging_object)) {
        //I am draggingn a Route or Position -> show the coordinates of the Position or of the Route's reference_position
        RenderDraggedObjectLabel(dc, position_label_dragged_object_now, label_dragged_object_now, wxGetApp().foreground_color, wxGetApp().background_color);
    }
    
}



//render the Routes whose point coordinates with respect to the origin of DrawPanel are stored in points_curves, and whose reference-position coordinates with respect to the origin of DrawPanel are stored in reference_positions. the Route #highlighted_route is rendered with larger thickness. If foreground_color != wxNUllColour, the Routes are rendered with the colors in color_list, otherwise they are rendered with foreground_color
inline void DrawPanel::RenderRoutes(
                                    wxDC& dc,
                                    const vector< vector< vector<wxPoint> > >& points_curves,
                                    const vector<wxPoint>& reference_positions,
                                    int highlighted_route,
                                    wxColor foreground_color
                                    ) {

    int i, j, color_id;
    double thickness, radius;
    wxPoint p;

    //render Routes
    for (i = 0, color_id = 0; i < (points_curves.size()); i++) {

        //set the route thickness and pen
        if (i == highlighted_route) {
            thickness = max((int)((((wxGetApp().large_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1);
            radius = thickness;
        }
        else {
            thickness = (wxGetApp().standard_thickness.value);
            radius = 4 * thickness;
        }

        if (foreground_color != wxNullColour) {
            dc.SetPen(wxPen(foreground_color, thickness));
            //dc.SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_SOLID));
        }
        else {
            dc.SetPen(wxPen((wxGetApp().color_list)[(color_id++) % ((wxGetApp().color_list).size())], thickness));
        }

        //draw  reference_position[i] only if it is included in the plot area
        if (DrawPanelToGeo(reference_positions[i], NULL)) {
            dc.DrawCircle(reference_positions[i], radius);
        }

        //draw the route points
        //run over all connected chunks of routes
        for (j = 0; j < (points_curves[i]).size(); j++) {

            if ((points_curves[i][j]).size() > 1) {
                //I need to add this consdition to make sure that I am not drawing an empty connected chunk

                
                dc.DrawSpline((int)((points_curves[i][j]).size()), (points_curves[i][j]).data());
            }
            
            //render points of Routes for debug
            /*
             for(unsigned int l=0; l<points_curves[i][j].size(); l++){
             dc.DrawCircle(points_curves[i][j][l], thickness);
             }
             */
            //render points of Routes for debug
            

        }

    }

}

//clear everything and re-render all GUI objects
inline void DrawPanel::CleanAndRenderAll(void) {
    
    wxClientDC dc(this);
    
    
    dc.Clear();
    
    RenderMousePositionLabel(
                             dc,
                             label_position_now,
                             position_label_position_now,
                             wxGetApp().foreground_color,
                             wxGetApp().background_color
                             );
    
    (this->*Render)(
                    &dc,
                    position_plot_area_now,
                    grid_now,
                    ticks_now,
                    parallels_and_meridians_labels_now,
                    positions_parallels_and_meridians_labels_now,
                    parent->polygon_position_now,
                    parent->coastline_polygons_now,
                    wxGetApp().foreground_color,
                    wxGetApp().background_color,
                    wxGetApp().standard_thickness.value
                    );
    
    RenderRoutes(dc,
                 points_route_list_now,
                 reference_positions_route_list_now,
                 (parent->parent->highlighted_route_now), wxNullColour
                 );
    
    RenderPositions(dc,
                    points_position_list_now,
                    (parent->parent->highlighted_position_now),
                    wxNullColour
                    );
    
    RenderDraggedObjectLabel(dc,
                             position_label_dragged_object_now,
                             label_dragged_object_now,
                             wxGetApp().foreground_color,
                             wxGetApp().background_color
                             );
    
    
}

//wipe out all Routes on *this and re-draw them: this method is used to replace on WIN32 the wxWidgets default function Refresh(), which is not efficient on WIN32
inline void DrawPanel::RefreshWIN32(void) {

    wxClientDC dc(this);

    //1. erase _before objects

    if ((parent->parent->mouse_moving)) {
        //the mouse is moving -> wipe out the  mouse position label at the preceeding step of mouse movement

        RenderMousePositionLabel(
            dc,
            label_position_before,
            position_label_position_now,
            wxGetApp().background_color,
            wxGetApp().background_color
        );

    }

    if ((parent->dragging_chart) || (parent->mouse_scrolling)) {
        //the whole chart is being dragged or scrolled -> wipe out all objects at the preceeding step of the drag

        //wipe out the Routes at the preceeding mouse position
        RenderRoutes(dc,
            points_route_list_before,
            reference_positions_route_list_before,
            (parent->parent->highlighted_route_now),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_before,
            (parent->parent->highlighted_position_now),
                        wxGetApp().background_color
                        );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_before,
                        grid_before,
                        ticks_before,
                        parallels_and_meridians_labels_before,
                        positions_parallels_and_meridians_labels_before,
                        parent->polygon_position_before,
                        parent->coastline_polygons_before,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
    }

    if ((parent->parent->dragging_object)) {

        //wipe out the Routes, Positions and label of dragged object at the preceeding dragging configuration
        RenderRoutes(dc,
            points_route_list_before,
            reference_positions_route_list_before,
            (parent->parent->highlighted_route_now),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_before,
            (parent->parent->highlighted_position_now),
                        wxGetApp().background_color
                        );
        RenderDraggedObjectLabel(dc,
                                 position_label_dragged_object_before,
                                 label_dragged_object_before,
                                 wxGetApp().background_color, wxGetApp().background_color
                                 );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
        
    }

    if ((parent->parent->changing_highlighted_object)) {

        //wipe out the Routes at the preceeding mouse position
        RenderRoutes(dc,
            points_route_list_now,
            reference_positions_route_list_now,
            (parent->parent->highlighted_route_before),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_now,
            (parent->parent->highlighted_position_before),
            wxGetApp().background_color
        );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
        
    }
    
    if ((parent->parent->selection_rectangle)) {

        //wipe out the preceeding selection rectangle
        RenderSelectionRectangle(dc,
            (parent->parent->geo_position_before),
            position_end_label_selection_rectangle_before,
            parent->parent->end_label_selection_rectangle_before,
            wxGetApp().background_color,
            wxGetApp().background_color
        );

        //wipe out the Routes at the preceeding mouse position
        RenderRoutes(dc,
            points_route_list_now,
            reference_positions_route_list_now,
            (parent->parent->highlighted_route_now),
            wxGetApp().background_color
        );
        RenderPositions(dc,
            points_position_list_now,
            (parent->parent->highlighted_position_now),
            wxGetApp().background_color
                        );
        
        //wipe out the background without painting a wxBitmap: to do this, I use the large thickness to make sure that the new background drawn with color background_color is wide enough to completely covert the preceeding one
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().background_color,
                        wxGetApp().background_color,
                        wxGetApp().large_thickness.value
                        );
        
        
    }


    //re-render  _new objects

    RenderMousePositionLabel(
        dc,
        label_position_now,
        position_label_position_now,
        wxGetApp().foreground_color,
        wxGetApp().background_color
    );

    if ((parent->dragging_chart) || (parent->mouse_scrolling) || (parent->parent->selection_rectangle) || (parent->parent->dragging_object) || (parent->parent->changing_highlighted_object)) {
        //I am either drawing a selection rectangle, dragging an object or changing the highlighted object -> I need to re-render all GUI objects
        
        //re-render all  objects in *this which may have been partially cancelled by the clean operation above
        (this->*Render)(
                        &dc,
                        position_plot_area_now,
                        grid_now,
                        ticks_now,
                        parallels_and_meridians_labels_now,
                        positions_parallels_and_meridians_labels_now,
                        parent->polygon_position_now,
                        parent->coastline_polygons_now,
                        wxGetApp().foreground_color,
                        wxGetApp().background_color,
                        wxGetApp().standard_thickness.value
                        );
        RenderRoutes(dc,
                     points_route_list_now,
                     reference_positions_route_list_now,
                     (parent->parent->highlighted_route_now), wxNullColour
        );
        RenderPositions(dc,
            points_position_list_now,
            (parent->parent->highlighted_position_now),
            wxNullColour
        );
        RenderDraggedObjectLabel(dc,
            position_label_dragged_object_now,
            label_dragged_object_now,
            wxGetApp().foreground_color,
            wxGetApp().background_color
        );



    }

    if ((parent->parent->selection_rectangle)) {

        //re-draw the current selection rectangle
        RenderSelectionRectangle(dc,
            parent->parent->geo_position_now,
            position_end_label_selection_rectangle_now,
            parent->parent->end_label_selection_rectangle_now,
            wxGetApp().foreground_color,
            wxGetApp().background_color
        );


    }

}



//render the Positions:  if foreground_color == wxNullColour, this method uses as foreground color the colors in color_list, otherwise it uses foreground_color
inline void DrawPanel::RenderPositions(wxDC& dc, 
                                       const vector<wxPoint>& points,
                                       int highlighted_position,
                                       wxColor foreground_color) {

    int i, color_id;
    double thickness, radius;
    wxPoint p;


    //draw Positions
    for (i = 0, color_id = 0; i < (points.size()); i++) {

        //set thickness and pen
        if (i == highlighted_position) {
            thickness = max((int)((((wxGetApp().large_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1);
            radius = thickness;
        }
        else {
            thickness = (wxGetApp().standard_thickness.value);
            radius = 4 * thickness;
        }

        if (foreground_color != wxNullColour) {
            dc.SetPen(wxPen(foreground_color, thickness));
        }
        else {
            dc.SetPen(wxPen((wxGetApp().color_list)[(color_id++) % ((wxGetApp().color_list).size())], thickness));
        }

        if (DrawPanelToGeo(points[i], NULL)) {
            //the point returned from GeoToDrawPanel falls within the plot area -> plot it
            dc.DrawCircle(points[i], radius);
        }

    }

}


//render the coordinates of an object (Route or Position) which is being dragged by rendering the label label_dragged_object at position position_label_dragged_object (reckoned with respect to the origin of *this)
inline void DrawPanel::RenderDraggedObjectLabel(wxDC& dc, 
                                                const wxPoint& position_label_dragged_object,
                                                const String& label_dragged_object,
                                                wxColor foreground_color,
                                                wxColor background_color) {


    //wipe out the space occupied by the label
    dc.SetPen(wxPen(background_color));
    dc.SetBrush(wxBrush(background_color, wxBRUSHSTYLE_SOLID));
    dc.DrawRectangle(position_label_dragged_object, get_size(label_dragged_object, &dc));


    //render label_dragged_object
    dc.SetTextForeground(foreground_color);
    dc.SetTextBackground(background_color);
    dc.DrawText(wxString(label_dragged_object.value), position_label_dragged_object);

}





//fit the size of the chart, of parent, of parent->panel to the content
void DrawPanel::FitAll() {

    //set the size of the DrawPanel and of the ChartFrame which is its parent and fit the size of ChartFrame parent in such a way that it just fits its content
    this->SetMinSize(size_chart);
    parent->SetMinSize(ToDIP(wxSize(
        (size_chart.GetWidth()) + ((parent->slider)->GetSize().GetWidth()) + 4 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
        (size_chart.GetHeight()) + ((label_position_now.get_size(this)).GetHeight()) + 6 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)
    )));

    //position position_label_position_now at the bottom left corner of *this
    position_label_position_now = wxPoint(
        (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
        (size_chart.GetHeight())
        - (size_label_vertical + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))
    );

    (parent->panel)->SetSizerAndFit(parent->sizer_v);
    (parent->panel)->Fit();
    parent->Fit();

}


//render the polygons stored in points_polygons and polygon_positions
inline void DrawPanel::RenderPolygons(wxDC* dc,
                                      const vector<unsigned long long int>& polygon_positions,
                                      const vector<wxPoint>& points_polygons,
                                      wxColor foreground_color,
                                      wxColor background_color,
                                      double thickness) {
    
    long long int i;
    
    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_SOLID));
    for(i = 0; i < ((long long int)(polygon_positions.size()))-1; i++) {
        //run through polygons
        
        if(polygon_positions[i+1] - polygon_positions[i] > 1){
            
            dc->DrawLines((int)(polygon_positions[i+1] - polygon_positions[i]), (points_polygons.data()) + polygon_positions[i]);
            
        }
        
    }
    
}


//remember that any Draw command in this function takes as coordinates the coordinates relative to the position of the DrawPanel object!
inline void DrawPanel::Render_Mercator(wxDC* dc,
                                       const wxPoint& position_plot_area,
                                       const vector< vector< vector<wxPoint> > >& grid,
                                       const vector< vector< vector<wxPoint> > >& ticks,
                                       const vector<wxString>& parallels_and_meridians_labels,
                                       const vector<wxPoint>& positions_parallels_and_meridians_labels,
                                       const vector<unsigned long long int>& polygon_positions,
                                       const vector<wxPoint>& points_polygons,
                                       wxColor foreground_color,
                                       wxColor background_color,
                                       double thickness) {

    Angle lambda, phi;
    Route route;
    wxPoint p;
    PositionProjection temp;
    Position q;
    //this = true if, while drawing the x or y axis labels, the label that I one is about to draw is the first one
    int i, j;

    //draw a rectangle (representing the border) whose border and fill are with color wxGetApp().background_color on bitmap_image, so it will have the right background color
    dc->SetBrush(wxBrush(background_color, wxBRUSHSTYLE_TRANSPARENT));
    dc->SetPen(wxPen(foreground_color, thickness));
    //dc->DrawRectangle(0, 0, (size_chart.GetWidth()), (size_chart.GetHeight()));
    dc->DrawRectangle(position_plot_area.x, position_plot_area.y, (size_plot_area.GetWidth()), (size_plot_area.GetHeight()));

    //render coastlines
    RenderPolygons(dc, polygon_positions, points_polygons, foreground_color, background_color, thickness);


    //set thickness to normal thicnkness
//    thickness = max((int)((((wxGetApp().standard_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth()), 1);

    //render parallels and meridians
    for (i = 0; i < grid.size(); i++) {
        for (j = 0; j < (grid[i]).size(); j++) {

            //            (grid[i]).DrawOld((parent->parent->data->n_points_routes.value), foreground_color, thickness, dc, this);

            if ((grid[i][j]).size() > 1) {
                dc->DrawLines((int)((grid[i][j]).size()), (grid[i][j]).data());
            }

        }
    }
    //render parallels and meridian ticks
    for (i = 0; i < ticks.size(); i++) {
        for (j = 0; j < (ticks[i]).size(); j++) {

            //            (ticks[i]).DrawOld((wxGetApp().n_points_minor_ticks.value), foreground_color, thickness, dc, this);
            if ((ticks[i][j]).size() > 1) {
                dc->DrawLines((int)((ticks[i][j]).size()), (ticks[i][j]).data());
            }
        }
    }



    //render labels on parallels and meridians
    //WIN32: THESE LINES YIELD PARALLEL AND MERIDIAN LABELS WITH THE CORRECT SIZE
    dc->SetTextForeground(foreground_color);
    dc->SetTextBackground(background_color);
    dc->SetBrush(wxBrush(wxNullBrush)); //Set the brush to the device context
    dc->SetBackgroundMode(wxSOLID);
    for (i = 0; i < parallels_and_meridians_labels.size(); i++) {

        dc->DrawText(parallels_and_meridians_labels[i], positions_parallels_and_meridians_labels[i] /*+ wxPoint(-width_label - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value), -height_label / 2)*/);

    }
    //WIN32: THESE LINES YIELD PARALLEL AND MERIDIAN LABELS WITH THE CORRECT SIZE


}


//This function writes into *output the text label for a parallel or a meridian. The latitude/longitude in the text label is q.phi/q.lambda, min and max are the minimal and maximal latitudes/longitudes that are covered in the drawing process of the label by DrawPanel::SetLabel, they must be sorted in such a way that (max.normalize_pm_pi_ret()).value > (min.normalize_pm_pi_ret()).value. mode = "NS" or "EW" specifices whether the label to be plotted is a latitude or a longitude label, respectively. The output is written int *output
void DrawPanel::WriteLabel(const Position& q, Angle min, Angle max, Int precision, String mode, wxString* output) {

    double delta;
    //a pointer to the angle which will be used to draw the label
    Angle angle_label;
    stringstream s;
    wxString wx_string;

    s.str("");

    if (mode == String("NS")) {
        //if I am drawing latitude labels, I set the angle relative to the label to q.phi, and delta to delta_phi, and I let labels point to label_phi

        angle_label = (q.phi);
        delta = delta_phi;

    }
    else {
        //if I am drawing longitude labels, I set the angle relative to the label to q.lambda, and delta to delta_lambda, and I let labels point to label_lambda

        angle_label = (q.lambda);
        delta = delta_lambda;

    }


    if (/*If this condition is true, then angle_label.value*K is an integer multiple of one degree*/fabs(K * (angle_label.value) - round(K * (angle_label.value))) < epsilon_double) {
        //in this case, (angle_label.value) (or, in other words, the latitude phi) = n degrees, with n integer: I write on the axis the value of phi  in degrees

        s << angle_label.deg_to_string(mode, (precision.value));

    }
    else {

        //in this case, delta  is not an integer multiple of a degree. However, (angle_label.value) may still be or not be a multiple integer of a degree
        if (k * fabs(K * (angle_label.value) - ((double)round(K * (angle_label.value)))) < delta / 2.0) {
            //in this case, (angle_label.value) coincides with an integer mulitple of a degree: I print out its arcdegree part only

            s << angle_label.deg_to_string(mode, (precision.value));

        }
        else {
            //in this case, (angle_label.value) deos not coincide with an integer mulitple of a degree: I print out its arcminute part only

            //                if(ceil((K*((parent->phi_max).value)))  - floor((K*((parent->phi_min).value))) != 1){
            if (ceil((K * ((max.normalize_pm_pi_ret()).value))) - floor((K * ((min.normalize_pm_pi_ret()).value))) != 1) {
                //in this case, the phi interval which is plotted spans more than a degree: there will already be at least one tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I print out its arcminute part only.

                s << angle_label.min_to_string(mode, (precision.value));

            }
            else {
                //in this case, the phi interval which is plotted spans less than a degree: there will be no tic in the plot which indicates the arcdegrees to which the arcminutes belong -> I add this tic by printing, at the first tic, both the arcdegrees and arcminutes.

                if (first_label) {

                    s << angle_label.to_string(mode, (precision.value), false);

                }
                else {

                    s << angle_label.min_to_string(mode, (precision.value));

                }

            }


        }

    }

    (*output) = wxString(s.str().c_str());


}

//This function draws into *this the text label for a parallel or a meridian, by placing it near the Position q. The label is pushed back to this->labels and its position is pushed back to this->positions_labels (this position is adjusted with respect to q in such a way that the label look nice and centered). The latitude/longitude in the text label is q.phi/q.lambda. min and max are the minimal and maximal latitudes/longitudes that are covered in the drawing process, they must be sorted in such a way that (max.normalize_pm_pi_ret()).value > (min.normalize_pm_pi_ret()).value. mode = "NS" or "EW" specifices whether the label to be plotted is a parallel or a meridian label, respectively.
void DrawPanel::DrawLabel(const Position& q, Angle min, Angle max, Int precision, String mode) {

    wxPoint p;
    wxSize size;

    if (/* convert temp to draw_panel coordinates p*/GeoToDrawPanel(q, &p, false)) {
        //if Position q lies on the visible side of the Earth, I proceed and draw its label

        wxString wx_string;

        //write the label into wx_string
        WriteLabel(q, min, max, precision, mode, &wx_string);


        parallels_and_meridians_labels_now.push_back(wx_string);
        positions_parallels_and_meridians_labels_now.push_back(p);


        size = String(parallels_and_meridians_labels_now.back().ToStdString()).get_size(this);

        if (mode == String("NS")) {
            //            I am drawing parallels label

            (positions_parallels_and_meridians_labels_now.back()) += wxPoint(-(size.GetWidth()) - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value), -(size.GetHeight()) / 2);

        }
        else {
            //            I am drawing meridians labels

            (positions_parallels_and_meridians_labels_now.back()) += wxPoint(-(size.GetWidth()) / 2, (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value));

        }

        first_label = false;

    }

}

//This function renders the chart in the 3D case. remember that any Draw command in this function takes as coordinates the coordinates relative to the position of the DrawPanel object!
inline void DrawPanel::Render_3D(
                                 wxDC* dc,
                                 const wxPoint& position_plot_area,
                                 const vector< vector< vector<wxPoint> > >& grid,
                                 const vector< vector< vector<wxPoint> > >& ticks,
                                 const vector<wxString>& parallels_and_meridians_labels,
                                 const vector<wxPoint>& positions_parallels_and_meridians_labels,
                                 const vector<unsigned long long int> & polygon_positions,
                                 const vector<wxPoint>& points_polygons,
                                 wxColor foreground_color,
                                 wxColor background_color,
                                 double thickness
                                 ) {
    
    int i, j;
    Double d_temp;
    Angle lambda;
    stringstream s;
    wxString wx_string;
    //this is a list of tabulated points for dummy_route, such as a meridian, which will be created and destroyed just after
    vector<wxPoint> points_dummy_route;
    PositionProjection dummy_projection;
    wxPoint p;
    Position q, temp;


    //draws a rectangle filled with color wxGetApp().background_color and with border wich color wxGetApp().foregrond_color on bitmap_image, so bitmap_image will have the right background color
    //dc->SetBrush(wxBrush(wxGetApp().background_color));
    //dc->SetPen(wxPen(foreground_color, thickness));
    //dc->DrawRectangle(0, 0, (size_chart.GetWidth()), (size_chart.GetHeight()));

    //render coastlines
    if((parent->parent->show_coastlines) == Answer('y', String(""))){
        RenderPolygons(dc, polygon_positions, points_polygons, foreground_color, background_color, thickness);
    }

    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_TRANSPARENT)); //Set the brush to the device context
    //render parallels and meridians
    for (i = 0; i < grid.size(); i++) {
        for (j = 0; j < (grid[i]).size(); j++) {

            //        (grid[i]).Draw((parent->parent->data->n_points_routes.value), foreground_color, background_color, thickness, dc, this, String(""));
            if ((grid[i][j]).size() > 1) {
                dc->DrawSpline((int)((grid[i][j]).size()), (grid[i][j]).data());
            }
        }
    }
    //render parallel and meridian ticks
    for (i = 0; i < ticks.size(); i++) {
        for (j = 0; j < (ticks[i]).size(); j++) {

            //        (ticks[i]).Draw((wxGetApp().n_points_minor_ticks.value), foreground_color, background_color, thickness, dc, this, String(""));
            if ((ticks[i][j]).size() > 1) {
                dc->DrawSpline((int)((ticks[i][j]).size()), (ticks[i][j]).data());
            }
        }
    }


    //render labels on parallels and meridians
    dc->SetTextForeground(foreground_color);
    dc->SetTextBackground(background_color);
    dc->SetBrush(wxBrush(wxNullBrush)); //Set the brush to the device context
    dc->SetBackgroundMode(wxSOLID);
    for (i = 0; i < parallels_and_meridians_labels.size(); i++) {

        dc->DrawText(parallels_and_meridians_labels[i], positions_parallels_and_meridians_labels[i]/* + wxPoint(-width_label - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value), -height_label / 2)*/);

    }



    //draw horizon circle
    //draw the circle repreentig the edge of the earth by creating a circle of equal altitude centered at GP_observer and with aperture omega_observer
    //set q to a point on the prime meridian and latitude equal to the maximal latitude of circle_observer, and convert it to 3D projection temp: the resulting temp.y is the radius of the circular horizon of the earth in 3d projection cooordinates
    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(foreground_color, wxBRUSHSTYLE_TRANSPARENT)); //Set the brush to the device context

    //set q
    (q.lambda).set(String(""), 0.0, String(""));
    (q.phi) = (circle_observer.omega);

    //obtain the coordinates of q in the reference frame x'y'z'
    gsl_vector_set((rp.r), 0, 0.0);
    gsl_vector_set((rp.r), 1, -cos(q.phi));
    gsl_vector_set((rp.r), 2, sin((q.phi)));

    //project rp into the 3D projection and obtain temp: temp.y is the radius of the horizon circle
    d_temp.set(String(""), -1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))), String(""));
    dummy_projection = PositionProjection(0.0, ((d_temp.value) * gsl_vector_get((rp.r), 2)) / ((d_temp.value) + 1.0 + gsl_vector_get((rp.r), 1)));
    //set the wxPen color for the horizon
//    dc->SetPen(wxPen(wxGetApp().color_horizon, 1));
    dc->SetPen(wxPen(foreground_color, thickness));
    dc->SetBrush(wxBrush(background_color, wxBRUSHSTYLE_TRANSPARENT));
    dc->SetBackground(background_color);
    //convert r.y to DrawPanel coordinates and trace a circle with the resulting radius
    dc->DrawCircle(
        (position_plot_area.x) + (int)(((double)(size_plot_area.GetWidth())) / 2.0),
        (position_plot_area.y) + (int)(((double)(size_plot_area.GetHeight())) / 2.0),
        (dummy_projection.y) / y_max * ((double)(size_plot_area.GetWidth())) / 2.0
    );

}




//this function tabulates into points_route_list_now and reference_positions_route_list_now the points and reference Positions, respectively, of all Routes. points_route_list will then be used to render the Routes
inline void DrawPanel::TabulateRoutes(void) {

    unsigned int i;
    wxPoint p;

    //resize points_route_list_now and reference_position_route_list_now, which needs to have the same size as (data->route_list), and clear up points_route_list
    points_route_list_now.resize((parent->parent->data->route_list).size());
    for (i = 0; i < (points_route_list_now.size()); i++) {
        (points_route_list_now[i]).clear();
    }

    reference_positions_route_list_now.clear();
    reference_positions_route_list_now.resize((parent->parent->data->route_list.size()));

    //tabulate the points of routes
    for (i = 0; i < parent->parent->data->route_list.size(); i++) {

        //write the points of the curves corresponding to the Routes into points_route_list_now
        //change this at the end, when you will have a function Draw that handles loxodromes. Then, you will use only the first case of this if
        if (((parent->parent->data->route_list)[i]).type != (Route_types[0])) {

            ((parent->parent->data->route_list)[i]).Draw((unsigned int)(wxGetApp().n_points_routes.value), this, (points_route_list_now.data()) + i, String(""));

        }
        else {

            ((parent->parent->data->route_list)[i]).DrawOld((unsigned int)(wxGetApp().n_points_routes.value), this, (points_route_list_now.data()) + i, String(""));

        }

        //write the reference Positions into reference_positions_route_list_now
        if (GeoToDrawPanel(((parent->parent->data->route_list)[i]).reference_position, &p, false)) {
            //the reference position falls in the plot area -> write it into reference_positions_route_list_now
            reference_positions_route_list_now[i] = p;
        }
        else {
            //the reference position does not fall in the plot area -> write a 'Null' value into reference_positions_route_list_now which will be ignored in other methods because it lies outside the plot area
            reference_positions_route_list_now[i] = wxPoint(0, 0);
        }

    }

}



//tabulate into points_position_list_now all the Positions. points_position_list will then be used to render the Positions
inline void DrawPanel::TabulatePositions(void) {

    unsigned int i;
    wxPoint p;

    //resize points_position_list_now and, which needs to have the same size as (data->position_list)
    points_position_list_now.clear();
    points_position_list_now.resize((parent->parent->data->position_list).size());

    //tabulate the points of Positions
    for (i = 0; i < (parent->parent->data->position_list.size()); i++) {

        //write the reference Positions into reference_positions_route_list_now
        if (GeoToDrawPanel((parent->parent->data->position_list)[i], &p, false)) {
            //the  position falls in the plot area -> write it into points_position_list_now
            points_position_list_now[i] = p;
        }
        else {
            //the  position does not fall in the plot area -> write a 'Null' value into points_position_list_now which will be ignored in other methods because it lies outside the plot area
            points_position_list_now[i] = wxPoint(0, 0);
        }

    }

}



//draws coastlines, Routes and Positions on the Mercator-projection case
inline void DrawPanel::PreRenderMercator(void) {

    PositionProjection delta_temp;
    unsigned int n_intervals_ticks, n_intervals_ticks_max;
    Position q;
    String prefix, new_prefix;
    wxPoint p;
    wxString dummy_label;
    Route route;
    Angle phi;

    //append \t to prefix
    prefix = String("");
    new_prefix = prefix.append(String("\t"));

    //client_dc->Clear();


    //here I compute multiple quantities relative to the y axis: this computation is done here, at the very beginning of PreRenderMercator, because these quantitites will be needed immediatly to compute size_label_horizontal
    //set phi_start, phi_end and delta_phi
    phi_span = (((parent->phi_max).normalize_pm_pi_ret()).value) - (((parent->phi_min).normalize_pm_pi_ret()).value);

    //gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (phi_span > k) {
        //in this case, phi_span is larger than one degree
        gamma_phi = 1;
        delta_phi_minor = -1.0;
    }
    else {
        if (phi_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > phi_span > 10 arcminutes
            gamma_phi = 60;
            delta_phi_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > phi_span
            gamma_phi = 60 * 10;
            delta_phi_minor = tenth_arcmin_radians;
        }
    }

    delta_phi = k / ((double)gamma_phi);
    while (((wxGetApp().n_intervals_ticks_preferred).value) * delta_phi < phi_span) {
        if (delta_phi == k / ((double)gamma_phi)) { delta_phi += k * 4.0 / ((double)gamma_phi); }
        else { delta_phi += k * 5.0 / ((double)gamma_phi); }
    }

    //here I set up things to plot meridians and parallels in Render_Mercator

    //set phi_start/end
    (phi_start.value) = floor((parent->phi_min.normalize_pm_pi_ret().value) / delta_phi) * delta_phi;
    (phi_end.value) = (parent->phi_max.normalize_pm_pi_ret().value);


    //compute size of largest label on parallel: run through all labels on parallels and set size_label_horizontal as the size of the largest label on parallel that has ben found
    for (size_label_horizontal = 0,
        first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminuted) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        ((q.phi).value) = (phi_start.value),
        (q.lambda) = (parent->lambda_min) - epsilon_double;
        ((q.phi).value) < (phi_end.value);
        ((q.phi).value) += delta_phi
        ) {

        WriteLabel(q, parent->phi_min, parent->phi_max, label_precision, String("NS"), &dummy_label);

        if ((GetTextExtent(dummy_label).GetWidth()) > ((int)size_label_horizontal)) {
            size_label_horizontal = (GetTextExtent(dummy_label).GetWidth());
        }

    }

    //take the angle 0° 0.0' expresed with display_precision: the height of this angle label is the largest possible -> set it equal to size_label_vertical
    size_label_vertical = (GetTextExtent(wxString((Angle(0, 0.0).to_string(String("NS"), (display_precision.value), false)))).GetHeight());


    //set x_min, ..., y_max for the following
    (this->*Set_x_y_min_max)();

    //set rectangle_obseerver
    rectangle_observer = PositionRectangle(Position(parent->lambda_min, parent->phi_max), Position(parent->lambda_max, parent->phi_min), String(""));

    /*set the aspect ratio between height and width equal to the ratio between the y and x range: in this way, the aspect ratio of the plot is equal to 1*/

    if ((!(parent->dragging_chart)) && (!(parent->mouse_scrolling))) {
        //the ChartFrame is not being dragged and the mouse is not scrolling -> the chart's size will change -> re-compute its size

        if ((y_max - y_min) > x_span()) {
            //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
            parent->SetSize(
                (((wxGetApp().rectangle_display).GetSize()).GetHeight()) / ((y_max - y_min) / x_span()),
                (((wxGetApp().rectangle_display).GetSize()).GetHeight())
            );

        }
        else {
            //set the height and width of ChartFrame with the correct aspect ratio and in such a way that the Chart Frame object fits into the screen
            parent->SetSize(
                (((wxGetApp().rectangle_display).GetSize()).GetHeight()),
                (((wxGetApp().rectangle_display).GetSize()).GetHeight()) * ((y_max - y_min) / x_span())
            );
        }
    }

    (this->*Set_size_chart)();
    //set the size of *this equal to the size of the chart, in such a way that draw_panel can properly contain the chart
    SetSize(size_chart);

    //I am about to modify position_plot_area_now -> save it in position_plot_area_before
    position_plot_area_before = position_plot_area_now;

    //sets size_plot_area and stores into position_plot_area the screen position of the top-left edge of the plot area.
    if (
        ((size_chart.GetWidth()) - (((int)size_label_horizontal) + 3 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))) * (size_chart.GetHeight()) / (size_chart.GetWidth())
        < (size_chart.GetHeight()) - (((int)size_label_vertical) + 3 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))
        ) {
        //if I set size_plot_area's width first to leave room for parallel labels and label_position_now, then there is enough space to set size_plot_area's height by keeping the aspect ratio

        size_plot_area.SetWidth(
            (size_chart.GetWidth())
            //space for  parallel labels
            - (((int)size_label_horizontal) + 3 * ((wxGetApp().border).value))
            //space for label_position_now
            - ((((int)size_label_vertical) + ((wxGetApp().border).value))) * (size_chart.GetWidth()) / (size_chart.GetHeight())
        );
        size_plot_area.SetHeight((size_plot_area.GetWidth()) * (size_chart.GetHeight()) / (size_chart.GetWidth()));

        position_plot_area_now = wxPoint(
            ((int)size_label_horizontal) + 2 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
            (((int)(size_chart.GetHeight())) - (((int)(size_plot_area.GetHeight())) + ((int)size_label_vertical) + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))) / 2
        );

    }
    else {
        //if I set size_plot_area's width first to leave room for  parallel labels and label_position_now and there is not enough space to set size_plot_area's height by keeping the aspect ratio -> I set size_plot_area's height first, by leaving space in the resulting height for meridian labels and label_position_now , and set the width later according to the aspect ratio

        size_plot_area.SetHeight(
            (size_chart.GetHeight())
            //space for meridian labels
            - (((int)size_label_vertical) + 3 * ((wxGetApp().border).value))
            //space for label_position_now
            - (((int)size_label_vertical) + ((wxGetApp().border).value))
        );
        size_plot_area.SetWidth((size_plot_area.GetHeight()) * (size_chart.GetWidth()) / (size_chart.GetHeight()));

        if (((size_plot_area.GetHeight()) * (size_chart.GetWidth()) / (size_chart.GetHeight())) < ((size_chart.GetWidth()) - (((int)size_label_horizontal) + 3 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)))) {
            //good: there is enough space

        }
        else {
            //bad: there is not enough space

        }

        position_plot_area_now = wxPoint(

            (((int)(size_chart.GetWidth())) - (((int)(size_plot_area.GetWidth())) - ((int)size_label_horizontal) - (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))) / 2


            ,

            (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)

        );

    }
    
    tick_length = (((wxGetApp().tick_length_over_width_plot_area)).value) * (size_plot_area.GetWidth());

    //set p_NW and p_SE
    //updates the position of the draw pane this
//    DrawPanelToGeo(wxPoint(position_plot_area_now) /*I move the NW boundary of the plot area to the interior by one pixel*/ + wxPoint(1, 1), &p_NW);
//    DrawPanelToGeo(wxPoint(position_plot_area_now + size_plot_area) /*I move the SE boundary of the plot area to the interior by one pixel*/ - wxPoint(1, 1), &p_SE);

    //fetch the data on the region that I am about to plot from the data files and store it into parent->coastline_polygons_now
    parent->GetCoastLineData_Mercator();

    //the number of ticks is given by the minimum between the preferred value and the value allowed by fitting the (maximum) size of each axis label into the witdh of the axis
    n_intervals_ticks_max = ((unsigned int)floor(((double)(size_plot_area.GetWidth())) / ((double)size_label_horizontal)));
    n_intervals_ticks = min(
        (unsigned int)(wxGetApp().n_intervals_ticks_preferred.value),
        n_intervals_ticks_max
    );



    //set lambda_span
    if ((((parent->lambda_min) < M_PI) && ((parent->lambda_max) > M_PI)) || ((parent->lambda_min) < (parent->lambda_max))) {
        //the 'anomalous' situation where the chart encompasses the Greenwich antimeridian

        lambda_span = ((parent->lambda_min).value) - ((parent->lambda_max).value) + 2.0 * M_PI;

    }
    else {
        //the 'normal' situation where the chart does not encompass the Greenwich antimeridian

        lambda_span = ((parent->lambda_min).value) - ((parent->lambda_max).value);

    }

    //gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (lambda_span > k) {
        //in this case, lambda_span is larger than one degree
        gamma_lambda = 1;
        delta_lambda_minor = -1.0;
    }
    else {
        if (lambda_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > lambda_span > 10 arcminutes
            gamma_lambda = 60;
            delta_lambda_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > lambda_span
            gamma_lambda = 60 * 10;
            delta_lambda_minor = tenth_arcmin_radians;
        }
    }

    delta_lambda = k / ((double)gamma_lambda);
    while (n_intervals_ticks * delta_lambda < lambda_span) {
        if (delta_lambda == k / ((double)gamma_lambda)) { delta_lambda += k * 4.0 / ((double)gamma_lambda); }
        else { delta_lambda += k * 5.0 / ((double)gamma_lambda); }
    }


    if (((parent->lambda_min) < M_PI) && ((parent->lambda_max) > M_PI)) {

        (lambda_start.value) = ceil(((parent->lambda_max).value) / delta_lambda) * delta_lambda;
        (lambda_end.value) = ((parent->lambda_min).value) + (2.0 * M_PI);

    }
    else {

        if ((parent->lambda_min) > (parent->lambda_max)) {

            (lambda_start.value) = ceil(((parent->lambda_max).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((parent->lambda_min).value);

        }
        else {

            (lambda_start.value) = ceil(((parent->lambda_max).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((parent->lambda_min).value) + 2.0 * M_PI;

        }

    }


    //compute labels on parallels and meridians
    //save parallels_and_meridians_labels_now and positions_parallels_and_meridians_labels_now into parallels_and_meridians_labels_before and  positions_parallels_and_meridians_labels_before, respectively.  clears all labels previously drawn
    parallels_and_meridians_labels_before = parallels_and_meridians_labels_now;
    positions_parallels_and_meridians_labels_before = positions_parallels_and_meridians_labels_now;
    parallels_and_meridians_labels_now.resize(0);
    positions_parallels_and_meridians_labels_now.resize(0);

    //compute labels on parallels
    for (first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminuted) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        ((q.phi).value) = (phi_start.value),
        (q.lambda) = (parent->lambda_min) - epsilon_double;
        ((q.phi).value) < (phi_end.value);
        ((q.phi).value) += delta_phi
        ) {

        DrawLabel(q, parent->phi_min, parent->phi_max, label_precision, String("NS"));

    }

    //compute labels on meridians
    for (first_label = true,
        //set the label precision: if gamma_lambda = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_lambda*K*60 (the spacing between labels in arcminutes) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_lambda == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_lambda * K * 60)))),
        ((q.lambda).value) = (lambda_start.value),
        (q.phi) = (parent->phi_min) + epsilon_double;
        ((q.lambda).value) < (lambda_end.value);
        ((q.lambda).value) += delta_lambda
        ) {

        DrawLabel(q, parent->lambda_max, parent->lambda_min, label_precision, String("EW"));

    }

    grid_now.clear();
    ticks_now.clear();

    //prerender meridians
    //set route equal to a meridian going through lambda: I set everything except for the longitude of the ground posision, which will vary in the loop befor and will be fixed inside the loop
    route.type.set(String(((Route_types[1]).value)));
    route.Z.set(String(""), 0.0, String(""));
    (route.reference_position.phi) = ((parent->phi_min));

    //draw the first chunk of intermediate ticks on the longitude axis
    if (gamma_lambda != 1) {

        route.length.set(String(""), Re * (((wxGetApp().tick_length_over_width_plot_area)).value) * phi_span, String(""));

        //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
        for ((route.reference_position.lambda.value) = (lambda_start.value) - delta_lambda;
            (route.reference_position.lambda.value) - ((lambda_start.value) - delta_lambda) < delta_lambda;
            (route.reference_position.lambda.value) += delta_lambda_minor) {

            //            ticks_now.push_back(route);
            ticks_now.resize((ticks_now.size()) + 1);
            route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));

            //            route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));

        }

    }


    for (route.length.set(String(""), Re* ((parent->phi_max.normalize_pm_pi_ret().value) - (parent->phi_min.normalize_pm_pi_ret().value)), String("")),
        (route.reference_position.lambda.value) = (lambda_start.value);
        (route.reference_position.lambda.value) < (lambda_end.value);
        (route.reference_position.lambda.value) += delta_lambda) {

        //add the current meridian that is being drawn (route) to meridians_now
//        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.Draw((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //             route.Draw(((((parent->parent)->data)->n_points_routes).value), foreground_color, background_color, thickness, dc, this, String(""));

        if (gamma_lambda != 1) {
            //draw intermediate ticks on the longitude axis

            (lambda_saved.value) = (route.reference_position.lambda.value);
            route.length.set(String(""), Re * (((wxGetApp().tick_length_over_width_plot_area)).value) * phi_span, String(""));

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for ((route.reference_position.lambda.value) = (lambda_saved.value);
                (route.reference_position.lambda.value) - (lambda_saved.value) < delta_lambda;
                (route.reference_position.lambda.value) += delta_lambda_minor) {

                //                ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //                     route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));
            }

            route.length.set(String(""), Re * ((parent->phi_max.normalize_pm_pi_ret().value) - (parent->phi_min.normalize_pm_pi_ret().value)), String(""));
            (route.reference_position.lambda.value) = (lambda_saved.value);

        }

    }

    //prerender parallels
    //set route equal to a parallel of latitude phi, i.e., a circle of equal altitude
    route.type.set(String(((Route_types[0]).value)));
    route.Z.set(String(""), M_PI_2, String(""));
    (route.reference_position.lambda) = (parent->lambda_min);

    //this loop runs over the latitude of the parallel, which we call phi
    for ((phi.value) = (phi_start.value);
        (phi.value) < (phi_end.value);
        (phi.value) += delta_phi
        ) {

        //route.omega  and route.reference_position.phi of the circle of equal altitude are set for each value of phi as functions of phi, in such a way that route.omega is always smaller than pi/2
        (route.reference_position.phi) = phi;
        route.length.set(String(""),
            Re * cos(phi) * ((

                ((((parent->lambda_min)) < M_PI) && (((parent->lambda_max)) > M_PI)) ? (((parent->lambda_min)) - ((parent->lambda_max)) + 2.0 * M_PI) : (((parent->lambda_min)) - ((parent->lambda_max)))

                ).value), String(""));

        //add the current parallel that is being drawn to parallels
//        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.DrawOld((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //here I use DrawOld because Draw cannot handle loxodromes
//        route.DrawOld((parent->parent->data->n_points_routes.value), foreground_color, thickness, dc, this);

        if (gamma_phi != 1) {
            //draw smaller ticks -> set route to a loxodrome pointing towards the E and draw it

            route.length.set(String(""), Re * (wxGetApp().tick_length_over_width_plot_area.value) * lambda_span, String(""));

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for (
                (route.reference_position.phi.value) = (phi.value);
                (route.reference_position.phi.value) - (phi.value) < delta_phi;
                (route.reference_position.phi.value) += delta_phi_minor
                ) {

                //                    ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.DrawOld((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //here I use DrawOld because Draw cannot handle loxodromes
//                route.DrawOld(((wxGetApp().n_points_minor_ticks)).value, foreground_color, thickness, dc, this);

            }

        }

    }


    TabulateRoutes();
    TabulatePositions();

    //tell PaintEvent that everything but highligghteable objects (coastlines, meridians ... ) must be re-drawn
    re_draw = true;

    //    (parent->point_coastline_now).clear();


    //center the parent in the middle of the screen because the plot shape has changed and the plot may thus be misplaced on the screen
    //    parent->CenterOnScreen();


}

//this function draws coastlines, Routes and Positions in the 3D case
inline void DrawPanel::PreRender3D(void) {

    Angle lambda_in, lambda_out, /*phi is an auxiliary variable used in the loop which draws parallels*/phi;
    Position q;
    PositionProjection temp;
    wxPoint p;
    wxString dummy_label;
    Route route;
    unsigned int n_intervals_ticks;


    //set zoom_factor, the boundaries of x and y for the chart, and the latitudes and longitudes which comrpise circle_observer
    (parent->zoom_factor).set(String(""), (parent->parent->circle_observer_0.omega.value) / (circle_observer.omega.value), String(""));
    (this->*Set_x_y_min_max)();
    (this->*Set_lambda_phi_min_max)();

    parent->GetCoastLineData_3D();

    if ((!(parent->dragging_chart)) && (!(parent->mouse_scrolling))) {
        //I am not dragging the chart nor scrolling -> the size of the chart may change -> re-compute it
        parent->SetSize(
            (((wxGetApp().rectangle_display).GetSize()).GetHeight()),
            (((wxGetApp().rectangle_display).GetSize()).GetHeight())
        );
    }
    (this->*Set_size_chart)();
    SetSize(size_chart);

    size_plot_area.SetWidth((size_chart.GetWidth()) * (length_plot_area_over_length_chart.value));
    size_plot_area.SetHeight((size_chart.GetHeight()) * (length_plot_area_over_length_chart.value));

    position_plot_area_before = position_plot_area_now;
    position_plot_area_now = wxPoint((int)(((double)(size_chart.GetWidth())) * (1.0 - (length_plot_area_over_length_chart.value)) / 2.0),
        (int)(((double)(size_chart.GetHeight())) * (1.0 - (length_plot_area_over_length_chart.value)) / 2.0));

    //the number of ticks is given by the minimum between the preferred value and the value allowed by fitting the (maximum) size of each axis label into the witdh of the axis
    n_intervals_ticks = (unsigned int)(wxGetApp().n_intervals_ticks_preferred.value);


    //here I set up things to plot paralles and meridians in Render_3D

    //set lambda_span
    if (((parent->lambda_min) == 0.0) && ((parent->lambda_max) == 0.0)) {
        //in this case circle_observer spans all longitudes

        //because in this case lambda_min/max span the whole angle 2 pi and cannot define a range for lambda_span, I set
        lambda_span = 2.0 * ((circle_observer.omega).value);

    }
    else {
        //in this case, there are two finite longitudes which encircle circle_observer

        if (((parent->lambda_min) < M_PI) && ((parent->lambda_max) > M_PI)) {

            lambda_span = ((parent->lambda_min).value) - ((parent->lambda_max).value) + 2.0 * M_PI;

        }
        else {

            lambda_span = ((parent->lambda_min).value) - ((parent->lambda_max).value);

        }

    }


    //gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (lambda_span > k) {
        //in this case, lambda_span is larger than one degree
        gamma_lambda = 1;
        delta_lambda_minor = -1.0;
    }
    else {
        if (lambda_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > lambda_span > 10 arcminutes
            gamma_lambda = 60;
            delta_lambda_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > lambda_span
            gamma_lambda = 60 * 10;
            delta_lambda_minor = tenth_arcmin_radians;
        }
    }

    //compute delta_lambda
    delta_lambda = k / ((double)gamma_lambda);
    while (n_intervals_ticks * delta_lambda < lambda_span) {
        if (delta_lambda == k / ((double)gamma_lambda)) { delta_lambda += k * 4.0 / ((double)gamma_lambda); }
        else { delta_lambda += k * 5.0 / ((double)gamma_lambda); }
    }

    //compute lambda_middle
    lambda_middle.set(String(""), round((((circle_observer.reference_position).lambda).value) / delta_lambda) * delta_lambda, String(""));


    //set lambda_start, lambda_end
    if (((parent->lambda_min) == 0.0) && ((parent->lambda_max) == 0.0)) {
        //in this case circle_observer spans all longitudes

        (lambda_start.value) = 0.0;
        (lambda_end.value) = 2.0 * M_PI;

    }
    else {
        //in this case, there are two finite longitudes which encircle circle_observer

        if (((parent->lambda_min) < M_PI) && ((parent->lambda_max) > M_PI)) {

            (lambda_start.value) = floor(((parent->lambda_max).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((parent->lambda_min).value) + (2.0 * M_PI);

        }
        else {

            (lambda_start.value) = floor(((parent->lambda_max).value) / delta_lambda) * delta_lambda;
            (lambda_end.value) = ((parent->lambda_min).value);

        }

    }




    //set phi_start, phi_end and delta_phi
    phi_span = 2.0 * ((circle_observer.omega).value);

    //gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
    if (phi_span > k) {
        //in this case, phi_span is larger than one degree
        gamma_phi = 1;
        delta_phi_minor = -1.0;
    }
    else {
        if (phi_span > 10.0 * arcmin_radians) {
            //in this case, one arcdegree > phi_span > 10 arcminutes
            gamma_phi = 60;
            delta_phi_minor = arcmin_radians;
        }
        else {
            //in this case, 10 arcminutes > phi_span
            gamma_phi = 60 * 10;
            delta_phi_minor = tenth_arcmin_radians;
        }
    }

    delta_phi = k / ((double)gamma_phi);
    while (((wxGetApp().n_intervals_ticks_preferred).value) * delta_phi < phi_span) {
        if (delta_phi == k / ((double)gamma_phi)) { delta_phi += k * 4.0 / ((double)gamma_phi); }
        else { delta_phi += k * 5.0 / ((double)gamma_phi); }
    }

    //set phi_start/end and phi_middle
    (phi_start.value) = floor((((parent->phi_min).normalize_pm_pi_ret()).value) / delta_phi) * delta_phi;
    (phi_end.value) = (((parent->phi_max).normalize_pm_pi_ret()).value);

    phi_middle.set(String(""), round((((circle_observer.reference_position).phi).value) / delta_phi) * delta_phi, String(""));
    //if the line above sets phi_middle equal to +/- pi/2. the labels of meridians will all be put at the same location on the screen (the N/S pole), and they would look odd ->
    if ((fabs((phi_middle.value) - M_PI_2) < epsilon_double) || (fabs((phi_middle.value) - (3.0 * M_PI_2)) < epsilon_double)) {
        (phi_middle.value) -= GSL_SIGN((phi_middle.normalize_pm_pi_ret()).value) * delta_phi;
    }



    //compute the size of labels, i.e., size_label_horizontal and size_label_vertical
    //compute size of largest label on parallel: run through all labels on parallels and set size_label_horizontal as the size of the largest label on parallel that has ben found
    for (size_label_horizontal = 0,
        first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminutes) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        ((q.phi).value) = (phi_start.value),
        (q.lambda) = (parent->lambda_min) - epsilon_double;
        ((q.phi).value) < (phi_end.value);
        ((q.phi).value) += delta_phi
        ) {

        WriteLabel(q, parent->phi_min, parent->phi_max, label_precision, String("NS"), &dummy_label);

        if ((GetTextExtent(dummy_label).GetWidth()) > ((int)size_label_horizontal)) {
            size_label_horizontal = (GetTextExtent(dummy_label).GetWidth());
        }

    }

    //take the angle 0° 0.0' expresed with display_precision: the height of this angle label is the largest possible -> set it equal to size_label_vertical
    size_label_vertical = (GetTextExtent(wxString((Angle(0, 0.0).to_string(String("NS"), (display_precision.value), false)))).GetHeight());

    TabulateRoutes();
    TabulatePositions();



    //compute labels on parallels and meridians
    //save parallels_and_meridians_labels_now and positions_parallels_and_meridians_labels_now into parallels_and_meridians_labels_before and  positions_parallels_and_meridians_labels_before, respectively.  clears all labels previously drawn
    parallels_and_meridians_labels_before = parallels_and_meridians_labels_now;
    positions_parallels_and_meridians_labels_before = positions_parallels_and_meridians_labels_now;
    parallels_and_meridians_labels_now.resize(0);
    positions_parallels_and_meridians_labels_now.resize(0);

    //compute labels on parallels
    for (first_label = true,
        //set the label precision: if gamma_phi = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_phi*K*60 (the spacing between labels in arcminuted) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_phi == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_phi * K * 60)))),
        (q.phi.value) = floor((circle_observer.reference_position.phi.normalize_pm_pi_ret().value - circle_observer.omega.value) / delta_phi) * delta_phi,
        (q.lambda) = lambda_middle;
        (q.phi.value) < (circle_observer.reference_position.phi.normalize_pm_pi_ret().value) + (circle_observer.omega.value);
        (q.phi.value) += delta_phi
        ) {

        DrawLabel(q, parent->phi_min, parent->phi_max, label_precision, String("NS"));

    }

    //compute labels on meridians
    for (first_label = true,
        //set the label precision: if gamma_lambda = 1, then labels correspond to integer degrees, and I set label_precision = display_precision. If not, I take the log delta_lambda*K*60 (the spacing between labels in arcminutes) -> I obtain the number of digits reqired to proprely display arcminutes in the labels -> round it up for safety with ceil() -> add 2 -> obtain the number of digits to safely display the digits before the '.' (2) and the digits after the '.' in the arcminute part of labels
        (label_precision.value) = (gamma_lambda == 1) ? (display_precision.value) : (2 + ceil(fabs(log(delta_lambda * K * 60)))),
        ((q.lambda).value) = (lambda_start.value),
        (q.phi) = phi_middle;
        ((q.lambda).value) < (lambda_end.value);
        ((q.lambda).value) += delta_lambda
        ) {

        DrawLabel(q, parent->lambda_max, parent->lambda_min, label_precision, String("EW"));

    }


    grid_now.clear();
    ticks_now.clear();

    //draw meridians
    //set route equal to a meridian going through lambda: I set everything except for the longitude of the ground posision, which will vary in the loop befor and will be fixed inside the loop
    route.type.set(String(((Route_types[1]).value)));
    route.length.set(String(""), Re * M_PI, String(""));
    route.Z.set(String(""), 0.0, String(""));
    (route.reference_position.phi) = -M_PI_2;

    for ((route.reference_position.lambda.value) = (lambda_start.value);
        (route.reference_position.lambda.value) < (lambda_end.value);
        (route.reference_position.lambda.value) += delta_lambda) {

        //add the current meridian that is being drawn (route) to meridians
        //        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.Draw((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //        route.Draw(((((parent->parent)->data)->n_points_routes).value), foreground_color, background_color, thickness, dc, this, String(""));

        if (gamma_lambda != 1) {
            //draw intermediate ticks on the longitude axis by setting route to an orthodrome pointing to the north

            (lambda_saved.value) = (route.reference_position.lambda.value);
            phi_saved = ((route.reference_position).phi);
            Z_saved = (route.Z);

            (route.Z).set(String(""), 0.0, String(""));
            route.length.set(String(""), Re * 2.0 * ((((wxGetApp().tick_length_over_aperture_circle_observer)).value) * ((circle_observer.omega).value)), String(""));
            ((route.reference_position).phi) = phi_middle;

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for ((route.reference_position.lambda.value) = (lambda_saved.value);
                (route.reference_position.lambda.value) - (lambda_saved.value) < delta_lambda;
                (route.reference_position.lambda.value) += delta_lambda_minor) {

                //                ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //                route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));

            }

            route.length.set(String(""), Re * M_PI, String(""));
            (route.Z) = Z_saved;
            (route.reference_position.lambda.value) = (lambda_saved.value);
            ((route.reference_position).phi) = phi_saved;

        }

    }

    //draw parallels
    //set route equal to a parallel of latitude phi, i.e., a circle of equal altitude
    route.type.set((Route_types[2]));
    ((route.reference_position).lambda) = lambda_middle;

    //this loop runs over the latitude of the parallel, which we call phi
    for ((phi.value) = (phi_start.value);
        (phi.value) < (phi_end.value);
        (phi.value) += delta_phi
        ) {

        //route.omega  and route.reference_position.phi of the circle of equal altitude are set for each value of phi as functions of phi, in such a way that route.omega is always smaller than pi/2
        route.omega.set(String(""), M_PI_2 - fabs(phi.value), String(""));
        route.length.set(String(""), 2.0 * M_PI * Re * sin(route.omega), String(""));
        route.reference_position.phi.set(String(""), GSL_SIGN(phi.value) * M_PI_2, String(""));

        //add the current parallel that is being drawn to parallels
        //        grid_now.push_back(route);
        grid_now.resize((grid_now.size()) + 1);
        route.Draw((wxGetApp().n_points_routes.value), this, &(grid_now.back()), String(""));
        //        route.Draw((parent->parent->data->n_points_routes.value), foreground_color, background_color, thickness, dc, this, String(""));

        if (gamma_phi != 1) {
            //to draw smaller ticks, I set route to a loxodrome pointing towards the E and draw it

            route.type.set(String(((Route_types[1]).value)));
            route.Z.set(String(""), M_PI_2, String(""));
            route.length.set(String(""), Re * 2.0 * ((((wxGetApp().tick_length_over_aperture_circle_observer)).value) * ((circle_observer.omega).value)), String(""));

            //set custom-made minor xticks every tenths (i/10.0) of arcminute (60.0)
            for (
                (route.reference_position.phi.value) = (phi.value);
                (route.reference_position.phi.value) - (phi.value) < delta_phi;
                (route.reference_position.phi.value) += delta_phi_minor
                ) {

                //                    ticks_now.push_back(route);
                ticks_now.resize((ticks_now.size()) + 1);
                route.Draw((wxGetApp().n_points_minor_ticks.value), this, &(ticks_now.back()), String(""));
                //                route.Draw(((wxGetApp().n_points_minor_ticks)).value, foreground_color, background_color, thickness, dc, this, String(""));

            }

            route.type.set((Route_types[2]));

        }

    }


    //updates the position of the DrawPanel *this
    draw_panel_origin = (this->GetScreenPosition());

    //tell PaintEvent that everything but highligghteable objects (coastlines, meridians ... ) must be re-drawn
    re_draw = true;

}


//constructs a color object by setting no arguments
Color::Color() : wxColour() {

}


//constructs a color object by setting no arguments
Color::Color(wxColour input) : wxColour(input) {

}


//constructs a color objct by setting its rgb values to red, green, blue
Color::Color(unsigned char red, unsigned char green, unsigned char blue) : wxColour(red, green, blue) {

}

//constructs a color objct by setting its rgb values to red, green, blue and transparency (alpha)
Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha) : wxColour(red, green, blue, alpha) {

}

//converts *this to RGB format
int Color::ToRGB(void) {

    int i, output;

    //convert this to BRG format which the wxWidgets function GetRGB() and then shift its bits so as to convert it to RGB format
    for (output = 0, i = 0; i < 3; i++) {

        output ^= ((((this->GetRGB()) >> 8 * i) & hexadecimal_mask) << 8 * (3 - (i + 1)));

    }

    return output;

}


//constructs a Bitmap object by setting no arguments
Bitmap::Bitmap() : wxBitmap() {

}

//constructs a Bitmap object by loading it from path path and rescaling it to size, by keeping its proportions
Bitmap::Bitmap(String path, wxSize size) :

#ifdef __APPLE__
    //I am on apple operating system->I set the bitmap from the file path

    wxBitmap(RescaleProportionally(wxBitmap(path.value, wxBITMAP_TYPE_PNG).ConvertToImage(), size)) {}

#endif

#ifdef _WIN32
//I am on windows operating system-> I load the bitmap from the windows resources

wxBitmap(wxBitmap(wxString(path.filename_without_folder_nor_extension(String("")).value), wxBITMAP_TYPE_PNG_RESOURCE).ConvertToImage().Scale(size.GetWidth(), size.GetHeight())) {}

#endif


//construct a StaticBitmap object by assignign to it the parent parent, loading it from path path and rescaling it to size
StaticBitmap::StaticBitmap(wxWindow* parent, String path, [[maybe_unused]] wxSize size) : wxStaticBitmap(parent, wxID_ANY, wxNullBitmap) {

    FileR file;

    file.set_name(path);

#ifdef __APPLE__
    //I am on apple operating system

    SetBitmap(
        Bitmap(file.name.value,
            ToDIP(wxSize(
                (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
            ))
        )
    );

#endif

#ifdef _WIN32
    //I am on windows operating system

    wxBitmap temp;

    temp = wxBitmap(wxString(file.name_without_folder_nor_extension.value), wxBITMAP_TYPE_PNG_RESOURCE);
    temp = wxBitmap(temp.ConvertToImage().Scale(size.GetWidth(), size.GetHeight()));
    SetBitmap(temp);

#endif

}


ChartFrame::ChartFrame(ListFrame* parent_input, Projection projection_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    stringstream s;
    unsigned long long int i, j;
    String new_prefix, default_projection, color;
    //empty wxStaticTexts to fill the empty spaces of the wxGridSizer sizer_buttons
    StaticText* empty_text_1, * empty_text_2, * empty_text_3, * empty_text_4, * empty_text_5;
    wxCommandEvent dummy_event;
    //a wxSizerFlags object to insert stuff into sizers
    wxSizerFlags flags;

    parent = parent_input;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    flags.Center();

    //when a ChartFrame is created, the chart is not being dragged
    dragging_chart = false;

    //read lambda_min, ...., phi_max from file_init
    lambda_min.read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    lambda_max.read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    phi_min.read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), new_prefix);
    phi_max.read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), new_prefix);


    this->Bind(wxEVT_CLOSE_WINDOW, &ChartFrame::OnPressCtrlW<wxCloseEvent>, this);

    mouse_scrolling = false;
    //set the zoom factor to 1 for the initial configuration of the projection
    zoom_factor.set(String(""), 1.0, String(""));
    //read zoom_factor_max from file_init
    wxGetApp().zoom_factor_max.read_from_file_to(String("maximal zoom factor"), (wxGetApp().path_file_init), String("R"), String(""));
    idling = false;
    unset_idling = new UnsetIdling<ChartFrame>(this);
    
    //coastline_polygons_now/before and polygon_position_now/before are resized to their maximum possible value
    for(i=0, j=0; i<parent->coastline_polygons_Position.size(); i++) {
        j += (parent->coastline_polygons_Position[i].size());
    }
    coastline_polygons_now.resize(j);
    polygon_position_now.resize(parent->coastline_polygons_Position.size());
    coastline_polygons_before.resize(j);
    polygon_position_before.resize(parent->coastline_polygons_Position.size());

    print_error_message = new PrintMessage<ChartFrame, UnsetIdling<ChartFrame> >(this, unset_idling);

    panel = new ChartPanel(this, wxDefaultPosition, wxDefaultSize);
    draw_panel = new DrawPanel(panel, wxDefaultPosition, wxDefaultSize);

    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_slider = new wxBoxSizer(wxVERTICAL);
    sizer_buttons = new wxGridSizer(3, 3, 0, 0);
    
    //initialize the variable neededed for slider
    //allocate the slider
    slider = new wxSlider(panel, wxID_ANY, 1, 1, (int)((wxGetApp().zoom_factor_max).value), wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL);

    //sets the coefficients for the function which relates the zoom factor to the slider value: read from file (wxGetApp().e_zoom) and set (wxGetApp().a_zoom), (wxGetApp().b_zoom)
    (wxGetApp().e_zoom).read_from_file_to(String("exponent zoom"), (wxGetApp().path_file_init), String("R"), String(""));
    (wxGetApp().a_zoom).set(String(""), (-1.0 + ((wxGetApp().zoom_factor_max).value)) / (-1.0 + pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value)), String(""));
    (wxGetApp().b_zoom).set(String(""), (pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value) - ((wxGetApp().zoom_factor_max).value)) / (-1.0 + pow(((double)(slider->GetMax())), (wxGetApp().e_zoom).value)), String(""));
 

    //text field showing the current value of the zoom slider
    s.str("");
    s << "1:" << (zoom_factor.value);

    text_slider = new StaticText(panel, wxString(s.str().c_str()), wxDefaultPosition, wxDefaultSize, 0);
    observer_height = new StaticLengthField<ChartFrame>(panel, &(draw_panel->d), LengthUnit_types[0]);

    //navigation buttons
    button_up = new wxButton(panel, wxID_ANY, wxT("N"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_down = new wxButton(panel, wxID_ANY, wxT("S"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_left = new wxButton(panel, wxID_ANY, wxT("W"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
    button_right = new wxButton(panel, wxID_ANY, wxT("E"), wxDefaultPosition, GetTextExtent(wxS("000")), wxBU_EXACTFIT);
//    button_reset = new wxButton(panel, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize);
    
    
    button_reset = new wxBitmapButton(
                                      panel,
                                      wxID_ANY,
                                      Bitmap(wxGetApp().path_file_reset_icon, (wxGetApp().size_large_button) - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                      wxDefaultPosition,
                                      wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)),
                                      wxBU_EXACTFIT | wxSIMPLE_BORDER
                                      );
    
    //button to show list
    button_show_list = new wxBitmapButton(
                                          panel,
                                          wxID_ANY,
                                          Bitmap(wxGetApp().path_file_list_icon, (wxGetApp().size_large_button) - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                          wxDefaultPosition,
                                          wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value)),
                                          wxBU_EXACTFIT | wxSIMPLE_BORDER
                                          );
    button_show_list->Bind(wxEVT_BUTTON, &MyApp::ShowList, &wxGetApp());
    button_show_list->SetToolTip(wxString("Show the list of sights, positions and routes"));

    projection = new ProjectionField<ChartFrame>(panel, &(wxGetApp().list_frame->data->recent_projections));
    projection->name->Bind(wxEVT_COMBOBOX, &DrawPanel::OnChooseProjection<wxCommandEvent>, draw_panel);
    projection->SetToolTip(String("Choose the projection used in the chart"));

    button_up->Bind(wxEVT_BUTTON, &ChartFrame::MoveNorth<wxCommandEvent>, this);
    button_down->Bind(wxEVT_BUTTON, &ChartFrame::MoveSouth<wxCommandEvent>, this);
    button_left->Bind(wxEVT_BUTTON, &ChartFrame::MoveWest<wxCommandEvent>, this);
    button_right->Bind(wxEVT_BUTTON, &ChartFrame::MoveEast<wxCommandEvent>, this);
    button_reset->Bind(wxEVT_BUTTON, &ChartFrame::ResetRenderAnimate<wxCommandEvent>, this);
    button_reset->SetToolTip("Reset the chart");

    //bind all the elemetns of *this to KeyDown method
    Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    panel->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    draw_panel->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);
    projection->Bind(wxEVT_KEY_DOWN, &ChartFrame::KeyDown<wxKeyEvent>, this);

    draw_panel->Bind(wxEVT_KEY_DOWN, &DrawPanel::KeyDown, draw_panel);
    panel->Bind(wxEVT_KEY_DOWN, &DrawPanel::KeyDown, draw_panel);

    draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, draw_panel);
    draw_panel->Bind(wxEVT_RIGHT_DOWN, &DrawPanel::OnMouseRightDown, draw_panel);
    draw_panel->Bind(wxEVT_LEFT_DOWN, &DrawPanel::OnMouseLeftDown, draw_panel);
    draw_panel->Bind(wxEVT_LEFT_UP, &DrawPanel::OnMouseLeftUp, draw_panel);
    draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseDrag, draw_panel);
    draw_panel->Bind(wxEVT_MOUSEWHEEL, &DrawPanel::OnMouseWheel, draw_panel);

    slider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, &ChartFrame::OnScroll<wxCommandEvent>, this);
    slider->Bind(wxEVT_LEFT_DOWN, &ChartFrame::OnMouseLeftDownOnSlider<wxMouseEvent>, this);
    slider->Bind(wxEVT_LEFT_UP, &ChartFrame::OnMouseLeftUpOnSlider<wxMouseEvent>, this);
    slider->SetToolTip(wxString("Zoom in or out"));

    empty_text_1 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_2 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_3 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_4 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
    empty_text_5 = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);

    //when the ChartFrame is initialized with projection_in = "", I choose to draw either the Mercator or the 3D chart, by reading the name of the projection from file_init. I set the value of projection->name to either of these,
    if (projection_in == Projection("")) {
        //if the constructor has been called with an empty projection_in, I use the default projection by reading it from the init file.

        default_projection.read_from_file_to(String("default projection"), (wxGetApp().path_file_init), String("R"), String(""));
        (projection->name)->SetValue(wxString(default_projection.value));

    }
    else {
        //if the construtor has been called with projection_in non-empty, I set projection_in equal to projection_in

        (projection->name)->SetValue(wxString(projection_in.value));

    }
    projection->value_before_editing = projection->name->GetValue();

    //create a dummy_event and then call SetProjection(dummy_event) to set all objects according to the choice of the projeciton above.
    //    draw_panel->OnChooseProjection(dummy_event);
    draw_panel->SetProjection(dummy_event);

    //stores the x_min .. y_max, (size_chart.GetWidth()), height chart the first time that the chart is shown into x_min_0 ... height_chart_0
    (draw_panel->*(draw_panel->Set_x_y_min_max))();
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);
    (draw_panel->x_span_0) = (draw_panel->x_span());
    
    ResetRender(dummy_event);

    (draw_panel->width_chart_0) = (draw_panel->size_chart.GetWidth());
    (draw_panel->height_chart_0) = (draw_panel->size_chart.GetHeight());


    sizer_buttons->Add(empty_text_1, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_up, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_2, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_left, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_3, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_right, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_4, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(button_down, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_buttons->Add(empty_text_5, 0, wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

    sizer_slider->Add(slider, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_slider->Add(text_slider, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    observer_height->LengthField<ChartFrame>::InsertIn(sizer_slider, flags);
    sizer_slider->Add(sizer_buttons, 0, wxALIGN_CENTER | wxALL, 0);
    sizer_slider->Add(button_reset, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    projection->InsertIn(sizer_slider, flags);
    sizer_slider->AddStretchSpacer(1);
    sizer_slider->Add(button_show_list, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));


    sizer_h->Add(draw_panel, 0, wxALIGN_TOP | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_h->Add(sizer_slider, 0, wxALIGN_TOP | wxALL | wxEXPAND, 0);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    //	sizer_v->Add(text_position_now, 0, wxALIGN_LEFT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
        //    sizer_v->Fit(panel);

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_v);
    panel->Fit();
    Fit();

    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //    CenterOnScreen();

}

//when a ChartFrame is closed, I destroy it, delete the respecive item in parent->chart_frames vector, and rename all the other ChartFrames in that vector to take into account the shifting of the CartFrame ids due to the Chartframe deletion
template<class T> void ChartFrame::OnPressCtrlW(T& event) {

    vector<ChartFrame*>::iterator i;
    unsigned int j;
    stringstream s;

    i = find((parent->chart_frames).begin(), (parent->chart_frames).end(), this);


    Destroy();  // you may also do:  event.Skip();

    (parent->chart_frames).erase(i);
    for (j = 0; j < (parent->chart_frames).size(); j++) {

        s.str("");
        s << "Chart #" << j + 1 << " - " << ((((parent->chart_frames)[j])->projection)->name)->GetValue() << " projection";

        ((parent->chart_frames)[j])->SetLabel(wxString(s.str().c_str()));

    }

    //given that one ChartFrame is gone, I show all ChartFrames and positions them properly on the screen
    wxGetApp().ShowCharts(event);
    wxGetApp().AnimateCharts();

}

//moves (makes slide) to the north the chart
template<class T> void ChartFrame::MoveNorth(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;


//    switch (((projection->name->GetValue()).ToStdString())[0]) {
    switch (position_in_vector(Projection((projection->name->GetValue()).ToStdString()), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;
        PositionProjection p_ceil_min, p_floor_max;

        //I set delta as a fraction of y_max - y_min
        delta = ((wxGetApp().relative_displacement).value) * ((draw_panel->y_max) - (draw_panel->y_min));

        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);


        if (((draw_panel->y_max) + delta < (p_floor_max.y)) && ((draw_panel->y_min) + delta > (p_ceil_min.y))) {
            //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart

            //update y_min, y_max according to the drag.
            (draw_panel->y_min) += delta;
            (draw_panel->y_max) += delta;

            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();


        }

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.b) += delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);



        break;

    }


    }

    //re-draw the charton
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//moves (makes slide) to the south the chart
//moves (makes slide) up the chart
template<class T> void ChartFrame::MoveSouth(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;

    switch (position_in_vector(Projection((projection->name->GetValue()).ToStdString()), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;
        PositionProjection p_ceil_min, p_floor_max;

        //I set delta as a fraction of y_max - y_min
        delta = ((wxGetApp().relative_displacement).value) * ((draw_panel->y_max) - (draw_panel->y_min));

        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);


        if (((draw_panel->y_max) - delta < (p_floor_max.y)) && ((draw_panel->y_min) - delta > (p_ceil_min.y))) {
            //if the movement operation does not bring the chart out of the min and max latitude contained in the data files, I update y_min, y_max and update the chart

            //update y_min, y_max according to the drag.
            (draw_panel->y_min) -= delta;
            (draw_panel->y_max) -= delta;

            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();


        }

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.b) -= delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);

        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}

//moves (makes slide) to the west the chart
template<class T> void ChartFrame::MoveWest(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;


    switch (position_in_vector(Projection((projection->name->GetValue().ToStdString())), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;

        delta = ((wxGetApp().relative_displacement).value) * (draw_panel->x_span());

        //update lambda_min, lambda_max according to the drag.
        (lambda_min.value) += delta;
        (lambda_max.value) += delta;

        lambda_min.normalize();
        lambda_max.normalize();

        (draw_panel->*(draw_panel->Set_x_y_min_max))();

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.a) -= delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);



        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;

    event.Skip(true);

}



//if a key is pressed in the keyboard, I call this function
void DrawPanel::KeyDown(wxKeyEvent& event) {
    
    switch (event.GetKeyCode()) {
            
        case WXK_UP:
            
            parent->MoveNorth<wxKeyEvent>(event);
            
            break;
            
        case WXK_DOWN:
            
            parent->MoveSouth<wxKeyEvent>(event);
            
            break;
            
        case WXK_LEFT:
            
            parent->MoveWest<wxKeyEvent>(event);
            
            break;
            
        case WXK_RIGHT:
            
            parent->MoveEast<wxKeyEvent>(event);
            
            break;
            
        case WXK_ESCAPE:
            
            if(parent->parent->selection_rectangle){
                //If the user presses esc, I cancel the selection process with the rectangle in all ChartFrames, re-enable button_reset in all ChartFrames (because button_reset had been disabled as one started drawing selection_rectangle), and call RefreshAll and FitAll to re-draw the chart without the selection rectangle

                (parent->parent->selection_rectangle) = false;
                
                for(unsigned int i=0; i<parent->parent->chart_frames.size(); i++){
                    parent->parent->chart_frames[i]->button_reset->Enable(true);
                    
                }
            
            }
            
            
            (parent->parent->start_label_selection_rectangle) = String("");
            (parent->parent->end_label_selection_rectangle_now) = String("");
            (parent->parent->end_label_selection_rectangle_before) = String("");
            
            parent->parent->RefreshAll();
            FitAll();
            
            break;
            
        case WXK_PLUS:
            //the + key is pressed and control is pressed too -> I zoom in by multiplying the slider value by 2
            
            if (event.ControlDown()) {
                parent->SetSlider(((parent->slider)->GetValue()) * 2);
            }
            
            break;
            
            
        case WXK_MINUS:
            //the - key is pressed and control is pressed too -> I zoom out by dividing the slider value by 2
            
            if (event.ControlDown()) {
                parent->SetSlider(round(((parent->slider)->GetValue()) / 2.0));
            }
            
            break;
            
    }
    
    //    }
    
    event.Skip(true);
    
}


//moves (makes slide) to the east the chart
template<class T> void ChartFrame::MoveEast(T& event) {

    //I am dragging the chart and the size of *this will not change -> set 
    dragging_chart = true;

    switch (position_in_vector(Projection((projection->name->GetValue().ToStdString())), Projection_types)) {

    case 0: {
        //I am using the mercator projection


        double delta;

        delta = ((wxGetApp().relative_displacement).value) * (draw_panel->x_span());

        //update lambda_min, lambda_max according to the drag.
        (lambda_min.value) -= delta;
        (lambda_max.value) -= delta;

        lambda_min.normalize();
        lambda_max.normalize();

        (draw_panel->*(draw_panel->Set_x_y_min_max))();

        break;

    }


    case  1: {
        //I am using the 3d projection

        Angle /*the angular displacement of the operation MoveNorth*/delta;

        //I set delta as a fraction of circle_obsrever.omega
        delta = ((draw_panel->circle_observer).omega) * ((wxGetApp().relative_displacement).value);

        //since I am moving north, I increase the b Euler ancgle of rotation
        (draw_panel->rotation.a) += delta;
        //I update rotation->matrix
        draw_panel->rotation.set(draw_panel->rotation.a, draw_panel->rotation.b, draw_panel->rotation.c);



        break;

    }


    }

    //re-draw the chart
    (draw_panel->*(draw_panel->PreRender))();
    draw_panel->Refresh();
    draw_panel->FitAll();

    //I stopped dragging the chart -> set
    dragging_chart = false;


    event.Skip(true);

}

//this method is called when a key is pressed
template<class T> void ChartFrame::KeyDown(T& event) {

    if (wxGetKeyState(WXK_CONTROL)) {
        //the command key (APPLE operating system) or the control key (WIN32 operating system) is down

        switch ((event.GetUnicodeKey())) {

        case 'Q': {
            //command (APPLE) or control (WIN32) + q key has been pressed

            wxGetApp().OnPressCtrlQ(event);

            break;

        }

        case 'W': {
            //command (APPLE) or control (WIN32) + w key has been pressed

            parent->OnPressCtrlW(event);

            break;

        }

        case 'S': {

            if (wxGetKeyState(WXK_SHIFT)) {

                //command (APPLE) or control (WIN32) + shift + s key has been pressed

                parent->OnPressCtrlShiftS(event);
            }
            else {
                //command (APPLE) or control (WIN32) + s key has been pressed

                parent->OnPressCtrlS(event);

            }

            break;

        }

        }

    }


    event.Skip(true);

}


//reset the chart to its starting configuration for x_min ... y_max
template<class T> void ChartFrame::Reset(T& event) {

    idling = false;
    (draw_panel->idling) = false;

    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        //read lambda_min, ...., phi_max from file_init
        lambda_min.read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
        lambda_max.read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
        phi_min.read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
        phi_max.read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
        draw_panel->Set_x_y_min_max_Mercator();
        ComputeZoomFactor_Mercator(draw_panel->x_span());

        //reset the chart boundaries to the initial ones
        //        (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();

    }

    if ((projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {
        //reset d abd the earth orientation to the initial one and set the zoom factor accordingly

        parent->circle_observer_0.omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), String(""));
        zoom_factor.set(String(""), 1.0, String(""));
        ComputeZoomFactor_3D();

        (draw_panel->rotation_0).read_from_file_to(String("rotation 0"), (wxGetApp().path_file_init), String("R"), String(""));
        draw_panel->rotation.set(draw_panel->rotation_0);
        draw_panel->Set_x_y_min_max_3D();
        (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();

    }

    //now that x_min ... y_max have been set, I set x_min_0 ... equal to x_min ...
    (draw_panel->x_min_0) = (draw_panel->x_min);
    (draw_panel->x_max_0) = (draw_panel->x_max);
    (draw_panel->y_min_0) = (draw_panel->y_min);
    (draw_panel->y_max_0) = (draw_panel->y_max);

 

    event.Skip(true);

}


//call Reset and Render everything
template<class T> void ChartFrame::ResetRender(T& event) {
    
    Reset<T>(event);
    
    (draw_panel->*(draw_panel->PreRender))();
     //now that (size_chart.GetWidth()) and (size_chart.GetHeight()) have been set, I set width_chart_0 and height_chart_0 equal to width_chart and (size_chart.GetHeight())
     (draw_panel->width_chart_0) = (draw_panel->size_chart.GetWidth());
     (draw_panel->height_chart_0) = (draw_panel->size_chart.GetHeight());

     draw_panel->CleanAndRenderAll();
     
     draw_panel->FitAll();
     UpdateSlider();
//     Animate();
    
    event.Skip(true);

}


template<class T> void ChartFrame::ResetRenderAnimate(T& event) {
    
    ResetRender(event);
    Animate();

}




//makes a nice animation to present *this, by dragging the center of the chart to the desired Position from a Position on the antipodes
void ChartFrame::Animate(void){
    
    //the transport handler used to transport the chart in *this
    ChartTransportHandler< UnsetIdling<ListFrame> >* chart_transport_handler;
        
    //allocate chart_transport_handler and set the starting Position and the Route for the transport
    switch (position_in_vector(Projection((projection->name->GetValue().ToStdString())), Projection_types)) {
            
        case 0: {
            //I am using Projection_types[0]
            
            PositionProjection q_NE, q_SW, q_center;
            Position p;
            
            q_NE.NormalizeAndSetMercator(Position(lambda_max, phi_max));
            q_SW.NormalizeAndSetMercator(Position(lambda_min, phi_min));
            q_center = (q_NE + q_SW)/2;
            
            (draw_panel->*(draw_panel->ProjectionToGeo))(q_center, &p);
            
            chart_transport_handler = new ChartTransportHandler< UnsetIdling<ListFrame> >(
                                                                this,
                                                                Route(
                                                                      Route_types[0],
                                                                      p.antipode_lambda(),
                                                                      p
                                                                      ),
                                                                Double(1.0),
                                                                      parent->unset_idling
                                                                );

            //trigger the animation
            chart_transport_handler->operator()();
            
            
            break;
            
        }
            
        case 1: {
            //I am using Projection_types[1]
            
            chart_transport_handler = new ChartTransportHandler< UnsetIdling<ListFrame> >(
                                                                this,
                                                                Route(
                                                                      Route_types[1],
                                                                      draw_panel->circle_observer.reference_position.half(),
                                                                      draw_panel->circle_observer.reference_position
                                                                      ),
                                                                Double(1.0),
                                                                      parent->unset_idling
                                                                );

            //trigger the animation
            chart_transport_handler->operator()();
            
            break;
            
        }
            
    }
        
}




//makes an animation which centers the chart on the object *object_in (which may be a Route, Position, ...) and adjust the chart zoom factor in such a way that *object_in is nicely visible at the end of the animation. Here f is the functor of the function that will be called at the end of the animation, and it is entered into the constructor of ChartTransrportHandler. If no functor is to be called at the end of the animation, one may let f point to a UnsetIdling<> functoer
template<class T, class F> void ListFrame::AnimateToObject(T* object_in, F* f){
    
    unsigned int i;
    //the Position where the chart will be centered by the animation triggered when the user presses ok
    Position target_position;
    //the aperture angle of circle_observer at the end of the animation
    Angle target_omega;
    //the transport handlers used to transport the chart: there is one ChartTransportHandler per ChartFrame
    vector< ChartTransportHandler<F>* > chart_transport_handlers;
    Length d;
    
    chart_transport_handlers.resize(chart_frames.size());
    
    
    //bring all charts to front to show the animation
    wxGetApp().ShowCharts();

    
    for(i=0; i<chart_frames.size(); i++){
        
        switch (position_in_vector(Projection(((chart_frames[i])->projection->name->GetValue().ToStdString())), Projection_types)) {
                
            case 0: {
                //I am using Projection_types[0]
                
                //the center of the Mercator projection
                Position center;
                //the size of the object to which the animation is directed, in units of the coordinates x, y of the Mercator projection
                PositionProjection target_size, size_rectangle_observer_0;
                Double zoom_factor;
                
                if(std::is_same<T, Route>::value){
                    //object is a Route
                    
                    //I introduce the Route* object and set object_in = object by casting object_in into a Route pointer. This is necessary to make this method work with multiple types T (T=Position, T=Route, ...)
                    Route* object;
                    
                    object = (Route*)object_in;
                    
                   if(object->type == Route_types[2]){
                       //*route is a circle of equal altiutde -> at the end of the animation, the chart must be centered at the center of the circle of equal altitude, i.e., at reference_position. target_omega is given by the aperture angle of the circle of equal altitude, i.e., route.omega
                       
                       target_position = object->reference_position;
                       //                       target_omega = object->omega;
                       
                       
                   }else{
                       //*route is a loxodrome or an orthodrome -> at the end of the animaiton, the chart must be centered at the middle point of *route for *route to be visible at the end of the animation. The aperture angle is estimated as half the length of *route divided by the radius of the Earth
                       
                       object->set_length_from_time_speed();
                       
                       object->compute_end(Length((object->length)/2.0), String(""));
                       target_position = object->end;
                       
                   }
                    
                    //compute the size of the object to which the animation is directed and store it into target_size
                    object->size_Mercator(&target_size);
                    
                }
                
                if(std::is_same<T, Position>::value){
                    //object is a Position
                    
                    Position* object;
                    
                    object = ((Position*)object_in);
                    
                    //the target Position of the animation is *object
                    target_position = (*object);
                    //Positions do not have a size such as Routes -> I move the chart on the Position with the animation by keeping the same omega as in the beginnign of the animation
//                    target_omega = ((chart_frames[i])->draw_panel->circle_observer.omega);
                    
                }
                
                //compute the distance between the start and end poisition of the proposed andimation and store it in d
                //                //I do the animaiton only if the start and end position of the animation are large enough, in order to avoid NaNs in the transporting_route
                //                if (d > (wxGetApp().minimal_animation_distance_over_size_of_observer_region.value) * Re*(chart_frames[i])->draw_panel->circle_observer.omega.value) {
                
                center.lambda.set(String(""), (((chart_frames[i])->lambda_min + (chart_frames[i])->lambda_max).value)/2.0, String(""));
                center.phi.set(String(""), mean_pm_pi((chart_frames[i])->phi_min, (chart_frames[i])->phi_max), String(""));
                
                rectangle_observer_0.SizeMercator(&size_rectangle_observer_0);
                
                zoom_factor.value = (wxGetApp().chart_transport_zoom_factor_coefficient.value) * min((size_rectangle_observer_0.x)/(target_size.x), (size_rectangle_observer_0.y)/(target_size.y));
                
                
                chart_transport_handlers[i] = new ChartTransportHandler<F>(
                                                                           (chart_frames[i]),
                                                                           Route(
                                                                                 Route_types[0],
                                                                                 center,
                                                                                 target_position
                                                                                 ),
                                                                           zoom_factor,
                                                                           f
                                                                           );
                
                //trigger the animation
                (chart_transport_handlers[i])->operator()();
                
                
                //                }
                
                
                
                break;
                
            }
                
            case 1: {
                //I am using Projection_types[1]
                
                //the aperture angle of circle_observer at the end of the animation
                Angle omega_end;
                
                
                if(std::is_same<T, Route>::value){
                    //object is a Route
                    
                    //I introduce the Route* object and set object_in = object by casting object_in into a Route pointer. This is necessary to make this method work with multiple types T (T=Position, T=Route, ...)
                    Route* object;
                    
                    object = (Route*)object_in;
                    
                   if(object->type == Route_types[2]){
                       //*route is a circle of equal altiutde -> at the end of the animation, the chart must be centered at the center of the circle of equal altitude, i.e., at reference_position. target_omega is given by the aperture angle of the circle of equal altitude, i.e., route.omega
                       
                       target_position = object->reference_position;
                       omega_end = object->omega;
                       
                       
                   }else{
                       //*route is a loxodrome or an orthodrome -> at the end of the animaiton, the chart must be centered at the middle point of *route for *route to be visible at the end of the animation. The aperture angle is estimated as half the length of *route divided by the radius of the Earth
                       
                       object->set_length_from_time_speed();
                       
                       object->compute_end(Length((object->length)/2.0), String(""));
                       target_position = object->end;
                       //                    target_position = route.reference_position;

                       omega_end = (object->length.value)/2.0/Re;
                       
                   }
                    
                }
                
                if(std::is_same<T, Position>::value){
                    //object is a Position
                    
                    Position* object;
                    
                    object = ((Position*)object_in);
                    
                    //the target Position of the animation is *object
                    target_position = (*object);
                    //Positions do not have a size such as Routes -> I move the chart on the Position with the animation by keeping the same omega as in the beginnign of the animation
                    omega_end = ((chart_frames[i])->draw_panel->circle_observer.omega);
                    
                }
            
                //compute the distance between the start and end poisition of the proposed andimation and store it in d
                target_position.distance((chart_frames[i])->draw_panel->circle_observer.reference_position, &d, String(""), String(""));
                
                //I do the animaiton only if the start and end position of the animation are large enough, in order to avoid NaNs in the transporting_route
                if (d > (wxGetApp().minimal_animation_distance_over_size_of_observer_region.value) * Re*(chart_frames[i])->draw_panel->circle_observer.omega.value) {
                    
                    chart_transport_handlers[i] = new ChartTransportHandler<F>(
                                                                               (chart_frames[i]),
                                                                               Route(
                                                                                     Route_types[1],
                                                                                     (chart_frames[i])->draw_panel->circle_observer.reference_position,
                                                                                     target_position
                                                                                     ),
                                                                               Double( ((wxGetApp().chart_transport_zoom_factor_coefficient.value) *  (circle_observer_0.omega.value) / (omega_end.value) ) ),
                                                                               f
                                                                               );
                    
                    //trigger the animation
                    (chart_transport_handlers[i])->operator()();
                    
                    
                }
                
                break;
                
            }
                
        }
        
    }
    
    
}


void DrawPanel::SetIdling(bool b) {

    idling = b;

}

//this function computes lambda_min, ... phi_max from x_min ... y_max for the mercator projection
void DrawPanel::Set_lambda_phi_min_max_Mercator(void) {

    (parent->lambda_min).set(String(""), k * lambda_mercator(x_min), String(""));
    (parent->lambda_max).set(String(""), k * lambda_mercator(x_max), String(""));

    (parent->phi_min).set(String(""), k * phi_mercator(y_min), String(""));
    (parent->phi_max).set(String(""), k * phi_mercator(y_max), String(""));

}

//this function computes lambda_min, ... phi_max (the  min/max latitudes and longitudes which encompass circle_observer) for the 3D projection
void DrawPanel::Set_lambda_phi_min_max_3D(void) {

    //compute circle_observer
    //    (circle_observer.omega).set(String(""), atan( sqrt(1.0 - gsl_pow_2(1.0/(1.0+(d.value))))/(1.0/(1.0+(d.value))) ), String(""));

    //consider the vector rp = {0,-1,0}, corresponding to the center of the circle of equal altitude above
    gsl_vector_set((rp.r), 0, 0.0);
    gsl_vector_set((rp.r), 1, -1.0);
    gsl_vector_set((rp.r), 2, 0.0);

    //convert rp -> r through rotation^{-1}
//        gsl_blas_dgemv(CblasTrans, 1.0, (rotation).matrix, (rp.r), 0.0, (r.r));
    cblas_dgemv(CblasRowMajor, CblasTrans, 3, 3, 1, rotation.matrix->data, 3, rp.r->data, 1, 0, r.r->data, 1);


    //obtain the  geographic position of the center of the circle of equal altitude above
    circle_observer.reference_position.setCartesian(String(""), r, String(""));


    //set lambda_min/max from circle_observer
    circle_observer.lambda_min_max(&(parent->lambda_min), &(parent->lambda_max), String(""));

    //set
    d.set(String(""), (-1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))))*Re, String(""));
    parent->observer_height->set();

    //set phi_min/max
    ((circle_observer.reference_position).phi).normalize_pm_pi();

    if (((((circle_observer.reference_position).phi).value) + ((circle_observer.omega).value) < M_PI_2) &&
        ((((circle_observer.reference_position).phi).value) - ((circle_observer.omega).value) > -M_PI_2)) {
        //in this case, circle_observer does not encircle the N/S pole

        (parent->phi_min) = ((circle_observer.reference_position).phi) - (circle_observer.omega);
        (parent->phi_max) = ((circle_observer.reference_position).phi) + (circle_observer.omega);

    }
    else {

        if ((((circle_observer.reference_position).phi).value) + ((circle_observer.omega).value) > M_PI_2) {
            //in this case, circle_observer encircles the N pole

            (parent->phi_min) = ((circle_observer.reference_position).phi) - (circle_observer.omega);
            (parent->phi_max).set(String(""), M_PI_2, String(""));

        }

        if ((((circle_observer.reference_position).phi).value) - ((circle_observer.omega).value) < -M_PI_2) {
            //in this case, circle_observer encircles the S pole

            (parent->phi_min).set(String(""), 3.0 * M_PI_2, String(""));
            (parent->phi_max) = ((circle_observer.reference_position).phi) + (circle_observer.omega);

        }

    }

    ((circle_observer.reference_position).phi).normalize();

}


//this function computes x_min, ... y_max and from lambda_min ... phi_max in the Mercator projection
void DrawPanel::Set_x_y_min_max_Mercator(void) {
    
    PositionProjection p_min, p_max;
    Position temp;
    
    //    (this->*GeoToProjection)(Position(parent->lambda_min, parent->phi_min), &p_min, true);
    //    (this->*GeoToProjection)(Position(parent->lambda_max, parent->phi_max), &p_max, true);
    
    temp = Position(parent->lambda_min, parent->phi_min);
    //    (p_min.x) = -(((temp.lambda).normalize_pm_pi_ret()).value);
    //    (p_min.y) = log(1.0 / cos((temp.phi)) + tan((temp.phi)));
    p_min.NormalizeAndSetMercator(temp);
    
    temp = Position(parent->lambda_max, parent->phi_max);
    //    (p_max.x) = -(((temp.lambda).normalize_pm_pi_ret()).value);
    //    (p_max.y) = log(1.0 / cos((temp.phi)) + tan((temp.phi)));
    p_max.NormalizeAndSetMercator(temp);
    
    
    x_min = (p_min.x);
    y_min = (p_min.y);
    x_max = (p_max.x);
    y_max = (p_max.y);

}

//sets size_chart for the Mercator projection
void DrawPanel::Set_size_chart_Mercator(void) {

    if ((y_max - y_min) > x_span()) {

        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object

        size_chart.SetHeight((length_chart_over_length_chart_frame.value) * (((wxGetApp().rectangle_display).GetSize()).GetHeight()));
        size_chart.SetWidth((size_chart.GetHeight()) / ((y_max - y_min) / x_span()));
    }
    else {
        //set the height and width of chart with the correct aspect ratio, and both similtaneously rescaled with respect to the size of the ChartFrame objest, in such a way that the chart fits into the ChartFrame object

        size_chart.SetWidth((length_chart_over_length_chart_frame.value) * (((wxGetApp().rectangle_display).GetSize()).GetHeight()));
        size_chart.SetHeight((size_chart.GetWidth()) * ((y_max - y_min) / x_span()));

    }

}

//set size_chart for the 3D projection
void DrawPanel::Set_size_chart_3D(void) {

    /*size_chart.SetHeight(((parent->GetSize()).GetHeight()) * 0.75);
    size_chart.SetWidth((size_chart.GetHeight()));*/


    size_chart.SetHeight((length_chart_over_length_chart_frame.value) * (((wxGetApp().rectangle_display).GetSize()).GetHeight()));
    size_chart.SetWidth((size_chart.GetHeight()));


}

/*returns a double: the width of the chart wich takes into account the fact that x_min and x_max may encompass the meridian lambda = pi*/
double DrawPanel::x_span(void) {

    if (x_max >= x_min) {
        //in this case, x_max, x_min do not encompass the meridian lambda = pi
        return(x_max - x_min);
    }
    else {
        //in this case, x_max, x_min encompass the meridian lambda = pi
        return(2.0 * M_PI - (x_min - x_max));
    }

}

//this function computes x_min, ... y_max from d in the 3D projection
void DrawPanel::Set_x_y_min_max_3D(void) {

    Double d_temp;

    //set d
    d_temp.set(String(""), -1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))), String(""));

    x_min = -((d_temp.value) / sqrt(gsl_pow_2(((d_temp).value) + 1.0) - 1.0));
    x_max = -x_min;
    y_min = x_min;
    y_max = -y_min;

}

//puts point *p which lies outside the plot area, back into the plot area . It returns true if p is in the plot area, and false otherwise
bool DrawPanel::PutBackIn(wxPoint q, wxPoint* p) {

    bool output;

    output = true;

    (*p) = q;

    if ((p->x) < (position_plot_area_now.x)) {
        (p->x) = (position_plot_area_now.x);
        output = false;
    }

    if ((p->x) > (position_plot_area_now.x) + (size_plot_area.GetWidth())) {
        (p->x) = (position_plot_area_now.x) + (size_plot_area.GetWidth());
        output = false;
    }

    if ((p->y) < (position_plot_area_now.y)) {
        (p->y) = (position_plot_area_now.y);
        output = false;
    }

    if ((p->y) > (position_plot_area_now.y) + (size_plot_area.GetHeight())) {
        (p->y) = (position_plot_area_now.y) + (size_plot_area.GetHeight());
        output = false;
    }

    return output;

}


//generate a Rotation from the two points start and end (which are referred to the origin of the screen) in the 3D projection.
Rotation DrawPanel::rotation_start_end(const wxPoint& start, const wxPoint& end) {

    Position temp;
    Position p_start, p_end;

    //call ScreenToGeo_3D to generate rp, and then convert rp into spherical coordinates by writing it into p_start
    ScreenToGeo_3D(start, &temp);
    p_start.setCartesian(String(""), rp, String(""));

    //call ScreenToGeo_3D to generate rp, and then convert rp into spherical coordinates by writing it into p_end
    ScreenToGeo_3D(end, &temp);
    p_end.setCartesian(String(""), rp, String(""));

    //construct a Rotation between p_start and p_end by calling the overloaded constructor of the Rotation class
    return(Rotation(p_start, p_end));

}



//updates the text in text_slider by writing in it the current value of the zoom factor, rounded to an integer for clarity
void ChartFrame::UpdateSliderLabel_Mercator(void) {

    stringstream s;
    Int scale_factor;
    
    //compute the scale factor
    scale_factor.set( ((unsigned int)(
                   /*length of the NS edge of the plot area as measured on the surface of the earth, in  nm*/(((phi_max.normalize_pm_pi_ret().value) - (phi_min.normalize_pm_pi_ret().value)) * K * 60.0) / ( /*length of the NS edge of the plot area as shown on the screen of the computer, in nm*/((double)(draw_panel->size_plot_area.y))/((double)(wxGetApp().display.GetPPI().x)) * my_inch/nm ) )) );
    
    
    scale_factor.my_round(display_precision);
    

    s.str("");
    s << "1:" << scale_factor.to_string_spaces().value;
    text_slider->SetLabel(s.str().c_str());
    
    //fir *this in order to account for the sliderlabal which has changed 
    Fit();
    

}

//updates the text in text_slider by writing in it the ration between circle_observer.omega and circle_observer_0.omega
void ChartFrame::UpdateSliderLabel_3D(void) {

    stringstream s;

    s << draw_panel->d.value << " nm";

    text_slider->SetLabel(s.str().c_str());

}

//computes the zoom factor of the chart based on the currenct value of span_x. It returns true and writes the value in zoom_factor if the zooming factor is smaller than (wxGetApp().zoom_factor_max), and returns false otherwise
bool ChartFrame::ComputeZoomFactor_Mercator(double delta_x) {

    double temp;
    bool output;

    temp = ((double)((draw_panel->size_chart).GetWidth())) / ((double)(draw_panel->width_chart_0)) * ((draw_panel->x_max_0) - (draw_panel->x_min_0)) / delta_x;

    output = ((1.0 <= temp) && (temp <= (wxGetApp().zoom_factor_max.value)));

    if (output) {
        zoom_factor.set(String(""), temp, String(""));
    }

    return(output);

}

//returns true if zoom_factor is valid, false otherwise
bool ChartFrame::ComputeZoomFactor_3D(void) {

    bool output;

    output = ((1.0 <= (zoom_factor.value)) && ((zoom_factor.value) <= (wxGetApp().zoom_factor_max.value)));

    if (output) {

        ((draw_panel->circle_observer).omega).set(String(""), (parent->circle_observer_0.omega.value) / (zoom_factor.value), String(""));

    }

    return(output);

}

//this function updates the slider according to the zoom factor of the chart: it sets the slider value to the integer value closest to zoom_factor
void ChartFrame::UpdateSlider(void) {

    int temp;

    //compute the zoom factor of the chart and write it into zoom_factor

    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        ComputeZoomFactor_Mercator((draw_panel->x_span()));

    }

    if ((projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {
        //is this necessary here ?
        ComputeZoomFactor_3D();

    }

    //a tentative value for the value of slizer
    temp = round(pow(((zoom_factor.value) - ((wxGetApp().b_zoom).value)) / ((wxGetApp().a_zoom).value), 1.0 / ((wxGetApp().e_zoom).value)));


    //if the tentative value exceeds the slider boundaries, I set it to the respective boundary
    if (temp > (slider->GetMax())) { temp = (slider->GetMax()); }
    if (temp < 1) { temp = 1; }

    //    cout << "\t\t\ttemp = " << temp << "\n";
    //    cout << "\t\t\tzoom_factor = " << (zoom_factor.value) << "\n";

    slider->SetValue(temp);

    (this->*UpdateSliderLabel)();

}


template<class P> template<class T>void CheckBody<P>::operator()(T& event) {

    SightFrame* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        //I check whether the name in the GUI field body matches one of the body names in catalog
        for (check = false, i = 0; (i < (p->catalog->list).size()) && (!check); i++) {
            if (String((p->name->GetValue().ToStdString())) == (((p->catalog->list)[i]).name)) {
                check = true;
            }
        }
        i--;

        if (check || (((p->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->name->GetValue()).ToStdString())) == String("")))) {
            //p->check either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            if (check) {

                if (((p->catalog->list)[i].name == String("sun")) || ((p->catalog->list)[i].name == String("moon"))) {
                    //in this case, the selected body is a body which has a limb -> I enable the limb field

                    f->limb->name->Enable(true);

                }
                else {
                    //in this case, the selected body is a body which has no limb -> I disable the limb field and set limb->ok to true (because the limb is unumportant here, so it can be considered to be ok)

                    f->limb->name->Enable(false);
                    (f->limb->ok) = true;

                }

                //insert body #i into data->recent_bodies
//                wxGetApp().list_frame->data->insert_recent_body(i);
                wxGetApp().list_frame->data->insert_recent_item(i, &(wxGetApp().list_frame->data->recent_bodies));
                //I update p->name according to the content of data->recent_bodies file
                p->Fill();

            }

            //if check is true (false) -> set ok to true (false)
            (p->ok) = check;
            //the background color is set to wxGetApp().foreground_color and the font to default_font, because in this case there is no erroneous value in name. I call Reset to reset the font colors of the items in the list to their default values
            p->name->SetForegroundColour(wxGetApp().foreground_color);
            p->name->SetFont(wxGetApp().default_font);
            Reset(p->name);

        }
        else {

            f->print_error_message->SetAndCall(p->name, String("Error"), String("Body not found in catalog! Body must be in catalog."), (wxGetApp().path_file_error_icon));

            (p->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckLimb<P>::CheckLimb(LimbField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckLimb<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        
        check = ((p->checked_items.GetCount()) == 1);

        if (check || (((p->name->GetForegroundColour()) != (wxGetApp().error_color)) && ((p->checked_items.GetCount()) == 0))) {
            //p->name either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set ok to true (false)
            (p->ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in name
            p->name->SetForegroundColour(wxGetApp().foreground_color);
            p->name->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall(p->name, String("Error"), String("Limb not valid! Limb must be upper, center or lower."), (wxGetApp().path_file_error_icon));
            (p->ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


//writes the value contained in the GUI field into the non-GUI field
template<class P> template<class T> void LimbField<P>::get(T& event) {

    if (ok) {

        if (name->IsEnabled()) {
            //if the limb is ok and the limb wxComboBox is enabled, I set the char in (limb->value) to the first letter in the string contained in the GUI field

            //            (limb->value) = ((String((name->GetValue().ToStdString()))).value)[0];
            (limb->value) = ((String(((limbs[checked_items.Item(0)]).ToStdString()))).value)[0];

        }
        else {
            //if the limb is ok and the limb wxComboBox is disabled, then the limb is irrelevant, and I set the char in limb->value to the null char.

            (limb->value) = '\0';

        }
    }

    event.Skip(true);

}

template<class P> CheckSign<P>::CheckSign(AngleField<P>* p_in) {

    p = p_in;

}

//checks the value of the sign in the GUI field
template<class P> template <class T> void CheckSign<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is in idling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        //I check whether the name in the GUI field sign matches one of the sign values in the list signs
        if ((p->format) == String("")) {
            //if the AngleField p has no sign, the check is ok

            check = true;

        }
        else {
            //if the AngleField p has a sign, I check it

            for (check = false, i = 0; (i < ((p->signs).GetCount())) && (!check); i++) {
                if ((p->sign->GetValue()) == (p->signs)[i]) {
                    check = true;
                }
            }

        }


        if (check || (((p->sign->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->sign->GetValue()).ToStdString())) == String("")))) {
            //p->sign either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set sign_ok to true (false)
            (p->sign_ok) = check;

            if ((p->format) != String("")) {
                //there exists a p->sign field

                //the background color is set to white, because in this case there is no erroneous value in sign
                p->sign->SetForegroundColour(wxGetApp().foreground_color);
                p->sign->SetFont(wxGetApp().default_font);
            }

        }
        else {

            f->print_error_message->SetAndCall((p->sign), String("Error"), String("Sign is not valid! Sign must be +-, NS or EW."), (wxGetApp().path_file_error_icon));
            (p->sign_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


//if i needs to be adjusted because it is not between -90 and +90, this method adjusts the pair of latitude, longitude (in arcdegrees) (i, j) and, if the pair is valud with respect to the latitude bounbdaries, it writes the adjusted vlaues in (*i_adjustged, *j_adjusted) and returns true, while it returns false otherwise
inline bool DrawPanel::AdjustLatitudeLongitude3D(const int& i, const int& j, int* i_adjusted, int* j_adjusted){
    
    bool check;
    
    //convert i,j into (*i_adjusted), (*j_adjusted)
    if (!((i >= -90) && (i <= 90))) {
        //in this case, i needs to be adjusted because it is not between -90 and +90
        
        if (i < -90) {
            
            if ((-(180 + i) - floor_min_lat >= 0) && (-(180 + i) - floor_min_lat < (parent->parent->coastline_polygons_Cartesian).size())) {
                
                (*i_adjusted) = -(180 + i);
                (*j_adjusted) = 180 + j;
                
                check = true;
                
            }
            else {
                
                check = false;
                
            }
            
        }
        
        if (i > 90) {
            
            if ((180 - i - floor_min_lat >= 0) && (180 - i - floor_min_lat < (parent->parent->coastline_polygons_Cartesian).size())) {
                
                (*i_adjusted) = 180 - i;
                (*j_adjusted) = 180 + j;
                
                check = true;
                
            }
            else {
                
                check = false;
                
            }
            
        }
        
        
    }
    else {
        
        if ((i - floor_min_lat >= 0) && (i - floor_min_lat < (parent->parent->coastline_polygons_Cartesian).size())) {
            
            (*i_adjusted) = i;
            (*j_adjusted) = j;
            
            check = true;
            
        }
        else {
            
            check = false;
            
        }
        
    }
    
    return check;
    
}


// the screen position p lies within the DrawPanel *this, it returns true and write it into the position q with respect to the DrawPanel *this. Otherwise, it returns alse, and does nothing with q
inline bool DrawPanel::ScreenToDrawPanel(const wxPoint& p, wxPoint* q) {

    bool check;

    check = ((p.x) >= (GetScreenPosition().x)) && ((p.x) <= (GetScreenPosition().x)) + (GetSize().x) && ((p.y) >= (GetScreenPosition().y)) && ((p.y) <= (GetScreenPosition().y)) + (GetSize().y);


    if (check) {
        (*q) = p - GetScreenPosition();
    }

    return check;

}

//if the point p (reckoned with respect to the origin of the screen) corresponds to a valid Position, i.e., it is in the plot area, convert p  into a geographic Position and  write the result into *q if q!=NULL. If p is in the plot area, it returns true and zero otherwise.
inline bool DrawPanel::ScreenToGeo_Mercator(const wxPoint& p, Position* q) {

    PositionProjection temp;
    bool check;

    //updates the position of the DrawPanel *this
    draw_panel_origin = (this->GetScreenPosition());


    check = ScreenToMercator(p, &temp);

    if (check && (q != NULL)) {

        ProjectionToGeo_Mercator(temp, q);
        
    }

    return check;

}

//convert the point p in the DrawPanel coordinates to the relative geographic position q, see specifics of ScreenToGeo_Mercator and ScreenToGeo_3D
inline bool DrawPanel::DrawPanelToGeo(const wxPoint& p, Position* q) {

    //computes the poisition of the DrawPanel *this which will be needed in the following
    draw_panel_origin = (this->GetScreenPosition());

    return(this->*ScreenToGeo)(p + draw_panel_origin, q);

}



//converts the point p on the screen with a 3D projection, to the relative geographic position q (if q!=NULL). It returns true if p lies within the circle denoting the boundaries of the earth, and false otherwise. If false is returned, q is the geographic position on the earth defined as follows: it lies on the intersection between the Earth and the x'z' plane and on the line between the center of the Earth and the vector rp corresponding to p (such vector rp lies on the x'z' plane)
inline bool DrawPanel::ScreenToGeo_3D(const wxPoint& p, Position* q) {

    PositionProjection temp;

    if (ScreenTo3D(p, &temp)) {
        //p lies within the circle of the earth

        if (q != NULL) {

            Double d_temp;

            //here I put the sign of (temp.x) in front of the square root, in order to pick the correct solutio among the two possible solutios for xp, yp. The correct solution is the one yielding the values of xp, yp on the visible side of the sphere. For example, for (temp.x)<0, a simple geometrical construction shows that the solution corresponding to the visible side of the sphere is the one with the larger (temp.x) -> I pick the solution with a positive sign in front of the square root through GSL_SIGN((temp.x))
            //set rp
            d_temp.set(String(""), -1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))), String(""));

            gsl_vector_set((rp.r), 0, (-(temp.x) * sqrt(arg_sqrt) + (d_temp.value) * ((d_temp.value) + 1.0) * (temp.x)) / (gsl_sf_pow_int((d_temp.value), 2) + gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2)));
            gsl_vector_set((rp.r), 2, (-sqrt(arg_sqrt) * (temp.y) + (d_temp.value) * ((d_temp.value) + 1.0) * (temp.y)) / ((gsl_sf_pow_int((d_temp.value), 2) + gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2))));
            gsl_vector_set((rp.r), 1, -sqrt(1.0 - (gsl_pow_2(gsl_vector_get((rp.r), 0)) + gsl_pow_2(gsl_vector_get((rp.r), 2)))));

            //r = (rotation.matrix)^T . rp
            //            gsl_blas_dgemv(CblasTrans, 1.0, rotation.matrix, (rp.r), 0.0, (r.r));
            cblas_dgemv(CblasRowMajor, CblasTrans, 3, 3, 1, rotation.matrix->data, 3, rp.r->data, 1, 0, r.r->data, 1);


            q->setCartesian(String(""), r, String(""));

        }

        return true;

    }
    else {
        //p does not lie within the circle of the earth

        if (q != NULL) {

            Double d_temp;

            d_temp.set(String(""), -1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))), String(""));

            //from projection, compute the relative point on the x'z' plane, which has y'=0
            gsl_vector_set((rp.r), 0, ((d_temp.value) + 1.0) / (d_temp.value) * (temp.x));
            gsl_vector_set((rp.r), 2, ((d_temp.value) + 1.0) / (d_temp.value) * (temp.y));
            gsl_vector_set((rp.r), 1, 0.0);

            //r = (rotation.matrix)^T . rp
            //            gsl_blas_dgemv(CblasTrans, 1.0, rotation.matrix, (rp.r), 0.0, (r.r));
            cblas_dgemv(CblasRowMajor, CblasTrans, 3, 3, 1, rotation.matrix->data, 3, rp.r->data, 1, 0, r.r->data, 1);

            q->setCartesian(String(""), r, String(""));

        }

        return false;

    }

}

//convert the point p on the screen to the  Mercator projection q of the relative geographic position, by writing into q only if q!=NULL. It returns true/false if q lies within the boundaris x_min .. y_max
inline bool DrawPanel::ScreenToMercator(const wxPoint& p, PositionProjection* q) {

    PositionProjection temp;
//    bool check_x;

    //updates the position of the draw pane this
    draw_panel_origin = (this->GetScreenPosition());

    (temp.x) = x_min + (((double)(p.x) - ((draw_panel_origin.x) + (position_plot_area_now.x))) / ((double)(size_plot_area.GetWidth()))) * x_span();
    (temp.y) = y_min - (((double)(p.y)) - ((draw_panel_origin.y) + (position_plot_area_now.y) + (size_plot_area.GetHeight()))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min);

    if (q) {
        (q->x) = (temp.x);
        (q->y) = (temp.y);
    }

    return(temp.CheckMercator(this));

}


//converts the point p on the screen (which is supposed to lie in the plot area), to the  3D projection (x,y), which is written in q if q!=NULL. If p lies within /outside the circle of the earth, it returns true/false.
inline bool DrawPanel::ScreenTo3D(const wxPoint& p, PositionProjection* q) {

    PositionProjection temp;
    Double d_temp;

    //set d for the following
    d_temp.set(String(""), -1.0 + sqrt(1.0 + gsl_pow_2(tan(circle_observer.omega))), String(""));

    //updates the position of the draw pane this
    draw_panel_origin = (this->GetScreenPosition());

    (temp.x) = x_min + ((((double)(p.x)) - ((draw_panel_origin.x) + (position_plot_area_now.x))) / ((double)(size_plot_area.GetWidth()))) * (x_max - x_min);
    (temp.y) = y_min - (((double)(p.y)) - ((draw_panel_origin.y) + (position_plot_area_now.y) + (size_plot_area.GetHeight()))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min);

    //I pulled out a factor (temp.x)^2 from arg_sqrt for clarity
    arg_sqrt = -((gsl_sf_pow_int((d_temp.value), 2) * (-1 + gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2)) + 2 * (d_temp.value) * (gsl_sf_pow_int((temp.x), 2) + gsl_sf_pow_int((temp.y), 2))));

    //if q!=NULL, I write in it the result. I do this even if p lies outside the sphere of the earth
    if (q) {

        (*q) = temp;

    }

    if (arg_sqrt >= 0.0) {

        return true;

    }
    else {

        return false;

    }

}


//convert the geographic Position p  to the  3D PositionProjection (x,y). / If the PositionProjection of p falls in the visible side of the earth,  write its PositionProjection into *q (if q!=NULL) and returs true. If not, it returns false and, if write = true,  write its Projection in *q (if q!=NULL)
inline bool DrawPanel::GeoTo3D(const Position& p, PositionProjection* q, bool write) {


    //    clock_t t1, t2, t3;
    //    double Ta, Tb;
    //
    //    t1 = clock();

//    bool check, out;

    //compute rp before hand to rule out early the cases where check = 0
    //set r according equal to the 3d vector corresponding to the geographic position p
    /*
    gsl_vector_set((r.r), 0, cos((p.lambda)) * cos((p.phi)));
    gsl_vector_set((r.r), 1, -(cos((p.phi)) * sin((p.lambda))));
    gsl_vector_set((r.r), 2, sin((p.phi)));
    */
//    p.getCartesian(String(""), &r, String(""));
    r.setPosition(p);

    return CartesianTo3D(r, q, write);
   
}



// If the PositionProjection of q falls within the plot area,  write its Projection into p (if p!=NULL) and return true. If not, it returns false and, if write = true, it writes its projection in q
inline bool DrawPanel::CartesianToMercator(const Cartesian& p, PositionProjection* q, bool write) {

    Position temp;

    temp.setCartesian(String(""), p, String(""));

    return ((this->*GeoToProjection)(temp, q, write));


}


//convert the Cartesian position p  to the  3D PositionProjection (x,y). / If the PositionProjection of p falls in the visible side of the earth,  write its PositionProjection into *q (if q!=NULL) and return true. If not,  return false and, if write = true,  write its Projection in *q (if q!=NULL)
inline bool DrawPanel::CartesianTo3D(const Cartesian& p, PositionProjection* q, bool write) {

    bool check, out;
    
    gsl_vector_set((rp.r), 1,
                   /*
                    gsl_matrix_get(rotation.matrix, 1, 0) * gsl_vector_get((p.r), 0) +
                    gsl_matrix_get(rotation.matrix, 1, 1) * gsl_vector_get((p.r), 1) +
                    gsl_matrix_get(rotation.matrix, 1, 2) * gsl_vector_get((p.r), 2)
                    */
                   cblas_ddot(3, (rotation.matrix->data)+3, 1, p.r->data, 1)
                   );
    
    
    check = (gsl_vector_get((rp.r), 1) < -1.0 / (1.0 + (d.value)/Re));



    //    t2 = clock();
    //    Ta = t2-t1;


    if (check || write) {

        if (q != NULL) {

            double temp;

            //rotate r by rotation, and write the result in rp!
            //            gsl_blas_dgemv(CblasNoTrans, 1.0, rotation.matrix, (p.r), 0.0, (rp.r));
            cblas_dgemv(CblasRowMajor, CblasNoTrans, 3, 3, 1, rotation.matrix->data, 3, p.r->data, 1, 0, rp.r->data, 1);
            
            temp = (d.value) / ((d.value) + Re*(1.0 + gsl_vector_get((rp.r), 1)));
            (q->x) = gsl_vector_get((rp.r), 0) * temp;
            (q->y) = gsl_vector_get((rp.r), 2) * temp;

        }

        out = check;


    }
    else {

        out = false;

    }

    //    t3 = clock();
    //    Tb = t3-t2;

    return out;


}





//this function converts the geographic position p into the screen position p
inline void DrawPanel::GeoToScreen(const Position& q, wxPoint* p) {

    //updates the position of the draw pane this
    draw_panel_origin = (this->GetScreenPosition());


    GeoToDrawPanel(q, p, false);

    (p->x) += (draw_panel_origin.x);
    (p->y) += (draw_panel_origin.y);

}


// If the projection of q falls within the plot area, it writes its projection into p (if p!=NULL) and returns true. If not, it returns false and, if write = true, it writes its projection in p
inline bool DrawPanel::GeoToMercator(const Position& q, PositionProjection* p, bool write) {

    //    clock_t t_start, t_end;
    //    t_start = clock();

    PositionProjection temp;
    bool /*check_x, */check, out;

    //    (temp.x) = -(normalize_pm_pi_ret(q.lambda).value);
    //    (temp.y) = log(1.0 / cos((q.phi)) + tan((q.phi)));
    temp.NormalizeAndSetMercator(q);
    check = temp.CheckMercator(this);
    

    if (check || write) {
        //if the point falls within the plot area, write it into x, y

        if (p != NULL) {

            (p->x) = (temp.x);
            //this is needed if lambda_min, lambda_max encompass the Greenwich antimeridian: if p->x is smaller than x_max, then it nees to be translated to the right by 2pi in order to be plotted
            if ((x_max < x_min) && ((p->x) <= x_max)) {
                (p->x) += 2.0*M_PI;
            }

            (p->y) = (temp.y);

        }

        out = check;


    }
    else {

        out = false;

    }

    return out;

}





// convert the geographic position q into the DrawPanel position p, reckoned with respect to the origin of the  DrawPanel. If q is a valid Position, it returns true and (if p!=NULL), it writes the resulting DrawPanel coordinates in p. If q is not a valid position, it returns false and, if write = true and p!=NULL, it writes the drawpanel position in p.
inline bool DrawPanel::GeoToDrawPanel(const Position& q, wxPoint* p, bool write) {

    PositionProjection temp;
    bool check;


    check = (this->*GeoToProjection)(q, &temp, write);

    if (check || write) {

        if (p) {
            (this->*ProjectionToDrawPanel)(temp, p, true);
        }

        return check;

    }
    else {

        return false;

    }

}


// convert the cartesian position q into the DrawPanel position p, reckoned with respect to the origin of the  DrawPanel. If q is a valid Cartesian position, return true and (if p!=NULL),  write the resulting DrawPanel coordinates in p. If q is not a valid Cartesian position,  return false and, if write = true and p!=NULL, it writes the drawpanel position in p.
inline bool DrawPanel::CartesianToDrawPanel(const Cartesian& q, wxPoint* p, bool write) {

    PositionProjection temp;
    bool check;

    check = (this->*CartesianToProjection)(q, &temp, write);

    if (check || write) {

        if (p) {
            (this->*ProjectionToDrawPanel)(temp, p, true);
        }

        return check;

    }
    else {

        return false;

    }

}

//converts the Mercator projection q into the DrawPanel position p, reckoned with respect to the origin of the mercator draw panel.   If q is a valid PositionProjection , return true and (if p!=NULL),  write the resulting DrawPanel coordinates in p. If q is not a valid  PositionProjection,  return false and, if write = true and p!=NULL, write the DrawPanel position in p.
inline bool DrawPanel::ProjectionToDrawPanel_Mercator(PositionProjection& q, wxPoint* p, bool write) {
    
    bool check;
    PositionProjection temp;
    
    check = q.CheckMercator(this);

    if(check || write){
        
        if(p){
            
            temp = q;
            
            //this is needed if lambda_min, lambda_max encompass the Greenwich antimeridian: if q.x is smaller than x_max, then it nees to be translated to the right by 2 * pi
            if ((x_max < x_min) && ((temp.x) <= x_max)) {
                (temp.x) += 2.0*M_PI;
            }
            
            (p->x) = (position_plot_area_now.x) + ((temp.x) - x_min) / x_span() * (size_plot_area.GetWidth());
            (p->y) = (position_plot_area_now.y) + (size_plot_area.GetHeight()) - (((temp.y) - y_min) / (y_max - y_min) * (size_plot_area.GetHeight()));
            
        }
        
        return check;
        
    }else{
        
        return false;
        
    }

}


//convert the Mercator Projection q into the Position p
inline void  DrawPanel::ProjectionToGeo_Mercator(const PositionProjection& q, Position* p) {

    (p->lambda).set(String(""), k * lambda_mercator(q.x), String(""));
    (p->phi).set(String(""), k * phi_mercator(q.y), String(""));
  
}


//convert the Mercator Projection q into the Position p
inline void  DrawPanel::ProjectionToGeo_3D(const PositionProjection& q, Position* p) {

  
}


//this function converts the 3D PositionProjection q into the DrawPanel position p, reckoned with respect to the origin of the mercator DrawPanel.  If q is a valid PositionProjection, return true and (if p!=NULL), write the resulting DrawPanel coordinates in p. If q is not a valid  PositionProjection,  return false and, if write = true and p!=NULL, write the DrawPanel position in p.
inline bool DrawPanel::ProjectionToDrawPanel_3D(PositionProjection& q, wxPoint* p, bool write) {
    
    bool check;
    
    
    check = ((fabs(q.x) <= x_max) && (fabs(q.y) <= y_max));
    
    if(check || write){
        
        if(p){
            
            (p->x) = ((double)(position_plot_area_now.x)) + (1.0 + (q.x) / x_max) * (((double)(size_plot_area.GetWidth())) / 2.0);
            (p->y) = ((double)(position_plot_area_now.y)) + (1.0 - (q.y) / y_max) * (((double)(size_plot_area.GetHeight())) / 2.0);
         
        }
        
        return check;
        
    }else{
        
        return false;
        
    }
    
}



//given a Position q if q lies witin *this, write in label a text with the geographic coordinates of q
void DrawPanel::ShowCoordinates(const Position& q, String* label) {

    wxPoint temp;

    if (GeoToDrawPanel(q, &temp, false)) {

        SetLabelAndAdjustPosition(q, &temp, label);

    }
    else {

        (*label) = String("");

    }

}


//given a geographic Positiojn q, if q lies within *this, write in label a text with the geographic coordinates corresponding to q, and write in *position the position of the label close to q (with some margin, for clarity). Otherwise, write "" in label and does nothing witg poisition
void DrawPanel::SetLabelAndPosition(const Position& q, wxPoint* position, String* label) {

    if (
        /*GeoToDrawPanel converts q into the wxPoint position, reckoned with respect to the origin *this*/(this->GeoToDrawPanel)(q, position, false)) {

        //SetCoordinateLabel uses position set from above, and ajusts it by including some margins
        SetLabelAndAdjustPosition(q, position, label);

    }
    else {

        (*label) = String("");


    }

}


//given a position q with respect to the origin of the screen, if q lies within *this, write in label a text with the geographic coordinates corresponding to q, and write in *position the position of the label close to q (with some margin, for clarity). Otherwise, write "" in label and does nothing witg poisition
void DrawPanel::SetLabelAndPosition(const wxPoint& q, wxPoint* position, String* label) {

    if ((this->ScreenToDrawPanel)(q, position)) {

        Position temp;

        (this->*ScreenToGeo)(q, &temp);
        SetLabelAndAdjustPosition(temp, position, label);

    }
    else {

        (*label) = String("");

    }

}


//given a geographic Position p and its corresponding wxPoint with respect to the origin of this *poisition, write a string containing the geographic coordinates of p into label, and adjust *poistiion in such a way that label is enclosed in *this
void DrawPanel::SetLabelAndAdjustPosition(const Position& p, wxPoint* position, String* label) {

    //the shift that will be applied to the position of *label
    wxPoint shift;

    //set the text of *label
    label->set(String(""), to_string(p, display_precision.value), String(""));

    //the default value of the shift
    shift = wxPoint(
        (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)
    );


    if ((position->x) + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value) + ((label->get_size(this)).x) > (size_plot_area.x) + (position_plot_area_now.x)) {
        //label does not fit into *this: it goes beyond the right edge of *this -> move it to the left

        shift -= wxPoint(
            (((label->get_size(this)).x) + 2 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value)),
            0
        );

    }

    if ((position->y) + (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value) + ((label->get_size(this)).y) > (size_plot_area.y) + (position_plot_area_now.y)) {
        //label does not fit into *this: it goes beyond the bottom edge of *this -> move up shift

        shift -= wxPoint(
            0,
            (((label->get_size(this)).y) + 2 * (wxGetApp().rectangle_display.GetWidth()) * (length_border_over_length_screen.value))
        );

    }

    //hift the label label with respect to p
//	label->SetPosition(position + shift);
    //adjust *position
    (*position) += shift;

}


//set ChartFrame title and all quantities according to a chosen projection
template<class E> void DrawPanel::SetProjection(E& event) {
    
    stringstream s;
    String temp;
    size_t pos;

    //set the ChartFrame title
    //extract the part of the title which comes before the name of the projection type
    temp = String((parent->GetLabel()).ToStdString());
    pos = (temp.value).find(" - ");
    temp.set(String(""), String((temp.value).substr(0, pos).c_str()), String(""));

    //put together temp and the new name of the projection type and write the result into the title of parent
    s.str("");
    s << (temp.value) << " - " << (((parent->projection)->name)->GetValue().ToStdString()) << " projection";
    temp.set(String(""), s.str(), String(""));
    parent->SetLabel(wxString(s.str().c_str()));


    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
        //if in projection "mercator" is selected, then I let the Draw function pointer point to PreRenderMercator, same for other functions, and I disable the fields of the angle for the Euler rotation of the 3d earth, which are not necessary

        PreRender = (&DrawPanel::PreRenderMercator);
        Render = (&DrawPanel::Render_Mercator);
        ProjectionToDrawPanel = (&DrawPanel::ProjectionToDrawPanel_Mercator);
        ProjectionToGeo = (&DrawPanel::ProjectionToGeo_Mercator);
        ScreenToProjection = (&DrawPanel::ScreenToMercator);
        CartesianToProjection = (&DrawPanel::CartesianToMercator);
        ScreenToGeo = (&DrawPanel::ScreenToGeo_Mercator);
        GeoToProjection = (&DrawPanel::GeoToMercator);
        Set_x_y_min_max = (&DrawPanel::Set_x_y_min_max_Mercator);
        Set_lambda_phi_min_max = (&DrawPanel::Set_lambda_phi_min_max_Mercator);
        Set_size_chart = (&DrawPanel::Set_size_chart_Mercator);
        (parent->UpdateSliderLabel) = (&ChartFrame::UpdateSliderLabel_Mercator);

    }

    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
        //if in projection ((Projection_types[1]).value) is selected, then I let the Draw function pointer point to PreRender3D, same for other functions, and I enable the angles for the 3d rotation of the 3d earth, which are now needed from the user.

        PreRender = (&DrawPanel::PreRender3D);
        Render = (&DrawPanel::Render_3D);
        ProjectionToDrawPanel = (&DrawPanel::ProjectionToDrawPanel_3D);
        ProjectionToGeo = (&DrawPanel::ProjectionToGeo_3D);
        ScreenToProjection = (&DrawPanel::ScreenTo3D);
        CartesianToProjection = (&DrawPanel::CartesianTo3D);
        ScreenToGeo = (&DrawPanel::ScreenToGeo_3D);
        GeoToProjection = (&DrawPanel::GeoTo3D);
        Set_x_y_min_max = (&DrawPanel::Set_x_y_min_max_3D);
        Set_lambda_phi_min_max = (&DrawPanel::Set_lambda_phi_min_max_3D);
        Set_size_chart = (&DrawPanel::Set_size_chart_3D);
        (parent->UpdateSliderLabel) = (&ChartFrame::UpdateSliderLabel_3D);

    }
    
    event.Skip(true);

}


//this method is called when the used has chosen Projection -> set all quantities according to the chosen Projection and call Reset to re-render everything
template<class E> void DrawPanel::OnChooseProjection(E& event) {

    SetProjection<E>(event);
    parent->ResetRender<E>(event);
    parent->Animate();

    event.Skip(true);

}


//This function obtains the geographical Position p of the mouse hovering on the map of the world. It returns true if the mouse is in the plot area, false otherwise
bool DrawPanel::GetMouseGeoPosition(Position* p) {

    //	(parent->parent->screen_position_now) = wxGetMousePosition();

    return ((this->*ScreenToGeo)((parent->parent->screen_position_now), p));

}

void DrawPanel::OnMouseMovement(wxMouseEvent& event) {

    wxPoint q;
    stringstream s;
    int i, j, l;
    
    //lines for debug
    //    cout << "\nMouse moved";
    //    //    cout << "Position of text_position_now = {" << ((parent->text_position_now)->GetPosition()).x << " , " << ((parent->text_position_now)->GetPosition()).x << "}\n";
    //    cout << "Position of mouse screen = {" << (parent->parent->screen_position_now).x << " , " << (parent->parent->screen_position_now).y << "}\n";
    //    cout << "Position of mouse draw panel = {" << ((parent->parent->screen_position_now)-draw_panel_origin).x << " , " << ((parent->parent->screen_position_now)-draw_panel_origin).y << "}\n";
    //lines for debug


#ifdef _WIN32

    //store the former _now positions into the _before positions
    (parent->parent->screen_position_before) = (parent->parent->screen_position_now);
    (parent->parent->geo_position_before) = (parent->parent->geo_position_now);
    label_position_before = label_position_now;

#endif

    //update the instantaneous screen and geographic position of the mouse on the chart and compute mouse_in_plot_area, which will be used by other methods.
    (parent->parent->mouse_moving) = true;
    (parent->parent->screen_position_now) = wxGetMousePosition();
    mouse_in_plot_area = (this->*ScreenToGeo)((parent->parent->screen_position_now), &((parent->parent->geo_position_now)));
    if (mouse_in_plot_area && (!parent->parent->selection_rectangle)) {
        //the mouse has a screen position corresponding to a geographic position and no selection rectangle is being drawn -> I show the instantaneous mouse coordinates : I write them into label_position_now, otherwise label_position_now is left empty,

        label_position_now = String((parent->parent->geo_position_now.to_string(display_precision.value)));

    }
    else {

        label_position_now = String("");

    }


    if ((parent->parent->selection_rectangle)) {
        //a selection rectangle is being drawn -> update the instantaneous position of the final corner of the rectangle

#ifdef __APPLE__

        for (i = 0; i < (parent->parent->chart_frames.size()); i++) {

            //write the label and position of the selection rectangle for each DrawPanel into end_label_selection_rectangle_now and position_end_label_selection_rectangle_now, respectively
            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                (parent->parent->geo_position_now),
                &(((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_now),
                &(parent->parent->end_label_selection_rectangle_now)
            );


        }


#endif

#ifdef _WIN32

        //on WIN32, the Refresh() command slows down things -> I don't call it but use RefreshWIN32(), which cleans up the former selections rectangle in *this and draws a new one
        (parent->parent->end_label_selection_rectangle_before) = (parent->parent->end_label_selection_rectangle_now);

        for (i = 0; i < (parent->parent->chart_frames.size()); i++) {

            (((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_before) = (((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_now);

            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition((parent->parent->geo_position_now), &(((parent->parent->chart_frames)[i])->draw_panel->position_end_label_selection_rectangle_now), &(parent->parent->end_label_selection_rectangle_now));

//            ((parent->parent->chart_frames)[i])->draw_panel->RefreshWIN32();

        }

#endif

        parent->parent->MyRefreshAll();

    }
    else {
        //no selection rectangle is being drawn

        //run over all the routes, check if the mouse is hovering over one of them, and change the background color of the related position in listcontrol_routes

        //I compute the position of the mouse with respect to the origin of the DrawPanel, so I can compare it with points_route_list[i], which are also with respect to the origin of the draw panel
        position_draw_panel_now = (parent->parent->screen_position_now) - draw_panel_origin;

        //save the id of the Route highlighted at the preceeding step into highlighted_route_before
        (parent->parent->highlighted_route_before) = (parent->parent->highlighted_route_now);

        for ((parent->parent->highlighted_route_now) = -1, i = 0; i < (parent->parent->data->route_list).size(); i++) {

            //set the backgorund color of the Route in listcontrol_routes and of its related sight to white
            //when only a fraction of the Routes is Drawn, this will create a problem ---
            ((parent->parent)->listcontrol_routes)->SetItemBackgroundColour(i, wxGetApp().background_color);
            //when only a fraction of the Routes is Drawn, this will create a problem ---

            if ((((parent->parent->data->route_list)[i]).related_sight).value != -1) {
                ((parent->parent)->listcontrol_sights)->SetItemBackgroundColour((((parent->parent->data->route_list)[i]).related_sight).value, wxGetApp().background_color);
            }

            //run over Routes and check whether the mouse is hovering over one of them
            for (j = 0; j < (points_route_list_now[i]).size(); j++) {

                for (l = 0; l < ((int)((points_route_list_now[i][j]).size())) - 1; l++) {

                    //if the mouse is hovering over one of the points of route #i, I set the background color of route i in listcontrol_routes to a color different from white, to highlight it, and I highlight also the related sight in listcontrol_sights

                    if (/*to recognize that the mouse is hovering over a Route, I need the abscissas of two subsequent points of the Route to be different. Otherwise, there is not space on the screen where to recognize the presence of the mouse*/ (((points_route_list_now[i][j][l]).x) != ((points_route_list_now[i][j][l + 1]).x))

                        &&/*I check the the mouse's abscissa falls within the abscissas of two subsewquent points of the Route*/

                        (((((points_route_list_now[i][j][l]).x) <= (position_draw_panel_now.x)) && ((position_draw_panel_now.x) <= ((points_route_list_now[i][j][l + 1]).x))) ||

                            ((((points_route_list_now[i][j][l + 1]).x) <= (position_draw_panel_now.x)) && ((position_draw_panel_now.x) <= ((points_route_list_now[i][j][l]).x))))

                        &&/*I check the the mouse's ordinate falls within the ordinates of the two subsewquent points of the Route above*/

                        (
                            fabs(
                                (position_draw_panel_now.y) -
                                (((points_route_list_now[i][j][l]).y) + ((double)(((points_route_list_now[i][j][l + 1]).y) - ((points_route_list_now[i][j][l]).y))) / ((double)(((points_route_list_now[i][j][l + 1]).x) - ((points_route_list_now[i][j][l]).x))) * ((double)((position_draw_panel_now.x) - ((points_route_list_now[i][j][l]).x))))
                            )

                            <= (thickness_route_selection_over_length_screen.value) * ((double)(wxGetApp().rectangle_display.GetWidth())) / 2.0
                            )
                        ) {
                        //the mouse is overing over a Route


                        //set the highlighted route to i, so as to use highlighted_route in other functions
                        (parent->parent->highlighted_route_now) = i;

                        parent->parent->listcontrol_routes->EnsureVisible(i);
                        if ((((parent->parent->data->route_list)[i]).related_sight.value) != -1) {
                            parent->parent->listcontrol_sights->EnsureVisible(((parent->parent->data->route_list)[i]).related_sight.value);
                        }

                        //set highlighted_sight_now and the beckgorund color of the Route in listcontrol_routes and of its related sight to a highlight color
                        ((parent->parent)->listcontrol_routes)->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                        if ((((parent->parent->data->route_list)[i]).related_sight.value) != -1) {

                            (parent->parent->highlighted_sight_now) = (((parent->parent->data->route_list)[i]).related_sight.value);

                            parent->parent->listcontrol_sights->SetItemBackgroundColour(
                                (parent->parent->highlighted_sight_now),
                                (wxGetApp().color_selected_item)
                            );
                        }
                        else {

                            (parent->parent->highlighted_sight_now) = -1;

                        }


                        // quit the loops over l ad j
                        break;
                        break;

                    }

                }

            }

        }



        if ((parent->parent->highlighted_route_now) == -1) {
            //no Route is highlighted -> in listcontrol_sights and listcontrol_routes go back to showing the first respective items

            if ((parent->parent->listcontrol_routes->GetItemCount()) > 0) {

                parent->parent->listcontrol_routes->EnsureVisible(0);

            }

            if ((parent->parent->listcontrol_sights->GetItemCount()) > 0) {

                parent->parent->listcontrol_sights->EnsureVisible(0);

            }

        }


        //run over all the Positions, check if the mouse is hovering over one of them, and change the background color of the related Position in listcontrol_positions
        (parent->parent->highlighted_position_before) = (parent->parent->highlighted_position_now);

        for ((parent->parent->highlighted_position_now) = -1, i = 0; i < (parent->parent->data->position_list).size(); i++) {

            GeoToScreen((parent->parent->data->position_list)[i], &q);

            if (sqrt(gsl_pow_2(((parent->parent->screen_position_now).x) - (q.x)) + gsl_pow_2(((parent->parent->screen_position_now).y) - (q.y))) <
                4.0 * ((((wxGetApp().standard_thickness_over_length_screen)).value) / 2.0 * (wxGetApp().rectangle_display).GetWidth())) {
                //the mouse is over a position

                //sets the highlighted position to i, so as to use highlighted_position_now in other functions
                ((parent->parent)->highlighted_position_now) = i;

                ((parent->parent)->listcontrol_positions)->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                parent->parent->listcontrol_positions->EnsureVisible(i);

            }
            else {
                //no Position is highlighted -> reset the background color in listcontrol positions, and in listcontrol_positions go back to showing the first  item

                ((parent->parent)->listcontrol_positions)->SetItemBackgroundColour(i, wxGetApp().background_color);
                parent->parent->listcontrol_positions->EnsureVisible(0);

            }

        }

        if (((parent->parent->highlighted_route_before) != (parent->parent->highlighted_route_now)) || ((parent->parent->highlighted_position_before) != (parent->parent->highlighted_position_now))) {
            //the highlighted Route or Position has changed -> update the charts

            (parent->parent->changing_highlighted_object) = true;

            parent->parent->MyRefreshAll();

            (parent->parent->changing_highlighted_object) = false;


        }
        else {
            //the highlighted Route has not changed ->  the chart does not need to be updated, but the coordinates of the instantaneous mouse position do -> call

            MyRefresh();

        }

    }


    (parent->parent->mouse_moving) = false;

    event.Skip(true);

}

//if the left button of the mouse is pressed, I record its position as the starting position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftDown(wxMouseEvent& event) {

    position_start_drag = wxGetMousePosition();
    (this->*ScreenToGeo)(position_start_drag, &geo_start_drag);

    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {

        //I store the boundaries of the plot at the beginning of the drag, so if the drag is aborted I will restore these boundaries
        x_min_start_drag = x_min;
        x_max_start_drag = x_max;
        y_min_start_drag = y_min;
        y_max_start_drag = y_max;
        x_span_start_drag = x_span();

    }

    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {

        //I store the orientation of the earth at the beginning of the drag in rotation_start_drag
        //        gsl_vector_memcpy((rp_start_drag.r), (rp.r));
        rotation_start_drag.set(rotation);
        //        geo_start_drag.print(String("position start drag"), String(""), cout);
        //        rotation_start_drag.print(String("rotation start drag"), String(""), cout);

    }

    event.Skip(true);

}

//if the left button of the mouse is released, I record its position as the ending position of a (potential) mouse-dragging event
void DrawPanel::OnMouseLeftUp(wxMouseEvent& event) {

    SetCursor(*wxCROSS_CURSOR);

    //if the mouse left button was previously down because of a dragging event, then the dragging event is now over, and I set mouse_dragging = false;
    if (mouse_dragging) {
        //the left button of the mouse has been lifted at the end of a drag

        mouse_dragging = false;
        //given that the mosue drag has ended, I re-bind OnMoueMOvement to the mouse motion event
        this->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, this);


        position_end_drag = wxGetMousePosition();
        (this->*ScreenToGeo)(position_start_drag, &geo_end_drag);



        if (((parent->parent->highlighted_route_now) == -1) && (((parent->parent)->highlighted_position_now) == -1)) {
            //I am dragging the chart (not a Route nor  a Position)

            if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                //I am using the Mercator projection

                double delta_y;
                PositionProjection p_ceil_min, p_floor_max;

                delta_y = ((double)((position_end_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min);

                (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
                (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);


                if ((!((y_max + delta_y < (p_floor_max.y)) && (y_min + delta_y > (p_ceil_min.y))))) {
                    //in this case,  the drag operation ends out  the min and max latitude contained in the data files -> reset y_min, y_max to their original values

                    //                    x_min = x_min_start_drag;
                    //                    x_max = x_max_start_drag;
                    y_min = y_min_start_drag;
                    y_max = y_max_start_drag;

                    (this->*Set_lambda_phi_min_max)();

                    //re-draw the chart
                    (this->*PreRender)();
                    Refresh();
                    FitAll();

                }

            }

            if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {

//                gsl_vector_memcpy((rp_end_drag.r), (rp.r));
                rotation_end_drag.set(rotation);
//                geo_end_drag.print(String("position end drag"), String(""), cout);
//                rotation_end_drag.print(String("rotation end drag"), String(""), cout);

            }

            //the drag operation has ended -> I set
            (parent->dragging_chart) = false;
            (parent->parent->i_object_to_disconnect) = -1;

        }
        else {
            //I am dragging a Route or Position

            int i;

            //given that the drag is finished, I set to empty label_dragged_object for all ChartFrames
            for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                ((parent->parent->chart_frames[i])->draw_panel->label_dragged_object_now) = String("");
            }

#ifdef __APPLE__

            parent->parent->RefreshAll();

#endif

#ifdef WIN32


            for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                (parent->parent->chart_frames[i])->draw_panel->RefreshWIN32();
            }

#endif

            (parent->parent->dragging_object) = false;


            if (!(((((draw_panel_origin.x) + (position_plot_area_now.x) < (position_end_drag.x)) && ((position_end_drag.x) < (draw_panel_origin.x) + (position_plot_area_now.x) + (size_plot_area.GetWidth()))) &&
                (((draw_panel_origin.y) + (position_plot_area_now.y) < (position_end_drag.y)) && ((position_end_drag.y) < (draw_panel_origin.y) + (position_plot_area_now.y) + (size_plot_area.GetHeight())))))) {
                // drag_end_position lies out the plot area

                if ((parent->parent->highlighted_route_now) != -1) {
                    //I am dragging a Route: I restore the starting position of the route under consideration to its value at the beginning of the drag and re-tabulate the route points

                    (((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position) = route_reference_position_drag_start;

                    TabulateRoutes();
                    Refresh();
                    FitAll();

                    print_error_message->SetAndCall(NULL, String("Error"), String("Route ground or start position outside plot area! Route start or start position must lie within the plot area."), (wxGetApp().path_file_error_icon));

                }

                if ((((parent->parent)->highlighted_position_now) != -1)) {
                    // I am dragging a Position: I restore the position under consideration to its value at the beginning of the drag

                    //convert the coordinates of position_start_drag into geographic coordinates, and assign these to the Position under consideration
                    (this->*ScreenToGeo)(position_start_drag, &((parent->parent->data->position_list)[((parent->parent)->highlighted_position_now)]));


                    //update the coordinates of the Position under consideration in listcontrol_positions
                    ((parent->parent->data->position_list)[((parent->parent)->highlighted_position_now)]).update_wxListCtrl(((parent->parent)->highlighted_position_now), (parent->parent)->listcontrol_positions);

                    //given that the position under consideration has changed, I re-pain the chart
                    Refresh();
                    FitAll();

                    print_error_message->SetAndCall(NULL, String("Error"), String("Position outside plot area! The position must lie within the plot area."), (wxGetApp().path_file_error_icon));

                }

            }

        }

    }
    else {
        //the left button of the mouse has not been lifted at the end of a drag

        //if, when the left button of the mouse was down, the mouse was hovering over a Position, then this position is selectd in listcontrol_positions and highlighted in color
        if (((parent->parent)->highlighted_position_now) != -1) {

            //deselect any previously selected item in listcontrol_positions, if any
            ((parent->parent)->listcontrol_positions)->DeselectAll();

            parent->parent->Raise();  // bring the ListFrame to front
            parent->parent->SetFocus();  // focus on the ListFrame

            //select the highlighted position in ListFrame
            ((parent->parent)->listcontrol_positions)->SetItemState((parent->parent)->highlighted_position_now, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

            //set the beckgorund color of the Position in listcontrol_positions in ListFrame to the color of selected items
            ((parent->parent)->listcontrol_positions)->SetItemBackgroundColour((parent->parent)->highlighted_position_now, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

        }

        //if, when the left button of the mouse was down, the mouse was hovering over a Route, then this Route and the related Sight (if any) is selectd in listcontrol_routes and listcontrol_sights, respectively, and highlighted in color
        if ((parent->parent->highlighted_route_now) != -1) {

            //deselect any previously selected item in listcontrol_routes, if any
            ((parent->parent)->listcontrol_routes)->DeselectAll();

            //deselect any previously selected item in listcontrol_sights, if any, to clear up things for the user and show only the selected Route in ListFrames
            ((parent->parent)->listcontrol_sights)->DeselectAll();


            parent->parent->Raise();  // bring the ListFrame to front
            parent->parent->SetFocus();  // focus on the ListFrame

            //select the highlighted Route in ListFrame
            ((parent->parent)->listcontrol_routes)->SetItemState((parent->parent)->highlighted_route_now, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

            //set the beckgorund color of the Route in listcontrol_routes in ListFrame to the color of selected items
            ((parent->parent)->listcontrol_routes)->SetItemBackgroundColour((parent->parent)->highlighted_route_now, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

            if ((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value != -1) {
                //the selected Route is related to a Sight



                //select the related Sight in ListFrame
                ((parent->parent)->listcontrol_sights)->SetItemState((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);

                //set the beckgorund color of the related Sight in listcontrol_sights in ListFrame to the color of selected items
                ((parent->parent)->listcontrol_sights)->SetItemBackgroundColour((((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));

            }

        }


    }

    event.Skip(true);

}


template<class T> void ChartFrame::OnMouseLeftDownOnSlider(T& event) {

    //mouse scrolling starts
    mouse_scrolling = true;

    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        (draw_panel->x_center_scrolling) = ((draw_panel->x_min) + (draw_panel->x_max)) / 2.0;
        (draw_panel->y_center_scrolling) = ((draw_panel->y_min) + (draw_panel->y_max)) / 2.0;

    }

    event.Skip(true);

}



template<class T> void ChartFrame::OnMouseLeftUpOnSlider(T& event) {

    //mouse scrolling ends
    mouse_scrolling = false;

    event.Skip(true);

}


void DrawPanel::OnMouseRightDown(wxMouseEvent& event) {
    
    stringstream s;
    
    //changes the 'sign' of selection rectangle
    (parent->parent->selection_rectangle) = (!(parent->parent->selection_rectangle));
    
    if ((parent->parent->selection_rectangle)) {
        //start drawing a selection rectangle
        
        int i;
        bool check;
        
        //disable all button_resets while a selection rectangle is being drawn
        for(i=0; i<parent->parent->chart_frames.size(); i++){
            parent->parent->chart_frames[i]->button_reset->Enable(false);
        }
        
        (parent->parent->geo_position_start) = (parent->parent->geo_position_now);
        //        GetMouseGeoPosition(&(parent->parent->geo_position_start));
        //        drawpanel_position_start = (parent->parent->screen_position_now);
        
        //store the position at the beginning of the selection process, to compute the zoom factor 
        //run over all ChartFrames : if in the i-th ChartFrame geo_position_start falls within the plot area -> set label and position of the start position of selection_rectangle in that ChartFrame
        for (i = 0, check = false; i < (parent->parent->chart_frames).size(); i++) {
            
            if (
                (((parent->parent->chart_frames)[i])->draw_panel->*(((parent->parent->chart_frames)[i])->draw_panel->GeoToProjection))((parent->parent->geo_position_start), &(((parent->parent->chart_frames)[i])->draw_panel->projection_start), false)) {
                    //geo_position_start is valid in the i-th DrawPanel -> start the selection rectangle in the i-th DrawPanel
                    
                    ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                                                                                         (parent->parent->geo_position_now),
                                                                                         &(((parent->parent->chart_frames)[i])->draw_panel->position_start_label_selection_rectangle),
                                                                                         &(parent->parent->start_label_selection_rectangle));
                    
                    ((parent->parent->chart_frames)[i])->draw_panel->Refresh();
                    check = true;
                    
                }
            
            
        }
        
        
        if (!check) {
            //geo_position_start is invalid in all DrawPanels -> delete the selection rectangle by setting selection_rectangle to false
            
            (parent->parent->selection_rectangle) = false;
            
        }
        
        
    }
    else {
        //end drawing a selection rectangle
        
        GetMouseGeoPosition(&(parent->parent->position_end));
        drawpanel_position_end = (parent->parent->screen_position_now);
        
        unsigned int i;
        
        //store the position at the end of the selection process, to compute the zoom factor later
        if ((this->*ScreenToProjection)(drawpanel_position_end, &projection_end)) {
            //drawpanel_position_end is valid
            
            Angle lambda_a, lambda_b;
            
            
            //convert all the angles to the format between -pi and pi, so I can sort them numerically
            parent->parent->geo_position_start.phi.normalize_pm_pi();
            parent->parent->geo_position_start.lambda.normalize_pm_pi();
            parent->parent->position_end.phi.normalize_pm_pi();
            parent->parent->position_end.lambda.normalize_pm_pi();
            
            lambda_a = (parent->parent->geo_position_start.lambda);
            lambda_b = (parent->parent->position_end.lambda);
            
            switch (position_in_vector(Projection((parent->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using the Mercator projection
                    
                    if ((parent->ComputeZoomFactor_Mercator(fabs((projection_end.x) - (projection_start.x))))) {
                        //if the zoom factor of the map resulting from the selection is valid, I update x_min, ... , y_max
                        
                        
                        //in order to properly set lambda_min and lambda_max, I need to tell apart the following cases
                        if(GSL_SIGN((lambda_a.normalize_pm_pi_ret().value)) == GSL_SIGN(lambda_b.normalize_pm_pi_ret().value)){
                            //lambda_a and lambda_b lie in the same hemisphere
                            
                            if(lambda_b > lambda_a){
                                
                                (parent->lambda_min) = lambda_b;
                                (parent->lambda_max) = lambda_a;
                                
                            }else{
                                
                                (parent->lambda_min) = lambda_a;
                                (parent->lambda_max) = lambda_b;
                                
                            }
                            
                        }else{
                            //lambda_a and lambda_b lie in different hemispheres
                            
                            if(((lambda_a.normalize_pm_pi_ret().value) >= 0.0) && (lambda_b.normalize_pm_pi_ret().value) <= 0.0){
                                //lambda_a lies in the poisitive-logitude hemishere (0 < lambda < 180), lambda_b in the nevative-longitude hemisphere (180 < lambda < 360)
                                
                                if((parent->lambda_min.normalize_pm_pi_ret().value) > (parent->lambda_max.normalize_pm_pi_ret().value)){
                                    
                                    (parent->lambda_min) = lambda_a;
                                    (parent->lambda_max) = lambda_b;
                                    
                                }else{
                                    
                                    (parent->lambda_min) = lambda_b;
                                    (parent->lambda_max) = lambda_a;
                                    
                                }
                                
                            }else{
                                //lambda_a lies in the negative-logitude hemishere (180 < lambda < 360), lambda_b in the positive-longitude hemisphere (0 < lambda < 180)
                                
                                if((parent->lambda_min.normalize_pm_pi_ret().value) > (parent->lambda_max.normalize_pm_pi_ret().value)){
                                    
                                    (parent->lambda_min) = lambda_b;
                                    (parent->lambda_max) = lambda_a;
                                    
                                }else{
                                    
                                    (parent->lambda_min) = lambda_a;
                                    (parent->lambda_max) = lambda_b;
                                    
                                }
                                
                            }
                            
                        }
                        
                        
                        
                        if ((parent->parent->geo_position_start.phi) > ((parent->parent->position_end).phi)) {
                            (parent->phi_max) = (((parent->parent)->geo_position_start).phi);
                            (parent->phi_min) = (((parent->parent)->position_end).phi);
                        }else {
                            (parent->phi_min) = (((parent->parent)->geo_position_start).phi);
                            (parent->phi_max) = (((parent->parent)->position_end).phi);
                        }
                        //I normalize lambda_min, ..., phi_max for future use.
                        parent->lambda_min.normalize();
                        parent->lambda_max.normalize();
                        parent->phi_min.normalize();
                        parent->phi_max.normalize();
                        
                        parent->parent->geo_position_start.phi.normalize();
                        parent->parent->geo_position_start.lambda.normalize();
                        parent->parent->position_end.phi.normalize();
                        parent->parent->position_end.lambda.normalize();
                        
                        (this->*PreRender)();
                        parent->parent->RefreshAll();
                        FitAll();
                        
                        parent->UpdateSlider();
                        //the aspect ratio of ChartFrame may have changed -> call ShowChart to reposition everything properly on the screen
                        wxGetApp().ShowCharts(event);
                        
                    }
                    else {
                        //if the zoom factor is not valid, then I print an error message
                        
                        s.str("");
                        s << "Zoom level must be >= 1 and <= " << (wxGetApp().zoom_factor_max.value) << ".";
                        
                        //set the title and message for the functor print_error_message, and then call the functor
                        print_error_message->SetAndCall(NULL, String("Zoom level exceeded its maximal value!"), String(s.str().c_str()), (wxGetApp().path_file_error_icon));
                        
                    }
                    
                    
                    break;
                }
                    
                case 1: {
                    //I am using the 3d projection
                    
                    Length l1, l2;
                    Position reference_position_old;
                    
                    //store the current ground position of circle_observer into reference_position_old
                    reference_position_old = (circle_observer.reference_position);
                    
                    //normalize lambda_a/b and then compute the algebric mean -> this is the correct value of the longitude of circle_observer
                    circle_observer.reference_position.lambda.set((lambda_a.normalize_ret().value + lambda_b.normalize_ret().value)/2.0);
    
                    //normalize the two latitudes between -pi and pi and then compute the algebraic mean -> this is the correct value of the two latitudes
                    circle_observer.reference_position.phi.set(mean_pm_pi(parent->parent->geo_position_start.phi, parent->parent->position_end.phi));
                    
                    
                    //compute omega as half of  the largest angular distance between the middle of selection rectangle and its corners
                    circle_observer.reference_position.distance((parent->parent->geo_position_start), &l1, String(""), String(""));
                    circle_observer.reference_position.distance(Position(parent->parent->geo_position_start.lambda, parent->parent->position_end.phi), &l2, String(""), String(""));
                    circle_observer.omega.set(String(""), ((max(l1, l2).value) / Re)/2.0, String(""));
                    
                    
                    //conpute the new rotation: the new rotation of the earth is the old one, composed with the rotation which brings the old reference_position onto the new one
                    //The coordinate transformation between a vector r in reference frame O and a vector r' in reference frame O' is r = (rotation^T).r', rotation . Rotation(circle_observer.reference_position, reference_position_old). (rotation^T) =   Rotation(circle_observer.reference_position, reference_position_old)' (i.e., Rotation(circle_observer.reference_position, reference_position_old) in reference frame O'), thus I set rotation = Rotation(circle_observer.reference_position, reference_position_old)' * rotation, and by simplifying I obtain
                    rotation.set((rotation * Rotation(circle_observer.reference_position, reference_position_old)));
                    
                    (this->*PreRender)();
                    parent->parent->RefreshAll();
                    FitAll();
                    
                    parent->UpdateSlider();
                    //the aspect ratio of ChartFrame may have changed -> call ShowChart to reposition everything properly on the screen
                    wxGetApp().ShowCharts(event);
                    
                    
                    break;
                }
                    
            }
            
            //set to empty the text fields of the geographical positions of the selek÷ction triangle, which is now useless
            
            (parent->parent->start_label_selection_rectangle) = String("");
            (parent->parent->end_label_selection_rectangle_now) = String("");
            
        }else {
            //the  end position for the selected rectangle is not valid -> cancel the rectangle by setting selection_rectangle to false and by setting to empty the text fields of the geographical positions of the selection triangle
            
            (parent->parent->selection_rectangle) = false;
            (parent->parent->start_label_selection_rectangle) = String("");
            (parent->parent->end_label_selection_rectangle_now) = String("");
            
        }
        
        //disable all button_resets while a selection rectangle is being drawn
        for(i=0; i<parent->parent->chart_frames.size(); i++){
            parent->parent->chart_frames[i]->button_reset->Enable(true);
        }
        
    }
    
    event.Skip(true);
    
}

//this function is called whenever mouse is dragged on *this
void DrawPanel::OnMouseDrag(wxMouseEvent& event) {
    
    if ((!idling) && (!(parent->idling)) && (!(parent->parent->idling))) {
        //I proceed only if this and its parent and the parent of its parent are not in idling mode
        
        if (wxGetMouseState().LeftIsDown()) {
            
            if (!mouse_dragging) {
                //the mouse has started dragging
                
                //If I am dragging a Route, I save the starting point of this Route into route_reference_position_drag_now
                
                //during the mouse drag, I disable DrawPanel::OnMouseMovement
                this->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, this);
                
                if ((parent->parent->highlighted_route_now) != -1) {
                    //set route_reference_position_drag_now to the start Position (if the route is a loxodrome / orthodrome) or to the ground Position (if the route is a circle of equal altitutde)
                    
                    if (((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).type) == (Route_types[2])) {
                        
                        route_reference_position_drag_start = ((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).reference_position);
                        
                        if ((((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value) != -1) {
                            //here I am dragging a circle of equal altitude originally related to a sight. After dragging, this circle of equal altitude no longer results from that sight, thus I disconnect the sight and the circle of equal altitude, and update the wxListCtrs in parent->parent accordingly
                            
                            (parent->parent->i_object_to_disconnect) = (((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).related_sight).value);
                            
                            parent->parent->DisconnectAndPromptMessage(event);
                            
                        }
                        
                    }
                    else {
                        
                        route_reference_position_drag_start = ((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).reference_position);
                        
                    }
                    
                    
                }
                
            }
            
            mouse_dragging = true;
            
            SetCursor(wxCURSOR_HAND);
            
            position_now_drag = wxGetMousePosition();
            
            
            if ((this->*ScreenToGeo)(position_now_drag, NULL)) {
                //position_drag_now is a valid Position
                
                if ((((parent->parent->highlighted_route_now) == -1) && ((parent->parent->highlighted_position_now) == -1))) {
                    //the whole chart is being dragged (the mouse is not over a Route nor a Position while dragging)
                    
                    (parent->dragging_chart) = true;
                    
                    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                        //I am using the mercator projection
                        
                        PositionProjection p_ceil_min, p_floor_max;
                        
                        (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * floor_max_lat)), &p_floor_max, true);
                        (this->*GeoToProjection)(Position(Angle(0.0), Angle(k * ceil_min_lat)), &p_ceil_min, true);
                        
                        
                        if ((y_max_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_max_start_drag) < (p_floor_max.y)) && (y_min_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max - y_min_start_drag) > (p_ceil_min.y))) {
                            //in this case, the drag operation does not end out of the min and max latitude contained in the data files
                            
                            //update x_min, ..., y_max according to the drag.
                            x_min = x_min_start_drag - ((double)((position_now_drag.x) - (position_start_drag.x))) / ((double)(size_plot_area.GetWidth())) * x_span_start_drag;
                            x_max = x_max_start_drag - ((double)((position_now_drag.x) - (position_start_drag.x))) / ((double)(size_plot_area.GetWidth())) * x_span_start_drag;
                            y_min = y_min_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max_start_drag - y_min_start_drag);
                            y_max = y_max_start_drag + ((double)((position_now_drag.y) - (position_start_drag.y))) / ((double)(size_plot_area.GetHeight())) * (y_max_start_drag - y_min_start_drag);
                            
                            if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                                (this->*Set_lambda_phi_min_max)();
                            }
                            
                            
#ifdef __APPLE__
                            //re-draw the chart
                            (this->*PreRender)();
#endif
#ifdef WIN32
                            //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
                            (parent->polygon_position_before) = (parent->polygon_position_now);
//                            parent->coastline_polygons_before.resize(parent->coastline_polygons_now.size());
                            copy_n(parent->coastline_polygons_now.begin(), parent->coastline_polygons_now.size(), parent->coastline_polygons_before.begin() );

                            position_plot_area_before = position_plot_area_now;
                            grid_before.clear();
                            grid_before = grid_now;
                            ticks_before.clear();
                            ticks_before = ticks_now;
                            
                            //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
                            points_route_list_before.clear();
                            points_route_list_before = points_route_list_now;
                            
                            points_position_list_before.clear();
                            points_position_list_before = points_position_list_now;
                            
                            reference_positions_route_list_before.clear();
                            reference_positions_route_list_before = reference_positions_route_list_now;
                            
                            //re-draw the chart
                            (this->*PreRender)();
                            
#endif
                            MyRefresh();
                            //							FitAll();
                            
                        }
                        
                    }
                    
                    if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
                        //I am using the 3d projection
                        
                        //compose rotation_start_drag with the rotation resulting from the drag, so as to rotate the entire earth according to the mouse drag
                        rotation.set(rotation_start_end(position_start_drag, position_now_drag) * rotation_start_drag);
#ifdef __APPLE__
                        
                        //re-render the chart
                        (this->*PreRender)();
#endif
#ifdef WIN32
                        //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
                        (parent->polygon_position_before) = (parent->polygon_position_now);
//                        parent->coastline_polygons_before.resize(parent->coastline_polygons_now.size());
                        copy_n(parent->coastline_polygons_now.begin(), parent->coastline_polygons_now.size(), parent->coastline_polygons_before.begin() );

                        position_plot_area_before = position_plot_area_now;
                        grid_before.clear();
                        grid_before = grid_now;
                        ticks_before.clear();
                        ticks_before = ticks_now;
                        
                        //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
                        points_route_list_before.clear();
                        points_route_list_before = points_route_list_now;
                        
                        points_position_list_before.clear();
                        points_position_list_before = points_position_list_now;
                        
                        reference_positions_route_list_before.clear();
                        reference_positions_route_list_before = reference_positions_route_list_now;
                        
                        //re-draw the chart
                        (this->*PreRender)();
                        
#endif
                        MyRefresh();
                        
                    }
                    
                    
                }
                else {
                    //an object is being dragged (a Position or a Route)
                    
                    unsigned int i;
                    
                    (parent->parent->dragging_object) = true;
                    
                    //the data in the file are being modified -> I call
                    parent->parent->OnModifyFile();
                    
                    if ((parent->parent->highlighted_route_now) != -1) {
                        //a Route is being dragged
                        
                        wxPoint q;
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                            
                            wxPoint p;
                            
                            //convert the coordinates of route_reference_position_drag_now into DrawPanel coordinates, shift these coordinates according to the mouse drag, and  assign the resulting point to the starting (ground) Position of the Route under consideration if the Route is a loxodrome or orthodrome (circle of equal altitude): in this way, the whole Route under consideration is dragged along with the mouse
                            
                            GeoToDrawPanel(route_reference_position_drag_start, &p, false);
                            
                            //this command is the same for all types of Routes
                            DrawPanelToGeo(p + (position_now_drag - position_start_drag), &((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).reference_position));
                            
                        }
                        
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
                            
                            //compose rotation with the rotation resulting from the drag and then apply it to route_reference_position_drag_now: route_reference_position_drag_now -> rotation^{-1}.(rotation due to drag).rotation.route_reference_position_drag_now. In this way, when Render() will plot the position route_reference_position_drag_now, it will apply to route_reference_position_drag_now the global rotation  'rotation' again, and the result will be rotation . rotation^{-1}.(rotation due to drag).rotation.route_reference_position_drag_now = (rotation due to drag).rotation.route_reference_position_drag_now, which is the desired result (i.e. route_reference_position_drag_now rotated by the global rotation 'rotation', and then rotated by the rotation due to the drag)
                            rotation_now_drag.set(
                            (rotation.inverse()) *
                            rotation_start_end(position_start_drag, position_now_drag) *
                            rotation);
                            
                            //                    (this->*GeoToDrawPanel)(route_reference_position_drag_now, &p);
                            
                            if (((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).type) == (Route_types[2])) {
                                
                                //                        DrawPanelToGeo(p + (position_now_drag - position_start_drag), &((((parent->parent->data)->route_list)[(parent->parent->highlighted_route)]).reference_position));
                                route_reference_position_drag_start.rotate(String(""), rotation_now_drag, &((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).reference_position), String(""));
                                
                            }
                            else {
                                
                                route_reference_position_drag_start.rotate(String(""), rotation_now_drag, &((((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).reference_position), String(""));
                                
                            }
                            
                        }
                        
                        
                        
                        
                        //update the data of the Route under consideration in listcontrol_routes
                        (((parent->parent->data)->route_list)[(parent->parent->highlighted_route_now)]).update_wxListCtrl((parent->parent->highlighted_route_now), parent->parent->listcontrol_routes);
                        
                        
                        for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                            //on APPLE, I compute the coordinates of the reference position of the Route that is being dragged and I call Refresh(), because Refresh() is fast. On WIN32 Refresh() is slow -> I use the RefreshWIN32() method, which wipes out graphical objects at the preceeding instant of time by drawing on them with color wxGetApp().background_color, and then renders the objects at the present instant of time with color wxGetApp().foreground_color
                            
#ifdef _WIN32
                            
                            //store the string with the coordinated of the object that is being dragged into label_dragged_position and its position into position_label_dragged_position, so PaintEvent will read it and draw the label of its coordinates on it
                            (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now);
                            (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now);
                            
#endif
                            
                            //obtain the coordinates of the reference position of the Route that is being dragged
                            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                                                                                                 ((parent->parent->data->route_list)[(parent->parent->highlighted_route_now)]).reference_position,
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now),
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now)
                                                                                                 );
                            
#ifdef __APPLE__
                            
                            //given that the Route under consideration has changed, I re-tabulate the Routes and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulateRoutes();
                            
#endif
#ifdef _WIN32
                            
                            //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before, for all DrawPanels
                            ((parent->parent->chart_frames)[i])->draw_panel->points_route_list_before.clear();
                            (((parent->parent->chart_frames)[i])->draw_panel->points_route_list_before) = (((parent->parent->chart_frames)[i])->draw_panel->points_route_list_now);
                            
                            ((parent->parent->chart_frames)[i])->draw_panel->reference_positions_route_list_before.clear();
                            (((parent->parent->chart_frames)[i])->draw_panel->reference_positions_route_list_before) = (((parent->parent->chart_frames)[i])->draw_panel->reference_positions_route_list_now);
                            
                            
                            //given that the Route under consideration has changed, I re-tabulate the Routes and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulateRoutes();
                            
#endif
                            
                            ((parent->parent->chart_frames)[i])->draw_panel->MyRefresh();
                            
                            
                        }
                        
                    }
                    
                    if ((parent->parent->highlighted_position_now) != -1) {
                        //a Position is being dragged
                        
                        wxPoint p;
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[0]).value))) {
                            
                            //convert the coordinates of position_now_drag into geographic coordinates, and assign these to the Position under consideration: in this way, the Position under consideration is dragged along with the mouse
                            (this->*ScreenToGeo)(position_now_drag, &(((parent->parent->data)->position_list)[(parent->parent->highlighted_position_now)]));
                            
                        }
                        
                        if ((((parent->projection)->name)->GetValue()) == wxString(((Projection_types[1]).value))) {
                            
                            //compose rotation with the rotation resulting from the drag and then apply it to pp == &(((parent->parent->data)->position_list)[(parent->parent->highlighted_position_now)]): pp -> rotation^{-1}.(rotation due to drag).rotation.pp. In this way, when Render() will plot the position pp, it will apply to pp the global rotation  'rotation' again, and the result will be rotation . rotation^{-1}.(rotation due to drag).rotation.pp = (rotation due to drag).rotation.pp, which is the desired result (i.e. pp rotated by the global rotation 'rotation', and then rotated by the rotation due to the drag)
                            rotation_now_drag =
                            (rotation.inverse()) *
                            rotation_start_end(position_start_drag, position_now_drag) *
                            rotation;
                            geo_start_drag.rotate(String(""), rotation_now_drag, &((parent->parent->data->position_list)[(parent->parent->highlighted_position_now)]), String(""));
                            
                        }
                        
                        //update the data of the Position under consideration in listcontrol_positions
                        ((parent->parent->data->position_list)[(parent->parent->highlighted_position_now)]).update_wxListCtrl((parent->parent->highlighted_position_now), parent->parent->listcontrol_positions);
                        
                        //given that the Position under consideration has changed, I re-paint the charts
                        for (i = 0; i < (parent->parent->chart_frames).size(); i++) {
                            //on APPLE, I compute the coordinates of the Position that is being dragged and I call Refresh(), because Refresh() is fast. On WIN32 Refresh() is slow ->  I use the RefreshWIN32() method, which wipes out graphical objects at the preceeding instant of time by drawing on them with color wxGetApp().background_color, and then renders the objects at the present instant of time with color wxGetApp().foreground_color
                            
                            
#ifdef _WIN32
                            
                            //store the string with the coordinated of the object that is being dragged into label_dragged_position and its position into position_label_dragged_position, so PaintEvent will read it and draw the label of its coordinates on it
                            (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now);
                            (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_before) = (((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now);
                            
                            
#endif
                            //obtain the coordinates of the reference position of the Route that is being dragged
                            ((parent->parent->chart_frames)[i])->draw_panel->SetLabelAndPosition(
                                                                                                 (parent->parent->data->position_list)[(parent->parent->highlighted_position_now)],
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->position_label_dragged_object_now),
                                                                                                 &(((parent->parent->chart_frames)[i])->draw_panel->label_dragged_object_now)
                                                                                                 );
                            
#ifdef __APPLE__
                            
                            //given that the Positions under consideration has changed, I re-tabulate the Positions and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulatePositions();
                            
#endif
#ifdef _WIN32
                            
                            ((parent->parent->chart_frames)[i])->draw_panel->points_position_list_before.clear();
                            (((parent->parent->chart_frames)[i])->draw_panel->points_position_list_before) = (((parent->parent->chart_frames)[i])->draw_panel->points_position_list_now);
                            
                            //given that the Positions under consideration has changed, I re-tabulate the Positions and re-render the charts
                            ((parent->parent->chart_frames)[i])->draw_panel->TabulatePositions();
                            
#endif
                            (((parent->parent->chart_frames)[i])->draw_panel)->MyRefresh();
                            
                            
                        }
                        
                    }
                    
                }
                
            }
            else {
                //in this case, position_drag_now is not a valid position : in the Mercator projection, this does not make sense and I do nothing. In the 3D projection, I am dragging the chart from outside circle observer (I am rotating the earth) -> I proceed implementing this rotation
                
                switch (position_in_vector(Projection((parent->projection->name->GetValue().ToStdString())), Projection_types)) {
                        
                    case 0: {
                        //I am using the mercator projection: then the position is invalid and I may print an error message
                        
                        //uncomment this if you want an info message to be printed
                        //print_error_message->SetAndCall(NULL,  String("The drag goes through an invalid point!"), String("The drag must go through valid points."));
                        
                        break;
                        
                    }
                        
                    case 1: {
                        //I am using the 3d projection: even if the Position lies outside the circular boundary of the Earth,  thus this Position is a valid position for a drag which rotates the earth about the y' axis -> I do this rotation
                        
                        (parent->dragging_chart) = true;
                        
                        //compose rotation_start_drag with the rotation resulting from the drag, so as to rotate the entire earth according to the mouse drag
                        rotation.set(rotation_start_end(position_start_drag, position_now_drag) * rotation_start_drag);
                        
                        
                        
#ifdef __APPLE__
                        //re-draw the chart
                        (this->*PreRender)();
#endif
#ifdef _WIN32
                        //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
                        (parent->polygon_position_before) = (parent->polygon_position_now);
//                        parent->coastline_polygons_before.resize(parent->coastline_polygons_now.size());
                        copy_n(parent->coastline_polygons_now.begin(), parent->coastline_polygons_now.size(), parent->coastline_polygons_before.begin() );
                        
                        position_plot_area_before = position_plot_area_now;
                        grid_before.clear();
                        grid_before = grid_now;
                        ticks_before.clear();
                        ticks_before = ticks_now;
                        
                        //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
                        points_route_list_before.clear();
                        points_route_list_before = points_route_list_now;
                        
                        points_position_list_before.clear();
                        points_position_list_before = points_position_list_now;
                        
                        reference_positions_route_list_before.clear();
                        reference_positions_route_list_before = reference_positions_route_list_now;
                        
                        //re-draw the chart
                        (this->*PreRender)();
                        
#endif
                        MyRefresh();
                        
                        FitAll();
                        
                        break;
                        
                    }
                        
                        
                }
                
            }
            
        }
        
    }
    
    event.Skip(true);
    
}


//this function is called whenever mouse wheel is turned on *this
void DrawPanel::OnMouseWheel(wxMouseEvent& event) {

    int i, j;

    j = (event.GetWheelRotation());

    //    cout << "\n\n\nWheel rotation = " << event.GetWheelRotation() << "\n";
    //    cout << "Slider value old = " << ((parent->slider)->GetValue()) << "\n";
    //    //    cout << "Zoom factor max = " << ((wxGetApp().zoom_factor_max).value) << "\n";
    //    cout << "A = " << (-1 + ((parent->slider)->GetValue())) << "\n";
    //    cout << "B = " << ((double)j)/(event.GetWheelDelta()) << "\n";
    //    cout << "(int)(A*B) = " << ((int)((-1.0 + ((parent->slider)->GetValue())) * ((double)j)/(event.GetWheelDelta()))) << "\n";
    
    i = ((int)((parent->slider)->GetValue())) /*I put the minus sign here because a zoom-in scroll with the mouse wheel corresponds to a negative j*/ - j;

    //    cout << "Slider value new = " << i << "\n";

    //if i gets out of range, put it back in the correct range
    if (i < 1) { i = 1; }
    if (i > ((parent->slider)->GetMax())) { i = ((parent->slider)->GetMax()); }

    parent->SetSlider(i);

    event.Skip(true);

}


template<class T> void ChartFrame::OnScroll(/*wxScrollEvent*/ T& event) {

    /*
     n = value of slider,
     z = zoom factor,

     1 <= z <= (wxGetApp().zoom_factor_max)
     1 <= n <= n_max

     n = ((double)(slider->GetValue()));
     n_max = ((double)(slider->GetMax()))

     z = (wxGetApp().a_zoom)*n^(wxGetApp().e_zoom) + (wxGetApp().b_zoom)
     n = log((z-(wxGetApp().b_zoom))/(wxGetApp().a_zoom))/(wxGetApp().e_zoom)


     (wxGetApp().a_zoom) = (-1 + (wxGetApp().zoom_factor_max))/(-1 + n_max^(wxGetApp().e_zoom));
     (wxGetApp().b_zoom) = (n_max^(wxGetApp().e_zoom) - (wxGetApp().zoom_factor_max))/(-1 + n_max^(wxGetApp().e_zoom));

     z = w/delta_x / (w_0/delta_x_0)

     delta_x = w/z/ (w_0/delta_x_0) = w*delta_x_0/(z*w_0)

     (size_chart.GetHeight())/(size_chart.GetWidth()) * x_span = (y_max-y_min);


     */


     //    cout << "Slider getvalue = " << ((double)(slider->GetValue())) << "\n";
     //     cout << "Zoom factor = " << ((double)zoom_factor.value) << "\n";

     //set zoom_factor from the value of slider
    zoom_factor.set(
        String(""),
        ((wxGetApp().a_zoom).value) * pow((slider->GetValue()), (wxGetApp().e_zoom).value) + ((wxGetApp().b_zoom).value)
        ,
        String("")
    );

    //if the resulting value of zoom_factor is outside the boundaries I set it back to the rspective boundary
    if (((zoom_factor.value) < 1.0) || ((slider->GetValue()) == (slider->GetMin()))) {

        (zoom_factor.value) = 1.0;

    }

    if (((zoom_factor.value) > (wxGetApp().zoom_factor_max.value)) || ((slider->GetValue()) == (slider->GetMax()))) {

        (zoom_factor.value) = (wxGetApp().zoom_factor_max.value);

    }

    
#ifdef WIN32
    //I am on WIN32 operating system: I will refresh the plot under the scroll operation, where I will wipe out the graphical objects in the former plot by drawing with background_color on top of them -> I need to keep track of the _before graphical objects and on the current _now graphical objects, and I do it here:
    
    //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
    polygon_position_before = polygon_position_now;
//    coastline_polygons_before.resize(coastline_polygons_now.size());
    copy_n(coastline_polygons_now.begin(), coastline_polygons_now.size(), coastline_polygons_before.begin());

    
    (draw_panel->position_plot_area_before) = (draw_panel->position_plot_area_now);
    draw_panel->grid_before.clear();
    (draw_panel->grid_before) = (draw_panel->grid_now);
    draw_panel->ticks_before.clear();
    (draw_panel->ticks_before) = (draw_panel->ticks_now);
    
    (draw_panel->parallels_and_meridians_labels_before) = (draw_panel->parallels_and_meridians_labels_now);
    (draw_panel->positions_parallels_and_meridians_labels_before) = (draw_panel->positions_parallels_and_meridians_labels_now);

    //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
    draw_panel->points_route_list_before.clear();
    (draw_panel->points_route_list_before) = (draw_panel->points_route_list_now);
    
    draw_panel->points_position_list_before.clear();
    (draw_panel->points_position_list_before) = (draw_panel->points_position_list_now);
    
    draw_panel->reference_positions_route_list_before.clear();
    (draw_panel->reference_positions_route_list_before) = (draw_panel->reference_positions_route_list_now);
    
#endif



    if ((projection->name->GetValue()) == wxString(((Projection_types[0]).value))) {

        PositionProjection p_min, p_max;

        //update x_min, ..., y_max according to the zoom (scroll) and lambda_min, ..., phi_max
        (draw_panel->x_min) = ((double)((draw_panel->x_center_scrolling))) - (((double)(((draw_panel->size_chart).GetWidth()) * (draw_panel->x_span_0))) / ((double)(((zoom_factor.value) * (draw_panel->width_chart_0))))) / 2.0;
        (draw_panel->x_max) = ((double)((draw_panel->x_center_scrolling))) + (((double)(((draw_panel->size_chart).GetWidth()) * (draw_panel->x_span_0))) / ((double)(((zoom_factor.value) * (draw_panel->width_chart_0))))) / 2.0;
        (draw_panel->y_min) = ((double)((draw_panel->y_center_scrolling))) - (((double)(((draw_panel->size_chart).GetHeight()) * (draw_panel->x_span()))) / ((double)((draw_panel->size_chart).GetWidth()))) / 2.0;
        (draw_panel->y_max) = ((double)((draw_panel->y_center_scrolling))) + (((double)(((draw_panel->size_chart).GetHeight()) * (draw_panel->x_span()))) / ((double)((draw_panel->size_chart).GetWidth()))) / 2.0;

        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(max_lat)), &p_max, true);
        (draw_panel->*(draw_panel->GeoToProjection))(Position(Angle(0.0), Angle(min_lat)), &p_min, true);

        if ((((draw_panel->y_max) <= (p_max.y)) && ((draw_panel->y_min) >= (p_min.y)) && ((draw_panel->x_span()) <= 2.0 * M_PI))) {

            (draw_panel->*(draw_panel->Set_lambda_phi_min_max))();
            //            ComputeZoomFactor_Mercator((draw_panel->x_span));

            (draw_panel->*(draw_panel->PreRender))();
            draw_panel->MyRefresh();
            draw_panel->FitAll();
            UpdateSlider();

        }
        else {
            //if the drag operation brings the chart out of the min and max latitude contained in the data files, I reset x_min, ..., y_max and the value of the slider to the values at the beginning of the drag, and set lambda_min, ..., phi_max accordingly.

            //uncomment this if you want to print an error message
            //print_error_message->SetAndCall(NULL, String("Error"), String("You moved the slider: Chart outside  boundaries! The chart must lie within the boundaries."));

            //I reset the chart to its original configuration
            Reset<T>(event);

        }

    }

    if ((projection->name->GetValue()) == wxString(((Projection_types[1]).value))) {

        (draw_panel->circle_observer.omega) = ((parent->circle_observer_0.omega) / (zoom_factor.value));

        (draw_panel->*(draw_panel->PreRender))();
        draw_panel->MyRefresh();
        draw_panel->FitAll();

        UpdateSlider();

    }


    event.Skip(true);

}

//If slider->GetMin() <= slider_value <= slider->GetMax() it returns true, and sets slider to slider_value and updates the chart zoom accordingly. It returns false otherwise
bool ChartFrame::SetSlider(unsigned int slider_value) {

    if ((slider_value >= ((unsigned int)(slider->GetMin()))) && (slider_value <= ((unsigned int)(slider->GetMax())))) {

        wxCommandEvent dummy;

        if (!mouse_scrolling) {
            OnMouseLeftDownOnSlider(dummy);
        }
        slider->SetValue(slider_value);

        //call OnScroll to update evrything adter the change of the value of slider
        OnScroll(dummy);
        OnMouseLeftUpOnSlider(dummy);

        return true;

    }
    else {

        return false;

    }


}

//writes to the non-GUI field angle the values written in the GUI fields sign, deg and min
template<class P> template <class T> void AngleField<P>::get(T& event) {

    if (is_ok()) {


        double min_temp;
        char c;

        //set a value to c to avoid uninitialized-variable warning
        c = ' ';

        if (format == String("")) {
            //in this case there is no sign in AngleField->this:

            c = '+';

        }
        else {
            //in this case there is a sign in AngleField->this: I write the sign in c

            wxString s;
            s = (sign->GetValue());

            if (format == String("+-")) {

                if (s == wxString("+")) { c = '+'; }
                else { c = '-'; }

            }

            if (format == String("EW")) {

                if (s == wxString("W")) { c = '+'; }
                else { c = '-'; }

            }

            if (format == String("NS")) {

                if (s == wxString("N")) { c = '+'; }
                else { c = '-'; }

            }

        }


        (min->GetValue()).ToDouble(&min_temp);

        angle->from_sign_deg_min(c, wxAtoi(deg->GetValue()), min_temp);

    }

    event.Skip(true);

}


//constructor of the struct, which initializes the Answer remove_related_route. If remove_related_route.value = 'y', then DeleteSight::operator() will delete both the sight and the related route. If remove_related_route.value = 'n', then it will remove the sight only.
DeleteSight::DeleteSight(ListFrame* f_in, Answer remove_related_route_in) {

    f = f_in;
    remove_related_route = remove_related_route_in;

}

ExistingRoute::ExistingRoute(ListFrame* f_in) {

    f = f_in;

}

NewRoute::NewRoute(ListFrame* f_in) {

    f = f_in;

}

AllRoutes::AllRoutes(ListFrame* f_in) {

    f = f_in;

}


SomeRoutes::SomeRoutes(ListFrame* f_in) {

    f = f_in;

}


template<class P> SetIdling<P>::SetIdling(P* parent_in) {

    parent = parent_in;

}

template<class P> UnsetIdling<P>::UnsetIdling(P* parent_in) {

    parent = parent_in;

}




AskRemoveRelatedSight::AskRemoveRelatedSight(ListFrame* parent_in) {

    parent = parent_in;

}

AskRemoveRelatedRoute::AskRemoveRelatedRoute(ListFrame* parent_in) {

    parent = parent_in;

}


SelectRoute::SelectRoute(ListFrame* parent_in) {

    parent = parent_in;

}


template<class P> FunctionOnPressOk<P>::FunctionOnPressOk(P* parent_in) {

    parent = parent_in;

}



void DeleteSight::operator()(wxCommandEvent& event) {

    //I remove the sight and the related route from  the non-GUI object data
    (f->data)->remove_sight(((unsigned int)i_sight_to_remove), remove_related_route, String(""));

    (f->listcontrol_sights)->set((f->data)->sight_list, false);
    (f->listcontrol_routes)->set((f->data)->route_list, false);

    //given that I called set in listcontrol_sights, no item is selected in listcontrol_sights, I call:
    (*(f->on_change_selection_in_listcontrol_sights))(event);

    if (remove_related_route == Answer('y', String(""))) {

        //given that I called set for listcontrol_routes, no item is selected in listcontrol_routes -> I call:
        (*(f->on_change_selection_in_listcontrol_routes))(event);
    }

    f->Resize();

    event.Skip(true);

}

void ExistingRoute::operator()(wxCommandEvent& event) {

    //by setting this to true, when the user presses enter, the method ListFrame::KeyDown will call on_select_route_in_listcontrol_routes_for_transport
    (f->transporting_with_selected_route) = true;

    //save data->route_list into route_list_saved
    f->route_list_saved.resize(((f->data)->route_list).size());
    copy((f->data->route_list).begin(), (f->data->route_list).end(), (f->route_list_saved).begin());

    //print an info message
    (f->print_question_message)->SetAndCall(NULL, String(""), String("You are about to transport with an existing route. Select the Route and press enter.\nDo you want to continue?"), String("Yes"), String("No, I want to cancel"));

    event.Skip(true);

}

void AllRoutes::operator()(wxCommandEvent& event) {

    unsigned int j;

    //there need to be at list two routes of type ((Route_types[2]).value) to compute crossings. Here I include all routes of type ((Route_types[2]).value) whhich are related to a Sight into crossing_route_list by writing their index into crossing_route_list
    for (((f->data)->crossing_route_list).clear(), j = 0; j < ((f->data)->route_list).size(); j++) {

        if ((((((f->data)->route_list))[j]).type.value) == ((Route_types[2]).value)) {
            ((f->data)->crossing_route_list).push_back(((((f->data)->route_list))[j]));
        }

    }

    f->OnComputePosition();

    event.Skip(true);

}

void SomeRoutes::operator()(wxCommandEvent& event) {


    int i;

    f->print_info_message->SetAndCall(NULL, String(""), String("Select the routes that you want to use to compute the astronomical position and press enter when done"), (wxGetApp().path_file_info_icon));

    //Given that a sight must be transported only with a Route that does not come from a Sight and a Route that is not a circle of equal altitude (it would not make sense), I store in route_list_for_transport the Routes in route_list which are not related to any sight and that are not circles of equal altitude, show route_list_for_transport in listcontrol_routes, and let the user select one item in route_list_for_transport to transport the Sight
    for ((f->crossing_route_list_temp.clear()), i = 0; i < (f->data->route_list).size(); i++) {

        if ((((f->data)->route_list)[i]).type == (Route_types[2])) {
            f->crossing_route_list_temp.push_back(((f->data->route_list)[i]));
        }

    }

    //setting this to true, now when the enter key is pressed the selected Routes are used to compute the position
    (f->selecting_route_for_position) = true;
    (f->listcontrol_routes)->set(f->crossing_route_list_temp, false);
    //I bind listcontrol_routes to on_select_route_in_listcontrol_routes_for_position in such a way that when the user will select an item in listcontrol, I perform the computation of the position
    //    (f->listcontrol_routes)->Bind(wxEVT_LIST_ITEM_SELECTED, *(f->on_select_route_in_listcontrol_routes_for_position));




    event.Skip(true);

}


void NewRoute::operator()(wxCommandEvent& event) {

    (f->transporting_with_new_route) = true;
    //call OnAddRoute to add a new Route
    (f->OnAddRouteForTransport)(event);

    //when button_ok in f->route_fram will be pressed, I call on_new_route_in_listcontrol_routes_for_transport to execute the transport with this Route
    //    f->route_frame->button_ok->Bind(wxEVT_BUTTON, *(f->on_new_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}


template<class P> void UnsetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = false;

    event.Skip(true);

}

//this is the same as template<class P> void UnsetIdling<P>::operator()(void){ but without the event argument
template<class P> void UnsetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}


template<class P> ConfirmTransport<P>::ConfirmTransport(P* parent_in) {

    parent = parent_in;

}


//I call this method when the user has decided to confirm that he/she wants to make the transport of an object
template<class P> void ConfirmTransport<P>::operator()(wxCommandEvent& event) {

    int i;

    //Given that an object must be transported only with a Route that does not come from a Sight and a Route that is not a circle of equal altitude (it would not make sense), I store in route_list_for_transport the Routes in route_list which are viable to be transporting Routes. These are the Routes that: 1. are not related to any sight, 2. that are not circles of equal altitude 3. That are different from  show route_list_for_transport in listcontrol_routes, and let the user select one item in route_list_for_transport to transport the Sight
    for ((parent->route_list_for_transport).clear(), (parent->map).clear(), i = 0; i < (parent->data->route_list).size(); i++) {

        if (
            /*condition that the Route is not relatied to a Sight*/
            (((((parent->data->route_list)[i]).related_sight).value) == -1) &&
            /*condition that the Route is not a circle of equal altitude*/
            (((parent->data->route_list)[i]).type != (Route_types[2])) &&
            /*condition that the Route does not coincide with the object to transport*/
            (((parent->transported_object_type) != String("route")) || ((parent->i_object_to_transport) != i))
            ) {

            (parent->route_list_for_transport).push_back((parent->data->route_list)[i]);
            (parent->map).push_back(i);

        }

    }

    parent->listcontrol_routes->set((parent->route_list_for_transport), false);
    parent->data->route_list.resize((parent->route_list_for_transport).size());
    copy((parent->route_list_for_transport).begin(), (parent->route_list_for_transport).end(), ((parent->data)->route_list).begin());
    parent->TabulateRoutesAll();
    parent->PreRenderAll();

    //I bind listcontrol_routes to on_select_route_in_listcontrol_routes_for_transport in such a way that when the user will double clock on an item in listcontrol (or single-click it and then press enter), I perform the transport
    parent->listcontrol_routes->Unbind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, parent);
    parent->listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, *(parent->on_select_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}

//this is the same as template<class P> void ConfirmTransport<P>::operator()(void){ but without the event argument)
template<class P> void ConfirmTransport<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}


template<class P> void SetIdling<P>::operator()(wxCommandEvent& event) {

    (parent->idling) = true;

    event.Skip(true);

}

//this is the same as template<class P> void SetIdling<P>::operator()(wxCommandEvent& event){ but without the event argument
template<class P> void SetIdling<P>::operator()(void) {

    wxCommandEvent dummy;

    (*this)(dummy);

}


void AskRemoveRelatedSight::operator()(wxCommandEvent& event) {

    int i_route_to_remove;

    //set i_route_to_remove equal to the currently relected Route in listcontrol_routes
    i_route_to_remove = ((int)(parent->listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    (parent->delete_route_and_related_sight->i_route_to_remove) = i_route_to_remove;
    (parent->delete_route->i_route_to_remove) = i_route_to_remove;


    if (((((parent->data->route_list)[i_route_to_remove]).related_sight).value) != -1) {
        //if the Route which I am about to remove is related to a Sight, I ask the user whether he wants to remove the related Sight too by showing a question_frame

        ShowQuestionFrame<ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame> >* print_question;

        print_question = new ShowQuestionFrame< ListFrame, DeleteRoute, DeleteRoute, UnsetIdling<ListFrame> >(parent, parent->delete_route_and_related_sight, parent->delete_route, parent->unset_idling);

        print_question->SetAndCall(NULL, String(""), String("The route that you are about to remove is related to a sight. Do you want to remove the sight related to this route?"), String("Yes"), String("No"));

    }else{
        //if not, I simply delete teh route

        (*(parent->delete_route))(event);

    }

    parent->OnModifyFile();

    event.Skip(true);

}

void AskRemoveRelatedRoute::operator()(wxCommandEvent& event) {

    int i_sight_to_remove;

    //set i_sight_to_remove equal to the currently relected Sight in listcontrol_sights
    i_sight_to_remove = ((int)((parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    ((parent->delete_sight)->i_sight_to_remove) = i_sight_to_remove;


    if ((((((parent->data)->sight_list)[i_sight_to_remove]).related_route).value) != -1) {
        //if the sight which I am about to remove is related to a Route, I ask the user whether he wants to remove the related Route too by showing  question_frame

        ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>* print_question;

        ((parent->delete_route_and_related_sight)->i_route_to_remove) = (((((parent->data)->sight_list)[i_sight_to_remove]).related_route).value);

        //remove the route from the non-GUI object data
        //ask the user whether he/she wants to remove the related sight as well: if the answer is yes, then QuestionFrame calls the functor delete_sight_and_related_sight. If no, it calls the functor delete_sight.

        print_question = new ShowQuestionFrame<ListFrame, DeleteRoute, DeleteSight, UnsetIdling<ListFrame>>(parent, parent->delete_route_and_related_sight, parent->delete_sight, parent->unset_idling);

        print_question->SetAndCall(NULL, String(""), String("Do you want to remove the route related to this sight??"), String("Yes"), String("No"));


    }
    else {
        //if not, I simply delete teh sight

        (*(parent->delete_sight))(event);

    }

    parent->OnModifyFile();

    event.Skip(true);

}



void SelectRoute::operator()(wxCommandEvent& event) {

    (parent->idling) = true;

    //temporarily unbind listcontrol_routes from &ListFrame::OnChangeSelectionInListControl
    (parent->listcontrol_routes)->Unbind(wxEVT_LIST_ITEM_SELECTED, *(parent->on_change_selection_in_listcontrol_routes));
    (parent->listcontrol_routes)->Unbind(wxEVT_LIST_ITEM_DESELECTED, *(parent->on_change_selection_in_listcontrol_routes));



    //brings parent to front
    parent->Raise();

    //deselect all previously selected items in listcontrol_routes to allow the user to properly select an item
    (parent->listcontrol_routes)->DeselectAll();

    //	(parent->listcontrol_routes)->Bind(wxEVT_LIST_ITEM_SELECTED, *(parent->on_select_route_in_listcontrol_routes_for_transport));

    event.Skip(true);

}




template<class P> void FunctionOnPressOk<P>::operator()(wxCommandEvent& event) {

    //do something

    event.Skip(true);

}


//constructor of the struct, which initializes the Answer remove_related_sight. If remove_related_sight.value = 'y', then DeleteRoute::operator() will delete both the route and the related sight. If remove_related_sight.value = 'n', then it will remove the route only.
DeleteRoute::DeleteRoute(ListFrame* f_in, Answer remove_related_sight_in) {

    f = f_in;
    remove_related_sight = remove_related_sight_in;

}

void DeleteRoute::operator()(wxCommandEvent& event) {

    int i_related_sight;

    i_related_sight = ((((f->data)->route_list)[i_route_to_remove]).related_sight).value;

    //remove the sight related to the route which I am about to remove from the GUI object listcontrol_sights
    if ((i_related_sight != -1) && (remove_related_sight == Answer('y', String("")))) {

        //        (f->listcontrol_sights)->DeleteItem(i_related_sight);

        //given that after one item is deleted in listcontrol_sights, now no item is selected in listcontrol_sights, I call:
        (*(f->on_change_selection_in_listcontrol_sights))(event);


    }

    //I remove the route and the related sight from both the non-GUI object data
    (f->data)->remove_route(((unsigned int)i_route_to_remove), remove_related_sight, String(""));

    (f->listcontrol_sights)->set((f->data)->sight_list, false);
    (f->listcontrol_routes)->set((f->data)->route_list, false);

    //given that I called set in listcontrol_routes, now no item is selected in listcontrol_routes, I call:
    (*(f->on_change_selection_in_listcontrol_routes))(event);
    f->Resize();
    //given that a Route has been removed, I re-draw everything
    f->PreRenderAll();

    event.Skip(true);

}


DeletePosition::DeletePosition(ListFrame* f_in) {

    f = f_in;

}

//delete the  selected position in the GUI object f->listcontrol_position and in the non-GUI object f->data. If no position is selected, it does nothing.
void DeletePosition::operator()(wxCommandEvent& event) {

    long i;

    i = (f->listcontrol_positions)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (i != -1) {

        (f->data)->remove_position(((unsigned int)i), String(""));
        (f->listcontrol_positions)->set((f->data)->position_list, false);

        //given that I called set in listcontrol_positions, now no item is selected in listcontrol_positions-> I call:
        (*(f->on_change_selection_in_listcontrol_positions))(event);
        f->Resize();
        f->OnModifyFile();

    }

    event.Skip(true);

}


//set *this to the current time
void String::set_to_current_time(void) {

    Time now;

    now.set_current();
    //I write in the non-GUI object *this

    set(String("String set to current time"), String(now.to_string(data_precision.value, true)), String(""));

}



template<class P> template <class T> void SetStringFieldToCurrentTime<P>::operator()(T& event) {

    //if the label is empty, I replace it with the local time and date
    if ((p->value->GetValue()).IsEmpty()) {

        Time now;

        now.set_current();
        //I write in the non-GUI object (p->string)
        (*(p->string)) = String(now.to_string(data_precision.value, true));

        p->set();

    }

    event.Skip(true);

}

template<class P> CheckBody<P>::CheckBody(BodyField<P>* p_in) {

    p = p_in;

}


ResetListFrame::ResetListFrame(ListFrame* p_in) {

    p = p_in;

}


//reset *this by destroying this->data, and allocating a new one
template <class T> void ResetListFrame::operator()(T& event) {

    //clear p->data and allocate a new one
    p->data->~Data();
    //the file now has no title and has not been modified
    (p->file_is_untitled) = true;
    (p->file_has_been_modified) = false;

    p->data = new Data(p->catalog, String(""));

    //empty all listcontrols
    p->listcontrol_sights->DeleteAllItems();
    p->listcontrol_positions->DeleteAllItems();
    p->listcontrol_routes->DeleteAllItems();

    //resize, set an 'untitled' label for the new, empty ListFrame, and update the chartframes
    p->Resize();
    p->SetLabel(wxString("untitled"));
    p->PreRenderAll();

    event.Skip(true);

}



template<class F> SaveAndReset<F>::SaveAndReset(F* frame_in) {

    frame = frame_in;

}


CloseApp::CloseApp(MyApp* app_in){
    
    app = app_in;
    
}


template <class T> void CloseApp::operator()([[maybe_unused]] T& event) {
    
    app->list_frame->Close();
    app->disclaimer->Close();
    
}




template<class F> CloseFrame<F>::CloseFrame(F* frame_in) {

    frame = frame_in;

}

//closes a frame of type F
template<class F> template <class T> void CloseFrame<F>::operator()([[maybe_unused]] T& event) {

    DestroyFrame<F>* destroy_frame;

    destroy_frame = new DestroyFrame<F>(frame);

    //destroys frame
    frame->CallAfter(*destroy_frame);

    event.Skip(true);


}

template<class F> ShowFrame<F>::ShowFrame(F* frame_in) {

    frame = frame_in;

}

//show a frame of type F
template<class F> template <class T> void ShowFrame<F>::operator()(T& event) {

    frame->Show(true);

    event.Skip(true);


}

ShowAll::ShowAll(ListFrame* frame_in) {

    frame = frame_in;

}

//show ListFrame and all ChartFrames
template <class T> void ShowAll::operator()(T& event) {

    ShowFrame<ListFrame>* show_frame;
    show_frame = new ShowFrame<ListFrame>(frame);

    (*show_frame)(event);
    for(unsigned int i=0; i<wxGetApp().list_frame->chart_frames.size(); i++){
        wxGetApp().list_frame->chart_frames[i]->ResetRender(event);
    }
    wxGetApp().ShowCharts(event);
    wxGetApp().AnimateCharts();

    event.Skip(true);

}

template<class F> DestroyFrame<F>::DestroyFrame(F* frame_in) {

    frame = frame_in;

}

//destroys a frame of type F
template<class F> void DestroyFrame<F>::operator()(void) {

    //destroys frame
    frame->Destroy();

}



//saves the data in frame->data to file frame->file ,and closes frame
template<class F> template <class T> void SaveAndReset<F>::operator()(T& event) {

    ResetListFrame* reset_list_frame;

    reset_list_frame = new ResetListFrame(frame);


    if (frame->file_is_untitled) {
        //the file has no name -> save as

        frame->OnPressCtrlShiftS(event);

        //        wxFileDialog openFileDialog(frame, _(""), default_open_directory, "", "nav files (*.nav)|*.nav", wxFD_SAVE | wxFD_FILE_MUST_EXIST);
        //
        //
        //        if((openFileDialog.ShowModal()) != wxID_CANCEL){
        //            // the user did not presse cancel -> proceed saving on the file chosen by the user;
        //
        //            (frame->file).set_name(String((openFileDialog.GetPath()).ToStdString()));
        //            //open a new file to save content on it
        //            (frame->file).open(String("out"), String(""));
        //            //write frame->data into file
        //            (frame->data)->print(false, String(""), ((frame->file).value));
        //            //close the file
        //            (frame->file).close(String(""));
        //
        //        }

    }
    else {
        //the file has a name -> save

        //remove the file to avoid overwriting
        (frame->data_file).remove(String(""));
        //open a new data_file
        (frame->data_file).open(String("out"), String(""));
        //write frame->data into data_file
        (frame->data)->print(false, String(""), *((frame->data_file).value));
        //close the data_file
        (frame->data_file).close(String(""));


    }

    (*reset_list_frame)(event);

    event.Skip(true);

}

template<class P> CheckString<P>::CheckString(StringField<P>* p_in) {

    p = p_in;

}

//this functor does nothing, delete it in the future
template<class P> template<class T> void CheckString<P>::operator()(T& event) {

    P* f = (p->parent);

    f->AllOk();

    event.Skip(true);

}


//I write in the non-GUI object string the value entered in the GUI object value
template<class P> template<class T> void StringField<P>::get(T& event) {

    //here I don't check whether the StringField is ok, because any value in the string field is ok
    (*string) = String((value->GetValue().ToStdString()));

    event.Skip(true);

}

template<class P> CheckAngle<P>::CheckAngle(AngleField<P>* p_in) {

    p = p_in;

    check_sign = new CheckSign<P>(p);
    check_arc_degree = new CheckArcDegree<P>(p);
    check_arc_minute = new CheckArcMinute<P>(p);

}

//this functor checks the whole angle field by calling the check on its sign, arcdegree and arcminute parts‰
template<class P> template <class T> void CheckAngle<P>::operator()(T& event) {

    //    if((p->format) != String("")){
    (*check_sign)(event);
    //    }
    (*check_arc_degree)(event);
    (*check_arc_minute)(event);

    event.Skip(true);

}


template<class NON_GUI, class P> ToDoAtEndOfTransport<NON_GUI, P>::ToDoAtEndOfTransport(NON_GUI* object_a_in, NON_GUI* object_b_in, P* parent_in){
    
    object_a = object_a_in;
    object_b = object_b_in;
    parent = parent_in;

}


//set *object_a equal to *object_b if object_a != NULL, and to the other tasks to be done at the end of a transport
template<class NON_GUI, class P> void ToDoAtEndOfTransport<NON_GUI, P>::operator()(void){
    
    if(object_a){
        (*object_a) = (*object_b);
    }
    
    if((parent->i_object_to_disconnect) != -1){
        
        //print an info message
        parent->print_info_message->SetAndCall(NULL, String("Warning"), String("The transported route  was related to a sight! The route has been disconnected from the sight."), (wxGetApp().path_file_info_icon));
        
       (parent->i_object_to_disconnect) = -1;
        
    }
    
    parent->listcontrol_sights->set((parent->data->sight_list), false);
    parent->listcontrol_routes->set((parent->data->route_list), false);
    parent->Resize();
    parent->PreRenderAll();

}


template<class P> CheckDate<P>::CheckDate(DateField<P>* p_in) {

    p = p_in;

    check_year = new CheckYear<P>(p);
    check_month = new CheckMonth<P>(p);
    check_day = new CheckDay<P>(p);

}

//this functor checks the whole date field by calling the check on its year, month and day parts
template<class P> template <class T> void CheckDate<P>::operator()(T& event) {

    (*check_year)(event);
    (*check_month)(event);
    (*check_day)(event);

    event.Skip(true);

}

//this functor writes the values written inthe whole GUI date field (year, month and day) in the respective non-GUI object date->D, date->M, date->D
template<class P> template <class T> void DateField<P>::get(T& event) {

    if (year_ok && (year->IsEnabled()) && month_ok && (month->IsEnabled()) && day_ok && (month->IsEnabled())) {

        (date->Y) = (unsigned int)wxAtoi(year->GetValue());
        (date->M) = (unsigned int)wxAtoi(month->GetValue());
        (date->D) = (unsigned int)wxAtoi(day->GetValue());

    }

    event.Skip(true);

}

template<class P> CheckArcDegree<P>::CheckArcDegree(AngleField<P>* p_in) {

    p = p_in;

}


template<class P> template<class T> void CheckArcDegree<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->deg->GetValue()).ToStdString(), NULL, true, 0, 360);

        if (check || (((p->deg->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->deg->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set deg_ok to true (false)
            (p->deg_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            p->deg->SetForegroundColour(wxGetApp().foreground_color);
            p->deg->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->deg), String("Entered value is not valid!"), String("Arcdegrees must be unsigned integer numbers between 0 and 359").append(wxGetApp().degree_symbol), (wxGetApp().path_file_error_icon));

            (p->deg_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckArcMinute<P>::CheckArcMinute(AngleField<P>* p_in) {

    p = p_in;

}

template<class P> template <class T> void CheckArcMinute<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->min->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

        if (check || (((p->min->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->min->GetValue()).ToStdString())) == String("")))) {
            //p->min either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set min_ok to true (false)
            (p->min_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in min
            p->min->SetForegroundColour(wxGetApp().foreground_color);
            p->min->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->min), String("Entered value is not valid!"), String("Arcminutes must be floating-point numbers >= 0' and < 60'"), (wxGetApp().path_file_error_icon));

            (p->min_ok) = false;


        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckLengthValue<P>::CheckLengthValue(DynamicLengthField<P>* p_in) {

    p = p_in;

}

//checks the value in the GUI field in DynamicLengthField
template<class P> template <class T> void CheckLengthValue<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

        if (check || (((p->value->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->value->GetValue()).ToStdString())) == String("")))) {
            //p->value either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set value_ok to true (false)
            (p->value_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in value
            p->value->SetForegroundColour(wxGetApp().foreground_color);
            p->value->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->value), String("Entered value is not valid!"), String("Lengths must be floating-point numbers >= 0 m"), (wxGetApp().path_file_error_icon));

            (p->value_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckLengthUnit<P>::CheckLengthUnit(DynamicLengthField<P>* p_in) {

    p = p_in;

}

//checks the unit in the GUI field in LengthField
template<class P> template <class T> void CheckLengthUnit<P>::operator()(T& event) {

    P* f = (p->parent);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        unsigned int i;
        bool check;

        
        p->unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::CheckInCatalog(&check, &i);

        if (check || (((p->unit->name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->unit->name->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set unit_ok to true (false)
            (p->unit_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            p->unit->name->SetForegroundColour(wxGetApp().foreground_color);
            p->unit->name->SetFont(wxGetApp().default_font);


        }else {

            stringstream temp;

            temp.str("");
            temp << "Available units are: ";
            for (i = 0; i < LengthUnit_types.size(); i++) {
                temp << (LengthUnit_types[i]).value << ((i < LengthUnit_types.size() - 1) ? ", " : ".");
            }

            f->print_error_message->SetAndCall((p->unit->name), String("Unit not found in list!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            (p->unit_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckLength<P>::CheckLength(DynamicLengthField<P>* p_in) {

    p = p_in;

    check_length_value = new CheckLengthValue<P>(p);
    check_length_unit = new CheckLengthUnit<P>(p);

}

//this functor checks the whole Length field by calling the check on its value and unit
template<class P> template <class T> void CheckLength<P>::operator()(T& event) {

    (*check_length_value)(event);
    (*check_length_unit)(event);

    event.Skip(true);

}



template<class P> CheckSpeedValue<P>::CheckSpeedValue(SpeedField<P>* p_in) {

    p = p_in;

}

//check the value in the GUI field in SpeedField
template<class P> template <class T> void CheckSpeedValue<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double((p->value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

        if (check || (((p->value->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->value->GetValue()).ToStdString())) == String("")))) {
            //p->value either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set value_ok to true (false)
            (p->value_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in value
            p->value->SetForegroundColour(wxGetApp().foreground_color);
            p->value->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->value), String("Entered value is not valid!"), String("Speeds must be floating-point numbers >= 0 m"), (wxGetApp().path_file_error_icon));

            (p->value_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


//write the value of the GUI field in LengthField into the non-GUI field length
template<class P> template <class T> void DynamicLengthField<P>::get(T& event) {
    
    if (is_ok()) {
        
        double length_temp;
        
        value->GetValue().ToDouble(&length_temp);
        
        
        switch (String((LengthField<P>::unit->name->GetValue()).ToStdString()).position_in_list(LengthField<P>::unit->catalog)) {
                
            case 0: {
                //unit = "nm"
                
                LengthField<P>::length->set(String(""), /*the length is entered in the GUI field is already in nm, thus no need to convert it*/length_temp, String(""));
                
                break;
                
            }
                
                
            case 1: {
                //unit = "m"
                
                LengthField<P>::length->set(String(""), /*the length is entered in the GUI field in meters, thus I convert it to nm here*/length_temp / (1e3 * nm), String(""));
                
                break;
                
            }
                
                
            case 2: {
                //unit = "ft"
                
                LengthField<P>::length->set(String(""), /*the length is entered in the GUI field in feet, thus I convert it to nm here*/length_temp / nm_ft, String(""));
                
                break;
                
            }
                
        }
        
    }
    
    event.Skip(true);
    
}



//if an item in listcontrol_sights/positions/routes is selected, I transport the Sight/Position/Route under consideration with such Route
template<class T> void OnSelectRouteInListControlRoutesForTransport::operator()(T& event) {
    
    //do the tasks tha need to be done at the end of the transport: set_back_transporting_route is used to set the transporting Route back to its original value after the animation transport is finished, in order to avoid the accumulation of numerical errors if one transported it back
    ToDoAtEndOfTransport<Route, ListFrame>* to_do_at_end_of_transport;
    
    //now I no longer need route_list to contain only the available Routes for transport -> I put back all the Routes before the transport into route_list by copying route_list_saved into route_list.
    // PaintEvent() will need points_route_list to be updated according to this change -> I call TabulateRoutesAll() to update points_route_list
    parent->data->route_list.resize((parent->route_list_saved.size()));
    copy((parent->route_list_saved.begin()), (parent->route_list_saved.end()), (parent->data->route_list.begin()));
    parent->TabulateRoutesAll();
    
    //this is the # of the transporting Route in the full Route list given by data->route_list
    (parent->i_transporting_route) = (parent->map)[(parent->listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED))];
    
    //during the animation, (parent->data->route_list)[(parent->i_transporting_route)] will be transported -> there may be accumulating numerical errors when I transport it back -> I store it in transporting_route_saved and then set (parent->data->route_list)[(parent->i_transporting_route)] equal to transporting_route_saved at the end of the whole animation
    (parent->transporting_route_saved) = (parent->data->route_list)[(parent->i_transporting_route)];
    
    to_do_at_end_of_transport = new ToDoAtEndOfTransport<Route, ListFrame>(
                                                                  &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                  &(parent->transporting_route_saved),
                                                                  parent
                                                                  );
    
    if (((parent->transported_object_type) == String("sight")) || (parent->transported_object_type) == String("route")) {
        //I am transporting a Sight or the Route related to it: allocate transport_handler with template NON_GUI = Route
        
        String new_label;
        
        
//        GraphicalFeatureTransportHandler<Route, UnsetIdling<ListFrame> >* transport_handler;
        //auxiliary_transport_handler_inbound will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the end of the transport (inbound), to set the transporting Route back where it was at the beginning
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* auxiliary_transport_handler_inbound;
        //transport_handler does the actual, main transport of the Route
        GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >* transport_handler;
        //auxiliary_transport_handler_outbount will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the beginning of the transport (outbound). Then the actual transport of transported_object will be done, and then the transporting Route is transported back to its original position
        GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >* auxiliary_transport_handler_outbound;
        
        
        
        if ((parent->transported_object_type) == String("sight")) {
            //the transported object is a Sight
            
            //the id of the Route that will be transported is the one of the Route related to the Sight that is being transported
            (parent->i_object_to_transport) = ((((parent->data->sight_list)[(parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value);
            
        }
        
//        transport_handler = new GraphicalFeatureTransportHandler<Route, UnsetIdling<ListFrame> >(
//                                                                                                 parent,
//                                                                                                 &((parent->data->route_list)[(parent->i_object_to_transport)]),
//                                                                                                 (parent->transported_object_type),
//                                                                                                 ((parent->data->route_list)[(parent->i_transporting_route)]),
//                                                                                                 parent->unset_idling
//                                                                                                 );
        
        
        auxiliary_transport_handler_inbound = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                             &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                             String("route"),
                                                                                                             Route(RouteType(Route_types[0]),
                                                                                                                   ((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position,
                                                                                                                   (parent->transporting_route_saved).reference_position),
                                                                                                             to_do_at_end_of_transport
                                                                                                             );
        transport_handler = new GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >(
                                                                                                                                        parent,
                                                                                                                                        &((parent->data->route_list)[(parent->i_object_to_transport)]),
                                                                                                                                        (parent->transported_object_type),
                                                                                                                                        ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                                        auxiliary_transport_handler_inbound
                                                                                                                                        );
        auxiliary_transport_handler_outbound = new GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >(
                                                                                                                                                                                                     parent,
                                                                                                                                                                                                     &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                                                                                                                     String("route"),
                                                                                                                                                                                                     Route(RouteType(Route_types[0]), (parent->transporting_route_saved).reference_position, ((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position),
                                                                                                                                                                                                                      transport_handler
                                                                                                                                                                                                                      );
        
        //start the transport
        //        (*transport_handler)();
        (*auxiliary_transport_handler_outbound)();

    }
    
    if ((parent->transported_object_type) == String("position")) {
        
        //auxiliary_transport_handler_inbound will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the end of the transport (inbound), to set the transporting Route back where it was at the beginning
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* auxiliary_transport_handler_inbound;
        //transport_handler does the actual, main transport of the Position
        GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >* transport_handler;
        //auxiliary_transport_handler_outbount will be used to transport the transporting Route in such a way that its starting point coincides with the object to transport at the beginning of the transport (outbound). Then the actual transport of transported_object will be done, and then the transporting Route is transported back to its original position
        GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >* auxiliary_transport_handler_outbound;
        
        
        //        (parent->transporting_route_saved) = (parent->data->route_list)[(parent->i_transporting_route)];
        
        //the id of the Position that will be transported,
        (parent->i_object_to_transport) = ((int)((parent->listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED))));
        
        auxiliary_transport_handler_inbound = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                             &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                             String("route"),
                                                                                                             Route(RouteType(Route_types[0]),
                                                                                                                   ((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                   (parent->transporting_route_saved).reference_position),
                                                                                                             to_do_at_end_of_transport
                                                                                                             );
        transport_handler = new GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > >(
                                                                                                                                        parent,
                                                                                                                                        &((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                                        (parent->transported_object_type),
                                                                                                                                        ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                                        auxiliary_transport_handler_inbound
                                                                                                                                        );
        auxiliary_transport_handler_outbound = new GraphicalFeatureTransportHandler<Route, GraphicalFeatureTransportHandler<Position, GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> > > >(
                                                                                                                                                                                                     parent,
                                                                                                                                                                                                     &(parent->data->route_list)[(parent->i_transporting_route)],
                                                                                                                                                                                                     String("route"),
                                                                                                                                                                                                     Route(RouteType(Route_types[0]), (parent->transporting_route_saved).reference_position, ((parent->data->position_list)[(parent->i_object_to_transport)])),
                                                                                                                                                                                                     transport_handler
                                                                                                                                                                                                     );
        

        
        //these timers of auxiliary_transport_handler and transport_handler run at the same time -> change this with CallAfter and a lambda call
        //start the auxiliary transport
        (*auxiliary_transport_handler_outbound)();
        //        (*transport_handler)();
        
    }
    
//    parent->CallAfter([this, (parent->transporting_route_saved)]()->void {
//        //set (parent->data->route_list)[(parent->i_transporting_route)] equal to its value before the transport, update parent and re-draw everthing
//        (parent->data->route_list)[(parent->i_transporting_route)]  = (parent->transporting_route_saved);
//        parent->listcontrol_sights->set((parent->data->sight_list), false);
//        parent->listcontrol_routes->set((parent->data->route_list), false);
//        parent->Resize();
//        parent->PreRenderAll();
//    });
    
    event.Skip(true);
    
}


//if a new item listcontrol_routes is created, I transport the sight/position under consideration with such Route
template<class T> void OnNewRouteInListControlRoutesForTransport::operator()(T& event) {
    
    //the id of the Route that will do the transport: it is the last item in listcontrol_routes, because it is the item of the newly added Route
    (parent->i_transporting_route) = ((parent->listcontrol_routes)->GetItemCount()) - 1;
    //do the tasks tha need to be done at the end of the transport
    ToDoAtEndOfTransport<Route, ListFrame>* to_do_at_end_of_transport;

    to_do_at_end_of_transport = new ToDoAtEndOfTransport<Route, ListFrame>(
                                                                  NULL,
                                                                  NULL,
                                                                  parent
                                                                  );


    if (((parent->transported_object_type) == String("sight")) || ((parent->transported_object_type) == String("route"))) {
        //I am transporting a Sight or the Route related to it: allocate transport_handler with template NON_GUI = Route
        
        GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >* transport_handler;

        
        if ((parent->transported_object_type) == String("sight")) {
            
            
            //the id of the Route that will be transported
            (parent->i_object_to_transport) = (((((parent->data)->sight_list)[(parent->listcontrol_sights)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value);
            
            
        }
        
        
        transport_handler = new GraphicalFeatureTransportHandler<Route, ToDoAtEndOfTransport<Route, ListFrame> >(
                                                                                                                 parent,
                                                                                                                 &((parent->data->route_list)[(parent->i_object_to_transport)]),
                                                                                                                 (parent->transported_object_type),
                                                                                                                 ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                 to_do_at_end_of_transport
                                                                                                                 );
        
        //start the transport
        (*transport_handler)();
        
    }
    
    if ((parent->transported_object_type) == String("position")) {
        //I am transporting a Position: allocate transport_handler with template NON_GUI = Position
        
        GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame> >* transport_handler;
        
        
        //the id of the Route or Position that will be transported
        (parent->i_object_to_transport) = ((int)(parent->listcontrol_positions)->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
        
        transport_handler = new GraphicalFeatureTransportHandler<Position, ToDoAtEndOfTransport<Route, ListFrame> >(parent,
                                                                                                                    &((parent->data->position_list)[(parent->i_object_to_transport)]),
                                                                                                                    (parent->transported_object_type),
                                                                                                                    ((parent->data->route_list)[(parent->i_transporting_route)]),
                                                                                                                    to_do_at_end_of_transport
                                                                                                                    );
        
        //start the transport
        (*transport_handler)();
    }
    
    event.Skip(true);
    
}



template<class T, typename FF_OK> void PrintMessage<T, FF_OK>::operator()(void) {
    
    SetIdling<T>* set_idling;
    UnsetIdling<T>* unset_idling;
    
    set_idling = new SetIdling<T>(f);
    unset_idling = new UnsetIdling<T>(f);
    
    //I may be about to prompt a temporary dialog window, thus I set f->idling to true
    (*set_idling)();
    
    if (control != NULL) {
        
        if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {
            
            message_frame = new MessageFrame<FF_OK>(f, f_ok, title.value, message.value, image_path, wxDefaultPosition, wxDefaultSize, String(""));
            message_frame->Show(true);
            message_frame->Raise();
            
            // control->SetFocus();
            control->SetForegroundColour((wxGetApp().error_color));
            control->SetFont(wxGetApp().error_font);
            //                Reset(control);
            
        }

    }
    else {

        message_frame = new MessageFrame<FF_OK>(f, f_ok, title.value, message.value, image_path, wxDefaultPosition, wxDefaultSize, String(""));
        message_frame->Show(true);
        message_frame->Raise();

    }


    //AFTER the dialog window has been closed, I set f->idling to calse
    f->CallAfter(*unset_idling);


}



SightFrame::SightFrame(ListFrame* parent_input, Sight* sight_in, long position_in_listcontrol_sights_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    parent = parent_input;

    //pointer to init.txt to read fixed sight data from in there
    String new_prefix;
    unsigned int common_width;
    bool check = true;


    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    (*(parent->set_idling))();

    set_idling = new SetIdling<SightFrame>(this);
    unset_idling = new UnsetIdling<SightFrame>(this);
    (*unset_idling)();

    print_error_message = new PrintMessage<SightFrame, UnsetIdling<SightFrame> >(this, unset_idling);


    catalog = new Catalog((wxGetApp().path_file_catalog), String(""));

    //if this SightFrame has been constructed with sight_in = NULL, then I allocate a new Sight object with the pointer this->sight and set position_in_listcontrol_sights to a 'NULL' value (position_in_listcontrol_sights = -1). Otherwise, the pointer sight_in points to a valid Sight object -> I let this->sight point to sight_in, and set position_in_listcontrol_sights to position_in_listcontrol_sights_in.
    if (sight_in != NULL) {
        sight = sight_in;
        position_in_listcontrol_sights = position_in_listcontrol_sights_in;
    }
    else {
        sight = new Sight();
        position_in_listcontrol_sights = -1;
    }

    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    sizer_grid_measurement = new wxFlexGridSizer(6, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_time = new wxFlexGridSizer(5, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_label = new wxFlexGridSizer(1, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_3 = new wxBoxSizer(wxHORIZONTAL);
    box_sizer_4 = new wxBoxSizer(wxHORIZONTAL);
    sizer = new wxBoxSizer(wxVERTICAL);

    //allocate buttons
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);
    button_reduce = new wxButton(panel, wxID_ANY, "Reduce", wxDefaultPosition, wxDefaultSize);


    //First off, I need to set TAI_minus_UTC, which will be used in the following. If sight_in = NULL,  I read it from from file_init
    if (sight_in == NULL) {

        (sight->TAI_minus_UTC).read_from_file_to(String("TAI - UTC at time of master-clock synchronization with UTC"), (wxGetApp().path_file_init), String("R"), new_prefix);

    }


    StaticText* text_combo_body = new StaticText(panel, wxT("Celestial body"), wxDefaultPosition, wxDefaultSize, 0);
    body = new BodyField<SightFrame>(panel, &(sight->body), catalog, &(wxGetApp().list_frame->data->recent_bodies));

    StaticText* text_limb = new StaticText(panel, wxT("Limb"), wxDefaultPosition, wxDefaultSize, 0);
    limb = new LimbField<SightFrame>(panel, &(sight->limb));
    (limb->name)->Enable(false);

    //sextant altitude
    StaticText* text_H_s = new StaticText(panel, wxT("Sextant altitude"), wxDefaultPosition, wxDefaultSize, 0);
    H_s = new AngleField<SightFrame>(panel, &(sight->H_s), String(""));

    //index error
    StaticText* text_index_error = new StaticText(panel, wxT("Index error"), wxDefaultPosition, wxDefaultSize, 0);
    //If sight_in = NULL, read index error from init file
    if (sight_in == NULL) {
        sight->index_error.read_from_file_to(String("index error"), (wxGetApp().path_file_init), String("R"), new_prefix);
    }
    index_error = new AngleField<SightFrame>(panel, &(sight->index_error), String("+-"));
    index_error->set();

    //artificial horizon
    StaticText* text_artificial_horizon_check = new StaticText(panel, wxT("Artificial horizon"), wxDefaultPosition, wxDefaultSize, 0);
    artificial_horizon_check = new CheckField<SightFrame, DynamicLengthField<SightFrame> >(panel, &(sight->artificial_horizon), NULL, false);

    //height of eye
    StaticText* text_height_of_eye = new StaticText(panel, wxT("Height of eye"), wxDefaultPosition, wxDefaultSize, 0);
    height_of_eye = new DynamicLengthField<SightFrame>(panel, &(sight->height_of_eye), String("m"));
    if (sight_in == NULL) {
        //given that the height of eye may be often the same, I write a default value in sight->height_of_eye and fill in the height of eye DynamicLengthField with this value, so the user won't have to enter the same value all the time
        (sight->height_of_eye).read_from_file_to(String("default height of eye"), (wxGetApp().path_file_init), String("R"), String(""));
        height_of_eye->set();

    }
    //now that height_of_eye has been allocatd, I link artificial_horizon_check to height_of_eye
    (artificial_horizon_check->related_field) = height_of_eye;

    //master-clock date
    //sets  sight.master_clock_date_and_hour.date and sight.time.date to the current UTC date if this constructor has been called with sight_in = NULL
    if (sight_in == NULL) {
        (sight->master_clock_date_and_hour).date.set_current();
        (sight->master_clock_date_and_hour).chrono.set_current();
        (sight->time).date.set_current();
        (sight->time).chrono.set_current();
    }
    StaticText* text_date = new StaticText(panel, wxT("Master-clock UTC date and hour of sight"), wxDefaultPosition, wxDefaultSize, 0);
    master_clock_date = new DateField<SightFrame>(panel, &(sight->master_clock_date_and_hour.date));
    master_clock_date->set((sight->master_clock_date_and_hour).date);
    //    (master_clock_date->year)->SetFont((wxGetApp().error_font));
    //I bind master_clock_date->year/month/day to OnEditTime in such a way that, if the user enters a master_clock_date such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (master_clock_date->year)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (master_clock_date->month)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);


    //master-clock chrono
    StaticText* text_space_1 = new StaticText(panel, wxT("\t"), wxDefaultPosition, wxDefaultSize, 0);
    master_clock_chrono = new ChronoField<SightFrame>(panel, &(sight->master_clock_date_and_hour.chrono));
    //I bind master_clock_chrono->hour/minute/second to OnEditTime in such a way that, if the user enters a master_clock_chrono such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (master_clock_chrono->hour)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (master_clock_chrono->minute)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (master_clock_chrono->second)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);

    //I initialize the GUI filed master_clock_chrono with the one written in sight_in.
    //    if(sight_in != NULL){
    master_clock_chrono->set(sight->master_clock_date_and_hour.chrono);
    //    }


    //check/uncheck stopwatch
    StaticText* text_stopwatch_check = new StaticText(panel, wxT("Stopwatch"), wxDefaultPosition, wxDefaultSize, 0);
    stopwatch_check = new CheckField<SightFrame, ChronoField<SightFrame> >(panel, &(sight->use_stopwatch), NULL, true);
    //I bind stopwatch_check to OnEditTime in such a way that, if the user enters a stopwatch_check such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (stopwatch_check->checkbox)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //if stopwatch_check is checked/unchecked, then I runm OnEditTime to verify that the time of sight lies within the ephemerides' time span
    //    (stopwatch_check->checkbox)->Bind(wxEVT_CHECKBOX, &SightFrame::OnEditTime<wxCommandEvent>, this);


    //stopwatch reading
    StaticText* text_stopwatch_reading = new StaticText(panel, wxT("Stopwatch reading"), wxDefaultPosition, wxDefaultSize, 0);
    //    stopwatch_reading = new ChronoField(this, &(sight.stopwatch));
    stopwatch_reading = new ChronoField<SightFrame>(panel, &(sight->stopwatch));
    //now that stopwatch_reading has been allocatd, I link stopwatch_check to stopwatch_reading
    (stopwatch_check->related_field) = stopwatch_reading;
    //I bind stopwatch_reading->hour/minute/second to OnEditTime in such a way that, if the user enters a stopwatch_reading such that sight->time lies outside the ephemerides' time interval, an error message is prompted
    //    (stopwatch_reading->hour)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (stopwatch_reading->minute)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);
    //    (stopwatch_reading->second)->Bind(wxEVT_KILL_FOCUS, &SightFrame::OnEditTime<wxFocusEvent>, this);


    //initialize stopwatch_check and stopwatch_reading
    (stopwatch_check->checkbox)->SetValue(false);
    stopwatch_reading->Enable(false);

    StaticText* text_TAI_minus_UTC = new StaticText(panel, wxT("TAI - UTC"), wxDefaultPosition, wxDefaultSize, 0);
    TAI_minus_UTC = new ChronoField<SightFrame>(panel, &(sight->TAI_minus_UTC));
    TAI_minus_UTC->set(sight->TAI_minus_UTC);

    //message and image shown if the time entered by the user is not covered by ephemerides' data. Both are set to empty at the construction of SightFrame
//    text_time_interval_not_ok = new StaticText(panel, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
//    image_time_interval_status = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize);

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<SightFrame>(panel, &(sight->label));


    button_cancel->Bind(wxEVT_BUTTON, &SightFrame::OnPressCancel, this);

    //I bind reduce button to label->set_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_reduce->Bind(wxEVT_BUTTON, &SightFrame::OnPressReduce, this);
    button_reduce->Bind(wxEVT_BUTTON, label->set_to_current_time);
    button_reduce->Bind(wxEVT_BUTTON, &SightFrame::update_recent_items<wxCommandEvent>, this);

    //If I press reduce, I want all the fields in this SightFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_reduce->Bind(wxEVT_BUTTON, &BodyField<SightFrame>::get<wxCommandEvent>, body);
    button_reduce->Bind(wxEVT_BUTTON, &LimbField<SightFrame>::get<wxCommandEvent>, limb);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, H_s);
    button_reduce->Bind(wxEVT_BUTTON, &AngleField<SightFrame>::get<wxCommandEvent>, index_error);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<SightFrame, DynamicLengthField<SightFrame> >::get<wxCommandEvent>, artificial_horizon_check);
    button_reduce->Bind(wxEVT_BUTTON, &DynamicLengthField<SightFrame>::get<wxCommandEvent>, height_of_eye);
    button_reduce->Bind(wxEVT_BUTTON, &DateField<SightFrame>::get<wxCommandEvent>, master_clock_date);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField<SightFrame>::get<wxCommandEvent>, master_clock_chrono);
    button_reduce->Bind(wxEVT_BUTTON, &CheckField<SightFrame, ChronoField<SightFrame> >::get<wxCommandEvent>, stopwatch_check);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField<SightFrame>::get<wxCommandEvent>, stopwatch_reading);
    button_reduce->Bind(wxEVT_BUTTON, &ChronoField<SightFrame>::get<wxCommandEvent>, TAI_minus_UTC);
    button_reduce->Bind(wxEVT_BUTTON, &StringField<SightFrame>::get<wxCommandEvent>, label);

    //bind the function SightFrame::KeyDown to the event where a keyboard dey is pressed down in panel, body, ... and all fields
    panel->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    body->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    limb->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    H_s->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    index_error->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    artificial_horizon_check->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    stopwatch_check->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    height_of_eye->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    master_clock_date->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    master_clock_chrono->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    stopwatch_reading->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    TAI_minus_UTC->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &SightFrame::KeyDown, this);


    //I enable the reduce button only if sight_in is a valid sight with the entries propely filled, i.e., only if sight_in != NULL
    button_reduce->Enable((sight_in != NULL));

    sizer_grid_measurement->Add(text_combo_body);
    body->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_limb);
    limb->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_H_s);
    H_s->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_index_error);
    index_error->InsertIn<wxBoxSizer>(box_sizer_3);
    sizer_grid_measurement->Add(box_sizer_3);

    sizer_grid_measurement->Add(text_artificial_horizon_check);
    artificial_horizon_check->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_height_of_eye);
    height_of_eye->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_time->Add(text_date);
    master_clock_date->InsertIn<wxBoxSizer>(box_sizer_4);
    box_sizer_4->Add(text_space_1);
    master_clock_chrono->InsertIn<wxBoxSizer>(box_sizer_4);
    sizer_grid_time->Add(box_sizer_4);

    sizer_grid_time->Add(text_stopwatch_check);
    stopwatch_check->InsertIn<wxFlexGridSizer>(sizer_grid_time);

    sizer_grid_time->Add(text_stopwatch_reading);
    stopwatch_reading->InsertIn<wxFlexGridSizer>(sizer_grid_time);

    sizer_grid_time->Add(text_TAI_minus_UTC);
    TAI_minus_UTC->InsertIn<wxFlexGridSizer>(sizer_grid_time);

//    sizer_grid_time->Add(text_time_interval_not_ok);
//    sizer_grid_time->Add(image_time_interval_status);

    sizer_grid_label->Add(text_label);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL);
    box_sizer_2->Add(button_reduce, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL);

    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Measurement");
    sizer_box_time = new wxStaticBoxSizer(wxVERTICAL, panel, "Time");

    sizer_box_measurement->Add(sizer_grid_measurement);
    sizer_box_time->Add(sizer_grid_time);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the StaticText containing "Master-clock UTC date and hour of sight"
    common_width = GetTextExtent(wxS("Master-clock UTC date and hour of sight   ")).GetWidth();
    text_combo_body->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_date->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_box_time, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    panel->Fit();
    Fit();


    //Maximize();


    if (!check) {
        cout << prefix.value << RED << "Cannot read sight!\n" << RESET;
    }
    if (sight_in != NULL) { set(); }

    //runs checl to write into the `ok` boolean variable of each field, and then AllOk to enable/disable button_reduce according to these `ok` variables
    Check();
    AllOk();

    Centre();

}


//if a key is pressed in the keyboard, I call this function
void SightFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        // the use pressed escape -> I do as if the user pressed button_cancel

        wxCommandEvent dummy;

        OnPressCancel(dummy);

    }
    else {

        if (((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
            //the user pressed return or numpad return

            /*
             //example of a lambda!
            CallAfter(
                      [  ] (  ) -> void
                      {
                          cout << "Lambda ! \n";
                      }
                      );
             */

            if (is_ok()) {
                //if all fields are ok, I do as if the user presssed button_reduce

                wxCommandEvent dummy;

                OnPressReduce(dummy);

            }

        }

    }

    event.Skip(true);

}

//run check on all the GUI fields that are members of SightFrame
template<class E> void SightFrame::Check(E& event) {

    (*(body->check))(event);
    if (((body->name->GetValue()) == wxString("sun")) || ((body->name->GetValue()) == wxString("moon"))) {
        //I check limb only if the body has a limb -> only if the body is either sun or moon
        (*(limb->check))(event);
    }
    (*(artificial_horizon_check->check))(event);
    (*(stopwatch_check->check))(event);
    (*(H_s->check))(event);
    (*(index_error->check))(event);
    if (!((artificial_horizon_check->checkbox)->GetValue())) {
        //I check height_of_eye only if the artificial horizon checkbox has been checked
        (*(height_of_eye->check))(event);
    }
    (*(master_clock_date->check))(event);
    (*(master_clock_chrono->check))(event);
    if ((stopwatch_check->checkbox)->GetValue()) {
        //I check stopwatch_reading only if the stopwatch checkbox has been checked
        (*(stopwatch_reading->check))(event);
    }
    (*(TAI_minus_UTC->check))(event);
    (*(label->check))(event);

    event.Skip(true);

}


//same as void SightFrame::Check(E& event)  but without event as an argument
void SightFrame::Check(void){
    
    wxCommandEvent dummy;
    
    Check(dummy);
    
    
}

    
template<class E> void SightFrame::update_recent_items(E& event) {

    unsigned int i;
    bool check;

    //I check whether the name in the GUI field body matches one of the body names in catalog, and store its id in i
    for (check = false, i = 0; (i < (body->catalog->list).size()) && (!check); i++) {
        if (String((body->name->GetValue().ToStdString())) == (((body->catalog->list)[i]).name)) {
            check = true;
        }
    }
    i--;

    //insert body #i into data->recent_bodies
//    wxGetApp().list_frame->data->insert_recent_body(i);
    wxGetApp().list_frame->data->insert_recent_item(i, &(wxGetApp().list_frame->data->recent_bodies));

    
    
    event.Skip(true);

}

PositionFrame::PositionFrame(ListFrame* parent_input, Position* position_in, long position_in_listcontrol_positions_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    parent = parent_input;

    String new_prefix, /*the label which appears on button_ok*/label_button_ok;
    unsigned int common_width;


    bool check = true;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    //SetColor(this);

    set_idling = new SetIdling<PositionFrame>(this);
    unset_idling = new UnsetIdling<PositionFrame>(this);
    (*unset_idling)();

    print_error_message = new PrintMessage<PositionFrame, UnsetIdling<PositionFrame> >(this, unset_idling);

    //if this PositionFrame has been constructed with position_in = NULL, then I allocate a new Position object with the pointer this->position and set position_in_listcontrol_positions to a 'NULL' value (position_in_listcontrol_positions = -1). Otherwise, the pointer position_in points to a valid Position object -> I let this->position point to position_in, and set position_in_listcontrol_positions to position_in_listcontrol_positions_in.
    if (position_in != NULL) {
        position = position_in;
        position_in_listcontrol_positions = position_in_listcontrol_positions_in;
    }
    else {
        position = new Position();
        position_in_listcontrol_positions = -1;
    }

    //if I am adding a brand new position, I name button_ok 'Add'. Otherwise I name it "Modify"
    if (position_in == NULL) {
        label_button_ok.set(String(""), String("Add"), String(""));
    }
    else {
        label_button_ok.set(String(""), String("Modify"), String(""));
    }


    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value, wxDefaultPosition, wxDefaultSize);
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);

    sizer_grid_measurement = new wxFlexGridSizer(2, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer_grid_label = new wxFlexGridSizer(1, 2,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value)
    );
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer_2 = new wxBoxSizer(wxHORIZONTAL);


    //latitude
    StaticText* text_lat = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    lat = new AngleField<PositionFrame>(panel, &(position->phi), String("NS"));

    //longitude
    StaticText* text_lon = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    lon = new AngleField<PositionFrame>(panel, &(position->lambda), String("EW"));

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<PositionFrame>(panel, &(position->label));


    //I bind  button_ok to label->set_to_current_time: in this way, whenever the reduce button is pressed, the GUI field label is filled with the current time (if empty)
    button_ok->Bind(wxEVT_BUTTON, label->set_to_current_time);
    //If I press button_ok, I want all the fields in this PositionFrame to be checked, and their values to be written in the respective non-GUI objects: to do this, I bind the presssing of reduce button to these functions
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lat);
    button_ok->Bind(wxEVT_BUTTON, &AngleField<PositionFrame>::get<wxCommandEvent>, lon);
    button_ok->Bind(wxEVT_BUTTON, &StringField<PositionFrame>::get<wxCommandEvent>, label);
    button_ok->Bind(wxEVT_BUTTON, &::PositionFrame::OnPressOk, this);
    button_cancel->Bind(wxEVT_BUTTON, &PositionFrame::OnPressCancel, this);


    panel->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    lat->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    lon->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &PositionFrame::KeyDown, this);


    //I enable the reduce button only if position_in is a valid position with the entries propely filled, i.e., only if position_in != NULL
    button_ok->Enable((position_in != NULL));

    sizer_grid_measurement->Add(text_lat, 0, wxALIGN_CENTER_VERTICAL);
    lat->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_measurement->Add(text_lon, 0, wxALIGN_CENTER_VERTICAL);
    lon->InsertIn<wxFlexGridSizer>(sizer_grid_measurement);

    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    box_sizer_2->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL
    );
    box_sizer_2->Add(button_ok, 0, wxALIGN_BOTTOM | wxALL,
        (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value),
        NULL
    );

    sizer_box_measurement = new wxStaticBoxSizer(wxVERTICAL, panel, "Coordinates");

    sizer_box_measurement->Add(sizer_grid_measurement);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the StaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_lat->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_lon->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    //add the various elements to sizer, by inserting a border of (wxGetApp().rectangle_display.GetSize().GetWidth())*(length_border_over_length_screen.value) in all directions
    sizer->Add(sizer_box_measurement, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer_2, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));


    //panel->SetSizer(sizer);

    //    CreateStatusBar();
    //    SetStatusText( "Welcome to UnsetIdling's text editor!" );

    if (!check) {
        cout << prefix.value << RED << "Cannot read position!\n" << RESET;
    }

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    //Maximize();
    panel->Fit();
    Fit();


    if (position_in != NULL) { set(); }

    Centre();

}


//create a new RouteFrame. If for_transport = true/false, it enables the fields related to the start position of the Route and disables the circle of equal altitude type 
RouteFrame::RouteFrame(ListFrame* parent_input, Route* route_in, bool for_transport_in, long position_in_listcontrol_routes_in, const wxString& title, const wxPoint& pos, const wxSize& size, String prefix) : wxFrame(parent_input, wxID_ANY, title, pos, size) {

    String new_prefix, label_button_ok;
    unsigned int common_width;
    bool check;


    parent = parent_input;
    check = true;
    for_transport = for_transport_in;
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    //SetColor(this);

    set_idling = new SetIdling<RouteFrame>(this);
    unset_idling = new UnsetIdling<RouteFrame>(this);
    (*unset_idling)();
    print_error_message = new PrintMessage<RouteFrame, UnsetIdling<RouteFrame> >(this, unset_idling);

    //if this RouteFrame has been constructed with route_in = NULL, then I allocate a new Route object with the pointer this->route and set list_route to a 'NULL' value (list_route = -1). Otherwise, the pointer route_in points to a valid Route object -> I let this->route point to route_in, and set list_route to list_route_in.
    //if I am adding a brand new route, I name button_ok 'Add' if I am not adding it for transport, while I name button_ok 'Transport' if I am adding the Route for transport. Otherwise I name it "Modify"
    if (route_in != NULL) {
        route = route_in;
        position_in_listcontrol_routes = position_in_listcontrol_routes_in;
        label_button_ok.set(String(""), String("Modify"), String(""));
    }
    else {
        route = new Route();
        position_in_listcontrol_routes = -1;
//        label_button_ok.set(String(""), String("Add"), String(""));
        label_button_ok.set(String(""), String((for_transport ? "Transport" : "Add")), String(""));

    }


    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));

    //allocate buttons
    button_ok = new wxButton(panel, wxID_ANY, label_button_ok.value, wxDefaultPosition, wxDefaultSize);
    button_cancel = new wxButton(panel, wxID_ANY, "Cancel", wxDefaultPosition, wxDefaultSize);


    sizer_grid_type = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_Z = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_l_format_l_t_v = new wxBoxSizer(wxVERTICAL);
    sizer_grid_l = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_t_v = new wxFlexGridSizer(2, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_start = new wxFlexGridSizer(2, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_GP = new wxFlexGridSizer(2, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_omega = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid_label = new wxFlexGridSizer(1, 2, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_box_data = new wxStaticBoxSizer(wxVERTICAL, panel, "Data");
    sizer_box_l_format_l_t_v = new wxStaticBoxSizer(wxVERTICAL, panel, "Length");
    sizer_box_l = new wxStaticBoxSizer(wxVERTICAL, panel, "Length");
    sizer_box_t_v = new wxStaticBoxSizer(wxVERTICAL, panel, "Time and speed");
    sizer_box_start = new wxStaticBoxSizer(wxVERTICAL, panel, "Start position");
    sizer_box_GP = new wxStaticBoxSizer(wxVERTICAL, panel, "Ground position");
    sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer = new wxBoxSizer(wxHORIZONTAL);


    //type:a wxComboBox which indicates the type of the route (loxodrome, orthordrome or circle of equal altitude)
    StaticText* text_type = new StaticText(panel, wxT("Type"), wxDefaultPosition, wxDefaultSize, 0);
    //if the Route of *this is for transport, then only show 'loxodrome' and 'orthodrome' in type
    if (for_transport) {
        type = new RouteTypeField<RouteFrame>(panel, &(route->type), Route_for_transport_types, &(wxGetApp().list_frame->data->recent_route_for_transport_types));
    }else{
        type = new RouteTypeField<RouteFrame>(panel, &(route->type), Route_types,  &(wxGetApp().list_frame->data->recent_route_types));
    }

    //Z
    StaticText* text_Z = new StaticText(panel, wxT("Z"), wxDefaultPosition, wxDefaultSize, 0);
    Z = new AngleField<RouteFrame>(panel, &(route->Z), String(""));

    //format in which lengths are expressed
    StaticText* text_l_format = new StaticText(panel, wxT("Length format"), wxDefaultPosition, wxDefaultSize, 0);
    length_format = new LengthFormatField<RouteFrame>(panel, &(route->length_format), &(wxGetApp().list_frame->data->recent_length_formats));

    //the field for time to set the Route length
    text_time = new StaticText(panel, wxT("Time"), wxDefaultPosition, wxDefaultSize, 0);
    time = new ChronoField<RouteFrame>(panel, &(route->time));
    //the field for speed to set the Route length
    text_speed = new StaticText(panel, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0);
    speed = new SpeedField<RouteFrame>(panel, &(route->speed), String("kt"));

    //the field for Length to set the Route length
    text_length = new StaticText(panel, wxT("Length"), wxDefaultPosition, wxDefaultSize, 0);
    length = new DynamicLengthField<RouteFrame>(panel, &(route->length), String("nm"));


    type->Bind(wxEVT_COMBOBOX, &LengthFormatField<RouteFrame>::OnEdit<wxCommandEvent>, length_format);
    type->Bind(wxEVT_KEY_UP, &LengthFormatField<RouteFrame>::OnEdit<wxKeyEvent>, length_format);

    //bind time and speed changes to UpdateLength, so every time time and speed GUI fields are edited, length is updated accordingly
    //I want ChronoField::OnEditHour, OnEditMinute and OnEditSecond to be called before RouteFrame::UpdateLength, because RouteFrame::UpdateLength checks the variables hour_ok, minute_ok and second_ok, which must have been set previously by ChronoField::OnEditHour, OnEditMinute and OnEditSecond, respectively -> To achieve this 1) I unbind ChronoField::OnEditHour, OnEditMinute and OnEditSecond from time->hour, minute and second, respectively, (they had been bound previously in the ChronoField constructor) 2) I bind RouteFrame::UpdateLength to time->hour, minute, second 3) I bind back ChronoField::OnEditHour, OnEditMinute, OnEditSecond to time->hour, minut and second, respectively. In this way, the Binding order has changed -> the order in which the event handlers will be called will be the right one.
    //1)
    time->hour->Unbind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditHour<wxCommandEvent>, time);
    time->hour->Unbind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditHour<wxKeyEvent>, time);
    time->minute->Unbind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditMinute<wxCommandEvent>, time);
    time->minute->Unbind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditMinute<wxKeyEvent>, time);
    time->second->Unbind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditSecond<wxKeyEvent>, time);
    //2)
    time->Bind(wxEVT_KEY_UP, &RouteFrame::UpdateLength<wxKeyEvent>, this);
    time->Bind(wxEVT_COMBOBOX, &RouteFrame::UpdateLength<wxCommandEvent>, this);
    //3)
    time->hour->Bind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditHour<wxCommandEvent>, time);
    time->hour->Bind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditHour<wxKeyEvent>, time);
    time->minute->Bind(wxEVT_COMBOBOX, &ChronoField<RouteFrame>::OnEditMinute<wxCommandEvent>, time);
    time->minute->Bind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditMinute<wxKeyEvent>, time);
    time->second->Bind(wxEVT_KEY_UP, &ChronoField<RouteFrame>::OnEditSecond<wxKeyEvent>, time);

    //I want SpeedField::OnEditValue and OnEditUnit to be called before RouteFrame::UpdateLength, because RouteFrame::UpdateLength checks the variables value_ok and unit_ok, which must have been set previously by SpeedField::OnEditValue and OnEditUnit, respectively -> To achieve this 1) I unbind SpeedField::OnEditValue and OnEditUnit from speed->value and unit, respectively, (they had been bound previously in the SpeedField constructor) 2) I bind RouteFrame::UpdateLength to speed->value and unit 3) I bind back SpeedField::OnEditValue and OnEditUnit to speed->value and unit, respectively. In this way, the Binding order has changed -> the order in which the event handlers will be called will be the right one. 
    speed->value->Unbind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditValue<wxKeyEvent>, speed);
    speed->unit->name->Unbind(wxEVT_COMBOBOX, &SpeedField<RouteFrame>::OnEditUnit<wxCommandEvent>, speed);
    speed->unit->name->Unbind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditUnit<wxKeyEvent>, speed);
    //2)
    speed->Bind(wxEVT_KEY_UP, &RouteFrame::UpdateLength<wxKeyEvent>, this);
    //3)
    speed->value->Bind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditValue<wxKeyEvent>, speed);
    speed->unit->Bind(wxEVT_COMBOBOX, &SpeedField<RouteFrame>::OnEditUnit<wxCommandEvent>, speed);
    speed->unit->Bind(wxEVT_KEY_UP, &SpeedField<RouteFrame>::OnEditUnit<wxKeyEvent>, speed);


    //start position
    //start_phi
    StaticText* text_start_phi = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    start_phi = new AngleField<RouteFrame>(panel, &((route->reference_position).phi), String("NS"));
    //start_lambda
    StaticText* text_start_lambda = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    start_lambda = new AngleField<RouteFrame>(panel, &((route->reference_position).lambda), String("EW"));

    //GP (ground position)
    //GP_phi
    StaticText* text_GP_phi = new StaticText(panel, wxT("Latitude"), wxDefaultPosition, wxDefaultSize, 0);
    GP_phi = new AngleField<RouteFrame>(panel, &((route->reference_position).phi), String("NS"));
    //GP_lambda
    StaticText* text_GP_lambda = new StaticText(panel, wxT("Longitude"), wxDefaultPosition, wxDefaultSize, 0);
    GP_lambda = new AngleField<RouteFrame>(panel, &((route->reference_position).lambda), String("EW"));

    //omega
    StaticText* text_omega = new StaticText(panel, wxT("Omega"), wxDefaultPosition, wxDefaultSize, 0);
    omega = new AngleField<RouteFrame>(panel, &(route->omega), String(""));

    //label
    StaticText* text_label = new StaticText(panel, wxT("Label"), wxDefaultPosition, wxDefaultSize, 0);
    label = new StringField<RouteFrame>(panel, &(route->label));

    //I enable the ok button only if route_in is a valid route with the entries propely filled, i.e., only if route_in != NULL
    button_ok->Bind(wxEVT_BUTTON, &RouteFrame::OnPressOk, this);
    button_ok->Enable((route_in != NULL));
    button_cancel->Bind(wxEVT_BUTTON, &RouteFrame::OnPressCancel, this);


    //bind the function SightFrame::KeyDown to the event where a keyboard dey is pressed down in panel, ... and all fields
    panel->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    type->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    Z->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    omega->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    start_phi->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    start_lambda->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    GP_phi->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    GP_lambda->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    length_format->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    length->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    time->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    speed->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);
    label->Bind(wxEVT_KEY_DOWN, &RouteFrame::KeyDown, this);


    sizer_grid_type->Add(text_type, 0, wxALIGN_CENTER_VERTICAL);
    type->InsertIn<wxFlexGridSizer>(sizer_grid_type);

    sizer_grid_Z->Add(text_Z, 0, wxALIGN_CENTER_VERTICAL);
    Z->InsertIn<wxFlexGridSizer>(sizer_grid_Z);

    sizer_grid_t_v->Add(text_time, 0, wxALIGN_CENTER_VERTICAL);
    time->InsertIn<wxFlexGridSizer>(sizer_grid_t_v);
    sizer_grid_t_v->Add(text_speed, 0, wxALIGN_CENTER_VERTICAL);
    speed->InsertIn<wxFlexGridSizer>(sizer_grid_t_v);
    sizer_box_t_v->Add(sizer_grid_t_v);

    sizer_grid_l->Add(text_length, 0, wxALIGN_CENTER_VERTICAL);
    length->InsertIn<wxFlexGridSizer>(sizer_grid_l);
    sizer_box_l->Add(sizer_grid_l);

    sizer_l_format_l_t_v->Add(text_l_format, 0, wxALIGN_LEFT);
    length_format->InsertIn<wxBoxSizer>(sizer_l_format_l_t_v);
    sizer_l_format_l_t_v->Add(sizer_box_t_v);
    sizer_l_format_l_t_v->Add(sizer_box_l);

    sizer_box_l_format_l_t_v->Add(sizer_l_format_l_t_v);

    sizer_grid_omega->Add(text_omega, 0, wxALIGN_CENTER_VERTICAL);
    omega->InsertIn<wxFlexGridSizer>(sizer_grid_omega);

    sizer_grid_start->Add(text_start_phi);
    start_phi->InsertIn<wxFlexGridSizer>(sizer_grid_start);
    sizer_grid_start->Add(text_start_lambda);
    start_lambda->InsertIn<wxFlexGridSizer>(sizer_grid_start);

    sizer_box_start->Add(sizer_grid_start);

    sizer_grid_GP->Add(text_GP_phi);
    GP_phi->InsertIn<wxFlexGridSizer>(sizer_grid_GP);
    sizer_grid_GP->Add(text_GP_lambda);
    GP_lambda->InsertIn<wxFlexGridSizer>(sizer_grid_GP);

    sizer_box_GP->Add(sizer_grid_GP);

    sizer_grid_label->Add(text_label, 0, wxALIGN_CENTER_VERTICAL);
    label->InsertIn<wxFlexGridSizer>(sizer_grid_label);

    sizer_box_data->Add(sizer_grid_type);
    sizer_box_data->Add(sizer_grid_Z);
    sizer_box_data->Add(sizer_box_l_format_l_t_v);
    sizer_box_data->Add(sizer_box_start);
    sizer_box_data->Add(sizer_box_GP);
    sizer_box_data->Add(sizer_grid_omega);

    box_sizer->Add(button_cancel, 0, wxALIGN_BOTTOM | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), NULL);
    box_sizer->Add(button_ok, 0, wxALIGN_BOTTOM | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value), NULL);

    //set the sizes of elements in each of the wxStaticBoxSizers to the same value -> the columns across different both sizers will be aligned vertically
    //sets common_width to the width of the largest entry in the left column, in this case the wxStaticText containing "Longitude"
    common_width = GetTextExtent(wxS("Longitude   ")).GetWidth();
    text_Z->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_omega->SetMinSize(ToDIP(wxSize(common_width, -1)));
    text_label->SetMinSize(ToDIP(wxSize(common_width, -1)));

    //add the various elements to sizer, by inserting a border of (wxGetApp().rectangle_display.GetSize().GetWidth())*(length_border_over_length_screen.value) in all directions
    sizer->Add(sizer_box_data, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(sizer_grid_label, 0, wxEXPAND | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer->Add(box_sizer, 1, wxALIGN_RIGHT | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer);
    panel->Fit();
    Fit();


    if (!check) {

        cout << prefix.value << RED << "Cannot read route!\n" << RESET;

    }

    if (route_in == NULL) {
        //If the user is about to enter a brand new route, then the GUI fields are disabled/enabled according to the currently selected value in *type

        //set as route type the most recent item in recetn _route_types (or recent route_for_transport_types if the new Route has been created for a transport): set first type->object and then write in type the value written in type->object

        if(for_transport){
            type->object->set(Route_types[wxGetApp().list_frame->data->recent_route_for_transport_types.front()]);
        }else{
            type->object->set(Route_types[wxGetApp().list_frame->data->recent_route_types.front()]);
        }
        type->MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame> >::set();
        
        length_format->object->set(LengthFormat_types[wxGetApp().list_frame->data->recent_length_formats.front()]);
        length_format->MultipleItemField<RouteFrame, LengthFormat, CheckLengthFormat<RouteFrame> >::set();

        //call this to enable/disable the GUI fields in according to the choice of RouteType
        type->OnEdit();
        //call this to enable/disable the GUI fields according ot the choice of LengthFormat
        OnChooseLengthFormatField();

    }
    else {
        //I am reading an existing Route -> I call set -> the LengthFormatField will be set -> I call OnChooseLengthFormat to enable/disable the related GUI fields accordingly

        set();

        start_phi->Enable(for_transport);
        start_lambda->Enable(for_transport);

    }

    //runs checl to write into the `ok` boolean variable of each field, and then AllOk to enable/disable button_reduce according to these `ok` variables
    Check();
    AllOk();
    
    Centre();

}



//set all the GUI fields in this equal to those in the non-GUI object this->position
void PositionFrame::set(void) {

    lat->set();
    lon->set();
    label->set();

}

//set all the non-GUI objects in this->position  equal to those in the GUI fields
template<class T> void PositionFrame::get(T& event) {

    lat->get(event);
    lon->get(event);
    label->get(event);

}


void PositionFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to false
    (*(parent->unset_idling))();

    Close(TRUE);

}


//this function is triggered when button_ok is pressed
void PositionFrame::OnPressOk(wxCommandEvent& event) {

    unsigned int i;
    stringstream s;

    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }

    //if I am adding a new Position, I resize points_position_list to add a new element to it
    if (position_in_listcontrol_positions == -1) {

        for (i = 0; i < (parent->chart_frames.size()); i++) {

            ((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).resize(((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).size() + 1);
            ((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).resize(((((parent->chart_frames)[i])->draw_panel)->points_position_list_now).size() + 1);

        }
    }


    //writes the values of the GUI fields in the non-GUI fields
    get(event);

    position->print(String("position entered via GUI"), String(""), cout);

    //if the constructor of PositionFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of position_list
    if (position_in_listcontrol_positions == -1) {
        (((this->parent)->data)->position_list).push_back(*position);
    }

    (parent->listcontrol_positions)->set((parent->data)->position_list, false);

    //given that I have reset the content of listcontrol_positions, now no items are selected in this ListControl -> I call:
    (*(parent->on_change_selection_in_listcontrol_positions))(event);

    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    
//    parent->PreRenderAll();
    parent->AnimateToObject<Position, UnsetIdling<ListFrame> >(position, parent->unset_idling);
    

    event.Skip(true);

    Close(TRUE);

}


void RouteFrame::OnPressOk(wxCommandEvent& event) {

    unsigned int i;
    stringstream s;
    
    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }


    //write the values of the GUI fields in the non-GUI fields
    get(event);

    if (position_in_listcontrol_routes == -1) {
        //I am creating a new Route

        //if the constructor of RouteFrame has been called with route_in = NULL, then I push back the newly allocated route to the end of route_list and reduce it
        parent->data->add_route(route, String(""));

    }else {
        //I am modifying an existing Route

        if ((route->related_sight) != -1) {
            //the Route that I am moidifying is related to a Sight

            //because I am modifying and thus altering the Route, I disconnect it from its related sight
            (parent->i_object_to_disconnect) = (route->related_sight.value);
//            parent->DisconnectAndPromptMessage(event);
            //set i_obeject_to_disconnect to its original value
            (parent->i_object_to_disconnect) = -1;

        }

    }


    //if I am adding a new Route, I resize points_route_list to add a new element to it
    if (position_in_listcontrol_routes == -1) {

        for (i = 0; i < (parent->chart_frames.size()); i++) {

            (((parent->chart_frames)[i])->draw_panel)->points_route_list_now.resize(((((parent->chart_frames)[i])->draw_panel)->points_route_list_now).size() + 1);
            (((parent->chart_frames)[i])->draw_panel)->reference_positions_route_list_now.resize(((parent->chart_frames)[i])->draw_panel->reference_positions_route_list_now.size() + 1);

        }
    }

    //call listcontrol_sights->set with true because I want to keep the selection in listcontrol_sights
    parent->listcontrol_sights->set((parent->data)->sight_list, true);
    parent->listcontrol_positions->set((parent->data)->position_list, true);
    parent->listcontrol_routes->set((parent->data)->route_list, false);

    //given that I have reset the content of listcontrol_sights and listcontrol_routes, now no items will be selected in these ListControls -> I call:
    (*(parent->on_change_selection_in_listcontrol_sights))(event);
    (*(parent->on_change_selection_in_listcontrol_routes))(event);

    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    //insert the animation here
    

//    parent->PreRenderAll();

    if ((parent->transporting_with_new_route)) {
        //if I am adding a new Route for transport, call on_new_route_in_listcontrol_routes_for_transport to execute the transport with this Route
        (*(parent->on_new_route_in_listcontrol_routes_for_transport))(event);

        //set the reference position of the transporting Route to the initial position of the object that has been transported: in thiw way, the transporting Route will look nice on the chart
        if ((parent->transported_object_type) == String("position")) {

            //store the starting position in geo_position_start
            ((parent->data->route_list)[(parent->i_transporting_route)]).reference_position = (parent->data->position_list)[(parent->i_object_to_transport)];

        }
        else {

            if (((parent->transported_object_type) == String("sight")) || (parent->transported_object_type) == String("route")) {

                //store the starting reference position in geo_position_start
                ((parent->data->route_list)[(parent->i_transporting_route)]).reference_position = (((parent->data->route_list)[(parent->i_object_to_transport)]).reference_position);

            }

        }

        //I refresh everything because of the modification above
        //call listcontrol_routes->set with true because I want to keep the selection in listcontrol_routes
        parent->listcontrol_routes->set((parent->data->route_list), false);
        parent->Resize();
        parent->OnModifyFile();
//        parent->PreRenderAll();

    }

    
    if((position_in_listcontrol_routes != -1) && ((route->related_sight) != -1)){
        //I am modifying an existing Route and the Route that I am modifying is related to a Sight -> prepare the warning message to be prompted at the end of the animation and call AnimateToObject with parent->print_info_message as an argument, in such a way that, at the end of the animation, this message is prompted

        parent->print_info_message->control = NULL;
        parent->print_info_message->title.set(String(""), String("Warning"), String(""));
        parent->print_info_message->message.set(String(""), String("The route which has been modified was related to a sight! Disconnecting the route from the sight."), String(""));
        
        parent->AnimateToObject<Route, PrintMessage<ListFrame, UnsetIdling<ListFrame> > >(route, parent->print_info_message);

        
    }else{
        //I am either entering a new Route or modifying a Route unrelated to a Sight -> in both cases, I don't need to prompt a message warning the user that the Route under consideration is being disconnected from its related Sight -> trigger the animation that centers the chart on *route by callling UnsetIdling (intended as 'do nothing special' here) at the end of the animation
        
        //If I am adding a new Route for transport, I do not call any animation, because there is already the transport animation that will be prompted. Otherwise, I call an animation that zooms on the newly added Route
        if (!(parent->transporting_with_new_route)) {
            
            parent->AnimateToObject<Route, UnsetIdling<ListFrame>  >(route, parent->unset_idling);
            
        }
        
    }
    
 
    event.Skip(true);

    Close(TRUE);

}

void RouteFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to false
    (*(parent->unset_idling))();

    Close(TRUE);

}


//write the content in the GUI fields into the non-GUI fields, and returns true if all is ok, false otherwise
bool RouteFrame::is_ok(void) {
    
    wxCommandEvent dummy;
    
    get(dummy);
    
    return((type->is_ok()) &&
           
           (
            (
             ((((type->name)->GetValue()) == wxString(((Route_types[0]).value))) || (((type->name)->GetValue()) == wxString(((Route_types[1]).value)))) &&
             (
              (Z->is_ok()) &&
              ((start_phi->is_ok()) || for_transport) &&
              ((start_lambda->is_ok()) || for_transport) &&
              (((((length_format->name)->GetValue()) == wxString(((LengthFormat_types[0]).value))) && ((time->is_ok()) && (speed->is_ok()))) || ((((length_format->name)->GetValue()) == wxString(((LengthFormat_types[1]).value))) && (length->is_ok())))
              )
             )
            
            ||
            
            (
             (((type->name)->GetValue()) == wxString(((Route_types[2]).value))) &&
             ((omega->is_ok()) &&
              (GP_phi->is_ok()) &&
              (GP_lambda->is_ok()))
             )
            )
           
           );
    
}

//tries to enable button_ok
void RouteFrame::AllOk(void) {

    button_ok->Enable(is_ok());

}



//if a key is pressed in the keyboard, I call this function
void RouteFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        //the user pressed escape -> I do as if the user pressed button_cancel

        wxCommandEvent dummy;

        OnPressCancel(dummy);

    }
    else {

        if (((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
            //the user pressed return or numpad return

            if (is_ok()) {
                //if all fields are ok, I do as if the user presssed button_ok

                wxCommandEvent dummy;

                OnPressOk(dummy);

            }

        }

    }

    event.Skip(true);

}

//run check on all the GUI fields that are members of SightFrame and write true/false in their ok variables
template<class E> void RouteFrame::Check(E& event) {

    (*(type->check))(event);
    
    
    if(((type->name->GetValue()) == wxString((Route_types[0]).value)) || ((type->name->GetValue()) == wxString((Route_types[1]).value))){
        //*route is either a loxodrome or an orthodrome -> I check the fields related to loxodromes and orthodromes
        
        (*(Z->check))(event);
        if(!for_transport){
            //if *route is not a Route for transport, then its starting position matters -> I check it
            (*(start_phi->check))(event);
            (*(start_lambda->check))(event);
        }
    
        
        switch ((String(length_format->name->GetValue().ToStdString()).position_in_list(length_format->catalog))) {
                
            case 0:
                //length format is time x speed
                
                (*(time->check))(event);
                (*(speed->check))(event);
                
                break;   
                
            case 1:
                //length format is simply length
                
                (*(length->check))(event);
                
                break;
            
        }
        
        
    }else{
        //*route is a circle of equal altitude ->  I check the fields related to circles of equal altitude
        
        (*(omega->check))(event);
        (*(GP_phi->check))(event);
        (*(GP_lambda->check))(event);

    }
    
    (*(label->check))(event);

    event.Skip(true);

}


void RouteFrame::Check(void) {
    
    wxCommandEvent dummy;
    
    Check(dummy);
    
}


//set the values in all the GUI fields equal to the values in the respective non-GUI fields
void RouteFrame::set(void) {

    type->set();
    length_format->set();

    if ((route->type.value) == wxString(((Route_types[2]).value))) {
        //I disable the GUI fields which do not define a circle of equal altitude and set the others

        Z->Enable(false);

        start_phi->Enable(false);
        start_lambda->Enable(false);

        GP_phi->set();
        GP_lambda->set();
        omega->set();

    }
    else {
        //I disable the GUI fields which do not define a loxodrome or orthodrome and set the others

        wxCommandEvent dummy;

        Z->set();

        start_phi->set();
        start_lambda->set();
        start_phi->Enable(!for_transport);
        start_lambda->Enable(!for_transport);

        GP_phi->Enable(false);
        GP_lambda->Enable(false);
        omega->Enable(false);

    }

    //enable the length or the time and speed fields
    OnChooseLengthFormatField();

    if ((route->length_format) == (LengthFormat_types[1])) {
        //the Route length is simply expressed as a length rather than as a time and speed -> set length field

        length->set();

    }
    else {
        //the Route length is expressed as a time and a speed -> set time and speed field, and set also the length field as the product of the time and speed

        time->set();
        speed->set();
        
        route->set_length_from_time_speed();
        //        (route->length) = Length(route->time, route->speed);
        length->set();

    }

    Check();
    label->set();


}


//set the values in all the non-GUI fields equal to the values in the respective GUI fields
template<class T> void RouteFrame::get(T& event) {

    type->MultipleItemField<RouteFrame, RouteType, CheckRouteType<RouteFrame> >::Get(event);

    if (((type->name)->GetValue()) == wxString(((Route_types[2]).value))) {

        GP_phi->get(event);
        GP_lambda->get(event);
        omega->get(event);

    }else {

        Z->get(event);
        start_phi->get(event);
        start_lambda->get(event);

        if ((length_format->name->GetValue()) == length_format->catalog[0]) {
            //in the GUI field, lengths are expressed at Chrono x Speed -> get t and v and set in the non-GUI field to true. I also set route->l according to time and speed

            (route->length_format) = LengthFormat(((LengthFormat_types[0]).value));
            time->get(event);
            speed->get(event);
            (route->length) = Length(route->time, route->speed);

        }else {
            //in the GUI field, lenght are expressed simply as a Length -> get l and set in the non-GUI field to false

            (route->length_format) = LengthFormat(((LengthFormat_types[1]).value));
            length->get(event);

        }

    }

    label->get(event);


}


//enable/disable the GUI fields in *this accoridng to the choice in type->name (the sleected type of Route)
template<class E> void RouteFrame::OnChooseLengthFormatField(E& event) {
    
    if ((type->is_ok())) {
        //type is valid
        
        if ((type->name->GetValue()) != wxString(((Route_types[2]).value))) {
            //the Route is either a loxodrome or an orthodrome -> the Route allows for a Length -> enable length_format
            
            bool b = false;
            
            //given that loxodromes and orthodromes allow for a Length, I enable length_format
            length_format->Enable(true);
            
            if(length_format->is_ok()){
                //length_format has a valid content -> enable / disable the relative fields
                
                //run over all entries of length_format->catalog and store in i the id of the entry that is equal to l_format->name->GetValue()
                switch ((String((length_format->name->GetValue().ToStdString())).position_in_list(length_format->catalog))) {
                        
                    case 0: {
                        //l_format->name->GetValue() = "Time x speed" -> disable l, enable v and t
                        
                        b = true;
                        break;
                        
                    }
                        
                    case 1: {
                        //l_format->name->GetValue() = ((LengthFormat_types[1]).value) -> enable l, disable v and t
                        
                        b = false;
                        break;
                        
                    }
                        
                }
                
                
                time->Enable(b);
                speed->Enable(b);
                length->Enable(!b);
                text_time->Enable(b);
                text_speed->Enable(b);
                text_length->Enable(!b);
                
            }else{
                //length_format does not have a valid content -> disable all relative fields
                
                time->Enable(false);
                speed->Enable(false);
                length->Enable(false);
                text_time->Enable(false);
                text_speed->Enable(false);
                text_length->Enable(false);
                
            }
            
        }else {
            //the Route is a circle of equal altitude -> the length is not defined -> disable the length_format field as well as all fields related to the length
            
            length_format->Enable(false);
            
            time->Enable(false);
            speed->Enable(false);
            length->Enable(false);
            text_time->Enable(false);
            text_speed->Enable(false);
            text_length->Enable(false);
            
        }
        
    }else{
        //type is not vlaid
        
        length_format->Enable(false);
        
        time->Enable(false);
        speed->Enable(false);
        length->Enable(false);
        text_time->Enable(false);
        text_speed->Enable(false);
        text_length->Enable(false);
        
    }
    
    event.Skip(true);
    
}


//when time or speed are edited in RouteFrame, thie method updates the length GUI field by writing in it time x speed
template<class E> void RouteFrame::UpdateLength(E& event) {

    if ((time->is_ok()) && (speed->is_ok())) {

        length->set(Length(*(time->chrono), *(speed->speed)));

    }
    else {

        length->value->SetValue(wxString(""));
        length->unit->name->SetValue(wxString(""));

    }

    event.Skip(true);

}


//same as RouteFrame::OnChooseLengthFormat(E& event), but it does not accept any argument
void RouteFrame::OnChooseLengthFormatField(void) {

    wxCommandEvent dummy;

    OnChooseLengthFormatField(dummy);

}


//write all the content in the GUI fields into the non-GUI objects, checks whether all the fields in PositionFrame are ok and if they are it returns true and false otherwise
bool PositionFrame::is_ok(void) {

    wxCommandEvent dummy;

    get(dummy);

    return((lat->is_ok()) && (lon->is_ok()));


}

//if all_ok() returns turue it enables  button_add, and it disables it otherwise
void PositionFrame::AllOk(void) {

    button_ok->Enable(is_ok());

}



//if a key is pressed in the keyboard, I call this function
void PositionFrame::KeyDown(wxKeyEvent& event) {

    if ((event.GetKeyCode()) == WXK_ESCAPE) {
        // the use pressed escape -> I do as if the user pressed button_cancel

        wxCommandEvent dummy;

        OnPressCancel(dummy);

    }
    else {

        if (((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
            //the user pressed return or numpad return

            if (is_ok()) {
                //if all fields are ok, I do as if the user presssed button_ok

                wxCommandEvent dummy;

                OnPressOk(dummy);

            }

        }

    }

    event.Skip(true);

}

//this is an event table with template
//BEGIN_EVENT_TABLE_TEMPLATE1(MessageFrame, /*here I put the derived class*/wxFrame, /*here I put the template argument*/FF_OK)
//    EVT_PAINT(MessageFrame<FF_OK>::OnPaint)
//END_EVENT_TABLE()


template<typename FF_OK> MessageFrame<FF_OK>::MessageFrame(wxWindow* parent, FF_OK* f_ok_in, const wxString& title, const wxString& message, String image_path, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) {

    wxRect rectangle;

    f_ok = f_ok_in;

    //SetColor(this);
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< MessageFrame<FF_OK> >(this);

    //image
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (wxGetApp().display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);

    //allocate sizers
    frame_sizer = new wxBoxSizer(wxVERTICAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    //    sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    //    sizer_grid = new wxGridSizer(3, 1, 0, 0);


    StaticText* text = new StaticText(panel, message, wxDefaultPosition, wxDefaultSize, 0);

    //bind the function MessageFrame<FF_OK>::KeyDown to the event where a keyboard dey is down
    panel->Bind(wxEVT_KEY_DOWN, &MessageFrame<FF_OK>::KeyDown, this);


    //buttons
    button_ok = new wxButton(panel, wxID_ANY, "Ok", wxDefaultPosition, wxDefaultSize);
    //    button_ok->Bind(wxEVT_BUTTON, &MessageFrame::OnPressOk, this);
    button_ok->Bind(wxEVT_BUTTON, *close_frame);
    button_ok->Bind(wxEVT_BUTTON, *f_ok);

    image = new StaticBitmap(
                             panel,
                             image_path,
                             //I use ToDIP to adjust the size independently of the screen resolution
                             ToDIP(wxSize(
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
                                          ))
                             );

    sizer_v->Add(text, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_v->Add(button_ok, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_v);
    panel->Fit();
    Fit();

    CentreOnScreen();

}

//if a key is pressed in the keyboard, I call this function
template<typename FF_OK> void MessageFrame<FF_OK>::KeyDown(wxKeyEvent& event) {

    if (((event.GetKeyCode()) == WXK_ESCAPE) || ((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) {
        //the user presses esc or return -> I close *this and set the idling variable to false

        (*close_frame)(event);
        ((f_ok->parent)->idling) = false;

    }

}

//template<typename FF_OK> void MessageFrame<FF_OK>::OnPaint(wxPaintEvent& WXUNUSED(event)){
//
//    wxPaintDC dc(this);
//    wxScopedPtr<wxGraphicsContext> gc(wxGraphicsContext::Create(dc));
//
//    gc->SetFont(*wxNORMAL_FONT, *wxBLACK);
//    gc->DrawBitmap(*m_bitmap, 0, 0,
//                   (wxGetApp().rectangle_display.GetWidth())*((wxGetApp().size_message_image_over_width_screen).value),
//                   (wxGetApp().rectangle_display.GetWidth())*((wxGetApp().size_message_image_over_width_screen).value)
//                   );
//
//}


template<typename F_A, typename F_B, typename F_ABORT> QuestionFrame<F_A, F_B, F_ABORT>::QuestionFrame(wxWindow* parent, F_A* f_a_in, String string_a_in, F_B* f_b_in, String string_b_in, F_ABORT* f_abort_in, bool enable_button_a_in, bool enable_button_b_in, bool bind_esc_to_button_b_in, const wxString& title, const wxString& message, String path_icon_file, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN) {

    wxRect rectangle;
    vector<StaticText*> text;
    //the wxString message will be split into multiple Strings which were separated by a '\n' in message, each string will be written in an entry of message_split
    vector<String> message_split;
    int i;

    f_a = f_a_in;
    string_a = string_a_in;
    f_b = f_b_in;
    string_b = string_b_in;
    f_abort = f_abort_in;
    
    enable_button_a = enable_button_a_in;
    enable_button_b = enable_button_b_in;
    
    bind_esc_to_button_b = bind_esc_to_button_b_in;

    //SetColor(this);
    panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));
    close_frame = new CloseFrame< QuestionFrame<F_A, F_B, F_ABORT> >(this);

    //image
    //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
    rectangle = (wxGetApp().display.GetClientArea());
    rectangle.SetWidth((int)((double)rectangle.GetWidth()) * 1. / 1000.0);
    rectangle.SetHeight((int)((double)rectangle.GetHeight()) * 1. / 1000.0);

    //allocate sizers
    sizer_v = new wxBoxSizer(wxVERTICAL);
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_grid = new wxGridSizer(1, 2, 0, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

    //write each line into text
    for (i = 0, message_split = String(message.ToStdString()).split(), text.resize(message_split.size()); i < (message_split.size()); i++) {
        text[i] = new StaticText(panel, wxString((message_split[i]).value), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
    }

    //buttons
    button_a = new wxButton(panel, wxID_ANY, string_a.value, wxDefaultPosition, wxDefaultSize);
    button_a->Bind(wxEVT_BUTTON, *f_a);
    button_a->Bind(wxEVT_BUTTON, *close_frame);
    button_b = new wxButton(panel, wxID_ANY, string_b.value, wxDefaultPosition, wxDefaultSize);
    button_b->Bind(wxEVT_BUTTON, *f_b);
    button_b->Bind(wxEVT_BUTTON, *close_frame);

    panel->Bind(wxEVT_KEY_DOWN, &QuestionFrame::KeyDown<wxKeyEvent>, this);

    image = new StaticBitmap(
                             panel,
                             path_icon_file,
                             //I use ToDIP to adjust the size independently of the screen resolution
                             ToDIP(wxSize(
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value),
                                          (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_message_image_over_width_screen).value)
                                          ))
                             );

    //add all entries of text to sizer_v
    for (i = 0; i < text.size(); i++) {
        sizer_v->Add(text[i], 0, wxALL | wxALIGN_CENTER);
    }
    sizer_v->Add(image, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_grid->Add(button_a, 0, wxALIGN_CENTER);
    sizer_grid->Add(button_b, 0, wxALIGN_CENTER);
    sizer_v->Add(sizer_grid, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    sizer_h->Add(sizer_v, 0, wxALL | wxALIGN_CENTER, 2 * (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));

#ifdef _WIN32
    //if I am on WIN32, I set the icon from the icon set in the .rc file
    SetIcon(wxICON(app_icon));
#endif

    //panel->SetSizer(sizer);
    //    Maximize(panel);
    panel->SetSizerAndFit(sizer_h);
    panel->Fit();
    Fit();

    CentreOnScreen();

}




//if the user presses return/escape, I call f_a / f_b
template<typename F_A, typename F_B, typename F_ABORT> template<class E> void QuestionFrame<F_A, F_B, F_ABORT>::KeyDown(E& event) {

    wxCommandEvent dummy;

    if (((event.GetKeyCode()) == WXK_ESCAPE) && enable_button_b) {
        //the user pressed esc
        
        if(bind_esc_to_button_b){
            //esc button is bound to button_b -> call *f_b
            
            (*f_b)(dummy);
            
        }else{
            //esc button is bound to *f_abort -> call *f_abort

            
            (*f_abort)(dummy);
            
        }


    }else {

        if ((((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER)) && enable_button_a) {
            //the user pressed return or numpad return

            (*f_a)(dummy);

        }

    }

    (*close_frame)(dummy);


    event.Skip(true);

}


template<class T, typename FF_OK> PrintMessage<T, FF_OK>::PrintMessage(T* f_in, FF_OK* f_ok_in) {

    f = f_in;
    f_ok = f_ok_in;

}

//set the wxControl, title,  message  and image_path for the functor *this, and I call the functor operator() with CallAfter
template<class T, typename FF_OK> void PrintMessage<T, FF_OK>::SetAndCall(wxControl* control_in, String title_in, String message_in, String image_path_in) {

    control = control_in;
    title = title_in;
    message = message_in;
    image_path = image_path_in;

    f->CallAfter(*this);

}

template<class T, typename F_YES, typename F_NO, typename F_ABORT> ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::ShowQuestionFrame(T* f_in, F_YES* f_yes_in, F_NO* f_no_in, F_ABORT* f_abort_in) {

    f = f_in;
    f_yes = f_yes_in;
    f_no = f_no_in;
    f_abort = f_abort_in;

}


//set the wxControl, title and question and answers for the functor *this,  set enable_button_y/n both to true,  and bind_esc_to_button_b to true. Then call the functor operator() with CallAfter
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::SetAndCall(wxControl* control_in, String title_in, String question_in, String answer_y_in, String answer_n_in) {

    control = control_in;
    title = title_in;
    question = question_in;
    answer_y = answer_y_in;
    answer_n = answer_n_in;
    
    enable_button_a = true;
    enable_button_b = true;
    
    bind_esc_to_button_b = true;

    f->CallAfter(*this);

}


//set the wxControl, title and question and answers for the functor *this,  set enable_butoon_y/n to enable_button_y/n_in, and set bind_esc_to_button_b = bind_esc_to_button_b_in.  I call the functor operator() with CallAfter
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::SetAndCall(wxControl* control_in, String title_in, String question_in, String answer_y_in, String answer_n_in, bool enable_button_a_in, bool enable_button_b_in, bool bind_esc_to_button_b_in) {

    control = control_in;
    title = title_in;
    question = question_in;
    answer_y = answer_y_in;
    answer_n = answer_n_in;
    
    enable_button_a = enable_button_a_in;
    enable_button_b = enable_button_b_in;
    
    bind_esc_to_button_b = bind_esc_to_button_b_in;

    f->CallAfter(*this);

}


//if question_frame != NULL, enable or disable question_frame->button_a/b according to the boolean variables enable_button_a/b
template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::EnableDisableButtons(void) {

    if(question_frame != NULL){
        
        question_frame->button_a->Enable(enable_button_a);
        question_frame->button_b->Enable(enable_button_b);
        
    }
    
}

template<class T, typename F_YES, typename F_NO, typename F_ABORT> void ShowQuestionFrame<T, F_YES, F_NO, F_ABORT>::operator()(void) {


    SetIdling<T>* set_idling;
    UnsetIdling<T>* unset_idling;

    set_idling = new SetIdling<T>(f);
    unset_idling = new UnsetIdling<T>(f);


    if (!(f->idling)) {

        //I may be about to prompt a temporary dialog window, thus I set f->idling to true
        (*set_idling)();

        if (control != NULL) {
            //this question has been prompted from a control

            if (((control->GetForegroundColour()) != (wxGetApp().error_color))) {

                question_frame = new QuestionFrame<F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
                question_frame->Show(true);
                question_frame->Raise();

                EnableDisableButtons();
                control->SetForegroundColour((wxGetApp().highlight_color));
                control->SetFont(wxGetApp().highlight_font);

            }

        }
        else {
            //this question has not been prompted from a control

            question_frame = new QuestionFrame<F_YES, F_NO, F_ABORT>(f, f_yes, answer_y, f_no, answer_n, f_abort, enable_button_a, enable_button_b, bind_esc_to_button_b, title.value, question.value, wxGetApp().path_file_question_icon, wxDefaultPosition, wxDefaultSize, String(""));
            question_frame->Show(true);
            question_frame->Raise();

            EnableDisableButtons();
            
        }

    }

    //AFTER the question has been aswered and the related frame closed, I unset idling in f
    f->CallAfter(*unset_idling);

}


ListFrame::ListFrame(const wxString& title, [[maybe_unused]] const wxString& message, const wxPoint& pos, const wxSize& size, [[maybe_unused]] String prefix) : wxFrame(NULL, wxID_ANY, title, pos, size) {

    unsigned int i, red, green, blue;
    wxListItem column, item;
    String s;
    vector<wxString> headers;
    wxBoxSizer* sizer_listcontrol_routes_plus_buttons, * sizer_big_buttons;
    vector<wxButton*> disableable_buttons;
    //temporary angles used to read and store the default values of lambda_min ... phi_max from init file
    Angle lambda_min_temp, lambda_max_temp, phi_min_temp, phi_max_temp;
    //pos_open denotes the positions, in the string s composed of the color '(i,j,k)', of '(', pos_comma_1 of the first ',', pos_comma_2 of the second ',', and pos_close of ')'.
    size_t pos_end;

    //the file has not been modified yet -> I set
    file_has_been_modified = false;
    //for the time being, the file has no title
    file_is_untitled = true;
    enable_highlight = true;
    selecting_route_for_position = false;
    transporting_with_new_route = false;
    transporting_with_selected_route = false;
    changing_highlighted_object = false;
    abort = false;
    mouse_moving = false;
    //when a ListFrame is created, no Route nor Position is  being dragged
    dragging_object = false;
    i_object_to_disconnect = -1;


    set_idling = new SetIdling<ListFrame>(this);
    unset_idling = new UnsetIdling<ListFrame>(this);
    confirm_transport = new ConfirmTransport<ListFrame>(this);
    close = new CloseFrame<ListFrame>(this);
    (*unset_idling)();

    ask_remove_related_sight = new AskRemoveRelatedSight(this);
    ask_remove_related_route = new AskRemoveRelatedRoute(this);
    select_route = new SelectRoute(this);
    print_warning_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(this, unset_idling);
    print_error_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(this, unset_idling);
    print_info_message = new PrintMessage<ListFrame, UnsetIdling<ListFrame> >(this, unset_idling);
    print_question_message = new ShowQuestionFrame<ListFrame, ConfirmTransport<ListFrame>, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(this, confirm_transport, unset_idling, unset_idling);
    //create extract_color with zero size, because I will need extract_color only to get colors
    
    //set icon paths to all print_*_message
    print_warning_message->image_path = wxGetApp().path_file_warning_icon;
    print_info_message->image_path = wxGetApp().path_file_info_icon;
    print_error_message->image_path = wxGetApp().path_file_error_icon;


    data = new Data(catalog, String(""));

    //read show_coastlines from file_init
    show_coastlines.read_from_file_to(String("show coastlines"), (wxGetApp().path_file_init), String("R"), String(""));
    //read load_sample_sight from file_init
    load_sample_sight.read_from_file_to(String("load sample sight"), (wxGetApp().path_file_init), String("R"), String(""));
    
    //set circle_observer_0.omega
    circle_observer_0.omega.read_from_file_to(String("omega draw 3d"), (wxGetApp().path_file_init), String("R"), String(""));
    
    //set rectangle_obseerver
    //read lambda_min, ...., phi_max from file_init
    lambda_min_temp.read_from_file_to(String("minimal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
    lambda_max_temp.read_from_file_to(String("maximal longitude"), (wxGetApp().path_file_init), String("R"), String(""));
    phi_min_temp.read_from_file_to(String("minimal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
    phi_max_temp.read_from_file_to(String("maximal latitude"), (wxGetApp().path_file_init), String("R"), String(""));
    rectangle_observer_0 = PositionRectangle(Position(lambda_min_temp, phi_max_temp), Position(lambda_max_temp, phi_min_temp), String(""));




    LoadCoastLineData(String(""));

    if (!abort) {
        //the user has not pressed cancel while charts were loading -> I proceed

        //obtain width and height of the display, and create an image with a size given by a fraction of the size of the display
        (wxGetApp().rectangle_display) = ((wxGetApp().display).GetClientArea());

        (wxGetApp().file_init).set_name((wxGetApp().path_file_init));

        //read color list from file_init
        s.read_from_file_to(String("color list"), (wxGetApp().path_file_init), String("R"), String(""));

        //in file_init, each color is written as '(i,j,k) ', where i, j, k are the integers for the levels of red, green and blue. To cound the number of colors, I thus count the number of '(' in the string
        (wxGetApp().color_list).resize(count((s.value).begin(), (s.value).end(), '('));

        //when the ListFrame is created there is no open selection rectangle in any ChartFrame
        selection_rectangle = false;

        //SetColor(this);

        for (i = 0; i < (wxGetApp().color_list).size(); i++) {

            //get rid of everything that comes before and at '(' at the beginnign of s
            pos_end = (s.value).find("(");
            s.set(String(""), String((s.value).substr(pos_end + 1).c_str()), String(""));
            //look for the first ','

            pos_end = (s.value).find(",");

            //read red
            red = stoi(((s.value).substr(0, pos_end)).c_str());

            //get rid of the first ','
            s.set(String(""), String((s.value).substr(pos_end + 1).c_str()), String(""));

            pos_end = (s.value).find(",");

            green = stoi((s.value).substr(0, pos_end).c_str());

            //get rid of the second ','
            s.set(String(""), String((s.value).substr(pos_end + 1).c_str()), String(""));

            pos_end = (s.value).find(")");
            //get rid of '('
            blue = stoi((s.value).substr(0, pos_end + 1).c_str());

            //write the color that I just read in color_list
            (wxGetApp().color_list)[i] = Color(red, green, blue);

        }

        //no positions nor routes are highlighted when ListFrame is constructed
        highlighted_route_now = -1;
        highlighted_position_now = -1;

        menu_bar = new wxMenuBar;
        menu_app = new wxMenu;
        menu_file = new wxMenu;
        menu_chart = new wxMenu;
        menu_new_chart = new wxMenu;
        menu_item_mercator = new wxMenu;
        menu_item_3d = new wxMenu;

        menu_new_chart->Append(wxID_HIGHEST + 1, "Mercator\tCtrl-m");
        menu_new_chart->Append(wxID_HIGHEST + 2, "3D\tCtrl-3");
        menu_chart->AppendSubMenu(menu_new_chart, wxT("New"), wxT(""));
        menu_chart->Append(wxID_HIGHEST + 3, "Close\tCtrl-c");
        menu_chart->Append(wxID_HIGHEST + 4, "Close all\tCtrl-a");
        menu_app->Append(wxID_HIGHEST + 5, "Quit\tCtrl-q");
        menu_file->Append(wxID_HIGHEST + 6, "Open\tCtrl-o");
        menu_file->Append(wxID_HIGHEST + 7, "Close\tCtrl-w");
        menu_file->Append(wxID_HIGHEST + 8, "Save\tCtrl-s");
        menu_file->Append(wxID_HIGHEST + 9, "Save as...\tCtrl-Shift-s");

        menu_bar->Append(menu_app, wxT("&App"));
        menu_bar->Append(menu_file, wxT("&File"));
        menu_bar->Append(menu_chart, wxT("&Chart"));
        SetMenuBar(menu_bar);

        menu_file->Enable(wxID_HIGHEST + 7, false);

        menu_new_chart->Bind(wxEVT_MENU, &ListFrame::OnAddChartFrame, this, wxID_HIGHEST + 1);
        menu_new_chart->Bind(wxEVT_MENU, &ListFrame::OnAddChartFrame, this, wxID_HIGHEST + 2);
        menu_chart->Bind(wxEVT_MENU, &ListFrame::OnCloseActiveChartFrame, this, wxID_HIGHEST + 3);
        menu_chart->Bind(wxEVT_MENU, &ListFrame::OnCloseAllChartFrames, this, wxID_HIGHEST + 4);
        menu_bar->Bind(wxEVT_MENU, &MyApp::OnPressCtrlQ<wxCommandEvent>, &(wxGetApp()), wxID_HIGHEST + 5);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlO<wxCommandEvent>, this, wxID_HIGHEST + 6);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlW<wxCommandEvent>, this, wxID_HIGHEST + 7);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlS<wxCommandEvent>, this, wxID_HIGHEST + 8);
        menu_file->Bind(wxEVT_MENU, &ListFrame::OnPressCtrlShiftS<wxCommandEvent>, this, wxID_HIGHEST + 9);


        on_select_route_in_listcontrol_routes_for_transport = new OnSelectRouteInListControlRoutesForTransport(this);
        on_new_route_in_listcontrol_routes_for_transport = new OnNewRouteInListControlRoutesForTransport(this);

        //initialize delete_sight, which defines the functor to delete the sight but not its related route (it is called when the user answers 'n' to QuestionFrame)
        delete_sight = new DeleteSight(this, Answer('n', String("")));
        //initialize delete_sight_and_related_route, which defines the functor to delete the sight and its related route (it is called when the user answers 'y' to QuestionFrame)
        delete_sight_and_related_route = new DeleteSight(this, Answer('y', String("")));

        //initialize delete_route, which defines the functor to delete the route but not its related sight (it is called when the user answers 'n' to QuestionFrame)
        delete_route = new DeleteRoute(this, Answer('n', String("")));
        //initialize delete_route_and_related_sight, which defines the functor to delete the route and its related sight (it is called when the user answers 'y' to QuestionFrame)
        delete_route_and_related_sight = new DeleteRoute(this, Answer('y', String("")));

        //initialize delete_position, which defines the functor to delete a Position
        delete_position = new DeletePosition(this);


        //initialized existing_route and create_route, which define the functors to modify / create a Route
        existing_route = new ExistingRoute(this);
        new_route = new NewRoute(this);

        catalog = new Catalog((wxGetApp().path_file_catalog), String(""));

        panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT(""));


        sizer_all = new wxBoxSizer(wxVERTICAL);
        sizer_h = new wxBoxSizer(wxHORIZONTAL);
        sizer_v = new wxBoxSizer(wxVERTICAL);
        sizer_listcontrol_routes_plus_buttons = new wxBoxSizer(wxHORIZONTAL);
        sizer_big_buttons = new wxBoxSizer(wxVERTICAL);

        sizer_buttons_sight = new wxBoxSizer(wxHORIZONTAL);
        sizer_buttons_position = new wxBoxSizer(wxHORIZONTAL);
        sizer_buttons_route = new wxBoxSizer(wxHORIZONTAL);

        sizer_box_sight = new wxStaticBoxSizer(wxVERTICAL, panel, "Sights");
        sizer_box_position = new wxStaticBoxSizer(wxVERTICAL, panel, "Positions");
        sizer_box_route = new wxStaticBoxSizer(wxVERTICAL, panel, "Routes");
        

        //button to modify a sight
        button_modify_sight = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_pencil_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_modify_sight->Bind(wxEVT_BUTTON, &ListFrame::OnModifySight<wxCommandEvent>, this);
        button_modify_sight->Enable(false);
        button_modify_sight->SetToolTip(wxString("Modify the selected sight"));

        //button to transport a sight
        button_transport_sight = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_arrow_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_transport_sight->Bind(wxEVT_BUTTON, &ListFrame::OnTransportSight, this);
        button_transport_sight->Enable(false);
        button_transport_sight->SetToolTip(wxString("Transport the selected sight"));


        //button to disconnect a sight
        button_disconnect_sight = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_disconnect_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_disconnect_sight->Bind(wxEVT_BUTTON, &ListFrame::OnDisconnectSight, this);
        button_disconnect_sight->Enable(false);
        button_disconnect_sight->SetToolTip(wxString("Disconnect the selected sight from its route"));



        //button to modify a position
        button_modify_position = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_pencil_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_modify_position->Bind(wxEVT_BUTTON, &ListFrame::OnModifyPosition<wxCommandEvent>, this);
        button_modify_position->Enable(false);
        button_modify_position->SetToolTip(wxString("Modify the selected position"));

        //button to transport a position
        button_transport_position = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_arrow_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_transport_position->Bind(wxEVT_BUTTON, &ListFrame::OnTransportPosition, this);
        button_transport_position->Enable(false);
        button_transport_position->SetToolTip(wxString("Transport the selected position"));

        //button to transport a Route
        button_transport_route = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_arrow_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_transport_route->Bind(wxEVT_BUTTON, &ListFrame::OnTransportRoute, this);
        button_transport_route->Enable(false);
        button_transport_route->SetToolTip(wxString("Transport the selected route"));

        //button to disconect a Route
        button_disconnect_route = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_disconnect_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_disconnect_route->Bind(wxEVT_BUTTON, &ListFrame::OnDisconnectRoute, this);
        button_disconnect_route->Enable(false);
        button_disconnect_route->SetToolTip(wxString("Disconnect the selected route from its sight"));


        //button to modify a route
        button_modify_route = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_pencil_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_modify_route->Bind(wxEVT_BUTTON, &ListFrame::OnModifyRoute<wxCommandEvent>, this);
        button_modify_route->Enable(false);
        button_modify_route->SetToolTip(wxString("Modify the selected route"));

        //button to delete a sight
        button_delete_sight = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_trash_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_delete_sight->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteSight<wxCommandEvent>, this);
        button_delete_sight->Enable(false);
        button_delete_sight->SetToolTip(wxString("Delete the selected sight"));

        //button to delete a position
        button_delete_position = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_trash_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_delete_position->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeletePosition<wxCommandEvent>, this);
        button_delete_position->Enable(false);
        button_delete_position->SetToolTip(wxString("Delete the selected position"));

        //button to delete a route
        button_delete_route = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_trash_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_delete_route->Bind(wxEVT_BUTTON, &ListFrame::OnPressDeleteRoute<wxCommandEvent>, this);
        button_delete_route->Enable(false);
        button_delete_route->SetToolTip(wxString("Delete the selected route"));


        //listcontrol_sights with sights
        disableable_buttons.clear();
        disableable_buttons.push_back(button_modify_sight);
        disableable_buttons.push_back(button_transport_sight);
        //    disableable_buttons.push_back(button_disconnect_sight);
        disableable_buttons.push_back(button_delete_sight);
        
        
        

        listcontrol_sights = new ListControl<Sight>(panel, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
        on_change_selection_in_listcontrol_sights = new OnChangeSelectionInListControl<Sight>(listcontrol_sights, String("sight"));
        //SetColor(listcontrol_sights);
        //    listcontrol_sights->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_sights);
        listcontrol_sights->Bind(wxEVT_LIST_ITEM_SELECTED, *on_change_selection_in_listcontrol_sights);
        listcontrol_sights->Bind(wxEVT_LIST_ITEM_DESELECTED, *on_change_selection_in_listcontrol_sights);
        listcontrol_sights->SetToolTip(wxString("List of sights"));

        headers.clear();
        headers.push_back(wxString("Number"));
        headers.push_back(wxString("Body"));
        headers.push_back(wxString("Limb"));
        headers.push_back(wxString("Artificial horizon"));
        headers.push_back(wxString("Sextant altitude"));
        headers.push_back(wxString("Index error"));
        headers.push_back(wxString("Height of eye"));
        headers.push_back(wxString("Master-clock date and hour (UTC)"));
        headers.push_back(wxString("Stopwatch"));
        headers.push_back(wxString("Stopwatch reading"));
        headers.push_back(wxString("TAI - UTC"));
        headers.push_back(wxString("Label"));
        headers.push_back(wxString("Related route"));
        listcontrol_sights->SetColumns(headers);




        sizer_box_sight->Add(listcontrol_sights, 0, wxALL, ((wxGetApp().border).value));


        //listcontrol_positions with positions
        disableable_buttons.clear();
        disableable_buttons.push_back(button_modify_position);
        disableable_buttons.push_back(button_transport_position);
        disableable_buttons.push_back(button_delete_position);


        listcontrol_positions = new ListControl<Position>(panel, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
        on_change_selection_in_listcontrol_positions = new OnChangeSelectionInListControl<Position>(listcontrol_positions, String("position"));
        listcontrol_positions->Bind(wxEVT_LIST_ITEM_SELECTED, *on_change_selection_in_listcontrol_positions);
        listcontrol_positions->Bind(wxEVT_LIST_ITEM_DESELECTED, *on_change_selection_in_listcontrol_positions);
        listcontrol_positions->SetToolTip(wxString("List of positions"));

        headers.clear();
        headers.push_back(wxString("Number"));
        headers.push_back(wxString("Latitude"));
        headers.push_back(wxString("Longitude"));
        headers.push_back(wxString("Label"));
        listcontrol_positions->SetColumns(headers);

        sizer_box_position->Add(listcontrol_positions, 1, wxALL, ((wxGetApp().border).value));


        //listcontrol routes with routes
        disableable_buttons.clear();
        disableable_buttons.push_back(button_modify_route);
        disableable_buttons.push_back(button_transport_route);
        //    disableable_buttons.push_back(button_disconnect_route);
        disableable_buttons.push_back(button_delete_route);

        listcontrol_routes = new ListControl<Route>(panel, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
        on_change_selection_in_listcontrol_routes = new OnChangeSelectionInListControl<Route>(listcontrol_routes, String("route"));
        //SetColor(listcontrol_routes);
        //    listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *on_select_in_listcontrol_routes);
        listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *on_change_selection_in_listcontrol_routes);
        listcontrol_routes->Bind(wxEVT_LIST_ITEM_DESELECTED, *on_change_selection_in_listcontrol_routes);
        listcontrol_routes->SetToolTip(wxString("List of routes"));
        //I bind ListFrame::OnMouseMovement to listcontrol_sights, listcontrol_routes and to panel, because I want ListFrame::OnMouseMovement to be called when the mouse is either on listcontrol_sights, listcontrol_routes and on panel
        listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);
        panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, this);


        //bind all listcontrols to mouse double-click event, so when the user double clicks on an item in the listcontrol, the SightFrame, PositionFrame or RouteFrame is opened to modify the sight, position or route
        listcontrol_sights->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifySight<wxListEvent>, this);
        listcontrol_positions->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyPosition<wxListEvent>, this);
        listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, this);


        headers.clear();
        headers.push_back(wxString("Number"));
        headers.push_back(wxString("Type"));
        headers.push_back(wxString("Start"));
        headers.push_back(wxString("Z"));
        headers.push_back(wxString("Length"));
        headers.push_back(wxString("Ground Position"));
        headers.push_back(wxString("Omega"));
        headers.push_back(wxString("Label"));
        headers.push_back(wxString("Related Sight"));
        listcontrol_routes->SetColumns(headers);


        sizer_box_route->Add(listcontrol_routes, 1, wxALL, ((wxGetApp().border).value));

        //bing everything to KeyDown method, so when a key is pressed on *this, panel, listcontrol... then KeyDown is called
        Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        panel->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        listcontrol_sights->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        listcontrol_routes->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);
        listcontrol_positions->Bind(wxEVT_KEY_DOWN, &ListFrame::KeyDown<wxKeyEvent>, this);


        if (load_sample_sight == Answer('y', String(""))) {
            //I read a sample sight from file default_open_directory/sample_sight.nav, store into sight and set all the fields in this to the data in sight with set()

            //I am loading data from path_file_sample_sight -> set name of data_file accordingly
            data_file.set_name((wxGetApp().path_file_sample_sight));



#ifdef __APPLE__
            //I am on APPLE operating system -> read the file from hard drive locatd in the Data directory

            data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("RW"), String(""));

#endif
#ifdef _WIN32
            //I am on WIN32 operating system


            //Fork
            //Case 1: If I open a sample sight file at startup stored in Windows resources, use this
            data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("R"), String(""));
            //Case 2: If I open a file on disk, use this
            // data->read_from_file_to(String("Data"), (wxGetApp().path_file_sample_sight), String("RW"), String(""));


#endif
            
            //test for Route between two points: I construct a loxodrome that connects (data->position_list)[0] to (data->position_list)[1]
            /*
            Route* r;
            r = new Route(RouteType(Route_types[0]), (data->position_list)[0], (data->position_list)[1]);
            r->compute_end(String(""));
            
            data->route_list.push_back(*r);
            
            */
            
            
            file_is_untitled = false;
            menu_file->Enable(wxID_HIGHEST + 7, true);
            set();
            SetLabel(data_file.name.value);
            PreRenderAll();

        }
        else {

            file_is_untitled = true;

            //there is no file-> I initialize recent_bodies ect in lexicographic order
            for (i = 0; i < (data->recent_bodies.size()); i++) {
                (data->recent_bodies)[i] = i;
            }
            for (i = 0; i < (data->recent_projections.size()); i++) {
                (data->recent_projections)[i] = i;
            }

        }


        set();
        
        //button to show map
        button_show_map = new wxBitmapButton(
                                             panel,
                                             wxID_ANY,
                                             Bitmap(wxGetApp().path_file_map_icon, wxGetApp().size_large_button - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                             wxDefaultPosition,
                                             (wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value))),
                                             wxBU_EXACTFIT | wxSIMPLE_BORDER
                                             );
        button_show_map->Bind(wxEVT_BUTTON, &MyApp::ShowCharts<wxCommandEvent>, &wxGetApp());
        button_show_map->SetToolTip(wxString("Show the chart"));
        
        //button to compute astronomical position
        button_compute_position = new wxBitmapButton(
                                                     panel,
                                                     wxID_ANY,
                                                     Bitmap(wxGetApp().path_file_position_icon, wxGetApp().size_large_button - ToDIP(wxSize(((wxGetApp().border).value), ((wxGetApp().border).value)))),
                                                     wxDefaultPosition,
                                                     (wxSize((wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value), (wxGetApp().rectangle_display.GetWidth()) * ((wxGetApp().size_large_button_over_width_screen).value))),
                                                     wxBU_EXACTFIT | wxSIMPLE_BORDER
                                                     );
        button_compute_position->Bind(wxEVT_BUTTON, &ListFrame::ComputePosition<wxCommandEvent>, this);
        button_compute_position->SetToolTip(wxString("Compute the astronomical position"));


        //button to add a sight
        button_add_sight = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_plus_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_add_sight->Bind(wxEVT_BUTTON, &ListFrame::OnAddSight, this);
        button_add_sight->SetToolTip(wxString("Add a sight"));


        //button to add a position
        button_add_position = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_plus_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_add_position->Bind(wxEVT_BUTTON, &ListFrame::OnAddPosition, this);
        button_add_position->SetToolTip(wxString("Add a position"));

        //button to add a route
        button_add_route = new wxBitmapButton(
            panel,
            wxID_ANY,
            Bitmap(wxGetApp().path_file_plus_icon, wxGetApp().size_small_button),
            wxDefaultPosition,
            wxDefaultSize,
            wxBU_EXACTFIT | wxSIMPLE_BORDER
        );
        button_add_route->Bind(wxEVT_BUTTON, &ListFrame::OnAddRoute, this);
        button_add_route->SetToolTip(wxString("Add a route"));

        
        extract_colors = new wxTextCtrl(panel, wxID_ANY, wxS(""), wxDefaultPosition, ToDIP(wxSize(0, 0)));


        sizer_buttons_sight->Add(button_add_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_modify_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_transport_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_disconnect_sight, 0, wxALIGN_CENTER);
        sizer_buttons_sight->Add(button_delete_sight, 0, wxALIGN_CENTER);
        sizer_box_sight->Add(sizer_buttons_sight, 0, wxALIGN_LEFT | wxALL, ((wxGetApp().border).value));

        sizer_buttons_position->Add(button_add_position, 0, wxALIGN_CENTER);
        sizer_buttons_position->Add(button_modify_position, 0, wxALIGN_CENTER);
        sizer_buttons_position->Add(button_transport_position, 0, wxALIGN_CENTER);
        sizer_buttons_position->Add(button_delete_position, 0, wxALIGN_CENTER);
        sizer_box_position->Add(sizer_buttons_position, 0, wxALIGN_LEFT | wxALL, ((wxGetApp().border).value));

        sizer_buttons_route->Add(button_add_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_modify_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_transport_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_disconnect_route, 0, wxALIGN_CENTER);
        sizer_buttons_route->Add(button_delete_route, 0, wxALIGN_CENTER);
        sizer_box_route->Add(sizer_buttons_route, 0, wxALIGN_LEFT | wxALL, ((wxGetApp().border).value));


        //

        //resize uniformly all columns
        //    for(i=0; i<(listcontrol_sights->GetColumnCount()); ++i){
        //        listcontrol_sights->SetColumnWidth(i, ((listcontrol_sights->GetSize()).GetWidth())/(listcontrol_sights->GetColumnCount()));
        //    }

        sizer_v->Add(sizer_box_sight, 1, wxALL, ((wxGetApp().border).value));
        sizer_v->Add(sizer_box_position, 1, wxALL, ((wxGetApp().border).value));
        sizer_listcontrol_routes_plus_buttons->Add(sizer_box_route, 0, wxALL, ((wxGetApp().border).value));
        //    sizer_listcontrol_routes_plus_buttons->AddStretchSpacer(1);
        //here I set the flag '0' to avoid button_show_map from being stretched
        sizer_big_buttons->Add(button_compute_position, 0, wxALL | wxALIGN_CENTER, ((wxGetApp().border).value));
        sizer_big_buttons->Add(button_show_map, 0, wxALL | wxALIGN_CENTER, ((wxGetApp().border).value));
        //    sizer_listcontrol_routes_plus_buttons->Add(sizer_big_buttons, 0);
        //by adding the flag wxEXPAND here, I let the StretchSpacer in sizer_listcontrol_routes_plus_buttons expand, and thus I flush to the right button_show_map
        sizer_v->Add(sizer_listcontrol_routes_plus_buttons, 1, wxALL, ((wxGetApp().border).value));
        sizer_h->Add(sizer_v, 1, wxALIGN_BOTTOM, ((wxGetApp().border).value));
        sizer_h->Add(sizer_big_buttons, 0, wxALIGN_CENTER, ((wxGetApp().border).value));
        sizer_all->Add(sizer_h, 1, wxALL | wxEXPAND, ((wxGetApp().border).value));

#ifdef _WIN32
        //if I am on WIN32, I set the icon from the icon set in the .rc file
        SetIcon(wxICON(app_icon));
#endif

        //panel->SetSizer(sizer);
        //	Maximize(panel);
        panel->SetSizerAndFit(sizer_all);
        panel->Fit();
        Fit();

        //given that I have incoroporated the listcontrols into the sizers, listrcontrols may have been resized -> I Fit() them so their content is properly shown
        Resize();
        Centre();

    }

}

//create a new ChartFrame and appends it to the end of chart_frames
void ListFrame::OnAddChartFrame(wxCommandEvent& event) {

    stringstream s;
    Projection projection;

    //recognizes whether the creation of a new chart frame has been triggered by pressing the ((Projection_types[0]).value) or the ((Projection_types[1]).value) button, and writes the respective proejction namee into projection.
    if (event.GetId() == wxID_HIGHEST + 1) {

        projection = Projection_types[0];

    }

    if (event.GetId() == wxID_HIGHEST + 2) {

        projection = Projection_types[1];

    }


    chart_frames.resize(chart_frames.size() + 1);

    s.str("");
    s << "Chart #" << (chart_frames.size()) << " - " << (projection.value) << " projection";

    (chart_frames.back()) = new ChartFrame(
        this,
        projection,
        s.str(),
        /*place each ChartFrame by shifting it with respect to the top-left corner of the screen*/
        wxPoint(0, 0),
        wxDefaultSize,
        String("")
    );
    //    (chart_frames.back())->Show(true);

    chart_frames.back()->Reset(event);
    wxGetApp().ShowCharts(event);
//    wxGetApp().AnimateCharts();
    chart_frames.back()->Animate();



}

//closes the ChartFrame that  has focus
void ListFrame::OnCloseActiveChartFrame(wxCommandEvent& event) {

    unsigned int i;

    //find the ChartFrame in chart_frames taht is Active and closes it
    for (i = 0; (i < (chart_frames.size())) && (((chart_frames[i])->IsActive()) == false); i++) {}

    if (i < (chart_frames.size())) {
        (chart_frames[i])->OnPressCtrlW(event);
    }

}

//closes the ChartFrame that  has focus
void ListFrame::OnCloseAllChartFrames(wxCommandEvent& event) {

    //closes all ChartFrames in chart_frames
    for (; 0 < (chart_frames.size()); ) {
        (chart_frames[0])->OnPressCtrlW(event);
    }


}

//this is the GUI function called when the user wants to compute the position: it calls the non-GUI method data->compute_position and returns GUI error/warning messages according to the output of data->compute_position
void ListFrame::OnComputePosition(void) {

    int output_compute_position;

    output_compute_position = (data->compute_position(String("\t")));

    if (output_compute_position == -1) {
        //the position could not be computed

        print_error_message->SetAndCall(NULL, String("Error"), String("I could not compute the astronomical position! No routes yield valid crossings"), (wxGetApp().path_file_error_icon));

    }else {
        
        switch (output_compute_position) {
                
            case 0:
                //the astronomical Position couldbe computed by using all crossings/Routes and [# used crossings] >= 2

                set();
                //bring all charts to the astronomical position with an animation and do nothing at the end of the animation
                AnimateToObject<Route, UnsetIdling<ListFrame> >(&(data->route_list.back()), unset_idling);

                break;
                
                
            case 1:
                //the astronomical Position couldbe computed by using only some crossings/Routes

                //set all parameters to prepare the printing of an error message, which will be called by ChartTransportHandler in AnimateToObject at the end of the animation. To do this, I enter print_error_message as an argument in the call to AnimateToObject
                print_warning_message->control = NULL;
                print_warning_message->message.set(String("Not all routes could be used to compute the astronomical position! Rome routes yield invalid crossings."));
                print_warning_message->title.set(String("Warning"));
                print_warning_message->image_path.set(wxGetApp().path_file_warning_icon);
     
                set();
                //bring all charts to the astronomical Position with an animation
                AnimateToObject<Route, PrintMessage<ListFrame, UnsetIdling<ListFrame> > >(&(data->route_list.back()), print_warning_message);

                break;
                
                
            case 2:
            
                //the astronomical Position could be computed but not its error -> a Position has been added to position_list, but no Route (repreenting its error circle) has been added to route_list

                //set all parameters to prepare the printing of an error message, which will be called by ChartTransportHandler in AnimateToObject at the end of the animation. To do this, I enter print_error_message as an argument in the call to AnimateToObject
                print_warning_message->control = NULL;
                print_warning_message->message.set(String("The error on the astronomical position could not be computed!"));
                print_warning_message->title.set(String("Warning"));
                print_warning_message->image_path.set(wxGetApp().path_file_warning_icon);
     
                set();
                //bring all charts to the astronomical Position with an animation
                AnimateToObject<Position, PrintMessage<ListFrame, UnsetIdling<ListFrame> > >(&(data->position_list.back()), print_warning_message);

                break;
 
        }

            
        


    }

}


//calls PreRender and FitAll in all che ChartFrames which are children of *this
void ListFrame::PreRenderAll(void) {

    for (long i = 0; i < (chart_frames.size()); i++) {

        //I call FitAll() because the positions have changed, so I need to re-draw the chart
        (((chart_frames[i])->draw_panel)->*(((chart_frames[i])->draw_panel)->PreRender))();
        ((chart_frames[i])->draw_panel)->Refresh();
        ((chart_frames[i])->draw_panel)->FitAll();

    }

}


//call Refresh()es on all chart_frames
void ListFrame::RefreshAll(void) {

    for (long i = 0; i < (chart_frames.size()); i++) {
        ((chart_frames[i])->draw_panel)->Refresh();
    }

}

//call MyRefresh on all chart_frames
void ListFrame::MyRefreshAll(void) {

    for (long i = 0; i < (chart_frames.size()); i++) {
        (chart_frames[i])->draw_panel->MyRefresh();
    }

}

//tabulate Routes in all chart_frames
void ListFrame::TabulateRoutesAll(void) {

    for (long i = 0; i < (chart_frames.size()); i++) {
        (chart_frames[i])->draw_panel->TabulateRoutes();
    }

}

//tabulate Positions in all chart_frames
void ListFrame::TabulatePositionsAll(void) {

    for (long i = 0; i < (chart_frames.size()); i++) {
        (chart_frames[i])->draw_panel->TabulatePositions();
    }

}


//fit the size of all listcontrols inside *this to their respective content and resize the respective sizers and *this to fit the new size of the listcontrols
void ListFrame::Resize(void) {

    listcontrol_sights->Resize(data->sight_list);
    sizer_box_sight->Layout();

    listcontrol_positions->Resize(data->position_list);
    sizer_box_position->Layout();

    listcontrol_routes->Resize(data->route_list);
    sizer_box_route->Layout();

    sizer_v->Layout();
    sizer_h->Layout();
    sizer_all->Layout();

    //    Maximize(panel);
    panel->Fit();
    Fit();
    Layout();

}

//set all the GUI fileds in *this from the data in this->data and adapts the size of columns and panel accordingly
void ListFrame::set() {

    //write the sights contained into data->sight_list into listcontrol_sights
    listcontrol_sights->set(data->sight_list, false);

    //write the positions into data->position_list into listcontrol_sights
    listcontrol_positions->set(data->position_list, false);

    //write the routes into data->route_list into listcontrol_routes
    listcontrol_routes->set(data->route_list, false);

    Resize();
    //    Maximize(panel);

}

void ListFrame::OnAddSight(wxCommandEvent& event) {

    SightFrame* sight_frame = new SightFrame(this, NULL, -1, "New sight", wxDefaultPosition, wxDefaultSize, String(""));
    sight_frame->Show(true);

    event.Skip(true);

}

void ListFrame::OnAddPosition(wxCommandEvent& event) {

    PositionFrame* position_frame = new PositionFrame(this, NULL, -1, "New position", wxDefaultPosition, wxDefaultSize, String(""));
    position_frame->Show(true);

    event.Skip(true);

}

//method to be called when a new Route is added to *this
void ListFrame::OnAddRoute(wxCommandEvent& event) {

    route_frame = new RouteFrame(this, NULL, false, -1, "New route", wxDefaultPosition, wxDefaultSize, String(""));
    route_frame->Show(true);

    event.Skip(true);

}

//method to be called when a new Route is added to *this to transport something
void ListFrame::OnAddRouteForTransport(wxCommandEvent& event) {

    route_frame = new RouteFrame(this, NULL, true, -1, "New route for transport", wxDefaultPosition, wxDefaultSize, String(""));
    route_frame->Show(true);

    event.Skip(true);

}

template<class E> void ListFrame::OnModifySight(E& event) {

    long item;
    item = listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (item != -1) {

        stringstream s;

        s.str("");
        s << "Sight #" << item + 1;

        SightFrame* sight_frame = new SightFrame(this, &((data->sight_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        //        (sight_frame->sight) = &((data->sight_list)[item]);
        sight_frame->Show(true);


    }


    event.Skip(true);

}

void ListFrame::OnTransportSight(wxCommandEvent& event) {

    //I am transporting a Route (related to a Sight)
    transported_object_type = String("sight");

    //here I call ShowQuestionFrame with third functor equal to unset_idling and bind_esc_to_button_b = false, because I want the esc key and button_b to do different things: by pressing esc, the operation is aborted, while by pressing button_b the operation keeps going by using a new Route as transporting Route
    ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>* print_question = new ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>(this, existing_route, new_route, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("You want to transport a sight. With what route do you want to transport? Press ESC to abort."), String("Existing route"), String("New route"), CheckRoutesForTransport(), true, false);

    OnModifyFile();

    event.Skip(true);

}

//this method is called when the useer wants to disconnect a Sight from its related Route
void ListFrame::OnDisconnectSight(wxCommandEvent& event) {

    //set i_object_to_disconnect to the currently selected Sight in listcontrol_sights and call Disconnect to disconnect that Sight from its related Route
    i_object_to_disconnect = ((int)(listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));
    DisconnectAndPromptMessage(event);

}

//this method is called when the useer wants to disconnect a Route from its related Sight
void ListFrame::OnDisconnectRoute(wxCommandEvent& event) {

    //set i_object_to_disconnect to the currently selected Route in listcontrol_routes and call Disconnect to disconnect that Route from its related Sight
    i_object_to_disconnect = (((data->route_list)[(listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED))]).related_sight.value);
    DisconnectAndPromptMessage(event);

}

void ListFrame::OnTransportPosition(wxCommandEvent& event) {

    // I am transporting a Position
    transported_object_type = String("position");

    //here I call ShowQuestionFrame with third functor equal to unset_idling and bind_esc_to_button_b = false, because I want the esc key and button_b to do different things: by pressing esc, the operation is aborted, while by pressing button_b the operation keeps going by using a new Route as transporting Route
    
    //ask the user whether he/she wants to transport the sight with a an existing Route or with a new Route.
    ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>* print_question = new ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>(this, existing_route, new_route, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("You want to transport a position. With what route do you want to transport? Press ESC to abort."), String("Existing route"), String("New route"), CheckRoutesForTransport(), true, false);

    OnModifyFile();

    event.Skip(true);

}



template<class E> void ListFrame::OnModifyPosition(E& event) {

    long item;
    item = listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (item != -1) {

        stringstream s;

        s.str("");
        s << "Position #" << item + 1;

        PositionFrame* position_frame = new PositionFrame(this, &((data->position_list)[item]), item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        position_frame->Show(true);

    }

    event.Skip(true);

}

template<class E> void ListFrame::OnModifyRoute(E& event) {


    long item;
    item = listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    if (item != -1) {

        stringstream s;

        s.str("");
        s << "Route #" << item + 1;

        route_frame = new RouteFrame(this, &((data->route_list)[item]), false, item, s.str().c_str(), wxDefaultPosition, wxDefaultSize, String(""));
        route_frame->Show(true);

    }


    event.Skip(true);

}

void ListFrame::OnTransportRoute(wxCommandEvent& event) {

    //I am transporting a Route
    transported_object_type = String("route");

    //I store the # of the selected Route into i_object_to_transport
    i_object_to_transport = ((int)(listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    //here set i_object_to_transport to the currently selected Route

    //here I call ShowQuestionFrame with third functor equal to unset_idling and bind_esc_to_button_b = false, because I want the esc key and button_b to do different things: by pressing esc, the operation is aborted, while by pressing button_b the operation keeps going by using a new Route as transporting Route
    //ask the user whether he/she wants to transport the sight with a an existing Route or with a new Route.
    ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>* print_question = new ShowQuestionFrame<ListFrame, ExistingRoute, NewRoute, UnsetIdling<ListFrame>>(this, existing_route, new_route, unset_idling);
    
    print_question->SetAndCall(NULL, String(""), String("You want to transport a route. With what route do you want to transport? Press ESC to abort."), String("Existing route"), String("New route"), CheckRoutesForTransport(), true, false);

    OnModifyFile();

    event.Skip(true);

}



template<class E> void ListFrame::OnPressDeleteSight(E& event) {

    //ask the user whether he/she really wants to remove the Sight: if the answer is yes, then QuestionFrame calls the functor ask_remove_related_route. If no, I call the functor unsed_idling, which does nothing and simply sets idling to false
    ShowQuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>* print_question;


    print_question = new ShowQuestionFrame<ListFrame, AskRemoveRelatedRoute, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(this, ask_remove_related_route, unset_idling, unset_idling);

    print_question->SetAndCall(NULL, String(""), String("Do you really want to remove this sight?"), String("Yes"), String("No"));


    event.Skip(true);

}

template<class E> void ListFrame::OnPressDeletePosition(E& event) {

    (delete_position->i_position_to_remove) = ((int)(listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)));

    //ask the user whether he/she really wants to remove the Position: if the answer is yes, then QuestionFrame calls the functor delete_position. If no, I call the functor unsed_idling, which does nothing and simply sets idling to false

    ShowQuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>* print_question;

    print_question = new ShowQuestionFrame<ListFrame, DeletePosition, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(this, delete_position, unset_idling, unset_idling);

    print_question->SetAndCall(NULL, String(""), String("Do you really want to remove this position?"), String("Yes"), String("No"));


    event.Skip(true);

}

//only some Routes are viable to be transporting Routes. These are the Routes that: 1. are not related to any sight, 2. that are not circles of equal altitude 3. do not coincide with the object to transport -> I count how many Routes are available for transport -> If there is at least one, return true, otherwise return false
bool ListFrame::CheckRoutesForTransport(void) {
    
    unsigned int n_routes_for_transport;
    int i;
    
    for(i = 0, n_routes_for_transport=0; i < data->route_list.size(); i++){

        if(
            /*condition that the Route is not relatied to a Sight*/
            ((((data->route_list)[i]).related_sight.value) == -1) &&
            /*condition that the Route is not a circle of equal altitude*/
            (((data->route_list)[i]).type != Route_types[2]) &&
            /*condition that the Route does not coincide with the object to transport*/
            ((transported_object_type != String("route")) || (i_object_to_transport != i))
            ){
                
                n_routes_for_transport++;
    
            }

    }
    
    return((n_routes_for_transport > 0 ? true : false));
    
}



template<class E> void ListFrame::OnPressDeleteRoute(E& event) {

    //ask the user whether he/she really wants to remove the Route: if the answer is yes, then QuestionFrame calls the functor ask_remove_related_sight. If no, I call the functor unsed_idling, which does nothing and simply sets idling to false
    QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>* question_frame = new QuestionFrame<AskRemoveRelatedSight, UnsetIdling<ListFrame>, UnsetIdling<ListFrame>>(NULL,
        ask_remove_related_sight, String("Yes"), unset_idling, String("No"), unset_idling, true, true, true,
        "",
        "Do you really want to remove this route?",
        wxGetApp().path_file_question_icon,
        wxDefaultPosition,
        wxDefaultSize,
        String(""));

    question_frame->Show(true);

    event.Skip(true);

}

//disconnects sight i_sight from its related Route
template<class E> void ListFrame::Disconnect(E& event) {

    int i_route;

    i_route = (((data->sight_list)[i_object_to_disconnect]).related_route).value;

    //disconnect route and sight
    (((data->sight_list)[i_object_to_disconnect]).related_route).set(String(""), -1, String(""));
    (((data->route_list)[i_route]).related_sight).set(String(""), -1, String(""));

    //update the related wxListCtrls in ListFrame
    ((data->sight_list)[i_object_to_disconnect]).update_wxListCtrl(i_object_to_disconnect, listcontrol_sights);
    ((data->route_list)[i_route]).update_wxListCtrl(i_route, listcontrol_routes);

    //set the background color of the related sight to white
    (listcontrol_sights)->SetItemBackgroundColour(i_object_to_disconnect, wxGetApp().background_color);

    //if an item is selected in listcontrol_sights, enable /disable button_transport_sight and button_disconnect_sight if the selected sight is related / unrelated to a Route
    if ((listcontrol_sights->GetSelectedItemCount()) != 0) {

        bool enable;

        enable = ((((data->sight_list)[listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)]).related_route).value != -1);

        button_transport_sight->Enable(enable);
        button_disconnect_sight->Enable(enable);

    }

    //the Route has been disconnected from the sight -> a new Route which is not connected to any Sight is created -> the data of the file has been modified
    OnModifyFile();

    event.Skip(true);

}


//disconnects sight i_sight from its related Route and prompt a message frame to inform the user
template<class E> void ListFrame::DisconnectAndPromptMessage(E& event) {

    Disconnect<E>(event);

    //print an info message
    print_info_message->SetAndCall(NULL, String("Warning"), String("The route which is being dragged was related to a sight! Disconnecting the route from the sight."), (wxGetApp().path_file_info_icon));

 
    event.Skip(true);

}


//when the mouse hovers over a given element of listcontrol_routes, sets highlighted_route_now equal to the id of that route, and the same for the relaetd sight in listcontrol_sights.
void ListFrame::OnMouseMovement(wxMouseEvent& event) {

    int i, j;

    //	            cout << "Position of mouse screen = {" << wxGetMousePosition().x << " , " << wxGetMousePosition().y << "}\n";

    //save the id of the  Sight Route and Position highlighted at the preceeding step into highlighted_route_before
    highlighted_route_before = highlighted_route_now;
    highlighted_position_before = highlighted_position_now;


    //check whether the mouse is hovering over an element of listcontrol_routes / listcontrol_sights
    MousePositionOnListControl(listcontrol_sights, &highlighted_sight_now);
    MousePositionOnListControl(listcontrol_positions, &highlighted_position_now);
    MousePositionOnListControl(listcontrol_routes, &highlighted_route_now);

    if ((highlighted_sight_now == wxNOT_FOUND) && (highlighted_position_now == wxNOT_FOUND) && (highlighted_route_now == wxNOT_FOUND)) {
        //the mouse is not hovering over an element in listcontrol_sights nor listcontrol_routes: set a white background in all elements in listonctrol_routes and listcontrol_sights

        //set the beckgorund color of the Routes in listcontrol_sights and listcontrol_routes  and the background color of the Positions in listcontrol_positions to white
        for (i = 0; i < (listcontrol_sights->GetItemCount()); i++) {
            listcontrol_sights->SetItemBackgroundColour(i, wxGetApp().background_color);
        }
        for (i = 0; i < (listcontrol_positions->GetItemCount()); i++) {
            listcontrol_positions->SetItemBackgroundColour(i, wxGetApp().background_color);
        }
        for (i = 0; i < (listcontrol_routes->GetItemCount()); i++) {
            listcontrol_routes->SetItemBackgroundColour(i, wxGetApp().background_color);
        }

    }
    else {
        //the mouse is hovering over either an element of listcontrol_sights, or an element of listcontrol_routes, or an element of listcontrol_positions

        if ((highlighted_sight_now != wxNOT_FOUND) && enable_highlight) {
            // the mouse is hovering over an element of listcontrool_sights -> highlight it and the related route in listcontrol_routes, and set  a white background in all other leements in listcontrol_sights and listcontorl_routes

            highlighted_route_now = (((data->sight_list)[highlighted_sight_now]).related_route.value);

            for (i = 0; i < (listcontrol_sights->GetItemCount()); i++) {

                if (i == highlighted_sight_now) {

                    //set the beckgorund color of the sight in listcontrol_sights and of its related route to a highlight color
                    listcontrol_sights->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                    if ((highlighted_route_now != -1) && ((listcontrol_routes->GetItemCount()) > highlighted_route_now)) {
                        listcontrol_routes->SetItemBackgroundColour(highlighted_route_now, (wxGetApp().color_selected_item));
                    }

                }
                else {

                    //set the beckgorund color of the sight in listcontrol_sights and of its related route to white
                    listcontrol_sights->SetItemBackgroundColour(i, wxGetApp().background_color);
                    if ((((((data->sight_list)[i]).related_route).value) != -1) && ((listcontrol_routes->GetItemCount()) > ((((data->sight_list)[i]).related_route).value))) {
                        listcontrol_routes->SetItemBackgroundColour(((((data->sight_list)[i]).related_route).value), wxGetApp().background_color);
                    }

                }

            }

        }


        if (highlighted_position_now != wxNOT_FOUND) {
            //the mouse is hovering over an element of listcontrool_positions -> highlight it and the related position in listcontrol_positions, and set  a white background in all other leements in listcontrol_positions

            for (i = 0; i < (listcontrol_positions->GetItemCount()); i++) {

                if (i == highlighted_position_now) {

                    //set the beckgorund color of the Position in listcontrol_positions to a highlight color
                    listcontrol_positions->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));

                }
                else {

                    //set the beckgorund color of the Route in listcontrol_routes and of its related sight to white
                    listcontrol_positions->SetItemBackgroundColour(i, wxGetApp().background_color);

                }

            }

        }

        if ((highlighted_route_now != wxNOT_FOUND) && enable_highlight) {
            //the mouse is hovering over an element of listcontrool_routes -> highlight it and the related sight in listcontrol_sights, and set  a white background in all other leements in listcontrol_routes and listcontorl_sights

            j = ((((data->route_list)[highlighted_route_now]).related_sight).value);

            for (i = 0; i < (listcontrol_routes->GetItemCount()); i++) {

                if (i == highlighted_route_now) {

                    //set the beckgorund color of the Route in listcontrol_routes and of its related sight to a highlight color
                    listcontrol_routes->SetItemBackgroundColour(i, (wxGetApp().color_selected_item));
                    if ((j != -1) && ((listcontrol_sights->GetItemCount()) > j)) {
                        listcontrol_sights->SetItemBackgroundColour(j, (wxGetApp().color_selected_item));
                    }

                }
                else {

                    //set the beckgorund color of the Route in listcontrol_routes and of its related sight to white
                    listcontrol_routes->SetItemBackgroundColour(i, wxGetApp().background_color);
                    if ((((((data->route_list)[i]).related_sight).value) != -1) && ((listcontrol_sights->GetItemCount()) > ((((data->route_list)[i]).related_sight).value))) {
                        listcontrol_sights->SetItemBackgroundColour(((((data->route_list)[i]).related_sight).value), wxGetApp().background_color);
                    }

                }

            }

        }

    }

    if ((highlighted_route_before != highlighted_route_now) || (highlighted_position_before != highlighted_position_now)) {
        //the highlighted Sight, or Route or Position has changed -> re-render the charts 

        changing_highlighted_object = true;

        MyRefreshAll();

        changing_highlighted_object = false;

    }


    event.Skip(true);

}

//this function is called every time the file is changed to mark that it is a modified file
void ListFrame::OnModifyFile(void) {

    if (!file_has_been_modified) {
        //file is being modified for the first time -> add a mark to the label of *this
        SetLabel((String(GetLabel().ToStdString()).append(String(" [modified]"))).value);
    }

    //file has been modified
    file_has_been_modified = true;

}



//this function is called every time the file is saved to mark that it is no longer a modified file
void ListFrame::OnSaveFile(void) {

    //set back the label of *this to the filename
    SetLabel(wxString((data_file.name).value));

    file_is_untitled = false;
    file_has_been_modified = false;

}



template<class E> void ListFrame::OnPressCtrlO(E& event) {

    wxFileDialog openFileDialog(this, _("Open"), (wxGetApp().default_open_directory).value, "", "nav files (*.nav)|*.nav", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if ((openFileDialog.ShowModal()) != wxID_CANCEL) {
        // proceed loading the file chosen by the user;

        wxFileInputStream input_stream(openFileDialog.GetPath());

        if (!input_stream.IsOk()) {
            //file could not be opened

            wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());

        }
        else {
            //file could be opened

            data_file.set_name(String((openFileDialog.GetPath()).ToStdString()));
            data->read_from_file_to(String("Data"), String((openFileDialog.GetPath()).ToStdString()), String("RW"), String(""));

            file_is_untitled = false;

            //emable the menu item to close data_file
            menu_file->Enable(wxID_HIGHEST + 7, true);
            //load the data in data into the GUI fields of *this
            set();
            //change the title of *this to the filename
            SetLabel(wxString((data_file.name).value));
            //resize and draw all charts according to the newly loaded data
            Resize();
            PreRenderAll();

        }


    }

    //the file has not been touched yet, thus
    file_has_been_modified = false;

    event.Skip(true);

}

template<class E> void ListFrame::OnPressCtrlW([[maybe_unused]] E& event) {

    ResetListFrame* reset_list_frame;

    reset_list_frame = new ResetListFrame(this);

    if (file_has_been_modified) {
        //the user wants to close a file that has been modified -> ask the user whethere he/she wants to save it before closing it

        SaveAndReset<ListFrame>* save_and_reset;

        ShowQuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>* print_question;

        save_and_reset = new SaveAndReset<ListFrame>(this);
        print_question = new ShowQuestionFrame<ListFrame, SaveAndReset<ListFrame>, ResetListFrame, ResetListFrame>(this, save_and_reset, reset_list_frame, reset_list_frame);

        print_question->SetAndCall(NULL, String("Warning"), String("You pressed Ctrl+W. You are about to close a file that has been modified. Do you want to save changes?"), String("Yes"), String("No"));

    }
    else {
        //the user wants to close a file that has not been modified -> close it

        wxCommandEvent dummy;

        (*reset_list_frame)(dummy);

    }


}

//write content of data into file
template<class E> void ListFrame::OnPressCtrlS(E& event) {

    data_file.open(String("out"), String(""));
    data->print(false, String(""), *(data_file.value));
    data_file.close(String(""));

    OnSaveFile();

    event.Skip(true);

}


//write content of data into a named file
template<class E> void ListFrame::OnPressCtrlShiftS(E& event) {

    wxFileDialog openFileDialog(this, _(""), (wxGetApp().default_open_directory).value, "", "nav files (*.nav)|*.nav", wxFD_SAVE);

    if ((openFileDialog.ShowModal()) != wxID_CANCEL) {
        // the user did not presse cancel -> proceed saving on the file chosen by the user;

        data_file.set_name(String((openFileDialog.GetPath()).ToStdString()));
        //open a new file to save content on it
        data_file.open(String("out"), String(""));
        //writ edata into data_file
        data->print(false, String(""), *(data_file.value));

        //        //Here I write only the recent projections of the zero-th ChartFrame
        //        if(chart_frames.size() != 0){
        //            String((chart_frames[0])->projection->name->GetValue().ToStdString()).print(String("Recent projections"), false, String("\t"), *(data_file.value));
        //        }
        //
                //close the file
        data_file.close(String(""));

        OnSaveFile();

    }


    event.Skip(true);

}



template<class E> void ListFrame::KeyDown(E& event) {

    if (((event.GetKeyCode()) == WXK_DELETE) || ((event.GetKeyCode()) == WXK_BACK)) {
        // the user pressed delete or backspace


        if ((listcontrol_sights->HasFocus()) && ((listcontrol_sights->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)) {
            //an item was selected in listcontrol_sights while listcontrol_sights has focus -> call OnPressDeleteSight

            OnPressDeleteSight(event);

        }

        if ((listcontrol_positions->HasFocus()) && ((listcontrol_positions->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)) {
            //an item was selected in listcontrol_positions while listcontrol_positions has focus -> call OnPressDeleteposition

            OnPressDeletePosition(event);

        }

        if ((listcontrol_routes->HasFocus()) && ((listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)) {
            //an item was selected in listcontrol_routes  while listcontrol_routes has focus-> call OnPressDeleteroute

            OnPressDeleteRoute(event);

        }


    }

    if ((((event.GetKeyCode()) == WXK_RETURN) || ((event.GetKeyCode()) == WXK_NUMPAD_ENTER))) {
        //the user pressed enter

        if (selecting_route_for_position) {

            long previous_item;

            previous_item = -1;
            (data->crossing_route_list).clear();
            do {

                previous_item = (listcontrol_routes->GetNextItem(previous_item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
                if (previous_item != -1) { (data->crossing_route_list).push_back(crossing_route_list_temp[previous_item]); }

            } while (previous_item != -1);

            OnComputePosition();


        }

        //        if(transporting_with_selected_route && ((listcontrol_routes->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1)){
        //            //the user is transporting an object with an existing Route and he/she selected that Route in listcontrol_routes -> proceed with the transport
        //            
        //           (*on_select_route_in_listcontrol_routes_for_transport)(event);
        //            
        //        }

    }

    event.Skip(true);

}



//write into all the non-GUI objects the values of the GUI fields
template<class T> void SightFrame::get(T& event) {

    body->get(event);
    limb->get(event);
    artificial_horizon_check->get(event);
    H_s->get(event);
    index_error->get(event);

    if (!((artificial_horizon_check->checkbox)->GetValue())) {
        height_of_eye->get(event);
    }

    master_clock_date->get(event);
    master_clock_chrono->get(event);

    //while setting the non-GUI fields equal to the values in the GUI fields, I set the value of sight->time: I firs set time to master_clock_date_and_hour ...
    (sight->time) = (sight->master_clock_date_and_hour);

    stopwatch_check->get(event);

    //.. then I add to it sight->stopwatch, if any ....
    if ((sight->use_stopwatch) == Answer('y', String(""))) {
        (sight->time) += (sight->stopwatch);
    }


    if (((stopwatch_check->checkbox)->GetValue())) {
        stopwatch_reading->get(event);
    }

    TAI_minus_UTC->get(event);

    //... then I add to it TAI_minus_UTC, to convert it from the UTC to the TAI scale.
    (sight->time) += (sight->TAI_minus_UTC);


    label->get(event);

    event.Skip(true);

}

//set all the GUI fields in this equal to those in the non-GUI object this->sight
void SightFrame::set(void) {

    //    Time temp;

    body->set();

    if (((body->name)->GetValue() == wxString("sun") || (body->name)->GetValue() == wxString("moon"))) {
        //if  body is sun or moon, then I write the value in the non-GUI field Limb into the GUI LimbField

        limb->Enable(true);
        limb->set();

    }
    else {
        //if  body is not sun or moon, then I set the limb GUI field to empty and disable it

        long i;

        for (i = 0; i < 3; i++) { limb->name->Check(((unsigned int)i), false); }
        limb->checked_items.Clear();

        limb->Enable(false);
    }


    artificial_horizon_check->set();
    H_s->set();
    index_error->set();

    if (!((artificial_horizon_check->checkbox)->GetValue())) {
        height_of_eye->Enable(true);
        height_of_eye->set();
    }
    else {
        height_of_eye->Enable(false);
    }

    //(sight->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    //    temp = (sight->master_clock_date_and_hour);
    //    temp += (sight->TAI_minus_UTC);


    master_clock_date->set((sight->master_clock_date_and_hour).date);
    master_clock_chrono->set((sight->master_clock_date_and_hour).chrono);

    stopwatch_check->set();

    if (((stopwatch_check->checkbox)->GetValue())) {
        stopwatch_reading->Enable(true);
        stopwatch_reading->set(sight->stopwatch);
    }
    else {
        stopwatch_reading->Enable(false);
    }

    TAI_minus_UTC->set(sight->TAI_minus_UTC);
    label->set();

}




//write the content in the GUI fields into the non=GUI fields, and check whether all the fields in SightFrame are ok and whether the time of sight lies within the ephemerides' time span:
bool SightFrame::is_ok(void) {

    bool time_interval_ok_before;
    
    wxCommandEvent dummy;

    get(dummy);

    //becore time_inverval_ok is recomputed, store its value into time_interval_ok_before
    time_interval_ok_before = time_interval_ok;

    
    //runs TimeIntervalOk to compute time_interval_ok, which will be used to determine whether button_reduce is enabled or not
    TimeIntervalOk(String(""));
    
    if((!time_interval_ok) && time_interval_ok_before){
        //the time interval lies outside the interval of ephemerides' data and it was ok before -> prompt an error message 
        
        print_error_message->SetAndCall(NULL, String("Error"), String("Time not included in ephemerides' data!"), (wxGetApp().path_file_error_icon));

    }
    
    return(
        (body->is_ok()) &&
        ((!(((body->name->GetValue()) == wxString("sun")) || ((body->name->GetValue()) == wxString("moon")))) || (limb->is_ok())) &&
        (H_s->is_ok()) &&
        (index_error->is_ok()) &&
        ((((artificial_horizon_check->checkbox)->GetValue())) || (height_of_eye->is_ok())) &&
        (master_clock_date->is_ok()) &&
        (master_clock_chrono->is_ok()) &&
        ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) &&
        (TAI_minus_UTC->is_ok()) && time_interval_ok);


}

//calls CheckAllOk: if it returns true/false, it enables/disables the button_reduce
void SightFrame::AllOk(void) {

    button_reduce->Enable(is_ok());

}

//compute time_interval_ok
void SightFrame::TimeIntervalOk([[maybe_unused]] String prefix) {

    if (
        (master_clock_date->is_ok()) &&
        (master_clock_chrono->is_ok()) &&
        ((!((stopwatch_check->checkbox)->GetValue())) || (stopwatch_reading->is_ok())) &&
        (TAI_minus_UTC->is_ok())
        )
    {
        //the fields that specify the time of the sight are all ok ...

        //        wxCommandEvent dummy;
        //
        //        master_clock_date->get(dummy);
        //        master_clock_chrono->get(dummy);
        //        stopwatch_check->get(dummy);
        //        if(((stopwatch_check->checkbox)->GetValue())){
        //            stopwatch_reading->get(dummy);
        //        }
        //        TAI_minus_UTC->get(dummy);

        //... compute if sight->time lies within the ephemerids' data time interval
        time_interval_ok = (sight->check_time_interval(String("")));


    }
    else {

        time_interval_ok = false;

    }


}


void SightFrame::OnPressCancel([[maybe_unused]] wxCommandEvent& event) {

    //I am about to close the frame,  thus I set parent->idling to true
    (*(parent->unset_idling))();

    Close(TRUE);

}


template<class P> CheckYear<P>::CheckYear(DateField<P>* p_in) {

    p = p_in;
    tabulate_days = new TabulateDays<P>(p);

}

template<class P> CheckMonth<P>::CheckMonth(DateField<P>* p_in) {

    p = p_in;
    tabulate_days = new TabulateDays<P>(p);

}

template<class P> CheckDay<P>::CheckDay(DateField<P>* p_in) {

    p = p_in;

}


template<class P> template<class T> void CheckYear<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->year->GetValue()).ToStdString(), NULL, false, 0, 0);

        if (check || (((p->year->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->year->GetValue()).ToStdString())) == String("")))) {
            //p->year either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set year_ok to true (false)
            (p->year_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in year
            p->year->SetForegroundColour(wxGetApp().foreground_color);
            p->year->SetFont(wxGetApp().default_font);

            if (check && (p->month_ok)) {

                (*tabulate_days)(event);

            }

            p->day->Enable(check && (p->month_ok));


        }
        else {

            f->print_error_message->SetAndCall((p->year), String("Entered value is not valid!"), String("Year must be an unsigned integer"), (wxGetApp().path_file_error_icon));

            (p->year_ok) = false;
            p->day->Enable(false);

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> template<class T> void CheckMonth<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int((p->month->GetValue()).ToStdString(), NULL, true, 1, 12 + 1);

        if (check || (((p->month->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->month->GetValue()).ToStdString())) == String("")))) {
            //p->month either contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            //if check is true (false) -> set month_ok to true (false)
            (p->month_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in month
            p->month->SetForegroundColour(wxGetApp().foreground_color);
            p->month->SetFont(wxGetApp().default_font);

            if (check && (p->year_ok)) {

                (*tabulate_days)(event);

            }

            p->day->Enable(check && (p->year_ok));

        }
        else {

            f->print_error_message->SetAndCall((p->month), String("Entered value is not valid!"), String("Month must be an unsigned integer >= 1 and <= 12"), (wxGetApp().path_file_error_icon));

            (p->month_ok) = false;
            p->day->Enable(false);

        }


        f->AllOk();

    }

    event.Skip(true);

}


template<class P> template<class T> void CheckDay<P>::operator()(T& event) {

    SightFrame* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        //this variable = true if the day field is formatted correctly
        bool check;

        p->date->check_leap_year();

        if (p->date->Y_is_leap_year) {

            check = check_unsigned_int((p->day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi(p->month->GetValue())) - 1] + 1);

        }
        else {

            check = check_unsigned_int((p->day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi(p->month->GetValue())) - 1] + 1);

        }


        if (check || (((p->day->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((p->day->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set day_ok to true (false)
            (p->day_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in day
            p->day->SetForegroundColour(wxGetApp().foreground_color);
            p->day->SetFont(wxGetApp().default_font);


        }
        else {

            f->print_error_message->SetAndCall((p->day), String("Entered value is not valid!"), String("Day must be an unsigned integer comprised between the days of the relative month"), (wxGetApp().path_file_error_icon));

            (p->day_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}



template<class P> TabulateDays<P>::TabulateDays(DateField<P>* p_in) {

    p = p_in;

}



template<class P> template<class T> void TabulateDays<P>::operator()(T& event) {

    unsigned int i;
    SightFrame* f = (p->parent_frame);

    if ((p->year_ok) && (p->month_ok)) {

        wxString temp;

        //save the old value of p->day into temp
        temp = (p->day->GetValue());

        //read the year
        ((((f->sight)->master_clock_date_and_hour).date).Y) = ((unsigned int)wxAtoi(p->year->GetValue()));
        (((f->sight)->master_clock_date_and_hour).date).check_leap_year();

        //read the month
        ((((f->sight)->master_clock_date_and_hour).date).M) = ((unsigned int)wxAtoi(p->month->GetValue()));

        if (((f->sight)->master_clock_date_and_hour).date.Y_is_leap_year) {
            //in this case the year is a leap year: I fill the list of days from days_per_month_leap

            for ((p->days).Clear(), i = 0; i < days_per_month_leap[(((f->sight)->master_clock_date_and_hour).date.M) - 1]; i++) {
                (p->days).Add(wxString::Format(wxT("%i"), i + 1));
            }

        }
        else {
            //in this case the year is a common year: I fill the list of days from days_per_month_common

            for ((p->days).Clear(), i = 0; i < days_per_month_common[(((f->sight)->master_clock_date_and_hour).date.M) - 1]; i++) {
                (p->days).Add(wxString::Format(wxT("%i"), i + 1));
            }
            //
        }

        p->day->Set(p->days);

        if (!(wxAtoi(temp) <= wxAtoi((p->days)[(p->days).GetCount() - 1]))) {
            //if the value in p->day is does not lie between the boundaries of the newly set days list (list of days of the month, then I reset it by setting it to 1

            p->day->SetValue(wxString("1"));

        }
        else {

            p->day->SetValue(temp);

        }

        p->day->Enable(true);

    }
    else {

        p->day->Enable(false);

    }

    event.Skip(true);

}

template<class P, class T>  CheckCheck<P, T>::CheckCheck(CheckField<P, T>* p_in) {

    p = p_in;

}

//this function reads the value in the GUI box checkbox, and enables/disables the related_field accordingly
template<class P, class T> template<class R> void CheckCheck<P, T>::operator()(R& event) {

    //I enable/disable related_field according to whether checkbox is checked or not, and according to the value of direct_reverse
    if (((p->checkbox->GetValue()) ^ (!(p->direct_reverse)))) {
        
        p->related_field->Enable(true);
        
    }else {
        
        p->related_field->Enable(false);
        
    }

    (*(p->related_field->check))(event);

    p->parent->AllOk();

    event.Skip(true);

}


//this function writes into the non-GUI field answer the value entered in the GUI box
template<class P, class T> template<class S> void CheckField<P, T>::get(S& event) {

    //I set p->answer to the value entered in the GUI checkbox
    if (checkbox->GetValue()) {
        (answer->value) = 'y';
    }
    else {
        (answer->value) = 'n';
    }

    event.Skip(true);

}

template<class P> CheckHour<P>::CheckHour(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckHour<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int(((p->hour)->GetValue()).ToStdString(), NULL, true, 0, 24);


        if (check || ((((p->hour)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->hour)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set hour_ok to true (false)
            (p->hour_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            (p->hour)->SetForegroundColour(wxGetApp().foreground_color);
            (p->hour)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->hour), String("Entered value is not valid!"), String("Hours must be unsigned integer numbers >= 0 and < 24"), (wxGetApp().path_file_error_icon));

            (p->hour_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckMinute<P>::CheckMinute(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P>  template<class T> void CheckMinute<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_unsigned_int(((p->minute)->GetValue()).ToStdString(), NULL, true, 0, 60);

        if (check || ((((p->minute)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->minute)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set minute_ok to true (false)
            (p->minute_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in minute
            (p->minute)->SetForegroundColour(wxGetApp().foreground_color);
            (p->minute)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->minute), String("Entered value is not valid!"), String("Minutes must be unsigned integer numbers >= 0 and < 60"), (wxGetApp().path_file_error_icon));

            (p->minute_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}

template<class P> CheckSecond<P>::CheckSecond(ChronoField<P>* p_in) {

    p = p_in;

}

template<class P> template<class T> void CheckSecond<P>::operator()(T& event) {

    P* f = (p->parent_frame);

    //I proceed only if the progam is not is indling mode
    if (!(f->idling)) {

        bool check;

        check = check_double(((p->second)->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

        if (check || ((((p->second)->GetForegroundColour()) != (wxGetApp().error_color)) && (String((((p->second)->GetValue()).ToStdString())) == String("")))) {

            //if check is true (false) -> set second_ok to true (false)
            (p->second_ok) = check;
            //the background color is set to white, because in this case there is no erroneous value in deg
            (p->second)->SetForegroundColour(wxGetApp().foreground_color);
            (p->second)->SetFont(wxGetApp().default_font);

        }
        else {

            f->print_error_message->SetAndCall((p->second), String("Entered value is not valid!"), String("Seconds must be floating-point numbers >= 0.0 and < 60.0"), (wxGetApp().path_file_error_icon));

            (p->second_ok) = false;

        }

        f->AllOk();

    }

    event.Skip(true);

}


template<class P> CheckChrono<P>::CheckChrono(ChronoField<P>* p_in) {

    p = p_in;

    check_hour = new CheckHour<P>(p);
    check_minute = new CheckMinute<P>(p);
    check_second = new CheckSecond<P>(p);

}

//this function writes into sight.stopwatch the value written into the respective GUI box
template<class P> template <class T> void CheckChrono<P>::operator()(T& event) {

    (*check_hour)(event);
    (*check_minute)(event);
    (*check_second)(event);

    event.Skip(true);

}

template<class P> CheckRouteType<P>::CheckRouteType(RouteTypeField<P>* p_in) {

    p = p_in;


}

//this functor checks the wxComboBox containing the Route type, and if it is equal to loxodrome or orthodrome, it enables only  the length, Z and start fields in RouteFrame (the latter if for_transport = false, becasue I don't need a start position if I am using the Route for transport). If it is equal to circle of equal altitude, it enables only the GP and omege fields.
template<class P> template<class T> void CheckRouteType<P>::operator()(T& event) {

    P* f = (p->parent);
    bool enable;
    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field

    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode

        unsigned int i;
        bool check;
      
        //I check whether the name in the GUI field body matches one of the route types  in catalog
        p->CheckInCatalog(&check, &i);

        if (check) {
            //the Route type is valid

            //enable/disable the related fields in RouteFrame f
            enable = ((((p->catalog)[i]) == wxString(((Route_types[0]).value))) || (((p->catalog)[i]) == wxString(((Route_types[1]).value))));

            (f->Z)->Enable(enable);

            (f->start_phi)->Enable(!(f->for_transport));
            (f->start_lambda)->Enable(!(f->for_transport));

            (f->GP_phi)->Enable(!enable);
            (f->GP_lambda)->Enable(!enable);
            (f->omega)->Enable(!enable);

        }else{
            //the Route type is not valid

            (f->Z)->Enable(false);
            (f->start_phi)->Enable(false);
            (f->start_lambda)->Enable(false);


            (f->GP_phi)->Enable(false);
            (f->GP_lambda)->Enable(false);
            (f->omega)->Enable(false);

        }

        f->OnChooseLengthFormatField();

    }
    
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}


template<class P> CheckLengthFormat<P>::CheckLengthFormat(LengthFormatField<P>* p_in) {

    p = p_in;

}



template<class P> template<class T> void CheckLengthFormat<P>::operator()(T& event) {
    
    
    P* f = (p->parent);
    bool enable;

    
    //I first do all the  operations  related to the check that are specific to the LenghtFormat field
    if (!(f->idling)) {
        //I proceed only if the progam is not is indling mode

        unsigned int i;
        bool check;
      
        //I check whether the name in the GUI field body matches one of the route types  in catalog
        p->CheckInCatalog(&check, &i);

        if (check) {
            //the length format is valid

            //enable/disable the related fields in RouteFrame f
            //if the Route type is a loxodrome or orthodrome, enable = true. Otherwise, enable = false
            enable = ((((p->catalog)[i]) == wxString(((Route_types[0]).value))) || (((p->catalog)[i]) == wxString(((Route_types[1]).value))));

            (f->Z)->Enable(enable);

            (f->start_phi)->Enable(!(f->for_transport));
            (f->start_lambda)->Enable(!(f->for_transport));

            (f->GP_phi)->Enable(!enable);
            (f->GP_lambda)->Enable(!enable);
            (f->omega)->Enable(!enable);

        }else {
            //the length format is not valid

            (f->Z)->Enable(false);
            (f->start_phi)->Enable(false);
            (f->start_lambda)->Enable(false);

            (f->GP_phi)->Enable(false);
            (f->GP_lambda)->Enable(false);
            (f->omega)->Enable(false);

        }

        f->OnChooseLengthFormatField();
        
    }
    
     
    //And then do the check operations related to a general MultipleItem field by calling the Check method of the MultipleItemField parent class
    p->Check(event);

    event.Skip(true);

}



template<class S> OnChangeSelectionInListControl<S>::OnChangeSelectionInListControl(ListControl<S>* caller_in, String type_in) {

    caller = caller_in;
    type = type_in;

}


//check whether there are some selected items in ListControl *caller, and enables/disables disableable_buttons accordingly
template<class S> template<class T>void OnChangeSelectionInListControl<S>::operator()(T& event) {
    
    long i;
    //a pointer to the ListFrame
    ListFrame* list_frame_pointer;
    
    list_frame_pointer = ((ListFrame*)(caller->GetParent()->GetParent()));
    
    for (i = 0; i < (caller->disableable_buttons).size(); i++) {
        (caller->disableable_buttons)[i]->Enable(caller->GetSelectedItemCount() != 0);
    }
    
    i = (caller->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED));
    
    if (i != -1) {
        //one item is selected
        
        switch (type.position_in_list(ListControl_types)) {
                
            case 0: {
                //type.value = "sight"->caller is listcontrol_sights
                
                //the selected Sight has/does not have a related Route -> enable/disable the buttons to disconnect and transport the selected Sight
                list_frame_pointer->button_disconnect_sight->Enable(((((list_frame_pointer->data->sight_list)[i]).related_route.value) != -1));
                list_frame_pointer->button_transport_sight->Enable(((((list_frame_pointer->data->sight_list)[i]).related_route.value) != -1));
                
                break;
                
            }
                
            case 2: {
                //type.value = "route" -> caller is listcontrol_routes
                
                //the selected Route has/does not have a related Sight -> enable/disable the button to disconnect the selected Route
                list_frame_pointer->button_disconnect_route->Enable(((((list_frame_pointer->data->route_list)[i]).related_sight.value) != -1));
                
                break;
                
            }
                
        }
        
    }
    else {
        //no item is selected -> disable the button to disconnect
        
        switch (type.position_in_list(ListControl_types)) {
                
            case 0: {
                //type.value = "sight"->caller is listcontrol_sights
                
                list_frame_pointer->button_disconnect_sight->Enable(false);
                
                break;
                
            }
                
            case 2: {
                //type.value = "route" -> caller is listcontrol_routes
                
                list_frame_pointer->button_disconnect_route->Enable(false);
                
                break;
                
            }
                
        }
        
        
    }
    
    event.Skip(true);
    
}


template<class P> OnChangeSelectionInLimbField<P>::OnChangeSelectionInLimbField(LimbField<P>* caller_in) {

    caller = caller_in;

}

//when an item is selected/deselcted in *caller, make sure that only one item stays selected in *calller
template<class P> template<class T>void OnChangeSelectionInLimbField<P>::operator()(T& event) {

    wxArrayInt temp;
    long i, j;

    temp.Clear();
    caller->name->GetCheckedItems(temp);


    if ((temp.GetCount()) <= 1) {
        //only one item is selected -> write temp into caller->selected_items

        caller->checked_items.Clear();
        (caller->checked_items) = temp;

    }
    else {
        //multiple items are selecte -> check for the newly selected item, and write it as the only element  into checked_items

        for (i = 0; i < temp.GetCount(); i++) {

            for (j = 0; (j < (caller->checked_items).GetCount()) && ((caller->checked_items.Item(j)) != (temp.Item(i))); j++) {}

            if (j == (caller->checked_items).GetCount()) {
                //the item temp.Item(i) is not present in (caller->checked_items) -> resize caller->checked_items to size one and write into its only element temp.Item(i)

                caller->checked_items.Clear();
                caller->checked_items.Add(temp.Item(i), 1);

                break;

            }
        }

    }

    //update the checked items in caller->name
    if ((caller->checked_items.GetCount()) == 0) {
        //checked_items is empty->uncheck everythig in name

        for (i = 0; i < (caller->limbs.size()); i++) {
            caller->name->Check(((unsigned int)i), false);
        }

    }
    else {
        //checked_items is not empty->check, in name, only the first element in checked_items

        for (i = 0; i < (caller->limbs.size()); i++) {
            caller->name->Check(((unsigned int)i), (i == (caller->checked_items.Item(0))));
        }

    }



    (caller->ok) = ((caller->checked_items.GetCount()) == 1);

    if (caller->ok) {

        caller->name->SetForegroundColour(wxGetApp().foreground_color);
        caller->name->SetFont(wxGetApp().default_font);

    }

    //tries to enable button_reduce
    caller->parent_frame->AllOk();

    event.Skip(true);

}


//this function writes into the non-GUI fields in chrono the value written into the respective GUI fields hour, minute and second
template<class P> template <class T> void ChronoField<P>::get(T& event) {

    if (hour_ok && (hour->IsEnabled()) && minute_ok && (minute->IsEnabled()) && second_ok && (second->IsEnabled())) {
        //I write only if hour, minute and second are ok and enabled

        double s_temp;

        (chrono->h) = ((unsigned int)wxAtoi(hour->GetValue()));
        ((chrono)->m) = ((unsigned int)wxAtoi(minute->GetValue()));
        ((second)->GetValue()).ToDouble(&s_temp);
        ((chrono)->s) = s_temp;

    }

    event.Skip(true);

}



void SightFrame::OnPressReduce(wxCommandEvent& event) {

    stringstream s;

    if (label->value->GetValue().ToStdString() == "") {
        //if the user entered no label, I set a label with the time at which Reduce has been pressed

        wxCommandEvent dummy;

        (label->set_to_current_time)(dummy);

    }


    //writes the values of the GUI fields in the non-GUI fields
    get(event);

    //    sight->print(String("sight entered via GUI"), String(""), cout);


    if (position_in_listcontrol_sights == -1) {
        //if the constructor of SightFrame has been called with sight_in = NULL, then I push back the newly allocated sight to the end of sight_list and reduce it

        ((this->parent)->data)->add_sight_and_reduce(sight, String(""));

        //add the sight and the related route to the GUI object listconstrol_sights and listcontrol_routes, respectively
        //        sight->add_to_wxListCtrl(position_in_listcontrol_sights, ((this->parent)->listcontrol_sights));
        //        ((((this->parent)->data)->route_list)[(sight->related_route).value]).add_to_wxListCtrl(position_in_listcontrol_sights, ((this->parent)->listcontrol_routes));

    }
    else {
        //if the constructor of SightFrame has been called with sight_in != NULL, then I am modifying an existing sight, and I reduce it and write the result in the related route, which already exists


        if ((sight->related_route.value) != -1) {
            //sight has a related Route -> reduce sight and write the resulting Route into such related Route

            sight->reduce(&((parent->data->route_list)[(sight->related_route).value]), String(""));

        }
        else {
            //sight has no related Route -> create a related Route and add it to listcontrol_routes

            ptrdiff_t sight_position;

            sight_position = sight - (&((parent->data->sight_list)[0]));

            (parent->data->route_list).resize((parent->data->route_list).size() + 1);
            sight->reduce(&((parent->data->route_list)[(parent->data->route_list).size() - 1]), String(""));

            //I link the Sight to the Route, and the Route to the Sight
            (sight->related_route.value) = ((int)(parent->data->route_list).size()) - 1;
            ((((parent->data->route_list)[(parent->data->route_list).size() - 1]).related_sight).value) = ((int)sight_position);


        }

    }

    parent->listcontrol_sights->set(parent->data->sight_list, false);
    //I call listcontrol_routes->set with true because I want to keep the selection in listcontrol_routes
    parent->listcontrol_routes->set(parent->data->route_list, true);

    //given that I have reset the content of listcontrol_sights and listcontrol_routes, now no items will be selected in these ListControls -> I call:
    (*(parent->on_change_selection_in_listcontrol_sights))(event);
    (*(parent->on_change_selection_in_listcontrol_routes))(event);


    (*(parent->unset_idling))();
    parent->Resize();
    parent->OnModifyFile();
    
//    parent->PreRenderAll();
    //animate the charts to bring them to the Route related to the newly reduced Sight
    parent->AnimateToObject<Route, UnsetIdling<ListFrame> >(&((parent->data->route_list)[(sight->related_route).value]), parent->unset_idling);
    
    event.Skip(true);

    Close(TRUE);

}


//constructor of a MultipleItemField object, which is into *panel_of_parent. The list of items in *this is stored into catalog_in. All items that are general enough to be common to all classes which are inherited from MultipleItemField are initialized here. Items that are specific to the inherited classes will be initialized in the inherited-class constructors.
template<class P, class NON_GUI, class CHECK> MultipleItemField<P, NON_GUI, CHECK>::MultipleItemField(wxPanel* panel_of_parent, NON_GUI* object_in, const vector<String>& catalog_in, vector<int>* recent_items_in){
    
    unsigned int i;

    //set the non-GUI object
    object = object_in;
    
    //I just created *this, thus it is not being edited 
    editing = false;
    //to be conservative, I set 
    ok = false;

    //set parent
    parent = ((P*)(panel_of_parent->GetParent()));
    //set catalog equal to catalog_in 
    for(catalog.Clear(), i=0; i<catalog_in.size(); i++){
        catalog.push_back(wxString(catalog_in[i].value));
    }
    recent_items = recent_items_in;

    items = catalog;

    name = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, items, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(name);
    Fill();
    name->SetValue(items[0]);
    //I just filled name with  a valid value, thus I store it in value_before_editing in order to start off with a valid value in value_before_editing
    value_before_editing = name->GetValue();
    AdjustWidth(name);
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call Check to check whether what has been entered in name is valid
    //    name->Bind(wxEVT_COMBOBOX, &MultipleItemField::OnEdit<wxCommandEvent>, this);
    //    name->Bind(wxEVT_KEY_UP, &MultipleItemField::OnEdit<wxKeyEvent>, this);
    //    name->Bind(wxEVT_KILL_FOCUS, *check);
    name->Bind(wxEVT_KILL_FOCUS, &MultipleItemField<P, NON_GUI, CHECK>::Check<wxFocusEvent>, this);
    name->Bind(wxEVT_COMBOBOX, &MultipleItemField<P, NON_GUI, CHECK>::Check<wxCommandEvent>, this);

    

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);

}


template<class P, class NON_GUI, class CHECK> template <typename EventTag, typename Method, typename Object> void MultipleItemField<P, NON_GUI, CHECK>::Bind(EventTag tag, Method method, Object object) {

    name->Bind(tag, method, object);

}


//set a tooltip with text text to *this
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::SetToolTip(const String& text) {

    name->SetToolTip(wxString(text.value));

}




template<class P, class NON_GUI, class CHECK> template<class T> void MultipleItemField<P, NON_GUI, CHECK>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P, class NON_GUI, class CHECK> template<class T> void MultipleItemField<P, NON_GUI, CHECK>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}


//set the value in the GUI object name equal to the value in the non-GUI NON_GUI object 'object'
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::set(void) {
        
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue((MultipleItemField<P, NON_GUI, CHECK>::object->value));

    MultipleItemField<P, NON_GUI, CHECK>::ok = true;

}


//set the value in the non-GUI object 'object' equal to the value in the GUI object name, with no abbreviations used (the value is copied as it is)
 template<class P, class NON_GUI, class CHECK> template<class E> void MultipleItemField<P, NON_GUI, CHECK>::Get(E& event) {
        
     
     if (MultipleItemField<P, NON_GUI, CHECK>::ok) {
              object->set(String(""), String((MultipleItemField<P, NON_GUI, CHECK>::name->GetValue()).ToStdString()), String(""));
      }

      event.Skip(true);

}


// if the content of the GUI field *this  matches one of the items in catalog, write true in *is_present and write the number of the corresponding entry in catalog in *i. If not, write false in *is_present and catalog.size() in *i
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::CheckInCatalog(bool* is_present, unsigned int * i) {
    

    for ((*is_present) = false, (*i) = 0; ((*i)<catalog.size()) && (!(*is_present)); (*i)++) {
        if ((name->GetValue()) == catalog[(*i)]) {
            (*is_present) = true;
        }
    }
    
    if((*is_present)){
        (*i)--;
    }else{
        (*i) = ((unsigned int)(catalog.size()));
    }
    
}


//this method is called whenever the user kills the focus on the GUI field in order to check the content of the GUI field and do the necessary operations
template<class P, class NON_GUI, class CHECK> template<class E> void MultipleItemField<P, NON_GUI, CHECK>::Check(E& event) {


    //I proceed only if the progam is not is indling mode
    if (!(parent->idling)) {

        unsigned int i;
        bool is_present;
        
        CheckInCatalog(&is_present, &i);

        if (is_present || (((name->GetForegroundColour()) != (wxGetApp().error_color)) && (String(((name->GetValue()).ToStdString())) == String("")))) {
            //the GUI field  contains a valid text, or it is empty and with a white background color, i.e., virgin -> I don't call an error message frame

            if (is_present) {
                //the content of the GUI field matches one of the items in catalog, i.e., it is valid -> I insert it into recent_items, which points to a suitable location (initialized when *this was constructed)

                //insert item #i into data->recent_bodies
                wxGetApp().list_frame->data->insert_recent_item(i, recent_items);

                //I update p->name according to the content of recent_itmes
                Fill();

            }


            //if check is true (false) -> set ok to true (false)
            ok = is_present;
            //the background color is set to wxGetApp().foreground_color and the font to default_font, because in this case there is no erroneous value in name. I call Reset to reset the font colors of the items in the list to their default values
            name->SetForegroundColour(wxGetApp().foreground_color);
            name->SetFont(wxGetApp().default_font);
            Reset(name);
            
            //if the value written in name is correct, I store it in value_before_editing
            if(ok){value_before_editing = (name->GetValue());}

        }else{
            //the GUI field  does not contain a valid text,  it is not empty and with a red background color-> I prompt an error message frame

            stringstream temp;

            temp.str("");
            temp << "Item must be one of the following: ";
            for (i = 0; i < (catalog.GetCount()); i++) {
                temp << (catalog[i]).ToStdString() << (i < (catalog.GetCount()) - 1 ? ", " : ".");
            }

            parent->print_error_message->SetAndCall(name, String("Item not found in list of suitable items!"), String(temp.str().c_str()), (wxGetApp().path_file_error_icon));

            ok = false;

        }

        if (!ok) {
            //the entered value is not valid: I set the value back to the value before the editing process had started
            name->SetValue(value_before_editing);
            ok = true;
            name->SetForegroundColour(wxGetApp().foreground_color);
            name->SetFont(wxGetApp().default_font);
            Reset(name);
        }
        parent->AllOk();

    }
    
    event.Skip(true);

}


//constructor of a ProjectionField object, based on the parent frame frame
template<class P> ProjectionField<P>::ProjectionField(
                                                      wxPanel* panel_of_parent,
                                                      vector<int>* recent_items_in) : MultipleItemField<P, Projection, void>(
                                                                                                                             panel_of_parent, 
                                                                                                                             NULL,
                                                                                                                             convert_vector<Projection, String>(Projection_types),
                                                                                                                             recent_items_in) {


    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, Projection, void>::name->Bind(wxEVT_COMBOBOX, &ProjectionField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, Projection, void>::name->Bind(wxEVT_KEY_UP, &ProjectionField::OnEdit<wxKeyEvent>, this);

}


//update the GUI dropdown menu of MultipleItemField according to MultipleItemField<P, NON_GUI>::recent_items in such a way that the recent items appear on top
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::Fill(void) {

    unsigned int i, j;
    wxArrayString items_temp;
    wxString name_temp;
    bool is_present;

    //save the current value of name in name_temp
    name_temp = (MultipleItemField<P, NON_GUI, CHECK>::name->GetValue());
    //create the temporary list of projections projections_temp from catalog
    for (items_temp.Clear(), i = 0; i < MultipleItemField<P, NON_GUI, CHECK>::items.GetCount(); i++) {
        items_temp.Add((MultipleItemField<P, NON_GUI, CHECK>::catalog)[i]);
    }

    //I first add to the items vector the recently selected items written in (MultipleItemField<P, NON_GUI>::recent_items)
    for (MultipleItemField<P, NON_GUI, CHECK>::items.Clear(), i = 0; i < (MultipleItemField<P, NON_GUI, CHECK>::recent_items->size()); i++) {

        MultipleItemField<P, NON_GUI, CHECK>::items.Add(items_temp[(*(MultipleItemField<P, NON_GUI, CHECK>::recent_items))[i]]);

    }

    //then, I fill the items vector with the remaining items
    for (i=0; i < items_temp.GetCount(); i++) {

        for (is_present = false, j = 0; (j < MultipleItemField<P, NON_GUI, CHECK>::items.GetCount()) && (!is_present); j++) {

            if (MultipleItemField<P, NON_GUI, CHECK>::items[j] == items_temp[i]) {
                is_present = true;
            }

        }

        if (!is_present) {
            MultipleItemField<P, NON_GUI, CHECK>::items.Add(items_temp[i]);
        }

    }

    MultipleItemField<P, NON_GUI, CHECK>::name->Set(MultipleItemField<P, NON_GUI, CHECK>::items);
    //because name->Set(projections clears the value of name, I set the value of name back to name_temp
    MultipleItemField<P, NON_GUI, CHECK>::name->SetValue(name_temp);
    //given that I just filled name with a valid item, I store this item in value_before_editing
    value_before_editing = name->GetValue();
    
    
    items_temp.Clear();

}

//return true(false) is *this is ok (not ok), i.e., if this->ok = true(false)
template<class P, class NON_GUI, class CHECK> bool MultipleItemField<P, NON_GUI, CHECK>::is_ok(void) {

    return(ok);

}


//this function enables/disable the LengthFormatField
template<class P, class NON_GUI, class CHECK> void MultipleItemField<P, NON_GUI, CHECK>::Enable(bool is_enabled) {

    name->Enable(is_enabled);

}



//constructor of a LengthFormatField object, based on the parent frame frame
template<class P> LengthFormatField<P>::LengthFormatField(wxPanel* panel_of_parent, LengthFormat* object_in, vector<int>* recent_items_in)  : MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >(panel_of_parent, object_in, LengthFormat_types, recent_items_in){

    
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::check = new CheckLengthFormat<P>(this);
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::check));

    
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_COMBOBOX, &LengthFormatField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->Bind(wxEVT_KEY_UP, &LengthFormatField::OnEdit<wxKeyEvent>, this);

}


//this function is called every time the user modifies the text in this->name: it checks whether the text entered so far in name is valid, if name is valid, it calls OnChooseLengthFormat to select the projection written in name
template<class P> template<class E> void LengthFormatField<P>::OnEdit(E& event) {

    String s;
    bool success;

    //I check whether the name in the GUI field body matches one of the body names in catalog
    find_and_replace_case_insensitive(MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name, MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::items, &success, NULL);

    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::ok = success;

    if (success) {

        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::name->SetFont(wxGetApp().default_font);
        //choses the length format entered in name button_reduce
        MultipleItemField<P, LengthFormat, CheckLengthFormat<P> >::parent->OnChooseLengthFormatField(event);

    }

    event.Skip(true);

}


LengthFormat::LengthFormat(void) : String() {


}


LengthFormat::LengthFormat(string input) : String(input) {


}

RouteType::RouteType(void) : String() {}

RouteType::RouteType(const String& input) : String(input.value) {}

RouteType::RouteType(const string& input) : String(input) {}



Projection::Projection(void) : String() {}


Projection::Projection(string input) : String(input) {}


LengthUnit::LengthUnit(void) : String() {}


LengthUnit::LengthUnit(const String& input) : String(input.value) {}


SpeedUnit::SpeedUnit(void) : String() {}


SpeedUnit::SpeedUnit(const String& input) : String(input.value) {}


//constructor of a BodyField object, based on panel_of_parent, which is the panel of the frame (of type P) which hosts *this
template<class P> BodyField<P>::BodyField(wxPanel* panel_of_parent, Body* p, Catalog* c, vector<int>* recent_items_in) : MultipleItemField<P, Body, CheckBody<P> >(panel_of_parent, p, c->get_names(), recent_items_in){

//    MultipleItemField<P, Body, CheckBody<P> >::parent = ((P*)(panel_of_parent->GetParent()));
    //I link the internal pointers p and c to the respective body and body catalog
//    MultipleItemField<P, Body, CheckBody<P> >::object = p;
    catalog = c;

    //sets the name of file_recent for future use
//	file_recent.set_name((wxGetApp().path_file_recent));

    MultipleItemField<P, Body, CheckBody<P> >::check = new CheckBody<P>(this);

//    MultipleItemField<P, Body, CheckBody<P> >::name = new wxComboBox(MultipleItemField<P, Body, CheckBody<P> >::parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, MultipleItemField<P, Body, CheckBody<P> >::items, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //    name->SetValue("");
    //SetColor(name);
//    fill_bodies();
//    AdjustWidth(MultipleItemField<P, Body, CheckBody<P> >::name);
    MultipleItemField<P, Body, CheckBody<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, Body, CheckBody<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, Body, CheckBody<P> >::name->Bind(wxEVT_COMBOBOX, &BodyField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, Body, CheckBody<P> >::name->Bind(wxEVT_KEY_UP, &BodyField::OnEdit<wxKeyEvent>, this);


//    MultipleItemField<P, Body, CheckBody<P> >::ok = false;

//    MultipleItemField<P, Body, CheckBody<P> >::sizer_h = new wxBoxSizer(wxHORIZONTAL);
//    MultipleItemField<P, Body, CheckBody<P> >::sizer_v = new wxBoxSizer(wxVERTICAL);

//    MultipleItemField<P, Body, CheckBody<P> >::sizer_v->Add(MultipleItemField<P, Body, CheckBody<P> >::sizer_h, 0, wxALIGN_LEFT);
//    MultipleItemField<P, Body, CheckBody<P> >::sizer_h->Add(MultipleItemField<P, Body, CheckBody<P> >::name, 0, wxALIGN_CENTER);

}




//sets the value in the GUI object name equal to the value in the non-GUI Body object body
template<class P> void BodyField<P>::set(void) {

    MultipleItemField<P, Body, CheckBody<P> >::name->SetValue(MultipleItemField<P, Body, CheckBody<P> >::object->name.value);
    MultipleItemField<P, Body, CheckBody<P> >::ok = true;

}

//sets the value in the non-GUI object body equal to the value in the GUI  object name
template<class P> template<class T> void BodyField<P>::get(T& event) {

    unsigned int i;
    bool success;

    if (MultipleItemField<P, Body, CheckBody<P> >::ok) {
        //If the GUI field's content is ok...

        //I find the position of the content of the GUI field in the list of  the body names in catalog
        for (success = false, i = 0; (i < (catalog->list).size()) && (!success); i++) {
            if (String(MultipleItemField<P, Body, CheckBody<P> >::name->GetValue().ToStdString()) == (((catalog->list)[i]).name)) {
                success = true;
            }
        }
        i--;

        //I set the value of the non-GUI object body to the value obtained from the GUI object.
        (*(MultipleItemField<P, Body, CheckBody<P> >::object)) = (catalog->list)[i];

    }

    event.Skip(true);

}

//sets the value in the GUI object name equal to the value in the non-GUI limb object limb
template<class P> void LimbField<P>::set(void) {

    checked_items.Clear();

    if ((limb->value) == 'u') {

        name->Check(0, true);
        name->Check(1, false);
        name->Check(2, false);
        checked_items.Add(0, 1);

    }

    if ((limb->value) == 'l') {

        name->Check(0, false);
        name->Check(1, false);
        name->Check(2, true);

        checked_items.Add(2, 1);

    }

    if ((limb->value) == 'c') {

        name->Check(0, false);
        name->Check(1, true);
        name->Check(2, false);

        checked_items.Add(1, 1);

    }

    ok = true;

}

//this function enables/disable the LimbField
template<class P> void LimbField<P>::Enable(bool is_enabled) {

    name->Enable(is_enabled);

}

//sets the value in the GUI object check equal to the value in the non-GUI limb object answer
template<class P, class T> void CheckField<P, T>::set(void) {

    if ((answer->value) == 'y') {
        checkbox->SetValue(true);
    }

    if ((answer->value) == 'n') {
        checkbox->SetValue(false);
    }

}

template<class P, class T> template <typename EventTag, typename Method, typename Object> void CheckField<P, T>::Bind(EventTag tag, Method method, Object object) {

    checkbox->Bind(tag, method, object);

}



//sets the value in the GUI objects deg and min equal to the value in the non-GUI Angle object angle
template <class P> void AngleField<P>::set(void) {

    unsigned int deg_temp;
    double min_temp;
    Angle angle_temp;


    if (format == String("")) {

        angle->to_deg_min(&deg_temp, &min_temp, display_precision.value);

    }
    else {
        //in this case format = +-, EW or NS

        if ((angle->value) < M_PI) {

            if (format == String("+-")) { sign->SetValue(wxString("+")); }
            if (format == String("EW")) { sign->SetValue(wxString("W")); }
            if (format == String("NS")) { sign->SetValue(wxString("N")); }

            angle->to_deg_min(&deg_temp, &min_temp, display_precision.value);

        }
        else {

            if (format == String("+-")) { sign->SetValue(wxString("-")); }
            if (format == String("EW")) { sign->SetValue(wxString("E")); }
            if (format == String("NS")) { sign->SetValue(wxString("S")); }

            (angle_temp.value) = 2.0 * M_PI - (angle->value);
            angle_temp.to_deg_min(&deg_temp, &min_temp, display_precision.value);

        }

    }

    //all the cases above must share these lines, so I put them here
    deg->SetValue(wxString::Format(wxT("%i"), deg_temp));
    min->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, min_temp));

    sign_ok = true;
    deg_ok = true;
    min_ok = true;

}


//set the value in the GUI field *this equal to the value in the non-GUI object *input
template<class P> void DynamicLengthField<P>::set(Length input) {
    
    switch (/*(unit_value.value)[0]*/ LengthField<P>::unit_value.position_in_list(LengthUnit_types)) {
            
        case 0: {
            //unit = String("nm")
            
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, (input.value)));
//            LengthField<P>::unit->name->SetValue(wxString("nm"));
            break;
            
        }
            
        case 1: {
            //unit = String("m")
            
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, /*I convert the lenght from nm to meters*/(input.value) * 1e3 * nm));
//            LengthField<P>::unit->name->SetValue(wxString("m"));
            
            break;
            
        }
            
        case 2: {
            //unit = String("ft")
            
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, /*I convert the lenght from nm to feet*/(input.value) * nm_ft));
//            LengthField<P>::unit->name->SetValue(wxString("ft"));
            
            break;
            
        }
            
    }
    
//    LengthField<P>::unit->set();

}


//set the value in the GUI object value equal to the value in the non-GUI object length
template<class P> void DynamicLengthField<P>::set(void) {

    set(*(LengthField<P>::length));

    value_ok = true;
    LengthField<P>::unit_ok = true;

}

//sets the value in the GUI objects year, month and day equal to the value in the non-GUI limb object date_in
template<class P> void DateField<P>::set(Date date_in) {

    //    Time time_UTC;
    //
    //    //((parent_frame->sight)->time) is in TAI time scale. I substact to it TAI-UTC and obtain time in UTC scale, which is the one that I want to display in the GUI field
    //    time_UTC = ((parent_frame->sight)->time);
    //    time_UTC -= ((parent_frame->sight)->TAI_minus_UTC);

    year->SetValue(wxString::Format(wxT("%i"), date_in.Y));
    month->SetValue(wxString::Format(wxT("%i"), date_in.M));
    day->SetValue(wxString::Format(wxT("%i"), date_in.D));

    year_ok = true;
    month_ok = true;
    day_ok = true;

}

//set color as the background color in all fields of *this
template<class P> void DateField<P>::SetBackgroundColor(Color color) {

    year->SetBackgroundColour(color);
    month->SetBackgroundColour(color);
    day->SetBackgroundColour(color);

}

//set the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object *chrono
template<class P> void ChronoField<P>::set(void) {

    set(*chrono);

}

//set the value in the GUI objects hour, minute and second equal to the value in the non-GUI Chrono object chrono_in
template<class P> void ChronoField<P>::set(Chrono chrono_in) {

    hour->SetValue(wxString::Format(wxT("%i"), chrono_in.h));
    minute->SetValue(wxString::Format(wxT("%i"), chrono_in.m));
    second->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, chrono_in.s));

    hour_ok = true;
    minute_ok = true;
    second_ok = true;

}

//set color as the background color in all fields of *this
template<class P> void ChronoField<P>::SetBackgroundColor(Color color) {

    hour->SetBackgroundColour(color);
    minute->SetBackgroundColour(color);
    second->SetBackgroundColour(color);

}

//sets the value in the GUI object equal to the value in the non-GUI  object string
template<class P> void RouteTypeField<P>::set(void) {
    
    switch (/*( (MultipleItemField<P, RouteType, CheckRouteType<P> >::object->value)[0])*/ (MultipleItemField<P, RouteType, CheckRouteType<P> >::object)->position_in_list(Route_types)) {
            
        case 0: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[0]).value));
            
            break;
            
        }
            
        case 1: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[1]).value));
            
            break;
            
        }
            
        case 2: {
            
            MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetValue(wxString((Route_types[2]).value));
            
            break;
            
        }
            
    }
    
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = true;
    
}

//sets the value in the GUI object value equal to the value in the non-GUI String object string
template<class P> void StringField<P>::set(void) {

    value->SetValue(wxString(string->value));

}



//constructor of a LimbField object, based on the parent frame frame
template<class P> LimbField<P>::LimbField(wxPanel* panel_of_parent, Limb* p) {

    long i;

    parent_frame = ((P*)(panel_of_parent->GetParent()));
    //I link the internal pointers p the respective Limb object
    limb = p;

    //initialize check
    check = new CheckLimb<P>(this);
    (check->p) = this;

    
    for(limbs.Clear(), i=0; i<Limb_types.size(); i++){limbs.push_back(wxString((Limb_types[i]).value));}

    name = new wxCheckListBox(parent_frame->panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, limbs, 0, wxDefaultValidator, wxString(""));
    name->Set(limbs);

    change_selection = new OnChangeSelectionInLimbField<P>(this);

    name->SetForegroundColour(parent_frame->GetForegroundColour());
    name->SetBackgroundColour(parent_frame->GetBackgroundColour());
    //SetColor(name);

    //uncheck all items and empty cecked_items accordingly
    for (i = 0; i < 3; i++) { name->Check(((unsigned int)i), false); }
    checked_items.Clear();
    ok = false;

    name->Bind(wxEVT_CHECKLISTBOX, (*check));
    //whenever an item is selected/deselected in name, I call change_selection->operator
    name->Bind(wxEVT_CHECKLISTBOX, *change_selection);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(name, 0, wxALIGN_CENTER);

}



//constructor of a CheckField object, based on the parent frame frame
template<class P, class T> CheckField<P, T>::CheckField(wxPanel* panel_of_parent, Answer* p, T* related_field_in, bool direct_reverse_in) {

    parent = ((P*)(panel_of_parent->GetParent()));
    //I link the internal pointers p and c to the respective Answer object
    answer = p;
    related_field = related_field_in;
    direct_reverse = direct_reverse_in;

    check = new CheckCheck<P, T>(this);

    checkbox = new wxCheckBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);
    //SetColor(checkbox);
    checkbox->Bind(wxEVT_CHECKBOX, (*check));

    checkbox->SetValue(false);

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(checkbox, 0, wxALIGN_CENTER);

}



//constructor of an AngleField object, based on the parent frame 'frame'. Here format_in is the format of the angle
template <class P> AngleField<P>::AngleField(wxPanel* panel_of_parent, Angle* p, String format_in) {

    unsigned int i;
    parent = ((P*)(panel_of_parent->GetParent()));
    angle = p;

    format = format_in;

    signs.Clear();
    degrees.Clear();
    if (format == String("+-")) {
        //tabulate signs
        signs.Add(wxString("+"));
        signs.Add(wxString("-"));
    }
    if (format == String("NS")) {
        signs.Add(wxString("N"));
        signs.Add(wxString("S"));
    }
    if (format == String("EW")) {
        //here W <-> + sign, E <-> - sign
        signs.Add(wxString("E"));
        signs.Add(wxString("W"));
    }

    //tabuate degrees
    if ((format == String(""))) {
        for (i = 0; i < 360; i++) {
            degrees.Add(wxString::Format(wxT("%i"), i));
        }
    }
    else {
        if (format == String("NS")) {
            for (i = 0; i <= 90; i++) {
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }
        else {
            for (i = 0; i <= 180; i++) {
                degrees.Add(wxString::Format(wxT("%i"), i));
            }
        }
    }



    //initialize check and its objects
    check = new CheckAngle<P>(this);

    //here the allocation of sign is inserted in the code in such a way that if format = "+-" the sign is allocated before deg, text_deg, min, text_min: In this way, when the user tabs through the fields in PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if (format == String("+-")) {
        sign = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    }

    deg = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, degrees, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    deg->SetInitialSize(deg->GetSizeFromTextSize(deg->GetTextExtent(wxS("000"))));
    //SetColor(deg);
    AdjustWidth(deg);
    deg->SetValue(wxString(""));
    deg_ok = false;
    deg->Bind(wxEVT_KILL_FOCUS, (*(check->check_arc_degree)));
    //as text is changed in deg from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    deg->Bind(wxEVT_COMBOBOX, &AngleField::OnEditArcDegree<wxCommandEvent>, this);
    deg->Bind(wxEVT_KEY_UP, &AngleField::OnEditArcDegree<wxKeyEvent>, this);


    text_deg = new StaticText((parent->panel), wxString(wxGetApp().degree_symbol.append(String(" ")).value), wxDefaultPosition, wxDefaultSize, 0);

    min = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    min->SetInitialSize(min->GetSizeFromTextSize(min->GetTextExtent(wxS(sample_width_floating_point_field))));
    //SetColor(min);
    min->SetValue(wxString(""));
    min_ok = false;
    min->Bind(wxEVT_KILL_FOCUS, (*(check->check_arc_minute)));
    //as text is changed min by the user with the keyboard, call OnEditArcMinute
    min->Bind(wxEVT_KEY_UP, &AngleField::OnEditArcMinute<wxKeyEvent>, this);


    text_min = new StaticText((parent->panel), wxT("' "), wxDefaultPosition, wxDefaultSize, 0);

    //here the allocation of sign is inserted in the code in such a way that if format = "NS" || "EW" the sign is allocated after deg, text_deg, min, text_min: In this way, when the user tabs through the fields in the PositionFrame, the tab will go through the different fields in the correct order (in the order in which the fields appear from left to right in PositionFrame)
    if ((format == String("NS")) || (format == String("EW"))) {
        sign = new wxComboBox(parent->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, signs, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    }

    if (format != String("")) {
        //if the AngleField format is either NS, EW or +-, the sign field is used -> I set its background colour, width and bind it to check->check_sign

        //SetColor(sign);
        AdjustWidth(sign);
        sign->SetValue(wxString(""));
        sign_ok = false;
        sign->Bind(wxEVT_KILL_FOCUS, (*(check->check_sign)));
        sign->Bind(wxEVT_COMBOBOX, (*(check->check_sign)));
        //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEditSign
        sign->Bind(wxEVT_COMBOBOX, &AngleField::OnEditSign<wxCommandEvent>, this);
        sign->Bind(wxEVT_KEY_UP, &AngleField::OnEditSign<wxKeyEvent>, this);


    }



    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    if (format != String("")) {

        if (format == String("+-")) {

            //in this case I display the sign before the numerical value of the angle
            sizer_h->Add(sign, 0, wxALIGN_LEFT);

            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);

        }
        else {

            //in this case I display the sign after the numerical value of the angle
            sizer_h->Add(deg, 0, wxALIGN_CENTER);
            sizer_h->Add(text_deg);
            sizer_h->Add(min, 0, wxALIGN_CENTER);
            sizer_h->Add(text_min);

            sizer_h->Add(sign, 0, wxALIGN_LEFT);

        }
    }
    else {

        //in this case I display only the numerical value of the angle
        sizer_h->Add(deg, 0, wxALIGN_CENTER);
        sizer_h->Add(text_deg);
        sizer_h->Add(min, 0, wxALIGN_CENTER);
        sizer_h->Add(text_min);

    }

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);

}


//constructor of a LengthField object, based on the parent frame frame
template<class P> LengthField<P>::LengthField(wxPanel* panel_of_parent, Length* p, String unit_value_in){

    parent = ((P*)(panel_of_parent->GetParent()));
    length = p;
    unit_value = unit_value_in;
    
    
    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);
    
    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);

}


//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entere   d so far in unit is valid and runs AllOk
template<class P> template<class E> void LengthField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in units
    find_and_replace_case_insensitive(unit->name, unit->catalog, &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        unit->name->SetForegroundColour(wxGetApp().foreground_color);
        unit->name->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    unit_ok = success;
    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//insert *this in *host
template<class P> template<class T> void LengthField<P>::InsertIn(T* host) {

    host->Add(LengthField<P>::sizer_v);

}


//same asLengthField<P>::InsertIn(T* host) but with flags to be provided
template<class P> template<class T> void LengthField<P>::InsertIn(T* host, wxSizerFlags& flag) {

    host->Add(sizer_v, flag);

}


//constructor of a EditableLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> DynamicLengthField<P>::DynamicLengthField(wxPanel* panel_of_parent, Length* p, String unit_value_in) : LengthField<P>( panel_of_parent, p, unit_value_in) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    
    //initialize check
    check = new CheckLength<P>(this);
    
    value = new wxTextCtrl((LengthField<P>::parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Length object
    value->SetValue(wxString(""));
    value_ok = false;
    value->Bind(wxEVT_KILL_FOCUS, (*(check->check_length_value)));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    value->Bind(wxEVT_KEY_UP, &DynamicLengthField::OnEditValue<wxKeyEvent>, this);
    
    LengthField<P>::unit = new LengthUnitField<P>((LengthField<P>::parent->panel), &(LengthField<P>::length->unit), &(wxGetApp().list_frame->data->recent_length_units));
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    LengthField<P>::unit->name->Bind(wxEVT_COMBOBOX, &LengthField<P>::template OnEditUnit<wxCommandEvent>, this);
    LengthField<P>::unit->name->Bind(wxEVT_KEY_UP, &LengthField<P>::template OnEditUnit<wxKeyEvent>, this);

    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    LengthField<P>::sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    LengthField<P>::unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::template InsertIn<wxBoxSizer>(LengthField<P>::sizer_h, flags);

}


//constructor of a StaticLengthField object, based on the parent frame frame. Note that some lines in this constructor could not be moved up to the constructor of LengthField<P>
template<class P> StaticLengthField<P>::StaticLengthField(wxPanel* panel_of_parent, Length* p, String unit_value_in) : LengthField<P>( panel_of_parent, p, unit_value_in) {

    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    flags.Center();

    
    value = new StaticText((LengthField<P>::parent->panel),  "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    value->SetLabel(wxString(""));
    
    //set unit_before to an empty value
    unit_before.set(String(""));
    
    LengthField<P>::unit = new LengthUnitField<P>((LengthField<P>::parent->panel), &(LengthField<P>::length->unit), &(wxGetApp().list_frame->data->recent_length_units));
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    LengthField<P>::unit->name->Bind(wxEVT_COMBOBOX, &LengthField<P>::template OnEditUnit<wxCommandEvent>, this);
    LengthField<P>::unit->name->Bind(wxEVT_KEY_UP, &LengthField<P>::template OnEditUnit<wxKeyEvent>, this);

    
    LengthField<P>::unit->name->Bind(wxEVT_COMBOBOX, &StaticLengthField<P>:: template ConvertUnit<wxCommandEvent>, this);
    LengthField<P>::unit->name->Bind(wxEVT_KEY_UP, &StaticLengthField<P>::template ConvertUnit<wxKeyEvent>, this);
    
    //add value to sizer_h, which has been initialized by the constructor of the parent class LengthField
    LengthField<P>::sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    LengthField<P>::unit->MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >::template InsertIn<wxBoxSizer>(LengthField<P>::sizer_h, flags);

}



//set the value in the GUI field *this equal to the value in the non-GUI object *length
template<class P> void StaticLengthField<P>::set(Length input) {
    
    switch (LengthField<P>::unit_value.position_in_list(LengthUnit_types)) {
            
        case 0: {
            //unit = String("nm")
            
            value->SetLabel(wxString::Format(wxT("%.*f"), display_precision.value, input.value));
//            LengthField<P>::unit->name->SetValue(wxString("nm"));
            break;
            
        }
            
        case 1: {
            //unit = String("m")
            
            value->SetLabel(wxString::Format(wxT("%.*f"), display_precision.value, /*I convert the lenght from nm to meters*/(input.value) * 1e3 * nm));
//            LengthField<P>::unit->name->SetValue(wxString("m"));
            
            break;
            
        }
            
        case 2: {
            //unit = String("ft")
            
            value->SetLabel(wxString::Format(wxT("%.*f"), display_precision.value, /*I convert the lenght from nm to feet*/(input.value) * nm_ft));
//            LengthField<P>::unit->name->SetValue(wxString("ft"));
            
            break;
            
        }
            
    }
    
    LengthField<P>::unit->set();
    
    //store the currently set length unit into unit_before for the future
    unit_before = (LengthField<P>::length->unit);
    
}

//set the value in the GUI object value equal to the value in the non-GUI object length
template<class P> void StaticLengthField<P>::set(void) {

    set(*(LengthField<P>::length));

    LengthField<P>::unit_ok = true;

}


//convert the numerical value stored into value according to the length unit unit 
template<class P> template<class E>  void StaticLengthField<P>::ConvertUnit(E& event) {
    
    
    
    
    
    
    
    
    //store the currently set length unit into unit_before for the future
    unit_before = (LengthField<P>::length->unit);
    
    event.Skip(true);

}


//constructor of a StringField object, based on the parent frame frame
template<class P> StringField<P>::StringField(wxPanel* panel_of_parent, String* p) {

    parent = ((P*)(panel_of_parent->GetParent()));
    string = p;

    //initialize check
    check = new CheckString<P>(this);

    (set_to_current_time.p) = this;

    value = new wxTextCtrl((parent->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_string_field))));
    value->SetValue("");
    value->Bind(wxEVT_KILL_FOCUS, (*check));

    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(value, 0, wxALIGN_CENTER);

}



//checks whether the contents of the GUI fiels in AngleField are valid
template<class P> bool AngleField<P>::is_ok(void) {

    bool output;

    if (format == String("")) {
        //if the angle is in format "", the sign field is not used, so there is no need to check whether it is ok

        output = (deg_ok && min_ok);

    }
    else {
        //if the angle is in format "NS", "EW" or "+-" the sign field is  used, so I check that it is ok

        output = (sign_ok && deg_ok && min_ok);


    }

    return(output);

}

//this function is called every time a keyboard button is lifted in this->sign: it checks whether the text entered so far in this->sign is valid and runs AllOk
template<class P> template<class E> void AngleField<P>::OnEditSign(E& event) {

    bool success;

    find_and_replace_case_insensitive(sign, signs, &success, NULL);

    if (success) {

        //because the text in sign is valid, I set the background color of sign to white
        sign->SetForegroundColour(wxGetApp().foreground_color);
        sign->SetFont(wxGetApp().default_font);

    }

    //sign_ok is true/false is the text entered in sign is valid/invalid
    sign_ok = success;

    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->deg: it checks whether the text entered so far in deg is valid and runs AllOk
template<class P> template<class E> void AngleField<P>::OnEditArcDegree(E& event) {

    bool success;

    success = check_unsigned_int((deg->GetValue()).ToStdString(), NULL, true, 0, 360);

    if (success) {

        //because the text in sign is valid, I set the background color of deg to white
        deg->SetForegroundColour(wxGetApp().foreground_color);
        deg->SetFont(wxGetApp().default_font);

    }

    //sign_ok is true/false is the text entered in sign is valid/invalid
    deg_ok = success;

    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->min: it checks whether the text entered so far in min is valid and runs AllOk
template<class P> template<class E> void AngleField<P>::OnEditArcMinute(E& event) {

    bool success;

    success = check_double((min->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

    if (success) {

        //because the text in sign is valid, I set the background color of min to white
        min->SetForegroundColour(wxGetApp().foreground_color);
        min->SetFont(wxGetApp().default_font);

    }

    //min_ok is true/false is the text entered in min is valid/invalid
    min_ok = success;

    //tries to enable button_reduce
    parent->AllOk();

    event.Skip(true);

}

//bind all GUI windows in *this to method
template<class P> template <typename EventTag, typename Method, typename Object> void AngleField<P>::Bind(EventTag tag, Method method, Object object) {


    if (format != String("")) {
        //the angle format contains a non-empty sign

        //I bind the sign to method
        sign->Bind(tag, method, object);
    }

    //I bind deg and min to method
    deg->Bind(tag, method, object);
    min->Bind(tag, method, object);

}


template<class P> bool DynamicLengthField<P>::is_ok(void) {

    return(value_ok && (LengthField<P>::unit_ok));

}


template<class P> bool ChronoField<P>::is_ok(void) {

    return(hour_ok && minute_ok && second_ok);

}

//this function is called every time a keyboard button is lifted in this->hour: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditHour(E& event) {

    bool success;

    success = check_unsigned_int((hour->GetValue()).ToStdString(), NULL, true, 0, 24);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        hour->SetForegroundColour(wxGetApp().foreground_color);
        hour->SetFont(wxGetApp().default_font);

    }


    //hour_ok is true/false is the text entered is valid/invalid
    hour_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->minute: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditMinute(E& event) {

    bool success;

    success = check_unsigned_int((minute->GetValue()).ToStdString(), NULL, true, 0, 60);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        minute->SetForegroundColour(wxGetApp().foreground_color);
        minute->SetFont(wxGetApp().default_font);

    }


    //minute_ok is true/false is the text entered is valid/invalid
    minute_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->second: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E> void ChronoField<P>::OnEditSecond(E& event) {

    bool success;

    success = check_double((second->GetValue()).ToStdString(), NULL, true, 0.0, 60.0);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        second->SetForegroundColour(wxGetApp().foreground_color);
        second->SetFont(wxGetApp().default_font);

    }


    //second_ok is true/false is the text entered is valid/invalid
    second_ok = success;
    //tries to enable button_ok
    parent_frame->AllOk();

    event.Skip(true);

}



template<class P> template <typename EventTag, typename Method, typename Object> void ChronoField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind hour, minute and second to method
    hour->Bind(tag, method, object);
    minute->Bind(tag, method, object);
    second->Bind(tag, method, object);

}


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void DynamicLengthField<P>::OnEditValue(E& event) {

    bool success;

    success = check_double((value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        value->SetForegroundColour(wxGetApp().foreground_color);
        value->SetFont(wxGetApp().default_font);

    }


    //value_ok is true/false is the text entered is valid/invalid
    value_ok = success;
    //tries to enable button_reduce
    LengthField<P>::parent->AllOk();

    event.Skip(true);

}


template<class P> template <typename EventTag, typename Method, typename Object> void DynamicLengthField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    LengthField<P>::unit->Bind(tag, method, object);

}


//constructor of a SpeedField object, based on the parent frame frame
template<class P> SpeedField<P>::SpeedField(wxPanel* panel_of_parent, Speed* p, String unit_value_in) {

    parent_frame = ((P*)(panel_of_parent->GetParent()));
    speed = p;
    unit_value = unit_value_in;
    //these flags will be used in the method InsertIn below, to insert this->unit
    wxSizerFlags flags;

    
    //initialize check
    check = new CheckSpeedValue<P>(this);

    flags.Center();

    value = new wxTextCtrl((parent_frame->panel), wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(value);
    value->SetInitialSize(value->GetSizeFromTextSize(value->GetTextExtent(wxS(sample_width_floating_point_field))));
    //I set the value to an empty value and the flag ok to false, because for the time being this object is not properly linked to a Speed object
    value->SetValue(wxString(""));
    value_ok = false;
    value->Bind(wxEVT_KILL_FOCUS, (*check));
    //as text is changed in value by the user with the keyboard, call OnEditValue
    value->Bind(wxEVT_KEY_UP, &SpeedField::OnEditValue<wxKeyEvent>, this);


//    (unit->name) = new wxComboBox((parent->panel), wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, units, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    unit = new SpeedUnitField<P>(parent_frame->panel, &(speed->unit), &(wxGetApp().list_frame->data->recent_speed_units));
    
    //as text is changed in unit from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    unit->Bind(wxEVT_COMBOBOX, &SpeedField::OnEditUnit<wxCommandEvent>, this);
    unit->Bind(wxEVT_KEY_UP, &SpeedField::OnEditUnit<wxKeyEvent>, this);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    
    sizer_h->Add(value, 0, wxALIGN_CENTER | wxALL, (wxGetApp().rectangle_display.GetSize().GetWidth()) * (length_border_over_length_screen.value));
    unit->InsertIn(sizer_h, flags);
    
}


//set the value in the GUI object value equal to the value in the non-GUI object Speed
template<class P> void SpeedField<P>::set(void) {
    
    
    //    for (i = 0; (i < (unit->catalog.size())) && ((unit->name->GetValue()) != ((unit->catalog)[i])); i++) {}
    
    
    switch(String((unit->name->GetValue()).ToStdString()).position_in_list(unit->catalog)) {
            
        case 0: {
            //unit =SpeedUnit_types[0]
            
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, (speed->value)));
            
            break;
        }
            
        case 1: {
            //unit = SpeedUnit_types[1]
            
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, /*I convert the Speed from kt to km/h*/(speed->value) * nm));
            
            break;
        }
            
        case 2: {
            //unit = SpeedUnit_types[2]
            
            value->SetValue(wxString::Format(wxT("%.*f"), display_precision.value, /*I convert the Speed from kt to m/s*/(speed->value) * nm * 1e3 / 3600.0));
            
            break;
        }
            
    }
    
    //    unit->name->SetValue((unit->catalog)[i]);
    
    value_ok = true;
    unit_ok = true;
    
}


//write the value of the GUI field in SpeedField into the non-GUI field length
template<class P> template <class T> void SpeedField<P>::get(T& event) {

    if (is_ok()) {

        double speed_temp;

        value->GetValue().ToDouble(&speed_temp);

        if ((unit->name->GetValue().ToStdString()) == "kt") {
            //unit = String("nm")
            speed->set(String(""), /*the speed is entered in the GUI field is already in nm, thus no need to convert it*/speed_temp, String(""));

        }
        else {
            //[m]/[s] = [km]/1e3/[h]*3600 = [kt]/nm/1e3*3600
            if ((unit->name->GetValue().ToStdString()) == "km/h") {
                //unit = String("km/h")
                speed->set(String(""), /*the speed is entered in the GUI field in km/h, thus I convert it to kt*/speed_temp / nm, String(""));

            }
            else {

                if ((unit->name->GetValue().ToStdString()) == "m/s") {
                    //unit = String("ft")

                    speed->set(String(""), /*the speed is entered in the GUI field in m/s, thus I convert it to kt*/speed_temp / (nm * 1e3) * 3600.0, String(""));

                }

            }

        }
    }

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->value: it checks whether the text entered so far in value is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditValue(E& event) {

    bool success;

    success = check_double((value->GetValue()).ToStdString(), NULL, true, 0.0, DBL_MAX);

    if (success) {

        //because the text in value is valid, I set the background color of value to white
        value->SetForegroundColour(wxGetApp().foreground_color);
        value->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    value_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}


//this function is called every time a keyboard button is lifted in this->unit: it checks whether the text entered so far in unit is valid and runs AllOk
template<class P> template<class E>  void SpeedField<P>::OnEditUnit(E& event) {

    bool success;

    //I check whether the name in the GUI field unit matches one of the unit names in (unit->catalog)
    find_and_replace_case_insensitive(unit->name, (unit->catalog), &success, NULL);


    if (success) {

        //because the text in value is valid, I set the background color of unit to white
        unit->name->SetForegroundColour(wxGetApp().foreground_color);
        unit->name->SetFont(wxGetApp().default_font);

    }

    //value_ok is true/false is the text entered is valid/invalid
    unit_ok = success;
    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}


//enable/disable the SpeedField
template<class P> void SpeedField<P>::Enable(bool is_enabled) {

    value->Enable(is_enabled);
    unit->Enable(is_enabled);

}


template<class P> template<class T> void SpeedField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P> template <typename EventTag, typename Method, typename Object> void SpeedField<P>::Bind(EventTag tag, Method method, Object object) {

    value->Bind(tag, method, object);
    unit->Bind(tag, method, object);

}


template<class P> bool SpeedField<P>::is_ok(void) {

    return(value_ok && unit_ok);

}

//constructor of a DateField object, based on the parent frame frame
template<class P> DateField<P>::DateField(wxPanel* panel_of_parent, Date* p) {

    unsigned int i;
    parent_frame = ((P*)(panel_of_parent->GetParent()));
    date = p;


    //initialize check and its objects
    check = new CheckDate<P>(this);
    //    (check.p) = this;
    //    ((check->check_year)->p) = this;
    //    (((check->check_year)->tabulate_days).p) = this;
    //    ((check->check_month)->p) = this;
    //    (((check->check_month)->tabulate_days).p) = this;
    //    ((check->check_day)->p) = this;

    for (months.Clear(), months.Add(wxT("")), i = 0; i < 12; i++) {
        months.Add(wxString::Format(wxT("%i"), i + 1));
    }

    year = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    //SetColor(year);
    year->SetInitialSize(year->GetSizeFromTextSize(year->GetTextExtent(wxS("0000"))));
    year->SetValue(wxString(""));
    year_ok = false;
    year->Bind(wxEVT_KILL_FOCUS, *(check->check_year));
    //as text is changed year by the user with the keyboard, call OnEditYear
    year->Bind(wxEVT_KEY_UP, &DateField::OnEditYear<wxKeyEvent>, this);


    text_hyphen_1 = new StaticText((parent_frame->panel), wxT("-"), wxDefaultPosition, wxDefaultSize, 0);
    month = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, months, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(month);
    AdjustWidth(month);
    month->SetValue(wxString(""));
    month_ok = false;
    month->Bind(wxEVT_KILL_FOCUS, *(check->check_month));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    month->Bind(wxEVT_COMBOBOX, &DateField::OnEditMonth<wxCommandEvent>, this);
    month->Bind(wxEVT_KEY_UP, &DateField::OnEditMonth<wxKeyEvent>, this);

    text_hyphen_2 = new StaticText((parent_frame->panel), wxT("-"), wxDefaultPosition, wxDefaultSize, 0);

    day = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, days, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(day);
    //I create a temporary days list to set the size of the wxComboBox day with AdjustWidth, and then destroy this temporary days list
    for (days.Clear(), i = 0; i < 31; i++) {
        days.Add(wxString::Format(wxT("%i"), i + 1));
    }
    day->Set(days);
    AdjustWidth(day);
    days.Clear();
    day->SetValue(wxString(""));
    day_ok = false;
    day->Bind(wxEVT_KILL_FOCUS, *(check->check_day));
    //as text is changed in day from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEditDay
    day->Bind(wxEVT_COMBOBOX, &DateField::OnEditDay<wxCommandEvent>, this);
    day->Bind(wxEVT_KEY_UP, &DateField::OnEditDay<wxKeyEvent>, this);





    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(year, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_1, wxALIGN_CENTER);
    sizer_h->Add(month, 0, wxALIGN_CENTER);
    sizer_h->Add(text_hyphen_2, wxALIGN_CENTER);
    sizer_h->Add(day, 0, wxALIGN_CENTER);

}

//constructor of a ChronoField object, based on the parent frame frame
template<class P> ChronoField<P>::ChronoField(wxPanel* panel_of_parent, Chrono* p) {

    unsigned int i;
    parent_frame = ((P*)(panel_of_parent->GetParent()));
    chrono = p;

    check = new CheckChrono<P>(this);
    //    (check.p) = this;

    for (hours.Clear(), hours.Add(wxT("")), i = 0; i < 24; i++) {
        hours.Add(wxString::Format(wxT("%i"), i));
    }
    for (minutes.Clear(), minutes.Add(wxT("")), i = 0; i < 60; i++) {
        minutes.Add(wxString::Format(wxT("%i"), i));
    }

    hour = new wxComboBox(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, hours, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(hour);
    //    hour->SetInitialSize(hour->GetSizeFromTextSize(hour ->GetTextExtent(wxS("00"))));
    AdjustWidth(hour);
    hour->SetValue(wxString(""));
    hour_ok = false;
    hour->Bind(wxEVT_KILL_FOCUS, *(check->check_hour));
    //as text is changed in hour from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    hour->Bind(wxEVT_COMBOBOX, &ChronoField::OnEditHour<wxCommandEvent>, this);
    hour->Bind(wxEVT_KEY_UP, &ChronoField::OnEditHour<wxKeyEvent>, this);

    static_text_1 = new StaticText((parent_frame->panel), wxT("h"), wxDefaultPosition, wxDefaultSize, 0);

    minute = new wxComboBox(parent_frame->panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, minutes, wxCB_DROPDOWN | wxTE_PROCESS_ENTER);
    //SetColor(minute);
    AdjustWidth(minute);
    //    minute->SetInitialSize(minute->GetSizeFromTextSize(minute->GetTextExtent(wxS("00"))));
    minute->SetValue(wxString(""));
    minute_ok = false;
    minute->Bind(wxEVT_KILL_FOCUS, *(check->check_minute));
    //as text is changed in minute from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    minute->Bind(wxEVT_COMBOBOX, &ChronoField::OnEditMinute<wxCommandEvent>, this);
    minute->Bind(wxEVT_KEY_UP, &ChronoField::OnEditMinute<wxKeyEvent>, this);

    static_text_2 = new StaticText((parent_frame->panel), wxT("m"), wxDefaultPosition, wxDefaultSize, 0);

    second = new wxTextCtrl(parent_frame->panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    second->SetInitialSize(second->GetSizeFromTextSize(second->GetTextExtent(wxS(sample_width_floating_point_field))));
    //SetColor(second);
    second->SetValue(wxString(""));
    second_ok = false;
    second->Bind(wxEVT_KILL_FOCUS, *(check->check_second));
    //as text is changed in second from the user with either a keyboard button, call OnEditSecond
    second->Bind(wxEVT_KEY_UP, &ChronoField::OnEditSecond<wxKeyEvent>, this);
    static_text_3 = new StaticText((parent_frame->panel), wxT("s"), wxDefaultPosition, wxDefaultSize, 0);


    sizer_h = new wxBoxSizer(wxHORIZONTAL);
    sizer_v = new wxBoxSizer(wxVERTICAL);

    sizer_v->Add(sizer_h, 0, wxALIGN_LEFT);
    sizer_h->Add(hour, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_1, 0, wxALIGN_CENTER);
    sizer_h->Add(minute, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_2, 0, wxALIGN_CENTER);
    sizer_h->Add(second, 0, wxALIGN_CENTER);
    sizer_h->Add(static_text_3, 0, wxALIGN_CENTER);

}

//constructor of a RouteTypeField object, based on the parent frame frame
template<class P> RouteTypeField<P>::RouteTypeField(wxPanel* panel_of_parent, RouteType* object_in, const vector<String>& route_types_in, vector<int>* recent_items_in) : MultipleItemField<P, RouteType, CheckRouteType<P> >(panel_of_parent, object_in, route_types_in, recent_items_in) {

    MultipleItemField<P, RouteType, CheckRouteType<P> >::check = new CheckRouteType<P>(this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KILL_FOCUS, *(MultipleItemField<P, RouteType, CheckRouteType<P> >::check));
    //as text is changed in name from the user, i.e., with either a keyboard button or a selection in the listbox, call OnEdit
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_COMBOBOX, &RouteTypeField::OnEdit<wxCommandEvent>, this);
    MultipleItemField<P, RouteType, CheckRouteType<P> >::name->Bind(wxEVT_KEY_UP, &RouteTypeField::OnEdit<wxKeyEvent>, this);

}



//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> LengthUnitField<P>::LengthUnitField(wxPanel* panel_of_parent, LengthUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, LengthUnit, CheckLengthUnit<P> >(panel_of_parent, object_in, LengthUnit_types, recent_items_in) {

}
 

//constructor of a LengthUnitField object, based on the parent frame frame
template<class P> SpeedUnitField<P>::SpeedUnitField(wxPanel* panel_of_parent, SpeedUnit* object_in, vector<int>* recent_items_in) : MultipleItemField<P, SpeedUnit, void >(panel_of_parent, object_in, SpeedUnit_types, recent_items_in) {

}


//this function enables/disable all the fields in AngleField
template<class P> void AngleField<P>::Enable(bool is_enabled) {

    if (format != String("")) {
        sign->Enable(is_enabled);
    }
    deg->Enable(is_enabled);
    min->Enable(is_enabled);

}


//this function enables/disable the DynamicLengthField
template<class P> void DynamicLengthField<P>::Enable(bool is_enabled) {

    value->Enable(is_enabled);
    LengthField<P>::unit->Enable(is_enabled);

}


//this function enables/disable the whole ChronoField
template<class P> void ChronoField<P>::Enable(bool is_enabled) {

    hour->Enable(is_enabled);
    minute->Enable(is_enabled);
    second->Enable(is_enabled);

}

//template<class P> bool BodyField<P>::is_ok(void) {
//
//    return(ok);
//
//}

//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void BodyField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success;


    //I check whether the name in the GUI field body matches one of the valid body names
    find_and_replace_case_insensitive(MultipleItemField<P, Body, CheckBody<P> >::name, MultipleItemField<P, Body, CheckBody<P> >::items, &success, &i);

    if (success) {
        //the text entered in name is valid

        //I enable the limb field if and only if the selected body allows for a field and I run check on the existing text in the limb field
        MultipleItemField<P, Body, CheckBody<P> >::parent->limb->name->Enable(((MultipleItemField<P, Body, CheckBody<P> >::items)[i] == wxString("sun")) || ((MultipleItemField<P, Body, CheckBody<P> >::items)[i] == wxString("moon")));
        (*(MultipleItemField<P, Body, CheckBody<P> >::parent->limb->check))(event);

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, Body, CheckBody<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, Body, CheckBody<P> >::name->SetFont(wxGetApp().default_font);

    }
    else {
        //the text entered in name is not valid: disable parent_frame->limb and set limb->ok to false because the body related to limb is invalid

        MultipleItemField<P, Body, CheckBody<P> >::parent->limb->name->Enable(false);
        MultipleItemField<P, Body, CheckBody<P> >::parent->limb->ok = false;

    }


    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, Body, CheckBody<P> >::ok = success;
    //tries to enable button_reduce
    MultipleItemField<P, Body, CheckBody<P> >::parent->AllOk();

    event.Skip(true);

}



template<class P> bool LimbField<P>::is_ok(void) {

    return(ok);

}

template<class P> template <typename EventTag, typename Method, typename Object> void LimbField<P>::Bind(EventTag tag, Method method, Object object) {

    name->Bind(tag, method, object);

}


////this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid and runs AllOk
//template<class E> void LimbField::OnEdit(E& event){
//
//    bool check;
//
//    //I check whether the name in the GUI field name matches one of the valid limb names
//    find_and_replace_case_insensitive(name, limbs, &check, NULL);
//
//    //ok is true/false is the text enteres is valid/invalid
////    (*on_change_selection_in_limb_field)(event);
////    ok = name->;
////
//    if(check){
//
//        name->SetForegroundColour(wxGetApp().foreground_color);
//        name->SetFont(wxGetApp().default_font);
//
//        name->SetForegroundColour(wxGetApp().foreground_color);
//        name->SetFont(wxGetApp().default_font);
//
//    }
//
//    //tries to enable button_reduce
//    parent_frame->AllOk();
//
//    event.Skip(true);
//
//}

template<class P> bool DateField<P>::is_ok(void) {

    return(year_ok && month_ok && day_ok);

}

//this function is called every time a keyboard button is lifted in this->year: it checks whether the text entered so far in year is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditYear(E& event) {

    bool success;

    success = check_unsigned_int((year->GetValue()).ToStdString(), NULL, false, 0, 0);

    if (success) {

        year->SetForegroundColour(wxGetApp().foreground_color);
        year->SetFont(wxGetApp().default_font);

    }

    //year_ok is true/false is the text enteres is valid/invalid
    year_ok = success;

    (*(((this->check)->check_month)->tabulate_days))(event);

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->month: it checks whether the text entered so far in month is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditMonth(E& event) {

    bool success;

    success = check_unsigned_int((month->GetValue()).ToStdString(), NULL, true, 1, 12 + 1);

    if (success) {

        month->SetForegroundColour(wxGetApp().foreground_color);
        month->SetFont(wxGetApp().default_font);

    }

    //month_ok is true/false is the text enteres is valid/invalid
    month_ok = success;

    (*(((this->check)->check_month)->tabulate_days))(event);

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}

//this function is called every time a keyboard button is lifted in this->day: it checks whether the text entered so far in day is valid and runs AllOk
template<class P> template<class E> void DateField<P>::OnEditDay(E& event) {

    bool success;

    date->check_leap_year();

    if (date->Y_is_leap_year) {

        success = check_unsigned_int((day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_leap[(wxAtoi(month->GetValue())) - 1] + 1);

    }
    else {

        success = check_unsigned_int((day->GetValue()).ToStdString(), NULL, true, 1, days_per_month_common[(wxAtoi(month->GetValue())) - 1] + 1);

    }

    if (success) {

        day->SetForegroundColour(wxGetApp().foreground_color);
        day->SetFont(wxGetApp().default_font);

    }

    //day_ok is true/false is the text enteres is valid/invalid
    day_ok = success;

    //tries to enable button_reduce
    parent_frame->AllOk();

    event.Skip(true);

}



template<class P> template <typename EventTag, typename Method, typename Object> void DateField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind year, month and day to method
    year->Bind(tag, method, object);
    month->Bind(tag, method, object);
    day->Bind(tag, method, object);

}


//template<class P> bool RouteTypeField<P>::is_ok(void) {
//
//    return(MultipleItemField<P, RouteType, CheckRouteType<P> >::ok);
//
//}


//this function is called every time a keyboard button is lifted in this->name: it checks whether the text entered so far in name is valid, tries to enable parent_frame->limb->name and runs AllOk
template<class P> template<class E> void RouteTypeField<P>::OnEdit(E& event) {

    unsigned int i;
    bool success, enable;


    //I check whether the name in the GUI field  matches one of the entries  of catalog
    find_and_replace_case_insensitive(MultipleItemField<P, RouteType, CheckRouteType<P> >::name, MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog, &success, &i);

    if (success) {
        //the text entered in name is valid

        //enable/disable the related fields in RouteFrame f
        enable = ((((MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog)[i]) == wxString(((Route_types[0]).value))) || (((MultipleItemField<P, RouteType, CheckRouteType<P> >::catalog)[i]) == wxString(((Route_types[1]).value))));

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->Z->Enable(enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_phi->Enable(enable && (!(MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->for_transport)));
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_lambda->Enable(enable && (!(MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->for_transport)));

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_phi->Enable(!enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_lambda->Enable(!enable);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->omega->Enable(!enable);

        //because the text in name is valid, I set the background color of name to white
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::name->SetFont(wxGetApp().default_font);

    }
    else {

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->Z->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_phi->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->start_lambda->Enable(false);

        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_phi->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->GP_lambda->Enable(false);
        MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->omega->Enable(false);

    }

    //ok is true/false is the text entered is valid/invalid
    MultipleItemField<P, RouteType, CheckRouteType<P> >::ok = success;

    //try to enable time, speed and length
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->OnChooseLengthFormatField(event);

    //try to enable button_reduce
    MultipleItemField<P, RouteType, CheckRouteType<P> >::parent->AllOk();

    event.Skip(true);

}

template<class P> void RouteTypeField<P>::OnEdit(void){
    
    wxCommandEvent dummy;
    
    OnEdit(dummy);
    
    
}

template<class P> template<class T> void LimbField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

template<class P, class T> template<class R> void CheckField<P, T>::InsertIn(R* host) {

    host->Add(sizer_v);

}


template<class P> template<class T> void AngleField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P> template<class T> void DateField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}


template<class P> template<class T> void StringField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

//bind this -> value to method
template <class P> template <typename EventTag, typename Method, typename Object> void StringField<P>::Bind(EventTag tag, Method method, Object object) {

    //I bind value to method
    value->Bind(tag, method, object);

}


template<class P> template<class T> void ChronoField<P>::InsertIn(T* host) {

    host->Add(sizer_v);

}

//template<class P> template<class T> void RouteTypeField<P>::InsertIn(T* host) {
//
//    host->Add(MultipleItemField<P, RouteType, CheckRouteType<P> >::sizer_v);
//
//}


//this function is called every time the user modifies the text in this->name: it checks whether the text entered so far in name is valid, if name is valid, it calls OnChooseProjection to select the projection written in name
template<class P> template<class E> void ProjectionField<P>::OnEdit(E& event) {

    String s;
    bool success;
    
    if(!(MultipleItemField<P, Projection, void>::editing)){
        //*the user has started editing *this 
        (MultipleItemField<P, Projection, void>::editing) = true;
    }

    //I check whether the name in the GUI field body matches one of the body names in catalog
    find_and_replace_case_insensitive(MultipleItemField<P, Projection, void>::name, MultipleItemField<P, Projection, void>::items, &success, NULL);

    //ok is true/false is the text enteres is valid/invalid
    MultipleItemField<P, Projection, void>::ok = success;

    if (success) {

        MultipleItemField<P, Projection, void>::name->SetForegroundColour(wxGetApp().foreground_color);
        MultipleItemField<P, Projection, void>::name->SetFont(wxGetApp().default_font);
        //choose the projection entered in name button_reduce
//        MultipleItemField<P, Projection, void>::parent->draw_panel->OnChooseProjection(event);

    }


    event.Skip(true);

}


OnSelectRouteInListControlRoutesForTransport::OnSelectRouteInListControlRoutesForTransport(ListFrame* f_in) {

    parent = f_in;

}

OnNewRouteInListControlRoutesForTransport::OnNewRouteInListControlRoutesForTransport(ListFrame* f_in) {

    parent = f_in;

}

//construct a StaticText object from a wxStaticText object, and sets its color
StaticText::StaticText(wxWindow* parent, const wxString& label, const wxPoint& pos, const wxSize& size, long style) : wxStaticText(parent, wxID_ANY, label, pos, size, style, wxT("")) {

    //SetColor(this);

}


template<class S> ListControl<S>::ListControl(wxWindow* parent_in, vector<wxButton*> disableable_buttons_in, const wxPoint& pos, const wxSize& size, long style) : wxListCtrl(parent_in, wxID_ANY, pos, size, style) {

    disableable_buttons = disableable_buttons_in;
    header_width.resize(0);

}

//set all columns of *this: add a first dummy column, which is not correctly sized on WIN32 (I don't know why) -> add the real columns -> remove the dummy column. The  size of column i that fits the header is stored in header_width[i]
template<class S> void ListControl<S>::SetColumns(vector<wxString> headers) {

    int i;

    for (i = 0, header_width.clear(), PushBackColumn(wxString("")); i < (headers.size()); i++) {
        PushBackColumn(headers[i]);
    }

    DeleteColumn(0);
    header_width.erase(header_width.begin());

}

//push back a column to ListControl and store the header size into header_size
template<class S> void ListControl<S>::PushBackColumn(wxString name) {

    //	wxListItem column;
    //
    //	column.SetId(GetColumnCount());
    //	column.SetText(name);
    //	column.SetAlign(wxLIST_FORMAT_LEFT);
    //	column.SetWidth(((this->GetSize()).GetWidth()) / ((this->GetColumnCount()) + 1));

    InsertColumn(GetColumnCount(), name, wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);
    header_width.push_back(GetColumnWidth(GetColumnCount() - 1));

}


//deselect all items in *this
template<class S> void ListControl<S>::DeselectAll(void) {

    int i;

    for (i = 0; i < (this->GetItemCount()); i++) {

        SetItemState(i, 0, wxLIST_STATE_SELECTED);

    }

}

//clears *this and sets all its items equal to the items in the non-GUI vector v. If keep_selected_items = true, I re-select the items in *this that were selected before ListControl::set was called (if they are compatible with the new size of *this)
template<class S> void ListControl<S>::set(vector<S> v, bool keep_selected_items) {

    unsigned int i;
    vector<long> selected_items;

    //store the selected items into selected_items
    GetSelectedItems(&selected_items);

    //set *this
    DeleteAllItems();
    for (i = 0; i < v.size(); i++) {
        (v[i]).add_to_wxListCtrl(-1, this);
    }

    if (keep_selected_items) {
        //restore selected items

        for (i = 0; i < selected_items.size(); i++) {
            if (selected_items[i] < GetItemCount()) {
                SetItemState(selected_items[i], wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            }
        }

    }

}


//if check = true/false it enables/disables all disableable buttons in *this
template<class S> void ListControl<S>::EnableButtons(bool check) {

    unsigned int i;

    for (i = 0; i < disableable_buttons.size(); i++) {

        (disableable_buttons[i])->Enable(check);

    }

}

// resize the sizes of columns of *this to the maximum between the header size and the largest item size
template<class S> void ListControl<S>::Resize(vector<S> list) {

    int j, item_width;
    //a dummy listcontrol, never shown, used to set the column widths. To avoid resizing *this multiple times as the items are checked (ugly looking), I create a dummy_listcontrol whose column size is adapted to the largest item -> add all the items in list to it -> compute the item_width -> set the column width of *this as the maximum between header_width (already computed) and item_width. In this way, all columns of *this will be large enough to accomodate both its headers and items
    ListControl<S>* dummy_listcontrol;
    wxFrame* dummy_frame;

    dummy_frame = new wxFrame(NULL, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize);
    dummy_listcontrol = new ListControl<S>(dummy_frame, disableable_buttons, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

    for (j = 0; j < (this->GetColumnCount()); j++) {
        dummy_listcontrol->PushBackColumn(wxString(""));
    }

    //((ListFrame*)(GetParent()->GetParent()))->data->sight_list
    dummy_listcontrol->set(list, false);

    for (j = 0; j < (dummy_listcontrol->GetColumnCount()); j++) {
        dummy_listcontrol->SetColumnWidth(j, wxLIST_AUTOSIZE);
        item_width = (dummy_listcontrol->GetColumnWidth(j));
        SetColumnWidth(j, max(header_width[j], item_width));
    }

    dummy_listcontrol->Destroy();
    dummy_frame->Destroy();

}


//get the selected items from *this, clears and reallocate selected_items, and writes them in selected_items
template<class S> void ListControl<S>::GetSelectedItems(vector<long>* selected_items) {

    long item;

    item = -1;
    selected_items->clear();

    do {
        item = GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (item != -1) { selected_items->push_back(item); }

    } while (item != -1);


}

template<class F> MotionHandler<F>::MotionHandler(ListFrame* parent_in, const Route& transporting_route_in, F* f_in){
    

    timer = new wxTimer();

    parent = parent_in;
    transporting_route = transporting_route_in;
    f = f_in;

    t = 0;
//    timer->Bind(wxEVT_TIMER, &GraphicalObjectTransportHandler::OnTimer, this);

}


//constructor of GraphicalFeatureTransportHandler: f_in is the functor to be provided if something is supposed to be executed at the end of the transport (e.g., do another transport, show a MessageFrame, etc...). If nothing is supposed to be executed, set f_in = NULL
template<class NON_GUI, class F> GraphicalFeatureTransportHandler<NON_GUI, F>::GraphicalFeatureTransportHandler(ListFrame* parent_in, NON_GUI* object_in,  const String& type_of_transported_object_in, const Route& transporting_route_in, F* f_in) : MotionHandler<F>(parent_in, transporting_route_in, f_in){

    transported_object = object_in;
    type_of_transported_object = type_of_transported_object_in;
//    (MotionHandler<F>::transporting_route) = transporting_route_in;
//    (MotionHandler<F>::f) = f_in;
    

    (MotionHandler<F>::timer)->Bind(wxEVT_TIMER, &GraphicalFeatureTransportHandler::OnTimer, this);

}


//this method triggers the animation
template<class NON_GUI, class F> void GraphicalFeatureTransportHandler<NON_GUI, F>::operator()(void) {
    
    //the animation transport starts here
    (MotionHandler<F>::timer)->Start(
        /*animation_time is converted in milliseconds, because Start() takes its first argument in milliseconds*/
        (wxGetApp().animation_time.get()) * 60.0 * 60.0 / ((double)((wxGetApp().n_animation_steps.value) - 1)) * 1000.0,
        wxTIMER_CONTINUOUS);
    
}

//this method iterates the animation
template<class NON_GUI, class F> void GraphicalFeatureTransportHandler<NON_GUI, F>::OnTimer([[maybe_unused]] wxTimerEvent& event) {

    if(((MotionHandler<F>::t) < (wxGetApp().n_animation_steps.value))) {
        //the time parameter is undedr its maximum value

        if((MotionHandler<F>::t) == 0) {
            //I am at the beginning of the transport and *parent is not in idling mode -> proceed with the transport
            
            //set parameters back to their original value and reset listcontrol_routes to the original list of Routes
            (*((MotionHandler<F>::parent)->set_idling))();

            (MotionHandler<F>::transporting_route_temp) = (MotionHandler<F>::transporting_route);
            
            //during the transport, I disconnect DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement from mouse movements
            for (unsigned int i = 0; i < ((MotionHandler<F>::parent)->chart_frames.size()); i++) {
                (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel);
            }
            (MotionHandler<F>::parent)->listcontrol_sights->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->listcontrol_positions->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->listcontrol_routes->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
            (MotionHandler<F>::parent)->panel->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));

            

            if (type_of_transported_object == String("position")) {

                //store the starting position in geo_position_start
                (MotionHandler<F>::start) = (*((Position*)transported_object));
                //highlight the Position that is being transported
                (MotionHandler<F>::parent)->highlighted_position_now = address_position_in_vector<Position>(((Position*)transported_object), (MotionHandler<F>::parent)->data->position_list);
                
            }else {

                if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                    //store the starting reference position in geo_position_start
                    (MotionHandler<F>::start) = (((Route*)transported_object)->reference_position);
                    //highlight the Position that is being transported
                    (MotionHandler<F>::parent)->highlighted_route_now = address_position_in_vector<Route>(((Route*)transported_object), (MotionHandler<F>::parent)->data->route_list);
     

                }
                
                (MotionHandler<F>::start) = (((Route*)transported_object)->reference_position);


            }

            ((MotionHandler<F>::transporting_route_temp).reference_position) = (MotionHandler<F>::start);

            //I brind all ChartFrames to front to show the animation
            wxGetApp().ShowCharts(event);
            
            (MotionHandler<F>::t)++;

        }
        
        if((MotionHandler<F>::t) > 0){
            //the transport animation is in progress -> do the next chunk

            (MotionHandler<F>::transporting_route_temp).length.set(
                String(""),
                ((MotionHandler<F>::transporting_route).length.value) *
                (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))))
                ,
                String(""));


            if (type_of_transported_object == String("position")) {

                (*((Position*)transported_object)) = (MotionHandler<F>::start);
                ((Position*)transported_object)->transport_to((MotionHandler<F>::transporting_route_temp), String(""));

                (MotionHandler<F>::parent)->TabulatePositionsAll();
                
            }
            else {

                if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                    (((Route*)transported_object)->reference_position) = (MotionHandler<F>::start);
                    ((Route*)transported_object)->reference_position.transport_to((MotionHandler<F>::transporting_route_temp), String(""));

                }

                (MotionHandler<F>::parent)->TabulateRoutesAll();
                
            }

            (MotionHandler<F>::parent)->RefreshAll();
            //            cout << "\t\t t= " << t << "\n";
            
            (MotionHandler<F>::t)++;

        }

    }else {
        //the transport  is over

        if (type_of_transported_object == String("position")) {
            
            //do the whole transport rather than combining many little transports, to avoid rounding errors
            (*((Position*)transported_object)) = (MotionHandler<F>::start);
            //un-highlight the Position that is being transported
            (MotionHandler<F>::parent)->highlighted_position_now = -1;
            ((Position*)transported_object)->transport_to((MotionHandler<F>::transporting_route), String(""));


            //update labels
            (((Position*)transported_object)->label) = ((Position*)transported_object)->label.append(String(" transported with ")).append(((MotionHandler<F>::transporting_route).label));
            
            //update the Position information in f
            ((Position*)transported_object)->update_wxListCtrl(
                                                               address_position_in_vector<Position>(((Position*)transported_object), (MotionHandler<F>::parent)->data->position_list),
                                                               (MotionHandler<F>::parent)->listcontrol_positions
                                                               );


        }
        else {

            if ((type_of_transported_object == String("sight")) || type_of_transported_object == String("route")) {

                String new_label;
                
                //un-highlight the Route that is being transported
                (MotionHandler<F>::parent)->highlighted_route_now = -1;

                //do the whole transport rather than combining many little transports, to avoid rounding errors
                (((Route*)transported_object)->reference_position) = (MotionHandler<F>::start);
                ((Route*)transported_object)->reference_position.transport_to((MotionHandler<F>::transporting_route), String(""));


                //update labels

                //the new label which will be given to the transported Route
                new_label = ((Route*)transported_object)->label.append(String(" transported with ")).append(((MotionHandler<F>::transporting_route).label));

                //set back listcontrol_routes to route_list, in order to include all Routes (not only those which are not related to a Sight)
                (MotionHandler<F>::parent)->listcontrol_routes->set(((MotionHandler<F>::parent)->data->route_list), false);

                if ((type_of_transported_object == String("sight")) || ( ((type_of_transported_object == String("route")) && ((((Route*)transported_object)->related_sight.value) != -1)) )) {
                    //I am transporting a Sight (i.e., Route related to a Sight) or I am transporting a Route that is connected to a Sight -> disconnect the Route from the sight

                    ((MotionHandler<F>::parent)->i_object_to_disconnect) = (((Route*)transported_object)->related_sight.value);
                    (MotionHandler<F>::parent)->Disconnect(event);

                }
                
            

                //change the label of *object by appending to it 'translated with [label of the translating Route]'
                (((Route*)transported_object)->label) = new_label;

            }

        }
        
        //set parameters back to their original value and reset listcontrol_routes to the original list of Routes

        (MotionHandler<F>::parent)->listcontrol_sights->set(((MotionHandler<F>::parent)->data->sight_list), false);
        (MotionHandler<F>::parent)->listcontrol_routes->set(((MotionHandler<F>::parent)->data->route_list), false);
        (MotionHandler<F>::parent)->Resize();
        //re-draw everything
        (MotionHandler<F>::parent)->PreRenderAll();
        
        //re-bind DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement once the transport is over
        for (unsigned int i = 0; i < ((MotionHandler<F>::parent)->chart_frames.size()); i++) {
            (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, (((MotionHandler<F>::parent)->chart_frames)[i])->draw_panel);
        }
        (MotionHandler<F>::parent)->listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));
        (MotionHandler<F>::parent)->panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, (MotionHandler<F>::parent));

        
        //re-bind listcontrol_routes to &ListFrame::OnChangeSelectionInListControl
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_SELECTED, *((MotionHandler<F>::parent)->on_change_selection_in_listcontrol_routes));
        (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_DESELECTED, *((MotionHandler<F>::parent)->on_change_selection_in_listcontrol_routes));


        if (((MotionHandler<F>::parent)->transporting_with_selected_route)) {
            //I am transporting with an existing, selected Route

            //the transport is over -> I reverse the Bind/Unbind(s) made before the transport started
            ((MotionHandler<F>::parent)->transporting_with_selected_route) = false;
            (MotionHandler<F>::parent)->listcontrol_routes->Unbind(wxEVT_LIST_ITEM_ACTIVATED, *((MotionHandler<F>::parent)->on_select_route_in_listcontrol_routes_for_transport));
            (MotionHandler<F>::parent)->listcontrol_routes->Bind(wxEVT_LIST_ITEM_ACTIVATED, &ListFrame::OnModifyRoute<wxListEvent>, (MotionHandler<F>::parent));

        }

        if (((MotionHandler<F>::parent)->transporting_with_new_route)) {
            //I am tranporting with a new Route

            ((MotionHandler<F>::parent)->transporting_with_new_route) = false;

        }

        (MotionHandler<F>::timer)->Stop();
        (*((MotionHandler<F>::parent)->unset_idling))();
        
        //call the functor to be called at the end of the animation, if any
        if((MotionHandler<F>::f) != NULL){
            (*(MotionHandler<F>::f))();
        }

    }

}


//constructor of ChartTransportHandler, which initializes *this with the Route transporting_route_in (used to to the transport) and with proposed zoom factor proposed _zoom_factor at end fo the transport.  This is a `proposed` zoom factor because, if such proposed zoom factor is < 1 or > zoom_factor_max, the actual zoom factor will be set to 1 and zoom_factor_max, respectively. Othersize, the actual zoom_factor will be equal to proposed_zoom_factor.
template<class F> ChartTransportHandler<F>::ChartTransportHandler(ChartFrame* chart_in, const Route& transporting_route_in, const Double& proposed_zoom_factor, F* f_in) : MotionHandler<F>(chart_in->parent, transporting_route_in, f_in){
    
    
    chart_frame = chart_in;
    
    if(proposed_zoom_factor.value < 1.0){
        zoom_factor = 1.0;
    }else{
        if(zoom_factor > wxGetApp().zoom_factor_max){
            zoom_factor = wxGetApp().zoom_factor_max;
        }else{
            zoom_factor = proposed_zoom_factor;
        }
    }
    
    
    
    switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
            
        case 0:{
            //I am using Projection_types[0]
            
            PositionProjection temp;
            
            //compute projection_size_end from zoom_factor and rectangle_observer_0
            chart_frame->parent->rectangle_observer_0.SizeMercator(&temp);
            projection_size_end = temp / zoom_factor;

            
            break;
            
        } 
        
        case 1:{
            //I am using Projection_types[1]

            //compute omega_end from zoom_factor and circle_observer_0.omega
            omega_end.set(String(""), (chart_frame->parent->circle_observer_0.omega.value) / (zoom_factor.value), String(""));

            break;
            
        }
            
            
    }
    
    

    
    
    (MotionHandler<F>::timer)->Bind(wxEVT_TIMER, &ChartTransportHandler::OnTimer, this);

}


//prompt the movement of the center of the chart from position a to position b
template<class F> void ChartTransportHandler<F>::operator()(void) {
//void ChartTransportHandler::MoveChart(const Position& a, const Position& b){
    
    if(!((MotionHandler<F>::parent)->idling)){
        
        //the animation transport starts here (only if the parent ChartFrame is not in idling mode)
        (MotionHandler<F>::timer)->Start(
                     /*animation_time is converted in milliseconds, because Start() takes its first argument in milliseconds*/
                     (wxGetApp().animation_time.get()) * 60.0 * 60.0 / ((double)((wxGetApp().n_animation_steps.value) - 1)) * 1000.0,
                     wxTIMER_CONTINUOUS);
        
    }
    
    
}

//this method iterates the animation
template<class F> void ChartTransportHandler<F>::OnTimer([[maybe_unused]] wxTimerEvent& event) {
    
    
    if(((MotionHandler<F>::t) < (wxGetApp().n_animation_steps.value))) {
        //the time parameter is undedr its maximum value

        if((MotionHandler<F>::t) == 0) {
            //I am at the beginning of the transport and *parent is not in idling mode -> proceed with the transport
            
            //set parameters back to their original value and reset listcontrol_routes to the original list of Routes
            (*((MotionHandler<F>::parent)->set_idling))();
            (chart_frame->dragging_chart) = true;
            chart_frame->EnableAll(false);

            (MotionHandler<F>::transporting_route_temp) = (MotionHandler<F>::transporting_route);
            
            (MotionHandler<F>::start) = (MotionHandler<F>::transporting_route).reference_position;
            
  
            //during the transport, I disconnect DrawPanel::OnMouseMovement and ListFrame::OnMouseMovement from mouse movements
            chart_frame->draw_panel->Unbind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, chart_frame->draw_panel);
            chart_frame->parent->listcontrol_sights->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            chart_frame->parent->listcontrol_positions->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            chart_frame->parent->listcontrol_routes->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            chart_frame->parent->panel->Unbind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
            
            //I don't want anything to be highlighted during the chart transport, so I set 
            (chart_frame->parent->highlighted_route_now) = -1;
            (chart_frame->parent->highlighted_sight_now) = -1;
            (chart_frame->parent->highlighted_position_now) = -1;

            
            (chart_frame->draw_panel->label_position_now) = String("");

       
            switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    PositionProjection q_A, q_B;
//                    Position p_A, p_B;
                    
    
                    //write in p_NW and p_SE the two corner points of the projection and write in projection_size the size (in x,y) of the relative rectangle
                    q_A.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->phi_max));
                    q_B.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->phi_min));
                    projection_size.x = chart_frame->draw_panel->x_span();
                    projection_size.y = (q_A.y) - (q_B.y);
                    projection_size_start = projection_size;
                    
                    
                    q_A.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->parent->rectangle_observer_0.p_NW.phi));
                    q_B.NormalizeAndSetMercator(Position(Angle(0.0), chart_frame->parent->rectangle_observer_0.p_SE.phi));
                    (projection_size_end.x) = chart_frame->draw_panel->x_span_0;
                    projection_size_end.y = (q_A.y) - (q_B.y);
                    projection_size_end /= (zoom_factor.value);

                    
                   
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
     
                    
                    //the Position where I start the animation (start) may not coincide with circle_observer.reference_position (for example, I may want to start the animaiton from the antipode of circle_observer.reference_position to show a nice turn of the earth during the animaiton): thus, to start the animation, I need to first set rotation to the rotation that brings circle_observer.reference_position to be centered on start -> to do this, I do 
                    chart_frame->draw_panel->rotation.set(((chart_frame->draw_panel->rotation) * Rotation((MotionHandler<F>::start), chart_frame->draw_panel->circle_observer.reference_position)));

                    
                    chart_frame->draw_panel->rotation_start_drag.set((chart_frame->draw_panel->rotation));
                    (chart_frame->draw_panel->circle_observer.reference_position) = (MotionHandler<F>::start);
                    omega_start = chart_frame->draw_panel->circle_observer.omega;
                    
                    break;
                    
                }
                    
            }
            
//            cout << "******************* Before the transport *******************" << endl;
//            (MotionHandler<F>::transporting_route).compute_end(String(""));
//            (MotionHandler<F>::transporting_route).reference_position.print(String("Start position of transporting route"), String("\t"), cout);
//            (MotionHandler<F>::transporting_route).end.print(String("Expected arrival position"), String("\t"), cout);
//            chart_frame->draw_panel->circle_observer.reference_position.print(String("Circle observer reference position"), String("\t"), cout);


         
            
            (MotionHandler<F>::t)++;

        }
        
        if((MotionHandler<F>::t) > 0){
            //the transport animation is in progress -> do the next chunk

            (MotionHandler<F>::transporting_route_temp).length.set(
                String(""),
                ((MotionHandler<F>::transporting_route).length.value) *
                                               (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value)))))
                                               ,
                                               String(""));
            
            
            switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
                    
                case 0: {
                    //I am using Projection_types[0]
                    
                    PositionProjection q_center;
                    
                    //transport the starting point of the animation, start, according to transporting_route_temp, and store the result in p_center -> this yields the updated center of the chart
                    (MotionHandler<F>::start).transport(&p_center, (MotionHandler<F>::transporting_route_temp), String(""));
                    //transform p_center into a PositionProjection
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->GeoToProjection))(p_center, &q_center, true);
                    
                    
                    //update projection_size
                    projection_size = projection_size_start + (projection_size_end - projection_size_start) * (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))));

                    
                    //shift q_center to the NE and to the SW by projection_size/2 -> these will be the updated values of p_NE and p_SE
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(q_center + projection_size/2.0, &p_NE);
                    (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(q_center - projection_size/2.0, &p_SW);

                    //set lambda / phi min/max according to p_NE and p_SW
                    (chart_frame->lambda_max) = (p_NE.lambda);
                    (chart_frame->phi_max) = (p_NE.phi);

                    (chart_frame->lambda_min) = p_SW.lambda;
                    (chart_frame->phi_min) = p_SW.phi;
          
                    
                    break;
                    
                }
                    
                case 1: {
                    //I am using Projection_types[1]
                    
                    (MotionHandler<F>::transporting_route_temp).compute_end(String(""));
                    
                    //conpute the new rotation: the new rotation of the earth is the old one, composed with the rotation which brings the old reference_position onto the new one
                    chart_frame->draw_panel->rotation.set(((chart_frame->draw_panel->rotation_start_drag) * Rotation((MotionHandler<F>::transporting_route_temp).end, (MotionHandler<F>::start))));
                    
                    (chart_frame->draw_panel->circle_observer.reference_position) = ((MotionHandler<F>::transporting_route_temp).end);

                    chart_frame->draw_panel->circle_observer.omega = omega_start.value + (omega_end.value - omega_start.value) * (M_EULER + gsl_sf_psi_n(0, ((double)((MotionHandler<F>::t) + 1)))) / (M_EULER + gsl_sf_psi_n(0, ((double)((wxGetApp().n_animation_steps.value) + 1))));
                    
                    
                    break;
                    
                }
                    
            }
            
//            cout << "********* t = " << t << " *************" << endl;
//            (MotionHandler<F>::transporting_route_temp).compute_end(String(""));
//            (MotionHandler<F>::transporting_route_temp).end.print(String("Expected arrival point with (MotionHandler<F>::transporting_route_temp)"), String("\t\t"), cout);
//            chart_frame->draw_panel->circle_observer.reference_position.print(String("Reference position"), String("\t\t"), cout);
            
            
#ifdef WIN32
            //I am about to update coastline_polygons_now-> save the previous configuration of points_coastline into coastline_polygons_before, which will be used by RefreshWIN32()
            (chart_frame->polygon_position_before) = (chart_frame->polygon_position_now);
//            chart_frame->coastline_polygons_before.resize(chart_frame->coastline_polygons_now.size());
            copy_n(chart_frame->coastline_polygons_now.begin(), chart_frame->coastline_polygons_now.size(), chart_frame->coastline_polygons_before.begin() );

            (chart_frame->draw_panel->position_plot_area_before) = (chart_frame->draw_panel->position_plot_area_now);
            chart_frame->draw_panel->grid_before.clear();
            (chart_frame->draw_panel->grid_before) = (chart_frame->draw_panel->grid_now);
            chart_frame->draw_panel->ticks_before.clear();
            (chart_frame->draw_panel->ticks_before) = (chart_frame->draw_panel->ticks_now);
            
            //store the data on the Routes at the preceeding step of the drag into points_route_list_before and reference_positions_route_list_before,
            chart_frame->draw_panel->points_route_list_before.clear();
            (chart_frame->draw_panel->points_route_list_before) = (chart_frame->draw_panel->points_route_list_now);
            
            chart_frame->draw_panel->points_position_list_before.clear();
            (chart_frame->draw_panel->points_position_list_before) = (chart_frame->draw_panel->points_position_list_now);
            
            chart_frame->draw_panel->reference_positions_route_list_before.clear();
            (chart_frame->draw_panel->reference_positions_route_list_before) = (chart_frame->draw_panel->reference_positions_route_list_now);
            
   
#endif
            //re-draw the chart
            (chart_frame->draw_panel->*(chart_frame->draw_panel->PreRender))();
            chart_frame->draw_panel->MyRefresh();
            chart_frame->UpdateSlider();
//            chart_frame->draw_panel->PaintNow();
            //

            //            cout << "\t\t t= " << t << "\n";
            
            (MotionHandler<F>::t)++;

        }

    }else {
        //the transport  is over

        
        switch (position_in_vector(Projection((chart_frame->projection->name->GetValue().ToStdString())), Projection_types)) {
                
            case 0: {
                //I am using Projection_types[0]
                
//                PositionProjection temp;
//
//                
//                start.transport(&p_NE, transporting_route, String(""));
//                (chart_frame->lambda_max) = (p_NE.lambda);
//                (chart_frame->phi_max) =( p_NE.phi);
//                
//                temp.SetMercator(p_NE);
//                (chart_frame->draw_panel->*(chart_frame->draw_panel->ProjectionToGeo))(temp - projection_size, &p_SW);
//                
//                (chart_frame->lambda_min) = p_SW.lambda;
//                (chart_frame->phi_min) = p_SW.phi;

                
                break;
                
            }
                
            case 1: {
                //I am using Projection_types[1]
                
                //do the whole transport rather than combining many little transports, to avoid rounding errors
//                chart_frame->draw_panel->circle_observer.reference_position = start;
//                chart_frame->draw_panel->circle_observer.reference_position.transport_to(transporting_route, String(""));

//                chart_frame->draw_panel->rotation.set(((chart_frame->draw_panel->rotation_start_drag) * Rotation(transporting_route.end, start)));

                chart_frame->draw_panel->rotation_end_drag.set((chart_frame->draw_panel->rotation));

                break;
                
            }
                
        }
            
//        cout << "******************* After the transport *******************" << endl;
//        transporting_route.compute_end(String(""));
//        transporting_route.end.print(String("Expected arrival position"), String("\t"), cout);
//        chart_frame->draw_panel->circle_observer.reference_position.print(String("Circle observer reference position"), String("\t"), cout);
        

        (chart_frame->dragging_chart) = false;
        chart_frame->EnableAll(true);
        chart_frame->Fit();

        //bind back all the methoud that have been unbound at the beginnign of the transport
        chart_frame->draw_panel->Bind(wxEVT_MOTION, &DrawPanel::OnMouseMovement, chart_frame->draw_panel);
        chart_frame->parent->listcontrol_sights->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_positions->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->listcontrol_routes->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
        chart_frame->parent->panel->Bind(wxEVT_MOTION, &ListFrame::OnMouseMovement, chart_frame->parent);
    
        (MotionHandler<F>::timer)->Stop();
        (*((MotionHandler<F>::parent)->unset_idling))();
        
        //call the functor to be called at the end of the animation, if any
        if((MotionHandler<F>::f) != NULL){
            (*(MotionHandler<F>::f))();
        }
        
    }

}
