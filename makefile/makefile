CXX=g++
CXXFLAGS=-I. -I/usr/local/lib/wx/include/osx_cocoa-unicode-3.1 -I/usr/local/include/wx-3.1 -D_FILE_OFFSET_BITS=64 -DWXUSINGDLL -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__ -L/usr/local/lib   -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL -lwx_osx_cocoau_xrc-3.1 -lwx_osx_cocoau_html-3.1 -lwx_osx_cocoau_qa-3.1 -lwx_osx_cocoau_core-3.1 -lwx_baseu_xml-3.1 -lwx_baseu_net-3.1 -lwx_baseu-3.1 -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -I/Applications/boost_1_66_0/ -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib -O3
DIR=/Users/macbookpro/Dropbox/sight_reduction_program/


main.o: lib.cpp main.cpp
	$(CXX) -o main.o  ${DIR}main.cpp $(CXXFLAGS)

clean:
	echo "I am cleaning everything!"
	rm -rf *.o
