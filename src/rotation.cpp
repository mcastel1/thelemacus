//
//  rotation.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "rotation.h"
#include "generic.h"



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
//inline 
void Rotation::set(const Rotation& r){
    
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
        rotation_angle.set(acos(cos_rotation_angle));


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
        set(matrix);

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

    if ((name.value) != "") {
        
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

template void Rotation::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);
template void Rotation::read_from_stream< basic_istringstream<char,struct char_traits<char>, allocator<char> > >( String, basic_istringstream<char,struct char_traits<char>, allocator<char> > *,bool, String);


//reads from file the content after 'name = ' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Rotation::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Rotation>(this, name, filename, mode, prefix);

}
