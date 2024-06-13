//
//  time.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef time_h
#define time_h






class Time{
    
public:
    Date date;
    Chrono chrono;
    //is s used?
    double s, MJD;
    void enter(String, String);
    void print(String, String, ostream&);
    void set_current(void);
    template<class S> bool read_from_stream(String, S*, bool, String);
    
    void to_MJD(void);
    void to_TAI(void);
    
    string to_string(unsigned int, bool);
    bool operator==(const Time&), operator!=(const Time&), operator> (const Time&);
    void operator += (const Chrono&);
    void operator -= (const Chrono&);
    
};





#endif
