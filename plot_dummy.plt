reset 
#window size

#notes:
#to kill all gnuplot processes type: pkill -9 gnuplot
#- using webgeocals improves agreement between circle of equal altitude and true position by ~ 100 m
#do not use stellarium to compute hour angle and declination: they are calculated with the reference frame sitting on the surface of the earth at the observer's location! 
#here 0 <= lambda < 360.0, and it grows in the W direction
#notes
#this command is such that when two points of a curve are both outisde the plotting window, the line that joins them is plotted
set clip two
K = 360.0/(2.0*pi);
k = 1.0/K;
#number of intervals for tics
#number of points for routes
#n_points_coastline
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

myf(x,y)=sprintf("%g\260 %g' %s , %g\260 %g' %s", myint(abs(phi_inv(y))), (abs(phi_inv(y)) - myint(abs(phi_inv(y))))*60.0, (phi_inv(y) > 0.0 ? 'N': 'S'), myint(abs(lambda_inv_pm(x))), (abs(lambda_inv_pm(x)) - myint(abs(lambda_inv_pm(x))))*60.0, ((lambda_inv_pm(x) > 0.0) ? 'W': 'E'))
set mouse mouseformat function myf(x, y)
set mouse mouseformat 8


#clear
set parametric
set multiplot

set size ratio -1
set key top left
#size of minor x and y tics with respect to screen size (1 = tic as long as screen size)
mtics_size = 0.01
set xtics rotate by 45 offset 0,graph -0.09

set format x "";     
set xtics ();

set format y "";     
set ytics ();

set ticscale 3,1
set samples n_points_routes
#increments in degrees
increment_phi = 20.0;
increment_lambda = 60.0;
list(start,end,increment)=system(sprintf("seq %g %g %g", start, increment, end))


#set tics scale 0

#latitude and longitude for a circle of equal altitude whose ground position has coordinates d, GHA, and whose angular aperture is omega
phi_cea(t, d, GHA, omega) =pi/2.-acos(cos(omega) * sin(d)-cos(d) * cos(t) * sin(omega))
lambda_cea(t, d, GHA, omega) = ( lambda = -(atan((-sin(GHA) * (cos(d)* cos(omega)+cos(t) * sin(d) * sin(omega))+cos(GHA) * sin(omega) * sin(t))/(cos(d) * cos(GHA) * cos(omega)+sin(omega)*  (cos(GHA) * cos(t) * sin(d)+sin(GHA)* sin(t)))) + (cos(d)* cos(GHA)* cos(omega) +  sin(omega) * (cos(GHA)* cos(t) * sin(d) + sin(GHA)  *sin(t)) >0.0 ? 0.0 : pi)) , lambda - 2.0*pi*floor(lambda/(2.0*pi)))

#latitude and longitude for an orthodrome whose starting point has coordinates d, GHA and whose initial heading is a
phi_lox(l, d, GHA, a, Re) = ( ( (( (0.0 <= a) && (a < pi/2.0) ) || ( (3.0*pi/2.0 <= a) && (a < 2.0*pi) )) ? tau = 1 : tau =-1 ), ( ((0.0 <= a) && (a < pi)) ? sigma = -1 : sigma = 1 ), C = (cos(a))**2.0, tt = -tau*sqrt((1.0-C)/C) * log( sqrt((1.0+sin(d))/(1.0-sin(d))) * tan( -tau*sqrt(C)*l/(2.0*Re) + atan(sqrt((1.0-sin(d))/(1.0+sin(d)))) ) ), asin( tanh( tau*sqrt(C/(1.0-C))*tt + atanh(sin(d)) ) ) )

lambda_lox(l, d, GHA, a, Re) = ( ( (( (0.0 <= a) && (a < pi/2.0) ) || ( (3.0*pi/2.0 <= a) && (a < 2.0*pi) )) ? tau = 1 : tau =-1 ), ( ((0.0 <= a) && (a < pi)) ? sigma = -1 : sigma = 1 ), C = (cos(a))**2.0, tt = -tau*sqrt((1.0-C)/C) * log( sqrt((1.0+sin(d))/(1.0-sin(d))) * tan( -tau*sqrt(C)*l/(2.0*Re) + atan(sqrt((1.0-sin(d))/(1.0+sin(d)))) ) ), lambda = GHA + sigma*tt, lambda - 2.0*pi*floor(lambda/(2.0*pi)) )


#latitude and longitude for an orthodrome whose starting point has coordinates d, GHA and whose initial heading is a
phi_ort(l, d, GHA, a, Re) = asin(cos(a) * cos(d) * sin(l/Re) + cos(l/Re) * sin(d))
lambda_ort(l, d, GHA, a, Re) = (lambda = - (atan((cos(GHA) * sin(l/Re) * sin(a) + sin(GHA) * (-cos(l/Re) * cos(d) +  cos(a) * sin(l/Re) * sin(d)))/( cos(l/Re) * cos(GHA) * cos(d) +  sin(l/Re) * (sin(a) * sin(GHA) -  cos(a) * cos(GHA) * sin(d)))) + ( ( cos(l/Re) * cos(GHA) * cos(d) + sin(l/Re) * (sin(a) * sin(GHA) - cos(a) * cos(GHA) * sin(d)) ) >= 0.0 ? 0.0 : pi ) ), lambda - 2.0*pi*floor(lambda/(2.0*pi)))

#angles are in degrees
#lambda_min is the angle which corresponds to the leftmost x coordinate x_min in the Mercator plot, and simiarly for lambda_max
#min_longitude
#max_longitude
#min_latitude
#max_latitude

set xrange [xe(lambda_min):xe(lambda_max)]
set yrange [ye(phi_min):ye(phi_max)]

dx = ye((phi_min+phi_max)/2.0+1./60.) -  ye((phi_min+phi_max)/2.0)

set style arrow 1 nohead ls 1 lw 1 linecolor rgb 'black'
set style arrow 2 nohead ls 1 lw 1 linecolor rgb 'gray'


label_rose(n) = sprintf("\\scalebox{0.3}{$\\color{mygray}{%d}$}",n)
label_deg_latitude(x) = ( x == 0.0 ) ? sprintf("0\260") : ( x > 0.0 ? sprintf("%.f\260 N", x - myint(x/360.0)*360.0) : sprintf("%.f\260 S", ((-x) - myint((-x)/360.0)*360.0)) )
label_deg_longitude(x) = ( x == 0.0 ) ? sprintf("0\260") : ( x < 180.0 ? sprintf("%.f\260 W", x - myint(x/360.0)*360.0) : sprintf("%.f\260 E", 360.0 - (x - myint(x/360.0)*360.0)) )
#this prints the arcminutes only of angle x
label_min(x) = sprintf("%.f'", minutes(x) ) 
#if the arcminutes are zero, I print out only the degrees for clarity
label_deg_min(x) =  ( minutes(x) == 0.0 ? sprintf("%.f\260", degrees(x)) : sprintf("%.f\260 %.f'", degrees(x), minutes(x)) )
#this function outputs angle x in the format which can be read from file from lib.h 
label_deg_min_c_lib(x) = sprintf("%.f° %.f'", degrees(x), minutes(x))


#GPS position
#lambda0 = 360.0 -(2.0 + 24./60. + 26.07/(60.**2.))
#phi0 = 48.0 + 51./60. + 19.63/(60.**2.)
#set object circle at  xe(lambda0),ye(phi0) radius char 1  fillcolor rgb 'red' fillstyle solid noborder

#coastlines
#	plot   '/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt' u (xe($2)):(ye($1)) w d linecolor rgb "gray" noti
p 0,0 w l noti

#unset parametric
unset multiplot

x_max_old = 0.0;

while(1){

	if(GPVAL_X_MAX!=x_max_old){


	unset arrow;
	set bmargin 6;
	

	#print "Recalculating tics ... ";

	lambda_min = lambda_inv(GPVAL_X_MIN);
	lambda_max = lambda_inv(GPVAL_X_MAX);
	
	phi_min = phi_inv(GPVAL_Y_MIN);
	phi_max = phi_inv(GPVAL_Y_MAX);

	set xrange [xe(lambda_min):xe(lambda_max)]
	set yrange [ye(phi_min):ye(phi_max)]

	phi_span = phi_max - phi_min;
	lambda_span = -(-(GPVAL_X_MAX - GPVAL_X_MIN)*K);
	
	#print "lambda in [" , lambda_min , " , " , lambda_max , "]";
	#print "phi in [" , phi_min , " , " , phi_max , "]";

	#print "x in [" , GPVAL_X_MIN , " , " , GPVAL_X_MAX , "]";
	#print "y in [" , GPVAL_Y_MIN , " , " , GPVAL_Y_MAX , "]";


	#print "Determining dlambda ...";

	#gamma_lambda is the compression factor which allows from switching from increments in degrees to increments in arcminutes
	if(lambda_span > 1.0){gamma_lambda = 1.0;}
	else{gamma_lambda = 60.0;}

	dlambda=1.0/gamma_lambda;
	while(n_intervals_tics*dlambda<lambda_span){
	#print dlambda;
	if(dlambda == 1.0/gamma_lambda){dlambda = dlambda + 4.0/gamma_lambda;}
	else{dlambda = dlambda + 5.0/gamma_lambda;}
	}
	if(dlambda > 1.0/gamma_lambda){
		   if(dlambda == 5.0/gamma_lambda){dlambda = dlambda - 4.0/gamma_lambda;}
		   else{dlambda = dlambda - 5.0/gamma_lambda;}
	}
	#print "... dlambda = " , dlambda;


	#print "Determining dphi ...";
	#print "phi_span = ", phi_span;

	#gamma_phi is the compression factor which allows from switching from increments in degrees to increments in arcminutes
	if(phi_span > 1.0){gamma_phi = 1.0;}
	else{gamma_phi = 60.0;}
	
	dphi=1.0/gamma_phi;
	while(n_intervals_tics*dphi<phi_span){
		#print dphi;
		if(dphi == 1.0/gamma_phi){dphi = dphi + 4.0/gamma_phi;}
		else{dphi = dphi + 5.0/gamma_phi;}
	}
	if(dphi > 1.0/gamma_phi){
		   if(dphi == 5.0/gamma_phi){dphi = dphi - 4.0/gamma_phi;}
		   else{dphi = dphi - 5.0/gamma_phi;}
	}
	#print "... dphi = " , dphi;



	#print "Setting xtics at dlambda intervals:";

	set format x "";     
	set xtics ();

	lambda= (int(lambda_min/dlambda))*dlambda;
	x = xe(lambda);

	#plot the first few custom-made myxics close to the left edge of the x axis. Here I consider 		x = x + dlambda*k (the first value of x which is an 'integer multiple of dlambda' and which is contained in the x axis), and then decrease it by one tenth of arcminute in the while loop
	if(gamma_lambda == 60.0){
					i=0.0;
					while(x + (dlambda + (i/10.0)*1.0/60.0)*k > GPVAL_X_MIN){
				             #set custom-made minor xtics every tenths (i/10.0) of arcminutes (1.0/60.0) 
					     set arrow from first x + (dlambda + (i/10.0)*1.0/60.0)*k, graph 0 to first x + (dlambda + (i/10.0)*1.0/60.0)*k, graph mtics_size nohead  linecolor "blue";
					     i=i-1.0;
					}
			 	   
	}


	while(x <= GPVAL_X_MAX){
 
		lambda = lambda_inv(x);

		if(x >= GPVAL_X_MIN){

		          if(is_int(lambda) == 1){
		       		      set xtics add (label_deg_longitude(lambda) x);
		          }else{
		       		      set xtics add (label_min(lambda) x);
		          }

			  if(gamma_lambda == 60.0){
			  	   i=0.0;
				   #plot the custom-made xtics from x to the next x (x + dlambda*k)
				   while(x + (i/10.0)*1.0/60.0*k <  x + dlambda*k){
					if(x + (i/10.0)*1.0/60.0*k <= GPVAL_X_MAX){
					     #set custom-made minor xtics every tenths (i/10.0) of arcminutes (1.0/60.0) 
					     set arrow from first x + (i/10.0)*1.0/60.0*k, graph 0 to first x + (i/10.0)*1.0/60.0*k, graph mtics_size nohead  linecolor "blue";
					}
					i=i+1.0;
			 	   }
		          }


			  set arrow from first x, graph 0 to first x, graph 1 nohead  linecolor "gray";


		}

		
		#print lambda_inv(x);
		x = x + dlambda*k;
		#lambda = lambda + dlambda;

	}




 
	#if(dlambda == 1.0){
	#	print "Setting mxtics";
	#	set mxtics 60;
	#	show mxtics;
	#}


	#print "Setting ytics at dphi intervals:";
	
	set format y "";     
	set ytics ();
	phi = (int(phi_min/dphi))*dphi;

	#plot the first few custom-made mytics close to the lower edge of the y axis. Here I consider phi + dphi (the first value of phi which is an 'integer multiple of dphi' and which is contained in the y axis), and then decrease it by one tenth of arcminute in the while loop
	if(gamma_phi == 60.0){
					i=0.0;
					while(ye(phi + dphi + (i/10.0)*1.0/60.0) >= GPVAL_Y_MIN){
						  #set custom-made minor ytics every tenths (i/10.0) of arcminutes (1.0/60.0) 
		       	 		       	  set arrow from graph 0,first ye(phi + dphi + (i/10.0)*1.0/60.0) to graph mtics_size, first ye(phi + dphi + (i/10.0)*1.0/60.0) nohead  linecolor "blue";
					i=i-1.0;
					}
			 	   
	}

	while(phi<=phi_max){
		if(phi >= phi_min){
		       if(is_int(phi) == 1){
		       		      set ytics add (label_deg_latitude(phi) ye(phi));
		       }else{
		       		      set ytics add (label_min(phi) ye(phi));
		       }

		       if(gamma_phi == 60.0){
			  	   i=0.0;
				   #plot the custom-made ytics from phi to the next phi (phi + dphi)
			 	   while(phi + (i/10.0)*1.0/60.0 < phi + dphi){
					if(phi + (i/10.0)*1.0/60.0 <= phi_max){
						#set custom-made minor ytics every tenths (i/10.0) of arcminutes (1.0/60.0) 
		       	 		       set arrow from graph 0,first ye(phi + (i/10.0)*1.0/60.0) to graph mtics_size, first ye(phi + (i/10.0)*1.0/60.0) nohead  linecolor "blue";
					}
					i=i+1.0;
			 	   }
		       }

		       set arrow from graph 0,first ye(phi) to graph 1, first ye(phi) nohead  linecolor "gray";
		}
		#print phi;
		phi = phi + dphi;


	}


	clear;

	set multiplot	
	#set the values lambda_min_get_coastline_data,  lambda_max_get_coastline_data which I will feed into get_coastline_data.cpp
	if((lambda_min < 180.0) && (lambda_max >= 180.0)){
		lambda_min_get_coastline_data = lambda_max - 360.0;
		lambda_max_get_coastline_data = lambda_min;
	}else{
		lambda_min_get_coastline_data = lambda_max;
		lambda_max_get_coastline_data = lambda_min;
	}

	#fetch coastline data in path_file_coastline_data_blocked, and produce file path_file_selected_coastline_data which contains n_points_coastline points of the coastline of the world
	system(sprintf("./get_coastline_data.o -p %g -P %g -l %g -L %g -N %g > /dev/null 2>&1", floor(phi_min), floor(phi_max), floor(lambda_min_get_coastline_data), floor(lambda_max_get_coastline_data), n_points_coastline));


	#route_plots
	#position_plots

	#coastlines
	number_of_lines_map_conv_selected = system("wc -l < /Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt")
	if(number_of_lines_map_conv_selected != 0){
			plot   '/Users/macbookpro/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_selected.txt' u (xe($2)):(ye($1)) w d linecolor rgb "black" noti
	}


	x_max_old = GPVAL_X_MAX;

        set print "boundary.txt";
	print "GPVAL_X_MIN = ", GPVAL_X_MIN, "\n", "GPVAL_X_MAX = ", GPVAL_X_MAX, "\n", "GPVAL_Y_MIN = ", GPVAL_Y_MIN, "\n", "GPVAL_Y_MAX = ", GPVAL_Y_MAX, "\n", "minimal longitude = ", label_deg_min_c_lib(lambda_min), "\n", "maximal longitude = ", label_deg_min_c_lib(lambda_max), "\n", "minimal latitude = ", label_deg_min_c_lib(phi_min), "\n", "maximal latitude = ", label_deg_min_c_lib(phi_max), "\n";
	unset print;
	system("chmod 777 boundary.txt");

	}


    	pause 1e-1      # waiting time in seconds
}
