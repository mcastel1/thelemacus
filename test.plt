clear
K = 360.0/(2.0*pi);
k = 1.0/K;

set size ratio -1

xe(lambda) =  (-1.0) * ( ((0.0 <= lambda) & (lambda < 180.0)) ? lambda: lambda - 360.0 )*k
ye(phi) = (log(1./cos(phi*k) + tan(phi*k)))

M=1;

plot   '/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_1/map_conv.csv' u (xe(-$1+360.0)):(ye($2)) every M w p pt 21	linecolor rgb "gray" noti
