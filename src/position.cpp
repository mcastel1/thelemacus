//
//  position.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "position.h"
#include "angle.h"
#include "generic.h"
#include "cartesian.h"

Position::Position(void) {

}

Position::Position(Angle lambda_in, Angle phi_in) {

    lambda = lambda_in;
    phi = phi_in;

}

//evaluates whether Position (*this) is equal to Position p
bool Position::operator==(const Position& p) {

    bool check;

    check = true;

    check &= (phi == (p.phi));
    check &= (lambda == (p.lambda));
    check &= (label == (p.label));

    return check;

}

//evaluates whether Position (*this) is different from Position p
bool Position::operator!=(const Position& p) {

    return (!((*this) == p));

}




//write the distance between *this and p in *l, and set l.unit to LengthUnit_types[0], here name is the name of the distance that I am computing; for example 'distance between positions A and B'
bool Position::distance(Position p, Length* l, String name, [[maybe_unused]] String prefix) {

    bool check;
    check = true;

    
    l->set(Re * acos(cos((lambda)-(p.lambda)) * cos(phi) * cos(p.phi) + sin(phi) * sin(p.phi)), LengthUnit_types[0]);

    check &= !isnan(l->value);

    if(check){

        Angle a;
        a.set(String(""), (l->value) / Re, prefix);
        l->print(name, prefix, cout);

    }else{

        cout << prefix.value << RED << "\tI could not compute distance!\n" << RESET;

    }

    return check;

}

//if route is a circle of equal altitude, it returns true if *this is into the circle, and zero otherwise. If route is not a circle of equal altitude, it prints an error message and returns false.
bool Position::is_in(Route route, [[maybe_unused]] String prefix) {

    if ((route.type) == (Route_types[2])) {

        Length d;

        distance((*(route.reference_position)), &d, String(""), prefix);

        return(d <= (Re * (route.omega.value)));


    }else{

        cout << prefix.value << RED << "route is not a circle of equal altitude: I cannot tell whether the position is into route!\n" << RESET;

        return false;

    }

}


//creates an element in listcontrol and writes into this element the values of all the fields contained in this
void Position::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    unsigned int i;
    wxListItem item;

    if (position_in_listcontrol == -1) {
        i = (listcontrol->GetItemCount());
    }
    else {
        i = ((unsigned int)position_in_listcontrol);
        listcontrol->DeleteItem(i);
    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_wxListCtrl(i, listcontrol);

}

//updates all the values in the GUI fields of item #i of listcontrol with the relative values of the non-GUI Position this
void Position::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

    int j;

    j = 0;

    //update number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //update latitude column
    listcontrol->SetItem(i, j++, wxString(phi.to_string(String("NS"), (display_precision.value), true)));

    //update longitude column
    listcontrol->SetItem(i, j++, wxString(lambda.to_string(String("EW"), (display_precision.value), true)));

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol
    //
    listcontrol->SetItem(i, j++, wxString(label.value));

    //    }else{
    //        //the label in *this is empty -> I write as label in listcontrol  and in *this the current date and time
    //
    //        Time now;
    //
    //        now.set_current(String(""));
    //
    //        label.set(String("Label replacing empty label"), now.to_string(display_precision.value), String(""));
    //        listcontrol->SetItem(i, j++, wxString(now.to_string(display_precision.value)));
    //
    //    }
    //
}

string Position::to_string(unsigned int precision) {

    stringstream output;

    output << phi.to_string(String("NS"), precision, true) << " " << lambda.to_string(String("EW"), precision, true);

    return (output.str().c_str());

}

//transport the Position *this with the Route route and write the result in *this. If route is not a circle of equal altitude, the transport is done and true is returned, otherwise no transport is done and false is returned.
bool Position::transport_to(Route route, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    if ((route.type) != (Route_types[2])) {
        //route.type = ((Route_types[0]).value) or ((Route_types[1]).value) -> I can transport *this

        Route temp;

        temp = route;
        (*(temp.reference_position)) = (*this);
        temp.compute_end(new_prefix);
        (*this) = (*(temp.end));

        return true;

    }
    else {
        //route.type = "circle fo equal altitude" -> I cannot transport *this

        cout << prefix.value << RED << "Cannot transport a position with a circle of equal altitude!\n" << RESET;

        return false;

    }

}


//check whether phi and lambda are equal to phi_min ... lambda_max within accuracy epsilon_double and, if they are, set them equal to phi_min ... lambda_max, respectively
//inline 
void Position::put_back_in(DrawPanel* draw_panel){
        
    switch (position_in_vector(Projection((draw_panel->parent->projection->name->GetValue()).ToStdString()), Projection_types)) {
            
        case 0: {
            //I am using the mercator projection
            
            if(equal_rel_epsilon_double(phi, (*(draw_panel->parent->phi_min)))){
                phi = (*(draw_panel->parent->phi_min));
            }
            
            if(equal_rel_epsilon_double(phi, (*(draw_panel->parent->phi_max)))){
                phi = (*(draw_panel->parent->phi_max));
            }
            
            if(equal_rel_epsilon_double(lambda, (*(draw_panel->parent->lambda_min)))){
                lambda = (*(draw_panel->parent->lambda_min));
            }
            
            if(equal_rel_epsilon_double(lambda, (*(draw_panel->parent->lambda_max)))){
                lambda = (*(draw_panel->parent->lambda_max));
            }
            
            break;
            
        }
            
        case 1: {
            //I am using the 3D projection
            
        }
            
    }
    
    
    
}


//return the antipode of *this on the earth
Position Position::half(void){
    
    Position result;
    
    result.lambda = lambda/2.0;
    result.phi = phi/2.0;
    
    return result;
    
}

//return the antipode (with respect to the longitude only) of *this on the earth
Position Position::antipode_lambda(void){
    
    Position result;
    
    result.lambda = lambda + M_PI;
    result.phi = phi;
    
    return result;
    
}


//transport the Position *this with the Route route and write the result in *result. If route is not a circle of equal altitude, the transport is done and true is returned, otherwise no transport is done and false is returned. result needs to be allocated before this method is called
bool Position::transport(Position* result, Route route, [[maybe_unused]] String prefix) {
        
    (*result) = (*this);

    return((result->transport_to(route, prefix)));

}


//rotates the Position (*this) according to the Rotation s, and writes the result in *p
void Position::rotate(const String& name, const Rotation& r, Position* p, [[maybe_unused]] const String& prefix) {

    Cartesian u, s;

    //write (*this) into u in cartesian coordinates
    gsl_vector_set((u.r), 0, cos(lambda) * cos(phi));
    gsl_vector_set((u.r), 1, -(cos(phi) * sin(lambda)));
    gsl_vector_set((u.r), 2, sin(phi));

    //rotate u according to r and write the result in s and then in (*this)
    //    gsl_blas_dgemv(CblasNoTrans, 1.0, (r.matrix), (u.r), 0.0, (s.r));
    cblas_dgemv(CblasRowMajor, CblasNoTrans, 3, 3, 1, r.matrix->data, 3, u.r->data, 1, 0, s.r->data, 1);


    //     cout << "\tNorm of u = " << gsl_blas_dnrm2(u);
    //     cout << "\tNorm of s = " << gsl_blas_dnrm2(s);

    p->setCartesian(name, s, prefix);

}

//read from stream input_stream the Position by starting at the current position of input_stream. Here name and search_entire_stream are unused and have been included as arguments of the function in order to match with the format of read_from_stream of other classes and so in order to use template<class C> void read_from_file(C* object, String name, String filename, [[maybe_unused]] String prefix) throughout the code
template<class S> void Position::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool search_entire_stream, [[maybe_unused]] String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    phi.read_from_stream<S>(String("latitude"), input_stream, false, new_prefix);
    lambda.read_from_stream<S>(String("longitude"), input_stream, false, new_prefix);

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }

}

template void Position::read_from_stream<std::__1::basic_fstream<char, std::__1::char_traits<char>>>(String, std::__1::basic_fstream<char, std::__1::char_traits<char>>*, bool, String);


//set the content (not the memory adresses) of *this equal to the content of x
void Position::set(const Position& x){
    
    phi.set(x.phi);
    lambda.set(x.lambda);
    
    label.set(x.label);
    
}


//set the polar coordinates lambda, phi of (*this) from the Cartesian position r
void Position::setCartesian(String name, const Cartesian r, [[maybe_unused]] String prefix) {

    String new_prefix, name_lambda, name_phi;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    if (name != String("")) {
        name_lambda = String("longitude");
        name_phi = String("latitude");
    }
    else {
        name_lambda = String("");
        name_phi = String("");
    }

    if (name != String("")) {
        cout << prefix.value << name.value << "\n";
    }

    lambda.set(name_lambda, -atan(gsl_vector_get((r.r), 0), gsl_vector_get((r.r), 1)), String(prefix));
    phi.set(name_phi, asin(gsl_vector_get((r.r), 2) / gsl_blas_dnrm2((r.r))), String(prefix));

}

//write the cartesian components of Position p into *r
void Position::getCartesian(Cartesian* r) {

    gsl_vector_set((r->r), 0, cos(phi) * cos(lambda));
    gsl_vector_set((r->r), 1, -cos(phi) * sin(lambda));
    gsl_vector_set((r->r), 2, sin(phi));

}



//same as Position::getCartesian(Cartesian* r) , but it returns the Cartesian position rather than writing it into *r
Cartesian Position::getCartesian(void){
    
    Cartesian temp;
    
    getCartesian(&temp);
    
    return temp;
    
}

void Position::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    ostr << prefix.value << name.value << ":\n";

    phi.print(String("latitude"), new_prefix, ostr);
    lambda.print(String("longitude"), new_prefix, ostr);

    label.print(String("label"), true, new_prefix, ostr);

}

//reads from file the content after 'Position # ...' and writes it into *this.
//if mode = 'RW' ('R') it reads form a FileRW (FileR)
void Position::read_from_file_to(String name, String filename, String mode, [[maybe_unused]] String prefix) {

    read_from_file<Position>(this, name, filename, mode, prefix);

}



