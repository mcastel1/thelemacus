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

//#include <boost/algorithm/string.hpp>
//#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll.hpp>

/*
 compile on mac with
 
 g++ wrapper.cpp -o wrapper.o -I/usr/local/include/  -Wall -Wno-c++11-extensions --std=c++17  -O3 -L/usr/local/bin  -lboost_filesystem -lboost_system

 */


using namespace std;



int main(int argc, const char * argv[]) {
    
    ifstream infile;
    ofstream outfile;
    stringstream run_directory, library_directory, command, ins;
    string dummy;
    
    cout << "Program location = " << boost::dll::program_location().parent_path() << "\n";
    
    ins.str("");
    ins << (boost::dll::program_location().parent_path());
    
    //remove " from the path obtained from program_location, write it into run_directory and add a / at the end
    dummy = ins.str().c_str();
    dummy.erase(std::remove(dummy.begin(), dummy.end(), '"'), dummy.end());
    run_directory.str("");
    run_directory << dummy << "/";
    
    
    
 
    //properly format run_diretory and library_directory
    dummy = run_directory.str().c_str();
//    run_directory.str("");
    library_directory.str("");
//    run_directory << "\"" << dummy << "\"";
    library_directory << "\"" << dummy << "../Resources/Libraries/\"";
    
    //export the libraries so the app knows where to find them later
    command.str("");
    command << "export DYLD_LIBRARY_PATH=\$DYLD_LIBRARY_PATH:"
    << library_directory.str().c_str()
    << "; echo \$DYLD_LIBRARY_PATH"
    << "; \"" << run_directory.str().c_str() << "./main.o\"";

    
    cout << "Run directory = " << run_directory.str().c_str() << "\n";
    cout << "Library directory = " << library_directory.str().c_str() << "\n";
    
    
    cout << "Command  = " << command.str().c_str() << "\n";

    system(command.str().c_str());

    
    return 0;
    
}
