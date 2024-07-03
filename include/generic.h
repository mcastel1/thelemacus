#ifndef generic_h
#define generic_h

#include <iostream>

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif
#include <wx/combobox.h>
#include <wx/listctrl.h>
#include <wx/dcclient.h>


#include "gsl_math.h"
#include "gsl_vector.h"

#include "angle.h"
#include "constants.h"
#include "position.h"
#include "double.h"
#include "length.h"
#include "file_rw.h"
#include "my_app.h"
#include "my_string.h"

using namespace std;


double cot(double);
double csc(double);
double sin(const Angle&);
double asin(const Double&);
double cos(const Angle&);
double tan(const Angle&);
double csc(const Angle&);
double sec(const Angle&);
double cot(const Angle&);
double acos(const Double&);
double atan(double, double);
int floor_exp(double);
void MousePositionOnListControl(wxListCtrl*, int*);
template<class C> void read_from_file(C*, String, String, [[maybe_unused]] String, [[maybe_unused]] String);
bool check_unsigned_int(string, unsigned int*, bool, unsigned int, unsigned int);
bool check_int(string, int*, bool, int, int);
void enter_unsigned_int(unsigned int*, bool, unsigned int, unsigned int, String, String);
bool check_double(string, double*, bool, double, double);
void enter_double(double*, bool, double, double, String, String);
double round_with_precision(double, unsigned int);
template<class T> unsigned int position_in_vector(T, const vector<T>&);
template<class A, class B> vector<B> convert_vector(const vector<A>&);
template<class T> unsigned int address_position_in_vector(T*, const vector<T>&);
template<class T> void delete_duplicates(vector<T>*);
template<class T> bool equal_rel_epsilon_double(const T&, const T&);
template<class T> void delete_duplicates(vector<T>*, bool (*)(const T&, const T&));
wxSize get_size(const String&, wxWindow*);
wxSize get_size(const String&, wxDC*);
Angle normalize_pm_pi_ret(const Angle&);
double mean_pm_pi(Angle&, Angle&);
double mean_value(Angle&, Angle&);
string to_string(const Position&, unsigned int);
bool my_cross(const gsl_vector*, const gsl_vector*, gsl_vector**);
void find_and_replace_case_insensitive(wxComboBox*, wxArrayString, bool*, unsigned int*);
template<class T> void SetColor(T*);
template <class T> void Reset(T*);
wxImage RescaleProportionally(wxImage, const wxSize);
String read_from_file(String, String, String, [[maybe_unused]] String);
void my_cout(int, ...);
bool operator < (const Length&, const Length&);
template<class S> void read_list_from_stream(String, S*, bool, vector<int>*);
double lambda_mercator(double);
double phi_mercator(double);
bool operator < (const Angle&, const Angle&);
bool operator < (const Angle&, const double&);
void AdjustWidth(wxComboBox*);



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
inline double round_with_precision(double x, unsigned int precision)
{
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


//normalize a and b between -pi and pi, and return  the algebraic mean between a.value and b.value
//inline 
double mean_pm_pi(Angle& a, Angle& b){
    
    return(((a.normalize_pm_pi_ret().value) + (b.normalize_pm_pi_ret().value))/2.0);
    
}


//return the mean between a.value and b.value
//inline 
double mean_value(Angle& a, Angle& b){
    
    return( ((a.value) + (b.value))/2.0 );
    
}



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


//this function returns the longitude value (expressed in degrees, positive towards W) of the inverse spherical Mercator projection from the rectangular x value
//inline 
double lambda_mercator(double x){
    
    return (-x*rad_to_deg - 360.0*floor((x-(-M_PI))/(2.0*M_PI)));
    
}



#endif


