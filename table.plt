set table "output.dat"

#filename = "/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_block.csv"
filename = "input.txt"

p filename every  ::0:3:1:4 using (textVar=stringcolumn(1)) w table  
unset table
