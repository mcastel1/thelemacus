#!/bin/bash
#this script replaces all occurences of 'dummy' in dummy.cpp with $1 (the runtime flag) and write the result in src/$1.cpp and similarly for .h file
clear
clear

sed 's/dummy/'$1'/g' src/dummy.cpp >  src/$1.cpp
sed 's/dummy/'$1'/g' include/dummy.h >  include/$1.h
 
