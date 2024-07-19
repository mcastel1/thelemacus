//
//  cartesian.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "cartesian.h"

#include "generic.h"


//inline 
Cartesian::Cartesian(void) {

    r = gsl_vector_alloc(3);

}


//construct *this setting its coordinates from the coordinates of the geographic Position p
//inline 
Cartesian::Cartesian(const Position* p) {

    r = gsl_vector_alloc(3);

    gsl_vector_set(r, 0, cos(p->phi) * cos(p->lambda));
    gsl_vector_set(r, 1, -cos(p->phi) * sin(p->lambda));
    gsl_vector_set(r, 2, sin(p->phi));


}

//set the content (not the memory addresses of *this) equal to the content of x
void Cartesian::set(const Cartesian& x){
    
    gsl_vector_memcpy(r, (x.r));
    
}


//set *this from the grographic Position p
//inline 
void Cartesian::setPosition(const Position* p){
    

    gsl_vector_set(r, 0, cos(p->phi) * cos(p->lambda));
    gsl_vector_set(r, 1, -cos(p->phi) * sin(p->lambda));
    gsl_vector_set(r, 2, sin(p->phi));
    
    
}


//return the dot product between *this and s
//inline 
double Cartesian::dot(const Cartesian& s){
    
    double result;
    
    gsl_blas_ddot(r, s.r, &result);
    
    return result;
    
}


//return the cross product between the vector of this and that of s
//inline 
Cartesian Cartesian::cross(const Cartesian& s){
    
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

