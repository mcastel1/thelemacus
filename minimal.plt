reset session

K = 360.0/(2.0*pi);
k = 1.0/K;

xe(lambda) =  (-1.0) * ( ((0.0 <= lambda) & (lambda < 180.0)) ? lambda: lambda - 360.0 )*k
ye(phi) = (log(1./cos(phi*k) + tan(phi*k)))

set parametric
set multiplot

PHI(t, d, GHA, omega) =pi/2.-acos(cos(omega) * sin(d)-cos(d) * cos(t) * sin(omega))
LAMBDA(t, d, GHA, omega) = ( lambda = -(atan((-sin(GHA) * (cos(d)* cos(omega)+cos(t) * sin(d) * sin(omega))+cos(GHA) * sin(omega) * sin(t))/(cos(d) * cos(GHA) * cos(omega)+sin(omega)*  (cos(GHA) * cos(t) * sin(d)+sin(GHA)* sin(t)))) + (cos(d)* cos(GHA)* cos(omega) +  sin(omega) * (cos(GHA)* cos(t) * sin(d) + sin(GHA)  *sin(t)) >0.0 ? 0.0 : pi)) , lambda - 2.0*pi*floor(lambda/(2.0*pi)))

lambda_min = 170;
lambda_max = 190;
phi_min = 310;
phi_max = 80;

set xrange [xe(lambda_min):xe(lambda_max)]
set yrange [ye(phi_min):ye(phi_max)]


plot [t=0   :2.09] "+" u (xe(K*LAMBDA(t, 0.46, 4.63, 1.25))):(ye(K*PHI(t, 0.46, 4.63, 1.25))) w l lc rgb "red"  ti "A" ,\
     [t=2.09:2*pi] "+" u (xe(K*LAMBDA(t, 0.46, 4.63, 1.25))):(ye(K*PHI(t, 0.46, 4.63, 1.25))) w l lc rgb "blue" ti "B"

