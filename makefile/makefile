CXX=g++
INPUT_DIR=/Users/macbookpro/Dropbox/sight_reduction_program/
OUTPUT_DIR=/Users/macbookpro/Dropbox/sight_reduction_program/
WX_DIR='/Applications/wxWidgets-3.2.2.1/'

CXXFLAGS=-I${INPUT_DIR} `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -I/Applications/boost_1_66_0/ -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib -O3

#the name of the app that you want to create
APP_NAME='Thelemacus'
#the path to the icon of the app
ICON_PATH='${INPUT_DIR}jolly_roger.icns'



main.o: lib.cpp main.cpp
	rm -rf temp.o ${APP_NAME}.app ${APP_NAME}
	$(CXX)  $(CXXFLAGS) -o ${OUTPUT_DIR}main.o  ${INPUT_DIR}main.cpp

clean:
	echo "I am cleaning everything!"
	rm -rf *.o
