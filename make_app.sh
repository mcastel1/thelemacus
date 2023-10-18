#!/bin/sh

#  make_app.sh
#  wxcocoa
#
#  Created by MacBook Pro on 08/03/2023.
#

clear; clear;

#the name of the app that you want to create
APP_NAME='Thelemacus'
#the path to the icon of the app
ICON_PATH='/Users/macbookpro/Documents/sight_reduction_program/jolly_roger.icns'
#the path to the .cpp file
INPUT_PATH='/Users/macbookpro/Documents/sight_reduction_program'
OUTPUT_PATH='/Users/macbookpro/Desktop'

rm -rf temp.o $OUTPUT_PATH/$APP_NAME.app $APP_NAME

mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries


#compile Thelemacus
/Applications/wxWidgets-3.2.2.1/build-cocoa-debug/bk-deps g++ `wx-config --cxxflags` -std=gnu++11 -mmacosx-version-min=10.10 -c -O3 -o temp.o -I/Applications/wxWidgets-3.2.2.1/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-3.2 -I/usr/local/include/gsl/  -I../../../include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I/Users/macbookpro/Documents/sight_reduction_program/ -DWXUSINGDLL -I../../../samples/minimal/../../samples -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -g -O0 -I/usr/local/Cellar/pcre2/10.39/include -fno-common  -fvisibility=hidden -fvisibility-inlines-hidden -dynamic -fPIC -DPIC   $INPUT_PATH/main.cpp
g++ `wx-config --cxxflags --libs`  -std=gnu++11 -mmacosx-version-min=12.0 -o $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS/main.o  temp.o    -L/Applications/wxWidgets-3.2.2.1/build-cocoa-debug/lib      -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lwx_osx_cocoau_core-3.2  -lwx_baseu-3.2        -lpng -lz -ljpeg -L/usr/local/Cellar/libtiff/4.3.0/lib -ltiff -framework WebKit  -lgsl -lcblas  -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lz -framework Security -lpthread -liconv -lcurl -lpcre2-32  -lz -framework Security -lpthread -liconv -lcurl -L/usr/local/bin  -lboost_filesystem -lboost_system
#compile wrapper
 g++ wrapper.cpp -o $APP_NAME -I/usr/local/include/  -Wall -Wno-c++11-extensions --std=c++17  -O3 -L/usr/local/bin  -lboost_filesystem -lboost_system




#copy data and images
cp -r $INPUT_PATH/Contents/Resources/Data/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
cp -r $INPUT_PATH/Contents/Resources/Images/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images

#copy libraries
cp /usr/local/lib/libwx_osx_cocoau_xrc-3.1* /usr/local/lib/libwx_baseu* /usr/local/lib/libpng* /usr/local/lib/libjpeg*  /usr/local/Cellar/libtiff/4.3.0/lib/*.* /usr/local/lib/libgsl* /usr/local/lib/libboost_filesystem* /usr/local/lib/libboost_system*  /usr/local/lib/libwx_osx_cocoau_html-3.1.dylib /usr/local/lib/libwx_osx_cocoau_qa-3.1.dylib  /usr/local/lib/libwx_osx_cocoau_core-3.1.dylib


 $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries

sed -e "s/\${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.wxwidgets.$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_EXECUTABLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_VERSION}/3.2.0/" \
    -e "s/\${MACOSX_BUNDLE_INFO_STRING}/$APP_NAME version 3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_LONG_VERSION_STRING}/3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_SHORT_VERSION_STRING}/3.2/" \
    /Applications/wxWidgets-3.2.2.1/src/osx/carbon/Info.plist.in >$OUTPUT_PATH/$APP_NAME.app/Contents/Info.plist
/bin/echo "APPL????" >$OUTPUT_PATH/$APP_NAME.app/Contents/PkgInfo
ln -f $APP_NAME $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS/$APP_NAME
cp -f $ICON_PATH $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/wxmac.icns

rm -rf temp.o $APP_NAME
