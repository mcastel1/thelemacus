//
//  int.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "int.h"



//set value equal to i
inline void Int::set(String name, int i, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    set(i);

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



