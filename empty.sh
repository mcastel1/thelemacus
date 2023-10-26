#!/bin/sh

SAVE_PATH=/Users/macbookpro/saved

mv /usr/local/lib/libwx* /usr/local/lib/libtiff.5.dylib /usr/local/lib/libgsl* /usr/local/lib/libjpeg.9.dylib /usr/local/lib/libpcre2-32.0.dylib /usr/local/lib/libpng16.16.dylib  $SAVE_PATH
mv /usr/local/opt/boost/lib/libboost_*.dylib  $SAVE_PATH
mv /usr/local/opt/libpng/lib/libpng16.16.dylib $SAVE_PATH
mv /usr/local/opt/xz/lib/liblzma.5.dylib $SAVE_PATH
#mv /usr/local/lib/libpng16.16.dylib $SAVE_PATH
