//
//  atmosphere.h
//  thelemacus
//
//  Created by Michele on 11/06/2024.
//

#ifndef atmosphere_h
#define atmosphere_h

#include <iostream>

#include <vector>

#include "length.h"

using namespace std;

class Length;

class Atmosphere{
    
public:
    
    unsigned int n_layers;
    double A, B, P_dry_0, alpha, beta, gamma, T0;
    vector<Length> /*heights delimiting the level of the US 1976 atmosphere model, see https://en.wikipedia.org/wiki/U.S._Standard_Atmosphere, expressed in units of LengthUnit_types[0] */h;
    vector<double> /*dT/dz, ecpressed in units of K / nm*/lambda, t;
    
    Atmosphere();
    void set(const Atmosphere&);
    void initialize(void);
    double T(Length), n(Length), dTdz(Length), dndz(Length);
    
};




#endif
