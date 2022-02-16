#!/bin/bash
clear; clear; 


rm sight_gui_app.cpp lib_app.h
sed 's/\/Users\/macbookpro\/Documents\/navigational_astronomy\/sight_reduction_program\/data/sight_gui_app.app\/Contents\/Resources/g' lib.h >> lib_app.h
sed 's/lib.h/lib_app.h/g' sight_gui.cpp >> sight_gui_app.cpp



/Applications/wxWidgets-3.1.5/bk-deps g++ -std=gnu++11 -mmacosx-version-min=10.10 -c -o sight_gui_app_sight_gui_app.o -I/Applications/wxWidgets-3.1.5/lib/wx/include/osx_cocoa-unicode-static-3.1 -I../../include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I.  -I./../../samples -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -O2 -fno-common -dynamic -fPIC -DPIC   -dynamic -fPIC -DPIC  `wx-config --cxxflags`  -I/usr/local/include/gsl/  ./sight_gui_app.cpp


g++ -std=gnu++11 -mmacosx-version-min=10.10 -o sight_gui_app  sight_gui_app_sight_gui_app.o    -L/Applications/wxWidgets-3.1.5/lib      -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lwx_osx_cocoau_core-3.1  -lwx_baseu-3.1    -lwxtiff-3.1 -lwxjpeg-3.1 -lwxpng-3.1  -framework WebKit  -lwxregexu-3.1  -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lz -framework Security -lpthread -liconv  -lz -framework Security -lpthread -liconv `wx-config --libs` -lgsl -lcblas

rm -rf sight_gui_app.app

mkdir -p sight_gui_app.app/Contents
mkdir -p sight_gui_app.app/Contents/MacOS
mkdir -p sight_gui_app.app/Contents/Resources

sed -e "s/IDENTIFIER/`echo . | sed -e 's,\.\./,,g' | sed -e 's,/,.,g'`/" \
        -e "s/EXECUTABLE/sight_gui_app/" \
        -e "s/VERSION/3.1.5/" \
        /Applications/wxWidgets-3.1.5/src/osx/carbon/Info.plist.in >sight_gui_app.app/Contents/Info.plist

/bin/echo "APPL????" >sight_gui_app.app/Contents/PkgInfo
ln -f sight_gui_app sight_gui_app.app/Contents/MacOS/sight_gui_app
cp -f jolly_roger.icns sight_gui_app.app/Contents/Resources/wxmac.icns
cp -r data/* sight_gui_app.app/Contents/Resources/

#rm lib_app.h sight_gui_app.cpp
