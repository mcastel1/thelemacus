#!/bin/bash

./compile.sh
./kill_gnuplot.sh
valgrind ./sight.o
