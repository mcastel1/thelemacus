//
//  rotation.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef rotation_h
#define rotation_h

#include "gsl_blas.h"

#include "angle.h"
#include "position.h"
#include "my_string.h"

class Position;


//this class denotes a rigid rotation R_z(c).R_x(-b).R_z(a) with Euler angles a, b, c
class Rotation{
    
public:
    
    //R[3*i+j] is the element of the j-th column, i-th row of the rotation matrix of this
    Angle a, b, c;
    gsl_matrix* matrix;
    
    Rotation();
    Rotation(Angle, Angle, Angle);
    Rotation(Position, Position);
    
    Rotation inverse(void);
    void set(Angle, Angle, Angle);
    void set(gsl_matrix*);
    void set(const Rotation&);
    void print(String, String, ostream&);
    template<class S> void read_from_stream(String, S*, bool, String);
    void read_from_file_to(String, String, String, String);
    
    Rotation operator *(const Rotation&);

};

#endif
