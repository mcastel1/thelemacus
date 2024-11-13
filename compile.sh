#!/bin/bash

clear
clear


rm ~/Desktop/t.dat ~/Desktop/errors.dat;
g++ main.cpp src/*.cpp -o main.o `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17   -rpath /Applications/boost_1_83_0/stage/lib -I/Users/michele/Documents/sight_reduction_program/include  -ferror-limit=0 >& ~/Desktop/t.dat;
grep 'is a pointer' ~/Desktop/t.dat > ~/Desktop/log.dat
rm ~/Desktop/t.dat


