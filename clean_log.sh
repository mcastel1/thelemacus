#!/bin/bash

sed -n '/ld: Undefined symbols:/,$p' ~/Desktop/Build*.txt > ~/Desktop/output.dat
wc -l ~/Desktop/output.dat
