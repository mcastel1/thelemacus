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
void mean_longitude(Angle&, Angle&, Angle*);
template<class T> void my_push_back(vector<T>*, const T&);
template<class T> void my_vector_memcpy(vector<T>*, const vector<T>);
string to_string(const Position&, unsigned int);
bool my_cross(const gsl_vector*, const gsl_vector*, gsl_vector**);
void find_and_replace_case_insensitive(wxComboBox*, wxArrayString, bool*, unsigned int*);
template<class T> void SetColor(T*);
template <class T> void Reset(T*);
wxImage RescaleProportionally(wxImage, const wxSize);
String read_from_file(String, String, String, [[maybe_unused]] String);
//void my_cout(int, ...);
bool operator < (const Length&, const Length&);
template<class S> void read_list_from_stream(String, S*, bool, vector<int>*);
double lambda_mercator(double);
double phi_mercator(double);
bool operator < (const Angle&, const Angle&);
bool operator < (const Angle&, const double&);
void AdjustWidth(wxComboBox*);
template <class Tp> struct my_allocator;

#endif


