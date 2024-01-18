#!/bin/sh

WXWIDGETS_LIB_DIRECTORY='/Applications/wxWidgets-3.2.4/build-cocoa-debug/lib'
BOOST_LIB_DIRECTORY='/Applications/boost_1_83_0/stage/lib'
LIBPNG_LIB_DIRECTORY='/usr/local/lib'
LIBJPEG_LIB_DIRECTORY='/opt/homebrew/opt/jpeg/lib'
LIBPCRE2_LIB_DIRECTORY='/opt/homebrew/opt/pcre2/lib'
LIBTIFF_LIB_DIRECTORY='/opt/homebrew/opt/libtiff/lib'
LIBICONV_LIB_DIRECTORY='/usr/local/lib'
LIBCURL_LIB_DIRECTORY='/opt/homebrew/Cellar/curl/8.5.0/lib'
LIBZ_LIB_DIRECTORY='/opt/homebrew/Cellar/zlib/1.3/lib'
LIBGSL_LIB_DIRECTORY='/usr/local/lib'

LIST_LIBRARIES_TO_MOVE=''
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$BOOST_LIB_DIRECTORY/libboost_filesystem.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$BOOST_LIB_DIRECTORY/libboost_system.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$BOOST_LIB_DIRECTORY/libboost_atomic.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$LIBPNG_LIB_DIRECTORY/libpng16.16.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$LIBZ_LIB_DIRECTORY/libz.1.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_xrc-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_html-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_qa-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_osx_cocoau_core-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu_xml-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu_net-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$WXWIDGETS_LIB_DIRECTORY/libwx_baseu-3.2.0.2.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$LIBICONV_LIB_DIRECTORY/libiconv.2.dylib
LIST_LIBRARIES_TO_MOVE=$LIST_LIBRARIES_TO_MOVE' '$LIBGSL_LIB_DIRECTORY/libgsl.27.dylib

SAVE_PATH=/Users/michele/saved

sudo mv $LIST_LIBRARIES_TO_MOVE $SAVE_PATH
