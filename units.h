/*
 the following constants are unit-coversion constants: given a unit of measure [a] and a unit of measure [b], the constant a_to_b is defined by
 
    [a] = a_to_b * [b]
 
    We habe [b] = (1/a_to_b) * [a] == b_to_a * [a], thus
 
    b_to_a = 1/a_to_b .
 
    Example: [a] = [nm], [b] = [km],  [nm] = nm_to_km * [km] , where [nm] and [km] denote the units of nautical miles and kilometers, respectively
 
 */

#define nm_to_km 1.852
#define km_to_nm (1.0/nm_to_km)
//[ft] = 0.3048 [m]
//[nm] = nm_to_km [km] = nm_to_km 1000 [m] = nm_to_km 1000 / 0.3048 [ft]
#define nm_to_ft (nm_to_km * (1e3) / 0.3048)
#define ft_to_nm (1.0/nm_to_ft)
//[nm] = nm_to_km [km] = nm_to_km 1000 [m]
#define nm_to_m ((1e3)*nm_to_km)
#define m_to_nm (1.0/nm_to_m)
//one inch in kilometers: [inch] = inch_to_km [km]
#define inch_to_km (25.4e-6)
#define km_to_inch (1.0/inch_to_km)

