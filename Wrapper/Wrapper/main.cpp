#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <fstream>
#include <strstream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <list>
#include <stdio.h>

using namespace std;

int main(int argc, const char * argv[]) {
    
    // insert code here...
    ifstream infile;
    string running_directory;
    
    system("rm -rf path.dat; pwd >> path.dat;");
    infile.open("path.dat");
    running_directory.clear();
    getline(infile, running_directory);
    infile.close();
    system("rm -rf path.dat");
    
    cout << "Running directory = " << running_directory << "\n";
    
    
    system("export DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH:/Users/macbookpro/Desktop/Thelemacus.app/Contents/Resources/Libraries/; echo \$DYLD_LIBRARY_PATH");

    std::cout << "Hello, World!\n";
    return 0;
    
}
