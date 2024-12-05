#!/bin/sh
:'
this script builds gsl for both architectures x86_64 and arm64
run with  
sudo ./build_gsl.sh [path where you decompressed the GSL .tar.gz file] [temporary path where to store the libarries, it will be deleted at the end]
example:
sudo ./build_gsl.sh /Applications/gsl-2.8 /Users/michelecastellana/Desktop
'


GSL_DIR=$1
TEMP_DIR=$2

echo "Directory where the GSL zip file has been expanded = " $GSL_DIR
echo "Temp directory where the GSL zip file has been expanded = " $TEMP_DIR


rm -rf $TEMP_DIR
mkdir -p $TEMP_DIR"/arm64" $TEMP_DIR"/x86_64" $TEMP_DIR"/universal"
cd $GSL_DIR


echo "Building for x86_64 architecture ..."

make clean
./configure CC="gcc  -arch x86_64" CXX="g++  -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E" --prefix=$TEMP_DIR/"x86_64"
make 
make install
rm -rf $TEMP_DIR/x86_64/lib/pkgconfig

echo "... done"


echo "Building for arm64 architecture ..."

make clean
./configure CC="gcc  -arch arm64" CXX="g++  -arch arm64" CPP="gcc -E" CXXCPP="g++ -E" --prefix=$TEMP_DIR/"arm64"
make 
make install
rm -r $TEMP_DIR/arm64/lib/pkgconfig

echo "... done"


echo "Merging files with architecture x86_64 and arm64 ... "

for dylib in $TEMP_DIR/arm64/lib/*; do 
  lipo -create -arch arm64 $dylib -arch x86_64 $TEMP_DIR/x86_64/lib/$(basename $dylib) -output $TEMP_DIR/universal/$(basename $dylib); 
  echo $(basename $dylib)
done

echo "... done"


echo "Here are the architectures of the merged files :"

for dylib in $TEMP_DIR/universal/*; do
  lipo $dylib -info;
done


mv $TEMP_DIR/universal/* /usr/local/lib
rm -rf $TEMP_DIR