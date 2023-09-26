#!/bin/sh

clear; clear;

#the name of the app that you want to create
APP_NAME=Thelemacus

INPUT_CODE_DIR=/Users/macbookpro/Documents/sight_reduction_program
INPUT_DATA_DIR=$INPUT_CODE_DIR/data
INPUT_IMAGE_DIR=$INPUT_CODE_DIR/images

OUTPUT_CODE_DIR=/Users/macbookpro/Desktop
OUTPUT_DATA_DIR=$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources/Data
OUTPUT_IMAGE_DIR=$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources/Images


rm -rf temp.o $OUTPUT_CODE_DIR/$APP_NAME.app $OUTPUT_CODE_DIR/$APP_NAME


/Applications/wxWidgets-3.2.2.1/build-cocoa-debug/bk-deps g++ `wx-config --cxxflags --libs` -std=gnu++11 -mmacosx-version-min=10.10 -c -O3 -o temp.o -I/Applications/wxWidgets-3.2.2.1/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-3.2 -I/usr/local/include/gsl/ -I/Applications/boost_1_66_0/ -I../../../include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I/Users/macbookpro/Dropbox/sight_reduction_program/ -DWXUSINGDLL -I../../../samples/minimal/../../samples -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -g -O0 -I/usr/local/Cellar/pcre2/10.39/include -fno-common  -fvisibility=hidden -fvisibility-inlines-hidden -dynamic -fPIC -DPIC   $INPUT_CODE_DIR/main.cpp
g++ `wx-config --cxxflags --libs`  -std=gnu++11 -mmacosx-version-min=12.0 -o $OUTPUT_CODE_DIR/$APP_NAME  temp.o    -L/Applications/wxWidgets-3.2.2.1/build-cocoa-debug/lib      -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lwx_osx_cocoau_core-3.2  -lwx_baseu-3.2        -lpng -lz -ljpeg -L/usr/local/Cellar/libtiff/4.3.0/lib -ltiff -framework WebKit  -lgsl -lcblas  -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL  -lz -framework Security -lpthread -liconv -lcurl -L/usr/local/Cellar/pcre2/10.39/lib -lpcre2-32  -lz -framework Security -lpthread -liconv -lcurl

mkdir -p $OUTPUT_CODE_DIR/$APP_NAME.app/Contents
mkdir -p $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources
mkdir -p $OUTPUT_DATA_DIR
mkdir -p $OUTPUT_IMAGE_DIR

sed -e 's#'$INPUT_DATA_DIR'#'$OUTPUT_DATA_DIR'#' \
    -e 's#'$INPUT_IMAGE_DIR'#'$OUTPUT_IMAGE_DIR'#' \
    -e 's#'$INPUT_CODE_DIR'#'$OUTPUT_CODE_DIR/$APP_NAME.app'#' \
    $INPUT_DATA_DIR/init.txt >$OUTPUT_DATA_DIR/init.txt


sed -e "s/\${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.wxwidgets.$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_EXECUTABLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_VERSION}/3.2.0/" \
    -e "s/\${MACOSX_BUNDLE_INFO_STRING}/$APP_NAME version 3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_LONG_VERSION_STRING}/3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_SHORT_VERSION_STRING}/3.2/" \
    /Applications/wxWidgets-3.2.2.1/src/osx/carbon/Info.plist.in >$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Info.plist
/bin/echo "APPL????" >$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/PkgInfo
ln -f $OUTPUT_CODE_DIR/$APP_NAME $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/MacOS/$APP_NAME
cp -f $INPUT_IMAGE_DIR/jolly_roger.icns $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources/wxmac.icns

rm -rf temp.o $OUTPUT_CODE_DIR/$APP_NAME
