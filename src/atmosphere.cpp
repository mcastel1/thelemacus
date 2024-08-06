//
//  atmosphere.cpp
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//



#include "atmosphere.h"

#include "constants.h"
#include "length.h"
#include "my_app.h"
#include "units.h"

Atmosphere::Atmosphere(void){}

//set the content of *this (not the memory adresses) equal to the content of x
void Atmosphere::set(const Atmosphere& x){
    
    n_layers = (x.n_layers);
    A = (x.A);
    B = (x.B);
    P_dry_0 = (x.P_dry_0);
    alpha = (x.alpha);
    beta = (x.beta);
    gamma = (x.gamma);
    T0 = (x.T0);
    h = (x.h);
    lambda = (x.lambda);
    t = (x.t);

}


double Atmosphere::T(Length z) {

    double x = 0.0;
    //cout << "z = " << (z.value) << "\n";

    if (z <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, check = true; (i < n_layers) && check; i++) {
            if ((z >= h[i]) && (z < h[i + 1])) {
                x = t[i] + lambda[i] * ((z - h[i]).convert(LengthUnit_types[0]).value);
                check = false;
            }
        }
        //cout << "i = " << i << "\n";

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return x;


}

double Atmosphere::dTdz(Length z) {

    double x = 0.0;
    //cout << "z = " << (z.value) << "\n";

    if (z <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, check = true; (i < n_layers) && check; i++) {
            if ((z >= h[i]) && (z < h[i + 1])) {
                x = lambda[i];
                check = false;
            }
        }

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return x;


}


double Atmosphere::n(Length z) {

    double x = 0.0;

    if (z <= h[n_layers]) {

        unsigned int i;
        bool check = true;

        for (i = 0, x = 0.0, check = true; (i < n_layers) && check; i++) {
            if ((z >= h[i]) && (z < h[i + 1])) {
                if (lambda[i] != 0.0) {
                    x -= B / lambda[i] * log((t[i] + lambda[i] * ((z - h[i]).convert(LengthUnit_types[0]).value)) / t[i]);
                }
                else {
                    x -= B * ((z - h[i]).convert(LengthUnit_types[0]).value) / t[i];
                }
                check = false;
            }
            else {
                if (lambda[i] != 0.0) {
                    x -= B / lambda[i] * log((t[i] + lambda[i] * ((h[i + 1] - h[i]).convert(LengthUnit_types[0]).value)) / t[i]);
                }
                else {
                    x -= B * ((h[i + 1] - h[i]).convert(LengthUnit_types[0]).value) / t[i];
                }
            }
        }

        /*
         int_0^z dz/(t_n+lambda_n*(z-h_n)) = log()
         */

    }
    else {

        cout << RED << "Value of z is not valid!\n" << RESET;
        x = -1.0;

    }

    return (A * P_dry_0 / T(z) * exp(x) / (1e6) + 1.0);


}

double Atmosphere::dndz(Length z) {

    return (-1.0 / T(z) * dTdz(z) * (n(z) - 1.0) - (n(z) - 1.0) * B / T(z));

}



void Atmosphere::initialize(void) {

    unsigned int i;
    double x;
    bool check;

    //cout << "Atmosphere model: US 1976.\n";

    n_layers = 7;
    A = 0.7933516713545163;
    B = 34.16 * nm_to_km;
    P_dry_0 = 101325.0;
    alpha = -6.5 * nm_to_km;
    beta = 2.8 * nm_to_km;
    gamma = -2.8 * nm_to_km;
    T0 = 288.15;

    h.resize(n_layers + 1);
    lambda.resize(n_layers);
    t.resize(n_layers);

    
    h[0] = 0.0;
    h[1] = 11.0 * km_to_nm;
    h[2] = 20.0 * km_to_nm;
    h[3] = 32.0 * km_to_nm;
    h[4] = 47.0 * km_to_nm;
    h[5] = 51.0 * km_to_nm;
    h[6] = 71.0 * km_to_nm;
    h[7] = 84.8520 * km_to_nm;

    lambda[0] = -6.5 * nm_to_km;
    lambda[1] = 0.0 * nm_to_km;
    lambda[2] = 1.0 * nm_to_km;
    lambda[3] = 2.8 * nm_to_km;
    lambda[4] = 0.0 * nm_to_km;
    lambda[5] = -2.8 * nm_to_km;
    lambda[6] = -2.0 * nm_to_km;


    for (i = 0, x = T0, check = true; (i < n_layers) && check; i++) {
        t[i] = x;
        x += lambda[i] * ((h[i + 1] - h[i]).convert(LengthUnit_types[0]).value);
    }


    /* for(int i=0; i<4+1; i++){ */
    /*   cout << "\t\t" << i << " " << h[i] << "\n"; */
    /* } */


}

