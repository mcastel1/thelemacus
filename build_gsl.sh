#!/bin/sh


GSL_DIR=$1
TEMP_DIR=$2

echo "Directory where the GSL zip file has been expanded = " $GSL_DIR
echo "Temp directory where the GSL zip file has been expanded = " $TEMP_DIR


# rm -rf $TEMP_DIR"/arm64" $TEMP_DIR"/x86_64" $TEMP_DIR"/universal"
# mkdir $TEMP_DIR"/arm64" $TEMP_DIR"/x86_64" $TEMP_DIR"/universal"
# cd $GSL_DIR


# echo "Building for x86_64 architecture ..."

# make clean
# ./configure CC="gcc  -arch x86_64" CXX="g++  -arch x86_64" CPP="gcc -E" CXXCPP="g++ -E" --prefix=$TEMP_DIR/"x86_64"
# make 
# make install

# echo "... done"


# echo "Building for arm64 architecture ..."

# make clean
# ./configure CC="gcc  -arch arm64" CXX="g++  -arch arm64" CPP="gcc -E" CXXCPP="g++ -E" --prefix=$TEMP_DIR/"arm64"
# make 
# make install

# echo "... done"

for dylib in $TEMP_DIR/*; do 
  lipo -create -arch arm64 arm64/$(basename $dylib) -arch x86_64 x86_64/$(basename $dylib) -output $TEMP_DIR/universal/(basename $dylib); 
done
for dylib in universal/*; do
  lipo $dylib -info;
done