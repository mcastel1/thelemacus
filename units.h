//
//  units.h
//  minimal_cocoa
//
//  Created by Michele on 06/06/2024.
//

//one nautical mile in kilometers: [nm] = nm_to_km * [km] where [nm] and [km] denote the units of nautical miles and kilometers, respectively, and `nm_to_km` the constant below.
#define nm_to_km 1.852
//one nautical mile in feet: [nm] = nm_to_ft [ft]
//[ft] = 0.3048 [m]
//[nm] = nm_to_km [km] = nm_to_km 1000 [m] = nm_to_km 1000 / 0.3048 [ft]
#define nm_to_ft (nm_to_km * (1e3) / 0.3048)
//[nm] = nm_to_km [km] = nm_to_km 1000 [m]
#define nm_to_m ((1e3)*nm_to_km)
//one inch in kilometers: [inch] = inch_to_km [km]
#define inch_to_km (25.4e-6)

