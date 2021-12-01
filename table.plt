set table "output.dat"

filename = "/Users/mcastellana/Documents/navigational_astronomy_large_files/coastlines_2/map_conv_blocked.csv"
#filename = "input.txt"

#. here data for a given lat and lon are grouped in a block. Put 'x' in place of blocks which are empty. The block size is variable. Every latitude has n correspnding values of the longitude. 
lat_min = 120
lat_max = 125
lon_min = 23
lon_max = 29
n = 360

#p filename every  ::line_min:block_min:line_max:block_max u (stringcolumn(1)) w table  
plot for [i=lat_min:lat_max] filename every :::(i*n+lon_min)::(i*n+lon_max) w table

unset table
