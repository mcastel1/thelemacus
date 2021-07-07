reset 
#set term epslatex  
#set output "chart4.tex"
#unset te
#set te x11

#notes:
#- using webgeocals improves agreement between circle of equal altitude and true position by ~ 100 m
#do not use stellarium to compute hour angle and declination: they are calculated with the reference frame sitting on the surface of the earth at the observer's location! 
#notes
K = 360.0/(2.0*pi);
k = 1.0/K;
N = 12.0;
#coastlines are plotted every S lines
S = 1e3;
M = 1e2;
myint(x) = x>0.0 ? int(x) : int(x)-1.0
clint(x) = abs(x-myint(x))<abs(x-(myint(x)+1.)) ? myint(x) : myint(x)+1.

#Mercator for the sphere
#MINUS SIGN ADDED BY HAND TO FLIP THE MAP
xe(lambda) =  (-1.0) * (lambda)*k
ye(phi) = (log(1./cos(phi*k) + tan(phi*k)))
lambda_inv(xe) = -xe*K
phi_inv(ye) = K*atan(sinh(ye))	

myf(x,y)=sprintf("%g\260 %g' %s , %g\260 %g' %s", myint(abs(phi_inv(y))), (abs(phi_inv(y)) - myint(abs(phi_inv(y))))*60.0, (phi_inv(y) > 0.0 ? 'N': 'S'), myint(abs(lambda_inv(x))), (abs(lambda_inv(x)) - myint(abs(lambda_inv(x))))*60.0, (lambda_inv(x) > 0.0 ? 'W': 'E'))
set mouse mouseformat function myf(x, y)
set mouse mouseformat 8


#clear
set parametric
set multiplot



set size ratio -1
set ticscale 3,1
set samples S
#increments in degrees
increment_phi = 20.0;
increment_lambda = 60.0;
list(start,end,increment)=system(sprintf("seq %g %g %g", start, increment, end))


#set tics scale 0
#here lambda is positive if W, negative if E


Phi(t, d, GHA, cH) =pi/2.-acos(cos(cH) * sin(d)-cos(d) * cos(t) * sin(cH))
Lambda(t, d, GHA, cH) = -(atan((-sin(GHA) * (cos(d)* cos(cH)+cos(t) * sin(d) * sin(cH))+cos(GHA) * sin(cH) * sin(t))/(cos(d) * cos(GHA) * cos(cH)+sin(cH)*  (cos(GHA) * cos(t) * sin(d)+sin(GHA)* sin(t)))) + (cos(d)* cos(GHA)* cos(cH) +  sin(cH) * (cos(GHA)* cos(t) * sin(d) + sin(GHA)  *sin(t)) >0.0 ? 0.0 :((-sin(GHA) * (cos(d)* cos(cH)+cos(t) * sin(d) * sin(cH))+cos(GHA) * sin(cH) * sin(t)) >0.0 ? pi : -pi)))



myeps=1e-6



lambda_min_deg = -180.0
lambda_min_min = 0.0
lambda_max_deg = 180.0
lambda_max_min = 0.0
phi_min_deg = -80.0
phi_min_min = 0.0
phi_max_deg = 80.0
phi_max_min = 00.0


phi_min = phi_min_deg + phi_min_min/60.
phi_max = phi_max_deg + phi_max_min/60.
lambda_min = lambda_min_deg + lambda_min_min/60.
lambda_max = lambda_max_deg + lambda_max_min/60.

#set x2tics 1e2
#set ytics 1e2
#set y2tics 1e2
 

dx = ye((phi_min+phi_max)/2.0+1./60.) -  ye((phi_min+phi_max)/2.0)
set style arrow 1 nohead ls 1 lw 1 linecolor rgb 'black'
set style arrow 2 nohead ls 1 lw 1 linecolor rgb 'gray'


set xrange [xe(lambda_max):xe(lambda_min)]
set yrange [ye(phi_min):ye(phi_max)]




label_rose(n) = sprintf("\\scalebox{0.3}{$\\color{mygray}{%d}$}",n)
#label_deg(n) = sprintf("\\scalebox{0.8}{$\\ang{%d}$}",n)
label_deg(x) = sprintf("%.f\260", x)
label_deg_min(m, n) = sprintf("$\\ang{%d} \\ang{;%d;}$",m,n)
#label_min(n) = sprintf("$\\ang{;%d;}$",n)
label_min(x) = sprintf("%.f'",x)

#lambda=lambda_min
#i=0
#load 'meridians_min.plt'



#phi=phi_min
#i=0
#load 'parallels_min.plt'


#phi=phi_min
#i=0
#load 'parallels_seconds.plt'

#lambda=lambda_min
#i=0
#load 'meridians_seconds.plt'

#Compass rose
#rho=xe(lambda_min)-xe(lambda_min+0.75) 
#drho = dx
#x0 = xe((lambda_min+lambda_max)/2.)
#y0 = ye((phi_min+phi_max)/2.)
#theta=1
#load 'rose.plt'


#GPS position
lambda0 = -(2.0 + 24./60. + 26.07/(60.**2.))
phi0 = 48.0 + 51./60. + 19.63/(60.**2.)
set object circle at  xe(lambda0),ye(phi0) radius char 1  fillcolor rgb 'red' fillstyle solid noborder

#coastlines
plot   '/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv.csv' u (xe(-$1)):(ye($2)) every M w d linecolor rgb "black" noti



#replot for [lambda in list(lambda_min,lambda_max,increment_lambda)] 'lat_min.dat' u (xe(lambda)):(ye($1)) noti w l linecolor rgb 'gray' lt 1,\
     for [phi in list(phi_min,phi_max,increment_phi)] 'lon_min.dat' u (xe($1)):(ye(phi)) noti w l linecolor rgb 'gray' lt 1



dummy_line

#unset parametric
unset multiplot

x_max_old = 0.0;

while(1){

	if(GPVAL_DATA_X_MAX!=x_max_old){


	unset arrow;
	

#	print "Recalculating tics ... ";

	lambda_min = lambda_inv(GPVAL_DATA_X_MAX);
	lambda_max = lambda_inv(GPVAL_DATA_X_MIN);
	
	phi_min = phi_inv(GPVAL_DATA_Y_MIN);
	phi_max = phi_inv(GPVAL_DATA_Y_MAX);

	set xrange [xe(lambda_max):xe(lambda_min)]
	set yrange [ye(phi_min):ye(phi_max)]


	phi_span = phi_max - phi_min;
	lambda_span = lambda_max - lambda_min;
	
#	print "lambda in [" , lambda_min , " , " , lambda_max , "]";
#	print "phi in [" , phi_min , " , " , phi_max , "]";

#	print "Determining dlambda ...";
	
	dlambda=1.0;
	while(N*dlambda<lambda_span){
#		print dlambda;
		if(dlambda == 1.0){dlambda = dlambda + 4.0;}
		else{dlambda = dlambda + 5.0;}
	}
	if(dlambda > 1.0){
		   if(dlambda == 5.0){dlambda = dlambda - 4.0;}
		   else{dlambda = dlambda - 5.0;}
	}
#	print "... dlambda = " , dlambda;

#	print "Determining dphi ...";
#	print "phi_span = ", phi_span;

	dphi=1.0;
	while(N*dphi<phi_span){
#		print dphi;
		if(dphi == 1.0){dphi = dphi + 4.0;}
		else{dphi = dphi + 5.0;}
	}
	if(dphi > 1.0){
		   if(dphi == 5.0){dphi = dphi - 4.0;}
		   else{dphi = dphi - 5.0;}
	}
#	print "... dphi = " , dphi;



#	print "Setting xtics at dlambda intervals:";

	set format x "";     
	set xtics ();
	lambda= (int(lambda_min/dlambda))*dlambda;
	while(lambda<lambda_max){
		set xtics add (label_deg(lambda) xe(lambda));
		set arrow from first xe(lambda), graph 0 to first xe(lambda), graph 1 nohead  linecolor "gray"

		
#		print lambda;
		lambda = lambda + dlambda;

	}

	#replot for [lambda in list((int(lambda_min/dlambda))*dlambda,lambda_max,dlambda)] 'lat.dat' u (xe(lambda)):(ye($1)) noti w l linecolor rgb 'gray' lt 1



 
	#if(dlambda == 1.0){
	#	print "Setting mxtics";
	#	set mxtics 60;
	#	show mxtics;
	#}


#	print "Setting ytics at dphi intervals:";
	
	set format y "";     
	set ytics ();
	phi = (int(phi_min/dphi))*dphi;
	while(phi<phi_max){
		set ytics add (label_deg(phi) ye(phi));
		set arrow from graph 0,first ye(phi) to graph 1, first ye(phi) nohead  linecolor "gray"
#		print phi;
		phi = phi + dphi;


	}

	#replot for [phi in list((int(phi_min/dphi))*dphi,phi_max,dphi)] 'lon.dat' u (xe($1)):(ye(phi)) noti w l linecolor rgb 'gray' lt 1


	refresh;
	x_max_old = GPVAL_DATA_X_MAX;

	}


    	pause 1e-1      # waiting time in seconds
}







