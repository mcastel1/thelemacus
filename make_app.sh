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
ICON_PATH='/Users/michele/Documents/sight_reduction_program/Contents/Resources/Images/Light/jolly_roger.icns'
#the path to the .cpp file
INPUT_PATH='/Users/michele/Documents/sight_reduction_program'
OUTPUT_PATH='/Users/michele/Desktop'
WX_VERSION='3.2.4'
LIBTIFF_VERSION='4.6.0'
WXWIDGETS_LIB_DIRECTORY='/Applications/wxWidgets-3.2.4/build-cocoa-debug/lib'
BOOST_INCLUDE_DIRECTORY='/Applications/boost_1_83_0/'
BOOST_LIB_DIRECTORY='/Applications/boost_1_83_0/stage/lib'
LIBPNG_LIB_DIRECTORY='/opt/homebrew/Cellar/libpng/1.6.40/lib'
LIBJPEG_LIB_DIRECTORY='/opt/homebrew/Cellar/jpeg/9f/lib'
LIBPCRE2_LIB_DIRECTORY='/opt/homebrew/Cellar/pcre2/10.42/lib'

export PATH="/Applications/wxWidgets-"$WX_VERSION"/build-cocoa-debug:$PATH"


rm -rf temp.o $OUTPUT_PATH/$APP_NAME.app $APP_NAME

mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries


#compile Thelemacus
/Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/bk-deps g++ `wx-config --cxxflags` -std=gnu++11 -mmacosx-version-min=10.10 -c -O3 -o temp.o -I/Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-3.2 -I/usr/local/include/gsl/  -I../../../include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I/Users/michele/Documents/sight_reduction_program/ -DWXUSINGDLL -I../../../samples/minimal/../../samples -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -g -O0 -I$BOOST_INCLUDE_DIRECTORY -I/usr/local/Cellar/pcre2/10.39/include -fno-common  -fvisibility=hidden -fvisibility-inlines-hidden -dynamic -fPIC -DPIC   $INPUT_PATH/main.cpp
g++ `wx-config --cxxflags --libs`  -std=gnu++11 -mmacosx-version-min=12.0 -o $APP_NAME  temp.o    -L/Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib    -L$LIBPNG_LIB_DIRECTORY -L$BOOST_LIB_DIRECTORY -L$LIBPCRE2_LIB_DIRECTORY -L$LIBJPEG_LIB_DIRECTORY -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL -lpng -ljpeg -L/opt/homebrew/Cellar/libtiff/$LIBTIFF_VERSION/lib -ltiff -framework WebKit  -lgsl -lcblas  -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL -framework Security -lpthread -liconv -lcurl -lpcre2-32  -lz -framework Security  -L/usr/local/bin  -lboost_filesystem -lboost_system
#compile wrapper
# g++ wrapper.cpp -o $APP_NAME -I/usr/local/include/  -Wall -Wno-c++11-extensions --std=c++17  -O3 -L/usr/local/bin  -lboost_filesystem -lboost_system


install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $APP_NAME
# /System/Library/Frameworks/IOKit.framework/Versions/A/IOKit  /System/Library/Frameworks/Carbon.framework/Versions/A/Carbon  /System/Library/Frameworks/Cocoa.framework/Versions/A/Cocoa  /System/Library/Frameworks/QuartzCore.framework/Versions/A/QuartzCore  /System/Library/Frameworks/AudioToolbox.framework/Versions/A/AudioToolbox     /System/Library/Frameworks/OpenGL.framework/Versions/A/OpenGL   /System/Library/Frameworks/WebKit.framework/Versions/A/WebKit  /System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/libBLAS.dylib   /System/Library/Frameworks/Security.framework/Versions/A/Security
install_name_tool -change /usr/local/lib/libgsl.27.dylib @rpath/libgsl.27.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.2.dylib @rpath/libwx_osx_cocoau_xrc-3.2.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.dylib @rpath/libwx_osx_cocoau_html-3.2.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.2.dylib @rpath/libwx_osx_cocoau_qa-3.2.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.dylib @rpath/libwx_osx_cocoau_core-3.2.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.2.dylib @rpath/libwx_baseu_xml-3.2.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.2.dylib @rpath/libwx_baseu_net-3.2.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.dylib @rpath/libwx_baseu-3.2.dylib $APP_NAME
install_name_tool -change /Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/libwx_osx_cocoau_core-3.2.0.2.2.dylib @rpath/libwx_osx_cocoau_core-3.2.0.2.2.dylib $APP_NAME
install_name_tool -change /Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/libwx_baseu-3.2.0.2.1.dylib @rpath/libwx_baseu-3.2.0.2.1.dylib $APP_NAME
install_name_tool -change $LIBPNG_LIB_DIRECTORY/libpng16.16.dylib @rpath/libpng16.16.dylib $APP_NAME
install_name_tool -change /usr/lib/libz.1.dylib @rpath/libz.1.dylib $APP_NAME
install_name_tool -change $LIBJPEG_LIB_DIRECTORY/libjpeg.9.dylib @rpath/libjpeg.9.dylib $APP_NAME
install_name_tool -change /usr/local/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $APP_NAME
install_name_tool -change /usr/lib/libiconv.2.dylib @rpath/libiconv.2.dylib $APP_NAME
install_name_tool -change /usr/lib/libcurl.4.dylib @rpath/libcurl.4.dylib $APP_NAME
install_name_tool -change $LIBPCRE2_LIB_DIRECTORY/libpcre2-32.0.dylib @rpath/libpcre2-32.0.dylib $APP_NAME
install_name_tool -change /usr/lib/libSystem.B.dylib @rpath/libSystem.B.dylib $APP_NAME
install_name_tool -change $BOOST_LIB_DIRECTORY/libboost_filesystem.dylib @rpath/libboost_filesystem.dylib $APP_NAME
install_name_tool -change $BOOST_LIB_DIRECTORY/libboost_system.dylib @rpath/libboost_system.dylib $APP_NAME
install_name_tool -change /usr/lib/libc++.1.dylib @rpath/libc++.1.dylib $APP_NAME




#copy data and images
cp -r $INPUT_PATH/Contents/Resources/Data/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
cp -r $INPUT_PATH/Contents/Resources/Images/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images

#copy libraries
#libraries for $APP_NAME
cp /usr/lib/dyld  $BOOST_LIB_DIRECTORY/libboost_filesystem.dylib  $BOOST_LIB_DIRECTORY/libboost_system.dylib  /usr/lib/libc++.1.dylib  /usr/lib/libSystem.B.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries

#libraries for main.o (removed /usr/lib/libc++.1.dylib /usr/lib/libSystem.B.dylib /usr/lib/dyld $BOOST_LIB_DIRECTORY/libboost_filesystem.dylib   $BOOST_LIB_DIRECTORY/libboost_system.dylib because they are alreday in the libraries for $APP_NAME)
cp  /System/Library/Frameworks/IOKit.framework/Versions/A/IOKit  /System/Library/Frameworks/Carbon.framework/Versions/A/Carbon  /System/Library/Frameworks/Cocoa.framework/Versions/A/Cocoa  /System/Library/Frameworks/QuartzCore.framework/Versions/A/QuartzCore  /System/Library/Frameworks/AudioToolbox.framework/Versions/A/AudioToolbox     /System/Library/Frameworks/OpenGL.framework/Versions/A/OpenGL  $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.2.dylib   $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.dylib   $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.2.dylib   $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.dylib   $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.2.dylib   $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.2.dylib   $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.dylib   /Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/libwx_osx_cocoau_core-3.2.0.2.2.dylib   /Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/libwx_baseu-3.2.0.2.1.dylib   $LIBPNG_LIB_DIRECTORY/libpng16.16.dylib   /usr/lib/libz.1.dylib   $LIBJPEG_LIB_DIRECTORY/libjpeg.9.dylib   $WXWIDGETS_LIB_DIRECTORY/libtiff.5.dylib   /System/Library/Frameworks/WebKit.framework/Versions/A/WebKit   $WXWIDGETS_LIB_DIRECTORY/libgsl.27.dylib   /System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks/vecLib.framework/Versions/A/libBLAS.dylib   /System/Library/Frameworks/Security.framework/Versions/A/Security   /usr/lib/libiconv.2.dylib   /usr/lib/libcurl.4.dylib   $LIBPCRE2_LIB_DIRECTORY/libpcre2-32.0.dylib  $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.0.2.2.dylib $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.2.2.dylib $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.2.0.2.2.dylib /usr/local/opt/xz/lib/liblzma.5.dylib  /usr/local/opt/libpng/lib/libpng16.16.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries


#libraries called by libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.2.2.dylib @rpath/libwx_osx_cocoau_core-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.0.2.2.dylib @rpath/libwx_osx_cocoau_html-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.2.0.2.2.dylib @rpath/libwx_baseu_xml-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @rpath/libpng16.16.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_xrc-3.2.dylib



#libraries called by libwx_osx_cocoau_html-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.2.2.dylib @rpath/libwx_osx_cocoau_core-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @rpath/libpng16.16.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.dylib


#libraries called by libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.2.2.dylib @rpath/libwx_osx_cocoau_core-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.2.0.2.2.dylib @rpath/libwx_baseu_xml-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @rpath/libpng16.16.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_qa-3.2.dylib

#libraries called by libwx_osx_cocoau_core-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @rpath/libpng16.16.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @rpath/libtiff.5.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.dylib

#libraries called by libwx_baseu-3.2.0.2.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.0.2.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.dylib @rpath/libwx_baseu-3.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.0.2.2.dylib

#libraries called by libwx_baseu_xml-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_xml-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_xml-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_xml-3.2.dylib

#libraries called by libwx_baseu_net-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_net-3.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_net-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib  $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_net-3.2.dylib

#libraries called by libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.2.2.dylib @rpath/libwx_osx_cocoau_core-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @rpath/libpng16.16.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_html-3.2.0.2.2.dylib



#libraries called by libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/libpng/lib/libpng16.16.dylib @rpath/libpng16.16.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/libtiff/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib

#libraries called by libwx_baseu_xml-3.2.0.2.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_xml-3.2.0.2.2.dylib
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib @rpath/libwx_baseu-3.2.0.2.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_xml-3.2.0.2.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu_xml-3.2.0.2.2.dylib


#libraries called by libtiff.5.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libtiff.5.dylib
install_name_tool -change /usr/local/opt/jpeg/lib/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libtiff.5.dylib

#libraries called by libwx_baseu-3.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.dylib
install_name_tool -change /usr/local/opt/xz/lib/liblzma.5.dylib @rpath/liblzma.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.dylib

#libraries called by libwx_baseu-3.2.0.2.1.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.0.2.1.dylib
install_name_tool -change $LIBPCRE2_LIB_DIRECTORY/libpcre2-32.0.dylib @rpath/libpcre2-32.0.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_baseu-3.2.0.2.1.dylib


#libraries called by libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change $LIBPNG_LIB_DIRECTORY/libpng16.16.dylib @rpath/libpng16.16.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change $LIBJPEG_LIB_DIRECTORY/libjpeg.9.dylib @rpath/libjpeg.9.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change /usr/local/lib/libtiff.5.dylib @rpath/libtiff.5.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change $LIBPCRE2_LIB_DIRECTORY/libpcre2-32.0.dylib @rpath/libpcre2-32.0.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib
install_name_tool -change /Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/libwx_baseu-3.2.0.2.1.dylib @rpath/libwx_baseu-3.2.0.2.1.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/libwx_osx_cocoau_core-3.2.0.2.2.dylib


#install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/CALLER_LIBRARY
#install_name_tool -change CALLED_LIBRARY @rpath/CALLED_LIBRARY $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries/CALLER_LIBRARY



#cp $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.1* $WXWIDGETS_LIB_DIRECTORY/libwx_baseu* $WXWIDGETS_LIB_DIRECTORY/libpng* /usr/local/lib/libjpeg*  /opt/homebrew/Cellar/libtiff/$LIBTIFF_VERSION/lib/*.* /usr/local/lib/libgsl* /usr/local/lib/libboost_filesystem* /usr/local/lib/libboost_system*  $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.dylib $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.2.dylib  $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.dylib $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries

sed -e "s/\${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.wxwidgets.$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_EXECUTABLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_VERSION}/3.2.0/" \
    -e "s/\${MACOSX_BUNDLE_INFO_STRING}/$APP_NAME version 3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_LONG_VERSION_STRING}/3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_SHORT_VERSION_STRING}/3.2/" \
    /Applications/wxWidgets-$WX_VERSION/src/osx/carbon/Info.plist.in >$OUTPUT_PATH/$APP_NAME.app/Contents/Info.plist
/bin/echo "APPL????" >$OUTPUT_PATH/$APP_NAME.app/Contents/PkgInfo
ln -f $APP_NAME $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS/$APP_NAME
cp -f $ICON_PATH $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/wxmac.icns

rm -rf temp.o $APP_NAME
