#!/bin/bash

APP_NAME=Thelemacus
INPUT_CODE_DIR=/Users/macbookpro/Documents/sight_reduction_program/autoconf
INPUT_DATA_DIR=$INPUT_CODE_DIR/data
INPUT_IMAGE_DIR=$INPUT_CODE_DIR/images
INPUT_FILE_INIT=$INPUT_DATA_DIR/init.txt
OUTPUT_CODE_DIR=/Users/macbookpro/Desktop
OUTPUT_DATA_DIR=$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources/Data
OUTPUT_IMAGE_DIR=$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources/Images
OUTPUT_FILE_INIT=${OUTPUT_DATA_DIR}/init.txt
WX_DIR=/Applications/wxWidgets-3.2.2.1



clear; clear;
rm -rf temp.o $OUTPUT_CODE_DIR/$APP_NAME.app $OUTPUT_CODE_DIR/$APP_NAME
mkdir -p $OUTPUT_CODE_DIR/$APP_NAME.app/Contents
mkdir -p $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/MacOS
mkdir -p $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources
mkdir -p ${OUTPUT_DATA_DIR}
mkdir -p ${OUTPUT_IMAGE_DIR}

sed -e 's#'$INPUT_DATA_DIR'#'$OUTPUT_DATA_DIR'#' \
    -e 's#'$INPUT_IMAGE_DIR'#'$OUTPUT_IMAGE_DIR'#' \
    -e 's#'$INPUT_CODE_DIR'#'$OUTPUT_CODE_DIR/$APP_NAME.app'#' \
    $INPUT_FILE_INIT >$OUTPUT_FILE_INIT


sed -e 's/${MACOSX_BUNDLE_GUI_IDENTIFIER}/org.wxwidgets.'$APP_NAME'/' \
-e 's/${MACOSX_BUNDLE_EXECUTABLE_NAME}/'$APP_NAME'/' \
-e 's/${MACOSX_BUNDLE_BUNDLE_NAME}/'$APP_NAME'/' \
-e 's/${MACOSX_BUNDLE_COPYRIGHT}/Copyright 2002-2022 wxWidgets/' \
-e 's/${MACOSX_BUNDLE_BUNDLE_VERSION}/3.2.0/' \
-e 's/${MACOSX_BUNDLE_INFO_STRING}/'$APP_NAME' version 3.2.0, (c) 2002-2022 wxWidgets/' \
-e 's/${MACOSX_BUNDLE_LONG_VERSION_STRING}/3.2.0, (c) 2002-2022 wxWidgets/' \
-e 's/${MACOSX_BUNDLE_SHORT_VERSION_STRING}/3.2/' \
${WX_DIR}/src/osx/carbon/Info.plist.in >$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Info.plist

#/bin/echo "APPL????" >$OUTPUT_CODE_DIR/$APP_NAME.app/Contents/PkgInfo
#ln -f main $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/MacOS/$APP_NAME
#cp -f ${INPUT_IMAGE_DIR}/jolly_roger.icns $OUTPUT_CODE_DIR/$APP_NAME.app/Contents/Resources/wxmac.icns
#
##copy the  files
#cp $INPUT_CODE_DIR/sample_sight.nav $OUTPUT_CODE_DIR/$APP_NAME.app/sample_sight.nav
#cp ${INPUT_IMAGE_DIR}/*.png ${OUTPUT_IMAGE_DIR}
#cp $INPUT_DATA_DIR/* ${OUTPUT_DATA_DIR}
#cp $INPUT_DATA_DIR/map_conv_blocked.csv $INPUT_DATA_DIR/n_line_map_conv_blocked.txt ${OUTPUT_DATA_DIR}
#
#
#rm -rf $OUTPUT_CODE_DIR/temp.o $OUTPUT_CODE_DIR/$APP_NAME
#rm -rf $OUTPUT_CODE_DIR/$APP_NAME.app/main.cpp
