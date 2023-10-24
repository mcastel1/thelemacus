#!/bin/sh

SAVE_PATH=/Users/macbookpro/saved

mv $SAVE_PATH/libwx* $SAVE_PATH/libtiff.5.dylib $SAVE_PATH/libgsl* $SAVE_PATH/libjpeg.9.dylib $SAVE_PATH/libpcre2-32.0.dylib $SAVE_PATH/libpng16.16.dylib  /usr/local/lib/
mv $SAVE_PATH/libboost_filesystem.dylib $SAVE_PATH/libboost_system.dylib /usr/local/opt/boost/lib/
#cp $SAVE_PATH/libpng16.16.dylib /usr/local/opt/libpng/lib/
mv $SAVE_PATH/libpng16.16.dylib /usr/local/lib/
cp  /usr/local/lib/libpng16.16.dylib /usr/local/opt/libpng/lib/
