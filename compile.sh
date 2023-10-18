#!/bin/bash

clear
clear

#REMEMBER TO INCLUDE -O3 TO GET FAST PERFORMANCE !!!
   g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -lboost_filesystem -lboost_system -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions --std=c++17
 
 #g++ main.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -lboost_filesystem -lboost_system -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions --std=c++17  -O3
 
