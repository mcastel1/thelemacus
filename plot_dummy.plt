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
#coastlines are plotted every M lines
M = 1e1;
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
set samples S
#increments in degrees
increment_phi = 20.0;
increment_lambda = 60.0;
list(start,end,increment)=system(sprintf("seq %g %g %g", start, increment, end))


#set tics scale 0


Phi(t, d, GHA, cH) =pi/2.-acos(cos(cH) * sin(d)-cos(d) * cos(t) * sin(cH))
Lambda(t, d, GHA, cH) = ( lambda = -(atan((-sin(GHA) * (cos(d)* cos(cH)+cos(t) * sin(d) * sin(cH))+cos(GHA) * sin(cH) * sin(t))/(cos(d) * cos(GHA) * cos(cH)+sin(cH)*  (cos(GHA) * cos(t) * sin(d)+sin(GHA)* sin(t)))) + (cos(d)* cos(GHA)* cos(cH) +  sin(cH) * (cos(GHA)* cos(t) * sin(d) + sin(GHA)  *sin(t)) >0.0 ? 0.0 : pi)) , lambda - 2.0*pi*floor(lambda/(2.0*pi)))





#angles are in degrees
lambda_min = 180.0 - epsilon
lambda_max = 180.0 + epsilon
phi_min = -80.0
phi_max = 80.0

set xrange [xe(lambda_min):xe(lambda_max)]
set yrange [ye(phi_min):ye(phi_max)]

dx = ye((phi_min+phi_max)/2.0+1./60.) -  ye((phi_min+phi_max)/2.0)

set style arrow 1 nohead ls 1 lw 1 linecolor rgb 'black'
set style arrow 2 nohead ls 1 lw 1 linecolor rgb 'gray'


label_rose(n) = sprintf("\\scalebox{0.3}{$\\color{mygray}{%d}$}",n)
label_deg(x) = sprintf("%.f\260   ", x - myint(x/360.0)*360.0)
#this prints the arcminutes only of angle x
label_min(x) = sprintf("%.f'", minutes(x) ) 
#if the arcminutes are zero, I print out only the degrees for clarity
label_deg_min(x) =  ( minutes(x) == 0.0 ? sprintf("%.f\260", degrees(x)) : sprintf("%.f\260 %.f'", degrees(x), minutes(x)) ) 


#GPS position
#lambda0 = 360.0 -(2.0 + 24./60. + 26.07/(60.**2.))
#phi0 = 48.0 + 51./60. + 19.63/(60.**2.)
#set object circle at  xe(lambda0),ye(phi0) radius char 1  fillcolor rgb 'red' fillstyle solid noborder

#coastlines
#plot   '/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv.csv' u (xe(-$1+360.0)):(ye($2)) every M w d linecolor rgb "gray" noti
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
	while(N*dlambda<lambda_span){
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
	while(N*dphi<phi_span){
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
					while(x + (dlambda + (i/10.0)*1.0/60.0)*k >= GPVAL_X_MIN){
				             #set custom-made minor xtics every tenths (i/10.0) of arcminutes (1.0/60.0) 
					     set arrow from first x + (dlambda + (i/10.0)*1.0/60.0)*k, graph 0 to first x + (dlambda + (i/10.0)*1.0/60.0)*k, graph mtics_size nohead  linecolor "blue";
					     i=i-1.0;
					}
			 	   
	}


	while(x <= GPVAL_X_MAX){
 
		lambda = lambda_inv(x);

		if(x >= GPVAL_X_MIN){

		          if(is_int(lambda) == 1){
		       		      set xtics add (label_deg(lambda) x);
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
		       		      set ytics add (label_deg(phi) ye(phi));
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
	#coastlines
	plot   '/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv.csv' u (xe(-$1+360.0)):(ye($2)) every M w d linecolor rgb "gray" noti

	#sight_plots
	#position_plots

	x_max_old = GPVAL_X_MAX;

	}


    	pause 1e-3      # waiting time in seconds
}