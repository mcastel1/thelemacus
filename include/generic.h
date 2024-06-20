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

#include "constants.h"
#include "position.h"
#include "double.h"
#include "length.h"
#include "file_rw.h"
#include "my_app.h"

using namespace std;

inline double cot(double);
inline double csc(double);
inline double atan(double, double);
inline int floor_exp(double);
void MousePositionOnListControl(wxListCtrl*, int*);
template<class C> void read_from_file(C*, String, String, [[maybe_unused]] String, [[maybe_unused]] String);
bool check_unsigned_int(string, unsigned int*, bool, unsigned int, unsigned int);
bool check_int(string, int*, bool, int, int);
inline double round_with_precision(double, unsigned int);


#endif
