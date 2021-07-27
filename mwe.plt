set parametric
set multiplot

set xrange [-1:1]
set yrange [-1:1]

p [0.:pi/2.] cos(t), sin(t) smo csp linecolor rgb "red" ti "A"
p  [pi:2.*pi] cos(t), sin(t) smo csp linecolor rgb "green" ti "B" 

unset multiplot
#refresh 

set multiplot
p [0.:pi/2.] cos(t), sin(t) smo csp linecolor rgb "red" ti "A"
p  [pi:2.*pi] cos(t), sin(t) smo csp linecolor rgb "green" ti "B" 
