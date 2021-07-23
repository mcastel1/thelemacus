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
S = 1e3;
#coastlines are plotted every M lines
M = 1e2;
epsilon=1e-12
myint(x) = x>0.0 ? int(x) : int(x)-1.0
clint(x) = abs(x-myint(x))<abs(x-(myint(x)+1.)) ? myint(x) : myint(x)+1.
#extracts degrees and minutes from floating-point angle in degree format
degrees(x) = floor(x);
minutes(x) = (x - floor(x))*60.0;
is_int(x) = ((( abs(x - floor(x)) < epsilon ) || ( abs(x - ceil(x)) < epsilon)) ? 1 : 0)



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
set bmargin 6
set xtics rotate by 45 offset 0,graph -0.09
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
label_deg(x) = sprintf("%.f\260   ", x)
#this prints the arcminutes only of angle x
label_min(x) = sprintf("%.f'", minutes(x) ) 
#if the arcminutes are zero, I print out only the degrees for clarity
label_deg_min(x) =  ( minutes(x) == 0.0 ? sprintf("%.f\260", degrees(x)) : sprintf("%.f\260 %.f'", degrees(x), minutes(x)) ) 


#GPS position
lambda0 = 360.0 -(2.0 + 24./60. + 26.07/(60.**2.))
phi0 = 48.0 + 51./60. + 19.63/(60.**2.)
set object circle at  xe(lambda0),ye(phi0) radius char 1  fillcolor rgb 'red' fillstyle solid noborder

#coastlines
plot   '/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv.csv' u (xe(-$1+360.0)):(ye($2)) every M w d linecolor rgb "gray" noti

#sight_plots
#point_plots

#unset parametric
unset multiplot

x_max_old = 0.0;

while(1){

	if(GPVAL_DATA_X_MAX!=x_max_old){


	unset arrow;
	

	#print "Recalculating tics ... ";

	lambda_min = lambda_inv(GPVAL_DATA_X_MIN);
	lambda_max = lambda_inv(GPVAL_DATA_X_MAX);
	
	phi_min = phi_inv(GPVAL_DATA_Y_MIN);
	phi_max = phi_inv(GPVAL_DATA_Y_MAX);

	set xrange [xe(lambda_min):xe(lambda_max)]
	set yrange [ye(phi_min):ye(phi_max)]

	phi_span = phi_max - phi_min;
	lambda_span = -(-(GPVAL_DATA_X_MAX - GPVAL_DATA_X_MIN)*K);
	
	#print "lambda in [" , lambda_min , " , " , lambda_max , "]";
	#print "phi in [" , phi_min , " , " , phi_max , "]";

	#print "x in [" , GPVAL_DATA_X_MIN , " , " , GPVAL_DATA_X_MAX , "]";
	#print "y in [" , GPVAL_DATA_Y_MIN , " , " , GPVAL_DATA_Y_MAX , "]";


	#print "Determining dlambda ...";

	#gamma is the compression factor which allows from switching from increments in degrees to increments in arcminutes
	if(lambda_span > 1.0){gamma = 1.0;}
	else{gamma = 60.0;}

	dlambda=1.0/gamma;
	while(N*dlambda<lambda_span){
	#print dlambda;
	if(dlambda == 1.0/gamma){dlambda = dlambda + 4.0/gamma;}
	else{dlambda = dlambda + 5.0/gamma;}
	}
	if(dlambda > 1.0/gamma){
		   if(dlambda == 5.0/gamma){dlambda = dlambda - 4.0/gamma;}
		   else{dlambda = dlambda - 5.0/gamma;}
	}
	#print "... dlambda = " , dlambda;


	#print "Determining dphi ...";
	#print "phi_span = ", phi_span;

	#gamma is the compression factor which allows from switching from increments in degrees to increments in arcminutes
	if(phi_span > 1.0){gamma = 1.0;}
	else{gamma = 60.0;}
	
	dphi=1.0/gamma;
	while(N*dphi<phi_span){
		#print dphi;
		if(dphi == 1.0/gamma){dphi = dphi + 4.0/gamma;}
		else{dphi = dphi + 5.0/gamma;}
	}
	if(dphi > 1.0/gamma){
		   if(dphi == 5.0/gamma){dphi = dphi - 4.0/gamma;}
		   else{dphi = dphi - 5.0/gamma;}
	}
	#print "... dphi = " , dphi;



	#print "Setting xtics at dlambda intervals:";

	set format x "";     
	set xtics ();

	lambda= (int(lambda_min/dlambda))*dlambda;
	x = xe(lambda);
	while(x <= GPVAL_DATA_X_MAX){
	#I use the following identities for lambda and the fact that delta = floor(delta) to obtain a value of lambda which is exacly an 'integer multiple' of dllambda, so as to avoid xlabels of the form '10 degrees 0'
	#delta = (lambda - (int(lambda_min/dlambda))*dlambda)/dlambda;
	#delta = floor(delta) because delta is an integer, and lambda = delta * dlambda + int(lambda_min/dlambda)*dlambda = ( floor(delta) + int(lambda_min/dlambda) ) * dlambda =
	#( floor( (lambda_inv(x) - (int(lambda_min/dlambda))*dlambda)/dlambda ) + int(lambda_min/dlambda) ) * dlambda; 
 
		lambda = ( floor( (lambda_inv(x) - (int(lambda_min/dlambda))*dlambda)/dlambda ) + int(lambda_min/dlambda) ) * dlambda;

		if(x >= GPVAL_DATA_X_MIN){

		          if(is_int(lambda) == 1){
		       		      set xtics add (label_deg(lambda) x);
		          }else{
		       		      set xtics add (label_min(lambda) x);
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
	while(phi<=phi_max){
		if(phi >= phi_min){
		       if(is_int(phi) == 1){
		       		      set ytics add (label_deg(phi) ye(phi));
		       }else{
		       		      set ytics add (label_min(phi) ye(phi));
		       }
		       set arrow from graph 0,first ye(phi) to graph 1, first ye(phi) nohead  linecolor "gray";
		}
		#print phi;
		phi = phi + dphi;


	}



	refresh;
	x_max_old = GPVAL_DATA_X_MAX;

	}


    	pause 1e-3      # waiting time in seconds
}