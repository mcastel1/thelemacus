//
//  sight.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#include "sight.h"

#include <sstream>

#include "gsl_roots.h"
#include "gsl_integration.h"
#include "gsl_errno.h"
#include "gsl_spline.h"


#include "constants.h"
#include "file_r.h"
#include "generic.h"
#include "my_app.h"
#include "units.h"


Sight::Sight(void) {
    
    
    atmosphere = new Atmosphere;
    body = new Body;
    height_of_eye = new Length;
    r = new Length;
    
    //height_of_eye is expressed in meters -> set its unit accordingly
    height_of_eye->unit->set(LengthUnit_types[1]);

    //this is the list of all the possible items that a Sight object can have: some Sight objects may have an item list with fewer elements than all_items. For instance, a star Sight does not have the "limb" element.
    all_items.push_back(String("body"));
    all_items.push_back(String("limb"));
    all_items.push_back(String("sextant altitude"));
    all_items.push_back(String("artificial horizon"));
    all_items.push_back(String("height of eye"));
    all_items.push_back(String("master-clock date and hour of sight"));
    all_items.push_back(String("use of stopwatch"));
    all_items.push_back(String("stopwatch reading"));
    all_items.push_back(String("label"));

    items.push_back(all_items[0]);
    items.push_back(all_items[2]);
    items.push_back(all_items[3]);
    items.push_back(all_items[5]);
    items.push_back(all_items[6]);
    items.push_back(all_items[8]);

    //initiazlie the limb to a 'n/a' value
    limb.value = 'n';
    atmosphere->initialize();
    (related_route.value) = -1;

}


//set the content (not the memory adresses) of *this equal to the content of x
void Sight::set(const Sight& x){
    
    master_clock_date_and_hour.set(x.master_clock_date_and_hour);
    time.set(x.time);
    TAI_minus_UTC.set(x.TAI_minus_UTC);
    stopwatch.set(x.stopwatch);
    
    index_error.set(x.index_error);
    H_s.set(x.H_s);
    H_a.set(x.H_a);
    H_o.set(x.H_o);
    H_i.set(x.H_i);
    DH_refraction.set(x.DH_refraction);
    DH_dip.set(x.DH_dip);
    DH_parallax_and_limb.set(x.DH_parallax_and_limb);
    
    r->set((*(x.r)));
    height_of_eye->set((*(x.height_of_eye)));
    
    atmosphere->set((*(x.atmosphere)));
    
    body->set((*(x.body)));
    
    limb.set(x.limb);
    
    artificial_horizon.set(x.artificial_horizon);
    use_stopwatch.set(x.use_stopwatch);
    
    label.set(x.label);
    
    related_route.set(x.related_route);

    my_vector_memcpy(&items, x.items);
    my_vector_memcpy(&all_items, x.all_items);
    
}


//this function simplifies the atmosphere between z=0 and z=eight of eye as a single layer, where within this layer the index of refracion is independent of z. Refine this in the future.
void Sight::compute_DH_dip(String prefix) {

    DH_dip.set(String("Dip correction"),
               -acos(atmosphere->n(Length(0.0)) / atmosphere->n((*height_of_eye))
                     * ((atmosphere->earth_radius->convert(LengthUnit_types[0]).value) / (((*(atmosphere->earth_radius)) + (*height_of_eye)).convert(LengthUnit_types[0]).value) )
              ), prefix);

}


bool Sight::compute_DH_refraction(String prefix) {

    gsl_integration_workspace* w = gsl_integration_workspace_alloc(1000);
    gsl_function F;
    double result, error;
    int status;
    bool check = true;


    F.function = &dH_refraction;
    F.params = this;

    /* cout << "Value = " << dH_refraction(1.0, &(*this)); */
    /* cin >> result; */

    status = gsl_integration_qags(&F, atmosphere->h.back().convert(LengthUnit_types[0]).value, atmosphere->h.front().convert(LengthUnit_types[0]).value, 0.0, epsilon_real, 1000, w, &result, &error);
    //status = GSL_FAILURE

    if (status == GSL_SUCCESS) {
        DH_refraction.set(String("refraction correction"), result, prefix);
    }else{
        check &= false;
        cout << prefix.value << RED << "GSL integration failed!\n" << RESET;
    }

    gsl_integration_workspace_free(w);

    return check;

}



void Sight::update_wxListCtrl(long i, wxListCtrl* listcontrol) {

    unsigned int j;
    Time time_UTC;


    j = 0;
    //set number column
    listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (int)(i + 1)));

    //set body column
    listcontrol->SetItem(i, j++, wxString(body->name->value));

    //set limb column
    if (((*(body->name)) == String("sun")) || ((*(body->name)) == String("moon"))) {

        if (wxString(limb.value) == 'u') { listcontrol->SetItem(i, j++, wxString((Limb_types[0].value))); }
        if (wxString(limb.value) == 'l') { listcontrol->SetItem(i, j++, wxString((Limb_types[2].value))); }
        if (wxString(limb.value) == 'c') { listcontrol->SetItem(i, j++, wxString((Limb_types[1].value))); }

    }
    else {

        listcontrol->SetItem(i, j++, wxString(""));

    }

    //set artificial horizon column
    listcontrol->SetItem(i, j++, wxString(artificial_horizon.value));

    //set sextant altitude column
    listcontrol->SetItem(i, j++, wxString((H_s).to_string(String(""), (display_precision.value), true)));

    //set index error
    listcontrol->SetItem(i, j++, wxString((index_error).to_string(String(""), (display_precision.value), true)));

    //set height of eye column
    if (artificial_horizon.value == 'n') {

        //write the height_of_eye with its original unit of measure
        listcontrol->SetItem(i, j++, wxString(height_of_eye->to_string( (display_precision.value))));

    }
    else {

        listcontrol->SetItem(i, j++, wxString(""));

    }

    //set column of master-clock date and hour of sight
    //I add to master_clock_date_and_hour the value stopwatch (if any): I write the result in time_UTC and I write in the GUI object  time_UTC
    time_UTC = master_clock_date_and_hour;
    //    if((use_stopwatch.value)=='y'){time_UTC += stopwatch;}
    listcontrol->SetItem(i, j++, wxString(time_UTC.to_string(display_precision.value, false)));

    //set use of stopwatch
    listcontrol->SetItem(i, j++, wxString((use_stopwatch.value)));

    //set stopwatch reading
    if ((use_stopwatch.value) == 'y') {
        listcontrol->SetItem(i, j++, wxString((stopwatch).to_string(display_precision.value, false)));
    }
    else {
        listcontrol->SetItem(i, j++, wxString(""));
    }

    //set TAI-UTC
    listcontrol->SetItem(i, j++, wxString((TAI_minus_UTC).to_string((display_precision.value), false)));

    //update label column
    //    if(label != String("")){
    //        //the label in *this is not empty -> I write it int listcontrol

    listcontrol->SetItem(i, j++, wxString(label.value));

    if ((related_route.value) == -1) {
        //if the sight is not connected to a route, I leave the column field empty

        listcontrol->SetItem(i, j++, wxString(""));

    }
    else {
        //if the sight is connected to a route, I write the # of the related route in the column field

        listcontrol->SetItem(i, j++, wxString::Format(wxT("%i"), (related_route.value) + 1));

    }


}


void Sight::add_to_wxListCtrl(long position_in_listcontrol, wxListCtrl* listcontrol) {

    long i;
    wxListItem item;

    if (position_in_listcontrol == -1) {

        i = (listcontrol->GetItemCount());

    }
    else {

        i = position_in_listcontrol;
        listcontrol->DeleteItem(i);

    }

    item.SetId(i);
    item.SetText(wxT(""));

    listcontrol->InsertItem(item);

    update_wxListCtrl(i, listcontrol);

}


//this function returns true if the reading operation has been performed without errors, false otherwise
template<class S> bool Sight::read_from_stream([[maybe_unused]] String name, S* input_stream, [[maybe_unused]] bool read_entire_stream, [[maybe_unused]] String prefix) {

    string line;
    bool check = true;
    String new_prefix;
    //this unsigned int counts how many additional entries have been inserted into the vector item
    unsigned int additional_items;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    additional_items = 0;

    body->read_from_stream<S>(String("body"), input_stream, false, new_prefix);
    if (body->type->value != "star") {
        items.insert(items.begin() + 1 + (additional_items++), all_items[1]);
        limb.read_from_stream<S>(String("limb"), input_stream, false, new_prefix);
    }
    H_s.read_from_stream<S>(String("sextant altitude"), input_stream, false, new_prefix);
    index_error.read_from_stream<S>(String("index error"), input_stream, false, new_prefix);
    artificial_horizon.read_from_stream<S>(String("artificial horizon"), input_stream, false, new_prefix);
    if (artificial_horizon == Answer('n', new_prefix)) {
        items.insert(items.begin() + 3 + (additional_items++), String("height of eye"));
        height_of_eye->read_from_stream<S>(String("height of eye"), input_stream, false, new_prefix);
    }

    check &= master_clock_date_and_hour.read_from_stream<S>(String("master-clock date and hour of sight"), input_stream, false, new_prefix);
    if (!check) {
        cout << prefix.value << RED << "\tMaster-clock date and hour is not valid!\n" << RESET;
    }
    time = master_clock_date_and_hour;

    use_stopwatch.read_from_stream<S>(String("use of stopwatch"), input_stream, false, new_prefix);

    if (use_stopwatch == Answer('y', new_prefix)) {

        items.insert(items.begin() + 5 + (additional_items++), String("stopwatch reading"));
        stopwatch.read_from_stream<S>(String("stopwatch"), input_stream, false, new_prefix);
        time += stopwatch;

    }

    TAI_minus_UTC.read_from_stream<S>(String("TAI - UTC at time of master-clock synchronization with UTC"), input_stream, false, new_prefix);
    time += TAI_minus_UTC;
    time.print(String("TAI date and hour of sight"), new_prefix, cout);

    //check whether the date and hour of sight falls within the time window covered by JPL data files
    check &= check_time_interval(prefix);

    label.read_from_stream<S>(String("label"), input_stream, false, new_prefix);
    if (label.value == "") {
        //if the value of label read from file is empty, set in label the time at which *this has been read

        label.set_to_current_time();

    }

    //given that the sight is not yet related to a route, I set
    (related_route.value) = -1;

    if (!check) {
        cout << prefix.value << RED << "Error reading sight!\n" << RESET;
    }

    return check;

}

template bool Sight::read_from_stream<basic_fstream<char, char_traits<char>>>(String, basic_fstream<char, char_traits<char>>*, bool, String);


//compute this->time and returns true if time lies within the data file of NASA JPL ephemerides files, and false otherwise. This function requires that body.name is specified: if body.name is "", it returns false.
bool Sight::check_time_interval(String prefix) {

    unsigned int i;
    int l_min, l_max;
    bool check;
    String new_prefix;

    
    //append \t to prefix
    new_prefix = prefix.append(String("\t"));


    //compute this->time
    
    //I first set time to master_clock_date_and_hour ...
    time = master_clock_date_and_hour;
    //.. then I add to it sight->stopwatch, if any ....
    if (use_stopwatch == Answer('y', String(""))) {
        time += stopwatch;
    }
    //... then I add to it TAI_minus_UTC, to convert it from the UTC to the TAI scale
    time += TAI_minus_UTC;
        
    //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
    l_min = (int)(n_lines_per_day_ephemerides * ((time.MJD) - MJD_min)) - (int)(n_lines_ephemerides / 2.0);
    //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
    l_max = (int)(n_lines_per_day_ephemerides * ((time.MJD) - MJD_min)) + (int)(n_lines_ephemerides / 2.0);
    
    
    
 
    //find the position of body->name in wxGetApp().list_frame->catalog and store the result in i
    for(i=0; (i<(wxGetApp().catalog->list.size())) && ((*(body->name)) != (*(((wxGetApp().catalog->list)[i]).name))); i++){}

    

    
    //check whether the lines from l_min to l_max, which are used for the data interpolation, are present in the file where data relative to the body are stored
    if ((l_min >= 0) && (l_max < (int)((wxGetApp().catalog->number_of_lines_bodies)[i]))) {
        check = true;
    }
    else {
        check = false;
        cout << new_prefix.value << RED << "Time lies outside interval of NASA's JPL data files!\n" << RESET;
    }
    
    return check;
    
    

}

void Sight::print(String name, String prefix, ostream& ostr) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));
    
    if ((name.value) != "") {
        
        ostr << prefix.value << name.value << ":\n";
        
        body->print(String("body"), new_prefix, ostr);
        if (body->type->value != "star") {
            limb.print(String("limb"), new_prefix, ostr);
        }
        H_s.print(String("sextant altitude"), new_prefix, ostr);
        index_error.print(String("index error"), new_prefix, ostr);
        artificial_horizon.print(String("artificial horizon"), new_prefix, ostr);
        if (artificial_horizon == Answer('n', new_prefix)) {
            height_of_eye->print(String("height of eye"), new_prefix, ostr);
        }
        master_clock_date_and_hour.print(String("master-clock date and hour of sight"), new_prefix, ostr);
        use_stopwatch.print(String("use of stopwatch"), new_prefix, ostr);
        if (use_stopwatch == Answer('y', new_prefix)) {
            stopwatch.print(String("stopwatch"), new_prefix, ostr);
        }
        TAI_minus_UTC.print(String("TAI - UTC at time of master-clock synchronization with UTC"), new_prefix, ostr);
        
        label.print(String("label"), true, new_prefix, ostr);
        
        if (((related_route.value) != -1) && (&ostr == &cout)) {
            ostr << new_prefix.value << "# of related route = " << (related_route.value) + 1 << "\n";
        }
        
    }

}



double Sight::rhs_DH_parallax_and_limb(double h, void* sight) {

    Sight* a = (Sight*)sight;

    return(-((a->H_i).value) + h + asin(((a->body)->radius->value) / sqrt(gsl_pow_2(((a->r)->value)) + gsl_pow_2(((a->atmosphere)->earth_radius->value)) - 2.0 * ((a->r)->value) * ((a->atmosphere)->earth_radius->value) * sin(h))) - atan((((a->atmosphere)->earth_radius->value) * cos(h)) / (((a->r)->value) - ((a->atmosphere)->earth_radius->value) * sin(h))));

}


double Sight::dH_refraction(double z, void* sight) {

    Sight* a = (Sight*)sight;
    Length z_Length, zero_Length;
    
//    z_Length.value = z;
    z_Length.set(z, LengthUnit_types[0]);
    zero_Length.set(0.0, LengthUnit_types[0]);

    return(-(a->atmosphere->earth_radius->value) * (a->atmosphere->n(zero_Length)) * cos((a->H_a)) * (a->atmosphere->dndz)(z_Length) / (a->atmosphere->n)(z_Length) / sqrt(gsl_pow_2(((a->atmosphere->earth_radius->value) + z) * (a->atmosphere->n)(z_Length)) - gsl_pow_2((a->atmosphere->earth_radius->value) * (a->atmosphere->n)(zero_Length) * cos((a->H_a)))));

}


bool Sight::reduce(Route* circle_of_equal_altitude, [[maybe_unused]] String prefix) {

    bool check = true;
    String new_prefix;
    stringstream temp;

    temp.clear();

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Reducing sight ...\n";

    (circle_of_equal_altitude->type.value) = ((Route_types[2]).value);

    compute_H_a(new_prefix);
    check &= get_coordinates(circle_of_equal_altitude, new_prefix);

    //link the circle of equal altitude (*circle_of_equal_altitude) to sight (*this)
    temp << body->name->value << " " << time.to_string(display_precision.value, false) << " TAI, " << label.value;
    (circle_of_equal_altitude->label).set(String(""), String(temp.str()), new_prefix);

    check &= compute_H_o(new_prefix);
    circle_of_equal_altitude->omega.set(M_PI_2 - (H_o.value));
    
    circle_of_equal_altitude->length->set(2.0 * M_PI * (Re.value) * sin(circle_of_equal_altitude->omega), LengthUnit_types[0]);

    if (!check) {

        cout << prefix.value << RED << "Sight cannot be reduced!\n" << RESET;

    }
    else {

        cout << prefix.value << "... done\n";

    }

    return check;

}


void Sight::compute_H_a(String prefix) {

    if (artificial_horizon == Answer('y', prefix)) {
        H_a = (H_s - index_error) / 2.0;
        H_a.print(String("apparent altitude"), prefix, cout);

    }
    else {
        compute_DH_dip(prefix);
        H_a = H_s - index_error + DH_dip;
        H_a.print(String("apparent altitude"), prefix, cout);
    }

}


bool Sight::compute_H_o(String prefix) {

    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Computing observed altitude ...\n";

    check &= compute_DH_refraction(new_prefix);

    if (check) {
        compute_DH_parallax_and_limb(new_prefix);
        H_o = H_a + DH_refraction + DH_parallax_and_limb;
        cout << prefix.value << "...done\n";
        H_o.print(String("observed altitude"), prefix, cout);
    }
    else {
        cout << prefix.value << RED << "H_o cannot be computed!\n" << RESET;
    }

    return check;

}

//here replace R -> R+height of eye for better precision
//check that for r = 0 the upper and lower limb give the same result
void Sight::compute_DH_parallax_and_limb(String prefix) {

    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    H_i = H_a + DH_refraction;
    H_i.print(String("intermediate altitude"), prefix, cout);

    if (body->type->value != "star") {

        switch ((limb.value)) {

        case 'u':
        {
            int status = 0;
            int iter = 0;
            double x = 0.0, x_lo = 0.0, x_hi = 2.0 * M_PI;
            gsl_function F;
            const gsl_root_fsolver_type* T;
            gsl_root_fsolver* s;

            F.function = &rhs_DH_parallax_and_limb;
            F.params = &(*this);

            T = gsl_root_fsolver_brent;
            s = gsl_root_fsolver_alloc(T);
            gsl_root_fsolver_set(s, &F, x_lo, x_hi);

            cout << new_prefix.value << "Using " << gsl_root_fsolver_name(s) << " method\n";

            cout << new_prefix.value << "iter" << "[lower" << ", upper] " << "root " << "err(est)\n";

            iter = 0;
            do {

                iter++;
                status = gsl_root_fsolver_iterate(s);

                x = gsl_root_fsolver_root(s);
                x_lo = gsl_root_fsolver_x_lower(s);
                x_hi = gsl_root_fsolver_x_upper(s);
                status = gsl_root_test_interval(x_lo, x_hi, 0.0, epsilon_real);
                if (status == GSL_SUCCESS) {
                    cout << new_prefix.value << "Converged.\n";
                }
                cout << new_prefix.value << iter << " [" << x_lo << ", " << x_hi << "] " << x << " " << x_hi - x_lo << "\n";

            } while ((status == GSL_CONTINUE) && (iter < max_iter));


            //H_o.value = (x_lo+x_hi)/2.0;
            DH_parallax_and_limb.value = (x_lo + x_hi) / 2.0 - (H_i.value);
            gsl_root_fsolver_free(s);

            break;
        }
        case 'l':
        {
            //    H_o.value = (H_i.value) + asin(((atmosphere.earth_radius->value)*cos(H_i)+(body.radius.value))/(r.value));
            DH_parallax_and_limb.value = asin(((atmosphere->earth_radius->value) * cos(H_i) + (body->radius->value)) / (r->value));
            break;
        }
        case 'c':
        {
            //H_o.value = (H_i.value) + asin((atmosphere.earth_radius->value)*cos(H_i)/(r.value));
            DH_parallax_and_limb.value = asin((atmosphere->earth_radius->value) * cos(H_i) / (r->value));
            break;
        }
        }

        DH_parallax_and_limb.print(String("parallax and limb correction"), prefix, cout);

    }
    else {

        DH_parallax_and_limb.value = 0.0;

    }


}



bool Sight::get_coordinates(Route* circle_of_equal_altitude, [[maybe_unused]] String prefix) {

    FileR file;
    stringstream filename, line_ins;
    string line, dummy, temp;
    int l, l_min, l_max;
    double MJD_tab[(unsigned int)n_lines_ephemerides], GHA_tab[(unsigned int)n_lines_ephemerides], d_tab[(unsigned int)n_lines_ephemerides], sum;
    gsl_interp_accel* acc = gsl_interp_accel_alloc();
    gsl_spline* interpolation_GHA = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)n_lines_ephemerides)), * interpolation_d = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)n_lines_ephemerides));
    bool check = true;
    String new_prefix;

    //append \t to prefix
    new_prefix = prefix.append(String("\t"));

    cout << prefix.value << "Fetching ephemerides' data ...\n";

    if ((body->type->value) != "star") {
        filename << (wxGetApp().data_directory).value << body->name->value << ".txt";
    }
    else {
        filename << (wxGetApp().data_directory).value << "j2000_to_itrf93.txt";
    }
    temp = filename.str();


    file.set_name(String(temp.c_str()));
    if ((file.open(new_prefix)) && check_time_interval(new_prefix)) {
        //the file corresponding to this->body exists and the time of *this lies within the time interval of NASA JPL ephemerides data


        //l_min is the ID of the line in NASA's webgeocalc data files at wihch the interpolation starts
        l_min = (int)(n_lines_per_day_ephemerides * ((time.MJD) - MJD_min)) - (int)(n_lines_ephemerides / 2.0);
        //l_max is the ID of the line in NASA's webgeocalc data files at wihch the interpolation ends
        l_max = (int)(n_lines_per_day_ephemerides * ((time.MJD) - MJD_min)) + (int)(n_lines_ephemerides / 2.0);

        /* cout << "\nl_min = " << l_min << "l_max = " << l_max; */

        //dummy read of file data
        for (l = 0; l < l_min; l++) {
            line.clear();
            getline(*(file.value), line);
        }


        if ((body->type->value) != "star") {
            //in this case I am getting the coordinate of a body with a non-zero size

            //if the body is not a star

            double r_tab[(unsigned int)n_lines_ephemerides];
            gsl_spline* interpolation_r = gsl_spline_alloc(gsl_interp_cspline, ((unsigned int)n_lines_ephemerides));

            for (; l < l_max; l++) {

                line.clear();
                line_ins.clear();

                getline(*(file.value), line);
                line_ins << line;
                cout << new_prefix.value << line << "\n";
                line_ins >> dummy >> dummy >> dummy >> GHA_tab[l - l_min] >> d_tab[l - l_min] >> r_tab[l - l_min] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy;

                MJD_tab[l - l_min] = ((double)(l - l_min)) / n_lines_per_day_ephemerides;

            }

            file.close(new_prefix);

            //convert to radians and nm
            for (l = 0; l < n_lines_ephemerides; l++) {

                //add minus sign because in JPL convention longitude is positive when it is E
                GHA_tab[l] *= (-1.0) * deg_to_rad;
                d_tab[l] *= deg_to_rad;
                r_tab[l] /= nm_to_km;

            }

            //remove discontinuous jumps in GHA to allow for interpolation
            for (sum = 0.0, l = 0; l < n_lines_ephemerides - 1; l++) {
                //cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
                if (((GHA_tab[l] - sum) > 0.0) && (GHA_tab[l + 1] < 0.0)) {
                    sum += 2.0 * M_PI;
                }
                GHA_tab[l + 1] += sum;
            }

            if (gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)n_lines_ephemerides) != GSL_SUCCESS) { check &= false; };

            if (gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)n_lines_ephemerides) != GSL_SUCCESS) { check &= false; }
            if (gsl_spline_init(interpolation_r, MJD_tab, r_tab, (unsigned int)n_lines_ephemerides) != GSL_SUCCESS) { check &= false; }


            cout << new_prefix.value << "Read values:\n";
            for (l = 0; l < n_lines_ephemerides; l++) {
                cout << new_prefix.value << MJD_tab[l] << " " << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
            }

            if (gsl_spline_eval_e(interpolation_GHA, (time.MJD) - MJD_min - ((double)l_min) / n_lines_per_day_ephemerides, acc, &((circle_of_equal_altitude->reference_position->lambda).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                circle_of_equal_altitude->reference_position->lambda.normalize();
                circle_of_equal_altitude->reference_position->lambda.print(String("GHA"), new_prefix, cout);
            }
            //(circle_of_equal_altitude->reference_position->lambda).set("GHA", gsl_spline_eval(interpolation_GHA, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);


            if (gsl_spline_eval_e(interpolation_d, (time.MJD) - MJD_min - ((double)l_min) / n_lines_per_day_ephemerides, acc, &(circle_of_equal_altitude->reference_position->phi.value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (circle_of_equal_altitude->reference_position->phi).normalize();
                (circle_of_equal_altitude->reference_position->phi).print(String("d"), new_prefix, cout);
            }
            //(circle_of_equal_altitude->reference_position->phi).set("d", gsl_spline_eval(interpolation_d, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

            if (gsl_spline_eval_e(interpolation_r, (time.MJD) - MJD_min - ((double)l_min) / n_lines_per_day_ephemerides, acc, &(r->value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                if ((r->check(String("r"), new_prefix))) {
                    r->print(String("r"), new_prefix, cout);
                }
                else {
                    check &= false;
                }
            }
            //r.set("r", gsl_spline_eval(interpolation_r, (time.MJD)-MJD_min-((double)l_min)/L, acc), new_prefix);

            gsl_spline_free(interpolation_r);


        }
        else {
            //in this case I am getting the coordinate of a body with a zero size (a star)

            //if the body is a star
            double phi3, phi2, phi1;

            for (; l < l_max; l++) {

                line.clear();
                line_ins.clear();

                getline(*(file.value), line);
                line_ins << line;
                cout << new_prefix.value << line << "\n";
                line_ins >> dummy >> dummy >> dummy >> phi3 >> phi2 >> phi1;

                phi1 *= deg_to_rad;
                phi2 *= deg_to_rad;
                phi3 *= deg_to_rad;

                d_tab[l - l_min] = asin(cos(phi2) * sin((*(body->d))) - cos((*(body->d))) * cos(phi1) * sin((body->RA->value)) * sin(phi2) + cos((*(body->RA))) * cos((*(body->d))) * sin(phi1) * sin(phi2));

                GHA_tab[l - l_min] = atan((-cos(phi3) * sin((*(body->d))) * sin(phi2) - cos((*(body->RA))) * cos((*(body->d))) * (-cos(phi2) * cos(phi3) * sin(phi1) - cos(phi1) * sin(phi3)) - cos((*(body->d))) * sin((body->RA->value)) * (cos(phi1) * cos(phi2) * cos(phi3) - sin(phi1) * sin(phi3))) / (sin((*(body->d))) * sin(phi2) * sin(phi3) + cos((*(body->d))) * sin((body->RA->value)) * (cos(phi3) * sin(phi1) + cos(phi1) * cos(phi2) * sin(phi3)) + cos((*(body->RA))) * cos((*(body->d))) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))));
                if ((sin((*(body->d))) * sin(phi2) * sin(phi3) + cos((*(body->d))) * sin((body->RA->value)) * (cos(phi3) * sin(phi1) + cos(phi1) * cos(phi2) * sin(phi3)) + cos((*(body->RA))) * cos((*(body->d))) * (cos(phi1) * cos(phi3) - cos(phi2) * sin(phi1) * sin(phi3))) < 0.0) {
                    GHA_tab[l - l_min] += M_PI;
                }
                GHA_tab[l - l_min] = GHA_tab[l - l_min] - 2.0 * M_PI * floor(GHA_tab[l - l_min] / (2.0 * M_PI));


                MJD_tab[l - l_min] = ((double)(l - l_min)) / n_lines_per_day_ephemerides;

            }

            file.close(new_prefix);



            //remove discontinuous jumps in GHA to allow for interpolation
            for (sum = 0.0, l = 0; l < n_lines_ephemerides - 1; l++) {
                //cout << GHA_tab[l] << " " << d_tab[l] << " " << r_tab[l] << "\n";
                if (((GHA_tab[l] - sum) > M_PI) && (GHA_tab[l + 1] < M_PI)) {
                    sum += 2.0 * M_PI;
                }
                GHA_tab[l + 1] += sum;
            }

            cout << new_prefix.value << "Read values:\n";
            for (l = 0; l < n_lines_ephemerides; l++) {
                cout << new_prefix.value << MJD_tab[l] << " \t\t" << GHA_tab[l] << "\t\t " << d_tab[l] << "\n";
            }

            if (gsl_spline_init(interpolation_GHA, MJD_tab, GHA_tab, (unsigned int)n_lines_ephemerides) != GSL_SUCCESS) { check &= false; }
            if (gsl_spline_init(interpolation_d, MJD_tab, d_tab, (unsigned int)n_lines_ephemerides) != GSL_SUCCESS) { check &= false; }


            if (gsl_spline_eval_e(interpolation_GHA, (time.MJD) - MJD_min - ((double)l_min) / n_lines_per_day_ephemerides, acc, &((circle_of_equal_altitude->reference_position->lambda).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (circle_of_equal_altitude->reference_position->lambda).normalize();
                (circle_of_equal_altitude->reference_position->lambda).print(String("GHA"), new_prefix, cout);
            }

            if (gsl_spline_eval_e(interpolation_d, (time.MJD) - MJD_min - ((double)l_min) / n_lines_per_day_ephemerides, acc, &((circle_of_equal_altitude->reference_position->phi).value)) != GSL_SUCCESS) {
                check &= false;
            }
            else {
                (circle_of_equal_altitude->reference_position->phi).normalize();
                (circle_of_equal_altitude->reference_position->phi).print(String("d"), new_prefix, cout);
            }

        }

        //set the length of the circle of equal altitude
        //circle_of_equal_altitude->l.set(String("length of circle of equal altitude"), 2.0*M_PI*Re*sin(circle_of_equal_altitude->omega.value), new_prefix);

    }
    else {

        check &= false;
    }

    if (!check) {

        cout << prefix.value << RED << "Cannot fetch ephemerides' data!\n" << RESET;

    }
    else {

        cout << prefix.value << "...done\n";

    }

    gsl_interp_accel_free(acc);
    gsl_spline_free(interpolation_GHA);
    gsl_spline_free(interpolation_d);

    return check;

}

