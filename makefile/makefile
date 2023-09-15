CXX=g++
INPUT_DIR='/Users/macbookpro/Dropbox/sight_reduction_program'
OUTPUT_DIR='/Users/macbookpro/Dropbox/sight_reduction_program'
WX_DIR='/Applications/wxWidgets-3.2.2.1'

CXXFLAGS=-I${INPUT_DIR} `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -I/Applications/boost_1_66_0/ -L/Applications/ChartDirector/lib  -Wall -Wno-c++11-extensions -lchartdir -rpath /Applications/ChartDirector/lib -O3 -std=gnu++11 -mmacosx-version-min=10.10 -I${WX_DIR}/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-3.2 -I/usr/local/include/gsl/ -I/Applications/boost_1_66_0/ -I../../../include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I/Users/macbookpro/Dropbox/sight_reduction_program/ -DWXUSINGDLL -I../../../samples/minimal/../../samples -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -g -O0 -I/usr/local/Cellar/pcre2/10.39/include -fno-common  -fvisibility=hidden -fvisibility-inlines-hidden -dynamic -fPIC -DPIC
#the name of the app that you want to create
APP_NAME='Thelemacus'
#the path to the icon of the app
ICON_PATH='${INPUT_DIR}jolly_roger.icns'



main.o: lib.cpp main.cpp
	clear; clear;
	rm -rf temp.o ${APP_NAME}.app ${APP_NAME}
	#$(CXX)  $(CXXFLAGS) -o ${OUTPUT_DIR}/main.o  ${INPUT_DIR}/main.cpp
	
	${WX_DIR}/build-cocoa-debug/bk-deps $(CXX) $(CXXFLAGS)  -c -o ${OUTPUT_DIR}/temp.o   ${OUTPUT_DIR}/main.cpp

clean:
	echo "I am cleaning everything!"
	rm -rf *.o
