bin_PROGRAMS = main
main_SOURCES = main.cpp
main_CXXFLAGS = `wx-config --cxxflags` -I/usr/local/include/gsl/ -I/Applications/boost_1_66_0/  -Wall -Wno-c++11-extensions --std=c++17  -O3
main_LDFLAGS = -L/Applications/ChartDirector/lib -lgsl -lcblas `wx-config --libs`
