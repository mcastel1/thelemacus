//
//  main.cpp
//  test_rpath
//
//  Created by MacBook Pro on 22/10/2023.
//


/*
 This is the procedure to link with rpath to a library located in [path_of the executable]/lib/
 
 g++ main.cpp -o main.o -lwx_osx_cocoau_xrc-3.1 -L/Users/macbookpro/Desktop/example/
 install_name_tool -add_rpath @executable_path/../lib main.o
 install_name_tool -change /usr/local/lib/libwx_osx_cocoau_xrc-3.1.dylib @rpath/libwx_osx_cocoau_xrc-3.1.dylib main.o

 */

#include <iostream>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
