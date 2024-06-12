//
//  angle.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef angle_h
#define angle_h


using namespace std;

class Angle{
    
public:
    
    double value;
    
    Angle();
    Angle(const double&);
    Angle(const unsigned int&, const double&);
    Angle(String, const double&, const String&);
    void normalize(void);
    Angle normalize_ret(void);
    void normalize_pm_pi(void);
    Angle normalize_pm_pi_ret(void);
    Angle span(Angle);
    void set(double);
    void set(String, double, String);
    void print(String, String, ostream&);
    bool is_zero_epsilon_double(void);
    void to_deg_min(unsigned int*, double*);
    void to_deg_min(unsigned int*, double*, unsigned int);
    void from_sign_deg_min(char, unsigned int, double);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    void to_deg_min_string(stringstream, stringstream);
    string to_string(String, unsigned int, bool);
    string deg_to_string(String, unsigned int);
    string min_to_string(String, unsigned int);

    bool operator == (const Angle&) const, operator == (const double&), operator != (const double&), operator !=(const Angle&), operator > (const Angle&), operator > (const double&);
    Angle operator + (const Angle&), operator - (const Angle&), operator / (const double&), operator * (const double&);
    void operator += (const Angle&), operator += (const double&), operator -= (const Angle&), operator -= (const double&);
    static bool strictly_smaller_normalize_pm_pi_ret(Angle&, Angle&), strictly_larger_normalize_pm_pi_ret(Angle&, Angle&);

    
};


#endif
