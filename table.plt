set table "output.dat"

#filename = "/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
filename = "input.txt"

line_min = 1
line_max = 2
block_min = 20
block_max = 233

p filename every  ::line_min:block_min:line_max:block_max u (stringcolumn(1)) w table  
unset table
