#!/bin/sh

#  make_app.sh
#  wxcocoa
#
#  Created by MacBook Pro on 08/03/2023.
#

clear; clear;

COLOR='\033[0;32m'
NC='\033[0m' # No Color


#the name of the app that you want to create
APP_NAME='ToyThelemacus'
APP_VERSION='1.0'
WXWIDGETS_TO_REPLACE_DIRECTORY='/usr/local/lib'

#settings for old mac
# : <<'END'
USER_NAME='macbookpro'
WXWIDGETS_ROOT_DIRECTORY='/Applications/wxWidgets-3.2.6-release'
BOOST_NAME='boost_1_87_0'
WXWIDGETS_LIB_DIRECTORY=$WXWIDGETS_ROOT_DIRECTORY'/my_build/lib'
LIBWX_COCOAU_XRC_NAME='libwx_osx_cocoau_xrc-3.2.0.3.0.dylib'
LIBWX_COCOAU_HTML_NAME='libwx_osx_cocoau_html-3.2.0.3.0.dylib'
LIBWX_COCOAU_QA_NAME='libwx_osx_cocoau_qa-3.2.0.3.0.dylib'
LIBWX_COCOAU_CORE_NAME='libwx_osx_cocoau_core-3.2.0.3.0.dylib'
LIBWX_BASEU_NAME='libwx_baseu-3.2.0.3.0.dylib'
LIBWX_BASEU_XML_NAME='libwx_baseu_xml-3.2.0.3.0.dylib'
LIBWX_BASEU_NET_NAME='libwx_baseu_net-3.2.0.3.0.dylib'

LIBPNG_LIB_DIRECTORY='/usr/local/Cellar/libpng/1.6.44/lib'
LIBPNG_NAME='libpng16.16.dylib'

LIBJPEG_LIB_DIRECTORY='/usr/local/opt/jpeg-turbo/lib'
LIBJPEG_NAME='libjpeg.8.dylib'

LIBPCRE2_LIB_DIRECTORY='/usr/local/Cellar/pcre2/10.44/lib'
LIBPCRE2_NAME='libpcre2-32.0.dylib'

LIBTIFF_LIB_DIRECTORY='/usr/local/Cellar/libtiff/4.7.0/lib'
LIBTIFF_NAME='libtiff.6.dylib'

LIBICONV_LIB_DIRECTORY='/usr/local/Cellar/libiconv/1.17/lib'
LIBICONV_NAME='libiconv.2.dylib'

LIBCURL_LIB_DIRECTORY=' /usr/local/Cellar/curl/8.11.0_1/lib'
LIBCURL_NAME='libcurl.4.dylib'

LIBGSL_LIB_DIRECTORY='/usr/local/lib'
LIBGSL_NAME='libgsl.28.dylib'

LIBLZMA_LIB_DIRECTORY='/usr/local/Cellar/xz/5.6.3/lib'
LIBLZMA_NAME='liblzma.5.dylib'

LIBZSTD_LIB_DIRECTORY='/usr/local/opt/zstd/lib'
LIBZSTD_NAME='libzstd.1.dylib'
# END

#settings for new mac
: <<'END'
USER_NAME='michelecastellana'
WXWIDGETS_ROOT_DIRECTORY='/Applications/wxWidgets-3.2.6-release'
BOOST_NAME='boost_1_86_0'
WXWIDGETS_LIB_DIRECTORY=$WXWIDGETS_ROOT_DIRECTORY'/my_build/lib'
LIBWX_COCOAU_XRC_NAME='libwx_osx_cocoau_xrc-3.2.0.3.0.dylib'
LIBWX_COCOAU_HTML_NAME='libwx_osx_cocoau_html-3.2.0.3.0.dylib'
LIBWX_COCOAU_QA_NAME='libwx_osx_cocoau_qa-3.2.0.3.0.dylib'
LIBWX_COCOAU_CORE_NAME='libwx_osx_cocoau_core-3.2.0.3.0.dylib'
LIBWX_BASEU_NAME='libwx_baseu-3.2.0.3.0.dylib'
LIBWX_BASEU_XML_NAME='libwx_baseu_xml-3.2.0.3.0.dylib'
LIBWX_BASEU_NET_NAME='libwx_baseu_net-3.2.0.3.0.dylib'

LIBPNG_LIB_DIRECTORY='/opt/homebrew/Cellar/libpng/1.6.44/lib'
LIBPNG_NAME='libpng16.16.dylib'

LIBJPEG_LIB_DIRECTORY='/opt/homebrew/opt/jpeg-turbo/lib'
LIBJPEG_NAME='libjpeg.8.dylib'

LIBPCRE2_LIB_DIRECTORY='/opt/homebrew/Cellar/pcre2/10.44/lib'
LIBPCRE2_NAME='libpcre2-32.0.dylib'

LIBTIFF_LIB_DIRECTORY='/opt/homebrew/Cellar/libtiff/4.7.0/lib'
LIBTIFF_NAME='libtiff.6.dylib'

LIBICONV_LIB_DIRECTORY='/opt/homebrew/Cellar/libiconv/1.17/lib'
LIBICONV_NAME='libiconv.2.dylib'

LIBCURL_LIB_DIRECTORY=' /opt/homebrew/Cellar/curl/8.11.0_1/lib'
LIBCURL_NAME='libcurl.4.dylib'

LIBGSL_LIB_DIRECTORY='/usr/local/lib'
LIBGSL_NAME='libgsl.28.dylib'

LIBLZMA_LIB_DIRECTORY='/opt/homebrew/opt/xz/lib'
LIBLZMA_NAME='liblzma.5.dylib'

LIBZSTD_LIB_DIRECTORY='/opt/homebrew/opt/zstd/lib'
LIBZSTD_NAME='libzstd.1.dylib'
END






MINIMUM_SYSTEM_VERSION='10.13'
#the path to the .cpp file
INPUT_PATH='/Users/'$USER_NAME'/Documents/thelemacus'
MINIMAL_PATH="/Applications/wxWidgets-3.2.6-release/samples/minimal"
OUTPUT_PATH='/Users/'$USER_NAME'/Desktop'
EXECUTABLE_PATH=$OUTPUT_PATH/$APP_NAME'.app/Contents/MacOS'
#the path to the icon of the app
ICON_NAME='juventus-icon.icns'
ICON_PATH=$MINIMAL_PATH'/'$ICON_NAME
APP_LIBRARY_DIRECTORY=$OUTPUT_PATH'/'$APP_NAME'.app/Contents/Resources/Libraries'
BOOST_LIB_DIRECTORY='/Applications/'$BOOST_NAME'/universal'





#this function links library $1 = LIB_A with library $2 = LIB_B, where $LIB_A and $LIB_B are the names of the libraries including their extensiosn. For example: $LIB_B='libpng16.16.dylib', $LIB_A= 'libz.1.dylib'. The function finds the path with which $1 calls $2 and it replaces it with the correct one (@rpath/$2). Usage: to link correctly libpng16.16.dylib and libz.1.dylib, put in this .sh file `link_pair 'libpng16.16.dylib' 'libz.1.dylib' `
link_pair(){
    rm -f out.txt; otool -L $1 >> out.txt
    # echo 'LIB_A = ' $1
    # echo 'LIB_B = ' $2
    local PATH_TO_REPLACE=$(grep -o '.*'$2 out.txt | sed 's/'$2'$//' | sed 's/[[:space:]]//g')
    # echo 'path to replace = ' $PATH_TO_REPLACE
    install_name_tool -change $PATH_TO_REPLACE$2 @rpath/$2 $1
}

# Function to process the list and other arguments
link_list() {

  local source=$1
  echo "Source library:\n\t" $source

  # Capture the list (first argument)
  list=($2)

  # Process the list
  echo "Target libraries:"
  for item in "${list[@]}"; do
    echo "\t$item"
  done



  # Process the list
  for item in "${list[@]}"; do
    link_pair $source $item
  done


  echo "${COLOR}\n*********** Printing linkage ...${NC}"
  otool -L $1
  echo "${COLOR}*********** ... done.${NC}"

}


LIST_LIBRARIES_TO_COPY=''



rm -rf $OUTPUT_PATH/$APP_NAME.app $APP_NAME

mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images
mkdir -p $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Libraries

#compile Thelemacus
g++ $INPUT_PATH/*.cpp -o $APP_NAME  `wx-config --cxxflags --libs` -lgsl -lcblas -I/usr/local/include/gsl/ -L/usr/local/bin  -I/Applications/$BOOST_NAME -L$BOOST_LIB_DIRECTORY -lboost_filesystem -lboost_system   -Wall -Wno-c++11-extensions --std=c++17  -O3 -rpath $BOOST_LIB_DIRECTORY -I$INPUT_PATH/include -I$INPUT_PATH/src -I$MINIMAL_PATH/


##copy data and images
cp -r $INPUT_PATH/Contents/Resources/Data/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Data
cp -r $INPUT_PATH/Contents/Resources/Images/* $OUTPUT_PATH/$APP_NAME.app/Contents/Resources/Images

#copy libraries
#libraries called by $APP_NAME
#built the list of the libraries to copy
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$BOOST_LIB_DIRECTORY'/libboost_filesystem.dylib'
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$BOOST_LIB_DIRECTORY'/libboost_system.dylib'
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$BOOST_LIB_DIRECTORY'/libboost_atomic.dylib'
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBPNG_LIB_DIRECTORY'/'$LIBPNG_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBTIFF_LIB_DIRECTORY'/'$LIBTIFF_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBJPEG_LIB_DIRECTORY'/'$LIBJPEG_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBZSTD_LIB_DIRECTORY'/'$LIBZSTD_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBLZMA_LIB_DIRECTORY'/'$LIBLZMA_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBICONV_LIB_DIRECTORY'/'$LIBICONV_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBPCRE2_LIB_DIRECTORY'/'$LIBPCRE2_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_COCOAU_XRC_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_COCOAU_HTML_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_COCOAU_QA_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_COCOAU_CORE_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_BASEU_XML_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_BASEU_NET_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$WXWIDGETS_LIB_DIRECTORY'/'$LIBWX_BASEU_NAME
LIST_LIBRARIES_TO_COPY=$LIST_LIBRARIES_TO_COPY' '$LIBGSL_LIB_DIRECTORY'/'$LIBGSL_NAME
#copy the list of libraries
cp $LIST_LIBRARIES_TO_COPY $APP_LIBRARY_DIRECTORY

#link the app execytable  to libraries contained in the application bundle, so the app is self-sufficient
install_name_tool -add_rpath @executable_path/../Resources/Libraries/ $APP_NAME
link_list "$APP_NAME" "$LIBGSL_NAME $LIBWX_BASEU_NET_NAME $LIBWX_BASEU_XML_NAME $LIBWX_COCOAU_CORE_NAME $LIBWX_COCOAU_HTML_NAME $LIBWX_COCOAU_QA_NAME $LIBWX_COCOAU_XRC_NAME $LIBWX_BASEU_NAME"
#link libraries to libraries contained in the application bundle, so the app is self-sufficient
link_list "$APP_LIBRARY_DIRECTORY/$LIBPNG_NAME" "$LIBWX_COCOAU_CORE_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_COCOAU_XRC_NAME" "$LIBWX_COCOAU_CORE_NAME $LIBWX_BASEU_XML_NAME $LIBWX_BASEU_NAME $LIBWX_COCOAU_HTML_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_COCOAU_HTML_NAME" "$LIBWX_COCOAU_CORE_NAME $LIBWX_BASEU_XML_NAME $LIBWX_BASEU_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_BASEU_NAME" "$LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_COCOAU_QA_NAME" "$LIBWX_COCOAU_CORE_NAME $LIBWX_BASEU_XML_NAME $LIBWX_BASEU_NAME $LIBWX_COCOAU_HTML_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_COCOAU_CORE_NAME" "$LIBWX_BASEU_XML_NAME $LIBWX_BASEU_NAME $LIBWX_COCOAU_HTML_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_COCOAU_HTML_NAME" "$LIBWX_COCOAU_CORE_NAME $LIBWX_BASEU_XML_NAME $LIBWX_BASEU_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_BASEU_XML_NAME" "$LIBWX_COCOAU_CORE_NAME  $LIBWX_BASEU_NAME $LIBWX_COCOAU_HTML_NAME $LIBJPEG_NAME $LIBPCRE2_NAME $LIBTIFF_NAME $LIBCURL_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBWX_BASEU_NET_NAME" "$LIBWX_COCOAU_CORE_NAME $LIBWX_BASEU_NAME $LIBJPEG_NAME $LIBCURL_NAME $LIBPCRE2_NAME"
link_list "$APP_LIBRARY_DIRECTORY/$LIBTIFF_NAME" "$LIBZSTD_NAME $LIBLZMA_NAME $LIBJPEG_NAME"


#create the .app folder and subfolders
sed -e "s/\${MACOSX_BUNDLE_GUI_IDENTIFIER}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_EXECUTABLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_NAME}/$APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_COPYRIGHT}/Copyright CNRS/" \
    -e "s/\${MACOSX_BUNDLE_BUNDLE_VERSION}/$APP_VERSION/" \
    -e "s/\${MACOSX_BUNDLE_INFO_STRING}/$APP_NAME version $APP_VERSION, (c) 2024 $APP_NAME/" \
    -e "s/\${MACOSX_BUNDLE_LONG_VERSION_STRING}/$APP_VERSION/" \
    -e "s/\${MACOSX_BUNDLE_SHORT_VERSION_STRING}/$APP_VERSION/" \
    -e "s/\${MACOSX_MINIMUM_SYSTEM_VERSION}/$MINIMUM_SYSTEM_VERSION/" \
    -e "s/\${MACOSX_ICON_NAME}/$ICON_NAME/" \
    $INPUT_PATH/Info.plist.in > $OUTPUT_PATH/$APP_NAME.app/Contents/Info.plist
/bin/echo "APPL????" >$OUTPUT_PATH/$APP_NAME.app/Contents/PkgInfo
ln -f $APP_NAME $OUTPUT_PATH/$APP_NAME.app/Contents/MacOS/$APP_NAME
cp -f $ICON_PATH $OUTPUT_PATH/$APP_NAME.app/Contents/Resources

#clean up
rm -rf $APP_NAME
