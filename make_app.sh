#!/bin/sh

#  make_app.sh
#  wxcocoa
#
#  Created by MacBook Pro on 08/03/2023.
#

clear; clear;

#the name of the app that you want to create
APP_NAME='Thelemacus'
APP_VERSION='1.0'
#the path to the icon of the app
ICON_PATH='/Users/macbookpro/Documents/thelemacus/Contents/Resources/Images/app_icon.icns'
#the path to the .cpp file
INPUT_PATH='/Users/macbookpro/Documents/thelemacus'
OUTPUT_PATH='/Users/macbookpro/Desktop'
APP_LIBRARY_DIRECTORY=$OUTPUT_PATH'/'$APP_NAME'.app/Contents/Resources/Libraries'
WXWIDGETS_ROOT_DIRECTORY='/Applications/wxWidgets-3.2.6'
WXWIDGETS_LIB_DIRECTORY=$WXWIDGETS_ROOT_DIRECTORY'/my_build/lib'
BOOST_INCLUDE_DIRECTORY='/Applications/boost_1_83_0/'
BOOST_LIB_DIRECTORY='/Applications/boost_1_83_0/stage/lib'
LIBPNG_LIB_DIRECTORY='/usr/local/Cellar/libpng/1.6.44/lib'
LIBJPEG_LIB_DIRECTORY='/usr/local/opt/jpeg/lib'
LIBPCRE2_LIB_DIRECTORY='/usr/local/opt/pcre2/lib'
LIBTIFF_LIB_DIRECTORY='/usr/local/opt/libtiff/lib'
LIBICONV_LIB_DIRECTORY='/usr/local/Cellar/libiconv/1.17/lib'
LIBCURL_LIB_DIRECTORY='/usr/local/Cellar/curl/8.5.0/lib'
LIBZ_LIB_DIRECTORY='/usr/local/Cellar/zlib/1.3.1/lib'
LIBGSL_LIB_DIRECTORY='/usr/local/lib'
LIST_LIBRARIES_TO_COPY=''



rm -rf $OUTPUT_PATH/$APP_NAME.app $APP_NAME

mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries


#compile Thelemacus
g++ main.cpp src/*.cpp -o $APP_NAME  `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/boost_1_83_0 -L/Applications/boost_1_83_0/stage/lib/ -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath /Applications/boost_1_83_0/stage/lib -I$INPUT_PATH/include -I$INPUT_PATH/src


#set links fo $APP_NAME
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $APP_NAME
install_name_tool -change /usr/lib/libz.1.dylib @rpath/libz.1.dylib $APP_NAME
install_name_tool -change $BOOST_LIB_DIRECTORY/libboost_filesystem.dylib @rpath/libboost_filesystem.dylib $APP_NAME
install_name_tool -change $BOOST_LIB_DIRECTORY/libboost_system.dylib @rpath/libboost_system.dylib $APP_NAME
install_name_tool -change $LIBPNG_LIB_DIRECTORY/libpng16.16.dylib @rpath/libpng16.16.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.2.0.3.0.dylib @rpath/libwx_osx_cocoau_xrc-3.2.0.3.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.0.3.0.dylib @rpath/libwx_osx_cocoau_html-3.2.0.3.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.2.0.3.0.dylib @rpath/libwx_osx_cocoau_qa-3.2.0.3.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.3.0.dylib @rpath/libwx_osx_cocoau_core-3.2.0.3.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.3.0.0.0.dylib @rpath/libwx_baseu_xml-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.3.0.0.0.dylib @rpath/libwx_baseu_net-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.3.0.0.0.dylib @rpath/libwx_baseu-3.3.0.0.0.dylib $APP_NAME
install_name_tool -change $LIBGSL_LIB_DIRECTORY/libgsl.28.dylib @rpath/libgsl.28.dylib $APP_NAME


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
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.2.0.3.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.0.3.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.2.0.3.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.3.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.3.0.0.0.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBICONV_LIB_DIRECTORY/libiconv.2.dylib
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBGSL_LIB_DIRECTORY/libgsl.28.dylib
#copy the list
cp $LIST_LIBRARIES_TO_COPY $APP_LIBRARY_DIRECTORY



##libraries called by libpng16.16.dylib
LIB_A='libpng16.16.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_xrc-3.2.0.3.0.dylib
LIB_A='libwx_osx_cocoau_xrc-3.2.0.3.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_osx_cocoau_html-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_html-3.2.0.3.0.dylib
LIB_A='libwx_osx_cocoau_html-3.2.0.3.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_osx_cocoau_html-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_baseu-3.3.0.0.0.dylib
LIB_A='libwx_baseu-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_qa-3.2.0.3.0.dylib
LIB_A='libwx_osx_cocoau_qa-3.2.0.3.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_osx_cocoau_html-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_core-3.2.0.3.0.dylib
LIB_A='libwx_osx_cocoau_core-3.2.0.3.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_osx_cocoau_html-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_osx_cocoau_html-3.2.0.3.0.dylib
LIB_A='libwx_osx_cocoau_html-3.2.0.3.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
#I comment this out otherwise I get an error when running this script (this path is already present so I get a dublicate error if I include it)
#install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_baseu_xml-3.3.0.0.0.dylib
LIB_A='libwx_baseu_xml-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu_xml-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_osx_cocoau_html-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A


#libraries called by libwx_baseu_net-3.3.0.0.0.dylib
LIB_A='libwx_baseu_net-3.3.0.0.0.dylib'
DIR_LIB_A=$APP_LIBRARY_DIRECTORY
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $DIR_LIB_A/$LIB_A
LIB_B='libwx_osx_cocoau_core-3.2.0.3.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libz.1.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libiconv.2.dylib'; DIR_LIB_B='/usr/lib'; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A
LIB_B='libwx_baseu-3.3.0.0.0.dylib'; DIR_LIB_B=$WXWIDGETS_LIB_DIRECTORY; install_name_tool -change $DIR_LIB_B/$LIB_B @rpath/$LIB_B $APP_LIBRARY_DIRECTORY/$LIB_A

#create the .app folder and subfolders
sed -e "s/\${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.wxwidgets.$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_EXECUTABLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_VERSION}/$APP_VERSION/" \
    -e "s/\${MACOSX_BUNDLE_INFO_STRING}/$APP_NAME version $APP_VERSION, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_LONG_VERSION_STRING}/3.2.0, (c) 2002-2022 wxWidgets/" \
    -e "s/\${MACOSX_BUNDLE_SHORT_VERSION_STRING}/3.2/" \
    $WXWIDGETS_ROOT_DIRECTORY/src/osx/carbon/Info.plist.in >$OUTPUT_PATH/$APP_NAME.app/Contents/Info.plist
/bin/echo "APPL????" >$OUTPUT_PATH/$APP_NAME.app/Contents/PkgInfo
ln -f $APP_NAME $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS/$APP_NAME
cp -f $ICON_PATH $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/wxmac.icns

#clean up
rm -rf $APP_NAME
