#!/bin/bash
#fswatch -o -r ~/Desktop/ | xargs -n1 -I{} ./compile_fig...

clear
clear

 cp src/dummy.cpp src/$1.cpp
 cp include/dummy.h include/$1.h
