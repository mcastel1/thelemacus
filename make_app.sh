#!/bin/bash
clear; clear;

#rm lib_app.cpp main_app.cpp lib_app.h main_app.h
#cp lib.h lib_app.h


#sed 's/\/Users\/macbookpro\/Documents\/navigational_astronomy\/sight_reduction_program\/data/main_app.app\/Contents\/Resources/g' lib.h >> lib_app.h
#sed 's/lib.h/lib_app.h/g' main.cpp >> main_app.cpp

#add this if you want to delete warnings related to unusted variables
#-Wno-unused-variable

/Applications/wxWidgets-3.2.0/build-cocoa-debug/bk-deps g++ -std=gnu++11 -mmacosx-version-min=12.0 -c -o main_main.o -I/Applications/wxWidgets-3.2.0/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-3.2 -I/Applications/wxWidgets-3.2.0/include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I/Applications/wxWidgets-3.2.0/samples/main -DWXUSINGDLL -I/Applications/wxWidgets-3.2.0/samples/minimal/../../samples `wx-config --cxxflags --libs` -I/usr/local/include/gsl/ -I/Applications/ChartDirector/include -I/Applications/boost_1_66_0/ -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -g -O0 -I/usr/local/Cellar/pcre2/10.39/include -fno-common  -fvisibility=hidden -fvisibility-inlines-hidden -dynamic -fPIC -DPIC   main.cpp

g++ -std=gnu++11 -mmacosx-version-min=12.0 -o main  main_main.o   `wx-config --cxxflags --libs` -L/Applications/wxWidgets-3.2.0/build-cocoa-debug/lib  -L/Applications/ChartDirector/lib -lgsl -lcblas  -lchartdir  -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lwx_osx_cocoau_core-3.2  -lwx_baseu-3.2        -lpng -lz -ljpeg -L/usr/local/Cellar/libtiff/4.3.0/lib -ltiff -framework WebKit    -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lz -framework Security -lpthread -liconv -lcurl -L/usr/local/Cellar/pcre2/10.39/lib -lpcre2-32  -lz -framework Security -lpthread -liconv -lcurl -rpath /Applications/ChartDirector/lib

mkdir -p main.app/Contents
mkdir -p main.app/Contents/MacOS
mkdir -p main.app/Contents/Resources
sed -e "s/\${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.wxwidgets.main/" \
    -e "s/\${MACOSX_BUNDLE_EXECUTABLE_NAME}/main/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_NAME}/main/" \
    -e "s/\${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_VERSION}/3.2.0/" \
    -e "s/\${MACOSX_BUNDLE_INFO_STRING}/main version 3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_LONG_VERSION_STRING}/3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_SHORT_VERSION_STRING}/3.2/" \
    /Applications/wxWidgets-3.2.0/src/osx/carbon/Info.plist.in >main.app/Contents/Info.plist
/bin/echo "APPL????" >main.app/Contents/PkgInfo
ln -f main main.app/Contents/MacOS/main
cp -f /Applications/wxWidgets-3.2.0/src/osx/carbon/wxmac.icns main.app/Contents/Resources/wxmac.icns
