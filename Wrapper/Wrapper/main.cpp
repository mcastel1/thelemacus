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
    stringstream library_directory, command;
    string running_directory;
    
    //run pwd and get the current running path of this .o -> write it into path.dat so the app can read it later
    system("rm -rf path.dat; pwd >> path.dat;");
    infile.open("path.dat");
    running_directory.clear();
    getline(infile, running_directory);
    infile.close();
//    system("rm -rf path.dat");
    
    cout << "Running directory = " << running_directory << "\n";
    
    library_directory.clear();
    library_directory << running_directory << "/../Resources/Libraries/";

    cout << "Library directory = " << library_directory.str().c_str() << "\n";

    //export the libraries so the app knows where to find them later
    command.clear();
    command << "export DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH:"
            << library_directory.str().c_str()
            << "; echo \$DYLD_LIBRARY_PATH";
//            << "; mv path.dat " << running_directory;
    
    cout << "Command  = " << command.str().c_str() << "\n";
    

    
    system(command.str().c_str());

    return 0;
    
}
