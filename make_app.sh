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
ICON_PATH='/Users/michelecastellana/Documents/thelemacus/Contents/Resources/Images/Light/jolly_roger.icns'
#the path to the .cpp file
INPUT_PATH='/Users/michelecastellana/Documents/thelemacus'
OUTPUT_PATH='/Users/michelecastellana/Desktop'
APP_LIBRARY_DIRECTORY=$OUTPUT_PATH'/'$APP_NAME'.app/Contents/Resources/Libraries'
WX_VERSION='3.2.4'
WXWIDGETS_LIB_DIRECTORY='/Applications/wxWidgets-3.2-beta/my_build/lib'
BOOST_INCLUDE_DIRECTORY='/Applications/boost_1_83_0/'
BOOST_LIB_DIRECTORY='/Applications/boost_1_83_0/stage/lib'
LIBPNG_LIB_DIRECTORY='/opt/homebrew/Cellar/libpng/1.6.44/lib'
LIBJPEG_LIB_DIRECTORY='/opt/homebrew/opt/jpeg/lib'
LIBPCRE2_LIB_DIRECTORY='/opt/homebrew/opt/pcre2/lib'
LIBTIFF_LIB_DIRECTORY='/opt/homebrew/opt/libtiff/lib'
LIBICONV_LIB_DIRECTORY='/usr/local/lib'
LIBCURL_LIB_DIRECTORY='/opt/homebrew/Cellar/curl/8.5.0/lib'
LIBZ_LIB_DIRECTORY='/opt/homebrew/Cellar/zlib/1.3.1/lib'
LIBGSL_LIB_DIRECTORY='/usr/local/lib'
LIST_LIBRARIES_TO_COPY=''

#export PATH="/Applications/wxWidgets-"$WX_VERSION"/build-cocoa-debug:$PATH"


rm -rf $OUTPUT_PATH/$APP_NAME.app $APP_NAME

mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries


#compile Thelemacus
#/Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/bk-deps g++ `wx-config --cxxflags` -std=gnu++11 -mmacosx-version-min=10.10 -c -O3 -o temp.o -I/Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib/wx/include/osx_cocoa-unicode-3.2 -I/usr/local/include/gsl/  -I../../../include -D_FILE_OFFSET_BITS=64  -D__WXOSX_COCOA__      -I/Users/michelecastellana/Documents/thelemacus/ -DWXUSINGDLL -I../../../samples/minimal/../../samples -Wall -Wundef -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -g -O0 -I$BOOST_INCLUDE_DIRECTORY -I/usr/local/Cellar/pcre2/10.39/include -fno-common  -fvisibility=hidden -fvisibility-inlines-hidden -dynamic -fPIC -DPIC   $INPUT_PATH/main.cpp
#g++ `wx-config --cxxflags --libs`  -std=gnu++11 -mmacosx-version-min=12.0 -o $APP_NAME  temp.o    -L/Applications/wxWidgets-$WX_VERSION/build-cocoa-debug/lib    -L$LIBPNG_LIB_DIRECTORY -L$BOOST_LIB_DIRECTORY -L$LIBPCRE2_LIB_DIRECTORY -L$LIBJPEG_LIB_DIRECTORY -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL -lpng -ljpeg -L$LIBTIFF_LIB_DIRECTORY -ltiff -framework WebKit  -lgsl -lcblas  -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox -framework System -framework OpenGL -framework Security -lpthread -liconv -lcurl -lpcre2-32  -lz -framework Security  -L/usr/local/bin  -lboost_filesystem -lboost_system
 g++ main.cpp src/*.cpp -o $APP_NAME  `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib -I$INPUT_PATH/include -I$INPUT_PATH/src


#set links fo $APP_NAME
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $APP_NAME
install_name_tool -change /usr/lib/libz.1.dylib @rpath/libz.1.dylib $APP_NAME
install_name_tool -change $BOOST_LIB_DIRECTORY/libboost_filesystem.dylib @rpath/libboost_filesystem.dylib $APP_NAME
install_name_tool -change $BOOST_LIB_DIRECTORY/libboost_system.dylib @rpath/libboost_system.dylib $APP_NAME
install_name_tool -change $LIBPNG_LIB_DIRECTORY/libpng16.16.dylib @rpath/libpng16.16.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.3.0.0.0.dylib @rpath/libwx_osx_cocoau_xrc-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY//libwx_osx_cocoau_html-3.3.0.0.0.dylib @rpath/libwx_osx_cocoau_html-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.3.0.0.0.dylib @rpath/libwx_osx_cocoau_html-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.3.0.0.0.dylib @rpath/libwx_osx_cocoau_qa-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.3.0.0.0.dylib @rpath/libwx_osx_cocoau_core-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.3.0.0.0.dylib @rpath/libwx_baseu_xml-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.3.0.0.0.dylib @rpath/libwx_baseu_net-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.3.0.0.0.dylib @rpath/libwx_baseu-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $LIBGSL_LIB_DIRECTORY/libgsl.27.dylib @rpath/libgsl.27.dylib $APP_NAME


##copy data and images
cp -r $INPUT_PATH/Contents/Resources/Data/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
cp -r $INPUT_PATH/Contents/Resources/Images/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images


#copy libraries
#libraries called by $APP_NAME
#built the list of the libraries to copy
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$BOOST_LIB_DIRECTORY/libboost_filesystem.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$BOOST_LIB_DIRECTORY/libboost_system.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$BOOST_LIB_DIRECTORY/libboost_atomic.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBPNG_LIB_DIRECTORY/libpng16.16.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBZ_LIB_DIRECTORY/libz.1.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY//libwx_osx_cocoau_html-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBICONV_LIB_DIRECTORY/libiconv.2.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBGSL_LIB_DIRECTORY/libgsl.27.dylib
#copy the list
cp $LIST_LIBRARIES_TO_COPY $APP_LIBRARY_DIRECTORY



##libraries called by libpng16.16.dylib
LIB_A='libpng16.16.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_xrc-3.3.0.0.0.dylib
LIB_A='libwx_osx_cocoau_xrc-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_html-3.3.0.0.0.dylib
LIB_A='libwx_osx_cocoau_html-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_baseu-3.3.0.0.0.dylib
LIB_A='libwx_baseu-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_qa-3.3.0.0.0.dylib
LIB_A='libwx_osx_cocoau_qa-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_core-3.3.0.0.0.dylib
LIB_A='libwx_osx_cocoau_core-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by /libwx_osx_cocoau_html-3.3.0.0.0.dylib
LIB_A='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_baseu_xml-3.3.0.0.0.dylib
LIB_A='libwx_baseu_xml-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='/libwx_osx_cocoau_html-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_baseu_net-3.3.0.0.0.dylib
LIB_A='libwx_baseu_net-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A

#create the .app folder and subfolders
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

#clean up
rm -rf $APP_NAME
