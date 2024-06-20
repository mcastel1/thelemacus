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

//I COMMENTED THESE OUT BECAUSE THEY YIELD CONFLICTING DECLARATIONS -> UNCOMMENT THESE AT THE END WHEN YOU WILL BE ABLE TO RENAME CLASS MEMBERS (AND THUS AVOID CONFLICTS WITH BOOST) MORE EASILY BECAUSE THERE WILL BE LESS COMPILATION ERRORS
//#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/replace.hpp>
//#include <boost/dll.hpp>
//#include <boost/filesystem.hpp>
//#include <boost/date_time.hpp>
//#include "boost/date_time/local_time/local_time.hpp"
//I COMMENTED THESE OUT BECAUSE THEY YIELD CONFLICTING DECLARATIONS -> UNCOMMENT THESE AT THE END WHEN YOU WILL BE ABLE TO RENAME CLASS MEMBERS (AND THUS AVOID CONFLICTS WITH BOOST) MORE EASILY BECAUSE THERE WILL BE LESS COMPILATION ERRORS

#include "angle.h"
#include "constants.h"
#include "position.h"
#include "double.h"
#include "length.h"
#include "file_rw.h"
#include "my_app.h"
#include "my_string.h"

using namespace std;


inline double cot(double);
inline double csc(double);
inline double sin(const Angle&);
inline double asin(const Double&);
inline double cos(const Angle&);
inline double tan(const Angle&);
inline double csc(const Angle&);
inline double sec(const Angle&);
inline double cot(const Angle&);
inline double acos(const Double&);
inline double atan(double, double);
inline int floor_exp(double);
void MousePositionOnListControl(wxListCtrl*, int*);
template<class C> void read_from_file(C*, String, String, [[maybe_unused]] String, [[maybe_unused]] String);
bool check_unsigned_int(string, unsigned int*, bool, unsigned int, unsigned int);
bool check_int(string, int*, bool, int, int);
void enter_unsigned_int(unsigned int*, bool, unsigned int, unsigned int, String, String);
bool check_double(string, double*, bool, double, double);
void enter_double(double*, bool, double, double, String, String);
inline double round_with_precision(double, unsigned int);
template<class T> unsigned int position_in_vector(T, const vector<T>&);
template<class A, class B> vector<B> convert_vector(const vector<A>&);
template<class T> unsigned int address_position_in_vector(T*, const vector<T>&);
template<class T> void delete_duplicates(vector<T>*);
template<class T> bool equal_rel_epsilon_double(const T&, const T&);
template<class T> void delete_duplicates(vector<T>*, bool (*)(const T&, const T&));
wxSize get_size(const String&, wxWindow*);
wxSize get_size(const String&, wxDC*);

inline double round_with_precision(double, unsigned int);


#endif
