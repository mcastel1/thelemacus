reset 

#notes:
#to kill all gnuplot processes type: pkill -9 gnuplot
#- using webgeocals improves agreement between circle of equal altitude and true position by ~ 100 m
#do not use stellarium to compute hour angle and declination: they are calculated with the reference frame sitting on the surface of the earth at the observer's location! 
#here 0 <= lambda < 360.0, and it grows in the W direction
#notes
K = 360.0/(2.0*pi);
k = 1.0/K;
N = 6.0;
#circles of equal altitude are plotted with S points
S = 1e4;
#plot_coastline_every
epsilon=1e-10
myint(x) = x>0.0 ? int(x) : int(x)-1.0
clint(x) = abs(x-myint(x))<abs(x-(myint(x)+1.)) ? myint(x) : myint(x)+1.
#extracts degrees and minutes from floating-point angle in degree format
degrees(x) = floor(x);
minutes(x) = (x > 0.0 ? (x - floor(x))*60.0 : (1.0 - (x - floor(x)))*60.0)
is_int(x) = ((( abs(x - floor(x)) < epsilon ) || ( abs(x - ceil(x)) < epsilon)) ? 1 : 0)
key_y = 0.98;
key_x = 0.07;
key_spacing = 0.04;


#Mercator for the sphere
#MINUS SIGN ADDED BY HAND TO FLIP THE MAP
xe(lambda) =  (-1.0) * ( ((0.0 <= lambda) & (lambda < 180.0)) ? lambda: lambda - 360.0 )*k
ye(phi) = (log(1./cos(phi*k) + tan(phi*k)))
lambda_inv(xe) = ( ((xe <= 0.0) & (xe > -pi)) ? -xe*K: -xe*K + 360.0 )
#given xe, this returns the corresponding lambda in non-standard format, i.e., lying in the interval [-pi, pi)
lambda_inv_pm(xe) = -xe*K 
phi_inv(ye) = K*atan(sinh(ye))	
set size ratio -1


plot   '/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt' u (xe($2)):(ye($1))  w d linecolor rgb "black" noti
