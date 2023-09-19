reset 
set terminal qt size 705,792;

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
n_intervals_tics = 6;
n_points_routes = 1000;
n_points_coastline = 200000;
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

myf(x,y)=sprintf("%g° %g' %s , %g° %g' %s", myint(abs(phi_inv(y))), (abs(phi_inv(y)) - myint(abs(phi_inv(y))))*60.0, (phi_inv(y) > 0.0 ? 'N': 'S'), myint(abs(lambda_inv_pm(x))), (abs(lambda_inv_pm(x)) - myint(abs(lambda_inv_pm(x))))*60.0, ((lambda_inv_pm(x) > 0.0) ? 'W': 'E'))
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
lambda_min = 170.20750000000001023181539494544;
lambda_max = 190.00016666666667219942610245198;
phi_min = 284.23166666666662649731733836234;
phi_max = 80.886666666666670266749861184508;

set xrange [xe(lambda_min):xe(lambda_max)]
set yrange [ye(phi_min):ye(phi_max)]

dx = ye((phi_min+phi_max)/2.0+1./60.) -  ye((phi_min+phi_max)/2.0)

set style arrow 1 nohead ls 1 lw 1 linecolor rgb 'black'
set style arrow 2 nohead ls 1 lw 1 linecolor rgb 'gray'


label_rose(n) = sprintf("\\scalebox{0.3}{$\\color{mygray}{%d}$}",n)
label_deg_latitude(x) = ( x == 0.0 ) ? sprintf("0°") : ( x > 0.0 ? sprintf("%.f° N", x - myint(x/360.0)*360.0) : sprintf("%.f° S", ((-x) - myint((-x)/360.0)*360.0)) )
label_deg_longitude(x) = ( x == 0.0 ) ? sprintf("0°") : ( x < 180.0 ? sprintf("%.f° W", x - myint(x/360.0)*360.0) : sprintf("%.f° E", 360.0 - (x - myint(x/360.0)*360.0)) )
#this prints the arcminutes only of angle x
label_min(x) = sprintf("%.f'", minutes(x) ) 
#if the arcminutes are zero, I print out only the degrees for clarity
label_deg_min(x) =  ( minutes(x) == 0.0 ? sprintf("%.f°", degrees(x)) : sprintf("%.f° %.f'", degrees(x), minutes(x)) )
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


	set key top right
plot [0.:2.*pi] "+" u (xe(K*lambda_cea(t, 5.9458048621362600272277632029727, 6.1026182720272093007451985613443, 1.1965015775636649308921732881572))) : (ye(K*phi_cea(t, 5.9458048621362600272277632029727, 6.1026182720272093007451985613443, 1.1965015775636649308921732881572))) w l dashtype 1 lt 1 ti "sun 2021-11-18 11:04:27 UTC TAI, MP",\
[0.:2.0949668887294916608254879974993 - epsilon] "+" u (xe(K*lambda_cea(t, 0.45963259516071180987850652854831, 4.6304851271552811553533501864877, 1.2547233420281749260993819916621))) : (ye(K*phi_cea(t, 0.45963259516071180987850652854831, 4.6304851271552811553533501864877, 1.2547233420281749260993819916621))) w l dashtype 2 lt 2 ti "moon 2021-11-22 20:10:15 UTC TAI, MP" ,\
[2.0949668887294916608254879974993 + epsilon:2.*pi] "+" u (xe(K*lambda_cea(t, 0.45963259516071180987850652854831, 4.6304851271552811553533501864877, 1.2547233420281749260993819916621))) : (ye(K*phi_cea(t, 0.45963259516071180987850652854831, 4.6304851271552811553533501864877, 1.2547233420281749260993819916621))) w l dashtype 2 lt 2 noti,\
[0.:2.2110503123438305372872036969056 - epsilon] "+" u (xe(K*lambda_cea(t, 0.45964246546832737694288084639993, 4.6571790867676545744302529783454, 1.2384329688916302369960931173409))) : (ye(K*phi_cea(t, 0.45964246546832737694288084639993, 4.6571790867676545744302529783454, 1.2384329688916302369960931173409))) w l dashtype 3 lt 3 ti "moon 2021-11-22 20:16:35 UTC TAI, MP" ,\
[2.2110503123438305372872036969056 + epsilon:2.*pi] "+" u (xe(K*lambda_cea(t, 0.45964246546832737694288084639993, 4.6571790867676545744302529783454, 1.2384329688916302369960931173409))) : (ye(K*phi_cea(t, 0.45964246546832737694288084639993, 4.6571790867676545744302529783454, 1.2384329688916302369960931173409))) w l dashtype 3 lt 3 noti,\
[0.:2.3561473623376998354217448650161 - epsilon] "+" u (xe(K*lambda_cea(t, 0.4596538628351821698103663038637, 4.6894927859929653735093779687304, 1.218103610124587232732551456138))) : (ye(K*phi_cea(t, 0.4596538628351821698103663038637, 4.6894927859929653735093779687304, 1.218103610124587232732551456138))) w l dashtype 4 lt 4 ti "moon 2021-11-22 20:24:15 UTC TAI, MP" ,\
[2.3561473623376998354217448650161 + epsilon:2.*pi] "+" u (xe(K*lambda_cea(t, 0.4596538628351821698103663038637, 4.6894927859929653735093779687304, 1.218103610124587232732551456138))) : (ye(K*phi_cea(t, 0.4596538628351821698103663038637, 4.6894927859929653735093779687304, 1.218103610124587232732551456138))) w l dashtype 4 lt 4 noti,\
[0.:2.4348252344487524290173041663365 - epsilon] "+" u (xe(K*lambda_cea(t, 0.45965971032051594624689982993004, 4.7067735719524836923710608971305, 1.2071966565555940498200016008923))) : (ye(K*phi_cea(t, 0.45965971032051594624689982993004, 4.7067735719524836923710608971305, 1.2071966565555940498200016008923))) w l dashtype 5 lt 5 ti "moon 2021-11-22 20:28:21 UTC TAI, MP" ,\
[2.4348252344487524290173041663365 + epsilon:2.*pi] "+" u (xe(K*lambda_cea(t, 0.45965971032051594624689982993004, 4.7067735719524836923710608971305, 1.2071966565555940498200016008923))) : (ye(K*phi_cea(t, 0.45965971032051594624689982993004, 4.7067735719524836923710608971305, 1.2071966565555940498200016008923))) w l dashtype 5 lt 5 noti,\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.36429428489997617290896414488088, 4.872465542318730413740013318602, 1.1682728814055294286333719355753))) : (ye(K*phi_cea(t, 0.36429428489997617290896414488088, 4.872465542318730413740013318602, 1.1682728814055294286333719355753))) w l dashtype 6 lt 6 ti "moon 2021-11-25 23:37:35 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.36414259295948320227509498181462, 4.8875938083653114318849475239404, 1.1582788506675369966103517072042))) : (ye(K*phi_cea(t, 0.36414259295948320227509498181462, 4.8875938083653114318849475239404, 1.1582788506675369966103517072042))) w l dashtype 7 lt 7 ti "moon 2021-11-25 23:41:010 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.36385867219106582393095550287399, 4.9158802414402433811346782022156, 1.1397743841303358269811951686279))) : (ye(K*phi_cea(t, 0.36385867219106582393095550287399, 4.9158802414402433811346782022156, 1.1397743841303358269811951686279))) w l dashtype 8 lt 8 ti "moon 2021-11-25 23:47:52 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.36378233012667338108769854443381, 4.9234796025048987999639393819962, 1.1346935478399537977622912876541))) : (ye(K*phi_cea(t, 0.36378233012667338108769854443381, 4.9234796025048987999639393819962, 1.1346935478399537977622912876541))) w l dashtype 9 lt 9 ti "moon 2021-11-25 23:49:40 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.36368969318462324169516364236188, 4.9326973578750763493872000253759, 1.1288027995639788869652875291649))) : (ye(K*phi_cea(t, 0.36368969318462324169516364236188, 4.9326973578750763493872000253759, 1.1288027995639788869652875291649))) w l dashtype 10 lt 10 ti "moon 2021-11-25 23:51:51 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.36313160549830530987591714620066, 4.9881448926199265514469516347162, 1.0928665663416565756449472246459))) : (ye(K*phi_cea(t, 0.36313160549830530987591714620066, 4.9881448926199265514469516347162, 1.0928665663416565756449472246459))) w l dashtype 11 lt 11 ti "moon 2021-11-26 00:04:59 UTC TAI, MP",\
[0.:3.3020157154938090116047533228993 - epsilon] "+" u (xe(K*lambda_cea(t, 5.9070713287650393752414856862742, 6.0530480482491935134703453513794, 1.2402020788664185335647971442086))) : (ye(K*phi_cea(t, 5.9070713287650393752414856862742, 6.0530480482491935134703453513794, 1.2402020788664185335647971442086))) w l dashtype 12 lt 12 ti "sun 2021-11-29 10:56:14 UTC TAI, MP" ,\
[3.3020157154938090116047533228993 + epsilon:2.*pi] "+" u (xe(K*lambda_cea(t, 5.9070713287650393752414856862742, 6.0530480482491935134703453513794, 1.2402020788664185335647971442086))) : (ye(K*phi_cea(t, 5.9070713287650393752414856862742, 6.0530480482491935134703453513794, 1.2402020788664185335647971442086))) w l dashtype 12 lt 12 noti,\
[0.:3.2826226895143446604663495236309 - epsilon] "+" u (xe(K*lambda_cea(t, 5.9070583080309058132684185693506, 6.0811118396807213315469198278151, 1.2370915053316604570454728673212))) : (ye(K*phi_cea(t, 5.9070583080309058132684185693506, 6.0811118396807213315469198278151, 1.2370915053316604570454728673212))) w l dashtype 13 lt 13 ti "sun 2021-11-29 11:02:40 UTC TAI, MP" ,\
[3.2826226895143446604663495236309 + epsilon:2.*pi] "+" u (xe(K*lambda_cea(t, 5.9070583080309058132684185693506, 6.0811118396807213315469198278151, 1.2370915053316604570454728673212))) : (ye(K*phi_cea(t, 5.9070583080309058132684185693506, 6.0811118396807213315469198278151, 1.2370915053316604570454728673212))) w l dashtype 13 lt 13 noti,\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34392250449859329552992903700215, 5.4165366475455298811425564053934, 0.83064121865521078902361296059098))) : (ye(K*phi_cea(t, 0.34392250449859329552992903700215, 5.4165366475455298811425564053934, 0.83064121865521078902361296059098))) w l dashtype 14 lt 14 ti "moon 2021-12-16 18:47:02 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34435984749136028248983620869694, 5.4586951484653507549182904767804, 0.80456622317250903186902633024147))) : (ye(K*phi_cea(t, 0.34435984749136028248983620869694, 5.4586951484653507549182904767804, 0.80456622317250903186902633024147))) w l dashtype 15 lt 15 ti "moon 2021-12-16 18:56:60 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34449646941021194734844357299153, 5.4718784045260822068712514010258, 0.79621717383402335066477917280281))) : (ye(K*phi_cea(t, 0.34449646941021194734844357299153, 5.4718784045260822068712514010258, 0.79621717383402335066477917280281))) w l dashtype 16 lt 16 ti "moon 2021-12-16 19:00:07 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34460820162342553629741814802401, 5.4826646738540949499451926385518, 0.78971128990343664177942173409974))) : (ye(K*phi_cea(t, 0.34460820162342553629741814802401, 5.4826646738540949499451926385518, 0.78971128990343664177942173409974))) w l dashtype 17 lt 17 ti "moon 2021-12-16 19:02:40 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34477462207411779804289153616992, 5.4987382780218387878790053946432, 0.77968003033643973331834331474965))) : (ye(K*phi_cea(t, 0.34477462207411779804289153616992, 5.4987382780218387878790053946432, 0.77968003033643973331834331474965))) w l dashtype 18 lt 18 ti "moon 2021-12-16 19:06:28 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34491541836954742183962707713363, 5.512344394287454818481819529552, 0.77165017211454833834238797862781))) : (ye(K*phi_cea(t, 0.34491541836954742183962707713363, 5.512344394287454818481819529552, 0.77165017211454833834238797862781))) w l dashtype 19 lt 19 ti "moon 2021-12-16 19:09:41 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34502552571166333894581157437642, 5.522989562958073861409502569586, 0.76523261337105319412899007147644))) : (ye(K*phi_cea(t, 0.34502552571166333894581157437642, 5.522989562958073861409502569586, 0.76523261337105319412899007147644))) w l dashtype 20 lt 20 ti "moon 2021-12-16 19:12:12 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34516619584237817619509769428987, 5.536595599672560652493302768562, 0.75726172904266131347128521156264))) : (ye(K*phi_cea(t, 0.34516619584237817619509769428987, 5.536595599672560652493302768562, 0.75726172904266131347128521156264))) w l dashtype 21 lt 21 ti "moon 2021-12-16 19:15:25 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34540218747170270408020087415935, 5.5594367215785531755045667523518, 0.74389711333796859626943387411302))) : (ye(K*phi_cea(t, 0.34540218747170270408020087415935, 5.5594367215785531755045667523518, 0.74389711333796859626943387411302))) w l dashtype 22 lt 22 ti "moon 2021-12-16 19:20:49 UTC TAI, 2022-01-26 17:21:22",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34554630625876942007224101871543, 5.5733951229774953972651019284967, 0.73571251813899385396666730230208))) : (ye(K*phi_cea(t, 0.34554630625876942007224101871543, 5.5733951229774953972651019284967, 0.73571251813899385396666730230208))) w l dashtype 23 lt 23 ti "moon 2021-12-16 19:24:07 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34567653123860059283245504957449, 5.5860140413491849997740246180911, 0.72844917167922096368215534312185))) : (ye(K*phi_cea(t, 0.34567653123860059283245504957449, 5.5860140413491849997740246180911, 0.72844917167922096368215534312185))) w l dashtype 24 lt 24 ti "moon 2021-12-16 19:27:06 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.34601889871155383504586211529386, 5.6192178273451389713954995386302, 0.70958171610843434606863411318045))) : (ye(K*phi_cea(t, 0.34601889871155383504586211529386, 5.6192178273451389713954995386302, 0.70958171610843434606863411318045))) w l dashtype 25 lt 25 ti "moon 2021-12-16 19:34:57 UTC TAI, PGP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.42830772294977287328876514038711, 5.0343160546136278554740783874877, 1.0170778626193057725402013602434))) : (ye(K*phi_cea(t, 0.42830772294977287328876514038711, 5.0343160546136278554740783874877, 1.0170778626193057725402013602434))) w l dashtype 26 lt 26 ti "moon 2021-12-21 21:23:30 UTC TAI, BGCB",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.4278944385538696604243114052224, 5.1041029584494310711306752637029, 0.97128445975979071924655272596283))) : (ye(K*phi_cea(t, 0.4278944385538696604243114052224, 5.1041029584494310711306752637029, 0.97128445975979071924655272596283))) w l dashtype 27 lt 27 ti "moon 2021-12-21 21:40:03 UTC TAI, BGCB",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.42773558010375717364226488825807, 5.1308091068704664294841677474324, 0.95396935058027476639352926213178))) : (ye(K*phi_cea(t, 0.42773558010375717364226488825807, 5.1308091068704664294841677474324, 0.95396935058027476639352926213178))) w l dashtype 28 lt 28 ti "moon 2021-12-21 21:46:23 UTC TAI, BGCB",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.42646565777505518468615264282562, 5.3420021180645536063025247131009, 0.81707965813358418571965557930525))) : (ye(K*phi_cea(t, 0.42646565777505518468615264282562, 5.3420021180645536063025247131009, 0.81707965813358418571965557930525))) w l dashtype 29 lt 29 ti "moon 2021-12-21 22:36:28 UTC TAI, BGCB",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39524959708966223015380592187284, 0.36913041613194275125664489678456, 0.56119663236333905409480848902604))) : (ye(K*phi_cea(t, 0.39524959708966223015380592187284, 0.36913041613194275125664489678456, 0.56119663236333905409480848902604))) w l dashtype 30 lt 30 ti "moon 2022-01-13 22:22:14 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39581212927640135879059357648657, 0.43897614769777243415305179041752, 0.59368044787904228343222712283023))) : (ye(K*phi_cea(t, 0.39581212927640135879059357648657, 0.43897614769777243415305179041752, 0.59368044787904228343222712283023))) w l dashtype 31 lt 31 ti "moon 2022-01-13 22:38:46 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39596768240995694476680455409223, 0.45833841503745453849205659935251, 0.60364878382648412546984673099359))) : (ye(K*phi_cea(t, 0.39596768240995694476680455409223, 0.45833841503745453849205659935251, 0.60364878382648412546984673099359))) w l dashtype 32 lt 32 ti "moon 2022-01-13 22:43:21 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39776413615071581908466669119662, 0.68349733781581367697555151607958, 0.72875817554161770317477930802852))) : (ye(K*phi_cea(t, 0.39776413615071581908466669119662, 0.68349733781581367697555151607958, 0.72875817554161770317477930802852))) w l dashtype 33 lt 33 ti "moon 2022-01-13 23:36:39 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39799445546451117650121886981651, 0.71257419365295537883042698013014, 0.74662480330791969329595758608775))) : (ye(K*phi_cea(t, 0.39799445546451117650121886981651, 0.71257419365295537883042698013014, 0.74662480330791969329595758608775))) w l dashtype 34 lt 34 ti "moon 2022-01-13 23:43:32 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39652683047838332974777131312294, 0.52811223469301893373994971625507, 0.63989655376110199380690346515621))) : (ye(K*phi_cea(t, 0.39652683047838332974777131312294, 0.52811223469301893373994971625507, 0.63989655376110199380690346515621))) w l dashtype 35 lt 35 ti "moon 2022-01-13 22:59:52 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39812088337193701770289067098929, 0.72855581706752103876567616680404, 0.75627898759766443159691107211984))) : (ye(K*phi_cea(t, 0.39812088337193701770289067098929, 0.72855581706752103876567616680404, 0.75627898759766443159691107211984))) w l dashtype 36 lt 36 ti "moon 2022-01-13 23:47:19 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39819824210205612136093122899183, 0.73834189088613288998175221422571, 0.762308035005968220865213424986))) : (ye(K*phi_cea(t, 0.39819824210205612136093122899183, 0.73834189088613288998175221422571, 0.762308035005968220865213424986))) w l dashtype 37 lt 37 ti "moon 2022-01-13 23:49:38 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39836561024083066984147194489196, 0.7595332379259822630501730600372, 0.77538930528668248420842701307265))) : (ye(K*phi_cea(t, 0.39836561024083066984147194489196, 0.7595332379259822630501730600372, 0.77538930528668248420842701307265))) w l dashtype 38 lt 38 ti "moon 2022-01-13 23:54:39 UTC TAI, MP",\
[0.:2.*pi] "+" u (xe(K*lambda_cea(t, 0.39959093114249594203002402537095, 0.91547288437450013987017882755026, 0.87367788154433712044522053474793))) : (ye(K*phi_cea(t, 0.39959093114249594203002402537095, 0.91547288437450013987017882755026, 0.87367788154433712044522053474793))) w l dashtype 39 lt 39 ti "moon 2022-01-14 00:31:34 UTC TAI, MP",\
	"+" u (xe(K*(6.2410583415053171307818047353067))):(ye(K*(0.85264552071863697335629694862291))) w p lw 2 lt 1 ti "Centre Sportif Maryse Hilsz (MH)",\
"+" u (xe(K*(6.241171014205057154811129294103))):(ye(K*(0.85268850862179457195111353939865))) w p lw 2 lt 2 ti "Baby Gym Campo (BGC)",\
"+" u (xe(K*(6.2411274295520877686271887796465))):(ye(K*(0.8526832726340385359975471146754))) w p lw 2 lt 3 ti "My Place (MP)"


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
